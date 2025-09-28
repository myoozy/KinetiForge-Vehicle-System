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

// Sets default values for this component's properties
UVehicleDriveAssemblyComponent::UVehicleDriveAssemblyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PostPhysics;

	// ...

	//load default curves
	if (!InputConfig.SteeringCurve)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> DefaultSteeringCurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultSteeringCurve.DefaultSteeringCurve'")
		);
		if (DefaultSteeringCurveObj.Succeeded())
		{
			InputConfig.SteeringCurve = DefaultSteeringCurveObj.Object;
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

	GeneratePowerUnit();

	//initialize autogearbox timer
	AutoShiftTimer = FMath::FRandRange(-AutoGearboxConfig.AutomaticGearboxRefreshTime, 0.0f) + AutoGearboxConfig.AutomaticGearboxRefreshTime;

}

void UVehicleDriveAssemblyComponent::OnRegister()
{
	Super::OnRegister();
	//...
	Carbody = UVehicleWheelCoordinatorComponent::FindPhysicalParent(this);
	GetOwner()->bRunConstructionScriptOnDrag = false;	//to improve performance
	GenerateAxles();
}

void UVehicleDriveAssemblyComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	//destory all axles
	if (Axles.Num())
	{
		for (UVehicleAxleAssemblyComponent* TempAxle : Axles)
		{
			if (IsValid(TempAxle) && !TempAxle->IsBeingDestroyed())TempAxle->DestroyComponent();
		}
		Axles.Empty();
	}

	if (Engine && !Engine->IsBeingDestroyed())Engine->DestroyComponent();

	if (Clutch && !Clutch->IsBeingDestroyed())Clutch->DestroyComponent();

	if (Gearbox && !Gearbox->IsBeingDestroyed())Gearbox->DestroyComponent();

	if (TransferCase && !TransferCase->IsBeingDestroyed())TransferCase->DestroyComponent();

	if (Carbody.IsValid())Carbody = nullptr;

	InputConfig.ThrottleCurve = nullptr;
	InputConfig.BrakeCurve = nullptr;
	InputConfig.ClutchCurve = nullptr;
	InputConfig.HandbrakeCurve = nullptr;
	InputConfig.SteeringCurve = nullptr;
	InputConfig.HighSpeedSteeringScale = nullptr;

	//...
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UVehicleDriveAssemblyComponent::UpdateInput(float InDeltaTime)
{
	UpdateThrottle(InDeltaTime);
	UpdateBrake(InDeltaTime);
	UpdateClutch(InDeltaTime);
	UpdateSteering(InDeltaTime);
}

void UVehicleDriveAssemblyComponent::UpdateThrottle(float InDeltaTime)
{
	float RealThrottleInput = InputValues.bSwitchThrottleAndBrake ? InputValues.BrakeInput : InputValues.ThrottleInput;

	//check if there's no need to edit throttle value
	if (Gearbox->GetIsInGear() || !InputAssistConfig.bAutomaticClutch)
	{
		InputValues.ThrottleValue = InterpInputValueConstant(InputValues.ThrottleValue, RealThrottleInput, InDeltaTime, InputConfig.ThrottleInterpSpeed);

		InputValues.ThrottleValue *= !(InputAssistConfig.bEVClutchLogic && Gearbox->GetCurrentGearRatio() == 0.f);
	}
	//if not in gear and should rev-match
	else if(Gearbox->GetShouldRavMatch() && FMath::Abs(Speed_kph) > 0.5 && AutoGearboxConfig.bSportMode)
	{
		InputValues.ThrottleValue += SafeDivide(InDeltaTime, Gearbox->Config.ShiftDelay);
		InputValues.ThrottleValue = FMath::Min(InputValues.ThrottleValue, InputAssistConfig.RevMatchMaxThrottle);
	}
	//if not in gear and no rev-matching
	else
	{
		InputValues.ThrottleValue = 0.f;
	}

	if (InputConfig.ThrottleCurve)
	{
		InputValues.RealThrottleValue = InputConfig.ThrottleCurve->GetFloatValue(InputValues.ThrottleValue);
	}
	else
	{
		InputValues.RealThrottleValue = InputValues.ThrottleValue;
	}
}

