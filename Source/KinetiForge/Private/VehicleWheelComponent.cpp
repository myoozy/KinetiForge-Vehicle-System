// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)


#include "VehicleWheelComponent.h"
#include "VehicleWheelCoordinatorComponent.h"
#include "VehicleUtil.h"
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
		static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultFx.DefaultFx'")
		);
		if (CurveObj.Succeeded())
		{
			TireConfig.Fx = CurveObj.Object;
		}
	}
	
	if (!TireConfig.Fy)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultFy.DefaultFy'")
		);
		if (CurveObj.Succeeded())
		{
			TireConfig.Fy = CurveObj.Object;
		}
	}

	if (!SuspensionKinematicsConfig.CamberCurve)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultCamberCurve.DefaultCamberCurve'")
		);
		if (CurveObj.Succeeded())
		{
			SuspensionKinematicsConfig.CamberCurve = CurveObj.Object;
		}
	}

	if (!SuspensionKinematicsConfig.AntiDiveCurve)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveObj(
				TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultAntiDiveCurve.DefaultAntiDiveCurve'")
		);
		if (CurveObj.Succeeded())
		{
			SuspensionKinematicsConfig.AntiDiveCurve = CurveObj.Object;
		}
	}

	if (!SuspensionKinematicsConfig.AntiSquatCurve)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultAntiSquatCurve.DefaultAntiSquatCurve'")
		);
		if (CurveObj.Succeeded())
		{
			SuspensionKinematicsConfig.AntiSquatCurve = CurveObj.Object;
		}
	}

	if (!SuspensionKinematicsConfig.AntiRollCurve)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultAntiRollCurve.DefaultAntiRollCurve'")
		);
		if (CurveObj.Succeeded())
		{
			SuspensionKinematicsConfig.AntiRollCurve = CurveObj.Object;
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

	TimeSinceLastConfigSync = FMath::FRandRange(0.f, ConfigSyncInterval);
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

	if (!Suspension.State.bHitGround)return;

	FVector WidthBias = FVector(0.f);
	FVector PosToApplyImpulse = FVector(0.f);

	switch (SuspensionKinematicsConfig.PositionToApplyForce)
	{
	case EPositionToApplyForce::ImpactPoint:
		PosToApplyImpulse = Suspension.State.ImpactPoint;
		break;
	case EPositionToApplyForce::WheelCenter:
		PosToApplyImpulse = Suspension.State.WheelWorldPos;
		break;
	default:
		PosToApplyImpulse = Suspension.State.ImpactPoint;
		break;
	}

	/*---------------------------ANTI-PITCH & ANTI-ROLL GEOMETRY------------------------------*/

	// get rigid handle to get world com position
	Chaos::FRigidBodyHandle_Internal* CarbodyHandle = nullptr;
	if (IsValid(Carbody))
	{
		if (const FBodyInstance* BodyInstance = Carbody->GetBodyInstance())
		{
			if (const auto Handle = BodyInstance->ActorHandle)
			{
				CarbodyHandle = Handle->GetPhysicsThreadAPI();
			}
		}
	}

	// get world com
	Chaos::FVec3 CarbodyWorldCOM = CarbodyHandle != nullptr ? 
		Chaos::FParticleUtilitiesGT::GetCoMWorldPosition(CarbodyHandle) : CarbodyAsyncWorldTransform.GetLocation();

	// get arm
	Chaos::FVec3 LeverArmVec = PosToApplyImpulse - CarbodyWorldCOM;
	float LeverArmLengthSq = LeverArmVec.SquaredLength();

	// get torque caused by tire force
	Chaos::FVec3 InducedTorqueWorld = Chaos::FVec3::CrossProduct(LeverArmVec, Wheel.State.TireForce);

	// transform torque into carbody space
	Chaos::FVec3 InducedTorqueLocal = CarbodyAsyncWorldTransform.GetRotation().UnrotateVector(InducedTorqueWorld);

	// get anti-pitch value
	bool bIsDiving = InducedTorqueLocal.Y < 0.f;
	float AntiPitchScale = bIsDiving ? Suspension.State.AntiDiveRatio : Suspension.State.AntiSquatRatio;

	// get anti-roll value
	float AntiRollScale = Suspension.State.AntiRollRatio;

	// get counter torque
	Chaos::FVec3 TargetCounterTorqueLocal = 
		-Chaos::FVec3(InducedTorqueLocal.X * AntiRollScale, InducedTorqueLocal.Y * AntiPitchScale, InducedTorqueLocal.Z);

	// get counter torque in world space
	Chaos::FVec3 TargetCounterTorqueWorld = CarbodyAsyncWorldTransform.GetRotation().RotateVector(TargetCounterTorqueLocal);

	// get normal of ground
	FVector ImpactNormal = (FVector)Suspension.State.ImpactNormal;

	// Torque Axis for Normal Force (1N)
	Chaos::FVec3 NormalTorqueAxis = Chaos::FVec3::CrossProduct(LeverArmVec, ImpactNormal);
	float EffectiveLeverArmSq = NormalTorqueAxis.SizeSquared();

	// get final jacking force
	Chaos::FVec3 FinalJackingForce = Chaos::FVec3(0.f);
	if (EffectiveLeverArmSq > SMALL_NUMBER)
	{
		float ForceMagnitude = Chaos::FVec3::DotProduct(TargetCounterTorqueWorld, NormalTorqueAxis) / EffectiveLeverArmSq;

		FinalJackingForce = ImpactNormal * ForceMagnitude;
	}

	/*-------------------------------------APPLY FORCE---------------------------------------*/

	// project the suspension force onto the impact normal
	// so that the car will not move by itself when parked
	FVector SuspensionForceProj = ImpactNormal * Suspension.State.ForceAlongImpactNormal;
	FVector Impulse = ((FVector)Wheel.State.TireForce + SuspensionForceProj + FinalJackingForce) * Wheel.State.PhysicsDeltaTime;
	Impulse *= 100.;	// because of the unit of unreal engine

	// apply force to carbody
	FVector AngularImpulse = FVector::CrossProduct(LeverArmVec, Impulse);
	CarbodyHandle->SetLinearImpulse(CarbodyHandle->LinearImpulse() + Impulse, false);
	CarbodyHandle->SetAngularImpulse(CarbodyHandle->AngularImpulse() + AngularImpulse, false);

	// also add force to the contacted component
	if (UPrimitiveComponent* HitComponent = Suspension.RayCastResult.Component.Get())
	{
		if (HitComponent->IsPhysicsStateCreated() &&
			HitComponent->IsSimulatingPhysics())
		{
			UAsyncTickFunctions::ATP_AddImpulseAtPosition(
				HitComponent,
				Suspension.State.ImpactPoint,
				-Impulse,
				Suspension.RayCastResult.BoneName
			);
		}
	}
}

