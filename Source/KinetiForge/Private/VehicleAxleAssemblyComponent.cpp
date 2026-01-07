// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)


#include "VehicleAxleAssemblyComponent.h"
#include "VehicleWheelComponent.h"
#include "VehicleDifferentialComponent.h"
#include "VehicleWheelCoordinatorComponent.h"

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
	WheelCoordinator = UVehicleWheelCoordinatorComponent::FindWheelCoordinator(Carbody);
	if (IsValid(WheelCoordinator))WheelCoordinator->RegisterAxle(this);
}

void UVehicleAxleAssemblyComponent::OnRegister()
{
	Super::OnRegister();

	//...
	Carbody = UVehicleWheelCoordinatorComponent::FindPhysicalParent(this);
	//PreviewWheelMesh();
	InitializeWheels();
}

void UVehicleAxleAssemblyComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	if (IsValid(Differential) && !Differential->IsBeingDestroyed())
	{
		Differential->DestroyComponent();
		Differential = nullptr;
	}
	if (IsValid(LeftWheel) && !LeftWheel->IsBeingDestroyed())
	{
		LeftWheel->DestroyComponent();
		LeftWheel = nullptr;
	}
	if (IsValid(RightWheel) && !RightWheel->IsBeingDestroyed())
	{
		RightWheel->DestroyComponent();
		RightWheel = nullptr;
	}

	if (IsValid(Carbody))Carbody = nullptr;

	//...
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UVehicleAxleAssemblyComponent::UpdateTwoWheelAxle(float InDriveTorque, float InReflectedInertia)
{
	UpdateSwaybarForce();

	if (AxleConfig.TorqueWeight <= 0 && FMath::IsNearlyZero(SimData.P3MotorTorque))
	{
		SimData.ReflectedInertiaOnWheel = 0.f;
		SimData.AxleDriveTorque = SimData.LeftDriveTorque = SimData.RightDriveTorque = 0.f;
	}
	else
	{
		UpdateTCS(InDriveTorque + SimData.P3MotorTorque);
		Differential->UpdateOutputShaft(
			SimData.AxleDriveTorque,
			LeftWheel->GetAngularVelocity(),
			RightWheel->GetAngularVelocity(),
			LeftWheel->GetTotalInertia(),
			RightWheel->GetTotalInertia(),
			SimData.PhysicsDeltaTime,
			InReflectedInertia + AxleConfig.DriveShaftInertia,
			SimData.LeftDriveTorque,
			SimData.RightDriveTorque,
			SimData.ReflectedInertiaOnWheel
		);
	}

	switch (SuspensionType)
	{
	case EVehicleAxleSuspensionType::Independent:
		UpdateIndependentSuspensionPhysics();
		break;
	case EVehicleAxleSuspensionType::Solid:
		UpdateSolidAxlePhysics();
		break;
	default:
		UpdateIndependentSuspensionPhysics();
		break;	
	}

	SimData.NumOfWheelOnGround = LeftWheel->GetRayCastResult() + RightWheel->GetRayCastResult();

	float ReflectedInertiaOfWheels = 0.f;
	Differential->UpdateInputShaft(
		LeftWheel->GetAngularVelocity(),
		RightWheel->GetAngularVelocity(),
		LeftWheel->WheelConfig.Inertia,
		RightWheel->WheelConfig.Inertia,
		SimData.AxleAngularVelocity,
		ReflectedInertiaOfWheels
	);

	SimData.AxleTotalInertia = ReflectedInertiaOfWheels + AxleConfig.DriveShaftInertia;
}