void UVehicleDriveAssemblyComponent::UpdateBrake(float InDeltaTime)
{
	float RealThrottleInput = InputValues.bSwitchThrottleAndBrake ? InputValues.BrakeInput : InputValues.ThrottleInput;
	float RealBrakeInput = InputValues.bSwitchThrottleAndBrake ? InputValues.ThrottleInput : InputValues.BrakeInput;

	//update brake
	InputValues.BrakeValue = InterpInputValueConstant(InputValues.BrakeValue, RealBrakeInput, InDeltaTime, InputConfig.BrakeInterpSpeed);

	if (InputConfig.BrakeCurve)
	{
		InputValues.RealBrakeValue = InputConfig.BrakeCurve->GetFloatValue(InputValues.BrakeValue);
	}
	else
	{
		InputValues.RealBrakeValue = InputValues.BrakeValue;
	}

	//update handbrake
	float HandbrakeTarget = InputValues.HandbrakeInput;
	if (InputAssistConfig.bAutoHold
		&& FMath::Abs(LocalLinearVelocity.X) < 0.1
		&& RealThrottleInput < SMALL_NUMBER
		&& !bIsInAir)HandbrakeTarget = 1.f;
	InputValues.HandbrakeValue = InterpInputValueConstant(InputValues.HandbrakeValue, HandbrakeTarget, InDeltaTime, InputConfig.HandbrakeInterpSpeed);

	if (InputConfig.HandbrakeCurve)
	{
		InputValues.RealHandbrakeValue = InputConfig.HandbrakeCurve->GetFloatValue(InputValues.HandbrakeValue);
	}
	else
	{
		InputValues.RealHandbrakeValue = InputValues.HandbrakeValue;
	}
}

void UVehicleDriveAssemblyComponent::UpdateClutch(float InDeltaTime)
{
	if (InputAssistConfig.bEVClutchLogic)
	{
		InputValues.ClutchValue = 0.f;
	}
	else if (InputAssistConfig.bAutomaticClutch)
	{
		//check if clutch has to be engaged
		bool bNotInGearAndNotSequential = !(Gearbox->GetIsInGear() || Gearbox->Config.bSequentialGearbox);
		float TargetClutchValue = (InputValues.HandbrakeInput > 0.9 || bNotInGearAndNotSequential) ? 1.f : InputValues.ClutchInput;

		//take engine rpm into account
		float Bias = FMath::GetMappedRangeValueClamped(InputAssistConfig.AutoClutchRange, FVector2D(1, 0), Engine->GetRPM());
		TargetClutchValue = FMath::Clamp(TargetClutchValue + Bias, 0.f, 1.f);
		InputValues.ClutchValue = FMath::Min(InputValues.ClutchValue + Bias, TargetClutchValue);

		//get interp speed, when changing gear, clutch should engage faster than gear changes
		FVector2D TempInterpSpeed;
		TempInterpSpeed.Y = InputConfig.ClutchInterpSpeed.Y;
		TempInterpSpeed.X = bNotInGearAndNotSequential ? SafeDivide(2.f, Gearbox->Config.ShiftDelay) : InputConfig.ClutchInterpSpeed.X;

		InputValues.ClutchValue = InterpInputValueConstant(InputValues.ClutchValue, TargetClutchValue, InDeltaTime, TempInterpSpeed);
	}
	else
	{
		InputValues.ClutchValue = InterpInputValueConstant(InputValues.ClutchValue, InputValues.ClutchInput, InDeltaTime, InputConfig.ClutchInterpSpeed);
	}

	if (InputConfig.ClutchCurve)
	{
		InputValues.RealClutchValue = InputConfig.ClutchCurve->GetFloatValue(InputValues.ClutchValue);
	}
	else
	{
		InputValues.RealClutchValue = InputValues.ClutchValue;
	}
}

void UVehicleDriveAssemblyComponent::UpdateSteering(float InDeltaTime)
{
	float RealSteeringInput = InputValues.SteeringInput;

	//reduce the steering value when driving at high speed
	if (InputConfig.HighSpeedSteeringScale)
	{
		RealSteeringInput *= InputConfig.HighSpeedSteeringScale->GetFloatValue(FMath::Abs(Speed_kph));
	}

	InputValues.SteeringValue = InterpInputValueConstant(InputValues.SteeringValue, RealSteeringInput, InDeltaTime, InputConfig.SteeringInterpSpeed);

	if (InputConfig.SteeringCurve)
	{
		float CurveValue = InputConfig.SteeringCurve->GetFloatValue(FMath::Abs(InputValues.SteeringValue));
		InputValues.RealSteeringValue = CurveValue * FMath::Sign(InputValues.SteeringValue);
	}
	else
	{
		InputValues.RealSteeringValue = InputValues.SteeringValue;
	}
}

