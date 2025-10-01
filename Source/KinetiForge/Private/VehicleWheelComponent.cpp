// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)


#include "VehicleWheelComponent.h"
#include "VehicleWheelCoordinatorComponent.h"

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

	WheelHubComponent = CreateDefaultSubobject<USceneComponent>(FName(TEXT("WheelHubComponent")));
	WheelMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("WheelMeshComponent")));
	BrakeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("BrakeMeshComponent")));
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
	//get parent
	CurrentWorld = GetWorld();
	Carbody = UVehicleWheelCoordinatorComponent::FindPhysicalParent(this);

	if (!IsValid(Carbody))
	{
		UE_LOG(LogTemp, Warning, TEXT("WheelPhysics: Carbody Not Found!"));
	}

	Suspension.Initialize(this);
	Wheel.Initialize(this);

	//initialize meshes
	GenerateMeshComponents();
}

void UVehicleWheelComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	if (IsValid(WheelHubComponent) && !WheelHubComponent->IsBeingDestroyed())
	{
		WheelHubComponent->DestroyComponent();
		WheelHubComponent = nullptr;
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
	if (SuspensionKinematicsConfig.CasterCurve)SuspensionKinematicsConfig.CasterCurve = nullptr;
	if (SuspensionKinematicsConfig.ToeCurve)SuspensionKinematicsConfig.ToeCurve = nullptr;
	
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

bool UVehicleWheelComponent::GenerateMeshComponents()
{
	if (!IsValid(WheelHubComponent))
	{
		WheelHubComponent = NewObject<USceneComponent>(this, USceneComponent::StaticClass());
		GetOwner()->AddInstanceComponent(WheelHubComponent);
		WheelHubComponent->RegisterComponent();
	}
	WheelHubComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

	if (!IsValid(WheelMeshComponent) && WheelHubComponent)
	{
		WheelMeshComponent = NewObject<UStaticMeshComponent>(this);
		GetOwner()->AddInstanceComponent(WheelMeshComponent);
		WheelMeshComponent->RegisterComponent();
	}
	WheelMeshComponent->AttachToComponent(WheelHubComponent, FAttachmentTransformRules::KeepRelativeTransform);

	if (!IsValid(BrakeMeshComponent) && WheelHubComponent)
	{
		BrakeMeshComponent = NewObject<UStaticMeshComponent>(this);
		GetOwner()->AddInstanceComponent(BrakeMeshComponent);
		BrakeMeshComponent->RegisterComponent();
	}
	BrakeMeshComponent->AttachToComponent(WheelHubComponent, FAttachmentTransformRules::KeepRelativeTransform);

	return RefreshWheelMesh();
}

// Called every frame
void UVehicleWheelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	//check if staticloadmass should be calculated again
	FVector NewRelativeLocation = GetRelativeLocation();
	if ((CachedComponentRelativeLocation - NewRelativeLocation).SquaredLength() > 1.f)
	{
		WheelCoordinator->NotifyWheelMoved();
	}

	Suspension.CheckIsDampingDirty();

	if (bUpdateAnimAutomatically) UpdateWheelAnim(DeltaTime, 0);
}

void UVehicleWheelComponent::SetSprungMass(float NewSprungMass)
{
	Suspension.SetSprungMass(NewSprungMass);
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
}

void UVehicleWheelComponent::GetWheelCoordinator(UVehicleWheelCoordinatorComponent*& OutWheelCoordinator)
{
	OutWheelCoordinator = WheelCoordinator;
}

float UVehicleWheelComponent::ComputeFeedBackTorque()
{
	FVector LeverArm = Suspension.SimData.HitStruct.ImpactPoint - Suspension.SimData.HitStruct.Location;
	FVector Torque = FVector::CrossProduct(LeverArm, Wheel.SimData.TyreForce);
	return FVector::DotProduct(Suspension.SimData.StrutDirection, Torque);
}

