// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)


#include "VehicleSuspensionSolver.h"
#include "VehicleWheelComponent.h"
#include "AsyncTickFunctions.h"
#include "VehicleUtil.h"

static void PrepareSimulation(
	FVehicleSuspensionSimContext& Ctx,
	const FTransform& ComponentRelativeTransform,
	const FTransform& AsyncCarbodyWorldTransform,
	const FVehicleSuspensionKinematicsConfig& Config)
{
	Ctx.RelativeTransform = (FTransform3f)ComponentRelativeTransform;
	Ctx.CarbodyWorldTransform = AsyncCarbodyWorldTransform;

	//dealing with transforms
	Ctx.WheelPos = FMath::Sign(Ctx.RelativeTransform.GetLocation().Y);
	Ctx.WheelPos += Ctx.WheelPos == 0.f;	//if(wheelpos == 0) wheelpos = 1;
	FVector3f TopMountLocalPos = Config.TopMountPosition;
	TopMountLocalPos.Y *= Ctx.WheelPos;
	Ctx.TopMountPos2D = FVehicleSuspensionSolver::ZYPlaneToSuspensionPlane(TopMountLocalPos, Ctx.WheelPos);
	Ctx.TopMountRelativePos = Ctx.RelativeTransform.TransformPositionNoScale(TopMountLocalPos);
	Ctx.ComponentRelativeForwardVector = Ctx.RelativeTransform.GetRotation().GetForwardVector();
}

static void ComputeRayCastLocation(
	FVehicleSuspensionSimContext& Ctx,
	const FVehicleSuspensionKinematicsConfig& Config)
{
	Ctx.StrutDirection2D = (Ctx.TopMountPos2D - Ctx.KnucklePos2D).GetSafeNormal();
	Ctx.RayCastLength = FMath::Abs(Ctx.StrutDirection2D.X * Config.Stroke);

	Ctx.RayCastStart2D.X = Ctx.TopMountPos2D.X - Ctx.StrutDirection2D.X * Config.MinStrutLength;
	Ctx.RayCastStart2D.Y = Ctx.KnucklePos2D.Y;

	FVector3f RayCastStartLocal = FVehicleSuspensionSolver::SuspensionPlaneToZYPlane(Ctx.RayCastStart2D, Ctx.WheelPos);
	FVector3f RayCastStartRelative = Ctx.RelativeTransform.TransformPositionNoScale(RayCastStartLocal) + Ctx.WheelCenterToKnuckle;
	Ctx.RayCastStartPos = Ctx.CarbodyWorldTransform.TransformPositionNoScale((FVector)RayCastStartRelative);

	FQuat ComponentWorldRot = Ctx.CarbodyWorldTransform.GetRotation() * FQuat(Ctx.RelativeTransform.GetRotation());
	Ctx.ComponentUpVector = ComponentWorldRot.GetUpVector();
	Ctx.RayCastEndPos = Ctx.RayCastStartPos - Ctx.RayCastLength * Ctx.ComponentUpVector;

	FQuat SteeringRot = FQuat(Ctx.ComponentUpVector, FMath::DegreesToRadians(Ctx.SteeringAngle));
	FQuat RayCastRot = FQuat(SteeringRot) * ComponentWorldRot;

	Ctx.RayCastTransform = FTransform(RayCastRot, Ctx.RayCastEndPos, FVector(1, 1, 1));
}

static bool ShouldDoRefinedTrace(
	FVehicleSuspensionSimContext& Ctx,
	const float HalfWheelWidth,
	const FVehicleSuspensionKinematicsConfig& Config)
{
	FVector LocalImpactPoint = Ctx.RayCastTransform.InverseTransformPositionNoScale(Ctx.HitStruct.ImpactPoint);

	return FMath::Abs(LocalImpactPoint.Y) > HalfWheelWidth || LocalImpactPoint.Z > Config.Stroke;
}

static void ComputeHitDistance(
	FVehicleSuspensionSimContext& Ctx,
	const float WheelRadius,
	const float EquivalentSphereTraceRadius)
{
	Ctx.HitDistance = Ctx.bHitGround ?
		Ctx.HitStruct.Distance + EquivalentSphereTraceRadius : Ctx.RayCastLength + WheelRadius;
	float HitDistanceNoBias = FMath::Max(0.f, Ctx.HitDistance - WheelRadius);
	Ctx.SuspensionExtensionRatio = UVehicleUtil::SafeDivide(HitDistanceNoBias, Ctx.RayCastLength);
}

static void CacheImpactFriction(
	FVehicleSuspensionSimContext& Ctx) 
{
	if (Ctx.HitStruct.bBlockingHit)
	{
		if (UPhysicalMaterial* HitPhysMat = Ctx.HitStruct.PhysMaterial.Get())
		{
			Ctx.ImpactFriction = HitPhysMat->Friction;
			return;
		}
	}
	Ctx.ImpactFriction = 1.f;
}

// returns EquivalentSphereTraceRadius
static float SuspensionLineTrace(
	FVehicleSuspensionSimContext& Ctx,
	const UWorld* World,
	const float WheelRadius,
	const float HalfWheelWidth,
	const FCollisionQueryParams& QueryParams,
	const FCollisionResponseParams& ResponseParams,
	const FVehicleSuspensionKinematicsConfig& Config)
{
	FVector WheelRightVector = Ctx.CarbodyWorldTransform.TransformRotation(
		FQuat(Ctx.WheelRelativeTransform.GetRotation())).GetRightVector();
	FVector WheelOuterSideToCenter = HalfWheelWidth * Ctx.WheelPos * WheelRightVector;
	FVector Start = Ctx.RayCastStartPos + WheelOuterSideToCenter;
	FVector End = Ctx.RayCastEndPos + WheelOuterSideToCenter;

	FVector LineTraceEnd = End - Ctx.ComponentUpVector * WheelRadius;

	Ctx.bRayCastRefined = false;

	/**************RaycastSingle**************/
	Ctx.bHitGround = FPhysicsInterface::RaycastSingle(World, Ctx.HitStruct,
		Start, LineTraceEnd, Config.TraceChannel, QueryParams, ResponseParams, 
		FCollisionObjectQueryParams::DefaultObjectQueryParam);

	float EquivalentSphereTraceRadius = 0.f;
	return EquivalentSphereTraceRadius;
}

// returns EquivalentSphereTraceRadius
static float SuspensionSphereTrace(
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
		World, FCollisionShape::MakeSphere(WheelRadius), FQuat::Identity, Ctx.HitStruct,
		Ctx.RayCastStartPos, Ctx.RayCastEndPos, Config.TraceChannel, QueryParams,
		ResponseParams, FCollisionObjectQueryParams::DefaultObjectQueryParam);

	if (Ctx.bHitGround)
	{
		Ctx.bRayCastRefined = ShouldDoRefinedTrace(Ctx, HalfWheelWidth, Config);

		if (Ctx.bRayCastRefined)
		{
			FVector SecondStepRayCastStart = Ctx.RayCastStartPos + Ctx.ComponentUpVector * (HalfWheelWidth - WheelRadius);
			FVector SecondStepRayCastEnd = SecondStepRayCastStart - Ctx.ComponentUpVector * Ctx.RayCastLength;

			Ctx.bHitGround = FPhysicsInterface::GeomSweepSingle(
				World, FCollisionShape::MakeSphere(HalfWheelWidth), FQuat::Identity, Ctx.HitStruct,
				SecondStepRayCastStart, SecondStepRayCastEnd, Config.TraceChannel, QueryParams,
				ResponseParams, FCollisionObjectQueryParams::DefaultObjectQueryParam);
		}
	}

	return WheelRadius;
}

