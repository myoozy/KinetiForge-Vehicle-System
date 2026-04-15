// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)


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
#include "VehicleUtilities.h"
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
	WheelCoordinator = UVehicleWheelCoordinatorComponent::FindWheelCoordinator(Chassis.Get());
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
	Chassis = UVehicleUtilities::FindPhysicalParent(this);
	if (AActor* Owner = GetOwner())
	{
#if WITH_EDITOR
		Owner->bRunConstructionScriptOnDrag = false;
#endif
		Owner->SetReplicates(true);
	}
	GenerateAxles();
}

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

	if (VehicleAsyncTickComponent.IsValid())VehicleAsyncTickComponent->UnRegister(this);

	Chassis = nullptr;

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
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_DriveAssembly_UpdateInput);

	if (AutoGearboxConfig.bAutomaticGearbox && AutoGearboxConfig.bArcadeAutoGearbox)
	{
		InputValues.bSwitchThrottleAndBrake = Gearbox->GetCurrentGear() < 0;
	}
	else
	{
		InputValues.bSwitchThrottleAndBrake = false;
	}

	UpdateThrottle(InDeltaTime);
	UpdateBrake(InDeltaTime);
	UpdateClutch(InDeltaTime);
	UpdateSteering(InDeltaTime);
	UpdateHandbrake(InDeltaTime);
}

void UVehicleDriveAssemblyComponent::UpdateThrottle(float InDeltaTime)
{
	float RealThrottleInput = InputValues.bSwitchThrottleAndBrake ? InputValues.Raw.Brake : InputValues.Raw.Throttle;
	
	if (UVehicleGearboxComponent* GearboxRaw = Gearbox.Get())
	{
		//if not in gear and should rev-match
		if (GearboxRaw->GetShouldRevMatch() &&
			RealThrottleInput < InputAssistConfig.RevMatchMaxThrottle && 
			FMath::Abs(LocalLinearVelocity.X) > 0.5 && 
			InputAssistConfig.bRevMatching)
		{
			const float Rate = 5.f;
			InputValues.Smoothened.Throttle += UVehicleUtilities::SafeDivide(InDeltaTime * Rate, GearboxRaw->Config.ShiftDelay);
			InputValues.Smoothened.Throttle = FMath::Min(InputValues.Smoothened.Throttle, InputAssistConfig.RevMatchMaxThrottle);
		}
		//if not in gear and no rev-matching and not sequential
		else if (!GearboxRaw->GetIsInGear() &&
			InputAssistConfig.bAutomaticClutch && 
			!GearboxRaw->Config.bSequentialGearbox)
		{
			InputValues.Smoothened.Throttle = 0.f;
		}
		else
		{
			InputValues.Smoothened.Throttle = FVehicleInputAxisConfig::InterpInputValueConstant(
				InputValues.Smoothened.Throttle, 
				RealThrottleInput, InDeltaTime, 
				InputConfig.Throttle.InterpSpeed
			);
			InputValues.Smoothened.Throttle *= !(InputAssistConfig.bEVClutchLogic && GearboxRaw->GetCurrentGearRatio() == 0.f);
		}
	}
	else
	{
		InputValues.Smoothened.Throttle = FVehicleInputAxisConfig::InterpInputValueConstant(InputValues.Smoothened.Throttle, RealThrottleInput, InDeltaTime, InputConfig.Throttle.InterpSpeed);
	}
	InputValues.Smoothened.Throttle = FMath::Clamp(InputValues.Smoothened.Throttle, 0.f, 1.f);

	if (IsValid(InputConfig.Throttle.ResponseCurve))
	{
		InputValues.Final.Throttle = InputConfig.Throttle.ResponseCurve->GetFloatValue(InputValues.Smoothened.Throttle);
	}
	else
	{
		InputValues.Final.Throttle = InputValues.Smoothened.Throttle;
	}
	InputValues.Final.Throttle = FMath::Clamp(InputValues.Final.Throttle, 0.f, 1.f);
}

