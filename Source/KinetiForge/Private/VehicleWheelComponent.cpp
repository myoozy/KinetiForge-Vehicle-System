// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)


#include "VehicleWheelComponent.h"
#include "VehicleWheelCoordinatorComponent.h"
#include "AsyncTickFunctions.h"

// Sets default values for this component's properties
UVehicleWheelComponent::UVehicleWheelComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PrePhysics;

	// ...
	//load default curves
	if (!TireConfig.Fx)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> DefaultMuLongCurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultFx.DefaultFx'")
		);
		if (DefaultMuLongCurveObj.Succeeded())
		{
			TireConfig.Fx = DefaultMuLongCurveObj.Object;
		}
	}
	
	if (!TireConfig.Fy)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> DefaultMuLatCurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultFy.DefaultFy'")
		);
		if (DefaultMuLatCurveObj.Succeeded())
		{
			TireConfig.Fy = DefaultMuLatCurveObj.Object;
		}
	}

	if (!TireConfig.Gx)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> DefaultMuLongCurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultGx.DefaultGx'")
		);
		if (DefaultMuLongCurveObj.Succeeded())
		{
			TireConfig.Gx = DefaultMuLongCurveObj.Object;
		}
	}

	if (!TireConfig.Gy)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> DefaultMuLatCurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultGy.DefaultGy'")
		);
		if (DefaultMuLatCurveObj.Succeeded())
		{
			TireConfig.Gy = DefaultMuLatCurveObj.Object;
		}
	}

	if (!SuspensionKinematicsConfig.CamberCurve)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> DefaultCamberCurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultCamberCurve.DefaultCamberCurve'")
		);
		if (DefaultCamberCurveObj.Succeeded())
		{
			SuspensionKinematicsConfig.CamberCurve = DefaultCamberCurveObj.Object;
		}
	}

	//load default wheel mesh
	if (!WheelMesh)
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultWheelMeshObj(
			TEXT("/Script/Engine.StaticMesh'/KinetiForge/DefaultConfigs/Meshes/DefaultWheel_235R33.DefaultWheel_235R33'")
		);
		if (DefaultWheelMeshObj.Succeeded())
		{
			WheelMesh = DefaultWheelMeshObj.Object;
		}
	}
}

// Called when the game starts
void UVehicleWheelComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	WheelCoordinator = UVehicleWheelCoordinatorComponent::FindWheelCoordinator(Carbody);
	if (IsValid(WheelCoordinator))
	{
		WheelCoordinator->RegisterWheel(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WheelPhysics: WheelCoordinatorNotFound! Sprung mass can not be computed."));
	}
}

void UVehicleWheelComponent::OnRegister()
{
	Super::OnRegister();

	//...
	InitializeWheel();
}

void UVehicleWheelComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	if (IsValid(WheelKnuckleComponent) && !WheelKnuckleComponent->IsBeingDestroyed())
	{
		WheelKnuckleComponent->DestroyComponent();
		WheelKnuckleComponent = nullptr;
	}

	if (IsValid(WheelMeshComponent) && !WheelMeshComponent->IsBeingDestroyed())
	{
		WheelMeshComponent->DestroyComponent();
		WheelMeshComponent = nullptr;
	}
	if (IsValid(BrakeMeshComponent) && !BrakeMeshComponent->IsBeingDestroyed())
	{
		BrakeMeshComponent->DestroyComponent();
		BrakeMeshComponent = nullptr;
	}

	if (IsValid(WheelCoordinator) && !WheelCoordinator->IsBeingDestroyed()) { WheelCoordinator->NotifyWheelMoved(); }

	if (IsValid(CurrentWorld))CurrentWorld = nullptr;
	if (IsValid(Carbody))Carbody = nullptr;
	if (TireConfig.Fx)TireConfig.Fx = nullptr;
	if (TireConfig.Fy)TireConfig.Fy = nullptr;
	if (TireConfig.Gx)TireConfig.Gx = nullptr;
	if (TireConfig.Gy)TireConfig.Gy = nullptr;
	if (SuspensionKinematicsConfig.CamberCurve)SuspensionKinematicsConfig.CamberCurve = nullptr;
	if (SuspensionKinematicsConfig.ToeCurve)SuspensionKinematicsConfig.ToeCurve = nullptr;
	if (SuspensionKinematicsConfig.CasterCurve)SuspensionKinematicsConfig.CasterCurve = nullptr;

	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