// Called every frame
void UVehicleWheelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	TimeSinceLastConfigSync += DeltaTime;
	if (TimeSinceLastConfigSync > ConfigSyncInterval)
	{
		TimeSinceLastConfigSync -= ConfigSyncInterval;

		// update cached curves
		Suspension.UpdateCachedRichCurves(SuspensionKinematicsConfig);
		Wheel.UpdateCachedRichCurves(TireConfig);

		// check if sprungmass should be updated
		FVector NewRelativeLocation = GetRelativeLocation();
		if ((CachedComponentRelativeLocation - NewRelativeLocation).SquaredLength() > 1.f)
		{
			if (IsValid(WheelCoordinator))WheelCoordinator->NotifyWheelMoved();
		}
	}
	
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

	//initialize meshes
	GenerateMeshComponents();

	Suspension.Initialize(this);
	Wheel.Initialize(this);

	// initialize animation
	UpdateWheelAnim();
}

void UVehicleWheelComponent::SetSprungMass(float NewSprungMass)
{
	Suspension.SetSprungMass(SuspensionSpringConfig, NewSprungMass);
}

FTransform3f UVehicleWheelComponent::GetWheelRelativeTransform()
{

	return FTransform3f(
		Suspension.State.WheelRelativeRotation,
		Suspension.State.KnuckleRelativePos + Suspension.State.WheelCenterToKnuckle);
}