// returns EquivalentSphereTraceRadius
static float SuspensionBoxTrace(
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
	FQuat Orientation = Ctx.RayCastTransform.TransformRotation(
		FQuat(0.0f, 0.38268343f, 0.0f, 0.92387953f));	// this is Rotator(0.0, 45.0, 0.0)

	/*****GeomSweepSingle*****/
	Ctx.bHitGround = FPhysicsInterface::GeomSweepSingle(
		World,
		FCollisionShape::MakeBox(HalfSize),
		Orientation,
		Ctx.HitStruct,
		Ctx.RayCastStartPos,
		Ctx.RayCastEndPos,
		Config.TraceChannel,
		QueryParams,
		ResponseParams,
		FCollisionObjectQueryParams::DefaultObjectQueryParam);

	if (Ctx.bHitGround)
	{
		// try to maintain the impact point in the middle of the shape so that it will be more stable
		FVector NormalVec = Ctx.CarbodyWorldTransform.GetRotation().GetRightVector();
		FVector RelativeBias = Ctx.HitStruct.ImpactPoint - Ctx.HitStruct.Location;
		FVector BiasProjection = FVector::VectorPlaneProject(RelativeBias, NormalVec);
		Ctx.HitStruct.ImpactPoint = Ctx.HitStruct.Location + BiasProjection;
	}

	return WheelRadius;
}

// returns EquivalentSphereTraceRadius
static float SuspensionSphereTraceNoRefinement(
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
	FVector TraceEnd = Ctx.RayCastEndPos - Ctx.ComponentUpVector * RadiusDifference;

	Ctx.bHitGround = FPhysicsInterface::GeomSweepSingle(
		World, FCollisionShape::MakeSphere(ValidRadius), FQuat::Identity, Ctx.HitStruct,
		Ctx.RayCastStartPos, TraceEnd, Config.TraceChannel, QueryParams,
		ResponseParams, FCollisionObjectQueryParams::DefaultObjectQueryParam);

	return ValidRadius;
}

// returns EquivalentSphereTraceRadius
static float SuspensionMultiSphereTrace(
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
		World, FCollisionShape::MakeSphere(WheelRadius), FQuat::Identity, Ctx.HitStruct,
		Ctx.RayCastStartPos, Ctx.RayCastEndPos, Config.TraceChannel, QueryParams,
		ResponseParams, FCollisionObjectQueryParams::DefaultObjectQueryParam);

	if (Ctx.bHitGround)
	{
		Ctx.bRayCastRefined = ShouldDoRefinedTrace(Ctx, HalfWheelWidth, Config);

		if (Ctx.bRayCastRefined)
		{
			// first test with a bigger box
			FVector HalfSize = FVector(WheelRadius, HalfWheelWidth, WheelRadius);
			bool bShouldMultiSphereTrace = FPhysicsInterface::GeomSweepSingle(
				World,
				FCollisionShape::MakeBox(HalfSize),
				Ctx.RayCastTransform.GetRotation(),
				Ctx.HitStruct,
				Ctx.RayCastStartPos,
				Ctx.RayCastEndPos,
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
					Ctx.RayCastStartPos,
					Ctx.RayCastEndPos,
					Config.TraceChannel,
					QueryParams,
					ResponseParams,
					FCollisionObjectQueryParams::DefaultObjectQueryParam
				);

				if (Ctx.bHitGround)
				{
					int32 ArrayLength = MultiHitResults.Num();
					Ctx.HitStruct = MultiHitResults[ArrayLength - 1];
				}
			}
		}
	}

	return WheelRadius;
}

static void SuspensionRayCast(
	FVehicleSuspensionSimContext& Ctx,
	const UWorld* World,
	const float WheelRadius,
	const float HalfWheelWidth,
	const FCollisionQueryParams& QueryParams,
	const FCollisionResponseParams& ResponseParams,
	const FVehicleSuspensionKinematicsConfig& Config)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UpdateVehicleSuspensionRayCast);

	// savety check
	if (Ctx.RayCastStartPos.ContainsNaN()
		|| Ctx.RayCastEndPos.ContainsNaN()
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
	case ESuspensionRayCastMode::LineTrace:
		EquivalentSphereTraceRadius = 
			SuspensionLineTrace(Ctx, World, WheelRadius, HalfWheelWidth, QueryParams, ResponseParams, Config);
		break;
	case ESuspensionRayCastMode::SphereTrace:
		EquivalentSphereTraceRadius =
			SuspensionSphereTrace(Ctx, World, WheelRadius, HalfWheelWidth, QueryParams, ResponseParams, Config);
		break;
	case ESuspensionRayCastMode::BoxTrace:
		EquivalentSphereTraceRadius =
			SuspensionBoxTrace(Ctx, World, WheelRadius, HalfWheelWidth, QueryParams, ResponseParams, Config);
		break;
	case ESuspensionRayCastMode::SphereTraceNoRefinement:
		EquivalentSphereTraceRadius =
			SuspensionSphereTraceNoRefinement(Ctx, World, WheelRadius, HalfWheelWidth, QueryParams, ResponseParams, Config);
		break;
	case ESuspensionRayCastMode::MultiSphereTrace:
		EquivalentSphereTraceRadius =
			SuspensionMultiSphereTrace(Ctx, World, WheelRadius, HalfWheelWidth, QueryParams, ResponseParams, Config);
		break;
	default:
		EquivalentSphereTraceRadius =
			SuspensionLineTrace(Ctx, World, WheelRadius, HalfWheelWidth, QueryParams, ResponseParams, Config);
		break;
	}

	CacheImpactFriction(Ctx);
	ComputeHitDistance(Ctx, WheelRadius, EquivalentSphereTraceRadius);
}

static void ComputeStraightSuspension(
	FVehicleSuspensionSimContext& Ctx,
	const float WheelRadius,
	const FVehicleSuspensionKinematicsConfig& Config)
{
	//Compute the position of the Knuckle on the suspension plane
	Ctx.KnucklePos2D.X = Ctx.RayCastStart2D.X - FMath::Max(0.f, Ctx.HitDistance - WheelRadius);
	Ctx.KnucklePos2D.Y = Config.ArmLength;

	Ctx.KnuckleRelativePos = Ctx.RelativeTransform.TransformPositionNoScale(
		FVehicleSuspensionSolver::SuspensionPlaneToZYPlane(Ctx.KnucklePos2D, Ctx.WheelPos));

	Ctx.StrutRelativeDirection = Ctx.RelativeTransform.GetRotation().GetUpVector();

	// the kingpin(steering axis) is the up vector
	FQuat4f SteeringBiasRotation = FQuat4f(Ctx.StrutRelativeDirection, FMath::DegreesToRadians(Ctx.SteeringAngle));

	FQuat4f SpindleMountRotation = FVehicleSuspensionSolver::GetSpindleMountQuat(
		Config.SpindleMountRotation,
		Ctx.WheelPos
	);
	Ctx.WheelRelativeTransform.SetRotation(SteeringBiasRotation * SpindleMountRotation);
	FVector3f WheelRelativeRightVec = Ctx.WheelRelativeTransform.GetRotation().GetRightVector();

	Ctx.WheelCenterToKnuckle = Config.AxialHubOffset * Ctx.WheelPos * WheelRelativeRightVec;
	Ctx.WheelRelativeTransform.SetLocation(Ctx.KnuckleRelativePos + Ctx.WheelCenterToKnuckle);

	Ctx.WheelRightVector = Ctx.CarbodyWorldTransform.TransformVectorNoScale(FVector(WheelRelativeRightVec));

	Ctx.WheelWorldPos = Ctx.CarbodyWorldTransform.TransformPositionNoScale(FVector(Ctx.WheelRelativeTransform.GetLocation()));
}

