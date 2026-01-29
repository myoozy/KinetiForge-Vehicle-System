// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)


#include "VehicleDriveAssemblyComponent.h"
#include "PhysicsEngine/BodyInstance.h"
#include "VehicleWheelComponent.h"
#include "VehicleDifferentialComponent.h"
#include "VehicleAxleAssemblyComponent.h"
#include "VehicleWheelCoordinatorComponent.h"
#include "VehicleEngineComponent.h"
#include "VehicleClutchComponent.h"
#include "VehicleGearboxComponent.h"
#include "VehicleAsyncTickComponent.h"
#include "VehicleUtil.h"
#include "AsyncTickFunctions.h"
#include "Net/UnrealNetwork.h" 
#include "GameFramework/SpringArmComponent.h"

// Sets default values for this component's properties
UVehicleDriveAssemblyComponent::UVehicleDriveAssemblyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PostPhysics;

	// ...
	SetIsReplicatedByDefault(true); // enable replication

	//load default curves
	if (!InputConfig.Steering.ResponseCurve)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> DefaultSteeringCurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultSteeringCurve.DefaultSteeringCurve'")
		);
		if (DefaultSteeringCurveObj.Succeeded())
		{
			InputConfig.Steering.ResponseCurve = DefaultSteeringCurveObj.Object;
		}
	}

	if (!InputConfig.HighSpeedSteeringScale)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> DefaultSteeringSpeedScaleCurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultHighspeedSteeringCurve.DefaultHighspeedSteeringCurve'")
		);
		if (DefaultSteeringSpeedScaleCurveObj.Succeeded())
		{
			InputConfig.HighSpeedSteeringScale = DefaultSteeringSpeedScaleCurveObj.Object;
		}
	}

	if (!AutoGearboxConfig.AutoGearboxShiftFactorCurve)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> DefaultAutoGearboxInputCurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultAutoGearboxCurve.DefaultAutoGearboxCurve'")
		);
		if (DefaultAutoGearboxInputCurveObj.Succeeded())
		{
			AutoGearboxConfig.AutoGearboxShiftFactorCurve = DefaultAutoGearboxInputCurveObj.Object;
		}
	}
}


// Called when the game starts
void UVehicleDriveAssemblyComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	WheelCoordinator = UVehicleWheelCoordinatorComponent::FindWheelCoordinator(Carbody.Get());
	if (WheelCoordinator.IsValid())WheelCoordinator->RegisterDriveAssembly(this);

	VehicleAsyncTickComponent = UVehicleAsyncTickComponent::FindVehicleAsyncTickComponent(GetOwner());
	if (VehicleAsyncTickComponent.IsValid())VehicleAsyncTickComponent->Register(this);

	SearchExistingAxles();
	GeneratePowerUnit();

	//initialize autogearbox timer
	AutoGearboxCount = FMath::FRandRange(-AutoGearboxConfig.AutomaticGearboxRefreshTime, 0.0f) + AutoGearboxConfig.AutomaticGearboxRefreshTime;

}

void UVehicleDriveAssemblyComponent::OnRegister()
{
	Super::OnRegister();
	//...
	Carbody = UVehicleWheelCoordinatorComponent::FindPhysicalParent(this);
	GetOwner()->bRunConstructionScriptOnDrag = false;	//to improve performance
	GetOwner()->SetReplicates(true);
	GenerateAxles();
}

#if WITH_EDITOR
void UVehicleDriveAssemblyComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	//...
}
#endif

void UVehicleDriveAssemblyComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	//destory all axles
	if (Axles.Num())
	{
		for (TWeakObjectPtr<UVehicleAxleAssemblyComponent> AxleToDestroy : Axles)
		{
			if (AxleToDestroy.IsValid() && !AxleToDestroy->IsBeingDestroyed())AxleToDestroy->DestroyComponent();
		}
		Axles.Empty();
	}

	if (Engine.IsValid() && !Engine->IsBeingDestroyed())Engine->DestroyComponent();

	if (Clutch.IsValid() && !Clutch->IsBeingDestroyed())Clutch->DestroyComponent();

	if (Gearbox.IsValid() && !Gearbox->IsBeingDestroyed())Gearbox->DestroyComponent();

	if (TransferCase.IsValid() && !TransferCase->IsBeingDestroyed())TransferCase->DestroyComponent();

	Carbody = nullptr;

	InputConfig.Throttle.ResponseCurve = nullptr;
	InputConfig.Brake.ResponseCurve = nullptr;
	InputConfig.Clutch.ResponseCurve = nullptr;
	InputConfig.Handbrake.ResponseCurve = nullptr;
	InputConfig.Steering.ResponseCurve = nullptr;
	InputConfig.HighSpeedSteeringScale = nullptr;

	//...
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UVehicleDriveAssemblyComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UVehicleDriveAssemblyComponent, ServerCurrentGear);
}

void UVehicleDriveAssemblyComponent::UpdateInput(float InDeltaTime)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UpdateVehicleInput);

	UpdateThrottle(InDeltaTime);
	UpdateBrake(InDeltaTime);
	UpdateClutch(InDeltaTime);
	UpdateSteering(InDeltaTime);
}