void UVehicleAxleAssemblyComponent::UpdateSingleWheelAxle(float InDriveTorque, float InReflectedInertia)
{
	float DiffGearRatio = Differential->Config.GearRatio;

	if (AxleConfig.TorqueWeight <= 0 && FMath::IsNearlyZero(SimData.P3MotorTorque))
	{
		SimData.ReflectedInertiaOnWheel = 0.f;
		SimData.AxleDriveTorque = SimData.LeftDriveTorque = SimData.RightDriveTorque = 0.f;
	}
	else
	{
		UpdateTCS(InDriveTorque + SimData.P3MotorTorque);//update inertia
		SimData.ReflectedInertiaOnWheel = InReflectedInertia * DiffGearRatio * DiffGearRatio;

		if (AxleLayout == EVehicleAxleLayout::TwoWheels)
		{
			Differential->GetOpenDiffOutputTorque(InDriveTorque, SimData.LeftDriveTorque, SimData.RightDriveTorque);
		}
		else
		{
			SimData.LeftDriveTorque = SimData.RightDriveTorque = DiffGearRatio * SimData.AxleDriveTorque;
		}
	}

	float ReflectedInertiaOfWheels = 0.f;

	if (IsValid(LeftWheel))
	{
		LeftWheel->UpdatePhysics(
			SimData.PhysicsDeltaTime,
			SimData.LeftDriveTorque,
			SimData.BrakeTorque,
			SimData.HandbrakeTorque,
			SimData.LeftWheelSteeringAngle,
			0.f,
			SimData.ReflectedInertiaOnWheel
		);

		SimData.NumOfWheelOnGround = LeftWheel->GetRayCastResult();
		SimData.AxleAngularVelocity = LeftWheel->GetAngularVelocity() * DiffGearRatio;
		ReflectedInertiaOfWheels = (DiffGearRatio > SMALL_NUMBER) ? LeftWheel->GetTotalInertia() / (DiffGearRatio * DiffGearRatio) : 0.f;
	}

	if (IsValid(RightWheel))
	{
		RightWheel->UpdatePhysics(
			SimData.PhysicsDeltaTime,
			SimData.RightDriveTorque,
			SimData.BrakeTorque,
			SimData.HandbrakeTorque,
			SimData.RightWheelSteeringAngle,
			0.f,
			SimData.ReflectedInertiaOnWheel
		);

		SimData.NumOfWheelOnGround = RightWheel->GetRayCastResult();
		SimData.AxleAngularVelocity = RightWheel->GetAngularVelocity() * DiffGearRatio;
		ReflectedInertiaOfWheels = (DiffGearRatio > SMALL_NUMBER) ? RightWheel->GetTotalInertia() / (DiffGearRatio * DiffGearRatio) : 0.f;
	}
	
	SimData.AxleTotalInertia = ReflectedInertiaOfWheels + AxleConfig.DriveShaftInertia;
}

void UVehicleAxleAssemblyComponent::UpdateSteering(float InSteeringInput)
{
	InSteeringInput = FMath::Clamp(InSteeringInput, -1.f, 1.f);
	float InsideWheelSteeringAngle = InSteeringInput * AxleSteeringConfig.MaxSteeringAngle;
	if (AxleSteeringConfig.bAckermannSteering 
		&& SimData.WheelBase > SMALL_NUMBER 
		&& AxleConfig.TrackWidth > SMALL_NUMBER)
	{
		//clamp angle to avoid divided by 0
		InsideWheelSteeringAngle = FMath::Clamp(InsideWheelSteeringAngle, -89, 89);
		if (FMath::IsNearlyZero(InsideWheelSteeringAngle))	//if no steering
		{
			SimData.LeftWheelSteeringAngle = SimData.RightWheelSteeringAngle = 0.f;
		}
		else
		{
			float UnsignedOutsideWheelAngle;
			float UnsignedInsideWheelAngle = FMath::Abs(InsideWheelSteeringAngle);

			float InsideSteerRad = FMath::DegreesToRadians(UnsignedInsideWheelAngle);
			float TanInside = FMath::Tan(InsideSteerRad);
			float TanOutSide = SimData.WheelBase / (SimData.WheelBase / TanInside + AxleConfig.TrackWidth);
			float OutsideSteerRad = FMath::Atan(TanOutSide);
			UnsignedOutsideWheelAngle = FMath::RadiansToDegrees(OutsideSteerRad);
			UnsignedOutsideWheelAngle = FMath::Lerp(UnsignedInsideWheelAngle, UnsignedOutsideWheelAngle, AxleSteeringConfig.AckermannRatio);
			

			if (InSteeringInput > 0)
			{
				SimData.LeftWheelSteeringAngle = UnsignedOutsideWheelAngle;
				SimData.RightWheelSteeringAngle = UnsignedInsideWheelAngle;
			}
			else
			{
				SimData.LeftWheelSteeringAngle = -UnsignedInsideWheelAngle;
				SimData.RightWheelSteeringAngle = -UnsignedOutsideWheelAngle;
			}
		}
	}
	else
	{
		SimData.LeftWheelSteeringAngle = SimData.RightWheelSteeringAngle = InsideWheelSteeringAngle;
	}
}

