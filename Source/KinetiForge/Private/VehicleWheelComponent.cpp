// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)


#include "VehicleWheelComponent.h"
#include "VehicleWheelCoordinatorComponent.h"
#include "VehicleUtilities.h"
#include "AsyncTickFunctions.h"

// Sets default values for this component's properties
UVehicleWheelComponent::UVehicleWheelComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PrePhysics;

	// ...
	// add meshes
	FString ThisName = FString();
	GetName(ThisName);
	FName NameWheelHub = FName(ThisName + "_WheelHub");
	FName NameWheelMesh = FName(ThisName + "_WheelMesh");
	FName NameBrakeMesh = FName(ThisName + "_BrakeMesh");
	WheelHubComponent = Cast<USceneComponent>(CreateDefaultSubobject<USceneComponent>(NameWheelHub));
	WheelMeshComponent = Cast<UStaticMeshComponent>(CreateDefaultSubobject<UStaticMeshComponent>(NameWheelMesh));
	BrakeMeshComponent = Cast<UStaticMeshComponent>(CreateDefaultSubobject<UStaticMeshComponent>(NameBrakeMesh));

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

#if 0
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
#endif
}

// Called when the game starts
void UVehicleWheelComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	WheelCoordinator = UVehicleWheelCoordinatorComponent::FindWheelCoordinator(Chassis.Get());
	if (WheelCoordinator.IsValid())
	{
		WheelCoordinator->RegisterWheel(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WheelPhysics: WheelCoordinatorNotFound! Sprung mass can not be computed."));
	}

#if WITH_EDITOR
	TimeSinceLastConfigSync = FMath::FRandRange(0.f, ConfigSyncInterval);
	TimeSinceLastConfigSync += ConfigSyncInterval;
#endif
}

void UVehicleWheelComponent::OnRegister()
{
	Super::OnRegister();

	//...
	InitializeWheel();
}

void UVehicleWheelComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	if (IsValid(WheelHubComponent) && !WheelHubComponent->IsBeingDestroyed())
	{
		WheelHubComponent->DestroyComponent();
	}
	WheelHubComponent = nullptr;

	if (IsValid(WheelMeshComponent) && !WheelMeshComponent->IsBeingDestroyed())
	{
		WheelMeshComponent->DestroyComponent();
	}
	WheelMeshComponent = nullptr;

	if (IsValid(BrakeMeshComponent) && !BrakeMeshComponent->IsBeingDestroyed())
	{
		BrakeMeshComponent->DestroyComponent();
	}
	BrakeMeshComponent = nullptr;

	if (WheelCoordinator.IsValid() && !WheelCoordinator->IsBeingDestroyed())
	{ 
		WheelCoordinator->NotifyWheelMoved(); 
	}

	Chassis = nullptr;
	TireConfig.Fx = nullptr;
	TireConfig.Fy = nullptr;
	SuspensionKinematicsConfig.CamberCurve = nullptr;
	SuspensionKinematicsConfig.ToeCurve = nullptr;
	SuspensionKinematicsConfig.CasterCurve = nullptr;
	SuspensionKinematicsConfig.AntiDiveCurve = nullptr;
	SuspensionKinematicsConfig.AntiSquatCurve = nullptr;
	SuspensionKinematicsConfig.AntiRollCurve = nullptr;

	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

bool UVehicleWheelComponent::InitializeMeshComponents()
{
	if (!IsValid(WheelHubComponent) || !IsValid(WheelMeshComponent) || !IsValid(BrakeMeshComponent))return false;

	if (WheelHubComponent->GetAttachParent() != this)
		WheelHubComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	if (WheelMeshComponent->GetAttachParent() != WheelHubComponent)
	WheelMeshComponent->AttachToComponent(WheelHubComponent, FAttachmentTransformRules::KeepRelativeTransform);

	if (BrakeMeshComponent->GetAttachParent() != WheelHubComponent)
	BrakeMeshComponent->AttachToComponent(WheelHubComponent, FAttachmentTransformRules::KeepRelativeTransform);

	return RefreshWheelMesh();
}