void UVehicleDriveAssemblyComponent::UpdateAutomaticGearbox(float InDeltaTime)
{	
	AutoShiftTimer += InDeltaTime;
	//reset timer
	//保留误差
	bool AutoGearboxTimerOverFlowed = AutoShiftTimer > 0.f;
	AutoShiftTimer -= AutoGearboxConfig.AutomaticGearboxRefreshTime * AutoGearboxTimerOverFlowed;

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
			if (InputValues.ThrottleInput > SMALL_NUMBER)
			{
				Gearbox->ShiftToTargetGear(1, AutoGearboxConfig.bArcadeShiftInstant);
				AutoShiftTimer += AutoGearboxConfig.AutomaticGearboxRefreshTime * AutoGearboxConfig.bArcadeShiftInstant;
				return;
			}
			else if (InputValues.BrakeInput > SMALL_NUMBER && InputValues.ThrottleInput <= SMALL_NUMBER)
			{
				Gearbox->ShiftToTargetGear(-1, AutoGearboxConfig.bArcadeShiftInstant);
				AutoShiftTimer += AutoGearboxConfig.AutomaticGearboxRefreshTime * AutoGearboxConfig.bArcadeShiftInstant;
				return;
			}
		}
		//if the vehicle is almost stopped, or going in wrong direction
		else if (LinearVelocityX < 1 || (LocalLinearVelocity.X * Gearbox->GetCurrentGear()) < 0)
		{
			if (InputValues.ThrottleInput > SMALL_NUMBER)
			{
				Gearbox->ShiftToTargetGear(1, AutoGearboxConfig.bArcadeShiftInstant);
				AutoShiftTimer += AutoGearboxConfig.AutomaticGearboxRefreshTime * AutoGearboxConfig.bArcadeShiftInstant;
				return;
			}
			else if (InputValues.BrakeInput > SMALL_NUMBER)
			{
				Gearbox->ShiftToTargetGear(-1, AutoGearboxConfig.bArcadeShiftInstant);
				AutoShiftTimer += AutoGearboxConfig.AutomaticGearboxRefreshTime * AutoGearboxConfig.bArcadeShiftInstant;
				return;
			}
			else
			{
				Gearbox->ShiftToTargetGear(0, AutoGearboxConfig.bArcadeShiftInstant);
				AutoShiftTimer += AutoGearboxConfig.AutomaticGearboxRefreshTime * AutoGearboxConfig.bArcadeShiftInstant;
				return;
			}
		}
	}

	//if in N gear, don't update
	if (!Gearbox->GetCurrentGear())return;

	SpeedRangeOfEachGear = CalculateSpeedRangeOfEachGear();

	//如果急刹车时，也希望尽早降档，发动机协助制动
	float Input = FMath::Max(InputValues.ThrottleValue, InputValues.BrakeValue);

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

	if (Gearbox->GetCurrentGear() != TargetGear && !(GearingUp && InputValues.BrakeValue > SMALL_NUMBER))
	{
		if (TargetGear * Gearbox->GetCurrentGear() > 0)
		{
			Gearbox->ShiftToTargetGear(TargetGear);
			AutoShiftTimer -= AutoGearboxConfig.AutoShiftCoolDown;
		}
	}
}

