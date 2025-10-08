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
#include "Net/UnrealNetwork.h" 

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
	WheelCoordinator = UVehicleWheelCoordinatorComponent::FindWheelCoordinator(Carbody);
	if (IsValid(WheelCoordinator))WheelCoordinator->RegisterDriveAssembly(this);

	VehicleAsyncTickComponent = UVehicleAsyncTickComponent::FindVehicleAsyncTickComponent(GetOwner());
	if (IsValid(VehicleAsyncTickComponent))VehicleAsyncTickComponent->Register(this);

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

void UVehicleDriveAssemblyComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	//destory all axles
	if (Axles.Num())
	{
		for (UVehicleAxleAssemblyComponent* AxleToDestroy : Axles)
		{
			if (IsValid(AxleToDestroy) && !AxleToDestroy->IsBeingDestroyed())AxleToDestroy->DestroyComponent();
		}
		Axles.Empty();
	}

	if (IsValid(Engine) && !Engine->IsBeingDestroyed())Engine->DestroyComponent();

	if (IsValid(Clutch) && !Clutch->IsBeingDestroyed())Clutch->DestroyComponent();

	if (IsValid(Gearbox) && !Gearbox->IsBeingDestroyed())Gearbox->DestroyComponent();

	if (IsValid(TransferCase) && !TransferCase->IsBeingDestroyed())TransferCase->DestroyComponent();

	if (IsValid(Carbody))Carbody = nullptr;

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

	//check if there's no need to edit throttle value
	if (Gearbox->GetIsInGear() || !InputAssistConfig.bAutomaticClutch)
	{
		InputValues.Smoothened.Throttle = FVehicleInputAxisConfig::InterpInputValueConstant(InputValues.Smoothened.Throttle, RealThrottleInput, InDeltaTime, InputConfig.Throttle.InterpSpeed);

		InputValues.Smoothened.Throttle *= !(InputAssistConfig.bEVClutchLogic && Gearbox->GetCurrentGearRatio() == 0.f);
	}
	//if not in gear and should rev-match
	else if(Gearbox->GetShouldRevMatch() && FMath::Abs(Speed_kph) > 0.5 && AutoGearboxConfig.bSportMode)
	{
		InputValues.Smoothened.Throttle += SafeDivide(InDeltaTime, Gearbox->Config.ShiftDelay);
		InputValues.Smoothened.Throttle = FMath::Min(InputValues.Smoothened.Throttle, InputAssistConfig.RevMatchMaxThrottle);
	}
	//if not in gear and no rev-matching
	else
	{
		InputValues.Smoothened.Throttle = 0.f;
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
	else if (InputAssistConfig.bAutomaticClutch)
	{
		//check if clutch has to be engaged
		bool bNotInGearAndNotSequential = !(Gearbox->GetIsInGear() || Gearbox->Config.bSequentialGearbox);
		float TargetClutchValue = (InputValues.Raw.Handbrake > 0.9 || bNotInGearAndNotSequential) ? 1.f : InputValues.Raw.Clutch;

		//take engine rpm into account
		float Bias = FMath::GetMappedRangeValueClamped(InputAssistConfig.AutoClutchRange, FVector2D(1, 0), Engine->GetRPM());
		TargetClutchValue = FMath::Clamp(TargetClutchValue + Bias, 0.f, 1.f);
		InputValues.Smoothened.Clutch = FMath::Min(InputValues.Smoothened.Clutch + Bias, TargetClutchValue);

		//get interp speed, when changing gear, clutch should engage faster than gear changes
		FVector2D FinalInterpSpeed;
		FinalInterpSpeed.Y = InputConfig.Clutch.InterpSpeed.Y;
		FinalInterpSpeed.X = bNotInGearAndNotSequential ? SafeDivide(2.f, Gearbox->Config.ShiftDelay) : InputConfig.Clutch.InterpSpeed.X;

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
		RealSteeringInput *= InputConfig.HighSpeedSteeringScale->GetFloatValue(FMath::Abs(Speed_kph));
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

	Gearbox->CalculateSpeedRangeOfEachGear(
		TransferCase->CalculateEffectiveWheelRadius(Axles),
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

	float MinShiftUpFactor = SafeDivide(Engine->NAConfig.EngineIdleRPM, Engine->NAConfig.EngineMaxRPM);
	ShiftFactor = FMath::Max(MinShiftUpFactor, ShiftFactor);

	int32 UnsignedCurrentGear = FMath::Abs(Gearbox->GetCurrentGear());
	int32 StartGear = FMath::Max(UnsignedCurrentGear - AutoGearboxConfig.MaxDownShiftSteps, 1);
	int32 EndGear = FMath::Min(UnsignedCurrentGear + AutoGearboxConfig.MaxUpShiftSteps,
		FMath::Max(Gearbox->Config.NumberOfGears, Gearbox->Config.NumOfReverseGears));
	
	int32 TargetGear = StartGear;
	float UnsignedSpeed = FMath::Abs(Speed_kph);

	//search for target gear
	for (int i = -EndGear; i < -StartGear; i++)
	{
		if (UnsignedSpeed > ShiftFactor * SpeedRangeOfEachGear[-i-1].Y)
		{
			TargetGear = -i;
			break;
		}
	}

	bool GearingUp = FMath::Abs(Gearbox->GetCurrentGear()) < TargetGear;

	//check sign
	if (Speed_kph < 0)TargetGear = -TargetGear;

	if (Gearbox->GetCurrentGear() != TargetGear && !(GearingUp && InputValues.Final.Brake > SMALL_NUMBER))
	{
		if (TargetGear * Gearbox->GetCurrentGear() > 0)
		{
			ShiftToTargetGear(TargetGear, 0.f);
			AutoGearboxCount -= AutoGearboxConfig.AutoShiftCoolDown;
		}
	}
}

//RPC
void UVehicleDriveAssemblyComponent::ServerInputThrottle_Implementation(float InValue)
{
	InputValues.Raw.Throttle = InValue;
	MultiCastInputThrottle(InValue);
}

void UVehicleDriveAssemblyComponent::MultiCastInputThrottle_Implementation(float InValue)
{
	APawn* p = Cast<APawn>(GetOwner());
	if (!p->IsLocallyControlled() && !p->HasAuthority())
	{
		InputValues.Raw.Throttle = InValue;
	}
}

void UVehicleDriveAssemblyComponent::ServerInputBrake_Implementation(float InValue)
{
	InputValues.Raw.Brake = InValue;
	MultiCastInputBrake(InValue);
}

void UVehicleDriveAssemblyComponent::MultiCastInputBrake_Implementation(float InValue)
{
	APawn* p = Cast<APawn>(GetOwner());
	if (!p->IsLocallyControlled() && !p->HasAuthority())
	{
		InputValues.Raw.Brake = InValue;
	}
}

void UVehicleDriveAssemblyComponent::ServerInputClutch_Implementation(float InValue)
{
	InputValues.Raw.Clutch = InValue;
	MultiCastInputClutch(InValue);
}

void UVehicleDriveAssemblyComponent::MultiCastInputClutch_Implementation(float InValue)
{
	APawn* p = Cast<APawn>(GetOwner());
	if (!p->IsLocallyControlled() && !p->HasAuthority())
	{
		InputValues.Raw.Clutch = InValue;
	}
}

void UVehicleDriveAssemblyComponent::ServerInputSteering_Implementation(float InValue)
{
	InputValues.Raw.Steering = InValue;
	MultiCastInputSteering(InValue);
}

void UVehicleDriveAssemblyComponent::MultiCastInputSteering_Implementation(float InValue)
{
	APawn* p = Cast<APawn>(GetOwner());
	if (!p->IsLocallyControlled() && !p->HasAuthority())
	{
		InputValues.Raw.Steering = InValue;
	}
}

void UVehicleDriveAssemblyComponent::ServerInputHandbrake_Implementation(float InValue)
{
	InputValues.Raw.Handbrake = InValue;
	MultiCastInputHandbrake(InValue);
}

void UVehicleDriveAssemblyComponent::MultiCastInputHandbrake_Implementation(float InValue)
{
	APawn* p = Cast<APawn>(GetOwner());
	if (!p->IsLocallyControlled() && !p->HasAuthority())
	{
		InputValues.Raw.Handbrake = InValue;
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

// Called every frame
void UVehicleDriveAssemblyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (IsValid(Engine) && IsValid(Gearbox) && IsValid(Carbody))
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

	if (!(IsValid(Engine) && IsValid(Clutch) && IsValid(Gearbox) && IsValid(TransferCase)))
	{
		UE_LOG(LogTemp, Warning, TEXT("The drivetrain is not complete"));
		return;
	}

	//update engine
	float ClutchTorque = (NumOfDriveAxles > 0) ? Clutch->GetCluchTorque() : 0.f;
	Engine->UpdatePhysics(PhysicsDeltaTime, InputValues.Final.Throttle, ClutchTorque);

	//get gearbox output torque
	float GearboxOutputTorque;
	float GearboxReflectedInertia;
	Gearbox->UpdateOutputShaft(ClutchTorque, GearboxOutputTorque, GearboxReflectedInertia);

	//update transfercase
	float SumAxleInertia;
	float GearboxOutputShaftAngularVelocity;
	NumOfDriveAxles = TransferCase->UpdateTransferCase(
		Axles,
		PhysicsDeltaTime,
		GearboxOutputTorque,
		GearboxReflectedInertia,
		InputValues.Final.Brake,
		InputValues.Final.Handbrake,
		InputValues.Final.Steering,
		InputAssistConfig.bBurnOutAssist
		&& InputValues.Final.Throttle > 0.9
		&& InputValues.Final.Brake > 0.9
		&& Speed_kph < 1.f,
		GearboxOutputShaftAngularVelocity,
		SumAxleInertia
	);

	//get gearbox inputshaft angular velocity
	float GearboxInputShaftVelocity;
	float SumReflectedInertia;
	float CurrentGearboxRatio;
	float FirstGearReflectedInertia;
	Gearbox->UpdateInputShaft(GearboxOutputShaftAngularVelocity, SumAxleInertia,
		GearboxInputShaftVelocity, SumReflectedInertia, CurrentGearboxRatio, FirstGearReflectedInertia);

	//get clutch torque for next frame
	Clutch->UpdatePhysics(PhysicsDeltaTime, InputValues.Final.Clutch,
		GearboxInputShaftVelocity, SumReflectedInertia, CurrentGearboxRatio, FirstGearReflectedInertia, Engine);

	//get velocity
	NumOfWheelsOnGround = 0;
	FVector SumLocalLinVel = FVector(0.f);
	FVector SumWorldLinVel = FVector(0.f);
	for (UVehicleAxleAssemblyComponent* Axle : Axles)
	{
		if (!IsValid(Axle))continue;

		//calculate linear velocity
		FVector LocalVel;
		FVector WorldVel;
		NumOfWheelsOnGround += Axle->GetNumOfWheelsOnGround();
		Axle->GetLinearVelocity(LocalVel, WorldVel);
		SumLocalLinVel += LocalVel;
		SumWorldLinVel += WorldVel;
	}
	bIsInAir = !NumOfWheelsOnGround;
	float NumGroundedWheelsInv = SafeDivide(1.f, (float)NumOfWheelsOnGround);
	LocalLinearVelocity = SumLocalLinVel * 2.f * NumGroundedWheelsInv;
	WorldLinearVelocity = SumWorldLinVel * 2.f * NumGroundedWheelsInv;
	LocalVelocityClamped = ClampToZero(LocalLinearVelocity, 0.1);
	Speed_kph = LocalLinearVelocity.X * 3.6;
}

void UVehicleDriveAssemblyComponent::InputThrottle(float InValue)
{
	if (!GetOwner()->HasAuthority())InputValues.Raw.Throttle = InValue;
	ServerInputThrottle(InValue);
}

void UVehicleDriveAssemblyComponent::InputBrake(float InValue)
{
	if (!GetOwner()->HasAuthority())InputValues.Raw.Brake = InValue;
	ServerInputBrake(InValue);
}

void UVehicleDriveAssemblyComponent::InputClutch(float InValue)
{
	if (!GetOwner()->HasAuthority())InputValues.Raw.Clutch = InValue;
	ServerInputClutch(InValue);
}

void UVehicleDriveAssemblyComponent::InputSteering(float InValue)
{
	if (!GetOwner()->HasAuthority())InputValues.Raw.Steering = InValue;
	ServerInputSteering(InValue);
}

void UVehicleDriveAssemblyComponent::InputHandbrake(float InValue)
{
	if (!GetOwner()->HasAuthority())InputValues.Raw.Handbrake = InValue;
	ServerInputHandbrake(InValue);
}

void UVehicleDriveAssemblyComponent::ShiftToTargetGear(int32 InTargetGear, float InAutoShiftCoolDown, bool bImmediate)
{
	if (IsValid(Gearbox)) 
	{
		AutoGearboxCount = -InAutoShiftCoolDown;

		//Client predict
		if (!GetOwner()->HasAuthority())
		{
			Gearbox->ShiftToTargetGear(InTargetGear, bImmediate);
		}
	}

	ServerShiftToTargetGear(InTargetGear, bImmediate);
}

void UVehicleDriveAssemblyComponent::ShiftUp(float InAutoShiftCoolDown, bool bImmediate)
{
	if (IsValid(Gearbox))
	{
		ShiftToTargetGear(Gearbox->GetCurrentGear() + 1, InAutoShiftCoolDown, bImmediate);
	}
}

void UVehicleDriveAssemblyComponent::ShiftDown(float InAutoShiftCoolDown, bool bImmediate)
{
	if (IsValid(Gearbox))
	{
		ShiftToTargetGear(Gearbox->GetCurrentGear() - 1, InAutoShiftCoolDown, bImmediate);
	}
}

EEngineState UVehicleDriveAssemblyComponent::StartVehicleEngine()
{
	return Engine->StartVehicleEngine();
}

EEngineState UVehicleDriveAssemblyComponent::ShutVehicleEngine()
{
	return Engine->ShutVehicleEngine();
}

void UVehicleDriveAssemblyComponent::UpdateDriftCamera(USceneComponent* InSpringArm, float InPitch, float InDriftCamRate, float InDriftCamInterpSpeed, float InDriftCamStartSpeed_mps)
{
	if (LocalLinearVelocity.SquaredLength() > FMath::Square(InDriftCamStartSpeed_mps))
	{
		FRotator CurrentCamRot = InSpringArm->GetRelativeRotation();

		FVector ScaledLinearVelocity = LocalVelocityClamped;
		ScaledLinearVelocity.Y *= InDriftCamRate;

		FRotator TargetRotator = FRotationMatrix::MakeFromX(ScaledLinearVelocity).Rotator();
		TargetRotator.Pitch = InPitch;

		FRotator NewRot = FMath::RInterpTo(CurrentCamRot, TargetRotator, GetWorld()->DeltaTimeSeconds, InDriftCamInterpSpeed);
		NewRot.Roll = 0;

		InSpringArm->SetRelativeRotation(NewRot);
	}
}

void UVehicleDriveAssemblyComponent::RotateCamera(USceneComponent* InSpringArm, FVector2D InMouseInput, bool bInvertYAxis, float InMaxPitch)
{
	FRotator Rot = InSpringArm->GetRelativeRotation();
	FRotator NewRot;

	NewRot.Roll = 0;
	NewRot.Pitch = FMath::Clamp(Rot.Pitch + InMouseInput.Y, -InMaxPitch, InMaxPitch);
	NewRot.Yaw = Rot.Yaw + InMouseInput.X;

	InSpringArm->SetRelativeRotation(NewRot);
}

TArray<UVehicleWheelComponent*> UVehicleDriveAssemblyComponent::GetWheels()
{
	TArray<UVehicleWheelComponent*> Wheels;

	for (UVehicleAxleAssemblyComponent* Axle : Axles)
	{
		if (!Axle)break;

		UVehicleWheelComponent* LeftWheel;
		UVehicleWheelComponent* RightWheel;

		Axle->GetWheels(LeftWheel, RightWheel);

		Wheels.Add(LeftWheel);
		Wheels.Add(RightWheel);
	}
	return Wheels;
}

void UVehicleDriveAssemblyComponent::DestroyTargetAxle(UVehicleAxleAssemblyComponent* InTargetAxle)
{
	Axles.Remove(InTargetAxle);
	InTargetAxle->DestroyComponent();
}

bool UVehicleDriveAssemblyComponent::GeneratePowerUnit()
{
	if (!IsValid(Engine))
	{
		if (EngineConfig)
		{
			Engine = Cast<UVehicleEngineComponent>
				(GetOwner()->AddComponentByClass(
					EngineConfig, false, FTransform(), false));
		}
		else
		{
			Engine = Cast<UVehicleEngineComponent>
				(GetOwner()->AddComponentByClass(
					UVehicleEngineComponent::StaticClass(), false, FTransform(), false));
		}
	}

	if (!IsValid(Clutch))
	{
		if (ClutchConfig)
		{
			Clutch = Cast<UVehicleClutchComponent>
				(GetOwner()->AddComponentByClass(
					ClutchConfig, false, FTransform(), false));
		}
		else
		{
			Clutch = Cast<UVehicleClutchComponent>
				(GetOwner()->AddComponentByClass(
					UVehicleClutchComponent::StaticClass(), false, FTransform(), false));
		}
	}

	if (!IsValid(Gearbox))
	{
		if (GearboxConfig)
		{
			Gearbox = Cast<UVehicleGearboxComponent>
				(GetOwner()->AddComponentByClass(
					GearboxConfig, false, FTransform(), false));
		}
		else
		{
			Gearbox = Cast<UVehicleGearboxComponent>
				(GetOwner()->AddComponentByClass(
					UVehicleGearboxComponent::StaticClass(), false, FTransform(), false));
		}
	}

	if (!IsValid(TransferCase))
	{
		if (TransferCaseConfig)
		{
			TransferCase = Cast<UVehicleDifferentialComponent>
				(GetOwner()->AddComponentByClass(
					TransferCaseConfig, false, FTransform(), false));
		}
		else
		{
			TransferCase = Cast<UVehicleDifferentialComponent>
				(GetOwner()->AddComponentByClass(
					UVehicleDifferentialComponent::StaticClass(), false, FTransform(), false));
			TransferCase->Config.GearRatio = 1.f;
		}
	}

	return IsValid(Engine) && IsValid(Clutch) && IsValid(Gearbox) && IsValid(TransferCase);
}

int UVehicleDriveAssemblyComponent::GenerateAxles()
{
	if (!IsValid(Carbody))return -1;

	//if there are axles, destroy them
	if (Axles.Num() > 0)
	{
		for (UVehicleAxleAssemblyComponent* Axle : Axles)
		{
			if (IsValid(Axle))Axle->DestroyComponent();
		}
		Axles.Empty();
	}

	int32 n = 0;
	for (FAxleAssemblyConfig AxleConfig : AxleConfigs)
	{
		TSubclassOf<UVehicleAxleAssemblyComponent> AxleClass = AxleConfig.AxleConfig;
		UVehicleAxleAssemblyComponent* Axle;
		if (AxleClass)
		{
			Axle = Cast<UVehicleAxleAssemblyComponent>
				(GetOwner()->AddComponentByClass(AxleClass, true, FTransform(FQuat(), AxleConfig.AxlePosition, FVector(1)), true));
		}
		else
		{
			Axle = Cast<UVehicleAxleAssemblyComponent>
				(GetOwner()->AddComponentByClass(UVehicleAxleAssemblyComponent::StaticClass(), true, FTransform(FQuat(), AxleConfig.AxlePosition, FVector(1)), true));
		}
		Axle->AttachToComponent(Carbody, FAttachmentTransformRules::KeepRelativeTransform);

		if (IsValid(Axle))
		{
			//override these before registering the axle component
			if (AxleConfig.WheelOverride)Axle->WheelConfig = AxleConfig.WheelOverride;
			if (AxleConfig.DifferentialOverride)Axle->DifferentialConfig = AxleConfig.DifferentialOverride;
			if (AxleConfig.TrackWidthOverride >= 0)Axle->AxleConfig.TrackWidth = AxleConfig.TrackWidthOverride;
			if (AxleConfig.bDiasbleSteering)Axle->AxleSteeringConfig.bAffectedBySteering = false;
			if (AxleConfig.bDisableHandbrake)Axle->AxleConfig.bAffectedByHandbrake = false;
			if (AxleConfig.bDisableTractionControl)Axle->TCSConfig.bTractionControlSystemEnabled = false;
			if (AxleConfig.TorqueWeightOverride >= 0)Axle->AxleConfig.TorqueWeight = AxleConfig.TorqueWeightOverride;

			Axles.Add(Axle);
			n++;
		}

		GetOwner()->FinishAddComponent(Axle, true, FTransform(FQuat(), AxleConfig.AxlePosition, FVector(1)));
	}
	return n;
}

float UVehicleDriveAssemblyComponent::SafeDivide(float a, float b)
{
	return (FMath::IsNearlyZero(b)) ? 0.0f : a / b;
}

FVector UVehicleDriveAssemblyComponent::SafeDivide(FVector a, float b)
{
	return (FMath::IsNearlyZero(b)) ? FVector(0.f) : a / b;
}

float UVehicleDriveAssemblyComponent::ClampToZero(float a, float Tolerance)
{
	if (FMath::Abs(a) > Tolerance)return a;
	return 0.0f;
}

FVector UVehicleDriveAssemblyComponent::ClampToZero(FVector v, float Tolerance)
{
	FVector VReturn;
	VReturn.X = ClampToZero(v.X, Tolerance);
	VReturn.Y = ClampToZero(v.Y, Tolerance);
	VReturn.Z = ClampToZero(v.Z, Tolerance);
	return VReturn;
}