void UVehicleWheelComponent::ApplyWheelForce(Chaos::FRigidBodyHandle_Internal* ChassisHandle)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForge_Wheel_ApplyForce);

	if (!Suspension.State.bHitGround)return;

	FVector WidthBias = FVector(0.f);
	FVector PosToApplyImpulse = FVector(0.f);

	switch (SuspensionKinematicsConfig.LocationToApplyForce)
	{
	case EPositionToApplyForce::ImpactPoint:
		PosToApplyImpulse = Suspension.State.ImpactWorldLocation;
		break;
	case EPositionToApplyForce::WheelCenter:
		PosToApplyImpulse = ChassisAsyncWorldTransform.TransformPositionNoScale(
			(FVector)Suspension.State.HubChassisLocation);
		break;
	default:
		PosToApplyImpulse = Suspension.State.ImpactWorldLocation;
		break;
	}

	// get world com
	Chaos::FVec3 ChassisWorldCOM = ChassisHandle != nullptr ? 
		Chaos::FParticleUtilitiesGT::GetCoMWorldPosition(ChassisHandle) : ChassisAsyncWorldTransform.GetLocation();

	// get arm
	Chaos::FVec3 LeverArmVec = PosToApplyImpulse - ChassisWorldCOM;

	// get normal of ground
	FVector ImpactNormal = (FVector)Suspension.State.ImpactWorldNormal;

	/*-------------------------------------APPLY FORCE---------------------------------------*/

	// project the suspension force onto the impact normal
	// so that the car will not move by itself when parked
	FVector SuspensionForceProj = ImpactNormal * Suspension.State.ForceAlongImpactNormal;
	FVector Impulse = ((FVector)Wheel.State.TireForce + SuspensionForceProj) * Wheel.State.PhysicsDeltaTime;
	Impulse *= 100.;	// because of the unit of unreal engine

	// apply force to Chassis
	if (ChassisHandle)
	{
		FVector AngularImpulse = FVector::CrossProduct(LeverArmVec, Impulse);
		ChassisHandle->SetLinearImpulse(ChassisHandle->LinearImpulse() + Impulse, false);
		ChassisHandle->SetAngularImpulse(ChassisHandle->AngularImpulse() + AngularImpulse, false);
	}

	// also add force to the contacted component
	if (UPrimitiveComponent* HitComponent = Suspension.RayCastResult.Component.Get())
	{
		const FName& BoneName = Suspension.RayCastResult.BoneName;
		if (HitComponent->IsPhysicsStateCreated() &&
			HitComponent->IsSimulatingPhysics(BoneName))
		{
			UAsyncTickFunctions::ATP_AddImpulseAtPosition(
				HitComponent,
				Suspension.State.ImpactWorldLocation,
				-Impulse,
				BoneName
			);
		}
	}
}

// Called every frame
void UVehicleWheelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

#if WITH_EDITOR
	TimeSinceLastConfigSync += DeltaTime;
	if (TimeSinceLastConfigSync > ConfigSyncInterval)
	{
		TimeSinceLastConfigSync -= FMath::Abs(ConfigSyncInterval);

		// update cached curves
		Suspension.UpdateCachedLUTs(SuspensionKinematicsConfig);
		Wheel.UpdateCachedLUTs(TireConfig);
	}
#endif
	
	// update animation
	if (bUpdateAnimAutomatically)
	{
		UpdateWheelAnim(DeltaTime, AnimWheelMaxRotationSpeed);
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
	Target->AnimWheelMaxRotationSpeed = Source->AnimWheelMaxRotationSpeed;
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
	Chassis = UVehicleUtilities::FindPhysicalParent(this);
	if (Chassis.IsValid() && Chassis != GetAttachParent())
	{
		AttachToComponent(Chassis.Get(), FAttachmentTransformRules::KeepWorldTransform);
	}
	if (!Chassis.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("WheelPhysics: Chassis Not Found!"));
	}

	//initialize meshes
	InitializeMeshComponents();

	Suspension.Initialize(this);
	Wheel.Initialize(TireConfig);

	CacheDesignedHubTransform();

	// initialize animation
	UpdateWheelAnim();
}