void UVehicleAxleAssemblyComponent::UpdateSteeringAssist(float InSteeringInput)
{
	if (SteeringAssistConfig.bSteeringAssistEnabled)
	{
		float TargetSteeringAssistInput = 0.f;
		FVector2D LocalVelocity2D = FVector2D(SimData.LocalLinearVelocity.X, SimData.LocalLinearVelocity.Y);
		LocalVelocity2D = LocalVelocity2D.GetSafeNormal();

		//get the slip angle of the carbody
		float CarbodySlipAngle = FMath::Asin(LocalVelocity2D.Y);
		CarbodySlipAngle = FMath::RadiansToDegrees(CarbodySlipAngle);

		//because sin(x)¡Öx when x is small, and we don't need this to be accurate
		//float CarbodySlipAngle = FMath::RadiansToDegrees(LocalVelocity2D.Y);
		//CarbodySlipAngle = FMath::Clamp(CarbodySlipAngle, -90.f, 90.f);

		if (SimData.LocalLinearVelocity.X > SteeringAssistConfig.ActivationSpeed
			&& FMath::Abs(CarbodySlipAngle) > SteeringAssistConfig.ActivationAngle)
		{
			TargetSteeringAssistInput
				= FMath::GetMappedRangeValueClamped(
					FVector2D(-AxleSteeringConfig.MaxSteeringAngle, AxleSteeringConfig.MaxSteeringAngle), FVector2D(-1.f, 1.f), CarbodySlipAngle * SteeringAssistConfig.Level);
		}

		float SmoothFactor = FMath::Clamp(SteeringAssistConfig.Smoothing, 0.f, 1.f);
		SimData.SteeringAssistInput = FMath::Clamp(TargetSteeringAssistInput + (SimData.SteeringAssistInput - TargetSteeringAssistInput) * SmoothFactor, -1.f, 1.f);
		SimData.RealSteeringValue = FMath::Clamp(InSteeringInput + SimData.SteeringAssistInput, -1.f, 1.f);
	}
	else
	{
		SimData.SteeringAssistInput = 0.f;
		SimData.RealSteeringValue = InSteeringInput;
	}
}

void UVehicleAxleAssemblyComponent::CalculateLinearVelocity()
{
	FVector3f LeftWorldVel, RightWorldVel = FVector3f(0.f);
	FQuat4f CarbodyRot = FQuat4f();
	if (IsValid(LeftWheel))
	{
		LeftWorldVel = LeftWheel->GetWorldLinearVelocity();
		CarbodyRot = (FQuat4f)LeftWheel->GetCarbodyAsyncWorldTransform().GetRotation();
	}
	if (IsValid(RightWheel))
	{
		RightWorldVel = RightWheel->GetWorldLinearVelocity();
		CarbodyRot = (FQuat4f)RightWheel->GetCarbodyAsyncWorldTransform().GetRotation();
	}
	SimData.WorldLinearVelocity = FVector3f(LeftWorldVel + RightWorldVel) / SimData.NumOfWheels;
	SimData.LocalLinearVelocity = CarbodyRot.UnrotateVector(SimData.WorldLinearVelocity);
}

void UVehicleAxleAssemblyComponent::UpdateSwaybarForce()
{
	SimData.SwaybarForce = AxleConfig.SwaybarStiffness * 0.5 * (RightWheel->GetSuspensionLength() - LeftWheel->GetSuspensionLength());
}

void UVehicleAxleAssemblyComponent::UpdateTCS(float TargetDriveTorque)
{
	float SumSlipRatio = 0.f;
	if (IsValid(LeftWheel))SumSlipRatio += FMath::Abs(LeftWheel->GetSlipRatio());
	if (IsValid(RightWheel))SumSlipRatio += FMath::Abs(RightWheel->GetSlipRatio());
	float AvrgSlipRatio = SumSlipRatio / SimData.NumOfWheels;

	SimData.bTCSTriggered =
		TCSConfig.bTractionControlSystemEnabled
		&& SimData.NumOfWheelOnGround
		&& !FMath::IsNearlyZero(TargetDriveTorque)
		&& FMath::Abs(SimData.LocalLinearVelocity.X) > TCSConfig.ActivationSpeed
		&& AvrgSlipRatio > TCSConfig.OptimalSlip;

	if (SimData.bTCSTriggered)
	{
		float Error = AvrgSlipRatio - TCSConfig.OptimalSlip;
		float TcsFactor = 1.0f - (Error * TCSConfig.Sensitivity);
		TcsFactor = FMath::Clamp(TcsFactor, 0.0f, 1.0f);
		SimData.AxleDriveTorque = TargetDriveTorque * TcsFactor;

		return;
	}

	SimData.AxleDriveTorque = TargetDriveTorque;
}