void UVehicleDriveAssemblyComponent::UpdateThrottle(float InDeltaTime)
{
	float RealThrottleInput = InputValues.bSwitchThrottleAndBrake ? InputValues.Raw.Brake : InputValues.Raw.Throttle;

	if (Gearbox.IsValid())
	{
		//check if there's no need to edit throttle value
		if (Gearbox->GetIsInGear() || !InputAssistConfig.bAutomaticClutch)
		{
			InputValues.Smoothened.Throttle = FVehicleInputAxisConfig::InterpInputValueConstant(InputValues.Smoothened.Throttle, RealThrottleInput, InDeltaTime, InputConfig.Throttle.InterpSpeed);

			InputValues.Smoothened.Throttle *= !(InputAssistConfig.bEVClutchLogic && Gearbox->GetCurrentGearRatio() == 0.f);
		}
		//if not in gear and should rev-match
		else if (Gearbox->GetShouldRevMatch() && FMath::Abs(LocalLinearVelocity.X) > 0.5 && InputAssistConfig.bRevMatching)
		{
			const float Rate = 5.f;
			InputValues.Smoothened.Throttle += VehicleUtil::SafeDivide(InDeltaTime * Rate, Gearbox->Config.ShiftDelay);
			InputValues.Smoothened.Throttle = FMath::Min(InputValues.Smoothened.Throttle, InputAssistConfig.RevMatchMaxThrottle);
		}
		//if not in gear and no rev-matching
		else
		{
			InputValues.Smoothened.Throttle = 0.f;
		}
	}
	else
	{
		InputValues.Smoothened.Throttle = FVehicleInputAxisConfig::InterpInputValueConstant(InputValues.Smoothened.Throttle, RealThrottleInput, InDeltaTime, InputConfig.Throttle.InterpSpeed);
	}

	if (IsValid(InputConfig.Throttle.ResponseCurve))
	{
		InputValues.Final.Throttle = InputConfig.Throttle.ResponseCurve->GetFloatValue(InputValues.Smoothened.Throttle);
	}
	else
	{
		InputValues.Final.Throttle = InputValues.Smoothened.Throttle;
	}
}

void UVehicleDriveAssemblyComponent::UpdateBrake(float InDeltaTime)
{
	float RealThrottleInput = InputValues.bSwitchThrottleAndBrake ? InputValues.Raw.Brake : InputValues.Raw.Throttle;
	float RealBrakeInput = InputValues.bSwitchThrottleAndBrake ? InputValues.Raw.Throttle : InputValues.Raw.Brake;

	//update brake
	InputValues.Smoothened.Brake = FVehicleInputAxisConfig::InterpInputValueConstant(InputValues.Smoothened.Brake, RealBrakeInput, InDeltaTime, InputConfig.Brake.InterpSpeed);

	if (IsValid(InputConfig.Brake.ResponseCurve))
	{
		InputValues.Final.Brake = InputConfig.Brake.ResponseCurve->GetFloatValue(InputValues.Smoothened.Brake);
	}
	else
	{
		InputValues.Final.Brake = InputValues.Smoothened.Brake;
	}

	//update handbrake
	float HandbrakeTarget = InputValues.Raw.Handbrake;
	if (InputAssistConfig.bAutoHold
		&& FMath::Abs(LocalLinearVelocity.X) < 0.1
		&& RealThrottleInput < SMALL_NUMBER
		&& !bIsInAir)HandbrakeTarget = 1.f;
	InputValues.Smoothened.Handbrake = FVehicleInputAxisConfig::InterpInputValueConstant(InputValues.Smoothened.Handbrake, HandbrakeTarget, InDeltaTime, InputConfig.Handbrake.InterpSpeed);

	if (IsValid(InputConfig.Handbrake.ResponseCurve))
	{
		InputValues.Final.Handbrake = InputConfig.Handbrake.ResponseCurve->GetFloatValue(InputValues.Smoothened.Handbrake);
	}
	else
	{
		InputValues.Final.Handbrake = InputValues.Smoothened.Handbrake;
	}
}

void UVehicleDriveAssemblyComponent::UpdateClutch(float InDeltaTime)
{
	if (InputAssistConfig.bEVClutchLogic)
	{
		InputValues.Smoothened.Clutch = 0.f;
	}
	else if (InputAssistConfig.bAutomaticClutch && Gearbox.IsValid() && Engine.IsValid())
	{
		//check if clutch has to be engaged
		bool bNotInGearAndNotSequential = !(Gearbox->GetIsInGear() || Gearbox->Config.bSequentialGearbox);
		float TargetClutchValue = (InputValues.Raw.Handbrake > 0.9 || bNotInGearAndNotSequential) ? 1.f : InputValues.Raw.Clutch;

		//take engine rpm into account
		float Bias = FMath::GetMappedRangeValueClamped(InputAssistConfig.AutoClutchRange, FVector2f(1, 0), Engine->GetRPM());
		TargetClutchValue = FMath::Clamp(TargetClutchValue + Bias, 0.f, 1.f);
		InputValues.Smoothened.Clutch = FMath::Min(InputValues.Smoothened.Clutch + Bias, TargetClutchValue);

		//get interp speed, when changing gear, clutch should engage faster than gear changes
		FVector2f FinalInterpSpeed;
		float Rate = 10.f;
		FinalInterpSpeed.Y = InputConfig.Clutch.InterpSpeed.Y;
		FinalInterpSpeed.X = bNotInGearAndNotSequential ? VehicleUtil::SafeDivide(Rate, Gearbox->Config.ShiftDelay) : InputConfig.Clutch.InterpSpeed.X;

		InputValues.Smoothened.Clutch = FVehicleInputAxisConfig::InterpInputValueConstant(InputValues.Smoothened.Clutch, TargetClutchValue, InDeltaTime, FinalInterpSpeed);
	}
	else
	{
		InputValues.Smoothened.Clutch = FVehicleInputAxisConfig::InterpInputValueConstant(InputValues.Smoothened.Clutch, InputValues.Raw.Clutch, InDeltaTime, InputConfig.Clutch.InterpSpeed);
	}

	if (IsValid(InputConfig.Clutch.ResponseCurve))
	{
		InputValues.Final.Clutch = InputConfig.Clutch.ResponseCurve->GetFloatValue(InputValues.Smoothened.Clutch);
	}
	else
	{
		InputValues.Final.Clutch = InputValues.Smoothened.Clutch;
	}
}