void UVehicleDriveAssemblyComponent::UpdateBrake(float InDeltaTime)
{
	float RealBrakeInput = InputValues.bSwitchThrottleAndBrake ? InputValues.Raw.Throttle : InputValues.Raw.Brake;

	//update brake
	InputValues.Smoothened.Brake = FVehicleInputAxisConfig::InterpInputValueConstant(InputValues.Smoothened.Brake, RealBrakeInput, InDeltaTime, InputConfig.Brake.InterpSpeed);
	InputValues.Smoothened.Brake = FMath::Clamp(InputValues.Smoothened.Brake, 0.f, 1.f);

	if (IsValid(InputConfig.Brake.ResponseCurve))
	{
		InputValues.Final.Brake = InputConfig.Brake.ResponseCurve->GetFloatValue(InputValues.Smoothened.Brake);
	}
	else
	{
		InputValues.Final.Brake = InputValues.Smoothened.Brake;
	}
	InputValues.Final.Brake = FMath::Clamp(InputValues.Final.Brake, 0.f, 1.f);
}

void UVehicleDriveAssemblyComponent::UpdateClutch(float InDeltaTime)
{
	UVehicleGearboxComponent* GearboxRaw = Gearbox.Get();
	UVehicleEngineComponent * EngineRaw = Engine.Get();

	if (InputAssistConfig.bEVClutchLogic)
	{
		InputValues.Smoothened.Clutch = 0.f;
	}
	else if (InputAssistConfig.bAutomaticClutch && GearboxRaw && EngineRaw)
	{
		//check if clutch has to be engaged
		bool bNotInGearAndNotSequential = !(GearboxRaw->GetIsInGear() || GearboxRaw->Config.bSequentialGearbox);
		float TargetClutchValue = (InputValues.Raw.Handbrake > 0.9 || bNotInGearAndNotSequential) ? 1.f : InputValues.Raw.Clutch;

		//take engine rpm into account
		float Bias = FMath::GetMappedRangeValueClamped(InputAssistConfig.AutoClutchRange, FVector2f(1, 0), EngineRaw->GetRPM());
		TargetClutchValue = FMath::Clamp(TargetClutchValue + Bias, 0.f, 1.f);
		InputValues.Smoothened.Clutch = FMath::Min(InputValues.Smoothened.Clutch + Bias, TargetClutchValue);

		//get interp speed, when changing gear, clutch should engage faster than gear changes
		FVector2f FinalInterpSpeed;
		float Rate = 10.f;
		FinalInterpSpeed.Y = InputConfig.Clutch.InterpSpeed.Y;
		FinalInterpSpeed.X = bNotInGearAndNotSequential ? UVehicleUtilities::SafeDivide(Rate, GearboxRaw->Config.ShiftDelay) : InputConfig.Clutch.InterpSpeed.X;

		InputValues.Smoothened.Clutch = FVehicleInputAxisConfig::InterpInputValueConstant(InputValues.Smoothened.Clutch, TargetClutchValue, InDeltaTime, FinalInterpSpeed);
	}
	else
	{
		InputValues.Smoothened.Clutch = FVehicleInputAxisConfig::InterpInputValueConstant(InputValues.Smoothened.Clutch, InputValues.Raw.Clutch, InDeltaTime, InputConfig.Clutch.InterpSpeed);
	}
	InputValues.Smoothened.Clutch = FMath::Clamp(InputValues.Smoothened.Clutch, 0.f, 1.f);

	if (IsValid(InputConfig.Clutch.ResponseCurve))
	{
		InputValues.Final.Clutch = InputConfig.Clutch.ResponseCurve->GetFloatValue(InputValues.Smoothened.Clutch);
	}
	else
	{
		InputValues.Final.Clutch = InputValues.Smoothened.Clutch;
	}
	InputValues.Final.Clutch = FMath::Clamp(InputValues.Final.Clutch, 0.f, 1.f);
}