// Called every frame
void UVehicleDriveAssemblyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (IsValid(Engine) && IsValid(Gearbox) && Carbody.IsValid())
	{
		UpdateInput(DeltaTime);

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
	TRACE_CPUPROFILER_EVENT_SCOPE(UpdateVehiclePhysics);

	if (!(IsValid(Engine) && IsValid(Clutch) && IsValid(Gearbox) && IsValid(TransferCase) && Carbody.IsValid()))return;

	PhysicsDeltaTime = InDeltaTime;

	//update engine
	float EngineLoadTorque = (NumOfDriveAxles > 0) ? Clutch->GetCluchTorque() : 0.f;
	Engine->UpdatePhysics(PhysicsDeltaTime, InputValues.RealThrottleValue, EngineLoadTorque);

	//get gearbox output torque
	float GearboxOutputTorque;
	float GearboxReflectedInertia;
	Gearbox->UpdateOutputShaft(Clutch->GetCluchTorque(), GearboxOutputTorque, GearboxReflectedInertia);

	//update transfercase
	float SumAxleInertia;
	float GearboxOutputShaftAngularVelocity;
	NumOfDriveAxles = TransferCase->UpdateTransferCase(
		Axles,
		PhysicsDeltaTime,
		GearboxOutputTorque,
		GearboxReflectedInertia,
		InputValues.RealBrakeValue,
		InputValues.RealHandbrakeValue,
		InputValues.RealSteeringValue,
		InputAssistConfig.bBurnOutAssist
		&& InputValues.RealThrottleValue > 0.9
		&& InputValues.RealBrakeValue > 0.9
		&& Speed_kph < 1.f,
		GearboxOutputShaftAngularVelocity,
		SumAxleInertia
	);

	//get velocity
	NumOfWheelsOnGround = 0.f;
	FVector SumLocalLinVel = FVector(0.f);
	FVector SumWorldLinVel = FVector(0.f);
	for (UVehicleAxleAssemblyComponent* TempAxle : Axles)
	{
		if (!IsValid(TempAxle))continue;

		//calculate linear velocity
		FVector TempLocalVel;
		FVector TempWorldVel;
		NumOfWheelsOnGround += TempAxle->GetNumOfWheelsOnGround();
		TempAxle->GetLinearVelocity(TempLocalVel, TempWorldVel);
		SumLocalLinVel += TempLocalVel;
		SumWorldLinVel += TempWorldVel;
	}
	bIsInAir = !NumOfWheelsOnGround;
	float NumGroundedWheelsInv = SafeDivide(1.f, (float)NumOfWheelsOnGround);
	LocalLinearVelocity = SumLocalLinVel * 2.f * NumGroundedWheelsInv;
	WorldLinearVelocity = SumWorldLinVel * 2.f * NumGroundedWheelsInv;
	LocalVelocityClamped = ClampToZero(LocalLinearVelocity, 0.1);
	Speed_kph = LocalLinearVelocity.X * 3.6;

	//get gearbox inputshaft angular velocity
	float GearboxInputShaftVelocity;
	float SumReflectedInertia;
	float CurrentGearboxRatio;
	float FirstGearReflectedInertia;
	Gearbox->UpdateInputShaft(GearboxOutputShaftAngularVelocity, SumAxleInertia,
		GearboxInputShaftVelocity, SumReflectedInertia, CurrentGearboxRatio, FirstGearReflectedInertia);

	//get clutch torque for next frame
	Clutch->UpdatePhysics(PhysicsDeltaTime, InputValues.RealClutchValue,
		GearboxInputShaftVelocity, SumReflectedInertia, CurrentGearboxRatio, FirstGearReflectedInertia, Engine);
}

void UVehicleDriveAssemblyComponent::InputThrottle(float InValue)
{
	InputValues.ThrottleInput = InValue;
}

void UVehicleDriveAssemblyComponent::InputBrake(float InValue)
{
	InputValues.BrakeInput = InValue;
}

void UVehicleDriveAssemblyComponent::InputClutch(float InValue)
{
	InputValues.ClutchInput = InValue;
}

void UVehicleDriveAssemblyComponent::InputSteering(float InValue)
{
	InputValues.SteeringInput = InValue;
}

void UVehicleDriveAssemblyComponent::InputHandbrake(float InValue)
{
	InputValues.HandbrakeInput = InValue;
}

void UVehicleDriveAssemblyComponent::ShiftToTargetGear(int32 InTargetGear, float InAutoShiftCoolDown, bool bImmediate)
{
	AutoShiftTimer = -InAutoShiftCoolDown;
	Gearbox->ShiftToTargetGear(InTargetGear, bImmediate);
}

void UVehicleDriveAssemblyComponent::ShiftUp(float InAutoShiftCoolDown, bool bImmediate)
{
	AutoShiftTimer = -InAutoShiftCoolDown;
	Gearbox->ShiftToTargetGear(Gearbox->GetCurrentGear() + 1, bImmediate);
}