void UVehicleDriveAssemblyComponent::UpdateSteering(float InDeltaTime)
{
	float RealSteeringInput = InputValues.Raw.Steering;

	//reduce the steering value when driving at high speed
	if (InputConfig.HighSpeedSteeringScale)
	{
		RealSteeringInput *= InputConfig.HighSpeedSteeringScale->GetFloatValue(FMath::Abs(LocalLinearVelocity.X));
	}

	InputValues.Smoothened.Steering = FVehicleInputAxisConfig::InterpInputValueConstant(InputValues.Smoothened.Steering, RealSteeringInput, InDeltaTime, InputConfig.Steering.InterpSpeed);

	if (IsValid(InputConfig.Steering.ResponseCurve))
	{
		float CurveValue = InputConfig.Steering.ResponseCurve->GetFloatValue(FMath::Abs(InputValues.Smoothened.Steering));
		InputValues.Final.Steering = CurveValue * FMath::Sign(InputValues.Smoothened.Steering);
	}
	else
	{
		InputValues.Final.Steering = InputValues.Smoothened.Steering;
	}
}

void UVehicleDriveAssemblyComponent::UpdateAutomaticGearbox(float InDeltaTime)
{	
	APawn* p = Cast<APawn>(GetOwner());
	if (!p->IsLocallyControlled())return;

	AutoGearboxCount += InDeltaTime;
	//reset timer
	//保留误差
	bool AutoGearboxTimerOverFlowed = AutoGearboxCount > 0.f;
	AutoGearboxCount -= AutoGearboxConfig.AutomaticGearboxRefreshTime * AutoGearboxTimerOverFlowed;

	if (bIsInAir || 
		!Gearbox.IsValid() ||
		!Engine.IsValid() ||
		!Gearbox->GetIsInGear() ||
		!AutoGearboxTimerOverFlowed ||
		(!Gearbox->Config.NumberOfGears && !Gearbox->Config.NumOfReverseGears)
		)return;

	float LinearVelocityX = FMath::Abs(LocalLinearVelocity.X);

	//arcade gearbox logic; automatically shift to N / D1 / R1 gear
	if (AutoGearboxConfig.bArcadeAutoGearbox)
	{
		//if in N gear
		if (Gearbox->GetCurrentGear() == 0)
		{
			if (InputValues.Raw.Throttle > SMALL_NUMBER)
			{
				ShiftToTargetGear(1, 0.f, AutoGearboxConfig.bArcadeShiftInstant);
				AutoGearboxCount += AutoGearboxConfig.AutomaticGearboxRefreshTime * AutoGearboxConfig.bArcadeShiftInstant;
				return;
			}
			else if (InputValues.Raw.Brake > SMALL_NUMBER && InputValues.Raw.Throttle <= SMALL_NUMBER)
			{
				ShiftToTargetGear(-1, 0.f, AutoGearboxConfig.bArcadeShiftInstant);
				AutoGearboxCount += AutoGearboxConfig.AutomaticGearboxRefreshTime * AutoGearboxConfig.bArcadeShiftInstant;
				return;
			}
		}
		//if the vehicle is almost stopped, or going in wrong direction
		else if (LinearVelocityX < 1 || (LocalLinearVelocity.X * Gearbox->GetCurrentGear()) < 0)
		{
			if (InputValues.Raw.Throttle > SMALL_NUMBER)
			{
				ShiftToTargetGear(1, 0.f, AutoGearboxConfig.bArcadeShiftInstant);
				AutoGearboxCount += AutoGearboxConfig.AutomaticGearboxRefreshTime * AutoGearboxConfig.bArcadeShiftInstant;
				return;
			}
			else if (InputValues.Raw.Brake > SMALL_NUMBER)
			{
				ShiftToTargetGear(-1, 0.f, AutoGearboxConfig.bArcadeShiftInstant);
				AutoGearboxCount += AutoGearboxConfig.AutomaticGearboxRefreshTime * AutoGearboxConfig.bArcadeShiftInstant;
				return;
			}
			else
			{
				ShiftToTargetGear(0, 0.f, AutoGearboxConfig.bArcadeShiftInstant);
				AutoGearboxCount += AutoGearboxConfig.AutomaticGearboxRefreshTime * AutoGearboxConfig.bArcadeShiftInstant;
				return;
			}
		}
	}

	//if in N gear, don't update
	if (!Gearbox->GetCurrentGear())return;

	TArray<UVehicleAxleAssemblyComponent*> AxlesRaw;
	GetAxles(AxlesRaw);
	Gearbox->CalculateSpeedRangeOfEachGear(
		TransferCase->CalculateEffectiveWheelRadius(AxlesRaw),
		Engine->NAConfig.EngineIdleRPM,
		Engine->NAConfig.EngineMaxRPM,
		SpeedRangeOfEachGear);

	//如果急刹车时，也希望尽早降档，发动机协助制动
	float Input = FMath::Max(InputValues.Final.Throttle, InputValues.Final.Brake);

	float ShiftFactor = AutoGearboxConfig.AutoGearboxShiftFactor;
	//in sport mode, shift as late as it can
	if (!AutoGearboxConfig.bSportMode)
	{
		if (AutoGearboxConfig.AutoGearboxShiftFactorCurve)
		{
			ShiftFactor *= AutoGearboxConfig.AutoGearboxShiftFactorCurve->GetFloatValue(Input);
		}
		else
		{
			ShiftFactor *= Input;
		}
	}

	// should shift down if rpm is too low
	float MinShiftUpFactor = VehicleUtil::SafeDivide(Engine->NAConfig.EngineIdleRPM, Engine->NAConfig.EngineMaxRPM);
	ShiftFactor = FMath::Max(MinShiftUpFactor, ShiftFactor);

	int32 UnsignedCurrentGear = FMath::Abs(Gearbox->GetCurrentGear());
	int32 StartGear = FMath::Max(UnsignedCurrentGear - AutoGearboxConfig.MaxDownShiftSteps, 1);
	int32 EndGear = FMath::Min(UnsignedCurrentGear + AutoGearboxConfig.MaxUpShiftSteps,
		FMath::Max(Gearbox->Config.NumberOfGears, Gearbox->Config.NumOfReverseGears));
	
	int32 TargetGear = StartGear;
	float UnsignedSpeed = FMath::Abs(LocalLinearVelocity.X);

	//search for target gear
	for (int i = EndGear; i > StartGear; i--)
	{
		if (UnsignedSpeed > ShiftFactor * SpeedRangeOfEachGear[i - 1].Y)
		{
			TargetGear = i;
			break;
		}
	}

	bool GearingUp = FMath::Abs(Gearbox->GetCurrentGear()) < TargetGear;

	//check sign
	if (LocalLinearVelocity.X < 0)TargetGear = -TargetGear;

	if (Gearbox->GetCurrentGear() != TargetGear
		&& TargetGear * Gearbox->GetCurrentGear() > 0
		&& !(GearingUp && InputValues.Final.Brake > SMALL_NUMBER))
	{
		ShiftToTargetGear(TargetGear, 0.f);
		AutoGearboxCount -= AutoGearboxConfig.AutoShiftCoolDown;
	}
}