float UVehicleWheelComponent::GetNormalizedSlip(float LongitudinalScale, float LateralScale)
{
	const FRichCurve& FxCurve = Wheel.CachedCurves.Fx;
	const FRichCurve& FyCurve = Wheel.CachedCurves.Fy;

	// get tire stiffness
	float Cx = FVehicleWheelSolver::GetTangentAtOrigin(FxCurve);
	float Cy = FVehicleWheelSolver::GetTangentAtOrigin(FyCurve) * 90.f;

	Cx = Cx > SMALL_NUMBER ? Cx : 1.f;
	Cy = Cy > SMALL_NUMBER ? Cy : 1.f;

	// get weight
	const float Bias = TireConfig.CombinedSlipBias;
	float Wx = (1.f - Bias) * Cx;
	float Wy = (Bias) * Cy;

	float NormFactor = 1.f / FMath::Max(FMath::Max(Wx, Wy), SMALL_NUMBER);

	// if the slip velocity is too low, should scale the affection of slip ratio, 0.5 is just magic number
	float SpeedScaleX = FMath::Clamp(0.5f * Wheel.State.LongSlipVelocity, -1.f, 1.f);
	float NormalizedSx = Wheel.SmoothenedSlipRatio * LongitudinalScale * SpeedScaleX * Wx * NormFactor;

	// also if the lateral velocity is too low, should scale the affection of slip angle
	float SpeedScaleY = FMath::Clamp(0.5f * Wheel.State.LocalLinearVelocity.Y, -1.f, 1.f);
	float NormalizedSy = Wheel.SmoothenedSlipAngle / 90.f * LateralScale * SpeedScaleY * Wy * NormFactor;

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
	PrevKnucklePos2D = Suspension.State.KnucklePos2D;
	PrevWheelRelativeRot = Suspension.State.WheelRelativeRotation;
	TimeSinceLastPhysicsTick = 0.f;

	CarbodyAsyncWorldTransform = UAsyncTickFunctions::ATP_GetTransform(Carbody);
	if (!CarbodyAsyncWorldTransform.IsRotationNormalized()) CarbodyAsyncWorldTransform.NormalizeRotation();

	//Suspension
	Suspension.UpdateSuspension(
		WheelConfig.Radius,
		WheelConfig.Width,
		SuspensionKinematicsConfig,
		SuspensionSpringConfig,
		GetRelativeTransform(),
		CarbodyAsyncWorldTransform,
		GetWorld(),
		Carbody,
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
		WheelConfig,
		TireConfig,
		ABSConfig,
		Suspension.State);

	ApplyWheelForce();
}

void UVehicleWheelComponent::StartUpdateSolidAxlePhysics(
	float InSteeringAngle,
	FVector& OutApporximatedWheelWorldPos,
	FVehicleSuspensionSimContext& Ctx
)
{
	CarbodyAsyncWorldTransform = UAsyncTickFunctions::ATP_GetTransform(Carbody);

	return Suspension.StartUpdateSolidAxle(
		WheelConfig.Radius,
		WheelConfig.Width,
		SuspensionKinematicsConfig,
		GetRelativeTransform(),
		CarbodyAsyncWorldTransform,
		GetWorld(),
		InSteeringAngle, 
		OutApporximatedWheelWorldPos, 
		Ctx);
}

void UVehicleWheelComponent::FinalizeUpdateSolidAxlePhysics(
	float InPhysicsDeltaTime,
	float InDriveTorque,
	float InBrakeTorque,
	float InHandbrakeTorque,
	float InSwaybarForce,
	float InReflectedInertia,
	FVehicleSuspensionSimContext& Ctx,
	const FVector& InKnuckleWorldPos,
	const FVector& InAxleWorldDirection
)
{
	Suspension.FinalizeUpdateSolidAxle(
		WheelConfig.Radius,
		SuspensionKinematicsConfig,
		SuspensionSpringConfig,
		Carbody,
		InPhysicsDeltaTime, 
		InSwaybarForce,
		Ctx,
		InKnuckleWorldPos,
		InAxleWorldDirection);

	Wheel.UpdateWheel(
		InPhysicsDeltaTime,
		InDriveTorque,
		InBrakeTorque,
		InHandbrakeTorque,
		InReflectedInertia,
		WheelConfig,
		TireConfig,
		ABSConfig,
		Suspension.State);

	ApplyWheelForce();
}