static void ComputeMacpherson(
	FVehicleSuspensionSimContext& Ctx,
	const float WheelRadius,
	const FVehicleSuspensionKinematicsConfig& Config)
{
	//Compute the position of the Knuckle on the suspension plane
	Ctx.KnucklePos2D.X = Ctx.RayCastStart2D.X - FMath::Max(0.f, Ctx.HitDistance - WheelRadius);
	Ctx.KnucklePos2D.X = FMath::Clamp(Ctx.KnucklePos2D.X, -Config.ArmLength, Config.ArmLength);
	Ctx.KnucklePos2D.Y = FMath::Sqrt(FMath::Square(Config.ArmLength) - FMath::Square(Ctx.KnucklePos2D.X));

	Ctx.KnuckleRelativePos = Ctx.RelativeTransform.TransformPositionNoScale(
		FVehicleSuspensionSolver::SuspensionPlaneToZYPlane(Ctx.KnucklePos2D, Ctx.WheelPos));

	FVector3f BaseUp = FVector3f(0.f, 0.f, 1.f);
	Ctx.StrutRelativeDirection = (Ctx.TopMountRelativePos - Ctx.KnuckleRelativePos).GetSafeNormal();
	FQuat4f StrutBiasRotation = FQuat4f::FindBetweenNormals(BaseUp, Ctx.StrutRelativeDirection);

	// the kingpin(steering axis) is the strut, for simplification
	FQuat4f SteeringBiasRotation = FQuat4f(Ctx.StrutRelativeDirection, FMath::DegreesToRadians(Ctx.SteeringAngle));

	FQuat4f SpindleMountRotation = FVehicleSuspensionSolver::GetSpindleMountQuat(
		Config.SpindleMountRotation,
		Ctx.WheelPos
	);
	Ctx.WheelRelativeTransform.SetRotation(SteeringBiasRotation * StrutBiasRotation * SpindleMountRotation);
	FVector3f WheelRelativeRightVec = Ctx.WheelRelativeTransform.GetRotation().GetRightVector();

	Ctx.WheelCenterToKnuckle = Config.AxialHubOffset * Ctx.WheelPos * WheelRelativeRightVec;
	Ctx.WheelRelativeTransform.SetLocation(Ctx.KnuckleRelativePos + Ctx.WheelCenterToKnuckle);

	Ctx.WheelRightVector = Ctx.CarbodyWorldTransform.TransformVectorNoScale(FVector(WheelRelativeRightVec));

	Ctx.WheelWorldPos = Ctx.CarbodyWorldTransform.TransformPositionNoScale(FVector(Ctx.WheelRelativeTransform.GetLocation()));
}

static void ComputeDoubleWishbone(
	FVehicleSuspensionSimContext& Ctx,
	const float WheelRadius,
	const FVehicleSuspensionKinematicsConfig& Config,
	const FVehicleSuspensionCachedRichCurves& KineCurves)
{
	//Compute the position of the Knuckle on the suspension plane
	Ctx.KnucklePos2D.X = Ctx.RayCastStart2D.X - FMath::Max(0.f, Ctx.HitDistance - WheelRadius);
	Ctx.KnucklePos2D.X = FMath::Clamp(Ctx.KnucklePos2D.X, -Config.ArmLength, Config.ArmLength);
	Ctx.KnucklePos2D.Y = FMath::Sqrt(FMath::Square(Config.ArmLength) - FMath::Square(Ctx.KnucklePos2D.X));

	Ctx.KnuckleRelativePos = Ctx.RelativeTransform.TransformPositionNoScale(
		FVehicleSuspensionSolver::SuspensionPlaneToZYPlane(Ctx.KnucklePos2D, Ctx.WheelPos));

	Ctx.StrutRelativeDirection = (Ctx.TopMountRelativePos - Ctx.KnuckleRelativePos).GetSafeNormal();

	FVector3f WheelAlignmentEuler =	
		FVehicleSuspensionSolver::GetCamberToeCasterFromCurve(
			KineCurves,
			1.f - Ctx.SuspensionExtensionRatio, 
			Ctx.WheelPos, 
			Config.SpindleMountRotation.Roll,
			Config.SpindleMountRotation.Yaw,
			Config.SpindleMountRotation.Pitch
		);
	FQuat4f BaseWheelRelativeRotation = FQuat4f(FRotator3f(WheelAlignmentEuler.X, WheelAlignmentEuler.Y, WheelAlignmentEuler.Z));

	// the kingpin(steering axis) is the upvector of BaseWheelRelativeRotation
	FQuat4f SteeringBiasRotation = FQuat4f(BaseWheelRelativeRotation.GetUpVector(), FMath::DegreesToRadians(Ctx.SteeringAngle));


	Ctx.WheelRelativeTransform.SetRotation(SteeringBiasRotation * BaseWheelRelativeRotation);
	FVector3f WheelRelativeRightVec = Ctx.WheelRelativeTransform.GetRotation().GetRightVector();

	Ctx.WheelCenterToKnuckle = Config.AxialHubOffset * Ctx.WheelPos * WheelRelativeRightVec;
	Ctx.WheelRelativeTransform.SetLocation(Ctx.KnuckleRelativePos + Ctx.WheelCenterToKnuckle);

	Ctx.WheelRightVector = Ctx.CarbodyWorldTransform.TransformVectorNoScale(FVector(WheelRelativeRightVec));

	Ctx.WheelWorldPos = Ctx.CarbodyWorldTransform.TransformPositionNoScale(FVector(Ctx.WheelRelativeTransform.GetLocation()));
}

static void ComputeSolidAxle(
	FVehicleSuspensionSimContext& Ctx,
	const float WheelRadius,
	const FVehicleSuspensionKinematicsConfig& Config,
	const FVector& InKnuckleWorldPos,
	const FVector& InAxleWorldDirection)
{
	// get relative position of the ball joint
	Ctx.KnuckleRelativePos = (FVector3f)Ctx.CarbodyWorldTransform.InverseTransformPositionNoScale(InKnuckleWorldPos);

	// the relative direction of the axle, which is also the right vector of the wheel
	// because solid axle usually does not have camber/toe
	FVector3f AxleDirection = (FVector3f)Ctx.CarbodyWorldTransform.InverseTransformVectorNoScale(InAxleWorldDirection);
	Ctx.WheelCenterToKnuckle = Config.AxialHubOffset * Ctx.WheelPos * AxleDirection;

	// get relative position of the wheel
	FVector3f WheelRelativePos = Ctx.KnuckleRelativePos + Ctx.WheelCenterToKnuckle;

	// get the relative rotation of the wheel
	FVector3f DefaultRight = FVector3f(0.f, 1.f, 0.f);
	FQuat4f AxleRotation = FQuat4f::FindBetweenNormals(DefaultRight, AxleDirection);
	FVector3f SteeringAxle = FVector3f::CrossProduct(Ctx.ComponentRelativeForwardVector, AxleDirection);
	FQuat4f SteeringBiasRotation = FQuat4f(SteeringAxle, FMath::DegreesToRadians(Ctx.SteeringAngle));

	Ctx.WheelRelativeTransform = FTransform3f(SteeringBiasRotation * AxleRotation, WheelRelativePos);
	FVector3f WheelRelativeRightVec = Ctx.WheelRelativeTransform.GetRotation().GetRightVector();
	Ctx.WheelRightVector = Ctx.CarbodyWorldTransform.TransformVectorNoScale((FVector)WheelRelativeRightVec);

	Ctx.WheelWorldPos = Ctx.CarbodyWorldTransform.TransformPositionNoScale((FVector)Ctx.WheelRelativeTransform.GetLocation());

	// the relative direction of the strut
	Ctx.StrutRelativeDirection = (Ctx.TopMountRelativePos - Ctx.KnuckleRelativePos).GetSafeNormal();

	// have to calculate the 2d position of the ball joint, to decide the raycast start for next frame
	FVector3f KnuckleLocalPos = Ctx.RelativeTransform.InverseTransformPositionNoScale(Ctx.KnuckleRelativePos);
	Ctx.KnucklePos2D = FVehicleSuspensionSolver::ZYPlaneToSuspensionPlane(KnuckleLocalPos, Ctx.WheelPos);
}