//RPC
void UVehicleDriveAssemblyComponent::ServerInputThrottle_Implementation(float InValue, bool bDirectInput)
{
	InputValues.Raw.Throttle = InValue;
	if (bDirectInput)
	{
		InputValues.Smoothened.Throttle = InValue;
		InputValues.Final.Throttle = InValue;
	}
	MultiCastInputThrottle(InValue, bDirectInput);
}

void UVehicleDriveAssemblyComponent::MultiCastInputThrottle_Implementation(float InValue, bool bDirectInput)
{
	APawn* p = Cast<APawn>(GetOwner());
	if (p && !p->IsLocallyControlled() && !p->HasAuthority())
	{
		InputValues.Raw.Throttle = InValue;
		if (bDirectInput)
		{
			InputValues.Smoothened.Throttle = InValue;
			InputValues.Final.Throttle = InValue;
		}
	}
}

void UVehicleDriveAssemblyComponent::ServerInputBrake_Implementation(float InValue, bool bDirectInput)
{
	InputValues.Raw.Brake = InValue;
	if (bDirectInput)
	{
		InputValues.Smoothened.Brake = InValue;
		InputValues.Final.Brake = InValue;
	}
	MultiCastInputBrake(InValue, bDirectInput);
}

void UVehicleDriveAssemblyComponent::MultiCastInputBrake_Implementation(float InValue, bool bDirectInput)
{
	APawn* p = Cast<APawn>(GetOwner());
	if (p && !p->IsLocallyControlled() && !p->HasAuthority())
	{
		InputValues.Raw.Brake = InValue;
		if (bDirectInput)
		{
			InputValues.Smoothened.Brake = InValue;
			InputValues.Final.Brake = InValue;
		}
	}
}

void UVehicleDriveAssemblyComponent::ServerInputClutch_Implementation(float InValue, bool bDirectInput)
{
	InputValues.Raw.Clutch = InValue;
	if (bDirectInput)
	{
		InputValues.Smoothened.Clutch = InValue;
		InputValues.Final.Clutch = InValue;
	}
	MultiCastInputClutch(InValue, bDirectInput);
}

void UVehicleDriveAssemblyComponent::MultiCastInputClutch_Implementation(float InValue, bool bDirectInput)
{
	APawn* p = Cast<APawn>(GetOwner());
	if (p && !p->IsLocallyControlled() && !p->HasAuthority())
	{
		InputValues.Raw.Clutch = InValue;
		if (bDirectInput)
		{
			InputValues.Smoothened.Clutch = InValue;
			InputValues.Final.Clutch = InValue;
		}
	}
}

void UVehicleDriveAssemblyComponent::ServerInputSteering_Implementation(float InValue, bool bDirectInput)
{
	InputValues.Raw.Steering = InValue;
	if (bDirectInput)
	{
		InputValues.Smoothened.Steering = InValue;
		InputValues.Final.Steering = InValue;
	}
	MultiCastInputSteering(InValue, bDirectInput);
}

void UVehicleDriveAssemblyComponent::MultiCastInputSteering_Implementation(float InValue, bool bDirectInput)
{
	APawn* p = Cast<APawn>(GetOwner());
	if (p && !p->IsLocallyControlled() && !p->HasAuthority())
	{
		InputValues.Raw.Steering = InValue;
		if (bDirectInput)
		{
			InputValues.Smoothened.Steering = InValue;
			InputValues.Final.Steering = InValue;
		}
	}
}

void UVehicleDriveAssemblyComponent::ServerInputHandbrake_Implementation(float InValue, bool bDirectInput)
{
	InputValues.Raw.Handbrake = InValue;
	if (bDirectInput)
	{
		InputValues.Smoothened.Handbrake = InValue;
		InputValues.Final.Handbrake = InValue;
	}
	MultiCastInputHandbrake(InValue, bDirectInput);
}

void UVehicleDriveAssemblyComponent::MultiCastInputHandbrake_Implementation(float InValue, bool bDirectInput)
{
	APawn* p = Cast<APawn>(GetOwner());
	if (p && !p->IsLocallyControlled() && !p->HasAuthority())
	{
		InputValues.Raw.Handbrake = InValue;
		if (bDirectInput)
		{
			InputValues.Smoothened.Handbrake = InValue;
			InputValues.Final.Handbrake = InValue;
		}
	}
}

void UVehicleDriveAssemblyComponent::ServerShiftToTargetGear_Implementation(int32 InTargetGear, bool bImmediate)
{
	//bind function to delegate
	FOnShiftFinishedDelegate Delegate;
	Delegate.BindUFunction(this, FName("ServerShiftFinishedCallback"));

	//shift to target gear on server
	Gearbox->ShiftToTargetGearWithDelegate(Delegate, InTargetGear, bImmediate);

	//multicast to every client
	MultiCastShiftToTargetGear(InTargetGear, bImmediate);
}

void UVehicleDriveAssemblyComponent::ServerShiftFinishedCallback_Implementation()
{
	//called after shift
	ServerCurrentGear = Gearbox->GetCurrentGear();
}