void UVehicleDriveAssemblyComponent::ShiftDown(float InAutoShiftCoolDown, bool bImmediate)
{
	AutoShiftTimer = -InAutoShiftCoolDown;
	Gearbox->ShiftToTargetGear(Gearbox->GetCurrentGear() - 1, bImmediate);
}

EEngineState UVehicleDriveAssemblyComponent::StartVehicleEngine()
{
	return Engine->StartVehicleEngine();
}

EEngineState UVehicleDriveAssemblyComponent::ShutVehicleEngine()
{
	return Engine->ShutVehicleEngine();
}

float UVehicleDriveAssemblyComponent::InterpInputValueConstant(float Current, float InTarget, float InDeltaTime, FVector2D InInterpSpeed)
{
	float InterpSpeed;
	if (InTarget < SMALL_NUMBER)	//if released
	{
		InterpSpeed = InInterpSpeed.Y;
	}
	else
	{
		InterpSpeed = InInterpSpeed.X;
	}

	if (InterpSpeed <= 0)
	{
		return InTarget;	//if interp speed is not valid, then return the target
	}
	else
	{
		return FMath::FInterpConstantTo(Current, InTarget, InDeltaTime, InterpSpeed);
	}
}

float UVehicleDriveAssemblyComponent::InterpInputValue(float Current, float InTarget, float InDeltaTime, FVector2D InInterpSpeed)
{
	float InterpSpeed;
	if (InTarget < SMALL_NUMBER)	//if released
	{
		InterpSpeed = InInterpSpeed.Y;
	}
	else
	{
		InterpSpeed = InInterpSpeed.X;
	}

	if (InterpSpeed <= 0)
	{
		return InTarget;	//if interp speed is not valid, then return the target
	}
	else
	{
		return FMath::FInterpTo(Current, InTarget, InDeltaTime, InterpSpeed);
	}
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
	TArray<UVehicleWheelComponent*> TempWheels;

	for (UVehicleAxleAssemblyComponent* TempAxle : Axles)
	{
		if (!TempAxle)break;

		UVehicleWheelComponent* TempLeftWheel;
		UVehicleWheelComponent* TempRightWheel;

		TempAxle->GetWheels(TempLeftWheel, TempRightWheel);

		TempWheels.Add(TempLeftWheel);
		TempWheels.Add(TempRightWheel);
	}
	return TempWheels;
}

void UVehicleDriveAssemblyComponent::DestroyTargetAxle(UVehicleAxleAssemblyComponent* InTargetAxle)
{
	Axles.Remove(InTargetAxle);
	InTargetAxle->DestroyComponent();
}

TArray<FVector2D> UVehicleDriveAssemblyComponent::CalculateSpeedRangeOfEachGear()
{
	if (Gearbox->IsGearDateDirty())Gearbox->CalculateGearRatios();

	TArray<FVector2D> SpeedRange;
	int32 NumGears = FMath::Max(Gearbox->Config.NumberOfGears, Gearbox->Config.NumOfReverseGears);
	SpeedRange.SetNum(NumGears + 1);
	SpeedRange[0] = FVector2D(0);

	float avgOmega = 0.f;
	if (IsValid(TransferCase))avgOmega = TransferCase->CalculateEffectiveWheelRadius(Axles);
	float RPMToRad = PI * 0.0333333333333f;
	float avgRPM = avgOmega * RPMToRad * 0.036;

	for (int32 i = 1; i <= NumGears; i++)
	{
		FVector2D TempRange;
		float TempGearRatio = Gearbox->GetGearRatio(i);
		TempRange.X = SafeDivide(Engine->NAConfig.EngineIdleRPM * avgRPM, TempGearRatio);
		TempRange.Y = SafeDivide(Engine->NAConfig.EngineMaxRPM * avgRPM, TempGearRatio);

		SpeedRange[i] = TempRange;
	}

	return SpeedRange;
}