bool UVehicleWheelComponent::GenerateMeshComponents()
{
	if (!IsValid(WheelKnuckleComponent))
	{
		WheelKnuckleComponent = Cast<USceneComponent>
			(GetOwner()->AddComponentByClass(USceneComponent::StaticClass(), false, FTransform(), false));
	}
	WheelKnuckleComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	if (!IsValid(WheelMeshComponent) && WheelKnuckleComponent)
	{
		WheelMeshComponent = Cast<UStaticMeshComponent>
			(GetOwner()->AddComponentByClass(UStaticMeshComponent::StaticClass(), false, FTransform(), false));
	}
	WheelMeshComponent->AttachToComponent(WheelKnuckleComponent, FAttachmentTransformRules::KeepRelativeTransform);

	if (!IsValid(BrakeMeshComponent) && WheelKnuckleComponent)
	{
		BrakeMeshComponent = Cast<UStaticMeshComponent>
			(GetOwner()->AddComponentByClass(UStaticMeshComponent::StaticClass(), false, FTransform(), false));
	}
	BrakeMeshComponent->AttachToComponent(WheelKnuckleComponent, FAttachmentTransformRules::KeepRelativeTransform);

	return RefreshWheelMesh();
}

void UVehicleWheelComponent::ApplyWheelForce()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UpdateVehicleWheelAddForce);

	if (!Suspension.SimData.HitStruct.bBlockingHit)return;

	const FHitResult& HitStruct = Suspension.SimData.HitStruct;

	FVector WidthBias = FVector(0.f);
	FVector PosToApplyImpulse = FVector(0.f);

	switch (SuspensionKinematicsConfig.PositionToApplyForce)
	{
	case EPositionToApplyForce::ImpactPoint:
		PosToApplyImpulse = HitStruct.ImpactPoint;
		break;
	case EPositionToApplyForce::WheelCenter:
		PosToApplyImpulse = Suspension.SimData.WheelWorldPos;
		break;
	default:
		PosToApplyImpulse = HitStruct.ImpactPoint;
		break;
	}

	/**** Anti-Pitch ****/
	// attention: the anti-pitch geometry does not work in this way in real life,
	// the logic will be fixed in the future (when I find a better solution)
	// the axis of the suspension is the forward vector of the component
	const FVector& ArmRelativeAxis = Suspension.SimData.ComponentRelativeForwardVector;
	FVector ArmAxis = Suspension.SimData.CarbodyWorldTransform.TransformVectorNoScale(ArmRelativeAxis);
	FVector ArmAxisProjOnGround = FVector::VectorPlaneProject(ArmAxis, HitStruct.ImpactNormal);
	float TireForceCausingAntiPitch = FVector::DotProduct(Wheel.SimData.TyreForce, ArmAxisProjOnGround.GetSafeNormal());
	FVector AntiPitchForce = (ArmAxis - ArmAxisProjOnGround) * TireForceCausingAntiPitch;

	float dt = Wheel.SimData.PhysicsDeltaTime;
	FVector SuspensionForceProj = Suspension.SimData.SuspensionForceVector.ProjectOnTo(HitStruct.ImpactNormal);
	FVector LinearImpulse = (Wheel.SimData.TyreForce + SuspensionForceProj + AntiPitchForce) * dt;

	if (Chaos::FRigidBodyHandle_Internal* RigidHandle = GetInternalHandle(Carbody, NAME_None))
	{
		// split linear force and angular force
		//FVector TotalImpulse = (Wheel.SimData.TyreForce + Suspension.SimData.SuspensionForceVector) * dt;
		const Chaos::FVec3 WorldCOM = Chaos::FParticleUtilitiesGT::GetCoMWorldPosition(RigidHandle);
		const Chaos::FVec3 AngularImpulse = Chaos::FVec3::CrossProduct(PosToApplyImpulse - WorldCOM, LinearImpulse);

		RigidHandle->SetLinearImpulse(RigidHandle->LinearImpulse() + LinearImpulse * 100, false);
		RigidHandle->SetAngularImpulse(RigidHandle->AngularImpulse() + AngularImpulse * 100, false);
	}

	// also add force to the contacted component
	if (IsValid(HitStruct.GetComponent()) &&
		HitStruct.GetComponent()->IsSimulatingPhysics())
	{
		UAsyncTickFunctions::ATP_AddImpulseAtPosition(
			HitStruct.GetComponent(),
			HitStruct.ImpactPoint, 
			LinearImpulse * -100,
			HitStruct.BoneName);
	}
}

