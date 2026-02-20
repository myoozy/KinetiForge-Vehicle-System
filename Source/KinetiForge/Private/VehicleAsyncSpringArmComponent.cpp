// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)


#include "VehicleAsyncSpringArmComponent.h"
#include "PhysicsEngine/PhysicsSettings.h"
#include "VehicleWheelCoordinatorComponent.h"
#include "VehicleUtil.h"
#include "VehicleAsyncTickComponent.h"

void UVehicleAsyncSpringArmComponent::UpdatePhysics(float DeltaTime)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(AsyncSpringArmComponent_PhysicsThreadUpdate);

	// update cache
	LastPhysicsDeltaTime = DeltaTime;
	TimeSinceLastPhysTick = 0.f;
	PrevDesiredTransformToPhysParent = DesiredTransformToPhysParent;

	// get rigid handle to get world com position
	UPrimitiveComponent* p = PhysicalParent.Get();
	if (!p)return;
	Chaos::FRigidBodyHandle_Internal* RigidHandle = UVehicleUtil::GetInternalHandle(p);
	if (!RigidHandle)return;

	// get relative transform to primitive component
	FTransform RelativeToPhysicalParent = ArmOriginToPhysParent;
	if (!RelativeToPhysicalParent.IsRotationNormalized())RelativeToPhysicalParent.NormalizeRotation();

	// get world transform of primitive component
	FTransform PhysParentWorldTransform = Chaos::FParticleUtilitiesGT::GetActorWorldTransform(RigidHandle);

	// get component world transform
	FTransform TargetWorldTransform = RelativeToPhysicalParent * PhysParentWorldTransform;

	// get current world transform
	FVector CurrLocation = AsyncDesiredArmOriginWorldTransform.GetLocation();
	FQuat CurrRotation = AsyncDesiredArmOriginWorldTransform.GetRotation();
	if (!CurrRotation.IsNormalized())CurrRotation.Normalize();

	// interp location
	if (bEnableCameraLag)
	{
		CurrLocation = FMath::VInterpTo(CurrLocation, TargetWorldTransform.GetLocation(), DeltaTime, CameraLagSpeed);
	}
	else
	{
		CurrLocation = TargetWorldTransform.GetLocation();
	}
	AsyncDesiredArmOriginWorldTransform.SetLocation(CurrLocation);

	// interp rotation
	if (bEnableCameraRotationLag)
	{
		CurrRotation = FMath::QInterpTo(CurrRotation, TargetWorldTransform.GetRotation(), DeltaTime, CameraRotationLagSpeed);
	}
	else
	{
		CurrRotation = TargetWorldTransform.GetRotation();
	}
	AsyncDesiredArmOriginWorldTransform.SetRotation(CurrRotation);

	// get relative transform
	FTransform SmoothRelativeToPhysicalParent = AsyncDesiredArmOriginWorldTransform.GetRelativeTransform(PhysParentWorldTransform);
	
	DesiredTransformToPhysParent = SmoothRelativeToPhysicalParent;

	// if we want camera to follow the direction of velocity
	if (bLookAtVelocity)
	{
		FVector ParentVelocity = FVector(0.);
		FVector Point = TargetWorldTransform.GetLocation();

		if (ensure(RigidHandle->CanTreatAsKinematic()))
		{
			const bool bIsRigid = RigidHandle->CanTreatAsRigid();
			const Chaos::FVec3 COM = bIsRigid
				? Chaos::FParticleUtilitiesGT::GetCoMWorldPosition(RigidHandle)
				: static_cast<Chaos::FVec3>(
					Chaos::FParticleUtilitiesGT::GetActorWorldTransform(RigidHandle).
					GetTranslation());
			const Chaos::FVec3 Diff = Point - COM;
			ParentVelocity = RigidHandle->V() - Chaos::FVec3::CrossProduct(Diff, RigidHandle->W());
		}

		// find velocity in local space
		FVector LocalParentVelocity = PhysParentWorldTransform.InverseTransformVectorNoScale(ParentVelocity);

		// get rotation
		FQuat TargetRot = FRotationMatrix::MakeFromX(LocalParentVelocity).ToQuat();

		// lerp
		float Alpha = FMath::GetMappedRangeValueClamped(
			MinVelocityToTrace,
			FVector2f(0.f, 1.f),
			LocalParentVelocity.Length()
		);
		DynArmRotOffset = FMath::Lerp(DynArmRotOffset, TargetRot, Alpha);
	}
	else
	{
		DynArmRotOffset = FQuat::Identity;
	}
}

void UVehicleAsyncSpringArmComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	// initialize world transform
	AsyncDesiredArmOriginWorldTransform = GetComponentTransform();
	AsyncDesiredArmOriginWorldTransform.SetLocation(
		AsyncDesiredArmOriginWorldTransform.GetLocation() + TargetOffset
	);

	// find physical parent
	PhysicalParent = UVehicleUtil::FindPhysicalParent(this);

	// find async tick component
	AActor* Owner = GetOwner();
	if (Owner)AsyncTickComponent = UVehicleAsyncTickComponent::FindVehicleAsyncTickComponent(Owner);
	if (UVehicleAsyncTickComponent* p = AsyncTickComponent.Get())p->Register(this);
}

void UVehicleAsyncSpringArmComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	if (UVehicleAsyncTickComponent* p = AsyncTickComponent.Get())p->UnRegister(this);

	//...
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UVehicleAsyncSpringArmComponent::UpdateDesiredArmLocation(
	bool bDoTrace,
	bool bDoLocationLag,
	bool bDoRotationLag,
	float DeltaTime)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(AsyncSpringArmComponent_GameThreadUpdate);

	/*----- code from spring arm class: -----*/
	
	// Get the spring arm 'origin', the target we want to look at
	FVector ArmOrigin = GetComponentLocation() + TargetOffset;
	// We lag the target, not the actual camera position, so rotating the camera around does not have lag
	FVector DesiredLoc = ArmOrigin;

	FRotator DesiredRot = GetTargetRotation();

	// If our viewtarget is simulating using physics, we may need to clamp deltatime
	if (bClampToMaxPhysicsDeltaTime)
	{
		// Use the same max timestep cap as the physics system to avoid camera jitter when the viewtarget simulates less time than the camera
		DeltaTime = FMath::Min(DeltaTime, UPhysicsSettings::Get()->MaxPhysicsDeltaTime);
	}

	// Apply 'lag' to rotation if desired
	if (bDoRotationLag)
	{
		if (bUseCameraLagSubstepping && DeltaTime > CameraLagMaxTimeStep && CameraRotationLagSpeed > 0.f)
		{
			const FRotator ArmRotStep = (DesiredRot - PreviousDesiredRot).GetNormalized() * (1.f / DeltaTime);
			FRotator LerpTarget = PreviousDesiredRot;
			float RemainingTime = DeltaTime;
			while (RemainingTime > UE_KINDA_SMALL_NUMBER)
			{
				const float LerpAmount = FMath::Min(CameraLagMaxTimeStep, RemainingTime);
				LerpTarget += ArmRotStep * LerpAmount;
				RemainingTime -= LerpAmount;

				DesiredRot = FRotator(FMath::QInterpTo(FQuat(PreviousDesiredRot), FQuat(LerpTarget), LerpAmount, CameraRotationLagSpeed));
				PreviousDesiredRot = DesiredRot;
			}
		}
		else
		{
			DesiredRot = FRotator(FMath::QInterpTo(FQuat(PreviousDesiredRot), FQuat(DesiredRot), DeltaTime, CameraRotationLagSpeed));
		}
	}
	PreviousDesiredRot = DesiredRot;

	// ...
	/*----- new code: -----*/
	
	TimeSinceLastPhysTick += DeltaTime;
	float Alpha = FMath::Clamp(UVehicleUtil::SafeDivide(TimeSinceLastPhysTick, LastPhysicsDeltaTime), 0.0f, 1.0f);

	// get the offset and normalize it
	FQuat LocalArmRotOffset = DynArmRotOffset;
	if (!LocalArmRotOffset.IsNormalized())LocalArmRotOffset.Normalize();

	// If the spring arm inherits all Euler angles from its parent component
	// then its rotation relative to the parent primitive component(PhysicalParent) remains constant (without additional input)
	// I refer to this relative rotation as PhysDesiredRot
	FRotator PhysDesiredRot = GetComponentTransform().TransformRotation(LocalArmRotOffset).Rotator();

	// find relative transform to physical parent
	UPrimitiveComponent* PhysParent = PhysicalParent.Get();
	if (PhysParent && (bDoLocationLag || bDoRotationLag))
	{
		// if physical parent is valid
		// and also if do lag

		// update the relative transform to physical parent
		FTransform ParentWorldTransform = PhysParent->GetComponentTransform();
		FTransform SelfWorldTransform = FTransform(PhysDesiredRot, DesiredLoc, GetComponentScale());
		FTransform RelativeToPhysParent = SelfWorldTransform.GetRelativeTransform(ParentWorldTransform);
		if (!RelativeToPhysParent.IsRotationNormalized())RelativeToPhysParent.NormalizeRotation();
		ArmOriginToPhysParent = RelativeToPhysParent;

		// get smoothend world transform
		FTransform SmoothRelativeTransform = FTransform(
			FMath::Lerp(PrevDesiredTransformToPhysParent.GetRotation(), DesiredTransformToPhysParent.GetRotation(), Alpha),
			FMath::Lerp(PrevDesiredTransformToPhysParent.GetLocation(), DesiredTransformToPhysParent.GetLocation(), Alpha),
			FMath::Lerp(PrevDesiredTransformToPhysParent.GetScale3D(), DesiredTransformToPhysParent.GetScale3D(), Alpha)
		);
		if (!SmoothRelativeTransform.IsRotationNormalized())SmoothRelativeTransform.NormalizeRotation();
		FTransform SmoothWorldTransform = SmoothRelativeTransform * ParentWorldTransform;

		// reuse the variable from the original code
		if (bDoLocationLag)
		{
			DesiredLoc = SmoothWorldTransform.GetLocation();
		}

		if (bDoRotationLag)
		{
			PhysDesiredRot = SmoothWorldTransform.GetRotation().Rotator();
		}

		// Determine which Euler angles of DesiredRot should use those from PhysDesiredRot
		// If a component inherits rotation from its parent along a specific axis
		// then that axis should use PhysDesiredRot
		// PhysDesiredRot is smoothed on the physics thread
		// to prevent strange jitter after asynchronous physics is enabled
		if (!IsUsingAbsoluteRotation())
		{
			if (bInheritPitch)
			{
				DesiredRot.Pitch = PhysDesiredRot.Pitch;
			}

			if (bInheritYaw)
			{
				DesiredRot.Yaw = PhysDesiredRot.Yaw;
			}

			if (bInheritRoll)
			{
				DesiredRot.Roll = PhysDesiredRot.Roll;
			}
		}
	}
	else
	{
		// if no physical parent
		// run the logic of original spring arm component
		if (bDoLocationLag)
		{
			if (bUseCameraLagSubstepping && DeltaTime > CameraLagMaxTimeStep && CameraLagSpeed > 0.f)
			{
				const FVector ArmMovementStep = (DesiredLoc - PreviousDesiredLoc) * (1.f / DeltaTime);
				FVector LerpTarget = PreviousDesiredLoc;

				float RemainingTime = DeltaTime;
				while (RemainingTime > UE_KINDA_SMALL_NUMBER)
				{
					const float LerpAmount = FMath::Min(CameraLagMaxTimeStep, RemainingTime);
					LerpTarget += ArmMovementStep * LerpAmount;
					RemainingTime -= LerpAmount;

					DesiredLoc = FMath::VInterpTo(PreviousDesiredLoc, LerpTarget, LerpAmount, CameraLagSpeed);
					PreviousDesiredLoc = DesiredLoc;
				}
			}
			else
			{
				DesiredLoc = FMath::VInterpTo(PreviousDesiredLoc, DesiredLoc, DeltaTime, CameraLagSpeed);
			}
		}
	}

	// save desired rot as a normalized quat
	const FQuat DesiredQuat = DesiredRot.Quaternion();

	// to clamp the lag distance
	if (bDoLocationLag)
	{
		// Clamp distance if requested
		bool bClampedDist = false;
		if (CameraLagMaxDistance > 0.f)
		{
			const FVector FromOrigin = DesiredLoc - ArmOrigin;
			if (FromOrigin.SizeSquared() > FMath::Square(CameraLagMaxDistance))
			{
				DesiredLoc = ArmOrigin + FromOrigin.GetClampedToMaxSize(CameraLagMaxDistance);
				bClampedDist = true;
			}
		}

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		if (bDrawDebugLagMarkers)
		{
			DrawDebugSphere(GetWorld(), ArmOrigin, 5.f, 8, FColor::Green);
			DrawDebugSphere(GetWorld(), DesiredLoc, 5.f, 8, FColor::Yellow);

			const FVector ToOrigin = ArmOrigin - DesiredLoc;
			DrawDebugDirectionalArrow(GetWorld(), DesiredLoc, DesiredLoc + ToOrigin * 0.5f, 7.5f, bClampedDist ? FColor::Red : FColor::Green);
			DrawDebugDirectionalArrow(GetWorld(), DesiredLoc + ToOrigin * 0.5f, ArmOrigin, 7.5f, bClampedDist ? FColor::Red : FColor::Green);
		}
#endif
	}

	// I don't know if this is necessary
	PreviousArmOrigin = ArmOrigin;
	PreviousDesiredLoc = DesiredLoc;

	// Now offset camera position back along our rotation
	DesiredLoc -= DesiredQuat.Vector() * TargetArmLength;
	// Add socket offset in local space
	DesiredLoc += DesiredQuat.RotateVector(SocketOffset);

	// Do a sweep to ensure we are not penetrating the world
	FVector ResultLoc;
	if (bDoTrace && (TargetArmLength != 0.0f))
	{
		bIsCameraFixed = true;
		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(SpringArm), false, GetOwner());

		FHitResult Result;
		GetWorld()->SweepSingleByChannel(Result, ArmOrigin, DesiredLoc, FQuat::Identity, ProbeChannel, FCollisionShape::MakeSphere(ProbeSize), QueryParams);

		UnfixedCameraPosition = DesiredLoc;

		ResultLoc = BlendLocations(DesiredLoc, Result.Location, Result.bBlockingHit, DeltaTime);

		if (ResultLoc == DesiredLoc)
		{
			bIsCameraFixed = false;
		}
	}
	else
	{
		ResultLoc = DesiredLoc;
		bIsCameraFixed = false;
		UnfixedCameraPosition = ResultLoc;
	}

	// Form a transform for new world transform for camera
	FTransform WorldCamTM(DesiredQuat, ResultLoc);
	// Convert to relative to component
	FTransform RelCamTM = WorldCamTM.GetRelativeTransform(GetComponentTransform());

	// Update socket location/rotation
	RelativeSocketLocation = RelCamTM.GetLocation();
	RelativeSocketRotation = RelCamTM.GetRotation();

	UpdateChildTransforms();
}