bool UVehicleDriveAssemblyComponent::GeneratePowerUnit()
{
	if (!IsValid(Engine))
	{
		if (EngineConfig)
		{
			Engine = Cast<UVehicleEngineComponent>(GetOwner()->AddComponentByClass(EngineConfig, false, FTransform(), false));
		}
		else
		{
			Engine = NewObject<UVehicleEngineComponent>(this);
			Engine->RegisterComponent();
		}
	}

	if (!IsValid(Clutch))
	{
		if (ClutchConfig)
		{
			Clutch = Cast<UVehicleClutchComponent>(GetOwner()->AddComponentByClass(ClutchConfig, false, FTransform(), false));
		}
		else
		{
			Clutch = NewObject<UVehicleClutchComponent>(this);
			Clutch->RegisterComponent();
		}
	}

	if (!IsValid(Gearbox))
	{
		if (GearboxConfig)
		{
			Gearbox = Cast<UVehicleGearboxComponent>(GetOwner()->AddComponentByClass(GearboxConfig, false, FTransform(), false));
		}
		else
		{
			Gearbox = NewObject<UVehicleGearboxComponent>(this);
			Gearbox->RegisterComponent();
		}
	}

	if (!IsValid(TransferCase))
	{
		if (TransferCaseConfig)
		{
			TransferCase = Cast<UVehicleDifferentialComponent>(GetOwner()->AddComponentByClass(TransferCaseConfig, false, FTransform(), false));
		}
		else
		{
			TransferCase = NewObject<UVehicleDifferentialComponent>(this);
			TransferCase->Config.GearRatio = 1.f;	//set gear ratio to 1
			TransferCase->RegisterComponent();
		}
	}

	return IsValid(Engine) && IsValid(Clutch) && IsValid(Gearbox) && IsValid(TransferCase);
}

int UVehicleDriveAssemblyComponent::GenerateAxles()
{
	if (!Carbody.IsValid())return -1;

	//if there are axles, destroy them
	if (Axles.Num() > 0)
	{
		for (UVehicleAxleAssemblyComponent* TempAxle : Axles)
		{
			if (IsValid(TempAxle))TempAxle->DestroyComponent();
		}
		Axles.Empty();
	}

	int32 n = 0;
	for (FAxleAssemblyConfig TempAxleConfig : AxleConfigs)
	{
		TSubclassOf<UVehicleAxleAssemblyComponent> TempAxleClass = TempAxleConfig.AxleConfig;
		UVehicleAxleAssemblyComponent* TempAxle;
		if (TempAxleClass)
		{
			TempAxle = Cast<UVehicleAxleAssemblyComponent>
				(GetOwner()->AddComponentByClass(TempAxleClass, true, FTransform(FQuat(), TempAxleConfig.AxlePosition, FVector(1)), true));
		}
		else
		{
			TempAxle = Cast<UVehicleAxleAssemblyComponent>
				(GetOwner()->AddComponentByClass(UVehicleAxleAssemblyComponent::StaticClass(), true, FTransform(FQuat(), TempAxleConfig.AxlePosition, FVector(1)), true));
		}
		TempAxle->AttachToComponent(Carbody.Get(), FAttachmentTransformRules::KeepRelativeTransform);

		if (IsValid(TempAxle))
		{
			//override these before registering the axle component
			if (TempAxleConfig.WheelOverride)TempAxle->WheelConfig = TempAxleConfig.WheelOverride;
			if (TempAxleConfig.DifferentialOverride)TempAxle->DifferentialConfig = TempAxleConfig.DifferentialOverride;
			if (TempAxleConfig.TrackWidthOverride >= 0)TempAxle->AxleConfig.TrackWidth = TempAxleConfig.TrackWidthOverride;
			if (TempAxleConfig.bDiasbleSteering)TempAxle->AxleSteeringConfig.bAffectedBySteering = false;
			if (TempAxleConfig.bDisableHandbrake)TempAxle->AxleConfig.bAffectedByHandbrake = false;
			if (TempAxleConfig.bDisableTractionControl)TempAxle->TCSConfig.bTractionControlSystemEnabled = false;
			if (TempAxleConfig.TorqueWeightOverride >= 0)TempAxle->AxleConfig.TorqueWeight = TempAxleConfig.TorqueWeightOverride;

			Axles.Add(TempAxle);
			n++;
		}

		GetOwner()->FinishAddComponent(TempAxle, true, FTransform(FQuat(), TempAxleConfig.AxlePosition, FVector(1)));
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