void UVehicleWheelComponent::CacheDesignedHubTransform(const float DesignedExtensionRatio)
{
	FVehicleSuspensionCachedLUTs TempLUTs;

	Suspension.CacheLUTs(TempLUTs, SuspensionKinematicsConfig);

	FVehicleSuspensionSimState DesignedState = Suspension.SolveKinematicsAtExtension(
		WheelConfig.Radius,
		SuspensionKinematicsConfig,
		GetRelativeTransform(),
		DesignedExtensionRatio,
		0.f,
		2,
		&TempLUTs
	);

	FTransform3f CompRelativeTrans3f = FTransform3f(GetRelativeTransform());

	DesignedHubLocalTransform = FTransform3f(
		CompRelativeTrans3f.InverseTransformRotation(DesignedState.HubChassisRotation),
		CompRelativeTrans3f.InverseTransformPositionNoScale(DesignedState.HubChassisLocation),
		FVector3f(1.f)
	);

	if (UVehicleWheelCoordinatorComponent* WheelCoord = WheelCoordinator.Get())
	{
		WheelCoordinator->NotifyWheelMoved();
	}
}

void UVehicleWheelComponent::SetWheelConfig(const FVehicleWheelConfig& NewConfig)
{
	WheelConfig = NewConfig;
}

void UVehicleWheelComponent::SetTireConfig(const FVehicleTireConfig& NewConfig)
{
	TireConfig = NewConfig;
	Wheel.UpdateCachedLUTs(NewConfig);
}

void UVehicleWheelComponent::SetABSConfig(const FVehicleABSConfig& NewConfig)
{
	ABSConfig = NewConfig;
}

void UVehicleWheelComponent::SetSuspensionKinematicsConfig(FVehicleSuspensionKinematicsConfig& NewConfig)
{
	SuspensionKinematicsConfig = NewConfig;
	Suspension.UpdateCachedLUTs(NewConfig);
	CacheDesignedHubTransform();
}

void UVehicleWheelComponent::SetSuspensionSpringConfig(FVehicleSuspensionSpringConfig& NewConfig)
{
	SuspensionSpringConfig = NewConfig;
}

void UVehicleWheelComponent::SetWheelMesh(UStaticMesh* NewMesh, const FTransform NewTransform, const bool bUseNewTransform)
{
	if (IsValid(NewMesh))
	{
		WheelMesh = NewMesh;
		WheelMeshComponent->SetStaticMesh(NewMesh);
	}
	if (bUseNewTransform)
	{
		WheelMeshTransform = NewTransform;
		WheelMeshComponent->SetRelativeTransform(NewTransform);
	}
}

void UVehicleWheelComponent::SetBrakeMesh(UStaticMesh* NewMesh, const FTransform NewTransform, const bool bUseNewTransform)
{
	if (IsValid(NewMesh))
	{
		BrakeMesh = NewMesh;
		BrakeMeshComponent->SetStaticMesh(NewMesh);
	}
	if (bUseNewTransform)
	{
		BrakeMeshTransform = NewTransform;
		BrakeMeshComponent->SetRelativeTransform(NewTransform);
	}
}

void UVehicleWheelComponent::SetSprungMass(float NewSprungMass)
{
	Suspension.SetSprungMass(SuspensionSpringConfig, NewSprungMass);
}

FVector3f UVehicleWheelComponent::GetTopMountRelativeLocation()
{
	FVector3f TopMountLocalPos = SuspensionKinematicsConfig.TopMountLocalLocation;
	TopMountLocalPos.Y *= FMath::Sign(GetRelativeLocation().Y);
	return (FVector3f)GetRelativeTransform().TransformPositionNoScale((FVector)TopMountLocalPos);
}

FTransform3f UVehicleWheelComponent::GetWheelRelativeTransform()
{

	return FTransform3f(
		Suspension.State.HubChassisRotation,
		Suspension.State.HubChassisLocation
	);
}

