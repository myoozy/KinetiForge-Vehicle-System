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

void UVehicleWheelComponent::ApplyWheelForce(Chaos::FRigidBodyHandle_Internal* InChassisHandle)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForge_Wheel_ApplyForce);

	if (!Suspension.State.bHitGround)return;

	FVector WidthBias = FVector(0.f);
	FVector PosToApplyImpulse = FVector(0.f);

	switch (SuspensionKinematicsConfig.LocationToApplyForce)
	{
	case EVehicleForceApplicationPoint::ImpactPoint:
		PosToApplyImpulse = Suspension.State.ImpactWorldLocation;
		break;
	case EVehicleForceApplicationPoint::WheelCenter:
		PosToApplyImpulse = ChassisAsyncWorldTransform.TransformPositionNoScale(
			(FVector)Suspension.State.HubChassisLocation);
		break;
	default:
		PosToApplyImpulse = Suspension.State.ImpactWorldLocation;
		break;
	}

	// get world com
	Chaos::FVec3 ChassisWorldCOM = InChassisHandle != nullptr ? 
		Chaos::FParticleUtilitiesGT::GetCoMWorldPosition(InChassisHandle) : ChassisAsyncWorldTransform.GetLocation();

	// get arm
	Chaos::FVec3 LeverArmVec = PosToApplyImpulse - ChassisWorldCOM;

	// get normal of ground
	FVector ImpactNormal = (FVector)Suspension.State.ImpactWorldNormal;

	/*-------------------------------------APPLY FORCE---------------------------------------*/

	// project the suspension force onto the impact normal
	// so that the car will not move by itself when parked
	FVector SuspensionForceProj = ImpactNormal * Suspension.State.ForceAlongImpactNormal;
	FVector Impulse = ((FVector)Wheel.State.TireForce + SuspensionForceProj) * Wheel.CurrentContext.MacroDeltaTime;
	Impulse *= 100.;	// because of the unit of unreal engine

	// apply force to Chassis
	if (InChassisHandle)
	{
		FVector AngularImpulse = FVector::CrossProduct(LeverArmVec, Impulse);
		InChassisHandle->SetLinearImpulse(InChassisHandle->LinearImpulse() + Impulse, false);
		InChassisHandle->SetAngularImpulse(InChassisHandle->AngularImpulse() + AngularImpulse, false);
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

void UVehicleWheelComponent::PreStepIndependentSuspension(
	float InMacroDeltaTime, 
	float InSteeringAngle, 
	float InSwaybarForce)
{
	// get rigid handle to get world com position
	ChassisHandle = UVehicleUtilities::GetInternalHandle(Chassis.Get());
	if (!ChassisHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("WheelPhysics: No Valid Chassis!!!"));
		return;
	}

	//Update anim cache
	PrevLowerBallJointChassisLocation = Suspension.State.LowerBallJointChassisLocation;
	PrevUpperBallJointChassisLocation = Suspension.State.UpperBallJointChassisLocation;
	PrevHubChassisLocation = Suspension.State.HubChassisLocation;
	PrevHubChassisRotation = Suspension.State.HubChassisRotation;
	TimeSinceLastPhysicsTick = 0.f;

	ChassisAsyncWorldTransform = Chaos::FParticleUtilitiesGT::GetActorWorldTransform(ChassisHandle);
	if (!ChassisAsyncWorldTransform.IsRotationNormalized()) ChassisAsyncWorldTransform.NormalizeRotation();

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
		InMacroDeltaTime,
		InSteeringAngle,
		InSwaybarForce);
}

void UVehicleWheelComponent::StartPreStepSolidAxleSuspension(
	float InSteeringAngle, 
	FVector& OutHitWorldLocation, 
	FVehicleSuspensionSimContext& Ctx)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForge_Wheel_UpdatePhysics);

	ChassisHandle = UVehicleUtilities::GetInternalHandle(Chassis.Get());
	if (!ChassisHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("WheelPhysics: No Valid Chassis!!!"));
		return;
	}

	ChassisAsyncWorldTransform = Chaos::FParticleUtilitiesGT::GetActorWorldTransform(ChassisHandle);

	Suspension.StartUpdateSolidAxle(
		WheelConfig.Radius,
		WheelConfig.Width,
		SuspensionKinematicsConfig,
		GetRelativeTransform(),
		ChassisAsyncWorldTransform,
		GetWorld(),
		InSteeringAngle,
		OutHitWorldLocation,
		Ctx);
}

