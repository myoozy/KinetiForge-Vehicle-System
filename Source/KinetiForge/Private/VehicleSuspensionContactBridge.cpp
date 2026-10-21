// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehicleSuspensionContactBridge.h"
#include "VehicleUtilities.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Physics/PhysicsInterfaceCore.h"

bool FVehicleSuspensionContactBridge::ShouldDoRefinedTrace(
	FVehicleSuspensionSimContext& Ctx,
	const float HalfWheelWidth,
	const FVehicleSuspensionKinematicsConfig& Config)
{
	FVector LocalImpactPoint = Ctx.RayCastWorldTransform.InverseTransformPositionNoScale(Ctx.HitResult.ImpactPoint);

	return FMath::Abs(LocalImpactPoint.Y) > HalfWheelWidth || LocalImpactPoint.Z > Config.Stroke;
}

void FVehicleSuspensionContactBridge::ComputeHitDistance(
	FVehicleSuspensionSimContext& Ctx,
	const float WheelRadius,
	const float EquivalentSphereTraceRadius)
{
	Ctx.HitDistance = Ctx.bHitGround ?
		Ctx.HitResult.Distance + EquivalentSphereTraceRadius : Ctx.RayCastLength + WheelRadius;
}

void FVehicleSuspensionContactBridge::CacheImpactFriction(
	FVehicleSuspensionSimContext& Ctx)
{
	if (Ctx.HitResult.bBlockingHit)
	{
		if (UPhysicalMaterial* HitPhysMat = Ctx.HitResult.PhysMaterial.Get())
		{
			Ctx.ImpactFriction = HitPhysMat->Friction;
			return;
		}
	}
	Ctx.ImpactFriction = 1.f;
}

// returns EquivalentSphereTraceRadius
float FVehicleSuspensionContactBridge::SuspensionLineTrace(
	FVehicleSuspensionSimContext& Ctx,
	const UWorld* World,
	const float WheelRadius,
	const float HalfWheelWidth,
	const FCollisionQueryParams& QueryParams,
	const FCollisionResponseParams& ResponseParams,
	const FVehicleSuspensionKinematicsConfig& Config)
{
	FVector WheelRightVector = Ctx.ChassisWorldTransform.TransformRotation(
		FQuat(Ctx.HubChassisTransform.GetRotation())).GetRightVector();
	FVector WheelOuterSideToCenter = HalfWheelWidth * Ctx.WheelSideSign * WheelRightVector;
	FVector Start = Ctx.RayCastStartWorldLocation + WheelOuterSideToCenter;
	FVector End = Ctx.RayCastEndWorldLocation + WheelOuterSideToCenter;
	FVector LineTraceEnd = End - Ctx.RayCastDirectionWorld * WheelRadius;

	Ctx.bRayCastRefined = false;

	/**************RaycastSingle**************/
	Ctx.bHitGround = FPhysicsInterface::RaycastSingle(World, Ctx.HitResult,
		Start, LineTraceEnd, Config.TraceChannel, QueryParams, ResponseParams,
		FCollisionObjectQueryParams::DefaultObjectQueryParam);

	float EquivalentSphereTraceRadius = 0.f;
	return EquivalentSphereTraceRadius;
}

// returns EquivalentSphereTraceRadius
float FVehicleSuspensionContactBridge::SuspensionSphereTrace(
	FVehicleSuspensionSimContext& Ctx,
	const UWorld* World,
	const float WheelRadius,
	const float HalfWheelWidth,
	const FCollisionQueryParams& QueryParams,
	const FCollisionResponseParams& ResponseParams,
	const FVehicleSuspensionKinematicsConfig& Config)
{
	Ctx.bRayCastRefined = false;
	Ctx.bHitGround = FPhysicsInterface::GeomSweepSingle(
		World, FCollisionShape::MakeSphere(WheelRadius), FQuat::Identity, Ctx.HitResult,
		Ctx.RayCastStartWorldLocation, Ctx.RayCastEndWorldLocation, Config.TraceChannel, QueryParams,
		ResponseParams, FCollisionObjectQueryParams::DefaultObjectQueryParam);

	if (Ctx.bHitGround)
	{
		Ctx.bRayCastRefined = ShouldDoRefinedTrace(Ctx, HalfWheelWidth, Config);

		if (Ctx.bRayCastRefined)
		{
			FVector SecondStepRayCastStart = Ctx.RayCastStartWorldLocation + Ctx.RayCastDirectionWorld * (HalfWheelWidth - WheelRadius);
			FVector SecondStepRayCastEnd = SecondStepRayCastStart - Ctx.RayCastDirectionWorld * Ctx.RayCastLength;

			Ctx.bHitGround = FPhysicsInterface::GeomSweepSingle(
				World, FCollisionShape::MakeSphere(HalfWheelWidth), FQuat::Identity, Ctx.HitResult,
				SecondStepRayCastStart, SecondStepRayCastEnd, Config.TraceChannel, QueryParams,
				ResponseParams, FCollisionObjectQueryParams::DefaultObjectQueryParam);
		}
	}

	return WheelRadius;
}