float UVehicleWheelComponent::GetNormalizedSlip(float LongitudinalScale, float LateralScale)
{
	// get tire stiffness
	float Cx = Wheel.CachedLUTs.Fx.FastEval(0.f).RightTangent;
	float Cy = Wheel.CachedLUTs.Fy.FastEval(0.f).RightTangent;

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
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForge_Wheel_UpdatePhysics);

	// get rigid handle to get world com position
	Chaos::FRigidBodyHandle_Internal* ChassisHandle = UVehicleUtilities::GetInternalHandle(Chassis.Get());
	if (!ChassisHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("WheelPhysics: No Valid Chassis!!!"));
		return;
	}

	//Update anim cache
	PrevHubChassisLocation = Suspension.State.HubChassisLocation;
	PrevHubChassisRotation = Suspension.State.HubChassisRotation;
	TimeSinceLastPhysicsTick = 0.f;

	ChassisAsyncWorldTransform = Chaos::FParticleUtilitiesGT::GetActorWorldTransform(ChassisHandle);
	if (!ChassisAsyncWorldTransform.IsRotationNormalized()) ChassisAsyncWorldTransform.NormalizeRotation();

	//Suspension
	Suspension.UpdateSuspension(
		WheelConfig.Radius,
		WheelConfig.Width,
		Wheel.State.TireForce,
		SuspensionKinematicsConfig,
		SuspensionSpringConfig,
		GetRelativeTransform(),
		ChassisAsyncWorldTransform,
		GetWorld(),
		ChassisHandle,
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

	ApplyWheelForce(ChassisHandle);
}

bool UVehicleWheelComponent::CheckHasBeenMoved()
{
	FVector3f CurrLocation = (FVector3f)GetRelativeLocation();
	FQuat4f CurrRotation = (FQuat4f)GetRelativeTransform().GetRotation();
	bool bIsMoved = CachedRelativeLocation != CurrLocation || CachedRelativeRotation != CurrRotation;
	if (bIsMoved)
	{
		CachedRelativeLocation = CurrLocation;
		CachedRelativeRotation = CurrRotation;

		if (WheelCoordinator.IsValid())
		{
			WheelCoordinator->NotifyWheelMoved();
		}
		else
		{
			WheelCoordinator = UVehicleWheelCoordinatorComponent::FindWheelCoordinator(Chassis.Get());
			if (WheelCoordinator.IsValid())
			{
				WheelCoordinator->NotifyWheelMoved();
			}
		}
	}
	return bIsMoved;
}

void UVehicleWheelComponent::StartUpdateSolidAxlePhysics(
	float InSteeringAngle,
	FVector& OutApporximatedWheelWorldPos,
	FVehicleSuspensionSimContext& Ctx
)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForge_Wheel_UpdatePhysics);

	Chaos::FRigidBodyHandle_Internal* ChassisHandle = UVehicleUtilities::GetInternalHandle(Chassis.Get());
	if (!ChassisHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("WheelPhysics: No Valid Chassis!!!"));
		return;
	}

	ChassisAsyncWorldTransform = Chaos::FParticleUtilitiesGT::GetActorWorldTransform(ChassisHandle);

	return Suspension.StartUpdateSolidAxle(
		WheelConfig.Radius,
		WheelConfig.Width,
		SuspensionKinematicsConfig,
		GetRelativeTransform(),
		ChassisAsyncWorldTransform,
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
	const FVector& InSolidAxleEndWorldPos,
	const FVector& InAxleWorldDirection
)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForge_Wheel_UpdatePhysics);

	// get rigid handle to get world com position
	Chaos::FRigidBodyHandle_Internal* ChassisHandle = nullptr;
	if (Chassis.IsValid())
	{
		if (const FBodyInstance* BodyInstance = Chassis->GetBodyInstance())
		{
			if (const auto Handle = BodyInstance->ActorHandle)
			{
				ChassisHandle = Handle->GetPhysicsThreadAPI();
			}
		}
	}
	if (!ChassisHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("WheelPhysics: No Valid Chassis!!!"));
		return;
	}

	Suspension.FinalizeUpdateSolidAxle(
		WheelConfig.Radius,
		SuspensionKinematicsConfig,
		SuspensionSpringConfig,
		ChassisAsyncWorldTransform,
		ChassisHandle,
		InPhysicsDeltaTime, 
		InSwaybarForce,
		Ctx,
		InSolidAxleEndWorldPos,
		InAxleWorldDirection,
		Wheel.State.TireForce
	);

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

	ApplyWheelForce(ChassisHandle);
}

