// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)


#include "VehicleAxleAssemblyComponent.h"
#include "VehicleWheelComponent.h"
#include "VehicleDifferentialComponent.h"
#include "VehicleWheelCoordinatorComponent.h"
#include "VehicleUtilities.h"

// Sets default values for this component's properties
UVehicleAxleAssemblyComponent::UVehicleAxleAssemblyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void UVehicleAxleAssemblyComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	SearchExistingWheels();
	GenerateDifferential();
	WheelCoordinator = UVehicleWheelCoordinatorComponent::FindWheelCoordinator(Chassis.Get());
	if (WheelCoordinator.IsValid())WheelCoordinator->RegisterAxle(this);
}

void UVehicleAxleAssemblyComponent::OnRegister()
{
	Super::OnRegister();

	//...
	Chassis = UVehicleUtilities::FindPhysicalParent(this);
	//PreviewWheelMesh();
	InitializeWheels();
}

void UVehicleAxleAssemblyComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	if (Differential.IsValid() && !Differential->IsBeingDestroyed())
	{
		Differential->DestroyComponent();
	}
	Differential = nullptr;

	if (LeftWheel.IsValid() && !LeftWheel->IsBeingDestroyed())
	{
		LeftWheel->DestroyComponent();
	}
	LeftWheel = nullptr;

	if (RightWheel.IsValid() && !RightWheel->IsBeingDestroyed())
	{
		RightWheel->DestroyComponent();
	}
	RightWheel = nullptr;

	Chassis = nullptr;

	//...
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UVehicleAxleAssemblyComponent::UpdateSteering(
	float InSteeringInput,
	UVehicleWheelComponent* WheelL,
	UVehicleWheelComponent* WheelR)
{
	InSteeringInput = FMath::Clamp(InSteeringInput, -1.f, 1.f);
	float InsideWheelSteeringAngle = InSteeringInput * AxleSteeringConfig.MaxSteeringAngle;

	if (AxleSteeringConfig.bAckermannSteering 
		&& State.WheelBase > SMALL_NUMBER 
		&& State.NumOfWheels == 2)
	{
		// approximate the track width
		float TrackWidth = GetTrackWidth(WheelL, WheelR);

		//clamp angle to avoid divided by 0
		InsideWheelSteeringAngle = FMath::Clamp(InsideWheelSteeringAngle, -89, 89);
		if (FMath::IsNearlyZero(InsideWheelSteeringAngle))	//if no steering
		{
			State.LeftWheelSteeringAngle = State.RightWheelSteeringAngle = 0.f;
		}
		else
		{
			float SteeringSign = FMath::Sign(InsideWheelSteeringAngle);

			float UnsignedOutsideWheelAngle = 0.f;
			float UnsignedInsideWheelAngle = FMath::Abs(InsideWheelSteeringAngle);

			float InsideSteerRad = FMath::DegreesToRadians(UnsignedInsideWheelAngle);
			float TanInside = FMath::Tan(InsideSteerRad);
			float TanOutSide = State.WheelBase / (State.WheelBase / TanInside + TrackWidth);
			float OutsideSteerRad = FMath::Atan(TanOutSide);
			UnsignedOutsideWheelAngle = FMath::RadiansToDegrees(OutsideSteerRad);
			UnsignedOutsideWheelAngle = FMath::Lerp(UnsignedInsideWheelAngle, UnsignedOutsideWheelAngle, AxleSteeringConfig.AckermannRatio);
			

			if (InSteeringInput > 0)
			{
				State.LeftWheelSteeringAngle = UnsignedOutsideWheelAngle * SteeringSign;
				State.RightWheelSteeringAngle = UnsignedInsideWheelAngle * SteeringSign;
			}
			else
			{
				State.LeftWheelSteeringAngle = UnsignedInsideWheelAngle * SteeringSign;
				State.RightWheelSteeringAngle = UnsignedOutsideWheelAngle * SteeringSign;
			}
		}
	}
	else
	{
		State.LeftWheelSteeringAngle = State.RightWheelSteeringAngle = InsideWheelSteeringAngle;
	}
}