void UVehicleWheelComponent::FinalizePreStepSolidAxleSuspension(
	float InMacroDeltaTime, 
	float InSwaybarForce, 
	FVehicleSuspensionSimContext& Ctx, 
	const float InTrackWidth, 
	const FVector& InThisWheelHitWorldLocation, 
	const FVector& InOtherWheelHitWorldLocation)
{
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
		InMacroDeltaTime,
		InSwaybarForce,
		Ctx,
		InTrackWidth,
		InThisWheelHitWorldLocation,
		InOtherWheelHitWorldLocation,
		Wheel.State.TireForce
	);
}

void UVehicleWheelComponent::PreStepWheel(
	float InMacroDeltaTime)
{
	Wheel.PreStep(InMacroDeltaTime, Suspension.State, TireConfig);
}

void UVehicleWheelComponent::SubStepWheel(
	float InSubstepDeltaTime, 
	float InDriveTorque, 
	float InBrakeTorque, 
	float InHandbrakeTorque, 
	float InReflectedInertia)
{
	Wheel.Substep(InSubstepDeltaTime, InDriveTorque, InBrakeTorque, InHandbrakeTorque, InReflectedInertia, 
		WheelConfig, TireConfig, ABSConfig, Suspension.State);
}

void UVehicleWheelComponent::PostStepWheel()
{
	Wheel.PostStep();
	ApplyWheelForce(ChassisHandle);
}

// Called every frame
void UVehicleWheelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

#if WITH_EDITOR
	if (ConfigSyncInterval >= 0.f)
	{
		TimeSinceLastConfigSync += DeltaTime;
		if (TimeSinceLastConfigSync > ConfigSyncInterval)
		{
			TimeSinceLastConfigSync -= ConfigSyncInterval;

			// update cached curves
			Suspension.UpdateCachedLUTs(SuspensionKinematicsConfig);
			Wheel.UpdateCachedLUTs(TireConfig);
		}
	}
	else
	{
		Suspension.UpdateCachedLUTs(SuspensionKinematicsConfig);
		Wheel.UpdateCachedLUTs(TireConfig);
	}
#endif
	
	// update animation
	if (bUpdateAnimAutomatically)
	{
		UpdateWheelAnim(DeltaTime, AnimWheelMaxRotationSpeed);
	}
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

void UVehicleWheelComponent::SetSuspensionKinematicsConfig(const FVehicleSuspensionKinematicsConfig& NewConfig)
{
	SuspensionKinematicsConfig = NewConfig;
	Suspension.UpdateCachedLUTs(NewConfig);
	CacheDesignedHubTransform();
}

void UVehicleWheelComponent::SetSuspensionSpringConfig(const FVehicleSuspensionSpringConfig& NewConfig)
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

FVector3f UVehicleWheelComponent::GetTopMountChassisLocation()
{
	return Suspension.GetTopMountChassisLocation(
		SuspensionKinematicsConfig,
		FTransform3f(GetRelativeTransform())
	);
}

void UVehicleWheelComponent::GetLowerWishboneState(FVector& OutPivotChassisLocation, FVector& OutAxisChassisDirection, FVector& OutBallJointChassisLocation)
{
	FVector3f Pivot3f, Axis3f, BallJoint3f;

	Suspension.GetLowerWishboneState(
		SuspensionKinematicsConfig,
		Suspension.State,
		FTransform3f(GetRelativeTransform()),
		Pivot3f,
		Axis3f,
		BallJoint3f
	);

	OutPivotChassisLocation = FVector(Pivot3f);
	OutAxisChassisDirection = FVector(Axis3f);
	OutBallJointChassisLocation = FVector(BallJoint3f);
}