// Called every frame
void UVehicleWheelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	// check if sprungmass should be updated
	FVector NewRelativeLocation = GetRelativeLocation();
	if ((CachedComponentRelativeLocation - NewRelativeLocation).SquaredLength() > 1.f)
	{
		if (IsValid(WheelCoordinator))WheelCoordinator->NotifyWheelMoved();
	}

	// check if suspension damping need to be updated
	Suspension.CheckIsDampingDirty();

	// update animation
	if (bUpdateAnimAutomatically)
	{
		UpdateWheelAnim(DeltaTime, 0);
	}

	// smoothen the slip
	Wheel.SmoothenSlip(DeltaTime, 10.f);
	/*
	* get the smoothend slip:
	* Wheel.SmoothendSlipRatio; Wheel.SmoothendSlipAngle;
	*/
}

Chaos::FRigidBodyHandle_Internal* UVehicleWheelComponent::GetInternalHandle(UPrimitiveComponent* Component, FName BoneName)
{
	if (IsValid(Component))
	{
		if (const FBodyInstance* BodyInstance = Component->GetBodyInstance(BoneName))
		{
			if (const auto Handle = BodyInstance->ActorHandle)
			{
				if (Chaos::FRigidBodyHandle_Internal* RigidHandle = Handle->GetPhysicsThreadAPI())
				{
					return RigidHandle;
				}
			}
		}
	}
	return nullptr;
}

void UVehicleWheelComponent::CopyWheelConfig(const UVehicleWheelComponent* Source, UVehicleWheelComponent* Target, bool bReInitialize)
{
	Target->WheelConfig = Source->WheelConfig;
	Target->TireConfig = Source->TireConfig;
	Target->ABSConfig = Source->ABSConfig;
	Target->SuspensionKinematicsConfig = Source->SuspensionKinematicsConfig;
	Target->SuspensionSpringConfig = Source->SuspensionSpringConfig;
	Target->bUpdateAnimAutomatically = Source->bUpdateAnimAutomatically;
	Target->AnimInterpSpeed = Source->AnimInterpSpeed;
	Target->WheelMesh = Source->WheelMesh;
	Target->WheelMeshTransform = Source->WheelMeshTransform;
	Target->BrakeMesh = Source->BrakeMesh;
	Target->BrakeMeshTransform = Source->BrakeMeshTransform;

	if (bReInitialize)
	{
		Target->InitializeWheel();
	}
}

void UVehicleWheelComponent::InitializeWheel()
{
	CurrentWorld = GetWorld();
	Carbody = UVehicleWheelCoordinatorComponent::FindPhysicalParent(this);
	if (IsValid(Carbody) && Carbody != GetAttachParent())
	{
		AttachToComponent(Carbody, FAttachmentTransformRules::KeepWorldTransform);
	}
	if (!IsValid(Carbody))
	{
		UE_LOG(LogTemp, Warning, TEXT("WheelPhysics: Carbody Not Found!"));
	}

	Suspension.Initialize(this);
	Wheel.Initialize(this);

	//initialize meshes
	GenerateMeshComponents();
}

void UVehicleWheelComponent::SetSprungMass(float NewSprungMass)
{
	Suspension.SetSprungMass(NewSprungMass);
}

