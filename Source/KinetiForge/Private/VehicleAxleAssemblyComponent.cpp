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
	FindWheelCoordinator();
	//GenerateComponents();

}

void UVehicleAxleAssemblyComponent::OnRegister()
{
	Super::OnRegister();

	//...
	ParentActor = GetOwner();
	Carbody = FindPhysicalParent();
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

void UVehicleAxleAssemblyComponent::UpdateSteering(float InSteeringInput)
{
	float InsideWheelSteeringAngle = InSteeringInput * AxleSteeringConfig.MaxSteeringAngle;
	if (AxleSteeringConfig.bAckermannSteering && SimData.WheelBase > SMALL_NUMBER && AxleConfig.TrackWidth > SMALL_NUMBER)
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

		//because sin(x)≈x when x is small, and we don't need this to be accurate
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
	SimData.WorldLinearVelocity = (LeftWheel->GetWorldLinaerVelocity() + RightWheel->GetWorldLinaerVelocity()) * 0.5;
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
		float AvrgSlipRatio = (FMath::Abs(LeftWheel->GetSlipRatio()) + FMath::Abs(RightWheel->GetSlipRatio())) * 0.5;

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

	if (!(IsValid(LeftWheel) && IsValid(RightWheel)))
	{
		UE_LOG(LogTemp, Warning, TEXT("AxleAssembly: No Valid Wheels"));
		return;
	}
	if (!IsValid(Carbody))
	{
		UE_LOG(LogTemp, Warning, TEXT("AxleAssembly: No Valid Carbody"));
		return;
	}

	SimData.PhysicsDeltaTime = InPhysicsDeltaTime;

	CalculateLinearVelocity();

	if (AxleSteeringConfig.bAffectedBySteering)
	{
		UpdateSteeringAssist(InSteeringInput);
		UpdateSteering(SimData.RealSteeringValue);
	}

	UpdateSwaybarForce();

	float DiffGearRatioSqr = Differential->Config.GearRatio * Differential->Config.GearRatio;
	if (AxleConfig.TorqueWeight <= 0 && FMath::IsNearlyZero(SimData.P3MotorTorque))
	{
		SimData.ReflectedInertiaOnWheel = 0.f;
		SimData.LeftDriveTorque = SimData.RightDriveTorque = 0.f;
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

	SimData.BrakeTorque = AxleConfig.MaxBrakeTorque * FMath::Clamp(InBrakeInput, 0.f, 1.f);
	if (AxleConfig.bAffectedByHandbrake)
	{
		SimData.HandbrakeTorque = AxleConfig.MaxHandbrakeTorque * FMath::Clamp(InHandbrakeInput, 0.f, 1.f);
	}
	else
	{
		SimData.HandbrakeTorque = 0;
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

	OutAxleTotalInertia = SimData.AxleTotalInertia = ReflectedInertiaOfWheels + AxleConfig.DriveShaftInertia;
	OutAngularVelocity = SimData.AxleAngularVelocity;
}

void UVehicleAxleAssemblyComponent::GetLinearVelocity(FVector& OutLocalVelocity, FVector& OutWorldVelocity)
{
	OutLocalVelocity = SimData.LocalLinearVelocity;
	OutWorldVelocity = SimData.WorldLinearVelocity;
}

FVector UVehicleAxleAssemblyComponent::GetAxleCenter()
{
	return 0.5 * (LeftWheel->GetRelativeLocation() + RightWheel->GetRelativeLocation());
}

UPrimitiveComponent* UVehicleAxleAssemblyComponent::FindPhysicalParent()
{
	if (USceneComponent* tempParent = GetAttachParent())
	{
		if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(tempParent))
		{
			return Primitive;
		}
	}

	// Fallback: 尝试找 Owner 的 RootComponent（通常是 mesh）
	if (AActor* tempOwner = GetOwner())
	{
		if (UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(tempOwner->GetRootComponent()))
		{
			return RootPrimitive;
		}
	}

	return nullptr;
}

bool UVehicleAxleAssemblyComponent::FindWheelCoordinator()
{
	//if already found (eg. founction is called multiple times)
	if (WheelCoordinator)return true;

	//ckeck valid carbody
	if (!Carbody)
	{
		//UE_LOG
		return false;
	}

	TArray<USceneComponent*> Children;
	Carbody->GetChildrenComponents(true, Children);
	for (USceneComponent* Child : Children)
	{
		if (UVehicleWheelCoordinatorComponent* Coord = Cast<UVehicleWheelCoordinatorComponent>(Child))
		{
			WheelCoordinator = Coord;
			WheelCoordinator->RegisterAxle(this);
			return true;
		}
	}

	//if not found
	//create one
	WheelCoordinator = NewObject<UVehicleWheelCoordinatorComponent>(this);
	WheelCoordinator->AttachToComponent(Carbody, FAttachmentTransformRules::KeepRelativeTransform);
	GetOwner()->AddInstanceComponent(WheelCoordinator);
	WheelCoordinator->RegisterComponent();
	WheelCoordinator->RegisterAxle(this);
	return false;

}

bool UVehicleAxleAssemblyComponent::GenerateComponents()
{
	if (!ParentActor)return false;
	if (!Carbody)return false;

	//set left wheel
	FQuat TempRot = FQuat(FRotator(VehicleWheelComponentSetupRotation.Pitch, -VehicleWheelComponentSetupRotation.Yaw, VehicleWheelComponentSetupRotation.Roll));
	FVector TempPos = FVector(GetRelativeLocation().X, GetRelativeLocation().Y - FMath::Abs(AxleConfig.TrackWidth * 0.5), GetRelativeLocation().Z);
	if (!IsValid(LeftWheel))
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
			LeftWheel = NewObject<UVehicleWheelComponent>(Carbody);
		}
		//setup attachment and transform
		LeftWheel->AttachToComponent(Carbody, FAttachmentTransformRules::KeepRelativeTransform);
		TempPos += FQuat(FRotator(0, -LeftWheel->SuspensionKinematicsConfig.BaseToe, -LeftWheel->SuspensionKinematicsConfig.BaseCamber)).GetRightVector() * LeftWheel->SuspensionKinematicsConfig.SteeringAxleOffset;
		if (WheelConfig)
		{
			ParentActor->FinishAddComponent(LeftWheel, false, FTransform(TempRot, TempPos));
		}
		else
		{
			LeftWheel->SetRelativeTransform(FTransform(TempRot, TempPos));
			ParentActor->AddInstanceComponent(LeftWheel);
			LeftWheel->RegisterComponent();
		}
	}

	//set right wheel
	TempRot = FQuat(FRotator(VehicleWheelComponentSetupRotation.Pitch, VehicleWheelComponentSetupRotation.Yaw, -VehicleWheelComponentSetupRotation.Roll));
	TempPos = FVector(GetRelativeLocation().X, GetRelativeLocation().Y + FMath::Abs(AxleConfig.TrackWidth * 0.5), GetRelativeLocation().Z);
	if (!IsValid(RightWheel))
	{
		//UE_LOG(LogTemp, Warning, TEXT("AxleAssembly: GeneratingRightWheel"));
		if (WheelConfig)
		{
			RightWheel = Cast<UVehicleWheelComponent>
				(ParentActor->AddComponentByClass(WheelConfig, true, FTransform(), true));
		}
		else
		{
			RightWheel = NewObject<UVehicleWheelComponent>(Carbody);
		}
		RightWheel->AttachToComponent(Carbody, FAttachmentTransformRules::KeepRelativeTransform);
		TempPos -= FQuat(FRotator(0, RightWheel->SuspensionKinematicsConfig.BaseToe, RightWheel->SuspensionKinematicsConfig.BaseCamber)).GetRightVector() * RightWheel->SuspensionKinematicsConfig.SteeringAxleOffset;
		if (WheelConfig)
		{
			ParentActor->FinishAddComponent(RightWheel, false, FTransform(TempRot, TempPos));
		}
		else
		{
			RightWheel->SetRelativeTransform(FTransform(TempRot, TempPos));
			ParentActor->AddInstanceComponent(RightWheel);
			RightWheel->RegisterComponent();
		}
	}

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
			Differential = NewObject<UVehicleDifferentialComponent>(this);
			ParentActor->AddInstanceComponent(Differential);
			Differential->RegisterComponent();
		}

	}

	return IsValid(LeftWheel) && IsValid(RightWheel) && IsValid(Differential);
}