void UVehicleAxleAssemblyComponent::UpdateSteeringAssist(
	float DeltaTime, 
	float InSteeringInput)
{
	if (SteeringAssistConfig.bSteeringAssistEnabled)
	{
		float TargetInput = 0.f;

		FVector2D LocalVelocity2D = FVector2D(State.LocalLinearVelocity.X, State.LocalLinearVelocity.Y);
		FVector2D LocalVelocityDirection = LocalVelocity2D.GetSafeNormal();

		//get the slip angle of the Chassis
		float ChassisSlipAngle = FMath::Asin(LocalVelocityDirection.Y);
		ChassisSlipAngle = FMath::RadiansToDegrees(ChassisSlipAngle);

		if (State.LocalLinearVelocity.X > SteeringAssistConfig.ActivationSpeed
			&& FMath::Abs(ChassisSlipAngle) > SteeringAssistConfig.ActivationAngle)
		{
			TargetInput  = FMath::GetMappedRangeValueClamped(
				FVector2D(-AxleSteeringConfig.MaxSteeringAngle, AxleSteeringConfig.MaxSteeringAngle), 
				FVector2D(-1.f, 1.f), 
				ChassisSlipAngle * SteeringAssistConfig.Level
			);
		}

		if (SteeringAssistConfig.MaxSteerRate > SMALL_NUMBER)
		{
			State.SteeringAssistInput = FMath::FInterpConstantTo(
				State.SteeringAssistInput,
				TargetInput,
				DeltaTime,
				SteeringAssistConfig.MaxSteerRate
			);
		}
		else
		{
			// teleport if max steer rate is not valid
			State.SteeringAssistInput = TargetInput;
		}

		State.SteeringAssistInput = FMath::Clamp(State.SteeringAssistInput, -1.f, 1.f);
		State.RealSteeringValue = FMath::Clamp(InSteeringInput + State.SteeringAssistInput, -1.f, 1.f);
	}
	else
	{
		State.SteeringAssistInput = 0.f;
		State.RealSteeringValue = InSteeringInput;
	}
}

void UVehicleAxleAssemblyComponent::UpdateLinearVelocity(
	UVehicleWheelComponent* WheelL, 
	UVehicleWheelComponent* WheelR)
{
	FVector3f LeftWorldVel, RightWorldVel = FVector3f(0.f);
	FQuat4f ChassisRot = FQuat4f();
	if (WheelL)
	{
		LeftWorldVel = WheelL->GetWorldLinearVelocity();
		ChassisRot = (FQuat4f)WheelL->GetChassisAsyncWorldTransform().GetRotation();
	}
	if (WheelR)
	{
		RightWorldVel = WheelR->GetWorldLinearVelocity();
		ChassisRot = (FQuat4f)WheelR->GetChassisAsyncWorldTransform().GetRotation();
	}
	State.WorldLinearVelocity = State.NumOfWheels ? 
		FVector3f(LeftWorldVel + RightWorldVel) / State.NumOfWheels : FVector3f(0.f);

	ChassisRot.Normalize();
	State.LocalLinearVelocity = ChassisRot.UnrotateVector(State.WorldLinearVelocity);
}

void UVehicleAxleAssemblyComponent::UpdateSwaybarForce(
	UVehicleWheelComponent* WheelL, 
	UVehicleWheelComponent* WheelR)
{
	float HeightL = WheelL->GetHubChassisLocation().Z;
	float HeightR = WheelR->GetHubChassisLocation().Z;
	State.SwaybarForce = AxleConfig.SwaybarStiffness * 0.5f * (HeightL - HeightR);
}

void UVehicleAxleAssemblyComponent::UpdateTCS(
	UVehicleWheelComponent* WheelL, 
	UVehicleWheelComponent* WheelR, 
	float TargetDriveTorque)
{
	float SumSlipRatio = 0.f;
	if (WheelL)SumSlipRatio += FMath::Abs(WheelL->GetPredictedSlipRatio());
	if (WheelR)SumSlipRatio += FMath::Abs(WheelR->GetPredictedSlipRatio());
	float AvrgSlipRatio = SumSlipRatio / State.NumOfWheels;

	State.bTCSTriggered =
		TCSConfig.bTractionControlSystemEnabled
		&& State.NumOfWheelOnGround
		&& State.LocalLinearVelocity.X * TargetDriveTorque > SMALL_NUMBER
		&& FMath::Abs(State.LocalLinearVelocity.X) > TCSConfig.ActivationSpeed
		&& AvrgSlipRatio > TCSConfig.OptimalSlip;

	if (State.bTCSTriggered)
	{
		float Error = AvrgSlipRatio - TCSConfig.OptimalSlip;
		float TcsFactor = 1.0f - (Error * TCSConfig.Sensitivity);
		TcsFactor = FMath::Clamp(TcsFactor, 0.0f, 1.0f);
		State.AxleDriveTorque = TargetDriveTorque * TcsFactor;

		return;
	}

	State.AxleDriveTorque = TargetDriveTorque;
}