float UVehicleWheelComponent::GetNormalizedSlip(float LongitudinalScale, float LateralScale)
{
	// if the slip velocity is too low, should scale the affection of slip ratio, 0.5 is just magic number
	float SpeedScaleX = FMath::Clamp(0.5f * Wheel.SimData.LongSlipVelocity, -1.f, 1.f);
	float NormalizedSx = Wheel.SmoothenedSlipRatio * LongitudinalScale * SpeedScaleX;

	// also if the lateral velocity is too low, should scale the affection of slip angle
	float SpeedScaleY = FMath::Clamp(0.5f * Wheel.SimData.LocalLinearVelocity.Y, -1.f, 1.f);
	float NormalizedSy = 0.011111f * Wheel.SmoothenedSlipAngle * LateralScale * SpeedScaleY;

	// combined slip
	float NormalizedSc = FMath::Sqrt(NormalizedSx * NormalizedSx + NormalizedSy * NormalizedSy);
	return FMath::Min(1.f, NormalizedSc);
}

void UVehicleWheelComponent::UpdatePhysics(
	float InPhysicsDeltaTime,
	float InDriveTorque,
	float InBrakeTorque, 
	float InHandbrakeTorque,
	float InSteeringAngle, 
	float InSwaybarForce, 
	float InReflectedInertia)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UpdateVehicleWheel);

	//check if carbody is valid
	if (!IsValid(Carbody))
	{
		UE_LOG(LogTemp, Warning, TEXT("WheelPhysics: No Valid CarBody!!!"));
		return;
	}

	//check if current world is valid
	if (!IsValid(CurrentWorld))
	{
		UE_LOG(LogTemp, Warning, TEXT("WheelPhysics: No Valid CurrentWorld!!!"));
		return;
	}

	//Update anim cache
	PrevKnucklePos2D = Suspension.SimData.KnucklePos2D;
	PrevWheelRelativeRot = Suspension.SimData.WheelRelativeTransform.GetRotation();
	TimeSinceLastPhysicsTick = 0.f;

	//Suspension
	Suspension.UpdateSuspension(
		InPhysicsDeltaTime, 
		InSteeringAngle,
		InSwaybarForce);

	//Wheel
	Wheel.UpdateWheel(
		InPhysicsDeltaTime,
		InDriveTorque,
		InBrakeTorque,
		InHandbrakeTorque,
		InReflectedInertia,
		Suspension.SimData);

	ApplyWheelForce();
}

void UVehicleWheelComponent::StartUpdateSolidAxlePhysics(float InSteeringAngle)
{
	Suspension.StartUpdateSolidAxle(InSteeringAngle);
}

void UVehicleWheelComponent::FinalizeUpdateSolidAxlePhysics(
	float InPhysicsDeltaTime,
	float InDriveTorque,
	float InBrakeTorque,
	float InHandbrakeTorque,
	float InSwaybarForce,
	float InReflectedInertia,
	const FVector& InKnuckleWorldPos,
	const FVector& InAxleWorldDirection
)
{
	Suspension.FinalizeUpdateSolidAxle(
		InPhysicsDeltaTime, 
		InSwaybarForce,
		InKnuckleWorldPos,
		InAxleWorldDirection);

	Wheel.UpdateWheel(
		InPhysicsDeltaTime,
		InDriveTorque,
		InBrakeTorque,
		InHandbrakeTorque,
		InReflectedInertia,
		Suspension.SimData);

	ApplyWheelForce();
}

void UVehicleWheelComponent::GetWheelCoordinator(UVehicleWheelCoordinatorComponent*& OutWheelCoordinator)
{
	OutWheelCoordinator = WheelCoordinator;
}

float UVehicleWheelComponent::ComputeFeedBackTorque()
{
	// get machanism trail
	FVector Arm = WheelConfig.Radius * Suspension.SimData.StrutDirection.ProjectOnToNormal(Suspension.SimData.HitStruct.ImpactNormal);
	FVector Torque = FVector::CrossProduct(Arm, Wheel.SimData.TyreForce);
	return FVector::DotProduct(Suspension.SimData.StrutDirection, Torque);
}