void UVehicleDriveAssemblyComponent::MultiCastShiftToTargetGear_Implementation(int32 InTargetGear, bool bImmediate)
{
	APawn* p = Cast<APawn>(GetOwner());
	if (!p->IsLocallyControlled() && !p->HasAuthority())
	{
		Gearbox->ShiftToTargetGear(InTargetGear, bImmediate);
	}
}

void UVehicleDriveAssemblyComponent::OnRep_ServerCurrentGear()
{
	//make sure the current gear equals to server current gear
	if (Gearbox->GetCurrentGear() != ServerCurrentGear)
	{
		Gearbox->ShiftToTargetGear(ServerCurrentGear, true);
	}
}

void UVehicleDriveAssemblyComponent::ServerStartVehicleEngine_Implementation()
{
	ServerVehicleEngineOperationMode = Engine->StartVehicleEngine();
	MultiCastStartVehicleEngine();
}

void UVehicleDriveAssemblyComponent::MultiCastStartVehicleEngine_Implementation()
{
	APawn* p = Cast<APawn>(GetOwner());
	if (!p->IsLocallyControlled() && !p->HasAuthority())
	{
		Engine->StartVehicleEngine();
	}
}

void UVehicleDriveAssemblyComponent::ServerShutVehicleEngine_Implementation()
{
	ServerVehicleEngineOperationMode = Engine->ShutVehicleEngine();
	MultiCastShutVehicleEngine();
}

void UVehicleDriveAssemblyComponent::MultiCastShutVehicleEngine_Implementation()
{
	APawn* p = Cast<APawn>(GetOwner());
	if (!p->IsLocallyControlled() && !p->HasAuthority())
	{
		Engine->ShutVehicleEngine();
	}
}

void UVehicleDriveAssemblyComponent::OnRep_ServerVehicleEngineOperationMode()
{
	FVehicleEngineSimState e;
	Engine->GetEngineMovement(e);

	if (e.OperationMode != ServerVehicleEngineOperationMode)
	{
		switch (ServerVehicleEngineOperationMode)
		{
		case EVehicleEngineOperationMode::On:
			Engine->StartVehicleEngine();
			break;
		case EVehicleEngineOperationMode::Off:
			Engine->ShutVehicleEngine();
			break;
		case EVehicleEngineOperationMode::Starting:
			Engine->StartVehicleEngine();
			break;
		case EVehicleEngineOperationMode::Shutting:
			Engine->ShutVehicleEngine();
			break;
		default:
			break;
		}
	}
}

// Called every frame
void UVehicleDriveAssemblyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (Engine.IsValid() && Gearbox.IsValid() && Carbody.IsValid())
	{
		//update automatic gearbox
		if (AutoGearboxConfig.bAutomaticGearbox)
		{
			UpdateAutomaticGearbox(DeltaTime);

			if (AutoGearboxConfig.bArcadeAutoGearbox)
			{
				InputValues.bSwitchThrottleAndBrake = Gearbox->GetCurrentGear() < 0;
			}
			else
			{
				InputValues.bSwitchThrottleAndBrake = false;
			}
		}
		else
		{
			InputValues.bSwitchThrottleAndBrake = false;
		}
	}
}

void UVehicleDriveAssemblyComponent::UpdatePhysics(float InDeltaTime)
{
	UpdateInput(InDeltaTime);

	TRACE_CPUPROFILER_EVENT_SCOPE(UpdateVehiclePhysics);

	PhysicsDeltaTime = InDeltaTime;

	if (!(Engine.IsValid() && Clutch.IsValid() && Gearbox.IsValid() && TransferCase.IsValid()))
	{
		UE_LOG(LogTemp, Warning, TEXT("The drivetrain is not complete"));
		return;
	}

	UVehicleEngineComponent* EngineRaw = Engine.Get();
	UVehicleClutchComponent* ClutchRaw = Clutch.Get();
	UVehicleGearboxComponent* GearboxRaw = Gearbox.Get();
	UVehicleDifferentialComponent* TransferCaseRaw = TransferCase.Get();
	TArray<UVehicleAxleAssemblyComponent*> AxlesRaw;
	GetAxles(AxlesRaw);

	//update engine
	float ClutchTorque = (NumOfDriveAxles > 0) ? Clutch->GetCluchTorque() : 0.f;
	EngineRaw->UpdatePhysics(PhysicsDeltaTime, InputValues.Final.Throttle, ClutchTorque);

	//get gearbox output torque
	float GearboxOutputTorque;
	float GearboxReflectedInertia;
	GearboxRaw->UpdateOutputShaft(ClutchTorque, GearboxOutputTorque, GearboxReflectedInertia);

	//update transfercase
	float SumAxleInertia;
	float GearboxOutputShaftAngularVelocity;
	NumOfDriveAxles = TransferCaseRaw->UpdateTransferCase(
		AxlesRaw,
		PhysicsDeltaTime,
		GearboxOutputTorque,
		GearboxReflectedInertia,
		InputValues.Final.Brake,
		InputValues.Final.Handbrake,
		InputValues.Final.Steering,
		InputAssistConfig.bBurnOutAssist
		&& InputValues.Final.Throttle > 0.9
		&& InputValues.Final.Brake > 0.9
		&& LocalLinearVelocity.X < 1.f,
		GearboxOutputShaftAngularVelocity,
		SumAxleInertia
	);

	//get gearbox inputshaft angular velocity
	float GearboxInputShaftVelocity;
	//float GearboxReflectedInertia; // already defined
	float GearboxInputShaftInertia;
	float CurrentGearboxRatio;
	float HighestGearReflectedInertia;
	GearboxRaw->UpdateInputShaft(
		GearboxOutputShaftAngularVelocity, 
		SumAxleInertia,
		GearboxInputShaftVelocity, 
		GearboxReflectedInertia, 
		GearboxInputShaftInertia, 
		CurrentGearboxRatio, 
		HighestGearReflectedInertia
	);

	//get clutch torque for next frame
	ClutchRaw->UpdatePhysics(
		PhysicsDeltaTime, 
		InputValues.Final.Clutch,
		GearboxInputShaftVelocity, 
		GearboxReflectedInertia,
		GearboxInputShaftInertia,
		CurrentGearboxRatio, 
		HighestGearReflectedInertia,
		EngineRaw->GetAngularVelocity(),
		EngineRaw->NAConfig,
		EngineRaw->TurboConfig
	);

	//get velocity
	NumOfWheelsOnGround = 0;
	FVector3f SumLocalLinVel = FVector3f(0.f);
	FVector3f SumWorldLinVel = FVector3f(0.f);
	for (UVehicleAxleAssemblyComponent* Axle : AxlesRaw)
	{
		if (!Axle)continue;

		//calculate linear velocity
		FVector3f LocalVel;
		FVector3f WorldVel;
		NumOfWheelsOnGround += Axle->GetNumOfWheelsOnGround();
		Axle->GetLinearVelocity(LocalVel, WorldVel);
		SumLocalLinVel += LocalVel;
		SumWorldLinVel += WorldVel;
	}
	bIsInAir = !NumOfWheelsOnGround;
	float NumGroundedWheelsInv = VehicleUtil::SafeDivide(1.f, (float)NumOfWheelsOnGround);
	LocalLinearVelocity = SumLocalLinVel * 2.f * NumGroundedWheelsInv;
	WorldLinearVelocity = SumWorldLinVel * 2.f * NumGroundedWheelsInv;
	LocalVelocityClamped = LocalLinearVelocity.SquaredLength() > 0.01f ? LocalLinearVelocity : FVector3f(0.f);

	//get acceleration
	FVector3f LastAbsVelocity = AbsoluteWorldLinearVelocity;
	AbsoluteWorldLinearVelocity = 0.01f * (FVector3f)UAsyncTickFunctions::ATP_GetLinearVelocity(Carbody.Get());
	WorldAcceleration = VehicleUtil::SafeDivide(AbsoluteWorldLinearVelocity - LastAbsVelocity, PhysicsDeltaTime);
	FQuat4f CarbodyRot = (FQuat4f)UAsyncTickFunctions::ATP_GetTransform(Carbody.Get()).GetRotation();
	LocalAcceleration = CarbodyRot.UnrotateVector(WorldAcceleration);
}

