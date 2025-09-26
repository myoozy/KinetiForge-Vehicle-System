// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)


#include "VehicleAxleAssemblyComponent.h"
#include "VehicleWheelComponent.h"
#include "VehicleDifferentialComponent.h"
#include "VehicleWheelCoordinatorComponent.h"
#include "AsyncTickFunctions.h"

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
	WheelCoordinator = UVehicleWheelCoordinatorComponent::FindWheelCoordinator(Carbody);
	if (IsValid(WheelCoordinator))WheelCoordinator->RegisterAxle(this);
}

void UVehicleAxleAssemblyComponent::OnRegister()
{
	Super::OnRegister();

	//...
	ParentActor = GetOwner();
	Carbody = UVehicleWheelCoordinatorComponent::FindPhysicalParent(this);
	//PreviewWheelMesh();
	GenerateComponents();
}

void UVehicleAxleAssemblyComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	if (Differential && !Differential->IsBeingDestroyed())
	{
		Differential->DestroyComponent();
		Differential = nullptr;
	}
	if (LeftWheel && !LeftWheel->IsBeingDestroyed())
	{
		LeftWheel->DestroyComponent();
		LeftWheel = nullptr;
	}
	if (RightWheel && !RightWheel->IsBeingDestroyed())
	{
		RightWheel->DestroyComponent();
		RightWheel = nullptr;
	}

	if (Carbody)Carbody = nullptr;

	if (ParentActor)ParentActor = nullptr;
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
		SimData.LeftDriveTorque = SimData.RightDriveTorque = DiffGearRatio * SimData.AxleDriveTorque;
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
	FVector LeftWorldVel, RightWorldVel = FVector(0.f);
	if (IsValid(LeftWheel))LeftWorldVel = LeftWheel->GetWorldLinaerVelocity();
	if (IsValid(RightWheel))RightWorldVel = RightWheel->GetWorldLinaerVelocity();
	SimData.WorldLinearVelocity = (LeftWorldVel + RightWorldVel) / SimData.NumOfWheels;
	SimData.LocalLinearVelocity = UAsyncTickFunctions::ATP_GetTransform(Carbody).InverseTransformVectorNoScale(SimData.WorldLinearVelocity);
}

void UVehicleAxleAssemblyComponent::UpdateSwaybarForce()
{
	SimData.SwaybarForce = AxleConfig.SwaybarStiffness * 0.5 * (RightWheel->GetSuspensionLength() - LeftWheel->GetSuspensionLength());
}

void UVehicleAxleAssemblyComponent::UpdateTCS(float TargetDriveTorque)
{
	if (!TCSConfig.bTractionControlSystemEnabled || !SimData.NumOfWheelOnGround)
	{
		SimData.AxleDriveTorque = TargetDriveTorque;
		SimData.bTCSTriggered = false;
	}
	else
	{
		float SumSlipRatio = 0.f;
		if (IsValid(LeftWheel))SumSlipRatio += FMath::Abs(LeftWheel->GetSlipRatio());
		if (IsValid(RightWheel))SumSlipRatio += FMath::Abs(RightWheel->GetSlipRatio());
		float AvrgSlipRatio = SumSlipRatio / SimData.NumOfWheels;

		SimData.bTCSTriggered = 
			AvrgSlipRatio > TCSConfig.MaxSlipRatio && 
			FMath::Abs(SimData.LocalLinearVelocity.X) > TCSConfig.ActivationSpeed && 
			TargetDriveTorque != 0;

		if (SimData.bTCSTriggered)TargetDriveTorque = 0;

		SimData.AxleDriveTorque = 
			FMath::FInterpTo(
				SimData.AxleDriveTorque,
				TargetDriveTorque,
				SimData.PhysicsDeltaTime,
				TCSConfig.InterpSpeed);
	}
}

// Called every frame
void UVehicleAxleAssemblyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//make sure the wheel preview is destoryed
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
	else
	{
		UpdateSingleWheelAxle(InDriveTorque, InReflectedInertia);
	}

	OutAxleTotalInertia = SimData.AxleTotalInertia;
	OutAngularVelocity = SimData.AxleAngularVelocity;
}