bool UVehicleWheelComponent::GetRayCastResult(FHitResult& OutHitResult, bool& OutRevised)
{
	OutHitResult = Suspension.SimData.HitStruct;
	OutRevised = Suspension.SimData.bRayCastRevised;
	return Suspension.SimData.bHitGround;
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

bool UVehicleWheelComponent::SetMesh(float SteeringAxleOffset,
	UStaticMesh* NewWheelMesh, FTransform WheelMeshRelatvieTransform,
	UStaticMesh* NewBrakeMesh, FTransform BrakeMeshRelativeTransform)
{
	if (!IsValid(WheelHubComponent) || !IsValid(WheelMeshComponent) || !IsValid(BrakeMeshComponent))return false;

	//if is left wheel
	if (GetRelativeLocation().Y < 0)
	{
		FTransform TempTrans = FTransform(FQuat(FRotator(0.f)), FVector(0.f), FVector(1.f, -1.f, 1.f));
		WheelMeshRelatvieTransform *= TempTrans;
		BrakeMeshRelativeTransform *= TempTrans;
	}

	FQuat TempRot = FQuat(FRotator(0, SuspensionKinematicsConfig.BaseToe * Suspension.SimData.WheelPos, SuspensionKinematicsConfig.BaseCamber * Suspension.SimData.WheelPos));
	WheelHubComponent->SetRelativeTransform(FTransform(GetRelativeTransform().InverseTransformRotation(TempRot), FVector(0), FVector(1)));

	if (NewWheelMesh)
	{
		WheelMeshComponent->SetStaticMesh(NewWheelMesh);
	}
	WheelMeshComponent->SetRelativeLocation(WheelMeshRelatvieTransform.GetLocation() + FVector(0, SteeringAxleOffset, 0));
	WheelMeshComponent->SetRelativeRotation(WheelMeshRelatvieTransform.GetRotation());
	WheelMeshComponent->SetRelativeScale3D(WheelMeshRelatvieTransform.GetScale3D());
	WheelMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (NewBrakeMesh)
	{
		BrakeMeshComponent->SetStaticMesh(NewBrakeMesh);
	}
	BrakeMeshComponent->SetRelativeLocation(BrakeMeshRelativeTransform.GetLocation() + FVector(0, SteeringAxleOffset, 0));
	BrakeMeshComponent->SetRelativeRotation(BrakeMeshRelativeTransform.GetRotation());
	BrakeMeshComponent->SetRelativeScale3D(BrakeMeshRelativeTransform.GetScale3D());
	BrakeMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	return WheelMeshComponent->GetStaticMesh() && BrakeMeshComponent->GetStaticMesh();
}

bool UVehicleWheelComponent::RefreshWheelMesh()
{
	return SetMesh(FMath::Sign(GetRelativeLocation().Y) * SuspensionKinematicsConfig.SteeringAxleOffset, WheelMesh, WheelMeshTransform, BrakeMesh, BrakeMeshTransform);
}

void UVehicleWheelComponent::UpdateWheelAnim(float DeltaTime, float MaxAnimAngularVelocity)
{
	if (!IsValid(WheelHubComponent) || !IsValid(WheelMeshComponent))return;

	WheelHubComponent->SetRelativeLocation(Suspension.SuspensionPlaneToZYPlane(Suspension.SimData.BallJointPos2D));
	WheelHubComponent->SetRelativeRotation(Suspension.SimData.RelativeTransform.InverseTransformRotation(Suspension.SimData.WheelRelativeTransform.GetRotation()));
	if (WheelMeshComponent)
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
		FVector2D(Suspension.SimData.BallJointPos2D.X, Suspension.SimData.BallJointPos2D.Y * -PosSign),
		Suspension.SimData.ComponentRelativeForwardVector);
	TempArmRot *= TempInitialRot;
	TempArmRot.Normalize();

	if (IsValid(InArmMesh))
	{
		InArmMesh->SetRelativeRotation(TempArmRot);
	}

	return TempArmRot;
}

FTransform UVehicleWheelComponent::UpdateSuspensionSpringAnim(USceneComponent* InSpringMesh, FVector InScaleAxis, float InOffsetAlongArm, FVector2D InBallJointOffset, FRotator InRotationOffset, float InLengthBias, FVector InInitialScale)
{
	if (!IsValid(InSpringMesh))return FTransform();

	float PosSign = Suspension.SimData.WheelPos;
	InRotationOffset.Yaw *= PosSign;
	InRotationOffset.Roll *= PosSign;

	InBallJointOffset.Y *= PosSign;

	FVector BallJointOffset3D = Suspension.SimData.WheelRelativeTransform.TransformPosition(FVector(0.f, InBallJointOffset.Y, InBallJointOffset.X));
	BallJointOffset3D -= Suspension.SimData.WheelRelativeTransform.GetLocation();

	FVector PivotPos = Suspension.SimData.RelativeTransform.TransformPosition(Suspension.SuspensionPlaneToZYPlane(FVector2D(0.f)));
	FVector ArmDir = (PivotPos - Suspension.SimData.BallJointRelativePos).GetSafeNormal();
	
	FVector OffsetJointPos = Suspension.SimData.BallJointRelativePos + ArmDir * InOffsetAlongArm;
	FVector OffsetInitialJointPos = Suspension.SimData.RelativeTransform.TransformPosition(FVector(0.f, InOffsetAlongArm * -PosSign, 0.f));

	OffsetJointPos += BallJointOffset3D;
	OffsetInitialJointPos += BallJointOffset3D;

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

void UVehicleWheelComponent::AttachWheelHubMeshToSuspension(USceneComponent* InWheelHub, FTransform InTransform)
{
	if (!IsValid(InWheelHub))return;
	InWheelHub->AttachToComponent(WheelHubComponent, FAttachmentTransformRules::KeepRelativeTransform);
	InWheelHub->SetRelativeTransform(InTransform);
}

float UVehicleWheelComponent::SafeDivide(float a, float b)
{
	return (FMath::IsNearlyZero(b)) ? 0.0f : a / b;
}