static void UpdateAntiPitchRollGeometry(
	FVehicleSuspensionSimContext& Ctx,
	Chaos::FRigidBodyHandle_Internal* CarbodyHandle,
	const FVehicleSuspensionCachedRichCurves& KineCurves,
	const FTransform& AsyncCarbodyWorldTransform,
	const FVector3f& TireForce)
{
	if (!Ctx.bHitGround)
	{
		Ctx.JackingForce = 0.f;
		return;
	}

	float Compression = 1.f - Ctx.SuspensionExtensionRatio;

	// get world com
	Chaos::FVec3 CarbodyWorldCOM = CarbodyHandle != nullptr ?
		Chaos::FParticleUtilitiesGT::GetCoMWorldPosition(CarbodyHandle) : AsyncCarbodyWorldTransform.GetLocation();

	// get arm
	Chaos::FVec3 LeverArmVec = Ctx.HitStruct.ImpactPoint - CarbodyWorldCOM;
	float LeverArmLengthSq = LeverArmVec.SquaredLength();

	// get torque caused by tire force
	Chaos::FVec3 InducedTorqueWorld = Chaos::FVec3::CrossProduct(LeverArmVec, TireForce);

	// transform torque into carbody space
	Chaos::FVec3 InducedTorqueLocal = AsyncCarbodyWorldTransform.GetRotation().UnrotateVector(InducedTorqueWorld);

	// get anti-pitch value
	bool bIsDiving = InducedTorqueLocal.Y > 0.f;
	Ctx.AntiPitchScale = bIsDiving ?
		KineCurves.AntiDiveCurve.Eval(Compression) :
		KineCurves.AntiSquatCurve.Eval(Compression);

	// get anti-roll value
	Ctx.AntiRollScale = KineCurves.AntiRollCurve.Eval(Compression);

	// get counter torque
	Chaos::FVec3 TargetCounterTorqueLocal =
		-Chaos::FVec3(InducedTorqueLocal.X * Ctx.AntiRollScale, InducedTorqueLocal.Y * Ctx.AntiPitchScale, 0.f);

	// get counter torque in world space
	Chaos::FVec3 TargetCounterTorqueWorld = AsyncCarbodyWorldTransform.GetRotation().RotateVector(TargetCounterTorqueLocal);

	// get normal of ground
	FVector ImpactNormal = Ctx.HitStruct.Normal;

	// Torque Axis for Normal Force (1N)
	Chaos::FVec3 NormalTorqueAxis = Chaos::FVec3::CrossProduct(LeverArmVec, ImpactNormal);
	float EffectiveLeverArmSq = NormalTorqueAxis.SizeSquared();

	// get final jacking force
	Ctx.JackingForce = UVehicleUtil::SafeDivide(
		(float)Chaos::FVec3::DotProduct(TargetCounterTorqueWorld, NormalTorqueAxis),
		EffectiveLeverArmSq
	);
}

static void UpdateImpactPointWorldVelocity(
	FVehicleSuspensionSimContext& Ctx,
	Chaos::FRigidBodyHandle_Internal* CarbodyHandle)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UpdateVehicleWheelLinearVelocity);

	if (Ctx.HitStruct.bBlockingHit)
	{
		FVector LinVelWorldA = FVector(0.f);
		if (CarbodyHandle)
		{
			if (ensure(CarbodyHandle->CanTreatAsKinematic()))
			{
				const bool bIsRigid = CarbodyHandle->CanTreatAsRigid();
				const Chaos::FVec3 COM = bIsRigid
					? Chaos::FParticleUtilitiesGT::GetCoMWorldPosition(CarbodyHandle)
					: static_cast<Chaos::FVec3>(
						Chaos::FParticleUtilitiesGT::GetActorWorldTransform(CarbodyHandle).
						GetTranslation());
				const Chaos::FVec3 Diff = Ctx.HitStruct.ImpactPoint - COM;
				LinVelWorldA = CarbodyHandle->V() - Chaos::FVec3::CrossProduct(Diff, CarbodyHandle->W());
			}
		}

		FVector LinVelWorldB = FVector(0.f);

		if (UPrimitiveComponent* HitComponent = Ctx.HitStruct.GetComponent())
		{
			if (HitComponent->IsPhysicsStateCreated() &&
				HitComponent->Mobility != EComponentMobility::Static)
			{
				LinVelWorldB = UAsyncTickFunctions::ATP_GetLinearVelocityAtPoint(
					HitComponent,
					Ctx.HitStruct.ImpactPoint,
					Ctx.HitStruct.BoneName
				);
			}
		}

		Ctx.ImpactPointWorldVelocity = FVector3f(0.01 * (LinVelWorldA - LinVelWorldB));
	}
	else
	{
		Ctx.ImpactPointWorldVelocity = FVector3f(0.f);
	}
}

static float GetCriticalDamping(
	const float SpringStiffness,
	const float SprungMass)
{
	if (SprungMass <= 0.f)
	{
		//UE_LOG(LogTemp, Warning, TEXT("VehicleSuspensionSolver: SprungMass not valid!"));
		return 0.1f * SpringStiffness;
	}
	else
	{
		float NaturalFrequency = FMath::Sqrt(SpringStiffness * 100.f / SprungMass);
		return 0.02f * SprungMass * NaturalFrequency;
	}
}