float UVehicleAxleAssemblyComponent::GetTrackWidth(
	UVehicleWheelComponent* WheelL, 
	UVehicleWheelComponent* WheelR)
{
	if (!(WheelL && WheelR))return AxleConfig.TrackWidth;

	const FTransform& TransformL = WheelL->GetRelativeTransform();
	const FTransform& TransformR = WheelR->GetRelativeTransform();

	float WheelL_Y = FTransform3f(TransformL).TransformPositionNoScale(WheelL->GetDesignedHubLocalTransform().GetLocation()).Y;
	float WheelR_Y = FTransform3f(TransformR).TransformPositionNoScale(WheelR->GetDesignedHubLocalTransform().GetLocation()).Y;

	return FMath::Abs(WheelR_Y - WheelL_Y);
}

void UVehicleAxleAssemblyComponent::PreStepSolidAxleSuspension(
	const float InMacroDeltaTime,
	const float SteerAngleLeft,
	const float SteerAngleRight,
	const float AntiRollBarForce,
	UVehicleWheelComponent* WheelL, 
	UVehicleWheelComponent* WheelR)
{
	FVector LeftHitLocation, RightHitLocation;

	FVehicleSuspensionSimContext LeftCtx;
	WheelL->StartPreStepSolidAxleSuspension(SteerAngleLeft, LeftHitLocation, LeftCtx);

	FVehicleSuspensionSimContext RightCtx;
	WheelR->StartPreStepSolidAxleSuspension(SteerAngleRight, RightHitLocation, RightCtx);

	float TrackWidth = GetTrackWidth();

	WheelL->FinalizePreStepSolidAxleSuspension(
		InMacroDeltaTime,
		AntiRollBarForce,
		LeftCtx,
		TrackWidth,
		LeftHitLocation,
		RightHitLocation
	);

	WheelR->FinalizePreStepSolidAxleSuspension(
		InMacroDeltaTime,
		-AntiRollBarForce,
		RightCtx,
		TrackWidth,
		RightHitLocation,
		LeftHitLocation
	);
}

// Called every frame
void UVehicleAxleAssemblyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UVehicleAxleAssemblyComponent::CopyAxleConfig(const UVehicleAxleAssemblyComponent* Source, UVehicleAxleAssemblyComponent* Target, bool bReInitializeWheel)
{
	Target->bUseExistingWheelComponent = Source->bUseExistingWheelComponent;
	Target->LeftWheelComponentName = Source->LeftWheelComponentName;
	Target->RightWheelComponentName = Source->RightWheelComponentName;
	Target->WheelClass = Source->WheelClass;
	Target->VehicleWheelComponentSetupRotation = Source->VehicleWheelComponentSetupRotation;
	Target->bUseExistingDifferentialComponent = Source->bUseExistingDifferentialComponent;
	Target->DifferentialComponentName = Source->DifferentialComponentName;
	Target->DifferentialClass = Source->DifferentialClass;
	Target->AxleLayout = Source->AxleLayout;
	Target->AxleConfig = Source->AxleConfig;
	Target->AxleSteeringConfig = Source->AxleSteeringConfig;
	Target->SteeringAssistConfig = Source->SteeringAssistConfig;
	Target->TCSConfig = Source->TCSConfig;

	if (bReInitializeWheel)
	{
		Target->InitializeWheels();
	}
}

void UVehicleAxleAssemblyComponent::ApplyInitialOverrides(const FVehicleAxleSpawnTemplate& AxleTemplate)
{
	//override these
	if (AxleTemplate.WheelClassOverride)WheelClass = AxleTemplate.WheelClassOverride;
	if (AxleTemplate.DifferentialClassOverride)DifferentialClass = AxleTemplate.DifferentialClassOverride;
	if (AxleTemplate.TrackWidthOverride >= 0)AxleConfig.TrackWidth = AxleTemplate.TrackWidthOverride;
	if (AxleTemplate.bDiasbleSteering)AxleSteeringConfig.bAffectedBySteering = false;
	if (AxleTemplate.bDisableHandbrake)AxleConfig.bAffectedByHandbrake = false;
	if (AxleTemplate.bDisableTractionControl)TCSConfig.bTractionControlSystemEnabled = false;
	if (AxleTemplate.TorqueWeightOverride >= 0)AxleConfig.TorqueWeight = AxleTemplate.TorqueWeightOverride;
}

