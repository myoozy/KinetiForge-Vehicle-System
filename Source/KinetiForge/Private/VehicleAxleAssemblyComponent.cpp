// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)


#include "VehicleAxleAssemblyComponent.h"
#include "VehicleWheelComponent.h"
#include "VehicleDifferentialComponent.h"
#include "VehicleWheelCoordinatorComponent.h"
#include "VehicleUtil.h"

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
	WheelCoordinator = UVehicleWheelCoordinatorComponent::FindWheelCoordinator(Carbody.Get());
	if (WheelCoordinator.IsValid())WheelCoordinator->RegisterAxle(this);
}

void UVehicleAxleAssemblyComponent::OnRegister()
{
	Super::OnRegister();

	//...
	Carbody = UVehicleUtil::FindPhysicalParent(this);
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

	Carbody = nullptr;

	//...
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UVehicleAxleAssemblyComponent::UpdateTwoWheelAxle(
	UVehicleWheelComponent* WheelL, 
	UVehicleWheelComponent* WheelR, 
	float InDriveTorque, 
	float InReflectedInertia)
{
	UVehicleDifferentialComponent* Diff = Differential.Get();
	if (!WheelL || !WheelR || !Diff)return;

	UpdateSwaybarForce(WheelL, WheelR);

	if (AxleConfig.TorqueWeight <= 0 && FMath::IsNearlyZero(State.P3MotorTorque))
	{
		State.ReflectedInertiaOnWheel = 0.f;
		State.AxleDriveTorque = State.LeftDriveTorque = State.RightDriveTorque = 0.f;
	}
	else
	{
		UpdateTCS(WheelL, WheelR, InDriveTorque + State.P3MotorTorque);
		Diff->UpdateOutputShaft(
			State.AxleDriveTorque,
			WheelL->GetAngularVelocity(),
			WheelR->GetAngularVelocity(),
			WheelL->GetTotalInertia(),
			WheelR->GetTotalInertia(),
			State.PhysicsDeltaTime,
			InReflectedInertia + AxleConfig.DriveShaftInertia,
			State.LeftDriveTorque,
			State.RightDriveTorque,
			State.ReflectedInertiaOnWheel
		);
	}

	switch (SuspensionType)
	{
	case EVehicleAxleSuspensionType::Independent:
		UpdateIndependentSuspensionPhysics(WheelL, WheelR);
		break;
	case EVehicleAxleSuspensionType::Solid:
		UpdateSolidAxlePhysics(WheelL, WheelR);
		break;
	default:
		UpdateIndependentSuspensionPhysics(WheelL, WheelR);
		break;	
	}

	State.NumOfWheelOnGround = WheelL->GetRayCastResult() + WheelR->GetRayCastResult();

	float ReflectedInertiaOfWheels = 0.f;
	Diff->UpdateInputShaft(
		WheelL->GetAngularVelocity(),
		WheelR->GetAngularVelocity(),
		WheelL->WheelConfig.Inertia,
		WheelR->WheelConfig.Inertia,
		State.AxleAngularVelocity,
		ReflectedInertiaOfWheels
	);

	State.AxleTotalInertia = ReflectedInertiaOfWheels + AxleConfig.DriveShaftInertia;
}

void UVehicleAxleAssemblyComponent::UpdateSingleWheelAxle(
	UVehicleWheelComponent* WheelL, 
	UVehicleWheelComponent* WheelR, 
	float InDriveTorque, 
	float InReflectedInertia)
{
	UVehicleDifferentialComponent* Diff = Differential.Get();

	float DiffGearRatio = Diff ? Diff->Config.GearRatio : 1.f;

	if (AxleConfig.TorqueWeight <= 0 && FMath::IsNearlyZero(State.P3MotorTorque))
	{
		State.ReflectedInertiaOnWheel = 0.f;
		State.AxleDriveTorque = State.LeftDriveTorque = State.RightDriveTorque = 0.f;
	}
	else
	{
		UpdateTCS(WheelL, WheelR, InDriveTorque + State.P3MotorTorque);//update inertia
		State.ReflectedInertiaOnWheel = InReflectedInertia * DiffGearRatio * DiffGearRatio;

		if (AxleLayout == EVehicleAxleLayout::TwoWheels && Diff)
		{
			Diff->GetOpenDiffOutputTorque(InDriveTorque, State.LeftDriveTorque, State.RightDriveTorque);
		}
		else
		{
			State.LeftDriveTorque = State.RightDriveTorque = DiffGearRatio * State.AxleDriveTorque;
		}
	}

	float ReflectedInertiaOfWheels = 0.f;

	if (WheelL)
	{
		WheelL->UpdatePhysics(
			State.PhysicsDeltaTime,
			State.LeftDriveTorque,
			State.BrakeTorque,
			State.HandbrakeTorque,
			State.LeftWheelSteeringAngle,
			0.f,
			State.ReflectedInertiaOnWheel
		);

		State.NumOfWheelOnGround = WheelL->GetRayCastResult();
		State.AxleAngularVelocity = WheelL->GetAngularVelocity() * DiffGearRatio;
		ReflectedInertiaOfWheels = (DiffGearRatio > SMALL_NUMBER) ? WheelL->GetTotalInertia() / (DiffGearRatio * DiffGearRatio) : 0.f;
	}

	if (WheelR)
	{
		WheelR->UpdatePhysics(
			State.PhysicsDeltaTime,
			State.RightDriveTorque,
			State.BrakeTorque,
			State.HandbrakeTorque,
			State.RightWheelSteeringAngle,
			0.f,
			State.ReflectedInertiaOnWheel
		);

		State.NumOfWheelOnGround = WheelR->GetRayCastResult();
		State.AxleAngularVelocity = WheelR->GetAngularVelocity() * DiffGearRatio;
		ReflectedInertiaOfWheels = (DiffGearRatio > SMALL_NUMBER) ? WheelR->GetTotalInertia() / (DiffGearRatio * DiffGearRatio) : 0.f;
	}
	
	State.AxleTotalInertia = ReflectedInertiaOfWheels + AxleConfig.DriveShaftInertia;
}

void UVehicleAxleAssemblyComponent::UpdateSteering(float InSteeringInput)
{
	InSteeringInput = FMath::Clamp(InSteeringInput, -1.f, 1.f);
	float InsideWheelSteeringAngle = InSteeringInput * AxleSteeringConfig.MaxSteeringAngle;
	if (AxleSteeringConfig.bAckermannSteering 
		&& State.WheelBase > SMALL_NUMBER 
		&& AxleConfig.TrackWidth > SMALL_NUMBER)
	{
		//clamp angle to avoid divided by 0
		InsideWheelSteeringAngle = FMath::Clamp(InsideWheelSteeringAngle, -89, 89);
		if (FMath::IsNearlyZero(InsideWheelSteeringAngle))	//if no steering
		{
			State.LeftWheelSteeringAngle = State.RightWheelSteeringAngle = 0.f;
		}
		else
		{
			float UnsignedOutsideWheelAngle;
			float UnsignedInsideWheelAngle = FMath::Abs(InsideWheelSteeringAngle);

			float InsideSteerRad = FMath::DegreesToRadians(UnsignedInsideWheelAngle);
			float TanInside = FMath::Tan(InsideSteerRad);
			float TanOutSide = State.WheelBase / (State.WheelBase / TanInside + AxleConfig.TrackWidth);
			float OutsideSteerRad = FMath::Atan(TanOutSide);
			UnsignedOutsideWheelAngle = FMath::RadiansToDegrees(OutsideSteerRad);
			UnsignedOutsideWheelAngle = FMath::Lerp(UnsignedInsideWheelAngle, UnsignedOutsideWheelAngle, AxleSteeringConfig.AckermannRatio);
			

			if (InSteeringInput > 0)
			{
				State.LeftWheelSteeringAngle = UnsignedOutsideWheelAngle;
				State.RightWheelSteeringAngle = UnsignedInsideWheelAngle;
			}
			else
			{
				State.LeftWheelSteeringAngle = -UnsignedInsideWheelAngle;
				State.RightWheelSteeringAngle = -UnsignedOutsideWheelAngle;
			}
		}
	}
	else
	{
		State.LeftWheelSteeringAngle = State.RightWheelSteeringAngle = InsideWheelSteeringAngle;
	}
}

void UVehicleAxleAssemblyComponent::UpdateSteeringAssist(float InSteeringInput)
{
	if (SteeringAssistConfig.bSteeringAssistEnabled)
	{
		float TargetInput = 0.f;

		FVector2D LocalVelocity2D = FVector2D(State.LocalLinearVelocity.X, State.LocalLinearVelocity.Y);
		FVector2D LocalVelocityDirection = LocalVelocity2D.GetSafeNormal();

		//get the slip angle of the carbody
		float CarbodySlipAngle = FMath::Asin(LocalVelocityDirection.Y);
		CarbodySlipAngle = FMath::RadiansToDegrees(CarbodySlipAngle);

		if (State.LocalLinearVelocity.X > SteeringAssistConfig.ActivationSpeed
			&& FMath::Abs(CarbodySlipAngle) > SteeringAssistConfig.ActivationAngle)
		{
			TargetInput  = FMath::GetMappedRangeValueClamped(
				FVector2D(-AxleSteeringConfig.MaxSteeringAngle, AxleSteeringConfig.MaxSteeringAngle), 
				FVector2D(-1.f, 1.f), 
				CarbodySlipAngle * SteeringAssistConfig.Level
			);
		}

		if (SteeringAssistConfig.MaxSteerRate > SMALL_NUMBER)
		{
			State.SteeringAssistInput = FMath::FInterpConstantTo(
				State.SteeringAssistInput,
				TargetInput,
				State.PhysicsDeltaTime,
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
	FQuat4f CarbodyRot = FQuat4f();
	if (WheelL)
	{
		LeftWorldVel = WheelL->GetWorldLinearVelocity();
		CarbodyRot = (FQuat4f)WheelL->GetCarbodyAsyncWorldTransform().GetRotation();
	}
	if (WheelR)
	{
		RightWorldVel = WheelR->GetWorldLinearVelocity();
		CarbodyRot = (FQuat4f)WheelR->GetCarbodyAsyncWorldTransform().GetRotation();
	}
	State.WorldLinearVelocity = State.NumOfWheels ? 
		FVector3f(LeftWorldVel + RightWorldVel) / State.NumOfWheels : FVector3f(0.f);

	CarbodyRot.Normalize();
	State.LocalLinearVelocity = CarbodyRot.UnrotateVector(State.WorldLinearVelocity);
}

void UVehicleAxleAssemblyComponent::UpdateSwaybarForce(
	UVehicleWheelComponent* WheelL, 
	UVehicleWheelComponent* WheelR)
{
	float HeightL = WheelL->GetKnuckleRelativePosition().Z;
	float HeightR = WheelR->GetKnuckleRelativePosition().Z;
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

void UVehicleAxleAssemblyComponent::UpdateIndependentSuspensionPhysics(
	UVehicleWheelComponent* WheelL, 
	UVehicleWheelComponent* WheelR)
{
	WheelR->UpdatePhysics(
		State.PhysicsDeltaTime,
		State.RightDriveTorque,
		State.BrakeTorque,
		State.HandbrakeTorque,
		State.RightWheelSteeringAngle,
		-State.SwaybarForce,
		State.ReflectedInertiaOnWheel);
	WheelL->UpdatePhysics(
		State.PhysicsDeltaTime,
		State.LeftDriveTorque,
		State.BrakeTorque,
		State.HandbrakeTorque,
		State.LeftWheelSteeringAngle,
		State.SwaybarForce,
		State.ReflectedInertiaOnWheel);
}

void UVehicleAxleAssemblyComponent::UpdateSolidAxlePhysics(
	UVehicleWheelComponent* WheelL, 
	UVehicleWheelComponent* WheelR)
{
	FVector LeftWorldPos = FVector(0.f);
	FVector RightWorldPos = FVector(0.f);

	FVehicleSuspensionSimContext LeftCtx;
	WheelL->StartUpdateSolidAxlePhysics(State.LeftWheelSteeringAngle, LeftWorldPos, LeftCtx);

	FVehicleSuspensionSimContext RightCtx;
	WheelR->StartUpdateSolidAxlePhysics(State.RightWheelSteeringAngle, RightWorldPos, RightCtx);

	// get the world direction of the axle
	FVector AxleDirection = (RightWorldPos - LeftWorldPos).GetSafeNormal();

	// the center of axle under world coordinate
	FVector AxleWorldCenter = (RightWorldPos + LeftWorldPos) * 0.5f;

	// the track width
	float DynTrackWidth = FMath::Abs(WheelL->GetRelativeLocation().Y - WheelR->GetRelativeLocation().Y);
	DynTrackWidth += WheelL->SuspensionKinematicsConfig.ArmLength + WheelR->SuspensionKinematicsConfig.ArmLength;

	// get the position of the ball joint(connecting the wheel and the suspension) of each wheel
	FVector LeftKnuckleWorldPos = AxleWorldCenter - AxleDirection * DynTrackWidth * 0.5f;
	FVector RightKnuckleWorldPos = AxleWorldCenter + AxleDirection * DynTrackWidth * 0.5f;

	WheelL->FinalizeUpdateSolidAxlePhysics(
		State.PhysicsDeltaTime,
		State.LeftDriveTorque,
		State.BrakeTorque,
		State.HandbrakeTorque,
		State.SwaybarForce,
		State.ReflectedInertiaOnWheel,
		LeftCtx,
		LeftKnuckleWorldPos,
		AxleDirection);
	WheelR->FinalizeUpdateSolidAxlePhysics(
		State.PhysicsDeltaTime,
		State.RightDriveTorque,
		State.BrakeTorque,
		State.HandbrakeTorque,
		-State.SwaybarForce,
		State.ReflectedInertiaOnWheel,
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
		if (LeftWheel.IsValid())
		{
			UVehicleWheelComponent::CopyWheelConfig(TemplateWheel, LeftWheel.Get());
		}
		if (RightWheel.IsValid())
		{
			UVehicleWheelComponent::CopyWheelConfig(TemplateWheel, RightWheel.Get());
		}
	}

	UpdateTrackWidth();

	if (LeftWheel.IsValid())
	{
		LeftWheel->InitializeWheel();
	}
	if (RightWheel.IsValid())
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
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_AxleAssembly_UpdatePhysics);

	if (!Carbody.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("AxleAssembly: No Valid Carbody"));
		return;
	}

	if(!Differential.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("AxleAssembly: No Valid Differential"));
		return;
	}


	UVehicleWheelComponent* WheelL = LeftWheel.Get();
	UVehicleWheelComponent* WheelR = RightWheel.Get();

	State.NumOfWheels = (WheelL != nullptr) + (WheelR != nullptr);
	if (State.NumOfWheels <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AxleAssembly: No Valid Wheels"));
		return;
	}

	State.PhysicsDeltaTime = InPhysicsDeltaTime;

	State.BrakeTorque = AxleConfig.MaxBrakeTorque * FMath::Clamp(InBrakeInput, 0.f, 1.f);
	if (AxleConfig.bAffectedByHandbrake)
	{
		State.HandbrakeTorque = AxleConfig.MaxHandbrakeTorque * FMath::Clamp(InHandbrakeInput, 0.f, 1.f);
	}
	else
	{
		State.HandbrakeTorque = 0;
	}

	UpdateLinearVelocity(WheelL, WheelR);

	if (AxleSteeringConfig.bAffectedBySteering)
	{
		UpdateSteeringAssist(InSteeringInput);
		UpdateSteering(State.RealSteeringValue);
	}
	
	if (State.NumOfWheels == 2)
	{
		UpdateTwoWheelAxle(WheelL, WheelR, InDriveTorque, InReflectedInertia);
	}
	else if (State.NumOfWheels == 1)
	{
		UpdateSingleWheelAxle(WheelL, WheelR, InDriveTorque, InReflectedInertia);
	}
	else
	{
		// if no wheels
		State.AxleTotalInertia = AxleConfig.DriveShaftInertia;
		State.AxleDriveTorque = InDriveTorque;
		float AngAcc = (State.AxleTotalInertia > SMALL_NUMBER) ? State.AxleDriveTorque / State.AxleTotalInertia : 0.f;
		State.AxleAngularVelocity += AngAcc * State.PhysicsDeltaTime;
	}

	OutAxleTotalInertia = State.AxleTotalInertia;
	OutAngularVelocity = State.AxleAngularVelocity;
}

void UVehicleAxleAssemblyComponent::SetWheelPosition(float NewTrackWidth)
{
	//set left wheel
	if (LeftWheel.IsValid())
	{
		if (Carbody.IsValid() && LeftWheel->GetAttachParent() != Carbody)
		{
			LeftWheel->AttachToComponent(Carbody.Get(), FAttachmentTransformRules::KeepRelativeTransform);
		}

		FVehicleSuspensionKinematicsConfig& KineConfig = LeftWheel->SuspensionKinematicsConfig;
		FQuat WheelCompRot = FQuat(FRotator(VehicleWheelComponentSetupRotation.Pitch, -VehicleWheelComponentSetupRotation.Yaw, VehicleWheelComponentSetupRotation.Roll));
		FVector WheelCompPos = FVector(GetRelativeLocation().X, GetRelativeLocation().Y - FMath::Abs(NewTrackWidth * 0.5), GetRelativeLocation().Z);
		FQuat WheelAlignmentRot = (FQuat)FVehicleSuspensionSolver::GetSpindleMountQuat(
			KineConfig.SpindleMountRotation, -1.f);
		WheelCompPos += WheelAlignmentRot.GetRightVector() * KineConfig.AxialHubOffset;
		WheelCompPos += WheelCompRot.GetRightVector() * KineConfig.ArmLength;
		LeftWheel->SetRelativeTransform(FTransform(WheelCompRot, WheelCompPos));
	}

	//set right wheel
	if (RightWheel.IsValid())
	{
		if (Carbody.IsValid() && RightWheel->GetAttachParent() != Carbody)
		{
			RightWheel->AttachToComponent(Carbody.Get(), FAttachmentTransformRules::KeepRelativeTransform);
		}

		FVehicleSuspensionKinematicsConfig& KineConfig = RightWheel->SuspensionKinematicsConfig;
		FQuat WheelCompRot = FQuat(FRotator(VehicleWheelComponentSetupRotation.Pitch, VehicleWheelComponentSetupRotation.Yaw, -VehicleWheelComponentSetupRotation.Roll));
		FVector WheelCompPos = FVector(GetRelativeLocation().X, GetRelativeLocation().Y + FMath::Abs(NewTrackWidth * 0.5), GetRelativeLocation().Z);
		FQuat WheelAlignmentRot = (FQuat)FVehicleSuspensionSolver::GetSpindleMountQuat(
			KineConfig.SpindleMountRotation, 1.f);
		WheelCompPos -= WheelAlignmentRot.GetRightVector() * KineConfig.AxialHubOffset;
		WheelCompPos -= WheelCompRot.GetRightVector() * KineConfig.ArmLength;
		RightWheel->SetRelativeTransform(FTransform(WheelCompRot, WheelCompPos));
	}
}

void UVehicleAxleAssemblyComponent::UpdateTrackWidth()
{
	SetWheelPosition(AxleConfig.TrackWidth);
}

void UVehicleAxleAssemblyComponent::UpdateSolidAxleAnim(
	USceneComponent* InSolidAxleMesh, 
	EVehicleSolidAxleAnimPivot AxleMeshAnchorPoint)
{
	if (!LeftWheel.IsValid() || !RightWheel.IsValid())return;

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

void UVehicleAxleAssemblyComponent::ApplySolidAxleStateDirect(float InExtensionRatio, float SteeringAngle)
{
	if (!LeftWheel.IsValid() || !RightWheel.IsValid())return;

	const int32 Iteration = 2;
	for (int32 i = 0; i < Iteration; i++)
	{
		FVector LeftWorldPos = FVector(0.f);
		FVector RightWorldPos = FVector(0.f);

		FVehicleSuspensionSimContext LeftCtx;
		LeftWheel->StartApplySolidAxleStateDirect(InExtensionRatio, SteeringAngle, LeftWorldPos, LeftCtx);

		FVehicleSuspensionSimContext RightCtx;
		RightWheel->StartApplySolidAxleStateDirect(InExtensionRatio, SteeringAngle, RightWorldPos, RightCtx);

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

		LeftWheel->FinalizeApplySolidAxleStateDirect(
			LeftCtx,
			LeftKnuckleWorldPos,
			AxleDirection);
		RightWheel->FinalizeApplySolidAxleStateDirect(
			RightCtx,
			RightKnuckleWorldPos,
			AxleDirection);
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
	int32 n = LeftWheel.IsValid() + RightWheel.IsValid();

	if (n == 2)
	{
		return 0.5f * (FVector3f)(LeftWheel->GetRelativeLocation() + RightWheel->GetRelativeLocation());
	}
	else
	{
		if (LeftWheel.IsValid())return (FVector3f)LeftWheel->GetRelativeLocation();
		if (RightWheel.IsValid())return (FVector3f)RightWheel->GetRelativeLocation();
		return FVector3f(0.f);
	}
}

bool UVehicleAxleAssemblyComponent::GenerateWheels()
{
	// if use Component
	if (bUseExistingWheelComponent)
	{
		if (LeftWheel.IsValid())
		{
			LeftWheel->DestroyComponent();
		}
		if (RightWheel.IsValid())
		{
			RightWheel->DestroyComponent();
		}
		return false;
	}

	//Destroy unused wheels
	if (LeftWheel.IsValid() && AxleLayout == EVehicleAxleLayout::SingleRight)
	{
		LeftWheel->DestroyComponent();
	}
	if (RightWheel.IsValid() && AxleLayout == EVehicleAxleLayout::SingleLeft)
	{
		RightWheel->DestroyComponent();
	}

	AActor* Owner = GetOwner();
	if (!IsValid(Owner))return false;

	//Generate left wheel
	if (!LeftWheel.IsValid() && AxleLayout != EVehicleAxleLayout::SingleRight)
	{
		LeftWheel = Cast<UVehicleWheelComponent>
			(Owner->AddComponentByClass(UVehicleWheelComponent::StaticClass(), false, FTransform(), false));
	}

	//Generate right wheel
	if (!RightWheel.IsValid() && AxleLayout != EVehicleAxleLayout::SingleLeft)
	{
		RightWheel = Cast<UVehicleWheelComponent>
			(Owner->AddComponentByClass(UVehicleWheelComponent::StaticClass(), false, FTransform(), false));
	}

	switch (AxleLayout)
	{
	case EVehicleAxleLayout::TwoWheels:
		return LeftWheel.IsValid() && RightWheel.IsValid();
		break;
	case EVehicleAxleLayout::SingleLeft:
		return LeftWheel.IsValid();
		break;
	case EVehicleAxleLayout::SingleRight:
		return RightWheel.IsValid();
		break;
	default:
		return LeftWheel.IsValid() && RightWheel.IsValid();
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
			LeftWheel = UVehicleUtil::GetComponentByName<UVehicleWheelComponent>(Owner, LeftWheelComponentName);
		}
		if (!RightWheel.IsValid())
		{
			RightWheel = UVehicleUtil::GetComponentByName<UVehicleWheelComponent>(Owner, RightWheelComponentName);
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
			Differential = UVehicleUtil::GetComponentByName<UVehicleDifferentialComponent>(Owner, DifferentialComponentName);
			bExistingDiffFound = Differential.IsValid();
		}

		// if not found, generate one
		if (!bExistingDiffFound)
		{
			//UE_LOG(LogTemp, Warning, TEXT("AxleAssembly: GeneratingDifferential"));
			if (DifferentialConfig)
			{
				Differential = Cast<UVehicleDifferentialComponent>
					(Owner->AddComponentByClass(DifferentialConfig, false, FTransform(), false));
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
	return UVehicleUtil::GetNamesOfComponentsOfActor<UVehicleWheelComponent>(this);
}

TArray<FName> UVehicleAxleAssemblyComponent::GetNamesOfDifferentialsOfOwner()
{
	return UVehicleUtil::GetNamesOfComponentsOfActor<UVehicleDifferentialComponent>(this);
}