// returns EquivalentSphereTraceRadius
float FVehicleSuspensionContactBridge::SuspensionBoxTrace(
	FVehicleSuspensionSimContext& Ctx,
	const UWorld* World,
	const float WheelRadius,
	const float HalfWheelWidth,
	const FCollisionQueryParams& QueryParams,
	const FCollisionResponseParams& ResponseParams,
	const FVehicleSuspensionKinematicsConfig& Config)
{
	Ctx.bRayCastRefined = false;

	FVector HalfSize = FVector(WheelRadius, HalfWheelWidth, WheelRadius) * 0.707;
	FQuat Orientation = Ctx.RayCastWorldTransform.TransformRotation(
		FQuat(0.0f, 0.38268343f, 0.0f, 0.92387953f));	// this is Rotator(0.0, 45.0, 0.0)

	/*****GeomSweepSingle*****/
	Ctx.bHitGround = FPhysicsInterface::GeomSweepSingle(
		World,
		FCollisionShape::MakeBox(HalfSize),
		Orientation,
		Ctx.HitResult,
		Ctx.RayCastStartWorldLocation,
		Ctx.RayCastEndWorldLocation,
		Config.TraceChannel,
		QueryParams,
		ResponseParams,
		FCollisionObjectQueryParams::DefaultObjectQueryParam);

	if (Ctx.bHitGround)
	{
		// try to maintain the impact point in the middle of the shape so that it will be more stable
		FVector NormalVec = Ctx.ChassisWorldTransform.GetRotation().GetRightVector();
		FVector RelativeBias = Ctx.HitResult.ImpactPoint - Ctx.HitResult.Location;
		FVector BiasProjection = FVector::VectorPlaneProject(RelativeBias, NormalVec);
		Ctx.HitResult.ImpactPoint = Ctx.HitResult.Location + BiasProjection;
	}

	return WheelRadius;
}

// returns EquivalentSphereTraceRadius
float FVehicleSuspensionContactBridge::SuspensionSphereTraceNoRefinement(
	FVehicleSuspensionSimContext& Ctx,
	const UWorld* World,
	const float WheelRadius,
	const float HalfWheelWidth,
	const FCollisionQueryParams& QueryParams,
	const FCollisionResponseParams& ResponseParams,
	const FVehicleSuspensionKinematicsConfig& Config)
{
	Ctx.bRayCastRefined = false;

	float ValidRadius = FMath::Min(WheelRadius, HalfWheelWidth);
	float RadiusDifference = WheelRadius - ValidRadius;
	FVector TraceEnd = Ctx.RayCastEndWorldLocation - Ctx.RayCastDirectionWorld * RadiusDifference;

	Ctx.bHitGround = FPhysicsInterface::GeomSweepSingle(
		World, FCollisionShape::MakeSphere(ValidRadius), FQuat::Identity, Ctx.HitResult,
		Ctx.RayCastStartWorldLocation, TraceEnd, Config.TraceChannel, QueryParams,
		ResponseParams, FCollisionObjectQueryParams::DefaultObjectQueryParam);

	return ValidRadius;
}