static void ComputeSuspensionForce(
	FVehicleSuspensionSimContext& Ctx,
	const float WheelRadius,
	const FVehicleSuspensionSpringConfig& SpringConfig,
	const FVehicleSuspensionKinematicsConfig& KineConfig,
	const FVehicleSuspensionCachedRichCurves& KineCurves)
{
	float DeltaTimeInv = UVehicleUtil::SafeDivide(1.f, Ctx.PhysicsDelatTime);

	const float SuspensionStroke = KineConfig.Stroke;

	const float LastLength = Ctx.SuspensionCurrentLength;

	Ctx.SuspensionCurrentLength = SuspensionStroke * Ctx.SuspensionExtensionRatio;
	Ctx.CriticalDamping = GetCriticalDamping(SpringConfig.SpringStiffness, Ctx.SprungMass);

	const float MotionRatio = KineCurves.MotionRatioCurve.Eval(1.f - Ctx.SuspensionExtensionRatio);

	//check suspension preload
	//preload should not be greater than gravity force on the wheel
	Ctx.StrutDirection = Ctx.CarbodyWorldTransform.TransformVectorNoScale((FVector)Ctx.StrutRelativeDirection);
	float Proj = FVector::DotProduct(FVector(0.f, 0.f, 1.f), Ctx.StrutDirection);
	float MaxPreload = UVehicleUtil::SafeDivide(0.99f * Ctx.WorldGravityZ * Ctx.SprungMass, Proj);
	Ctx.ValidPreload = FMath::Min(SpringConfig.SpringPreload * MotionRatio, MaxPreload);

	const float EquivSpringStiffness = SpringConfig.SpringStiffness * MotionRatio * MotionRatio;
	float SpringForce = EquivSpringStiffness * (SuspensionStroke - Ctx.SuspensionCurrentLength);
	SpringForce += Ctx.ValidPreload;

	float DampingRatio = (Ctx.SuspensionCurrentLength > LastLength) ? 
		SpringConfig.ReboundDampingRatio : SpringConfig.CompressionDampingRatio;
	float DamperStiffness = Ctx.CriticalDamping * DampingRatio;
	float EquivDamperStiffness = DamperStiffness * MotionRatio * MotionRatio;
	float DampingForce = EquivDamperStiffness * (LastLength - Ctx.SuspensionCurrentLength) * DeltaTimeInv;

	Ctx.SuspensionForce = SpringForce + DampingForce;

	FVector SwaybarForceVector = Ctx.SwaybarForce * Ctx.ComponentUpVector;
	FVector SuspensionForceVector = Ctx.StrutDirection * Ctx.SuspensionForce + SwaybarForceVector;

	// if suspension is compeletly compressed
	float ForceToHoldCar = 0.f;
	if (Ctx.SuspensionCurrentLength < SMALL_NUMBER)
	{
		// try to stop the car immediately
		float VelocityIntoSurface = FVector::DotProduct(Ctx.HitStruct.Normal, (FVector)Ctx.ImpactPointWorldVelocity);
		float ForceToBringToStop = -VelocityIntoSurface * DeltaTimeInv * Ctx.SprungMass;
		ForceToHoldCar = ForceToBringToStop * SpringConfig.BottomOutStiffness;

		// the spring system in another direction (normal of impact surface)
		FVector WheelCenterToImpactPoint = Ctx.WheelWorldPos - Ctx.HitStruct.ImpactPoint;
		float DistanceToSurface = FVector::DotProduct(Ctx.HitStruct.Normal, WheelCenterToImpactPoint);
		SpringForce = (WheelRadius - DistanceToSurface) * EquivSpringStiffness;
		DampingForce = -VelocityIntoSurface * EquivDamperStiffness;
		ForceToHoldCar += SpringForce + DampingForce;
	}

	float SuspensionForceProj = FVector::DotProduct(SuspensionForceVector, Ctx.HitStruct.Normal);
	Ctx.ForceAlongImpactNormal = SuspensionForceProj + ForceToHoldCar + Ctx.JackingForce;

	if (!Ctx.bHitGround)
	{
		Ctx.ForceAlongImpactNormal = 0.f;
		Ctx.SuspensionForce = 0.f;
	}
}

FVehicleSuspensionSolver::FVehicleSuspensionSolver()
{
}

FVehicleSuspensionSolver::~FVehicleSuspensionSolver()
{
}

bool FVehicleSuspensionSolver::Initialize(UVehicleWheelComponent* WheelComponent)
{
	if (WheelComponent != nullptr)
	{
		UpdateCachedRichCurves(WheelComponent->SuspensionKinematicsConfig);

		QueryParams.AddIgnoredActor(WheelComponent->GetOwner());
		QueryParams.bReturnPhysicalMaterial = true;
		QueryParams.bReturnFaceIndex = false;
		QueryParams.bTraceComplex = false;
		
		float WheelPos = FMath::Sign(WheelComponent->GetRelativeTransform().GetLocation().Y);
		State.bIsRightWheel = WheelPos >= 0.f;
		
		ApplySuspensionStateDirect(
			WheelComponent->WheelConfig.Radius,
			WheelComponent->SuspensionKinematicsConfig,
			WheelComponent->GetRelativeTransform(),
			WheelComponent->GetCarbodyAsyncWorldTransform(),
			1.f,
			0.f
		);

		//initialize the cache, so that damping will not be Computed twice when game starts
		CachedSpringStiffness  = WheelComponent->SuspensionSpringConfig.SpringStiffness;
		
		return true;
	}
	else
	{
		return false;
	}
}

void FVehicleSuspensionSolver::SetSprungMass(
	const FVehicleSuspensionSpringConfig& SpringConfig,
	const float NewSprungMass)
{
	if (NewSprungMass <= 0)
	{
		State.SprungMass = 0;
	}
	else
	{
		State.SprungMass = NewSprungMass;
	}
}

void FVehicleSuspensionSolver::UpdateSuspension(
	const float WheelRadius,
	const float WheelWidth,
	const FVector3f& TireForce,
	const FVehicleSuspensionKinematicsConfig& KineConfig,
	const FVehicleSuspensionSpringConfig& SpringConfig,
	const FTransform& ComponentRelativeTransform,
	const FTransform& AsyncCarbodyWorldTransform,
	const UWorld* CurrentWorld,
	Chaos::FRigidBodyHandle_Internal* CarbodyHandle,
	float InDeltaTime,
	float InSteeringAngle, 
	float InSwaybarForce)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UpdateVehicleSuspensionSolver);

	FVehicleSuspensionSimContext Ctx;

	CopyStateToContext(Ctx);

	Ctx.PhysicsDelatTime = InDeltaTime;
	Ctx.SteeringAngle = InSteeringAngle;
	Ctx.SwaybarForce = InSwaybarForce;

	PrepareSimulation(
		Ctx,
		ComponentRelativeTransform,
		AsyncCarbodyWorldTransform,
		KineConfig
	);

	ComputeRayCastLocation(
		Ctx, 
		KineConfig
	);

	SuspensionRayCast(
		Ctx, 
		CurrentWorld, 
		WheelRadius, 
		WheelWidth * 0.5f, 
		QueryParams, 
		ResponseParams, 
		KineConfig
	);

	switch (KineConfig.SuspensionType)
	{
	case ESuspensionType::StraightLine:
		ComputeStraightSuspension(Ctx, WheelRadius, KineConfig);
		break;
	case ESuspensionType::Macpherson:
		ComputeMacpherson(Ctx, WheelRadius, KineConfig);
		break;
	case ESuspensionType::DoubleWishbone:
		ComputeDoubleWishbone(Ctx, WheelRadius, KineConfig, CachedCurves);
		break;
	default:
		ComputeStraightSuspension(Ctx, WheelRadius, KineConfig);
		break;
	}

	UpdateAntiPitchRollGeometry(
		Ctx,
		CarbodyHandle,
		CachedCurves,
		AsyncCarbodyWorldTransform,
		TireForce
	);

	UpdateImpactPointWorldVelocity(
		Ctx, 
		CarbodyHandle
	);

	ComputeSuspensionForce(
		Ctx, 
		WheelRadius, 
		SpringConfig, 
		KineConfig, 
		CachedCurves
	);

	CopyContextToState(Ctx);
}