void UVehicleDriveAssemblyComponent::UpdateSteering(float InDeltaTime)
{
	float RealSteeringInput = InputValues.Raw.Steering;
	float Scale = 1.f;

	//reduce the steering value when driving at high speed
	if (InputConfig.HighSpeedSteeringScale)
	{
		Scale = InputConfig.HighSpeedSteeringScale->GetFloatValue(FMath::Abs(LocalLinearVelocity.X));
	}

	InputValues.Smoothened.Steering = FVehicleInputAxisConfig::InterpInputValueConstant(InputValues.Smoothened.Steering, RealSteeringInput, InDeltaTime, InputConfig.Steering.InterpSpeed);
	InputValues.Smoothened.Steering = FMath::Clamp(InputValues.Smoothened.Steering, -1.f, 1.f);

	if (IsValid(InputConfig.Steering.ResponseCurve))
	{
		float CurveValue = InputConfig.Steering.ResponseCurve->GetFloatValue(FMath::Abs(InputValues.Smoothened.Steering));
		InputValues.Final.Steering = CurveValue * FMath::Sign(InputValues.Smoothened.Steering) * Scale;
	}
	else
	{
		InputValues.Final.Steering = InputValues.Smoothened.Steering * Scale;
	}
	InputValues.Final.Steering = FMath::Clamp(InputValues.Final.Steering, -1.f, 1.f);
}

void UVehicleDriveAssemblyComponent::UpdateHandbrake(float InDeltaTime)
{
	float RealThrottleInput = InputValues.bSwitchThrottleAndBrake ? InputValues.Raw.Brake : InputValues.Raw.Throttle;

	//update handbrake
	float HandbrakeTarget = InputValues.Raw.Handbrake;
	if (InputAssistConfig.bAutoHold
		&& FMath::Abs(LocalLinearVelocity.X) < 0.1
		&& RealThrottleInput < SMALL_NUMBER
		&& !bIsInAir)HandbrakeTarget = 1.f;
	InputValues.Smoothened.Handbrake = FVehicleInputAxisConfig::InterpInputValueConstant(InputValues.Smoothened.Handbrake, HandbrakeTarget, InDeltaTime, InputConfig.Handbrake.InterpSpeed);
	InputValues.Smoothened.Handbrake = FMath::Clamp(InputValues.Smoothened.Handbrake, 0.f, 1.f);

	if (IsValid(InputConfig.Handbrake.ResponseCurve))
	{
		InputValues.Final.Handbrake = InputConfig.Handbrake.ResponseCurve->GetFloatValue(InputValues.Smoothened.Handbrake);
	}
	else
	{
		InputValues.Final.Handbrake = InputValues.Smoothened.Handbrake;
	}
	InputValues.Final.Handbrake = FMath::Clamp(InputValues.Final.Handbrake, 0.f, 1.f);
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

	UVehicleGearboxComponent* GearboxRaw = Gearbox.Get();
	UVehicleEngineComponent* EngineRaw = Engine.Get();

	if (bIsInAir || 
		!GearboxRaw ||
		!EngineRaw ||
		!GearboxRaw->GetIsInGear() ||
		!AutoGearboxTimerOverFlowed ||
		(!GearboxRaw->Config.NumberOfGears && !GearboxRaw->Config.NumOfReverseGears)
		)return;

	float LinearVelocityX = FMath::Abs(LocalLinearVelocity.X);

	//arcade gearbox logic; automatically shift to N / D1 / R1 gear
	if (AutoGearboxConfig.bArcadeAutoGearbox)
	{
		//if in N gear
		if (GearboxRaw->GetCurrentGear() == 0)
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
		else if (LinearVelocityX < 1 || (LocalLinearVelocity.X * GearboxRaw->GetCurrentGear()) < 0)
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
	if (!GearboxRaw->GetCurrentGear())return;

	TArray<UVehicleAxleAssemblyComponent*> AxlesRaw;
	GetAxles(AxlesRaw);
	GearboxRaw->CalculateSpeedRangeOfEachGear(
		TransferCase->CalculateEffectiveWheelRadius(AxlesRaw),
		EngineRaw->GetIdleRPM(),
		EngineRaw->GetMaxRPM(),
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
	float MinShiftUpFactor = UVehicleUtilities::SafeDivide(EngineRaw->GetIdleRPM(), EngineRaw->GetMaxRPM());
	ShiftFactor = FMath::Max(MinShiftUpFactor, ShiftFactor);

	int32 UnsignedCurrentGear = FMath::Abs(GearboxRaw->GetCurrentGear());
	int32 StartGear = FMath::Max(UnsignedCurrentGear - AutoGearboxConfig.MaxDownShiftSteps, 1);
	int32 EndGear = FMath::Min(UnsignedCurrentGear + AutoGearboxConfig.MaxUpShiftSteps,
		FMath::Max(GearboxRaw->Config.NumberOfGears, GearboxRaw->Config.NumOfReverseGears));
	
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

	bool GearingUp = FMath::Abs(GearboxRaw->GetCurrentGear()) < TargetGear;

	//check sign
	if (LocalLinearVelocity.X < 0)TargetGear = -TargetGear;

	if (GearboxRaw->GetCurrentGear() != TargetGear
		&& TargetGear * GearboxRaw->GetCurrentGear() > 0
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
		if (InputValues.bSwitchThrottleAndBrake)
		{
			InputValues.Smoothened.Brake = InValue;
		}
		else
		{
			InputValues.Smoothened.Throttle = InValue;
		}
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
			if (InputValues.bSwitchThrottleAndBrake)
			{
				InputValues.Smoothened.Brake = InValue;
			}
			else
			{
				InputValues.Smoothened.Throttle = InValue;
			}
		}
	}
}

void UVehicleDriveAssemblyComponent::ServerInputBrake_Implementation(float InValue, bool bDirectInput)
{
	InputValues.Raw.Brake = InValue;
	if (bDirectInput)
	{
		if (InputValues.bSwitchThrottleAndBrake)
		{
			InputValues.Smoothened.Throttle = InValue;
		}
		else
		{
			InputValues.Smoothened.Brake = InValue;
		}
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
			if (InputValues.bSwitchThrottleAndBrake)
			{
				InputValues.Smoothened.Throttle = InValue;
			}
			else
			{
				InputValues.Smoothened.Brake = InValue;
			}
		}
	}
}