void UVehicleWheelComponent::GetUpperWishboneState(FVector& OutPivotChassisLocation, FVector& OutAxisChassisDirection, FVector& OutBallJointChassisLocation)
{
	FVector3f Pivot3f, Axis3f, BallJoint3f;

	Suspension.GetUpperWishboneState(
		SuspensionKinematicsConfig,
		Suspension.State,
		FTransform3f(GetRelativeTransform()),
		Pivot3f,
		Axis3f,
		BallJoint3f
	);

	OutPivotChassisLocation = FVector(Pivot3f);
	OutAxisChassisDirection = FVector(Axis3f);
	OutBallJointChassisLocation = FVector(BallJoint3f);
}

FTransform3f UVehicleWheelComponent::GetHubChassisTransform()
{
	return FTransform3f(
		Suspension.State.HubChassisRotation,
		Suspension.State.HubChassisLocation
	);
}

float UVehicleWheelComponent::GetSlipRatio(bool bTransientSlip)
{
	if (bTransientSlip)
	{
		return Wheel.State.TransientSlip.X;
	}
	else
	{
		return Wheel.State.SlipRatio;
	}
}

float UVehicleWheelComponent::GetSlipAngle(bool bTransientSlip)
{
	if (bTransientSlip)
	{
		return FMath::RadiansToDegrees(FMath::Atan(Wheel.State.TransientSlip.Y));
	}
	else
	{
		return Wheel.State.SlipAngle;
	}
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
	float NormalizedSx = Wheel.State.TransientSlip.X * LongitudinalScale * SpeedScaleX * Wx * NormFactor;

	// also if the lateral velocity is too low, should scale the affection of slip angle
	float SpeedScaleY = FMath::Clamp(0.5f * Wheel.State.LocalLinearVelocity.Y, -1.f, 1.f);
	float NormalizedSy = FMath::Atan(Wheel.State.TransientSlip.Y) / (0.5f * PI) * LateralScale * SpeedScaleY * Wy * NormFactor;

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

	PreStepIndependentSuspension(
		InPhysicsDeltaTime,
		InSteeringAngle,
		InSwaybarForce
	);

	PreStepWheel(
		InPhysicsDeltaTime
	);
	SubStepWheel(
		InPhysicsDeltaTime,
		InDriveTorque,
		InBrakeTorque,
		InHandbrakeTorque,
		InReflectedInertia
	);
	PostStepWheel();
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
	FVector& OutHitWorldLocation,
	FVehicleSuspensionSimContext& Ctx
)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForge_Wheel_UpdatePhysics);

	StartPreStepSolidAxleSuspension(
		InSteeringAngle,
		OutHitWorldLocation,
		Ctx
	);
}

void UVehicleWheelComponent::FinalizeUpdateSolidAxlePhysics(
	float InPhysicsDeltaTime,
	float InDriveTorque,
	float InBrakeTorque,
	float InHandbrakeTorque,
	float InSwaybarForce,
	float InReflectedInertia,
	FVehicleSuspensionSimContext& Ctx,
	const float InTrackWidth,
	const FVector& InThisWheelHitWorldLocation,
	const FVector& InOtherWheelHitWorldLocation
)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForge_Wheel_UpdatePhysics);

	FinalizePreStepSolidAxleSuspension(
		InPhysicsDeltaTime,
		InSwaybarForce,
		Ctx,
		InTrackWidth,
		InThisWheelHitWorldLocation,
		InOtherWheelHitWorldLocation
	);

	PreStepWheel(
		InPhysicsDeltaTime
	);
	SubStepWheel(
		InPhysicsDeltaTime,
		InDriveTorque,
		InBrakeTorque,
		InHandbrakeTorque,
		InReflectedInertia
	);
	PostStepWheel();
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
	FVector& OutHitWorldLocation,
	FVehicleSuspensionSimContext& Ctx)
{
	Suspension.StartSolveSolidAxleAtExtension(
		Suspension.State,
		WheelConfig.Radius,
		SuspensionKinematicsConfig,
		GetRelativeTransform(),
		InExtensionRatio, 
		InSteeringAngle, 
		OutHitWorldLocation,
		Ctx
	);
}