void FVehicleSuspensionSolver::StartUpdateSolidAxle(
	const float WheelRadius,
	const float WheelWidth,
	const FVehicleSuspensionKinematicsConfig& KineConfig,
	const FTransform& ComponentRelativeTransform,
	const FTransform& AsyncCarbodyWorldTransform,
	const UWorld* CurrentWorld,
	float InSteeringAngle,
	FVector& OutApporximatedWheelWorldPos,
	FVehicleSuspensionSimContext& Ctx)
{
	CopyStateToContext(Ctx);

	Ctx.SteeringAngle = InSteeringAngle;

	PrepareSimulation(
		Ctx,
		ComponentRelativeTransform,
		AsyncCarbodyWorldTransform,
		KineConfig
	);

	ComputeRayCastLocation(Ctx, KineConfig);
	SuspensionRayCast(Ctx, CurrentWorld, WheelRadius, WheelWidth * 0.5f, QueryParams, ResponseParams, KineConfig);

	float DistanceToRayCastStart = FMath::Max(0.f, Ctx.HitDistance - WheelRadius);
	FVector OffsetToRayCastStart = Ctx.ComponentUpVector * DistanceToRayCastStart;
	OutApporximatedWheelWorldPos = Ctx.RayCastStartPos - OffsetToRayCastStart;

	return;
}

void FVehicleSuspensionSolver::FinalizeUpdateSolidAxle(
	const float WheelRadius,
	const FVehicleSuspensionKinematicsConfig& KineConfig,
	const FVehicleSuspensionSpringConfig& SpringConfig,
	const FTransform& AsyncCarbodyWorldTransform,
	Chaos::FRigidBodyHandle_Internal* CarbodyHandle,
	float InDeltaTime, 
	float InSwaybarForce,
	FVehicleSuspensionSimContext& Ctx,
	const FVector& InKnuckleWorldPos,
	const FVector& InAxleWorldDirection,
	const FVector3f& TireForce)
{
	Ctx.PhysicsDelatTime = InDeltaTime;
	Ctx.SwaybarForce = InSwaybarForce;

	ComputeSolidAxle(
		Ctx,
		WheelRadius,
		KineConfig,
		InKnuckleWorldPos,
		InAxleWorldDirection
	);

	UpdateAntiPitchRollGeometry(
		Ctx,
		CarbodyHandle,
		CachedCurves,
		AsyncCarbodyWorldTransform,
		TireForce
	);

	// update velocity
	UpdateImpactPointWorldVelocity(
		Ctx, 
		CarbodyHandle
	);

	// get suspension force
	ComputeSuspensionForce(
		Ctx, 
		WheelRadius, 
		SpringConfig, 
		KineConfig, 
		CachedCurves
	);

	CopyContextToState(Ctx);
}

void FVehicleSuspensionSolver::ApplySuspensionStateDirect(
	const float WheelRadius,
	const FVehicleSuspensionKinematicsConfig& KineConfig,
	const FTransform& ComponentRelativeTransform,
	const FTransform& AsyncCarbodyWorldTransform, 
	float InExtensionRatio, 
	float InSteeringAngle)
{
	FVehicleSuspensionSimContext Ctx;
	CopyStateToContext(Ctx);
	Ctx.SuspensionExtensionRatio = InExtensionRatio;
	Ctx.SteeringAngle = InSteeringAngle;

	const FVehicleSuspensionCachedRichCurves& CachedKineCurves = CachedCurves;

	PrepareSimulation(
		Ctx,
		ComponentRelativeTransform,
		AsyncCarbodyWorldTransform,
		KineConfig
	);
	ComputeRayCastLocation(Ctx, KineConfig);

	Ctx.HitDistance = InExtensionRatio * Ctx.RayCastLength + WheelRadius;

	switch (KineConfig.SuspensionType)
	{
	case ESuspensionType::StraightLine:
		ComputeStraightSuspension(Ctx, WheelRadius, KineConfig);
		break;
	case ESuspensionType::Macpherson:
		ComputeMacpherson(Ctx, WheelRadius, KineConfig);
		break;
	case ESuspensionType::DoubleWishbone:
		ComputeDoubleWishbone(Ctx, WheelRadius, KineConfig, CachedKineCurves);
		break;
	default:
		ComputeStraightSuspension(Ctx, WheelRadius, KineConfig);
		break;
	}

	CopyContextToState(Ctx);
}

void FVehicleSuspensionSolver::StartApplySolidAxleStateDirect(
	const float WheelRadius,
	const FVehicleSuspensionKinematicsConfig& KineConfig,
	const FTransform& ComponentRelativeTransform,
	const FTransform& AsyncCarbodyWorldTransform,
	float InExtensionRatio, 
	float InSteeringAngle, 
	FVector& OutApporximatedWheelWorldPos, 
	FVehicleSuspensionSimContext& Ctx)
{
	CopyStateToContext(Ctx);
	Ctx.SuspensionExtensionRatio = InExtensionRatio;
	Ctx.SteeringAngle = InSteeringAngle;

	const FVehicleSuspensionCachedRichCurves& CachedKineCurves = CachedCurves;

	PrepareSimulation(
		Ctx, 
		ComponentRelativeTransform, 
		AsyncCarbodyWorldTransform, 
		KineConfig
	);
	ComputeRayCastLocation(Ctx, KineConfig);

	Ctx.HitDistance = InExtensionRatio * Ctx.RayCastLength + WheelRadius;
	float DistanceToRayCastStart = FMath::Max(0.f, Ctx.HitDistance - WheelRadius);
	FVector OffsetToRayCastStart = Ctx.ComponentUpVector * DistanceToRayCastStart;
	OutApporximatedWheelWorldPos = Ctx.RayCastStartPos - OffsetToRayCastStart;
}

void FVehicleSuspensionSolver::FinalizeApplySolidAxleStateDirect(
	const float WheelRadius,
	const FVehicleSuspensionKinematicsConfig& KineConfig, 
	FVehicleSuspensionSimContext& Ctx, 
	const FVector& InKnuckleWorldPos, 
	const FVector& InAxleWorldDirection)
{
	ComputeSolidAxle(
		Ctx,
		WheelRadius,
		KineConfig,
		InKnuckleWorldPos,
		InAxleWorldDirection
	);
	CopyContextToState(Ctx);
}