void UVehicleDriveAssemblyComponent::ServerInputClutch_Implementation(float InValue, bool bDirectInput)
{
	InputValues.Raw.Clutch = InValue;
	if (bDirectInput)
	{
		InputValues.Smoothened.Clutch = InValue;
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
		}
	}
}

void UVehicleDriveAssemblyComponent::ServerInputSteering_Implementation(float InValue, bool bDirectInput)
{
	InputValues.Raw.Steering = InValue;
	if (bDirectInput)
	{
		InputValues.Smoothened.Steering = InValue;
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
		}
	}
}

void UVehicleDriveAssemblyComponent::ServerInputHandbrake_Implementation(float InValue, bool bDirectInput)
{
	InputValues.Raw.Handbrake = InValue;
	if (bDirectInput)
	{
		InputValues.Smoothened.Handbrake = InValue;
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
	if (Engine.IsValid() && Gearbox.IsValid() && Chassis.IsValid())
	{
		//update automatic gearbox
		if (AutoGearboxConfig.bAutomaticGearbox)
		{
			UpdateAutomaticGearbox(DeltaTime);
		}
	}
}

void UVehicleDriveAssemblyComponent::UpdatePhysics(float InDeltaTime)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_DriveAssembly_UpdatePhysics);

	UpdateInput(InDeltaTime);

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

	// update engine
	float ClutchTorque = (NumOfDriveAxles > 0) ? Clutch->GetCluchTorque() : 0.f;
	// sequential transmission disables spark when shifting up
	bool bDisableSpark = GearboxRaw->GetShouldCutSpark();
	EngineRaw->UpdatePhysics(PhysicsDeltaTime, InputValues.Final.Throttle, ClutchTorque, bDisableSpark);

	// get gearbox output torque
	float GearboxOutputTorque;
	float GearboxReflectedInertia;
	GearboxRaw->UpdateOutputShaft(ClutchTorque, GearboxOutputTorque, GearboxReflectedInertia);

	// update transfercase
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

	// get gearbox inputshaft angular velocity
	float GearboxInputShaftVelocity;
	// float GearboxReflectedInertia; // already defined
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

	// get clutch torque for next frame
	ClutchRaw->UpdatePhysics(
		PhysicsDeltaTime, 
		InputValues.Final.Clutch,
		GearboxInputShaftVelocity, 
		GearboxReflectedInertia,
		GearboxInputShaftInertia,
		CurrentGearboxRatio, 
		HighestGearReflectedInertia,
		EngineRaw->GetAngularVelocity(),
		EngineRaw->GetEngineInertia(),
		EngineRaw->GetMaxEngineTorque()
	);

	// get velocity
	NumOfWheelsOnGround = 0;
	FVector3f SumLocalLinVel = FVector3f(0.f);
	FVector3f SumWorldLinVel = FVector3f(0.f);
	for (UVehicleAxleAssemblyComponent* Axle : AxlesRaw)
	{
		if (!Axle)continue;

		// calculate linear velocity
		FVector3f LocalVel;
		FVector3f WorldVel;
		NumOfWheelsOnGround += Axle->GetNumOfWheelsOnGround();
		Axle->GetLinearVelocity(LocalVel, WorldVel);
		SumLocalLinVel += LocalVel;
		SumWorldLinVel += WorldVel;
	}
	bIsInAir = !NumOfWheelsOnGround;
	float NumGroundedWheelsInv = UVehicleUtilities::SafeDivide(1.f, (float)NumOfWheelsOnGround);
	LocalLinearVelocity = SumLocalLinVel * 2.f * NumGroundedWheelsInv;
	WorldLinearVelocity = SumWorldLinVel * 2.f * NumGroundedWheelsInv;
	LocalVelocityClamped = LocalLinearVelocity.SquaredLength() > 0.01f ? LocalLinearVelocity : FVector3f(0.f);

	// get acceleration
	FVector3f LastAbsVelocity = AbsoluteWorldLinearVelocity;
	AbsoluteWorldLinearVelocity = 0.01f * (FVector3f)UAsyncTickFunctions::ATP_GetLinearVelocity(Chassis.Get());
	WorldAcceleration = UVehicleUtilities::SafeDivide(AbsoluteWorldLinearVelocity - LastAbsVelocity, PhysicsDeltaTime);
	FQuat4f ChassisRot = (FQuat4f)UAsyncTickFunctions::ATP_GetTransform(Chassis.Get()).GetRotation();
	LocalAcceleration = ChassisRot.UnrotateVector(WorldAcceleration);
}