void UVehicleAxleAssemblyComponent::PreStepAxle(
	float InMacroDeltaTime, 
	float InSteeringInput)
{
	UVehicleWheelComponent* WheelL = LeftWheel.Get();
	UVehicleWheelComponent* WheelR = RightWheel.Get();

	State.NumOfWheels = (int32)(WheelL != nullptr) + (int32)(WheelR != nullptr);
	if (State.NumOfWheels <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AxleAssembly: No Valid Wheels"));
		return;
	}

	UpdateLinearVelocity(WheelL, WheelR);

	if (AxleSteeringConfig.bAffectedBySteering)
	{
		UpdateSteeringAssist(InMacroDeltaTime, InSteeringInput);
		UpdateSteering(State.RealSteeringValue, WheelL, WheelR);
	}

	if (State.NumOfWheels == 2)
	{
		UpdateSwaybarForce(WheelL, WheelR);

		switch (SuspensionType)
		{
		default:
		case EVehicleAxleSuspensionType::Independent:
			WheelL->PreStepIndependentSuspension(
				InMacroDeltaTime,
				State.LeftWheelSteeringAngle,
				State.SwaybarForce
			);
			WheelR->PreStepIndependentSuspension(
				InMacroDeltaTime,
				State.RightWheelSteeringAngle,
				-State.SwaybarForce
			);
			break;
		case EVehicleAxleSuspensionType::Solid:
			PreStepSolidAxleSuspension(
				InMacroDeltaTime,
				State.LeftWheelSteeringAngle,
				State.RightWheelSteeringAngle,
				State.SwaybarForce,
				WheelL,
				WheelR
			);
			break;
		}
		State.NumOfWheelOnGround = WheelL->GetRayCastResult() + WheelR->GetRayCastResult();
		WheelL->PreStepWheel(InMacroDeltaTime);
		WheelR->PreStepWheel(InMacroDeltaTime);
	}
	else
	{
		if (WheelL)
		{
			WheelL->PreStepIndependentSuspension(
				InMacroDeltaTime,
				State.LeftWheelSteeringAngle,
				0.f
			);
			State.NumOfWheelOnGround = WheelL->GetRayCastResult();
			WheelL->PreStepWheel(InMacroDeltaTime);
		}

		if (WheelR)
		{
			WheelR->PreStepIndependentSuspension(
				InMacroDeltaTime,
				State.RightWheelSteeringAngle,
				0.f
			);
			State.NumOfWheelOnGround = WheelR->GetRayCastResult();
			WheelR->PreStepWheel(InMacroDeltaTime);
		}
	}
}

void UVehicleAxleAssemblyComponent::SubstepAxle(
	float InSubstepDeltaTime,
	float InDriveTorque,
	float InBrakeInput,
	float InHandbrakeInput,
	float InReflectedInertia,
	float& OutAxleEffectiveInertia,
	float& OutAngularVelocity)
{
	UVehicleWheelComponent* WheelL = LeftWheel.Get();
	UVehicleWheelComponent* WheelR = RightWheel.Get();
	UVehicleDifferentialComponent* Diff = Differential.Get();

	// number of living wheels
	State.NumOfWheels = (WheelL ? 1 : 0) + (WheelR ? 1 : 0);

	// get brake torque
	State.BrakeTorque = AxleConfig.MaxBrakeTorque * FMath::Clamp(InBrakeInput, 0.f, 1.f);
	State.HandbrakeTorque = AxleConfig.bAffectedByHandbrake ?
		AxleConfig.MaxHandbrakeTorque * FMath::Clamp(InHandbrakeInput, 0.f, 1.f) : 0.f;

	const bool bNoDriveTorque = AxleConfig.TorqueWeight <= SMALL_NUMBER && FMath::IsNearlyZero(State.P3MotorTorque);
	const bool bWheelNotDriven = bNoDriveTorque || (Diff == nullptr) || (State.NumOfWheels == 0);

	// 1. data preparation
	State.LeftDriveTorque = 0.f;
	State.RightDriveTorque = 0.f;
	State.ReflectedInertiaOnWheel = 0.f;

	// 2. torque distribution and inertia calculation
	if (bWheelNotDriven)
	{
		// losing power or differential broken
		State.AxleDriveTorque = bNoDriveTorque ? 0.f : InDriveTorque;
		State.AxleEffectiveInertia = AxleConfig.DriveShaftInertia;
		float AngAcc = UVehicleUtilities::SafeDivide(State.AxleDriveTorque, State.AxleEffectiveInertia);
		State.AxleAngularVelocity += AngAcc * InSubstepDeltaTime;
	}
	else
	{
		UpdateTCS(WheelL, WheelR, InDriveTorque + State.P3MotorTorque);

		if (State.NumOfWheels == 2)
		{
			// two wheels: limited-slip-differential
			Diff->UpdateOutputShaft(
				State.AxleDriveTorque,
				WheelL->GetAngularVelocity(), WheelR->GetAngularVelocity(),
				WheelL->GetEffectiveInertia(), WheelR->GetEffectiveInertia(),
				InSubstepDeltaTime, InReflectedInertia + AxleConfig.DriveShaftInertia,
				State.LeftDriveTorque, State.RightDriveTorque, State.ReflectedInertiaOnWheel
			);
		}
		else // single wheel
		{
			const float DiffGearRatio = Diff->GetConfig().GearRatio;
			State.ReflectedInertiaOnWheel = InReflectedInertia * DiffGearRatio * DiffGearRatio;

			if (AxleLayout == EVehicleAxleLayout::TwoWheels)
			{
				// open diff
				Diff->GetOpenDiffOutputTorque(State.AxleDriveTorque, State.LeftDriveTorque, State.RightDriveTorque);
			}
			else
			{
				// if there was initially only one wheel
				State.LeftDriveTorque = State.RightDriveTorque = DiffGearRatio * State.AxleDriveTorque;
			}
		}
	}

	// 3. wheel substepping
	if (WheelL) WheelL->SubStepWheel(InSubstepDeltaTime, State.LeftDriveTorque, State.BrakeTorque, State.HandbrakeTorque, State.ReflectedInertiaOnWheel);
	if (WheelR) WheelR->SubStepWheel(InSubstepDeltaTime, State.RightDriveTorque, State.BrakeTorque, State.HandbrakeTorque, State.ReflectedInertiaOnWheel);

	// 4. get axle effective inertia and angular velocity
	if (!bWheelNotDriven)
	{
		float ReflectedInertiaOfWheels = 0.f;

		if (State.NumOfWheels == 2)
		{
			Diff->UpdateInputShaft(
				WheelL->GetAngularVelocity(), WheelR->GetAngularVelocity(),
				WheelL->GetIntrinsicInertia(), WheelR->GetIntrinsicInertia(), // only the inertia of the wheel itself
				State.AxleAngularVelocity, ReflectedInertiaOfWheels
			);
		}
		else
		{
			// single wheel
			const float DiffGearRatio = Diff->GetConfig().GearRatio;
			UVehicleWheelComponent* ActiveWheel = WheelL ? WheelL : WheelR;
			State.AxleAngularVelocity = ActiveWheel->GetAngularVelocity() * DiffGearRatio;
			ReflectedInertiaOfWheels = UVehicleUtilities::SafeDivide(ActiveWheel->GetIntrinsicInertia(), DiffGearRatio * DiffGearRatio);
		}

		State.AxleEffectiveInertia = ReflectedInertiaOfWheels + AxleConfig.DriveShaftInertia;
	}

	// 5. output effective inertia and angular velocity
	OutAxleEffectiveInertia = State.AxleEffectiveInertia;
	OutAngularVelocity = State.AxleAngularVelocity;
}

