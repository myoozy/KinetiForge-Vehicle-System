// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#include "VehicleUtil.h"

UVehicleUtil::UVehicleUtil()
{
}

UVehicleUtil::~UVehicleUtil()
{
}

void UVehicleUtil::SetInertiaTensor(UPrimitiveComponent* InComponent, FVector InInertia)
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

void UVehicleUtil::RotateCamera(USceneComponent* InSpringArm, FVector2D InMouseInput, bool bInvertYAxis, float InMaxPitch)
{
	if (IsValid(InSpringArm))
	{
		FRotator Rot = InSpringArm->GetRelativeRotation();
		FRotator NewRot;

		NewRot.Roll = 0;
		NewRot.Pitch = FMath::Clamp(Rot.Pitch + InMouseInput.Y, -InMaxPitch, InMaxPitch);
		NewRot.Yaw = Rot.Yaw + InMouseInput.X;

		InSpringArm->SetRelativeRotation(NewRot);
	}
}

void UVehicleUtil::CameraLookAtVelocity(UPrimitiveComponent* InPrimitiveComponent, USceneComponent* InSpringArm, float InPitch, float InSensitivity, float InInterpSpeed, float InStartSpeed_mps)
{
	if (IsValid(InSpringArm) && IsValid(InPrimitiveComponent))
	{
		FRotator CurrentCamRot = InSpringArm->GetRelativeRotation();

		FVector WorldVelocity = InPrimitiveComponent->GetPhysicsLinearVelocity();
		FVector LocalVelocity = InPrimitiveComponent->GetComponentTransform().InverseTransformVector(WorldVelocity);

		FVector ScaledLinearVelocity = LocalVelocity;
		ScaledLinearVelocity.Y *= InSensitivity;

		FRotator TargetRotator = FRotationMatrix::MakeFromX(ScaledLinearVelocity).Rotator();
		TargetRotator.Pitch = InPitch;

		FRotator NewRot = FMath::RInterpTo(
			CurrentCamRot, 
			TargetRotator, 
			InPrimitiveComponent->GetWorld()->DeltaTimeSeconds, 
			InInterpSpeed
		);
		NewRot.Roll = 0;

		InSpringArm->SetRelativeRotation(NewRot);
	}
}