// returns EquivalentSphereTraceRadius
float FVehicleSuspensionContactBridge::SuspensionMultiSphereTrace(
	FVehicleSuspensionSimContext& Ctx,
	const UWorld* World,
	const float WheelRadius,
	const float HalfWheelWidth,
	const FCollisionQueryParams& QueryParams,
	const FCollisionResponseParams& ResponseParams,
	const FVehicleSuspensionKinematicsConfig& Config)
{
	Ctx.bRayCastRefined = false;
	Ctx.bHitGround = FPhysicsInterface::GeomSweepSingle(
		World, FCollisionShape::MakeSphere(WheelRadius), FQuat::Identity, Ctx.HitResult,
		Ctx.RayCastStartWorldLocation, Ctx.RayCastEndWorldLocation, Config.TraceChannel, QueryParams,
		ResponseParams, FCollisionObjectQueryParams::DefaultObjectQueryParam);

	if (Ctx.bHitGround)
	{
		Ctx.bRayCastRefined = ShouldDoRefinedTrace(Ctx, HalfWheelWidth, Config);

		if (Ctx.bRayCastRefined)
		{
			// first test with a bigger box
			FVector HalfSize = FVector(WheelRadius, HalfWheelWidth, WheelRadius);
			FHitResult BoxTraceResult;
			bool bShouldMultiSphereTrace = FPhysicsInterface::GeomSweepSingle(
				World,
				FCollisionShape::MakeBox(HalfSize),
				Ctx.RayCastWorldTransform.GetRotation(),
				BoxTraceResult,
				Ctx.RayCastStartWorldLocation,
				Ctx.RayCastEndWorldLocation,
				Config.TraceChannel,
				QueryParams,
				ResponseParams,
				FCollisionObjectQueryParams::DefaultObjectQueryParam);

			// then if there is someting inside the box, we do multi-sphere trace
			if (bShouldMultiSphereTrace)
			{
				TArray<FHitResult> MultiHitResults;
				Ctx.bHitGround = FPhysicsInterface::GeomSweepMulti(
					World,
					FCollisionShape::MakeSphere(WheelRadius),
					FQuat::Identity,
					MultiHitResults,
					Ctx.RayCastStartWorldLocation,
					Ctx.RayCastEndWorldLocation,
					Config.TraceChannel,
					QueryParams,
					ResponseParams,
					FCollisionObjectQueryParams::DefaultObjectQueryParam
				);

				for (FHitResult HitResult : MultiHitResults)
				{
					if (HitResult.Time > BoxTraceResult.Time)
					{
						Ctx.HitResult = HitResult;
						break;
					}
					else if (HitResult.Time == BoxTraceResult.Time)
					{
						Ctx.HitResult = BoxTraceResult;
						break;
					}
				}
			}
		}
	}

	return WheelRadius;
}

void FVehicleSuspensionContactBridge::Query(
	FVehicleSuspensionSimContext& Ctx,
	const UWorld* World,
	const float WheelRadius,
	const float HalfWheelWidth,
	const FCollisionQueryParams& QueryParams,
	const FCollisionResponseParams& ResponseParams,
	const FVehicleSuspensionKinematicsConfig& Config)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_Wheel_SuspensionSolver_RayCast);

	// savety check
	if (Ctx.RayCastStartWorldLocation.ContainsNaN()
		|| Ctx.RayCastEndWorldLocation.ContainsNaN()
		|| !IsValid(World))
	{
		// reset the suspension
		Ctx.bHitGround = false;
		ComputeHitDistance(Ctx, WheelRadius, 0.f);
		CacheImpactFriction(Ctx);
		return;
	}

	float EquivalentSphereTraceRadius = WheelRadius;
	switch (Config.RayCastMode)
	{
	default:
	case EVehicleSuspensionRayCastMode::LineTrace:
		EquivalentSphereTraceRadius =
			SuspensionLineTrace(Ctx, World, WheelRadius, HalfWheelWidth, QueryParams, ResponseParams, Config);
		break;
	case EVehicleSuspensionRayCastMode::SphereTrace:
		EquivalentSphereTraceRadius =
			SuspensionSphereTrace(Ctx, World, WheelRadius, HalfWheelWidth, QueryParams, ResponseParams, Config);
		break;
	case EVehicleSuspensionRayCastMode::BoxTrace:
		EquivalentSphereTraceRadius =
			SuspensionBoxTrace(Ctx, World, WheelRadius, HalfWheelWidth, QueryParams, ResponseParams, Config);
		break;
	case EVehicleSuspensionRayCastMode::SphereTraceNoRefinement:
		EquivalentSphereTraceRadius =
			SuspensionSphereTraceNoRefinement(Ctx, World, WheelRadius, HalfWheelWidth, QueryParams, ResponseParams, Config);
		break;
	case EVehicleSuspensionRayCastMode::MultiSphereTrace:
		EquivalentSphereTraceRadius =
			SuspensionMultiSphereTrace(Ctx, World, WheelRadius, HalfWheelWidth, QueryParams, ResponseParams, Config);
		break;
	}

	CacheImpactFriction(Ctx);
	ComputeHitDistance(Ctx, WheelRadius, EquivalentSphereTraceRadius);

	if (Ctx.HitDistance < WheelRadius)
	{
		// if bottom out, do line trace
		EquivalentSphereTraceRadius =
			SuspensionLineTrace(Ctx, World, WheelRadius, HalfWheelWidth, QueryParams, ResponseParams, Config);
		CacheImpactFriction(Ctx);
		ComputeHitDistance(Ctx, WheelRadius, EquivalentSphereTraceRadius);
	}
}