void UVehicleAxleAssemblyComponent::UpdateIndependentSuspensionPhysics()
{
	RightWheel->UpdatePhysics(
		SimData.PhysicsDeltaTime,
		SimData.RightDriveTorque,
		SimData.BrakeTorque,
		SimData.HandbrakeTorque,
		SimData.RightWheelSteeringAngle,
		-SimData.SwaybarForce,
		SimData.ReflectedInertiaOnWheel);
	LeftWheel->UpdatePhysics(
		SimData.PhysicsDeltaTime,
		SimData.LeftDriveTorque,
		SimData.BrakeTorque,
		SimData.HandbrakeTorque,
		SimData.LeftWheelSteeringAngle,
		SimData.SwaybarForce,
		SimData.ReflectedInertiaOnWheel);
}

void UVehicleAxleAssemblyComponent::UpdateSolidAxlePhysics()
{
	FVector LeftWorldPos = FVector(0.f);
	FVector RightWorldPos = FVector(0.f);

	FVehicleSuspensionSimContext LeftCtx;
	LeftWheel->StartUpdateSolidAxlePhysics(SimData.LeftWheelSteeringAngle, LeftWorldPos, LeftCtx);

	FVehicleSuspensionSimContext RightCtx;
	RightWheel->StartUpdateSolidAxlePhysics(SimData.RightWheelSteeringAngle, RightWorldPos, RightCtx);

	// get the world direction of the axle
	FVector AxleDirection = (RightWorldPos - LeftWorldPos).GetSafeNormal();

	// the center of axle under world coordinate
	FVector AxleWorldCenter = (RightWorldPos + LeftWorldPos) * 0.5f;

	// the track width
	float DynTrackWidth = FMath::Abs(LeftWheel->GetRelativeLocation().Y - RightWheel->GetRelativeLocation().Y);
	DynTrackWidth += LeftWheel->SuspensionKinematicsConfig.ArmLength + RightWheel->SuspensionKinematicsConfig.ArmLength;

	// get the position of the ball joint(connecting the wheel and the suspension) of each wheel
	FVector LeftKnuckleWorldPos = AxleWorldCenter - AxleDirection * DynTrackWidth * 0.5f;
	FVector RightKnuckleWorldPos = AxleWorldCenter + AxleDirection * DynTrackWidth * 0.5f;

	LeftWheel->FinalizeUpdateSolidAxlePhysics(
		SimData.PhysicsDeltaTime,
		SimData.LeftDriveTorque,
		SimData.BrakeTorque,
		SimData.HandbrakeTorque,
		SimData.SwaybarForce,
		SimData.ReflectedInertiaOnWheel,
		LeftCtx,
		LeftKnuckleWorldPos,
		AxleDirection);
	RightWheel->FinalizeUpdateSolidAxlePhysics(
		SimData.PhysicsDeltaTime,
		SimData.RightDriveTorque,
		SimData.BrakeTorque,
		SimData.HandbrakeTorque,
		-SimData.SwaybarForce,
		SimData.ReflectedInertiaOnWheel,
		RightCtx,
		RightKnuckleWorldPos,
		AxleDirection);
}

// Called every frame
void UVehicleAxleAssemblyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//make sure the wheel preview is destoryed
}