void UVehicleAxleAssemblyComponent::SetWheelPosition(float NewTrackWidth)
{
	//set left wheel
	if (IsValid(LeftWheel))
	{
		FQuat WheelCompRot = FQuat(FRotator(VehicleWheelComponentSetupRotation.Pitch, -VehicleWheelComponentSetupRotation.Yaw, VehicleWheelComponentSetupRotation.Roll));
		FVector WheelCompPos = FVector(GetRelativeLocation().X, GetRelativeLocation().Y - FMath::Abs(NewTrackWidth * 0.5), GetRelativeLocation().Z);
		FQuat WheelAlignmentRot = FQuat(FRotator(0, -LeftWheel->SuspensionKinematicsConfig.BaseToe, -LeftWheel->SuspensionKinematicsConfig.BaseCamber));
		WheelCompPos += WheelAlignmentRot.GetRightVector() * LeftWheel->SuspensionKinematicsConfig.SteeringAxleOffset;
		LeftWheel->SetRelativeTransform(FTransform(WheelCompRot, WheelCompPos));
	}

	//set right wheel
	if (IsValid(RightWheel))
	{
		FQuat WheelCompRot = FQuat(FRotator(VehicleWheelComponentSetupRotation.Pitch, VehicleWheelComponentSetupRotation.Yaw, -VehicleWheelComponentSetupRotation.Roll));
		FVector WheelCompPos = FVector(GetRelativeLocation().X, GetRelativeLocation().Y + FMath::Abs(NewTrackWidth * 0.5), GetRelativeLocation().Z);
		FQuat WheelAlignmentRot = FQuat(FRotator(0, RightWheel->SuspensionKinematicsConfig.BaseToe, RightWheel->SuspensionKinematicsConfig.BaseCamber));
		WheelCompPos -= WheelAlignmentRot.GetRightVector() * RightWheel->SuspensionKinematicsConfig.SteeringAxleOffset;
		RightWheel->SetRelativeTransform(FTransform(WheelCompRot, WheelCompPos));
	}
}

void UVehicleAxleAssemblyComponent::UpdateTrackWidth()
{
	SetWheelPosition(AxleConfig.TrackWidth);
}

void UVehicleAxleAssemblyComponent::GetLinearVelocity(FVector& OutLocalVelocity, FVector& OutWorldVelocity)
{
	OutLocalVelocity = SimData.LocalLinearVelocity;
	OutWorldVelocity = SimData.WorldLinearVelocity;
}

FVector UVehicleAxleAssemblyComponent::GetAxleCenter()
{
	int32 n = IsValid(LeftWheel) + IsValid(RightWheel);

	if (n == 2)
	{
		return 0.5 * (LeftWheel->GetRelativeLocation() + RightWheel->GetRelativeLocation());
	}
	else
	{
		if (IsValid(LeftWheel))return LeftWheel->GetRelativeLocation();
		if (IsValid(RightWheel))return RightWheel->GetRelativeLocation();
		return FVector(0.f);
	}
}

bool UVehicleAxleAssemblyComponent::GenerateWheels()
{
	if (!ParentActor)return false;
	if (!Carbody)return false;
	
	//set left wheel
	if (!IsValid(LeftWheel) && AxleLayout != EVehicleAxleLayout::SingleRight)
	{
		//UE_LOG(LogTemp, Warning, TEXT("AxleAssembly: GeneratingLeftWheel"));
		// check if user has set wheel class
		if (WheelConfig)
		{
			LeftWheel = Cast<UVehicleWheelComponent>
				(ParentActor->AddComponentByClass(WheelConfig, true, FTransform(), true));
		}
		else
		{
			LeftWheel = Cast<UVehicleWheelComponent>
				(ParentActor->AddComponentByClass(UVehicleWheelComponent::StaticClass(), true, FTransform(), true));
		}
		LeftWheel->AttachToComponent(Carbody, FAttachmentTransformRules::KeepRelativeTransform);
		ParentActor->FinishAddComponent(LeftWheel, false, FTransform());
	}

	//set right wheel
	if (!IsValid(RightWheel) && AxleLayout != EVehicleAxleLayout::SingleLeft)
	{
		//UE_LOG(LogTemp, Warning, TEXT("AxleAssembly: GeneratingRightWheel"));
		if (WheelConfig)
		{
			RightWheel = Cast<UVehicleWheelComponent>
				(ParentActor->AddComponentByClass(WheelConfig, true, FTransform(), true));
		}
		else
		{
			RightWheel = Cast<UVehicleWheelComponent>
				(ParentActor->AddComponentByClass(UVehicleWheelComponent::StaticClass(), true, FTransform(), true));
		}
		RightWheel->AttachToComponent(Carbody, FAttachmentTransformRules::KeepRelativeTransform);
		ParentActor->FinishAddComponent(RightWheel, false, FTransform());
	}

	UpdateTrackWidth();
	if (IsValid(LeftWheel))
	{
		LeftWheel->RefreshWheelMesh();
	}
	if (IsValid(RightWheel))
	{
		RightWheel->RefreshWheelMesh();
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

bool UVehicleAxleAssemblyComponent::GenerateDifferential()
{
	//set differential
	if (!IsValid(Differential))
	{
		//UE_LOG(LogTemp, Warning, TEXT("AxleAssembly: GeneratingDifferential"));
		if (DifferentialConfig)
		{
			Differential = Cast<UVehicleDifferentialComponent>
				(ParentActor->AddComponentByClass(DifferentialConfig, false, FTransform(), false));
		}
		else
		{
			Differential = Cast<UVehicleDifferentialComponent>
				(ParentActor->AddComponentByClass(UVehicleDifferentialComponent::StaticClass(), false, FTransform(), false));
		}
	}

	return IsValid(Differential);
}

bool UVehicleAxleAssemblyComponent::GenerateComponents()
{
	return GenerateWheels() && GenerateDifferential();
}