void UVehicleWheelComponent::SetInertiaTensor(UPrimitiveComponent* InComponent, FVector InInertia)
{
	if (!IsValid(InComponent)) return;
	FBodyInstance* BI = InComponent->GetBodyInstance();
	if (!BI || !BI->IsValidBodyInstance()) return;

	if (!FPhysicsInterface::IsValid(BI->ActorHandle) || !FPhysicsInterface::IsRigidBody(BI->ActorHandle))
	{
		// actor is not created, maybe try again later
		return;
	}


	// cm^2 to m^2
	InInertia *= 10000;

	// input check
	if (InInertia.X <= 0.f || InInertia.Y <= 0.f || InInertia.Z <= 0.f)
	{
		FVector OriginalInertia = BI->GetBodyInertiaTensor();
		if (InInertia.X <= 0)InInertia.X = OriginalInertia.X;
		if (InInertia.Y <= 0)InInertia.Y = OriginalInertia.X;
		if (InInertia.Z <= 0)InInertia.Z = OriginalInertia.X;
	}

	// setup in physical callback
	FPhysicsCommand::ExecuteWrite(BI->ActorHandle, [&](FPhysicsActorHandle& Actor)
	{
		FPhysicsInterface::SetMassSpaceInertiaTensor_AssumesLocked(Actor, InInertia);
	});
}

bool UVehicleWheelComponent::GetRayCastResult(FHitResult& OutHitResult, bool& OutRefinement)
{
	OutHitResult = Suspension.SimData.HitStruct;
	OutRefinement = Suspension.SimData.bRayCastRefined;
	return Suspension.SimData.bHitGround;
}

FVector UVehicleWheelComponent::GetRayCastHitLocation()
{
	float DistanceToRayCastStart = FMath::Max(0.f, Suspension.SimData.HitDistance - WheelConfig.Radius);
	FVector OffsetToRayCastStart = Suspension.SimData.ComponentUpVector * DistanceToRayCastStart;
	return Suspension.SimData.RayCastStartPos - OffsetToRayCastStart;
}

void UVehicleWheelComponent::DrawSuspension(float Duration, float Thickness, bool bDrawSuspension, bool bDrawWheel, bool bDrawRayCast)
{
	Suspension.DrawSuspension(Duration, Thickness, bDrawSuspension, bDrawWheel, bDrawRayCast);
}

void UVehicleWheelComponent::DrawSuspensionForce(float Duration, float Thickness, float Length)
{
	Suspension.DrawSuspensionForce(Duration, Thickness, Length);
}

void UVehicleWheelComponent::DrawWheelForce(float Duration, float Thickness, float Length, bool bDrawVelocity, bool bDrawSlip, bool bDrawInertia)
{
	Wheel.DrawWheelForce(CurrentWorld, Suspension.SimData, Duration, Thickness, Length, bDrawVelocity, bDrawSlip, bDrawInertia);
}

bool UVehicleWheelComponent::SetMesh(
	float AxialHubOffset,
	UStaticMesh* NewWheelMesh, 
	FTransform WheelMeshRelatvieTransform,
	UStaticMesh* NewBrakeMesh, 
	FTransform BrakeMeshRelativeTransform)
{
	if (!IsValid(WheelKnuckleComponent) || !IsValid(WheelMeshComponent) || !IsValid(BrakeMeshComponent))return false;

	WheelMeshRelatvieTransform.SetLocation(WheelMeshRelatvieTransform.GetLocation() + FVector(0, AxialHubOffset, 0));
	BrakeMeshRelativeTransform.SetLocation(BrakeMeshRelativeTransform.GetLocation() + FVector(0, AxialHubOffset, 0));

	float WheelPos = FMath::Sign(GetRelativeLocation().Y);

	//if is left wheel
	if (WheelPos < 0)
	{
		FTransform TempTrans = FTransform(FQuat(FRotator(0.f)), FVector(0.f), FVector(1.f, -1.f, 1.f));
		WheelMeshRelatvieTransform *= TempTrans;
		BrakeMeshRelativeTransform *= TempTrans;
	}

	FQuat TempRot = FQuat(FRotator(0, SuspensionKinematicsConfig.BaseToe * WheelPos, SuspensionKinematicsConfig.BaseCamber * WheelPos));
	FVector KnucklePos = GetRelativeRotation().Quaternion().GetRightVector() * SuspensionKinematicsConfig.ArmLength * WheelPos;
	WheelKnuckleComponent->SetRelativeTransform(FTransform(GetRelativeTransform().InverseTransformRotation(TempRot), KnucklePos, FVector(1)));

	if (NewWheelMesh)
	{
		WheelMeshComponent->SetStaticMesh(NewWheelMesh);
	}
	WheelMeshComponent->SetRelativeTransform(WheelMeshRelatvieTransform);
	WheelMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (NewBrakeMesh)
	{
		BrakeMeshComponent->SetStaticMesh(NewBrakeMesh);
	}
	BrakeMeshComponent->SetRelativeTransform(BrakeMeshRelativeTransform);
	BrakeMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	return WheelMeshComponent->GetStaticMesh() && BrakeMeshComponent->GetStaticMesh();
}