void UVehicleAxleAssemblyComponent::CopyAxleConfig(const UVehicleAxleAssemblyComponent* Source, UVehicleAxleAssemblyComponent* Target, bool bReInitializeWheel)
{
	Target->bUseExistingWheelComponent = Source->bUseExistingWheelComponent;
	Target->LeftWheelComponentName = Source->LeftWheelComponentName;
	Target->RightWheelComponentName = Source->RightWheelComponentName;
	Target->WheelConfig = Source->WheelConfig;
	Target->VehicleWheelComponentSetupRotation = Source->VehicleWheelComponentSetupRotation;
	Target->bUseExistingDifferentialComponent = Source->bUseExistingDifferentialComponent;
	Target->DifferentialComponentName = Source->DifferentialComponentName;
	Target->DifferentialConfig = Source->DifferentialConfig;
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

void UVehicleAxleAssemblyComponent::InitializeWheels()
{
	GenerateWheels();

	if (IsValid(WheelConfig))
	{
		const UVehicleWheelComponent* TemplateWheel = Cast<UVehicleWheelComponent>(WheelConfig->GetDefaultObject());
		if (IsValid(LeftWheel))
		{
			UVehicleWheelComponent::CopyWheelConfig(TemplateWheel, LeftWheel);
		}
		if (IsValid(RightWheel))
		{
			UVehicleWheelComponent::CopyWheelConfig(TemplateWheel, RightWheel);
		}
	}

	UpdateTrackWidth();

	if (IsValid(LeftWheel))
	{
		LeftWheel->InitializeWheel();
	}
	if (IsValid(RightWheel))
	{
		RightWheel->InitializeWheel();
	}
}

void UVehicleAxleAssemblyComponent::UpdatePhysics(
	float InPhysicsDeltaTime,
	float InDriveTorque, 
	float InBrakeInput,
	float InHandbrakeInput,
	float InSteeringInput,
	float InReflectedInertia, 
	float& OutAxleTotalInertia,
	float& OutAngularVelocity)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UpdateVehicleAxle);

	if (!IsValid(Carbody))
	{
		UE_LOG(LogTemp, Warning, TEXT("AxleAssembly: No Valid Carbody"));
		return;
	}

	if(!IsValid(Differential))
	{
		UE_LOG(LogTemp, Warning, TEXT("AxleAssembly: No Valid Differential"));
		return;
	}

	SimData.NumOfWheels = IsValid(LeftWheel) + IsValid(RightWheel);
	if (SimData.NumOfWheels <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AxleAssembly: No Valid Wheels"));
		return;
	}

	SimData.PhysicsDeltaTime = InPhysicsDeltaTime;

	SimData.BrakeTorque = AxleConfig.MaxBrakeTorque * FMath::Clamp(InBrakeInput, 0.f, 1.f);
	if (AxleConfig.bAffectedByHandbrake)
	{
		SimData.HandbrakeTorque = AxleConfig.MaxHandbrakeTorque * FMath::Clamp(InHandbrakeInput, 0.f, 1.f);
	}
	else
	{
		SimData.HandbrakeTorque = 0;
	}

	CalculateLinearVelocity();

	if (AxleSteeringConfig.bAffectedBySteering)
	{
		UpdateSteeringAssist(InSteeringInput);
		UpdateSteering(SimData.RealSteeringValue);
	}
	
	if (SimData.NumOfWheels == 2)
	{
		UpdateTwoWheelAxle(InDriveTorque, InReflectedInertia);
	}
	else if (SimData.NumOfWheels == 1)
	{
		UpdateSingleWheelAxle(InDriveTorque, InReflectedInertia);
	}
	else
	{
		// if no wheels
		SimData.AxleTotalInertia = AxleConfig.DriveShaftInertia;
		SimData.AxleDriveTorque = InDriveTorque;
		float AngAcc = (SimData.AxleTotalInertia > SMALL_NUMBER) ? SimData.AxleDriveTorque / SimData.AxleTotalInertia : 0.f;
		SimData.AxleAngularVelocity += AngAcc * SimData.PhysicsDeltaTime;
	}

	OutAxleTotalInertia = SimData.AxleTotalInertia;
	OutAngularVelocity = SimData.AxleAngularVelocity;
}