void UVehicleAxleAssemblyComponent::PostStepAxle()
{
	UVehicleWheelComponent* WheelL = LeftWheel.Get();
	UVehicleWheelComponent* WheelR = RightWheel.Get();

	if (WheelL)
	{
		WheelL->PostStepApplyForce();
	}
	if (WheelR)
	{
		WheelR->PostStepApplyForce();
	}
}

void UVehicleAxleAssemblyComponent::InitializeWheels()
{
	GenerateWheels();

	UVehicleWheelComponent* WheelL = LeftWheel.Get();
	UVehicleWheelComponent* WheelR = RightWheel.Get();

	if (IsValid(WheelClass))
	{
		const UVehicleWheelComponent* TemplateWheel = Cast<UVehicleWheelComponent>(WheelClass->GetDefaultObject());
		if (WheelL)
		{
			UVehicleWheelComponent::CopyWheelConfig(TemplateWheel, WheelL);
		}
		if (WheelR)
		{
			UVehicleWheelComponent::CopyWheelConfig(TemplateWheel, WheelR);
		}
	}

	UpdateTrackWidth();

	if (WheelL)
	{
		WheelL->InitializeWheel();
	}
	if (WheelR)
	{
		WheelR->InitializeWheel();
	}
}

void UVehicleAxleAssemblyComponent::UpdatePhysics(
	float InPhysicsDeltaTime,
	float InDriveTorque, 
	float InBrakeInput,
	float InHandbrakeInput,
	float InSteeringInput,
	float InReflectedInertia,
	float& OutAxleEffectiveInertia,
	float& OutAngularVelocity)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_AxleAssembly_UpdatePhysics);

	if (!Chassis.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("AxleAssembly: No Valid Chassis"));
		return;
	}

	PreStepAxle(InPhysicsDeltaTime, InSteeringInput);
	SubstepAxle(InPhysicsDeltaTime,
		InDriveTorque, InBrakeInput, InHandbrakeInput,
		InReflectedInertia, OutAxleEffectiveInertia, OutAngularVelocity);
	PostStepAxle();
}