void FVehicleSuspensionContactBridge::CalculateImpactPointWorldVelocity(
	FVehicleSuspensionSimContext& Ctx,
	const FVehicleChassisSimState& ChassisState)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_Wheel_SuspensionSolver_GetVelocity);

	FVector3f LastImpactWorldVelocity = Ctx.ImpactWorldVelocity;

	if (Ctx.HitResult.bBlockingHit)
	{
		const FVector Arm = Ctx.HitResult.ImpactPoint - ChassisState.CoMWorldLocation;
		FVector LinVelWorldA = ChassisState.LinearVelocity - FVector::CrossProduct(Arm, ChassisState.AngularVelocity);

		FVector LinVelWorldB = FVector(0.f);
		if (UPrimitiveComponent* HitComponent = Ctx.HitResult.GetComponent())
		{
			if (HitComponent->IsPhysicsStateCreated() &&
				HitComponent->Mobility != EComponentMobility::Static)
			{
				if (Chaos::FRigidBodyHandle_Internal* HitHandle =
					UVehicleUtilities::GetInternalHandle(HitComponent, Ctx.HitResult.BoneName))
				{
					if (ensure(HitHandle->CanTreatAsKinematic()))
					{
						const bool bIsRigid = HitHandle->CanTreatAsRigid();
						const Chaos::FVec3 COM = bIsRigid
							? Chaos::FParticleUtilitiesGT::GetCoMWorldPosition(HitHandle)
							: static_cast<Chaos::FVec3>(
								Chaos::FParticleUtilitiesGT::GetActorWorldTransform(HitHandle).GetTranslation());
						const Chaos::FVec3 Diff = Ctx.HitResult.ImpactPoint - COM;
						LinVelWorldB = HitHandle->V() - Chaos::FVec3::CrossProduct(Diff, HitHandle->W());
					}
				}
			}
		}

		Ctx.ImpactWorldVelocity = FVector3f(0.01 * (LinVelWorldA - LinVelWorldB));
	}
	else
	{
		Ctx.ImpactWorldVelocity = FVector3f(0.f);
	}
}

void FVehicleSuspensionContactBridge::ApplyContactReactionImpulse(
	const FVehicleSuspensionHitResult& Contact,
	const FVector& ImpactWorldLocation,
	const FVector& ChassisImpulse)
{
	// also add force to the contacted component
	if (UPrimitiveComponent* HitComponent = Contact.Component.Get())
	{
		const FName& BoneName = Contact.BoneName;
		if (HitComponent->IsPhysicsStateCreated() &&
			HitComponent->IsSimulatingPhysics(BoneName))
		{
			if (Chaos::FRigidBodyHandle_Internal* HitHandle =
				UVehicleUtilities::GetInternalHandle(HitComponent, BoneName))
			{
				const Chaos::FVec3 WorldCOM = Chaos::FParticleUtilitiesGT::GetCoMWorldPosition(HitHandle);
				const Chaos::FVec3 ContactImpulse = -ChassisImpulse;
				const Chaos::FVec3 AngularImpulse =
					Chaos::FVec3::CrossProduct(ImpactWorldLocation - WorldCOM, ContactImpulse);
				HitHandle->SetLinearImpulse(HitHandle->LinearImpulse() + ContactImpulse, false);
				HitHandle->SetAngularImpulse(HitHandle->AngularImpulse() + AngularImpulse, false);
			}
		}
	}
}