void UVehicleDriveAssemblyComponent::InputThrottle(float InValue, bool bDirectInput)
{
	if (!GetOwner()->HasAuthority())
	{
		InputValues.Raw.Throttle = InValue;
		if (bDirectInput)
		{
			InputValues.Smoothened.Throttle = InValue;
			InputValues.Final.Throttle = InValue;
		}
	}
	ServerInputThrottle(InValue, bDirectInput);
}

void UVehicleDriveAssemblyComponent::InputBrake(float InValue, bool bDirectInput)
{
	if (!GetOwner()->HasAuthority())
	{
		InputValues.Raw.Brake = InValue;
		if (bDirectInput)
		{
			InputValues.Smoothened.Brake = InValue;
			InputValues.Final.Brake = InValue;
		}
	}
	ServerInputBrake(InValue, bDirectInput);
}

void UVehicleDriveAssemblyComponent::InputClutch(float InValue, bool bDirectInput)
{
	if (!GetOwner()->HasAuthority())
	{
		InputValues.Raw.Clutch = InValue;
		if (bDirectInput)
		{
			InputValues.Smoothened.Clutch = InValue;
			InputValues.Final.Clutch = InValue;
		}
	}
	ServerInputClutch(InValue, bDirectInput);
}

void UVehicleDriveAssemblyComponent::InputSteering(float InValue, bool bDirectInput)
{
	if (!GetOwner()->HasAuthority())
	{
		InputValues.Raw.Steering = InValue;
		if (bDirectInput)
		{
			InputValues.Smoothened.Steering = InValue;
			InputValues.Final.Steering = InValue;
		}
	}
	ServerInputSteering(InValue, bDirectInput);
}

void UVehicleDriveAssemblyComponent::InputHandbrake(float InValue, bool bDirectInput)
{
	if (!GetOwner()->HasAuthority())
	{
		InputValues.Raw.Handbrake = InValue;
		if (bDirectInput)
		{
			InputValues.Smoothened.Handbrake = InValue;
			InputValues.Final.Handbrake = InValue;
		}
	}
	ServerInputHandbrake(InValue, bDirectInput);
}

void UVehicleDriveAssemblyComponent::ShiftToTargetGear(int32 InTargetGear, float InAutoShiftCoolDown, bool bImmediate)
{
	if (Gearbox.IsValid()) 
	{
		AutoGearboxCount = -InAutoShiftCoolDown;

		//Client predict
		if (!GetOwner()->HasAuthority())
		{
			Gearbox->ShiftToTargetGear(InTargetGear, bImmediate);
		}

		ServerShiftToTargetGear(InTargetGear, bImmediate);
	}
}

void UVehicleDriveAssemblyComponent::ShiftUp(float InAutoShiftCoolDown, bool bImmediate)
{
	if (Gearbox.IsValid())
	{
		ShiftToTargetGear(Gearbox->GetCurrentGear() + 1, InAutoShiftCoolDown, bImmediate);
	}
}

void UVehicleDriveAssemblyComponent::ShiftDown(float InAutoShiftCoolDown, bool bImmediate)
{
	if (Gearbox.IsValid())
	{
		ShiftToTargetGear(Gearbox->GetCurrentGear() - 1, InAutoShiftCoolDown, bImmediate);
	}
}

EVehicleEngineOperationMode UVehicleDriveAssemblyComponent::StartVehicleEngine()
{
	if (Engine.IsValid())
	{
		if (!GetOwner()->HasAuthority())
		{
			Engine->StartVehicleEngine();
		}

		ServerStartVehicleEngine();
		return ServerVehicleEngineOperationMode;
	}
	else
	{
		return EVehicleEngineOperationMode::Off;
	}
}