void UVehicleAxleAssemblyComponent::SetWheelPosition(float NewTrackWidth)
{
	//set left wheel
	if (IsValid(LeftWheel))
	{
		if (IsValid(Carbody) && LeftWheel->GetAttachParent() != Carbody)
		{
			LeftWheel->AttachToComponent(Carbody, FAttachmentTransformRules::KeepRelativeTransform);
		}

		FVehicleSuspensionKinematicsConfig& KineConfig = LeftWheel->SuspensionKinematicsConfig;
		FQuat WheelCompRot = FQuat(FRotator(VehicleWheelComponentSetupRotation.Pitch, -VehicleWheelComponentSetupRotation.Yaw, VehicleWheelComponentSetupRotation.Roll));
		FVector WheelCompPos = FVector(GetRelativeLocation().X, GetRelativeLocation().Y - FMath::Abs(NewTrackWidth * 0.5), GetRelativeLocation().Z);
		FQuat WheelAlignmentRot = FQuat(FRotator(0, -KineConfig.BaseToe, -KineConfig.BaseCamber));
		WheelCompPos += WheelAlignmentRot.GetRightVector() * KineConfig.AxialHubOffset;
		WheelCompPos += WheelCompRot.GetRightVector() * KineConfig.ArmLength;
		LeftWheel->SetRelativeTransform(FTransform(WheelCompRot, WheelCompPos));
	}

	//set right wheel
	if (IsValid(RightWheel))
	{
		if (IsValid(Carbody) && RightWheel->GetAttachParent() != Carbody)
		{
			RightWheel->AttachToComponent(Carbody, FAttachmentTransformRules::KeepRelativeTransform);
		}

		FVehicleSuspensionKinematicsConfig& KineConfig = RightWheel->SuspensionKinematicsConfig;
		FQuat WheelCompRot = FQuat(FRotator(VehicleWheelComponentSetupRotation.Pitch, VehicleWheelComponentSetupRotation.Yaw, -VehicleWheelComponentSetupRotation.Roll));
		FVector WheelCompPos = FVector(GetRelativeLocation().X, GetRelativeLocation().Y + FMath::Abs(NewTrackWidth * 0.5), GetRelativeLocation().Z);
		FQuat WheelAlignmentRot = FQuat(FRotator(0, KineConfig.BaseToe, KineConfig.BaseCamber));
		WheelCompPos -= WheelAlignmentRot.GetRightVector() * KineConfig.AxialHubOffset;
		WheelCompPos -= WheelCompRot.GetRightVector() * KineConfig.ArmLength;
		RightWheel->SetRelativeTransform(FTransform(WheelCompRot, WheelCompPos));
	}
}

void UVehicleAxleAssemblyComponent::UpdateTrackWidth()
{
	SetWheelPosition(AxleConfig.TrackWidth);
}

void UVehicleAxleAssemblyComponent::UpdateSolidAxleAnim(USceneComponent* InSolidAxleMesh, EVehicleSolidAxleAnimPivot AxleMeshAnchorPoint)
{
	if (!IsValid(LeftWheel) || !IsValid(RightWheel))return;

	// get the position of the ball joint(connecting the wheel and the suspension) of each wheel
	FVector LeftKnuckleRelativePos = LeftWheel->GetKnuckleRelativePosition();
	FVector RightKnuckleRelativePos = RightWheel->GetKnuckleRelativePosition();

	FVector LeftKnuckleWorldPos = Carbody->GetComponentTransform().TransformPositionNoScale(LeftKnuckleRelativePos);
	FVector RightKnuckleWorldPos = Carbody->GetComponentTransform().TransformPositionNoScale(RightKnuckleRelativePos);

	// get the world direction of the axle
	FVector AxleDirection = (RightKnuckleWorldPos - LeftKnuckleWorldPos).GetSafeNormal();

	// the center of axle under world coordinate
	FVector AxleCenter = (RightKnuckleWorldPos + LeftKnuckleWorldPos) * 0.5f;

	// get the relative rotation of the axle
	FVector DefaultRight = FVector(0.f, 1.f, 0.f);
	FQuat AxleRotation = FQuat::FindBetweenNormals(DefaultRight, AxleDirection);
	InSolidAxleMesh->SetWorldRotation(AxleRotation);

	switch (AxleMeshAnchorPoint)
	{
	case EVehicleSolidAxleAnimPivot::Center:
		InSolidAxleMesh->SetWorldLocation(AxleCenter);
		break;
	case EVehicleSolidAxleAnimPivot::Left:
		InSolidAxleMesh->SetWorldLocation(LeftKnuckleWorldPos);
		break;
	case EVehicleSolidAxleAnimPivot::Right:
		InSolidAxleMesh->SetWorldLocation(RightKnuckleWorldPos);
		break;
	default:
		break;
	}
}

void UVehicleAxleAssemblyComponent::GetLinearVelocity(FVector3f& OutLocalVelocity, FVector3f& OutWorldVelocity)
{
	OutLocalVelocity = SimData.LocalLinearVelocity;
	OutWorldVelocity = SimData.WorldLinearVelocity;
}

FVector3f UVehicleAxleAssemblyComponent::GetAxleCenter()
{
	int32 n = IsValid(LeftWheel) + IsValid(RightWheel);

	if (n == 2)
	{
		return 0.5f * (FVector3f)(LeftWheel->GetRelativeLocation() + RightWheel->GetRelativeLocation());
	}
	else
	{
		if (IsValid(LeftWheel))return (FVector3f)LeftWheel->GetRelativeLocation();
		if (IsValid(RightWheel))return (FVector3f)RightWheel->GetRelativeLocation();
		return FVector3f(0.f);
	}
}