void FVehicleSuspensionSolver::DrawSuspension(
	UVehicleWheelComponent* WheelComponent, 
	float Duration, 
	float Thickness, 
	bool bDrawSuspension, 
	bool bDrawWheel, 
	bool bDrawRayCast)
{
	UWorld* TempWorld = WheelComponent->GetWorld();
	if (!IsValid(TempWorld))return;

	const FTransform& CarbodyWorldTrans = WheelComponent->GetCarbodyAsyncWorldTransform();

	FVector WheelRelativePos = FVector(State.KnuckleRelativePos + State.WheelCenterToKnuckle);
	FVector WheelWorldPos = CarbodyWorldTrans.TransformPositionNoScale(WheelRelativePos);
	
	float WheelPos = State.bIsRightWheel ? 1.f : -1.f;

	if (bDrawSuspension)
	{
		FVector TempKnuckleWorldPos = CarbodyWorldTrans.TransformPositionNoScale((FVector)State.KnuckleRelativePos);
		//draw arm
		FTransform WorldTrans = WheelComponent->GetRelativeTransform() * CarbodyWorldTrans;
		FVector PivotPos = WorldTrans.TransformPositionNoScale((FVector)SuspensionPlaneToZYPlane(FVector2f(0.f), WheelPos));
		DrawDebugLine(TempWorld, PivotPos, TempKnuckleWorldPos, FColor(0, 0, 255), false, Duration, 0, Thickness);
		//draw strut
		DrawDebugLine(TempWorld, CarbodyWorldTrans.TransformPositionNoScale(FVector(State.TopMountRelativePos)), TempKnuckleWorldPos, FColor(255, 255, 0), false, Duration, 0, Thickness);
		//draw wheel offset
		DrawDebugLine(TempWorld, TempKnuckleWorldPos, WheelWorldPos, FColor(0, 255, 255), false, Duration, 0, Thickness);
	}

	if (bDrawWheel)
	{
		//draw cylinder
		FVector W = FVector(State.WheelRightVector) * WheelComponent->WheelConfig.Width * 0.5;
		DrawDebugCylinder(TempWorld, WheelWorldPos + W, WheelWorldPos - W,
			WheelComponent->WheelConfig.Radius, 8, FColor(128, 128, 128), false, Duration, 0, Thickness);
	}

	if (bDrawRayCast)
	{
		float TempR = WheelComponent->WheelConfig.Radius;
		float TempHalfWidth = WheelComponent->WheelConfig.Width * 0.5;
		float ValidR = FMath::Min(TempR, TempHalfWidth);
		FVector HalfSize = FVector(TempR, TempHalfWidth, TempR) * 0.707;
		FQuat Orientation = FQuat(RayCastResult.TraceRot *
			FQuat4f(0.0f, 0.38268343f, 0.0f, 0.92387953f));	// this is Rotator(0.0, 45.0, 0.0)
		float SphereTraceR = (State.ImpactPoint - RayCastResult.Location).Length();
		float RayCastLength = (RayCastResult.TraceStart - RayCastResult.TraceEnd).Length();
		switch (WheelComponent->SuspensionKinematicsConfig.RayCastMode)
		{
		case ESuspensionRayCastMode::LineTrace:
			DrawDebugLine(TempWorld, RayCastResult.TraceStart, RayCastResult.TraceEnd, FColor(0, 255, 0), false, Duration, 0, Thickness);
			if (RayCastResult.bBlockingHit)DrawDebugPoint(TempWorld, RayCastResult.Location, Thickness, FColor(255, 0, 0), false, Duration, Thickness);
			break;
		case ESuspensionRayCastMode::SphereTrace:
			//draw capsule
			DrawDebugCapsule(TempWorld, (RayCastResult.TraceStart + RayCastResult.TraceEnd) * 0.5,
				RayCastLength * 0.5 + SphereTraceR, SphereTraceR, (FQuat)RayCastResult.TraceRot.GetNormalized(), FColor(0, 255, 0), false, Duration, 0, Thickness);
			if (RayCastResult.bBlockingHit)DrawDebugSphere(TempWorld, RayCastResult.Location, SphereTraceR, 8, FColor(255, 0, 0), false, Duration, 0, Thickness);
			break;
		case ESuspensionRayCastMode::BoxTrace:
			if (RayCastResult.bBlockingHit)DrawDebugBox(TempWorld, RayCastResult.Location, HalfSize, Orientation, FColor(0, 255, 0), false, Duration, 0, Thickness);
			break;
		case ESuspensionRayCastMode::SphereTraceNoRefinement:
			DrawDebugCapsule(TempWorld, (RayCastResult.TraceStart + RayCastResult.TraceEnd) * 0.5,
				RayCastLength * 0.5 + ValidR,
				ValidR, (FQuat)RayCastResult.TraceRot.GetNormalized(), FColor(0, 255, 0), false, Duration, 0, Thickness);
			if (RayCastResult.bBlockingHit)DrawDebugSphere(TempWorld, RayCastResult.Location, ValidR, 8, FColor(255, 0, 0), false, Duration, 0, Thickness);
			break;
		case ESuspensionRayCastMode::MultiSphereTrace:
			DrawDebugCapsule(TempWorld, (RayCastResult.TraceStart + RayCastResult.TraceEnd) * 0.5,
				RayCastLength * 0.5 + TempR, TempR, (FQuat)RayCastResult.TraceRot.GetNormalized(), FColor(0, 255, 0), false, Duration, 0, Thickness);
			if (RayCastResult.bBlockingHit)DrawDebugSphere(TempWorld, RayCastResult.Location, TempR, 8, FColor(255, 0, 0), false, Duration, 0, Thickness);
			break;
		default:
			DrawDebugLine(TempWorld, RayCastResult.TraceStart, RayCastResult.TraceEnd, FColor(0, 255, 0), false, Duration, 0, Thickness);
			if (RayCastResult.bBlockingHit)DrawDebugPoint(TempWorld, RayCastResult.Location, Thickness, FColor(255, 0, 0), false, Duration, Thickness);
			break;
		}
	}
}

void FVehicleSuspensionSolver::DrawSuspensionForce(
	UVehicleWheelComponent* WheelComponent, 
	float Duration, 
	float Thickness, 
	float Length)
{
	UWorld* TempWorld = WheelComponent->GetWorld();
	if (!IsValid(TempWorld))return;

	FColor TempColor = FColor(0, 255, 127);
	if (State.ForceAlongImpactNormal < 0.f)TempColor = FColor(255 - TempColor.R, 255 - TempColor.G, 255 - TempColor.B);

	const FTransform& CarbodyTrans = WheelComponent->GetCarbodyAsyncWorldTransform();

	FVector TempStart = CarbodyTrans.TransformPositionNoScale(FVector(State.TopMountRelativePos));
	FVector TempEnd = TempStart + FVector(State.ImpactNormal) * State.ForceAlongImpactNormal * Length * 0.01;
	DrawDebugLine(TempWorld, TempStart, TempEnd, TempColor, false, Duration, 0, Thickness);
}

void FVehicleSuspensionSolver::UpdateCachedRichCurves(
	FVehicleSuspensionKinematicsConfig& KineConfig)
{
	const FVehicleSuspensionKinematicsConfig& Config = KineConfig;

	if (IsValid(Config.CamberCurve))
	{
		CachedCurves.CamberCurve = Config.CamberCurve->FloatCurve;
	}
	else
	{
		CachedCurves.CamberCurve.Reset();
	}
	if (IsValid(Config.ToeCurve))
	{
		CachedCurves.ToeCurve = Config.ToeCurve->FloatCurve;
	}
	else
	{
		CachedCurves.ToeCurve.Reset();
	}
	if (IsValid(Config.CasterCurve))
	{
		CachedCurves.CasterCurve = Config.CasterCurve->FloatCurve;
	}
	else
	{
		CachedCurves.CasterCurve.Reset();
	}
	if (IsValid(Config.AntiDiveCurve))
	{
		CachedCurves.AntiDiveCurve = Config.AntiDiveCurve->FloatCurve;
	}
	else
	{
		CachedCurves.AntiDiveCurve.Reset();
	}
	if (IsValid(Config.AntiSquatCurve))
	{
		CachedCurves.AntiSquatCurve = Config.AntiSquatCurve->FloatCurve;
	}
	else
	{
		CachedCurves.AntiSquatCurve.Reset();
	}
	if (IsValid(Config.AntiRollCurve))
	{
		CachedCurves.AntiRollCurve = Config.AntiRollCurve->FloatCurve;
	}
	else
	{
		CachedCurves.AntiRollCurve.Reset();
	}
	if (IsValid(Config.SpringMotionRatioCurve))
	{
		CachedCurves.MotionRatioCurve = Config.SpringMotionRatioCurve->FloatCurve;
	}
	else
	{
		CachedCurves.MotionRatioCurve.Reset();
		CachedCurves.MotionRatioCurve.AddKey(0.f, 1.f);
	}
}