void UVehicleAxleAssemblyComponent::SetWheelPosition(float NewTrackWidth)
{
	NewTrackWidth = FMath::Abs(NewTrackWidth);

	UPrimitiveComponent* ChassisPtr = Chassis.Get();
	if (!ChassisPtr)Chassis = UVehicleUtilities::FindPhysicalParent(this);

	// attach itself to chassis
	if (ChassisPtr && GetAttachParent() != ChassisPtr)
	{
		AttachToComponent(ChassisPtr, FAttachmentTransformRules::KeepWorldTransform);
	}

	//set left wheel
	if (LeftWheel.IsValid())
	{
		if (Chassis.IsValid() && LeftWheel->GetAttachParent() != Chassis)
		{
			LeftWheel->AttachToComponent(Chassis.Get(), FAttachmentTransformRules::KeepRelativeTransform);
		}

		FQuat WheelCompRot = FQuat(FRotator(VehicleWheelComponentSetupRotation.Pitch, -VehicleWheelComponentSetupRotation.Yaw, -VehicleWheelComponentSetupRotation.Roll)).GetNormalized();
		FVector WheelCompPos = GetRelativeLocation();
		WheelCompPos.Y += -1.f * NewTrackWidth * 0.5f;
		LeftWheel->SetRelativeTransform(FTransform(WheelCompRot, WheelCompPos));

		LeftWheel->CacheDesignedHubTransform();

		const FVector DesignedHubLocalLocation = (FVector)LeftWheel->GetDesignedHubLocalTransform().GetLocation(); // in the space of wheel component
		const FVector HubOffset = WheelCompRot.RotateVector(DesignedHubLocalLocation);
		WheelCompPos -= HubOffset;
		LeftWheel->SetRelativeTransform(FTransform(WheelCompRot, WheelCompPos));
	}

	//set right wheel
	if (RightWheel.IsValid())
	{
		if (Chassis.IsValid() && RightWheel->GetAttachParent() != Chassis)
		{
			RightWheel->AttachToComponent(Chassis.Get(), FAttachmentTransformRules::KeepRelativeTransform);
		}

		FQuat WheelCompRot = FQuat(FRotator(VehicleWheelComponentSetupRotation.Pitch, VehicleWheelComponentSetupRotation.Yaw, VehicleWheelComponentSetupRotation.Roll)).GetNormalized();
		FVector WheelCompPos = GetRelativeLocation();
		WheelCompPos.Y += +1.f * NewTrackWidth * 0.5f;
		RightWheel->SetRelativeTransform(FTransform(WheelCompRot, WheelCompPos));

		RightWheel->CacheDesignedHubTransform();

		const FVector DesignedHubLocalLocation = (FVector)RightWheel->GetDesignedHubLocalTransform().GetLocation(); // in the space of wheel component
		const FVector HubOffset = WheelCompRot.RotateVector(DesignedHubLocalLocation);
		WheelCompPos -= HubOffset;
		RightWheel->SetRelativeTransform(FTransform(WheelCompRot, WheelCompPos));
	}
}

void UVehicleAxleAssemblyComponent::UpdateTrackWidth()
{
	SetWheelPosition(AxleConfig.TrackWidth);
}

float UVehicleAxleAssemblyComponent::GetTrackWidth()
{
	UVehicleWheelComponent* WheelL = LeftWheel.Get();
	UVehicleWheelComponent* WheelR = RightWheel.Get();
	if (WheelL && WheelR)
	{
		return GetTrackWidth(WheelL, WheelR);
	}
	else
	{
		return AxleConfig.TrackWidth;
	}
}