bool UVehicleAxleAssemblyComponent::GenerateWheels()
{
	// if use Component
	if (bUseExistingWheelComponent)
	{
		if (IsValid(LeftWheel))
		{
			LeftWheel->DestroyComponent();
		}
		if (IsValid(RightWheel))
		{
			RightWheel->DestroyComponent();
		}
		return false;
	}

	//Destroy unused wheels
	if (IsValid(LeftWheel) && AxleLayout == EVehicleAxleLayout::SingleRight)
	{
		LeftWheel->DestroyComponent();
	}
	if (IsValid(RightWheel) && AxleLayout == EVehicleAxleLayout::SingleLeft)
	{
		RightWheel->DestroyComponent();
	}

	//Generate left wheel
	if (!IsValid(LeftWheel) && AxleLayout != EVehicleAxleLayout::SingleRight)
	{
		LeftWheel = Cast<UVehicleWheelComponent>
			(GetOwner()->AddComponentByClass(UVehicleWheelComponent::StaticClass(), false, FTransform(), false));
	}

	//Generate right wheel
	if (!IsValid(RightWheel) && AxleLayout != EVehicleAxleLayout::SingleLeft)
	{
		RightWheel = Cast<UVehicleWheelComponent>
			(GetOwner()->AddComponentByClass(UVehicleWheelComponent::StaticClass(), false, FTransform(), false));
	}

	switch (AxleLayout)
	{
	case EVehicleAxleLayout::TwoWheels:
		return IsValid(LeftWheel) && IsValid(RightWheel);
		break;
	case EVehicleAxleLayout::SingleLeft:
		return IsValid(LeftWheel);
		break;
	case EVehicleAxleLayout::SingleRight:
		return IsValid(RightWheel);
		break;
	default:
		return IsValid(LeftWheel) && IsValid(RightWheel);
		break;
	}
}

bool UVehicleAxleAssemblyComponent::SearchExistingWheels()
{
	if (bUseExistingWheelComponent && GetOwner())
	{
		// search for wheels
		for (UActorComponent* Comp : GetOwner()->GetComponents())
		{
			UVehicleWheelComponent* Wheel;
			Wheel = Cast<UVehicleWheelComponent>(Comp);
			if (IsValid(Wheel))
			{
				if (!IsValid(LeftWheel))
				{
					if (Wheel->GetName() == LeftWheelComponentName)
					{
						LeftWheel = Wheel;
						LeftWheel->InitializeWheel();
					}
				}
				if (!IsValid(RightWheel))
				{
					if (Wheel->GetName() == RightWheelComponentName)
					{
						RightWheel = Wheel;
						RightWheel->InitializeWheel();
					}
				}
				if (IsValid(LeftWheel) && IsValid(RightWheel)) return true;
			}
		}
		return false;
	}
	else
	{
		return IsValid(LeftWheel) && IsValid(RightWheel);
	}
}

bool UVehicleAxleAssemblyComponent::GenerateDifferential()
{
	//set differential
	if (!IsValid(Differential) && GetOwner())
	{
		bool bExistingDiffFound = false;
		if (bUseExistingDifferentialComponent)
		{
			// search for differential
			for (UActorComponent* Comp : GetOwner()->GetComponents())
			{
				UVehicleDifferentialComponent* Diff;
				Diff = Cast<UVehicleDifferentialComponent>(Comp);
				if (IsValid(Diff) && Comp->GetFName() == DifferentialComponentName)
				{
					Differential = Diff;
					bExistingDiffFound = true;
					break;
				}
			}
		}

		// if not found, generate one
		if (!bExistingDiffFound)
		{
			//UE_LOG(LogTemp, Warning, TEXT("AxleAssembly: GeneratingDifferential"));
			if (DifferentialConfig)
			{
				Differential = Cast<UVehicleDifferentialComponent>
					(GetOwner()->AddComponentByClass(DifferentialConfig, false, FTransform(), false));
			}
			else
			{
				Differential = Cast<UVehicleDifferentialComponent>
					(GetOwner()->AddComponentByClass(UVehicleDifferentialComponent::StaticClass(), false, FTransform(), false));
			}
		}
	}

	return IsValid(Differential);
}