bool FVehicleSuspensionSolver::CheckAndFixTriangle(
	FVehicleSuspensionKinematicsConfig& KineConfig)
{
	FVehicleSuspensionKinematicsConfig& Config = KineConfig;

	bool valid = true;
	
	float TowerToPivotDist = FVector2D(Config.TopMountPosition.Y, Config.TopMountPosition.X + Config.ArmLength).Length();

	FVector2D StrutLengthRange;		//X = min length; Y = max Length
	StrutLengthRange.X = FMath::Abs(Config.ArmLength - TowerToPivotDist);
	StrutLengthRange.Y = Config.ArmLength + TowerToPivotDist;

	if (Config.MinStrutLength > StrutLengthRange.Y)
	{
		Config.MinStrutLength = StrutLengthRange.Y;
		valid = false;
	}
	else if (Config.MinStrutLength < StrutLengthRange.X)
	{
		Config.MinStrutLength = StrutLengthRange.Y;
		valid = false;
	}

	float StrutLength = Config.MinStrutLength + Config.Stroke;

	if (StrutLength > StrutLengthRange.Y)
	{
		Config.Stroke = StrutLengthRange.Y - Config.MinStrutLength;
		return false;
	}
	else if (StrutLength < StrutLengthRange.X)
	{
		Config.Stroke = StrutLengthRange.X - Config.MinStrutLength;
		return false;
	}
	else
	{
		return valid;
	}
}

float FVehicleSuspensionSolver::GetVector2dAngleDegrees(FVector2D V2D)
{
	V2D = V2D.GetSafeNormal();
	float AngleRadian = FMath::Acos(V2D.X);
	AngleRadian *= FMath::Sign(V2D.Y);
	return FMath::RadiansToDegrees(AngleRadian);
}

FVector2D FVehicleSuspensionSolver::ComputeCircleIntersection(FVector2D A, float RA, float R0, bool ReturnX1)
{
	float DSq = A.SquaredLength();
	float c = R0 * R0 - RA * RA + DSq;
	float Delta = 4 * DSq * R0 * R0 - c * c;
	float DSqInv = UVehicleUtil::SafeDivide(1.f, DSq);
	float DeltaSqrt = FMath::Sqrt(Delta);

	if (Delta < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("VehicleSuspensionSolver: CalculatingCircle: Delta is Not Valid!"));
		return FVector2D();
	}

	float x;
	float y;
	if (ReturnX1)
	{
		x = A.X * c - A.Y * DeltaSqrt;
		x *= 0.5 * DSqInv;

		y = A.Y * c + A.X * DeltaSqrt;
		y *= 0.5 * DSqInv;
	}
	else
	{
		x = A.X * c + A.Y * DeltaSqrt;
		x *= 0.5 * DSqInv;

		y = A.Y * c - A.X * DeltaSqrt;
		y *= 0.5 * DSqInv;
	}

	return FVector2D(x, y);
}

FQuat4f FVehicleSuspensionSolver::MakeQuatFrom2DVectors(const FVector2f From, const FVector2f To, const FVector3f Axis)
{
	FVector2f A = From.GetSafeNormal();
	FVector2f B = To.GetSafeNormal();

	float cosTheta = FVector2f::DotProduct(A, B);              // cosθ
	float sinTheta = A.X * B.Y - A.Y * B.X;                    // sinθ（2D 叉积）

	// Clamp for numerical stability
	cosTheta = FMath::Clamp(cosTheta, -1.0f, 1.0f);

	float cosHalf = FMath::Sqrt((1.0f + cosTheta) * 0.5f);    // cos(θ/2)
	float sinHalf = FMath::Sqrt((1.0f - cosTheta) * 0.5f);    // sin(θ/2)
	sinHalf *= FMath::Sign(sinTheta);                         // 保持方向

	FVector3f axisNorm = Axis.GetSafeNormal();
	return FQuat4f(
		axisNorm.X * sinHalf,
		axisNorm.Y * sinHalf,
		axisNorm.Z * sinHalf,
		cosHalf
	);
}

void FVehicleSuspensionSolver::CopyContextToState(const FVehicleSuspensionSimContext& Context)
{
	State.bIsRightWheel = Context.WheelPos >= 0.f;
	State.bHitGround = Context.bHitGround;
	State.SteeringAngle = Context.SteeringAngle;
	State.SuspensionCurrentLength = Context.SuspensionCurrentLength;
	State.ForceAlongImpactNormal = Context.ForceAlongImpactNormal;
	State.KnucklePos2D = FVector2f(Context.KnucklePos2D);
	State.WheelRightVector = FVector3f(Context.WheelRightVector);
	State.WheelCenterToKnuckle = FVector3f(Context.WheelCenterToKnuckle);
	State.KnuckleRelativePos = FVector3f(Context.KnuckleRelativePos);
	State.TopMountRelativePos = FVector3f(Context.TopMountRelativePos);
	State.StrutDirection = FVector3f(Context.StrutDirection);
	State.ImpactPointWorldVelocity = FVector3f(Context.ImpactPointWorldVelocity);
	State.ImpactNormal = FVector3f(Context.HitStruct.Normal);
	State.ImpactPoint = Context.HitStruct.ImpactPoint;
	State.WheelWorldPos = Context.WheelWorldPos;
	State.WheelRelativeRotation = FQuat4f(Context.WheelRelativeTransform.GetRotation());
	State.AntiPitchScale = Context.AntiPitchScale;
	State.AntiRollScale = Context.AntiRollScale;

	State.ImpactFriction = Context.ImpactFriction;

	// copy hitresult
	RayCastResult = FVehicleSuspensionHitResult(
		Context.HitStruct.PhysMaterial,
		Context.HitStruct.Component,
		Context.HitStruct.BoneName,
		Context.HitStruct.bBlockingHit,
		Context.HitStruct.TraceStart,
		Context.HitStruct.TraceEnd,
		Context.HitStruct.Location,
		(FQuat4f)Context.RayCastTransform.GetRotation()
	);
}

void FVehicleSuspensionSolver::CopyStateToContext(FVehicleSuspensionSimContext& Context)
{
	Context.SuspensionCurrentLength = State.SuspensionCurrentLength;
	Context.SprungMass = State.SprungMass;
	Context.KnucklePos2D = State.KnucklePos2D;
	Context.WheelCenterToKnuckle = State.WheelCenterToKnuckle;
	Context.WheelRelativeTransform.SetRotation(State.WheelRelativeRotation);
	Context.WheelRelativeTransform.SetLocation(State.WheelCenterToKnuckle + State.KnuckleRelativePos);
}

FVector3f FVehicleSuspensionSolver::GetCamberToeCasterFromCurve(
	const FVehicleSuspensionCachedRichCurves& Curves,
	float CompressionRatio,
	float WheelYPosSign,
	float BaseCamber,
	float BaseToe,
	float BaseCaster)
{
	FVector3f v;
	v.X = BaseCaster;
	v.Y = BaseToe;
	v.Z = BaseCamber;

	v.Y += Curves.ToeCurve.Eval(CompressionRatio);
	v.Z += Curves.CamberCurve.Eval(CompressionRatio);

	// flip the caster and toe if necessary
	v *= WheelYPosSign;

	v.X += Curves.CasterCurve.Eval(CompressionRatio);

	return v;
}

FQuat4f FVehicleSuspensionSolver::GetSpindleMountQuat(
	const FRotator3f& InSpindleMountRotationConfig,
	const float WheelPos)
{
	return FQuat4f(FRotator3f(
		InSpindleMountRotationConfig.Pitch,
		InSpindleMountRotationConfig.Yaw * WheelPos,
		InSpindleMountRotationConfig.Roll * WheelPos
	));
}