void UVehicleAxleAssemblyComponent::UpdateSolidAxleAnim(
	USceneComponent* InSolidAxleMesh, 
	EVehicleSolidAxleAnimPivot AxleMeshAnchorPoint, 
	const FVector InMeshRightVector,
	const FVector InMeshForwardVector)
{
	if (!InSolidAxleMesh || !LeftWheel.IsValid() || !RightWheel.IsValid() || !Chassis.IsValid()) return;

	FVector LeftLocal = (FVector)LeftWheel->GetAnimHubChassisLocation();
	FVector RightLocal = (FVector)RightWheel->GetAnimHubChassisLocation();

	FVector AxleDirLocal = (RightLocal - LeftLocal).GetSafeNormal();
	FVector AxleCenterLocal = (RightLocal + LeftLocal) * 0.5f;

	// 第一步：把网格体的主轴（如 Y 轴）对齐到左右轮的连线上
	FQuat Q1 = FQuat::FindBetweenNormals(InMeshRightVector, AxleDirLocal);

	// 此时，网格体可能沿着车轴发生了任意角度的“自转”。
	// 我们把它的副轴（如 X 轴）经过第一步旋转后的实际朝向拿出来：
	FVector RotatedForward = Q1.RotateVector(InMeshForwardVector);

	// 我们希望它的副轴尽量指向底盘的正前方 (1, 0, 0)。
	// 把 (1, 0, 0) 投影到垂直于车轴的平面上，得到理想的朝向：
	FVector TargetForward = FVector::VectorPlaneProject(FVector(1.f, 0.f, 0.f), AxleDirLocal).GetSafeNormal();

	// 第二步：算出消除自转的纠正 Quat
	FQuat Q2 = FQuat::FindBetweenNormals(RotatedForward, TargetForward);

	// 合并两步旋转：先做 Q1，再做 Q2 (注意四元数乘法从右向左读)
	FQuat FinalLocalRotation = Q2 * Q1;

	FVector FinalLocalLocation = AxleCenterLocal;
	switch (AxleMeshAnchorPoint)
	{
	case EVehicleSolidAxleAnimPivot::Left:
		FinalLocalLocation = LeftLocal;
		break;
	case EVehicleSolidAxleAnimPivot::Right:
		FinalLocalLocation = RightLocal;
		break;
	case EVehicleSolidAxleAnimPivot::Center:
	default:
		break;
	}

	FTransform ChassisTransform = Chassis->GetComponentTransform();
	FVector WorldLocation = ChassisTransform.TransformPositionNoScale(FinalLocalLocation);
	FQuat WorldRotation = ChassisTransform.GetRotation() * FinalLocalRotation;

	InSolidAxleMesh->SetWorldLocationAndRotation(WorldLocation, WorldRotation, false, nullptr, ETeleportType::TeleportPhysics);
}

void UVehicleAxleAssemblyComponent::ApplySolidAxleStateDirect(float InExtensionRatio, float SteeringAngle)
{
	if (!LeftWheel.IsValid() || !RightWheel.IsValid())return;

	const int32 Iteration = 2;
	for (int32 i = 0; i < Iteration; i++)
	{
		FVector LeftHitLocation, RightHitLocation;

		FVehicleSuspensionSimContext LeftCtx;
		LeftWheel->StartApplySolidAxleStateDirect(InExtensionRatio, SteeringAngle, LeftHitLocation, LeftCtx);

		FVehicleSuspensionSimContext RightCtx;
		RightWheel->StartApplySolidAxleStateDirect(InExtensionRatio, SteeringAngle, RightHitLocation, RightCtx);

		// the track width
		float TrackWidth = GetTrackWidth();

		LeftWheel->FinalizeApplySolidAxleStateDirect(
			LeftCtx,
			TrackWidth,
			LeftHitLocation,
			RightHitLocation
		);
		RightWheel->FinalizeApplySolidAxleStateDirect(
			RightCtx,
			TrackWidth,
			RightHitLocation,
			LeftHitLocation
		);
	}
}

void UVehicleAxleAssemblyComponent::ApplySuspensionStateDirect(float InExtensionRatio, float InSteeringAngle)
{
	switch (SuspensionType)
	{
	case EVehicleAxleSuspensionType::Independent:
		if (LeftWheel.IsValid())LeftWheel->ApplySuspensionStateDirect(InExtensionRatio, InSteeringAngle);
		if (RightWheel.IsValid())RightWheel->ApplySuspensionStateDirect(InExtensionRatio, InSteeringAngle);
		break;
	case EVehicleAxleSuspensionType::Solid:
		ApplySolidAxleStateDirect(InExtensionRatio, InSteeringAngle);
		break;
	default:
		if (LeftWheel.IsValid())LeftWheel->ApplySuspensionStateDirect(InExtensionRatio, InSteeringAngle);
		if (RightWheel.IsValid())RightWheel->ApplySuspensionStateDirect(InExtensionRatio, InSteeringAngle);
		break;
	}
}

void UVehicleAxleAssemblyComponent::GetLinearVelocity(FVector3f& OutLocalVelocity, FVector3f& OutWorldVelocity)
{
	OutLocalVelocity = State.LocalLinearVelocity;
	OutWorldVelocity = State.WorldLinearVelocity;
}

FVector3f UVehicleAxleAssemblyComponent::GetAxleCenter()
{
	UVehicleWheelComponent* WheelL = LeftWheel.Get();
	UVehicleWheelComponent* WheelR = RightWheel.Get();

	int32 n = (WheelL != nullptr) + (WheelR != nullptr);

	if (n == 2)
	{
		return 0.5f * (FVector3f)(WheelL->GetRelativeLocation() + WheelR->GetRelativeLocation());
	}
	else
	{
		if (WheelL)return (FVector3f)WheelL->GetRelativeLocation();
		if (WheelR)return (FVector3f)WheelR->GetRelativeLocation();
		return FVector3f(0.f);
	}
}