void UVehicleDriveAssemblyComponent::InputThrottle(float InValue, bool bDirectInput)
{
	if (!GetOwner()->HasAuthority())
	{
		InputValues.Raw.Throttle = InValue;
		if (bDirectInput)
		{
			if (InputValues.bSwitchThrottleAndBrake)
			{
				InputValues.Smoothened.Brake = InValue;
			}
			else
			{
				InputValues.Smoothened.Throttle = InValue;
			}
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
			if (InputValues.bSwitchThrottleAndBrake)
			{
				InputValues.Smoothened.Throttle = InValue;
			}
			else
			{
				InputValues.Smoothened.Brake = InValue;
			}
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
		}
	}
	ServerInputHandbrake(InValue, bDirectInput);
}

void UVehicleDriveAssemblyComponent::ShiftToTargetGear(int32 InTargetGear, float InAutoShiftCoolDown, bool bImmediate)
{
	if (Gearbox.IsValid()) 
	{
		if (AutoGearboxConfig.bAutomaticGearbox)
		{
			AutoGearboxCount = -InAutoShiftCoolDown;
		}

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

void UVehicleDriveAssemblyComponent::PauseAutoGearbox(float InCoolDownTime)
{
	if (AutoGearboxConfig.bAutomaticGearbox)
	{
		AutoGearboxCount = -InCoolDownTime;
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

void UVehicleDriveAssemblyComponent::StretchSpringArmBySpeed(
	USpringArmComponent* InSpringArm, 
	float InInitialSpringArmLength, 
	UCurveFloat* InScaleCurve)
{
	if (IsValid(InSpringArm) && IsValid(InScaleCurve))
	{
		float Scale = InScaleCurve->GetFloatValue(FMath::Abs(LocalVelocityClamped.X));
		float L = InInitialSpringArmLength * Scale;
		InSpringArm->TargetArmLength = L;
	}
}

void UVehicleDriveAssemblyComponent::CameraLookAtVelocity(
	USceneComponent* InSpringArm, 
	float InPitch, 
	float InSensitivity, 
	float InInterpSpeed, 
	FVector2D InStartSpeed_meter_per_second)
{
	if (IsValid(InSpringArm))
	{
		FVector2f LerpSpeedRange = (FVector2f)InStartSpeed_meter_per_second;
		FVector2f LocalVelocity2D = FVector2f(LocalLinearVelocity.X, LocalLinearVelocity.Y);
		float CurrSpeed = LocalVelocity2D.Length();
		float Alpha = FMath::GetMappedRangeValueClamped(
			LerpSpeedRange,
			FVector2f(0.f, 1.f),
			CurrSpeed
		);

		FRotator CurrentCamRot = InSpringArm->GetRelativeRotation();
		FQuat CurrCamQuat = CurrentCamRot.Quaternion();

		FVector ScaledLinearVelocity = (FVector)LocalVelocityClamped;
		ScaledLinearVelocity.Y *= InSensitivity;

		FRotator TargetRotator = FRotationMatrix::MakeFromX(ScaledLinearVelocity).Rotator();
		TargetRotator.Pitch = InPitch;

		// remain rotation if speed too low
		FQuat TargetQuat = FMath::Lerp(CurrCamQuat, TargetRotator.Quaternion(), Alpha);

		FRotator NewRot = FMath::QInterpTo(CurrCamQuat, TargetQuat, GetWorld()->DeltaTimeSeconds, InInterpSpeed).Rotator();
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

TArray<UVehicleAxleAssemblyComponent*> UVehicleDriveAssemblyComponent::GetAxles()
{
	TArray<UVehicleAxleAssemblyComponent*> Result;
	GetAxles(Result);
	return Result;
}

TArray<TWeakObjectPtr<UVehicleAxleAssemblyComponent>>& UVehicleDriveAssemblyComponent::GetAxlesRef()
{
	return Axles;
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

TArray<UVehicleWheelComponent*> UVehicleDriveAssemblyComponent::GetWheels()
{
	TArray<UVehicleWheelComponent*> Result;
	GetWheels(Result);
	return Result;
}

void UVehicleDriveAssemblyComponent::DestroyTargetAxle(UVehicleAxleAssemblyComponent* InTargetAxle)
{
	Axles.Remove(InTargetAxle);
	InTargetAxle->DestroyComponent();
}

float UVehicleDriveAssemblyComponent::GetMaxTrackWidth()
{
	if (UVehicleWheelCoordinatorComponent* WheelCoord = WheelCoordinator.Get())
	{
		return WheelCoord->GetMaxTrackWidth();
	}
	return 0.f;
}

float UVehicleDriveAssemblyComponent::GetMaxWheelBase()
{
	if (UVehicleWheelCoordinatorComponent* WheelCoord = WheelCoordinator.Get())
	{
		return WheelCoord->GetMaxWheelBase();
	}
	return 0.f;
}

void UVehicleDriveAssemblyComponent:: GetVehicleBound(
	FVector& OutExtent, 
	FVector& OutOrigin, 
	FTransform& OutTransform,
	bool bReturnWorldTransform
)
{
	if (UPrimitiveComponent* Prim = Chassis.Get())
	{
		FBoxSphereBounds CollisionBounds = Prim->CalcBounds(FTransform::Identity);
		OutExtent = CollisionBounds.BoxExtent; // HalfSize
		OutOrigin = CollisionBounds.Origin;

		FTransform CompTransform = bReturnWorldTransform ?
			Prim->GetComponentTransform() : Prim->GetRelativeTransform();

		OutTransform = FTransform(
			CompTransform.GetRotation(),
			CompTransform.TransformPosition(CollisionBounds.Origin),
			CompTransform.GetScale3D()
		);
	}
}

float UVehicleDriveAssemblyComponent::GetVehicleForwardSpeed()
{
	return LocalLinearVelocity.X;
}

int32 UVehicleDriveAssemblyComponent::GetCurrentGear()
{
	return Gearbox.IsValid() ? Gearbox->GetCurrentGear() : 0;
}

float UVehicleDriveAssemblyComponent::GetSteeringValueFromAxles()
{
	float Sum = 0.f;
	float Num = 0.f;

	for (TWeakObjectPtr<UVehicleAxleAssemblyComponent> AxlePtr : Axles)
	{
		if (UVehicleAxleAssemblyComponent* Axle = AxlePtr.Get())
		{
			if (Axle->GetAxleSteeringConfig().bAffectedBySteering)
			{
				Num += 1.f;
				Sum += Axle->GetSteeringValue();
			}
		}
	}

	return UVehicleUtilities::SafeDivide(Sum, Num);
}

float UVehicleDriveAssemblyComponent::GetMinTurningRadius()
{
	const float cm2m = 0.01f;

	// Calculate the vehicle's maximum curvature
	float MaxCurvature = 0.f;
	for (TWeakObjectPtr<UVehicleAxleAssemblyComponent> AxlePtr : Axles)
	{
		if (UVehicleAxleAssemblyComponent* Axle = AxlePtr.Get())
		{
			if (Axle->GetAxleSteeringConfig().bAffectedBySteering)
			{
				float SteeringRad = FMath::DegreesToRadians(Axle->GetAxleSteeringConfig().MaxSteeringAngle);
				float AxleWheelBase_m = Axle->GetWheelBase() * cm2m;
				MaxCurvature += FMath::Abs(UVehicleUtilities::SafeDivide(FMath::Tan(SteeringRad), AxleWheelBase_m));
			}
		}
	}
	return UVehicleUtilities::SafeDivide(1.f, MaxCurvature);
}

float UVehicleDriveAssemblyComponent::GetMaxSteeringAngle()
{
	// Obtain the vehicle's maximum steering angle (for mapping to SteeringInput)
	float MaxSteeringAngle = 0.f;
	for (TWeakObjectPtr<UVehicleAxleAssemblyComponent> AxlePtr : Axles)
	{
		if (UVehicleAxleAssemblyComponent* Axle = AxlePtr.Get())
		{
			if (Axle->GetAxleSteeringConfig().bAffectedBySteering)
			{
				MaxSteeringAngle = FMath::Max(FMath::Abs(Axle->GetAxleSteeringConfig().MaxSteeringAngle), MaxSteeringAngle);
			}
		}
	}
	return MaxSteeringAngle;
}

TArray<AActor*> UVehicleDriveAssemblyComponent::GetRayCastHitActors()
{
	TArray<AActor*> HitActors;
	for (TWeakObjectPtr<UVehicleAxleAssemblyComponent>AxlePtr : Axles)
	{
		if (UVehicleAxleAssemblyComponent* Axle = AxlePtr.Get())
		{
			UVehicleWheelComponent* WheelL = nullptr;
			UVehicleWheelComponent* WheelR = nullptr;
			Axle->GetWheels(WheelL, WheelR);
			if (WheelL)
			{
				if (UPrimitiveComponent* HitComponent = WheelL->GetRayCastHitComponent())
				{
					if (AActor* HitActor = HitComponent->GetOwner())
					{
						HitActors.AddUnique(HitActor);
					}
				}
			}
			if (WheelR)
			{
				if (UPrimitiveComponent* HitComponent = WheelR->GetRayCastHitComponent())
				{
					if (AActor* HitActor = HitComponent->GetOwner())
					{
						HitActors.AddUnique(HitActor);
					}
				}
			}
		}
	}
	return HitActors;
}

TArray<UPrimitiveComponent*> UVehicleDriveAssemblyComponent::GetRayCastHitComponents()
{

	TArray<UPrimitiveComponent*> HitComps;
	for (TWeakObjectPtr<UVehicleAxleAssemblyComponent>AxlePtr : Axles)
	{
		if (UVehicleAxleAssemblyComponent* Axle = AxlePtr.Get())
		{
			UVehicleWheelComponent* WheelL = nullptr;
			UVehicleWheelComponent* WheelR = nullptr;
			Axle->GetWheels(WheelL, WheelR);
			if (WheelL)
			{
				if (UPrimitiveComponent* HitComponent = WheelL->GetRayCastHitComponent())
				{
					HitComps.AddUnique(HitComponent);
				}
			}
			if (WheelR)
			{
				if (UPrimitiveComponent* HitComponent = WheelR->GetRayCastHitComponent())
				{
					HitComps.AddUnique(HitComponent);
				}
			}
		}
	}
	return HitComps;
}

bool UVehicleDriveAssemblyComponent::GeneratePowerUnit()
{
	AActor* Owner = GetOwner();
	if (!IsValid(Owner))return false;

	if (!Engine.IsValid())
	{
		if (bUseExistingEngineComponent)
		{
			Engine = UVehicleUtilities::GetComponentByName<UVehicleEngineComponent>(Owner, EngineComponentName);
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
			Clutch = UVehicleUtilities::GetComponentByName<UVehicleClutchComponent>(Owner, ClutchComponentName);
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
			Gearbox = UVehicleUtilities::GetComponentByName<UVehicleGearboxComponent>(Owner, GearboxComponentName);
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
			TransferCase = UVehicleUtilities::GetComponentByName<UVehicleDifferentialComponent>(Owner, TransferCaseComponentName);
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
	if (!Chassis.IsValid())
	{
		Chassis = UVehicleUtilities::FindPhysicalParent(this);
		if (!Chassis.IsValid())
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

	FString ThisName = FString();
	GetName(ThisName);

	int32 n = 0;
	for (FVehicleAxleSpawnTemplate AxleTemplate : AxleTemplates)
	{
		if (!AxleTemplate.bUseExistingComponent)
		{
			// generate new axle
			FName Name = FName(ThisName + "_Axle_"+ FString::FromInt(n));
			UVehicleAxleAssemblyComponent* Axle = UVehicleUtilities::CreateComponentByClass<UVehicleAxleAssemblyComponent>(
					Owner,
					nullptr,
					Name
				);
			if (IsValid(Axle))
			{
				Axle->AttachToComponent(Chassis.Get(), FAttachmentTransformRules::KeepRelativeTransform);
				Axle->SetRelativeLocation(AxleTemplate.AxlePosition);
			}

			if (IsValid(AxleTemplate.AxleClass))
			{
				const UVehicleAxleAssemblyComponent* TemplateAxle = Cast<UVehicleAxleAssemblyComponent>(AxleTemplate.AxleClass->GetDefaultObject());
				if (IsValid(Axle))
				{
					UVehicleAxleAssemblyComponent::CopyAxleConfig(TemplateAxle, Axle);
				}
			}
			
			if (IsValid(Axle))
			{
				Axle->ApplyInitialOverrides(AxleTemplate);

				// initialize
				Axle->InitializeWheels();

				Axles.AddUnique(Axle);
				n++;
			}
		}		
	}
	return n;
}

int UVehicleDriveAssemblyComponent::SearchExistingAxles()
{
	int32 n = 0;
	for (FVehicleAxleSpawnTemplate Template : AxleTemplates)
	{
		AActor* Owner = GetOwner();
		if (Template.bUseExistingComponent && IsValid(Owner))
		{
			UVehicleAxleAssemblyComponent* Axle =
				UVehicleUtilities::GetComponentByName<UVehicleAxleAssemblyComponent>(Owner, Template.AxleComponentName);
			if (IsValid(Axle))
			{
				Axles.AddUnique(Axle);
				n++;
			}
		}
	}
	return n;
}

TArray<FName> UVehicleDriveAssemblyComponent::GetNamesOfAxlesOfOwner()
{
	return UVehicleUtilities::GetNamesOfComponentsOfActor<UVehicleAxleAssemblyComponent>(this);
}

TArray<FName> UVehicleDriveAssemblyComponent::GetNamesOfEnginesOfOwner()
{
	return UVehicleUtilities::GetNamesOfComponentsOfActor<UVehicleEngineComponent>(this);
}

TArray<FName> UVehicleDriveAssemblyComponent::GetNamesOfClutchesOfOwner()
{
	return UVehicleUtilities::GetNamesOfComponentsOfActor<UVehicleClutchComponent>(this);
}

TArray<FName> UVehicleDriveAssemblyComponent::GetNamesOfGearboxesOfOwner()
{
	return UVehicleUtilities::GetNamesOfComponentsOfActor<UVehicleGearboxComponent>(this);
}

TArray<FName> UVehicleDriveAssemblyComponent::GetNamesOfTransferCasesOfOwner()
{
	return UVehicleUtilities::GetNamesOfComponentsOfActor<UVehicleDifferentialComponent>(this);
}