EVehicleEngineOperationMode UVehicleDriveAssemblyComponent::ShutVehicleEngine()
{
	if (Engine.IsValid())
	{
		if (!GetOwner()->HasAuthority())
		{
			Engine->ShutVehicleEngine();
		}

		ServerShutVehicleEngine();
		return ServerVehicleEngineOperationMode;
	}
	else
	{
		return EVehicleEngineOperationMode::On;
	}
}

void UVehicleDriveAssemblyComponent::RotateCamera(USceneComponent* InSpringArm, FVector2D InMouseInput, bool bInvertYAxis, float InMaxPitch)
{
	if (IsValid(InSpringArm))
	{
		FRotator Rot = InSpringArm->GetRelativeRotation();
		FRotator NewRot;

		NewRot.Roll = 0;
		NewRot.Pitch = FMath::Clamp(Rot.Pitch + InMouseInput.Y, -InMaxPitch, InMaxPitch);
		NewRot.Yaw = Rot.Yaw + InMouseInput.X;

		InSpringArm->SetRelativeRotation(NewRot);
	}
}

void UVehicleDriveAssemblyComponent::StretchSpringArmBySpeed(USpringArmComponent* InSpringArm, float InInitialSpringArmLength, UCurveFloat* InScaleCurve)
{
	if (IsValid(InSpringArm) && IsValid(InScaleCurve))
	{
		float Scale = InScaleCurve->GetFloatValue(FMath::Abs(LocalVelocityClamped.X));
		float L = InInitialSpringArmLength * Scale;
		InSpringArm->TargetArmLength = L;
	}
}

void UVehicleDriveAssemblyComponent::CameraLookAtVelocity(USceneComponent* InSpringArm, float InPitch, float InSensitivity, float InInterpSpeed, float InStartSpeed_mps)
{
	if (IsValid(InSpringArm) && LocalLinearVelocity.SquaredLength() > FMath::Square(InStartSpeed_mps))
	{
		FRotator CurrentCamRot = InSpringArm->GetRelativeRotation();

		FVector ScaledLinearVelocity = (FVector)LocalVelocityClamped;
		ScaledLinearVelocity.Y *= InSensitivity;

		FRotator TargetRotator = FRotationMatrix::MakeFromX(ScaledLinearVelocity).Rotator();
		TargetRotator.Pitch = InPitch;

		FRotator NewRot = FMath::RInterpTo(CurrentCamRot, TargetRotator, GetWorld()->DeltaTimeSeconds, InInterpSpeed);
		NewRot.Roll = 0;

		InSpringArm->SetRelativeRotation(NewRot);
	}
}

void UVehicleDriveAssemblyComponent::GetAxles(TArray<UVehicleAxleAssemblyComponent*>& OutAxles)
{
	OutAxles.Reset();
	for (TWeakObjectPtr<UVehicleAxleAssemblyComponent> AxlePtr : Axles)
	{
		if (UVehicleAxleAssemblyComponent* AxleRaw = AxlePtr.Get())
		{
			OutAxles.Add(AxleRaw);
		}
	}
}

void UVehicleDriveAssemblyComponent::GetWheels(TArray<UVehicleWheelComponent*>& OutWheels)
{
	OutWheels.Reset();
	for (TWeakObjectPtr<UVehicleAxleAssemblyComponent> Axle : Axles)
	{
		if (!Axle.IsValid())continue;

		UVehicleWheelComponent* LeftWheel;
		UVehicleWheelComponent* RightWheel;

		Axle->GetWheels(LeftWheel, RightWheel);

		OutWheels.Add(LeftWheel);
		OutWheels.Add(RightWheel);
	}
}

void UVehicleDriveAssemblyComponent::DestroyTargetAxle(UVehicleAxleAssemblyComponent* InTargetAxle)
{
	Axles.Remove(InTargetAxle);
	InTargetAxle->DestroyComponent();
}

int32 UVehicleDriveAssemblyComponent::GetCurrentGear()
{
	return Gearbox.IsValid() ? Gearbox->GetCurrentGear() : 0;
}

bool UVehicleDriveAssemblyComponent::GeneratePowerUnit()
{
	AActor* Owner = GetOwner();
	if (!IsValid(Owner))return false;

	if (!Engine.IsValid())
	{
		if (bUseExistingEngineComponent)
		{
			Engine = VehicleUtil::GetComponentByName<UVehicleEngineComponent>(Owner, EngineComponentName);
		}

		if (!Engine.IsValid())
		{
			if (EngineConfig)
			{
				Engine = Cast<UVehicleEngineComponent>
					(Owner->AddComponentByClass(
						EngineConfig, false, FTransform(), false));
			}
			else
			{
				Engine = Cast<UVehicleEngineComponent>
					(Owner->AddComponentByClass(
						UVehicleEngineComponent::StaticClass(), false, FTransform(), false));
			}
		}
	}

	if (!Clutch.IsValid())
	{
		if (bUseExistingClutchComponent)
		{
			Clutch = VehicleUtil::GetComponentByName<UVehicleClutchComponent>(Owner, ClutchComponentName);
		}

		if (!Clutch.IsValid())
		{
			if (ClutchConfig)
			{
				Clutch = Cast<UVehicleClutchComponent>
					(Owner->AddComponentByClass(
						ClutchConfig, false, FTransform(), false));
			}
			else
			{
				Clutch = Cast<UVehicleClutchComponent>
					(Owner->AddComponentByClass(
						UVehicleClutchComponent::StaticClass(), false, FTransform(), false));
			}
		}
	}

	if (!Gearbox.IsValid())
	{
		if (bUseExistingGearboxComponent)
		{
			Gearbox = VehicleUtil::GetComponentByName<UVehicleGearboxComponent>(Owner, GearboxComponentName);
		}

		if (!Gearbox.IsValid())
		{
			if (GearboxConfig)
			{
				Gearbox = Cast<UVehicleGearboxComponent>
					(Owner->AddComponentByClass(
						GearboxConfig, false, FTransform(), false));
			}
			else
			{
				Gearbox = Cast<UVehicleGearboxComponent>
					(Owner->AddComponentByClass(
						UVehicleGearboxComponent::StaticClass(), false, FTransform(), false));
			}
		}
	}

	if (!TransferCase.IsValid())
	{
		if (bUseExistingTransferCaseComponent)
		{
			TransferCase = VehicleUtil::GetComponentByName<UVehicleDifferentialComponent>(Owner, TransferCaseComponentName);
		}

		if (!TransferCase.IsValid())
		{
			if (TransferCaseConfig)
			{
				TransferCase = Cast<UVehicleDifferentialComponent>
					(Owner->AddComponentByClass(
						TransferCaseConfig, false, FTransform(), false));
			}
			else
			{
				TransferCase = Cast<UVehicleDifferentialComponent>
					(Owner->AddComponentByClass(
						UVehicleDifferentialComponent::StaticClass(), false, FTransform(), false));
				TransferCase->Config.GearRatio = 1.f;
			}
		}
	}

	return Engine.IsValid() && Clutch.IsValid() && Gearbox.IsValid() && TransferCase.IsValid();
}