void UVehicleWheelComponent::ApplySuspensionStateDirect(float InExtensionRatio, float InSteeringAngle)
{
	const int32 Iteration = 2;
	for (int32 i = 0; i < Iteration; i++)
	{
		Suspension.ApplySuspensionStateDirect(
			WheelConfig.Radius,
			SuspensionKinematicsConfig,
			GetRelativeTransform(),
			CarbodyAsyncWorldTransform,
			InExtensionRatio, 
			InSteeringAngle);
		UpdateWheelAnim();
	}
}

void UVehicleWheelComponent::StartApplySolidAxleStateDirect(
	float InExtensionRatio, 
	float InSteeringAngle, 
	FVector& OutApporximatedWheelWorldPos, 
	FVehicleSuspensionSimContext& Ctx)
{
	Suspension.StartApplySolidAxleStateDirect(
		WheelConfig.Radius,
		SuspensionKinematicsConfig,
		GetRelativeTransform(),
		CarbodyAsyncWorldTransform,
		InExtensionRatio, 
		InSteeringAngle, 
		OutApporximatedWheelWorldPos, 
		Ctx
	);
}

void UVehicleWheelComponent::FinalizeApplySolidAxleStateDirect(
	FVehicleSuspensionSimContext& Ctx, 
	const FVector& InKnuckleWorldPos, 
	const FVector& InAxleWorldDirection)
{
	Suspension.FinalizeApplySolidAxleStateDirect(
		WheelConfig.Radius,
		SuspensionKinematicsConfig,
		Ctx,
		InKnuckleWorldPos,
		InAxleWorldDirection
	);

	UpdateWheelAnim();
}

void UVehicleWheelComponent::GetWheelCoordinator(UVehicleWheelCoordinatorComponent*& OutWheelCoordinator)
{
	OutWheelCoordinator = WheelCoordinator;
}