bool UVehicleWheelComponent::RefreshWheelMesh()
{
	return SetMesh(
		SuspensionKinematicsConfig.AxialHubOffset, 
		WheelMesh,
		WheelMeshTransform, 
		BrakeMesh, 
		BrakeMeshTransform);
}

void UVehicleWheelComponent::UpdateWheelAnim(float DeltaTime, float MaxAnimAngularVelocity)
{
	if (!IsValid(WheelKnuckleComponent) || !IsValid(WheelMeshComponent))return;

	if (DeltaTime > 0)
	{
		TimeSinceLastPhysicsTick += DeltaTime;

		// blend between physics frames
		float Alpha = FMath::Clamp(TimeSinceLastPhysicsTick / Wheel.SimData.PhysicsDeltaTime, 0.0f, 1.0f);
		FVector2D TargetAnimKnucklePos2D = FMath::Lerp(PrevKnucklePos2D, Suspension.SimData.KnucklePos2D, Alpha);
		FQuat TargetAnimWheelRelativeRot = FMath::Lerp(PrevWheelRelativeRot, Suspension.SimData.WheelRelativeTransform.GetRotation(), Alpha);

		// interp
		AnimKnucklePos2D = FMath::Vector2DInterpTo(AnimKnucklePos2D, TargetAnimKnucklePos2D, DeltaTime, AnimInterpSpeed);
		AnimWheelRelativeRot = FMath::QInterpTo(AnimWheelRelativeRot, TargetAnimWheelRelativeRot, DeltaTime, AnimInterpSpeed);
	}
	else
	{
		TimeSinceLastPhysicsTick = 0.f;
		AnimKnucklePos2D = Suspension.SimData.KnucklePos2D;
		AnimWheelRelativeRot = Suspension.SimData.WheelRelativeTransform.GetRotation();
	}

	WheelKnuckleComponent->SetRelativeLocation(Suspension.SuspensionPlaneToZYPlane(AnimKnucklePos2D));
	WheelKnuckleComponent->SetRelativeRotation(Suspension.SimData.RelativeTransform.InverseTransformRotation(AnimWheelRelativeRot));
	
	if (IsValid(WheelMeshComponent))
	{
		if (MaxAnimAngularVelocity > 0)
		{
			WheelMeshComponent->AddLocalRotation(FRotator(FMath::RadiansToDegrees(
				FMath::Clamp(-Wheel.SimData.AngularVelocity, -MaxAnimAngularVelocity, MaxAnimAngularVelocity)) * DeltaTime, 0, 0));
		}
		else
		{
			WheelMeshComponent->AddLocalRotation(FRotator(FMath::RadiansToDegrees(-Wheel.SimData.AngularVelocity) * DeltaTime, 0, 0));
		}
	}
}

FTransform UVehicleWheelComponent::GetSkidMarkWorldTransform(float InSkidMarkBias, float InSkidMarkScale)
{
	FVector N = Suspension.SimData.HitStruct.ImpactNormal;
	FQuat q = FRotationMatrix::MakeFromZX(N, Wheel.SimData.WorldLinearVelocity).ToQuat();
	FVector v = Suspension.SimData.HitStruct.ImpactPoint + N;	//to avoid spawned in the ground
	FVector rightV = FVector::VectorPlaneProject(Suspension.SimData.WheelRightVector, N).GetSafeNormal();
	v += rightV * InSkidMarkBias * Suspension.SimData.WheelPos;
	v += q.GetForwardVector() * -0.25 * InSkidMarkScale;	//-0.25 is for no reason
	FVector LocalLinVel = FVector(Wheel.SimData.LocalLinearVelocity, 0.f);
	return FTransform(q, v, LocalLinVel);
}