int UVehicleDriveAssemblyComponent::GenerateAxles()
{
	if (!Carbody.IsValid())
	{
		Carbody = UVehicleWheelCoordinatorComponent::FindPhysicalParent(this);
		if (!Carbody.IsValid())
		{
			return -1;
		}
	}

	//if there are axles, destroy them
	if (Axles.Num() > 0)
	{
		for (TWeakObjectPtr<UVehicleAxleAssemblyComponent> Axle : Axles)
		{
			if (Axle.IsValid())Axle->DestroyComponent();
		}
		Axles.Empty();
	}

	AActor* Owner = GetOwner();
	if (!IsValid(Owner))return 0;

	int32 n = 0;
	for (FAxleAssemblyConfig AxleConfig : AxleConfigs)
	{
		if (!AxleConfig.bUseExistingComponent)
		{
			// generate new axle
			UVehicleAxleAssemblyComponent* Axle = Cast<UVehicleAxleAssemblyComponent>
				(Owner->AddComponentByClass(UVehicleAxleAssemblyComponent::StaticClass(), false, FTransform(), false));
			if (IsValid(Axle))
			{
				Axle->AttachToComponent(Carbody.Get(), FAttachmentTransformRules::KeepRelativeTransform);
				Axle->SetRelativeLocation(AxleConfig.AxlePosition);
			}

			if (IsValid(AxleConfig.AxleConfig))
			{
				const UVehicleAxleAssemblyComponent* TemplateAxle = Cast<UVehicleAxleAssemblyComponent>(AxleConfig.AxleConfig->GetDefaultObject());
				if (IsValid(Axle))
				{
					UVehicleAxleAssemblyComponent::CopyAxleConfig(TemplateAxle, Axle);
				}
			}
			
			if (IsValid(Axle))
			{
				//override these
				if (AxleConfig.WheelOverride)Axle->WheelConfig = AxleConfig.WheelOverride;
				if (AxleConfig.DifferentialOverride)Axle->DifferentialConfig = AxleConfig.DifferentialOverride;
				if (AxleConfig.TrackWidthOverride >= 0)Axle->AxleConfig.TrackWidth = AxleConfig.TrackWidthOverride;
				if (AxleConfig.bDiasbleSteering)Axle->AxleSteeringConfig.bAffectedBySteering = false;
				if (AxleConfig.bDisableHandbrake)Axle->AxleConfig.bAffectedByHandbrake = false;
				if (AxleConfig.bDisableTractionControl)Axle->TCSConfig.bTractionControlSystemEnabled = false;
				if (AxleConfig.TorqueWeightOverride >= 0)Axle->AxleConfig.TorqueWeight = AxleConfig.TorqueWeightOverride;

				// initialize
				Axle->InitializeWheels();

				Axles.Add(Axle);
				n++;
			}
		}		
	}
	return n;
}

int UVehicleDriveAssemblyComponent::SearchExistingAxles()
{
	int32 n = 0;
	for (FAxleAssemblyConfig AxleConfig : AxleConfigs)
	{
		AActor* Owner = GetOwner();
		if (AxleConfig.bUseExistingComponent && IsValid(Owner))
		{
			UVehicleAxleAssemblyComponent* Axle =
				VehicleUtil::GetComponentByName<UVehicleAxleAssemblyComponent>(Owner, AxleConfig.AxleComponentName);
			if (IsValid(Axle))
			{
				Axles.Add(Axle);
				n++;
			}
		}
	}
	return n;
}

TArray<FName> UVehicleDriveAssemblyComponent::GetNamesOfAxlesOfOwner()
{
	return VehicleUtil::GetNamesOfComponentsOfActor<UVehicleAxleAssemblyComponent>(this);
}

TArray<FName> UVehicleDriveAssemblyComponent::GetNamesOfEnginesOfOwner()
{
	return VehicleUtil::GetNamesOfComponentsOfActor<UVehicleEngineComponent>(this);
}

TArray<FName> UVehicleDriveAssemblyComponent::GetNamesOfClutchesOfOwner()
{
	return VehicleUtil::GetNamesOfComponentsOfActor<UVehicleClutchComponent>(this);
}

TArray<FName> UVehicleDriveAssemblyComponent::GetNamesOfGearboxesOfOwner()
{
	return VehicleUtil::GetNamesOfComponentsOfActor<UVehicleGearboxComponent>(this);
}

TArray<FName> UVehicleDriveAssemblyComponent::GetNamesOfTransferCasesOfOwner()
{
	return VehicleUtil::GetNamesOfComponentsOfActor<UVehicleDifferentialComponent>(this);
}