void UVehicleWheelComponent::ApplySuspensionStateDirect(float InExtensionRatio, float InSteeringAngle)
{
	const int32 Iteration = 2;
	for (int32 i = 0; i < Iteration; i++)
	{
		Suspension.State = Suspension.SolveKinematicsAtExtension(
			WheelConfig.Radius,
			SuspensionKinematicsConfig,
			GetRelativeTransform(),
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
	Suspension.StartSolveSolidAxleAtExtension(
		Suspension.State,
		WheelConfig.Radius,
		SuspensionKinematicsConfig,
		GetRelativeTransform(),
		InExtensionRatio, 
		InSteeringAngle, 
		OutApporximatedWheelWorldPos, 
		Ctx
	);
}

void UVehicleWheelComponent::FinalizeApplySolidAxleStateDirect(
	FVehicleSuspensionSimContext& Ctx, 
	const FVector& InSolidAxleEndWorldPos, 
	const FVector& InAxleWorldDirection)
{
	Suspension.State = Suspension.FinalizeSolveSolidAxleAtExtension(
		WheelConfig.Radius,
		SuspensionKinematicsConfig,
		Ctx,
		InSolidAxleEndWorldPos,
		InAxleWorldDirection
	);

	UpdateWheelAnim();
}

void UVehicleWheelComponent::GetWheelCoordinator(UVehicleWheelCoordinatorComponent*& OutWheelCoordinator)
{
	OutWheelCoordinator = WheelCoordinator.Get();
}

float UVehicleWheelComponent::ComputeFeedBackTorque()
{
	// get machanism trail
	FTransform3f ChassisTransform = FTransform3f(ChassisAsyncWorldTransform);
	FVector3f SteerAxis = ChassisTransform.TransformVector(Suspension.State.SteerAxisChassisDirection);
	FVector3f Arm = WheelConfig.Radius * SteerAxis.ProjectOnToNormal(Suspension.State.ImpactWorldNormal);
	FVector3f Torque = FVector3f::CrossProduct(Arm, Wheel.State.TireForce);
	return FVector3f::DotProduct(SteerAxis, Torque);
}

FTransform UVehicleWheelComponent::GetChassisWorldTransform()
{
	return Chassis.Get() ? Chassis->GetComponentTransform() : FTransform();
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
	UStaticMesh* NewWheelMesh, 
	FTransform WheelMeshRelatvieTransform,
	UStaticMesh* NewBrakeMesh, 
	FTransform BrakeMeshRelativeTransform)
{
	if (!IsValid(WheelHubComponent) || !IsValid(WheelMeshComponent) || !IsValid(BrakeMeshComponent))return false;

	WheelHubComponent->PrimaryComponentTick.bCanEverTick = false;
	WheelHubComponent->PrimaryComponentTick.bStartWithTickEnabled = false;

	//if is left wheel
	if (GetRelativeLocation().Y < 0)
	{
		FTransform TempTrans = FTransform(FQuat(FRotator(0.f)), FVector(0.f), FVector(1.f, -1.f, 1.f));
		WheelMeshRelatvieTransform *= TempTrans;
		BrakeMeshRelativeTransform *= TempTrans;
	}

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
		WheelMesh,
		WheelMeshTransform, 
		BrakeMesh, 
		BrakeMeshTransform);
}

void UVehicleWheelComponent::UpdateWheelAnim(float DeltaTime, float MaxAnimAngularVelocity)
{
	if (!IsValid(WheelHubComponent) || !IsValid(WheelMeshComponent))return;

	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForge_Wheel_UpdateAnimation);

	// update wheel position & alignment
	if (DeltaTime > 0)
	{
		TimeSinceLastPhysicsTick += DeltaTime;

		// blend between physics frames
		float Alpha = FMath::Clamp(TimeSinceLastPhysicsTick / Wheel.State.PhysicsDeltaTime, 0.0f, 1.0f);
		FVector3f TargetAnimHubePos = FMath::Lerp(PrevHubChassisLocation, Suspension.State.HubChassisLocation, Alpha);
		FQuat4f TargetAnimHubRot = FMath::Lerp(PrevHubChassisRotation, Suspension.State.HubChassisRotation, Alpha);

		// interp
		AnimHubChassisLocation = (FVector3f)FMath::VInterpTo((FVector)AnimHubChassisLocation, (FVector)TargetAnimHubePos, DeltaTime, AnimInterpSpeed);
		AnimHubChassisRotation = (FQuat4f)FMath::QInterpTo((FQuat)AnimHubChassisRotation, (FQuat)TargetAnimHubRot, DeltaTime, AnimInterpSpeed);
	}
	else
	{
		TimeSinceLastPhysicsTick = 0.f;
		AnimHubChassisLocation = Suspension.State.HubChassisLocation;
		AnimHubChassisRotation = Suspension.State.HubChassisRotation;
	}

	FTransform T = FTransform(
		GetRelativeTransform().InverseTransformRotation((FQuat)AnimHubChassisRotation),
		GetRelativeTransform().InverseTransformPositionNoScale((FVector)AnimHubChassisLocation),
			FVector(1.f)
	);
	WheelHubComponent->SetRelativeTransform(
		T,
		false,
		nullptr,
		ETeleportType::TeleportPhysics
	);
	
	// update wheel spin
	float Omega = (MaxAnimAngularVelocity > 0.f) ?
		FMath::Clamp(Wheel.State.AngularVelocity, -MaxAnimAngularVelocity, MaxAnimAngularVelocity) :
		Wheel.State.AngularVelocity;

	AnimWheelRotationAngle += FMath::RadiansToDegrees(Omega) * DeltaTime;
	AnimWheelRotationAngle = FMath::Fmod(AnimWheelRotationAngle, 360.0f);

	FQuat NewRot = FQuat(FVector(0.f, 1.f, 0.f), FMath::DegreesToRadians(AnimWheelRotationAngle));

	if (IsValid(WheelMeshComponent))
	{
		WheelMeshComponent->SetRelativeRotation(
			NewRot,
			false,
			nullptr,
			ETeleportType::TeleportPhysics
		);
	}
}