FQuat UVehicleWheelComponent::UpdateSuspensionArmAnim(USceneComponent* InArmMesh, FRotator InRotationOffset)
{
	float PosSign = Suspension.SimData.WheelPos;
	InRotationOffset.Yaw *= PosSign;
	InRotationOffset.Roll *= PosSign;
	FQuat TempInitialRot = InRotationOffset.Quaternion();
	FQuat TempArmRot = Suspension.MakeQuatFrom2DVectors(
		FVector2D(0.f, -PosSign),
		FVector2D(Suspension.SimData.KnucklePos2D.X, Suspension.SimData.KnucklePos2D.Y * -PosSign),
		Suspension.SimData.ComponentRelativeForwardVector);
	TempArmRot *= TempInitialRot;
	TempArmRot.Normalize();

	if (IsValid(InArmMesh))
	{
		InArmMesh->SetRelativeRotation(TempArmRot);
	}

	return TempArmRot;
}

FTransform UVehicleWheelComponent::UpdateSuspensionSpringAnim(
	USceneComponent* InSpringMesh, 
	FVector InScaleAxis, 
	float InOffsetAlongArm, 
	FVector2D InKnuckleOffset,
	FRotator InRotationOffset, 
	float InLengthBias, 
	FVector InInitialScale)
{
	if (!IsValid(InSpringMesh))return FTransform();

	float PosSign = Suspension.SimData.WheelPos;
	InRotationOffset.Yaw *= PosSign;
	InRotationOffset.Roll *= PosSign;

	InKnuckleOffset.Y *= PosSign;

	FVector KnuckleOffset3D = Suspension.SimData.WheelRelativeTransform.TransformPosition(FVector(0.f, InKnuckleOffset.Y, InKnuckleOffset.X));
	KnuckleOffset3D -= Suspension.SimData.WheelRelativeTransform.GetLocation();

	FVector PivotPos = Suspension.SimData.RelativeTransform.TransformPosition(Suspension.SuspensionPlaneToZYPlane(FVector2D(0.f)));
	FVector ArmDir = (PivotPos - Suspension.SimData.KnuckleRelativePos).GetSafeNormal();
	
	FVector OffsetJointPos = Suspension.SimData.KnuckleRelativePos + ArmDir * InOffsetAlongArm;
	FVector OffsetInitialJointPos = Suspension.SimData.RelativeTransform.TransformPosition(FVector(0.f, InOffsetAlongArm * -PosSign, 0.f));

	OffsetJointPos += KnuckleOffset3D;
	OffsetInitialJointPos += KnuckleOffset3D;

	FVector AnimStrut = InSpringMesh->GetRelativeLocation() - OffsetJointPos;
	FVector AnimStrutDir = AnimStrut.GetSafeNormal();

	FQuat AnimStrutRot = FRotationMatrix::MakeFromXZ(Suspension.SimData.ComponentRelativeForwardVector, AnimStrutDir).ToQuat();

	float InitialLength = (InSpringMesh->GetRelativeLocation() - OffsetInitialJointPos).Length();
	float CurrentLength = AnimStrut.Length();

	float Scale = SafeDivide(CurrentLength, InitialLength);
	FVector Scale3D = FVector(1.f) + InScaleAxis * (Scale - 1.f);
	Scale3D *= InInitialScale;

	InSpringMesh->SetRelativeRotation(AnimStrutRot);
	InSpringMesh->SetRelativeScale3D(Scale3D);

	return FTransform(AnimStrutRot, InSpringMesh->GetRelativeLocation(), Scale3D);
}

void UVehicleWheelComponent::AttachComponentToKnuckle(USceneComponent* InComponent, FTransform InTransform)
{
	if (!IsValid(InComponent))return;
	InComponent->AttachToComponent(WheelKnuckleComponent, FAttachmentTransformRules::KeepRelativeTransform);
	InComponent->SetRelativeTransform(InTransform);
}

float UVehicleWheelComponent::SafeDivide(auto a, auto b)
{
	return (FMath::IsNearlyZero(b)) ? 0.0f : a / b;
}