float UVehicleWheelComponent::ComputeFeedBackTorque()
{
	// get machanism trail
	FVector3f Arm = WheelConfig.Radius * Suspension.State.StrutDirection.ProjectOnToNormal(Suspension.State.ImpactNormal);
	FVector3f Torque = FVector3f::CrossProduct(Arm, Wheel.State.TireForce);
	return FVector3f::DotProduct(Suspension.State.StrutDirection, Torque);
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

FTransform UVehicleWheelComponent::GetCarbodyWorldTransform()
{
	return IsValid(Carbody) ? Carbody->GetComponentTransform() : FTransform();
}

void UVehicleWheelComponent::DrawSuspension(float Duration, float Thickness, bool bDrawSuspension, bool bDrawWheel, bool bDrawRayCast)
{
	Suspension.DrawSuspension(this, Duration, Thickness, bDrawSuspension, bDrawWheel, bDrawRayCast);
}

void UVehicleWheelComponent::DrawSuspensionForce(float Duration, float Thickness, float Length)
{
	Suspension.DrawSuspensionForce(this, Duration, Thickness, Length);
}

void UVehicleWheelComponent::DrawWheelForce(float Duration, float Thickness, float Length, bool bDrawVelocity, bool bDrawSlip, bool bDrawInertia)
{
	Wheel.DrawWheelForce(this, Suspension.State, Duration, Thickness, Length, bDrawVelocity, bDrawSlip, bDrawInertia);
}

bool UVehicleWheelComponent::SetMesh(
	float AxialHubOffset,
	UStaticMesh* NewWheelMesh, 
	FTransform WheelMeshRelatvieTransform,
	UStaticMesh* NewBrakeMesh, 
	FTransform BrakeMeshRelativeTransform)
{
	if (!IsValid(WheelKnuckleComponent) || !IsValid(WheelMeshComponent) || !IsValid(BrakeMeshComponent))return false;

	WheelKnuckleComponent->PrimaryComponentTick.bCanEverTick = false;
	WheelKnuckleComponent->PrimaryComponentTick.bStartWithTickEnabled = false;

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
	WheelMeshComponent->SetGenerateOverlapEvents(false);
	WheelMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WheelMeshComponent->SetCanEverAffectNavigation(false);
	WheelMeshComponent->PrimaryComponentTick.bCanEverTick = false;
	WheelMeshComponent->PrimaryComponentTick.bStartWithTickEnabled = false;
	if (NewBrakeMesh)
	{
		BrakeMeshComponent->SetStaticMesh(NewBrakeMesh);
	}
	BrakeMeshComponent->SetRelativeTransform(BrakeMeshRelativeTransform);
	BrakeMeshComponent->SetGenerateOverlapEvents(false);
	BrakeMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BrakeMeshComponent->SetCanEverAffectNavigation(false);
	BrakeMeshComponent->PrimaryComponentTick.bCanEverTick = false;
	BrakeMeshComponent->PrimaryComponentTick.bStartWithTickEnabled = false;

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

	TRACE_CPUPROFILER_EVENT_SCOPE(UpdateVehicleWheelAnimation);

	if (DeltaTime > 0)
	{
		TimeSinceLastPhysicsTick += DeltaTime;

		// blend between physics frames
		float Alpha = FMath::Clamp(TimeSinceLastPhysicsTick / Wheel.State.PhysicsDeltaTime, 0.0f, 1.0f);
		FVector2f TargetAnimKnucklePos2D = FMath::Lerp(PrevKnucklePos2D, Suspension.State.KnucklePos2D, Alpha);
		FQuat4f TargetAnimWheelRelativeRot = FMath::Lerp(PrevWheelRelativeRot, Suspension.State.WheelRelativeRotation, Alpha);

		// interp
		AnimKnucklePos2D = (FVector2f)FMath::Vector2DInterpTo((FVector2D)AnimKnucklePos2D, (FVector2D)TargetAnimKnucklePos2D, DeltaTime, AnimInterpSpeed);
		AnimWheelRelativeRot = (FQuat4f)FMath::QInterpTo((FQuat)AnimWheelRelativeRot, (FQuat)TargetAnimWheelRelativeRot, DeltaTime, AnimInterpSpeed);
	}
	else
	{
		TimeSinceLastPhysicsTick = 0.f;
		AnimKnucklePos2D = Suspension.State.KnucklePos2D;
		AnimWheelRelativeRot = Suspension.State.WheelRelativeRotation;
	}

	float WheelPos = Suspension.State.bIsRightWheel ? 1.f : -1.f;
	FTransform T = FTransform(
		GetRelativeTransform().InverseTransformRotation((FQuat)AnimWheelRelativeRot),
		(FVector)Suspension.SuspensionPlaneToZYPlane(AnimKnucklePos2D, WheelPos),
		FVector(1.f)
	);
	WheelKnuckleComponent->SetRelativeTransform(
		T,
		false,
		nullptr,
		ETeleportType::TeleportPhysics
	);
	
	if (IsValid(WheelMeshComponent))
	{
		if (MaxAnimAngularVelocity > 0)
		{
			WheelMeshComponent->AddLocalRotation(FRotator(FMath::RadiansToDegrees(
				FMath::Clamp(-Wheel.State.AngularVelocity, -MaxAnimAngularVelocity, MaxAnimAngularVelocity)) * DeltaTime, 0, 0));
		}
		else
		{
			WheelMeshComponent->AddLocalRotation(FRotator(FMath::RadiansToDegrees(-Wheel.State.AngularVelocity) * DeltaTime, 0, 0));
		}
	}
}

FTransform UVehicleWheelComponent::GetSkidMarkWorldTransform(float InSkidMarkBias, float InSkidMarkScale)
{
	if (SuspensionKinematicsConfig.RayCastMode == ESuspensionRayCastMode::LineTrace)
	{
		// the impact point of line trace is always on the outer side of the wheel
		InSkidMarkBias -= WheelConfig.Width * 0.5f;
	}
	FVector N = (FVector)Suspension.State.ImpactNormal;
	FQuat q = FRotationMatrix::MakeFromZX(N, FVector(Suspension.State.ImpactPointWorldVelocity)).ToQuat();
	FVector v = Suspension.State.ImpactPoint + N;	//to avoid spawned in the ground
	FVector rightV = FVector::VectorPlaneProject(FVector(Suspension.State.WheelRightVector), N).GetSafeNormal();
	v += rightV * InSkidMarkBias * (Suspension.State.bIsRightWheel ? 1.f : -1.f);
	v += q.GetForwardVector() * -0.25 * InSkidMarkScale;	//-0.25 is for no reason
	FVector LocalLinVel = FVector((FVector2D)Wheel.State.LocalLinearVelocity, 0.f);
	return FTransform(q, v, LocalLinVel);
}

FQuat4f UVehicleWheelComponent::UpdateSuspensionArmAnim(USceneComponent* InArmMesh, FRotator InRotationOffset)
{
	float PosSign = Suspension.State.bIsRightWheel ? 1.f : -1.f;
	InRotationOffset.Yaw *= PosSign;
	InRotationOffset.Roll *= PosSign;
	FQuat4f TempInitialRot = (FQuat4f)InRotationOffset.Quaternion();
	FQuat4f TempArmRot = Suspension.MakeQuatFrom2DVectors(
		FVector2f(0.f, -PosSign),
		FVector2f(AnimKnucklePos2D.X, AnimKnucklePos2D.Y * -PosSign),
		(FVector3f)GetRelativeTransform().GetRotation().GetForwardVector());
	TempArmRot *= TempInitialRot;
	TempArmRot.Normalize();

	if (IsValid(InArmMesh))
	{
		InArmMesh->SetRelativeRotation((FQuat)TempArmRot);
	}

	return TempArmRot;
}

FTransform3f UVehicleWheelComponent::UpdateSuspensionSpringAnim(
	USceneComponent* InSpringMesh, 
	FVector InScaleAxis, 
	float InOffsetAlongArm, 
	FVector InKnuckleOffset,
	FRotator InRotationOffset, 
	float InLengthBias, 
	FVector InInitialScale)
{
	if (!IsValid(InSpringMesh))return FTransform3f();

	float PosSign = Suspension.State.bIsRightWheel ? 1.f : -1.f;
	InRotationOffset.Yaw *= PosSign;
	InRotationOffset.Roll *= PosSign;

	InKnuckleOffset.Y *= PosSign;

	FTransform3f RelativeTrans = (FTransform3f)GetRelativeTransform();
	FVector3f KnuckleRelativePos = FVector3f(Suspension.State.KnuckleRelativePos);
	FTransform3f WheelRelativeTransform = GetWheelRelativeTransform();
	FVector3f KnuckleOffset3D = WheelRelativeTransform.TransformPosition(FVector3f(0.f, InKnuckleOffset.Y, InKnuckleOffset.X));
	KnuckleOffset3D -= WheelRelativeTransform.GetLocation();

	FVector3f PivotPos = RelativeTrans.TransformPosition(Suspension.SuspensionPlaneToZYPlane(FVector2f(0.f), PosSign));
	FVector3f ArmDir = (PivotPos - KnuckleRelativePos).GetSafeNormal();
	
	FVector3f OffsetJointPos = KnuckleRelativePos + ArmDir * InOffsetAlongArm;
	FVector3f OffsetInitialJointPos = RelativeTrans.TransformPosition(FVector3f(0.f, InOffsetAlongArm * -PosSign, 0.f));

	OffsetJointPos += KnuckleOffset3D;
	OffsetInitialJointPos += KnuckleOffset3D;

	FVector3f SpringMeshRelativePos = (FVector3f)InSpringMesh->GetRelativeLocation();
	FVector3f AnimStrut = SpringMeshRelativePos - OffsetJointPos;
	FVector3f AnimStrutDir = AnimStrut.GetSafeNormal();

	FQuat4f AnimStrutRot = FRotationMatrix44f::MakeFromXZ(RelativeTrans.GetRotation().GetForwardVector(), AnimStrutDir).ToQuat();
	
	float InitialLength = (SpringMeshRelativePos - OffsetInitialJointPos).Length();
	float CurrentLength = AnimStrut.Length();

	float Scale = VehicleUtil::SafeDivide(CurrentLength, InitialLength);
	FVector3f Scale3D = FVector3f(1.f) + (FVector3f)InScaleAxis * (Scale - 1.f);
	Scale3D *= (FVector3f)InInitialScale;

	InSpringMesh->SetRelativeRotation((FQuat)AnimStrutRot);
	InSpringMesh->SetRelativeScale3D((FVector)Scale3D);

	return FTransform3f(AnimStrutRot, SpringMeshRelativePos, Scale3D);
}

void UVehicleWheelComponent::AttachComponentToKnuckle(USceneComponent* InComponent, FTransform InTransform)
{
	if (!IsValid(InComponent))return;
	InComponent->AttachToComponent(WheelKnuckleComponent, FAttachmentTransformRules::KeepRelativeTransform);
	InComponent->SetRelativeTransform(InTransform);
}