FTransform UVehicleWheelComponent::GetSkidMarkWorldTransform(float InSkidMarkBias, float InSkidMarkScale)
{
	if (SuspensionKinematicsConfig.RayCastMode == ESuspensionRayCastMode::LineTrace)
	{
		// the impact point of line trace is always on the outer side of the wheel
		InSkidMarkBias -= WheelConfig.Width * 0.5f;
	}
	FVector N = (FVector)Suspension.State.ImpactWorldNormal;
	FQuat q = FRotationMatrix::MakeFromZX(N, FVector(Suspension.State.ImpactWorldVelocity)).ToQuat();
	FVector v = Suspension.State.ImpactWorldLocation + N;	//to avoid spawned in the ground
	FVector rightV = FVector::VectorPlaneProject(FVector(Suspension.State.WheelWorldRightVector), N).GetSafeNormal();
	v += rightV * InSkidMarkBias * (Suspension.State.bIsRightWheel ? 1.f : -1.f);
	v += q.GetForwardVector() * -0.25 * InSkidMarkScale;	//-0.25 is for no reason
	FVector LocalLinVel = FVector((FVector2D)Wheel.State.LocalLinearVelocity, 0.f);
	return FTransform(q, v, LocalLinVel);
}

FQuat4f UVehicleWheelComponent::UpdateSuspensionArmAnim(USceneComponent* InArmMesh, FRotator InRotationOffset)
{
	//float PosSign = Suspension.State.bIsRightWheel ? 1.f : -1.f;
	//InRotationOffset.Yaw *= PosSign;
	//InRotationOffset.Roll *= PosSign;
	//FQuat4f TempInitialRot = (FQuat4f)InRotationOffset.Quaternion();
	//FQuat4f TempArmRot = Suspension.MakeQuatFrom2DVectors(
	//	FVector2f(0.f, -PosSign),
	//	FVector2f(AnimKnucklePos2D.X, AnimKnucklePos2D.Y * -PosSign),
	//	(FVector3f)GetRelativeTransform().GetRotation().GetForwardVector());
	//TempArmRot *= TempInitialRot;
	//TempArmRot.Normalize();
	//
	//if (IsValid(InArmMesh))
	//{
	//	InArmMesh->SetRelativeRotation((FQuat)TempArmRot);
	//}
	//
	//return TempArmRot;
	return FQuat4f::Identity;
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

	//float PosSign = Suspension.State.bIsRightWheel ? 1.f : -1.f;
	//InRotationOffset.Yaw *= PosSign;
	//InRotationOffset.Roll *= PosSign;
	//
	//InKnuckleOffset.Y *= PosSign;
	//
	//FTransform3f RelativeTrans = (FTransform3f)GetRelativeTransform();
	//FVector3f KnuckleRelativePos = FVector3f(Suspension.State.KnuckleRelativePos);
	//FTransform3f WheelRelativeTransform = GetWheelRelativeTransform();
	//FVector3f KnuckleOffset3D = WheelRelativeTransform.TransformPosition(FVector3f(0.f, InKnuckleOffset.Y, InKnuckleOffset.X));
	//KnuckleOffset3D -= WheelRelativeTransform.GetLocation();
	//
	//FVector3f PivotPos = RelativeTrans.TransformPosition(Suspension.SuspensionPlaneToZYPlane(FVector2f(0.f), PosSign));
	//FVector3f ArmDir = (PivotPos - KnuckleRelativePos).GetSafeNormal();
	//
	//FVector3f OffsetJointPos = KnuckleRelativePos + ArmDir * InOffsetAlongArm;
	//FVector3f OffsetInitialJointPos = RelativeTrans.TransformPosition(FVector3f(0.f, InOffsetAlongArm * -PosSign, 0.f));
	//
	//OffsetJointPos += KnuckleOffset3D;
	//OffsetInitialJointPos += KnuckleOffset3D;
	//
	//FVector3f SpringMeshRelativePos = (FVector3f)InSpringMesh->GetRelativeLocation();
	//FVector3f AnimStrut = SpringMeshRelativePos - OffsetJointPos;
	//FVector3f AnimStrutDir = AnimStrut.GetSafeNormal();
	//
	//FQuat4f AnimStrutRot = FRotationMatrix44f::MakeFromXZ(RelativeTrans.GetRotation().GetForwardVector(), AnimStrutDir).ToQuat();
	//
	//float InitialLength = (SpringMeshRelativePos - OffsetInitialJointPos).Length();
	//float CurrentLength = AnimStrut.Length();
	//
	//float Scale = UVehicleUtilities::SafeDivide(CurrentLength, InitialLength);
	//FVector3f Scale3D = FVector3f(1.f) + (FVector3f)InScaleAxis * (Scale - 1.f);
	//Scale3D *= (FVector3f)InInitialScale;
	//
	//InSpringMesh->SetRelativeRotation((FQuat)AnimStrutRot);
	//InSpringMesh->SetRelativeScale3D((FVector)Scale3D);
	//
	//return FTransform3f(AnimStrutRot, SpringMeshRelativePos, Scale3D);
	return FTransform3f::Identity;
}

void UVehicleWheelComponent::AttachComponentToKnuckle(USceneComponent* InComponent, FTransform InTransform)
{
	//if (!IsValid(InComponent))return;
	//InComponent->AttachToComponent(WheelKnuckleComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//InComponent->SetRelativeTransform(InTransform);
}