void UVehicleWheelComponent::FinalizeApplySolidAxleStateDirect(
	FVehicleSuspensionSimContext& Ctx,
	const float InTrackWidth,
	const FVector& InThisWheelHitWorldLocation,
	const FVector& InOtherWheelHitWorldLocation)
{
	Suspension.State = Suspension.FinalizeSolveSolidAxleAtExtension(
		WheelConfig.Radius,
		SuspensionKinematicsConfig,
		Ctx,
		InTrackWidth,
		InThisWheelHitWorldLocation,
		InOtherWheelHitWorldLocation
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
		float Alpha = FMath::Clamp(TimeSinceLastPhysicsTick * Wheel.CurrentContext.MacroDeltaTimeInv, 0.0f, 1.0f);
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
	if (SuspensionKinematicsConfig.RayCastMode == EVehicleSuspensionRayCastMode::LineTrace)
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

FTransform UVehicleWheelComponent::UpdateWishboneAnim(
	USceneComponent* InArmMesh,
	const bool bFollowUpperWishbone,
	const FTransform InOffset,
	const FVector InMeshForwardVector,
	const FVector InMeshRightVector,
	const bool bScaleToMatchLength,
	const float MeshDesignLength
)
{
	const FTransform3f& TransformRelative = FTransform3f(GetRelativeTransform());

	FVector3f PivotChassis;
	FVector3f WishboneAxisChassis;
	FVector3f BallJointChassis;
	FVector3f WishboneDirectionChassis;
	float RealWishboneLength = 0.f;

	if (bFollowUpperWishbone)
	{
		Suspension.GetUpperWishboneState(
			SuspensionKinematicsConfig,
			Suspension.State,
			TransformRelative,
			PivotChassis,
			WishboneAxisChassis,
			BallJointChassis
		);

		FVector3f PivotToBJ = BallJointChassis - PivotChassis;
		RealWishboneLength = PivotToBJ.Size();
		WishboneDirectionChassis = PivotToBJ.GetSafeNormal();
	}
	else
	{
		Suspension.GetLowerWishboneState(
			SuspensionKinematicsConfig,
			Suspension.State,
			TransformRelative,
			PivotChassis,
			WishboneAxisChassis,
			BallJointChassis
		);

		FVector3f PivotToBJ = BallJointChassis - PivotChassis;
		RealWishboneLength = PivotToBJ.Size();
		WishboneDirectionChassis = PivotToBJ.GetSafeNormal();
	}

	// 1. Determine the target coordinate system (Target Basis)
	FVector3f TargetAxis = WishboneAxisChassis;
	FVector3f TargetDir = WishboneDirectionChassis;

	// Force orthogonality: Ensure that TargetDir and TargetAxis are strictly perpendicular 
	// (to prevent unnatural, non-uniform distortion in the swing arm model)
	TargetDir = TargetDir - FVector3f::DotProduct(TargetDir, TargetAxis) * TargetAxis;
	TargetDir.Normalize();

	// 2. Two-step quaternion rotation method
	// Step 1: First, align the mesh's Forward axis with the swing arm's pivot
	FQuat4f Q1 = FQuat4f::FindBetweenNormals(FVector3f(InMeshForwardVector), TargetAxis);

	// Step 2: After rotating the mesh's Right vector by Q1, 
	// it may not yet be aligned with the sphere's center.
	// We extract its current actual direction 
	// and then “twist” it around the rotation axis to align it with TargetDir.
	FVector3f RotatedMeshRight = Q1.RotateVector(FVector3f(InMeshRightVector));
	FQuat4f Q2 = FQuat4f::FindBetweenNormals(RotatedMeshRight, TargetDir);

	// Quaternion multiplication is read from right to left: 
	// Q1 is evaluated first, followed by Q2
	FQuat4f FinalRotation = Q2 * Q1;

	FVector3f FinalScale = FVector3f::OneVector;
	if (bScaleToMatchLength && MeshDesignLength > KINDA_SMALL_NUMBER)
	{
		float ScaleRatio = RealWishboneLength / MeshDesignLength;

		// Use GetAbs() to extract the specified direction.
		// For example, if InMeshRightVector is (0, 1, 0), GetAbs() also returns (0, 1, 0)
		// After adding (ScaleRatio - 1), Scale becomes (1, ScaleRatio, 1)
		FinalScale += FVector3f(InMeshRightVector).GetAbs() * (ScaleRatio - 1.0f);
	}

	FTransform3f BaseTransform(FinalRotation, PivotChassis, FinalScale);
	FTransform FinalTransform = InOffset * FTransform(BaseTransform);

	const FTransform& ChassisWorldTransform = Chassis.IsValid() ? Chassis->GetComponentTransform() : ChassisAsyncWorldTransform;
	if (InArmMesh)
	{
		InArmMesh->SetWorldTransform(FinalTransform * ChassisWorldTransform, false, nullptr, ETeleportType::TeleportPhysics);
	}

	return FinalTransform;
}

FTransform UVehicleWheelComponent::UpdateSuspensionArmAnim(USceneComponent* InArmMesh, const bool bFollowUpperWishbone, const FTransform InOffset, const FVector InMeshForwardVector, const FVector InMeshRightVector, const bool bScaleToMatchLength, const float MeshDesignLength)
{
	return UpdateWishboneAnim(
		InArmMesh,
		bFollowUpperWishbone,
		InOffset,
		InMeshForwardVector,
		InMeshRightVector,
		bScaleToMatchLength,
		MeshDesignLength
	);
}

void UVehicleWheelComponent::UpdateShockAbsorberAnim(USceneComponent* InUpperStrutMesh, USceneComponent* InLowerStrutMesh, USceneComponent* InSpringMesh, const float SpringDesignLength, const FVector InMeshUpVector)
{
	// 1. 获取物理塔顶和底座在 Chassis 空间下的坐标
	// (注意：对于双叉臂，底座可能在下摆臂上；对于麦弗逊，可能是转向节。这里统一用你 state 里存好的)
	FVector TopMountChassis = (FVector)Suspension.State.TopMountChassisLocation;
	FVector LowerMountChassis = (FVector)Suspension.State.LowerBallJointChassisLocation;

	// 2. 算出这条共享的“减震器绝对直线”
	FVector StrutVector = TopMountChassis - LowerMountChassis;
	float CurrentStrutLength = StrutVector.Size();
	FVector StrutDirectionUp = StrutVector.GetSafeNormal();   // 指向塔顶
	FVector StrutDirectionDown = -StrutDirectionUp;           // 指向地面

	// 3. 算出共同的旋转基准 (将模型默认的 UpVector 对齐到减震器轴线上)
	FQuat RotationLookingUp = FQuat::FindBetweenNormals(InMeshUpVector, StrutDirectionUp);
	FQuat RotationLookingDown = FQuat::FindBetweenNormals(InMeshUpVector, StrutDirectionDown);

	const FTransform& ChassisWorldTransform = Chassis.IsValid() ? Chassis->GetComponentTransform() : ChassisAsyncWorldTransform;

	// ==========================================
	// 组装 1：上半筒 (Upper Strut)
	// 原点在塔顶，不可压缩，方向朝下 (插入下半筒)
	// ==========================================
	if (InUpperStrutMesh)
	{
		FTransform UpperTransform(RotationLookingDown, TopMountChassis, FVector::OneVector);
		InUpperStrutMesh->SetWorldTransform(UpperTransform * ChassisWorldTransform, false, nullptr, ETeleportType::TeleportPhysics);
	}

	// ==========================================
	// 组装 2：下半筒 (Lower Strut)
	// 原点在底座，不可压缩，方向朝上 (套住上半筒)
	// ==========================================
	if (InLowerStrutMesh)
	{
		FTransform LowerTransform(RotationLookingUp, LowerMountChassis, FVector::OneVector);
		InLowerStrutMesh->SetWorldTransform(LowerTransform * ChassisWorldTransform, false, nullptr, ETeleportType::TeleportPhysics);
	}

	// ==========================================
	// 组装 3：弹簧 (Spring)
	// 原点在底座，方向朝上，并且 Z 轴需要根据行程动态缩放！
	// ==========================================
	if (InSpringMesh)
	{
		// 弹簧的当前长度，可能需要减去一些两端底座的厚度常数，这里简化为总长度
		// 如果你的弹簧只是减震器的一部分，你可以传入一个 SpringOffset 扣除掉不可压缩长度
		float ScaleZ = Suspension.State.SuspensionCurrentLength / FMath::Max(0.1f, SpringDesignLength);

		FVector SpringScale = FVector::OneVector;
		// 同样的魔法：根据传入的轴向，只缩放那个轴
		SpringScale += InMeshUpVector.GetAbs() * (ScaleZ - 1.0f);

		FTransform SpringTransform(RotationLookingUp, LowerMountChassis, SpringScale);
		InSpringMesh->SetWorldTransform(SpringTransform * ChassisWorldTransform, false, nullptr, ETeleportType::TeleportPhysics);
	}
}

void UVehicleWheelComponent::UpdateSuspensionSpringAnim(USceneComponent* InUpperStrutMesh, USceneComponent* InLowerStrutMesh, USceneComponent* InSpringMesh, const float SpringDesignLength, const FVector InMeshUpVector)
{
	UpdateShockAbsorberAnim(
		InUpperStrutMesh,
		InLowerStrutMesh,
		InSpringMesh,
		SpringDesignLength,
		InMeshUpVector
	);
}

void UVehicleWheelComponent::OrientAndScaleToLocation(USceneComponent* InComponent, const FVector& TargetWorldLocation, const float UnscaledLength, const FVector TrackingAxisLocal)
{
	if (!InComponent) return;

	// 1. 获取当前世界坐标，计算目标方向和距离
	FVector StartLoc = InComponent->GetComponentLocation();
	FVector Dir = TargetWorldLocation - StartLoc;
	float Dist = Dir.Size();

	// 防呆：如果距离极短（比如重合了），直接跳过计算，防止除零崩溃
	if (Dist < KINDA_SMALL_NUMBER) return;

	FVector DirNormal = Dir / Dist;

	// 2. 【解决旋转】：计算需要补充的旋转量
	// 先把模型局部的 TrackingAxis 转换到当前的世界方向
	FVector CurrentTrackingWorld = InComponent->GetComponentQuat().RotateVector(TrackingAxisLocal).GetSafeNormal();

	// 计算从当前轴向旋转到目标方向的差值 Quat
	FQuat DeltaRot = FQuat::FindBetweenNormals(CurrentTrackingWorld, DirNormal);

	// 叠加到现有的旋转上
	InComponent->SetWorldRotation(DeltaRot * InComponent->GetComponentQuat(), false, nullptr, ETeleportType::TeleportPhysics);

	// 3. 【解决缩放】：沿指定轴向单向拉伸
	float ScaleFactor = Dist / FMath::Max(0.1f, UnscaledLength);

	// 假设模型基础缩放为 1，利用 GetAbs() 自动锁定到策划填写的那个轴上
	FVector NewScale = FVector::OneVector + TrackingAxisLocal.GetAbs() * (ScaleFactor - 1.0f);

	InComponent->SetWorldScale3D(NewScale);
}

void UVehicleWheelComponent::AttachComponentToWheelHub(
	USceneComponent* InComponent,
	bool bKeepWorldTransform)
{
	if (InComponent)
	{
		InComponent->AttachToComponent(
			WheelHubComponent, 
			bKeepWorldTransform ? FAttachmentTransformRules::KeepWorldTransform : FAttachmentTransformRules::KeepRelativeTransform
		);
	}
}