bool UVehicleAxleAssemblyComponent::GenerateWheels()
{
	UVehicleWheelComponent* WheelL = LeftWheel.Get();
	UVehicleWheelComponent* WheelR = RightWheel.Get();

	// if use Component
	if (bUseExistingWheelComponent)
	{
		if (WheelL)
		{
			WheelL->DestroyComponent();
		}
		if (WheelR)
		{
			WheelR->DestroyComponent();
		}
		return false;
	}

	//Destroy unused wheels
	if (WheelL && AxleLayout == EVehicleAxleLayout::SingleRight)
	{
		WheelL->DestroyComponent();
	}
	if (WheelR && AxleLayout == EVehicleAxleLayout::SingleLeft)
	{
		WheelR->DestroyComponent();
	}

	UObject* Outer = nullptr;
	if (AActor* Owner = GetOwner())Outer = Owner;

	if (!IsValid(Outer))return false;

	FString ThisName = FString();
	GetName(ThisName);

	//Generate left wheel
	if (!WheelL && AxleLayout != EVehicleAxleLayout::SingleRight)
	{
		FName Name = FName(ThisName + "_Wheel_L");
		LeftWheel = UVehicleUtilities::CreateComponentByClass<UVehicleWheelComponent>(
			Outer,
			nullptr,
			Name
		);
	}

	//Generate right wheel
	if (!WheelR && AxleLayout != EVehicleAxleLayout::SingleLeft)
	{
		FName Name = FName(ThisName + "_Wheel_R");
		RightWheel = UVehicleUtilities::CreateComponentByClass<UVehicleWheelComponent>(
			Outer,
			nullptr,
			Name
		);
	}

	switch (AxleLayout)
	{
	default:
	case EVehicleAxleLayout::TwoWheels:
		return LeftWheel.IsValid() && RightWheel.IsValid();
		break;
	case EVehicleAxleLayout::SingleLeft:
		return LeftWheel.IsValid();
		break;
	case EVehicleAxleLayout::SingleRight:
		return RightWheel.IsValid();
		break;
	}
}

bool UVehicleAxleAssemblyComponent::SearchExistingWheels()
{
	AActor* Owner = GetOwner();
	if (bUseExistingWheelComponent && IsValid(Owner))
	{
		if (!LeftWheel.IsValid())
		{
			LeftWheel = UVehicleUtilities::GetComponentByName<UVehicleWheelComponent>(Owner, LeftWheelComponentName);
		}
		if (!RightWheel.IsValid())
		{
			RightWheel = UVehicleUtilities::GetComponentByName<UVehicleWheelComponent>(Owner, RightWheelComponentName);
		}
		bool bIsLeftValid = LeftWheel.IsValid();
		if (bIsLeftValid)
		{
			LeftWheel->InitializeWheel();
		}
		bool bIsRightValid = RightWheel.IsValid();
		if (bIsRightValid)
		{
			RightWheel->InitializeWheel();
		}
		return bIsLeftValid && bIsRightValid;
	}
	else
	{
		return LeftWheel.IsValid() && RightWheel.IsValid();
	}
}

bool UVehicleAxleAssemblyComponent::GenerateDifferential()
{
	AActor* Owner = GetOwner();
	//set differential
	if (!Differential.IsValid() && IsValid(Owner))
	{
		bool bExistingDiffFound = false;
		if (bUseExistingDifferentialComponent)
		{
			Differential = UVehicleUtilities::GetComponentByName<UVehicleDifferentialComponent>(Owner, DifferentialComponentName);
			bExistingDiffFound = Differential.IsValid();
		}

		// if not found, generate one
		if (!bExistingDiffFound)
		{
			//UE_LOG(LogTemp, Warning, TEXT("AxleAssembly: GeneratingDifferential"));
			if (DifferentialClass)
			{
				Differential = Cast<UVehicleDifferentialComponent>
					(Owner->AddComponentByClass(DifferentialClass, false, FTransform(), false));
			}
			else
			{
				Differential = Cast<UVehicleDifferentialComponent>
					(Owner->AddComponentByClass(UVehicleDifferentialComponent::StaticClass(), false, FTransform(), false));
			}
		}
	}

	return Differential.IsValid();
}

TArray<FName> UVehicleAxleAssemblyComponent::GetNamesOfWheelsOfOwner()
{
	return UVehicleUtilities::GetNamesOfComponentsOfActor<UVehicleWheelComponent>(this);
}

TArray<FName> UVehicleAxleAssemblyComponent::GetNamesOfDifferentialsOfOwner()
{
	return UVehicleUtilities::GetNamesOfComponentsOfActor<UVehicleDifferentialComponent>(this);
}
