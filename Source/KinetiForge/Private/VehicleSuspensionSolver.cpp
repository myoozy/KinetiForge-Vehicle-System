// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)


#include "VehicleSuspensionSolver.h"
#include "VehicleWheelComponent.h"
#include "AsyncTickFunctions.h"

static FORCEINLINE auto SafeDivide(auto a, auto b)
{
	return (FMath::IsNearlyZero(b)) ? 0.0f : a / b;
}

static void PrepareSimulation(
	FVehicleSuspensionSimContext& Ctx,
	UVehicleWheelComponent* TargetWheelComponent)
{
	Ctx.RelativeTransform = (FTransform3f)TargetWheelComponent->GetRelativeTransform();
	Ctx.CarbodyWorldTransform = TargetWheelComponent->GetCarbodyAsyncWorldTransform();

	//dealing with transforms
	Ctx.WheelPos = FMath::Sign(Ctx.RelativeTransform.GetLocation().Y);
	Ctx.WheelPos += Ctx.WheelPos == 0.f;	//if(wheelpos == 0) wheelpos = 1;
	FVector3f TopMountLocalPos = TargetWheelComponent->SuspensionKinematicsConfig.TopMountPosition;
	TopMountLocalPos.Y *= Ctx.WheelPos;
	Ctx.TopMountPos2D = FVehicleSuspensionSolver::ZYPlaneToSuspensionPlane(TopMountLocalPos, Ctx.WheelPos);
	Ctx.TopMountRelativePos = Ctx.RelativeTransform.TransformPositionNoScale(TopMountLocalPos);
	Ctx.ComponentRelativeForwardVector = Ctx.RelativeTransform.GetRotation().GetForwardVector();
}

static float ComputeValidPreload(
	FVehicleSuspensionSimContext& Ctx,
	UVehicleWheelComponent* TargetWheelComponent)
{
	//check suspension preload
	//preload should not be greater than gravity force on the wheel
	float Proj = FVector3f::DotProduct(FVector3f(0.f, 0.f, 1.f), Ctx.StrutRelativeDirection);
	float MaxPreload = SafeDivide(0.99 * Ctx.WorldGravityZ * Ctx.SprungMass, Proj);

	return Ctx.ValidPreload = FMath::Min(TargetWheelComponent->SuspensionSpringConfig.SpringPreload, MaxPreload);
}

static void ComputeRayCastLocation(
	FVehicleSuspensionSimContext& Ctx,
	UVehicleWheelComponent* TargetWheelComponent)
{
	Ctx.StrutDirection2D = (Ctx.TopMountPos2D - Ctx.KnucklePos2D).GetSafeNormal();
	Ctx.RayCastLength = FMath::Abs(Ctx.StrutDirection2D.X * TargetWheelComponent->SuspensionKinematicsConfig.Stroke);

	Ctx.RayCastStart2D.X = Ctx.TopMountPos2D.X - Ctx.StrutDirection2D.X * TargetWheelComponent->SuspensionKinematicsConfig.MinStrutLength;
	Ctx.RayCastStart2D.Y = Ctx.KnucklePos2D.Y;

	FVector3f RayCastStartLocal = FVehicleSuspensionSolver::SuspensionPlaneToZYPlane(Ctx.RayCastStart2D, Ctx.WheelPos);
	FVector3f RayCastStartRelative = Ctx.RelativeTransform.TransformPositionNoScale(RayCastStartLocal) + Ctx.WheelCenterToKnuckle;
	Ctx.RayCastStartPos = Ctx.CarbodyWorldTransform.TransformPositionNoScale((FVector)RayCastStartRelative);

	FQuat ComponentWorldRot = FQuat(Ctx.RelativeTransform.GetRotation()) * Ctx.CarbodyWorldTransform.GetRotation();
	Ctx.ComponentUpVector = ComponentWorldRot.GetUpVector();
	Ctx.RayCastEndPos = Ctx.RayCastStartPos - Ctx.RayCastLength * Ctx.ComponentUpVector;

	FQuat SteeringRot = FQuat(Ctx.ComponentUpVector, FMath::DegreesToRadians(Ctx.SteeringAngle));
	FQuat RayCastRot = FQuat(SteeringRot) * ComponentWorldRot;

	Ctx.RayCastTransform = FTransform(RayCastRot, Ctx.RayCastEndPos, FVector(1, 1, 1));
}

static bool ShouldDoRefinedTrace(
	FVehicleSuspensionSimContext& Ctx,
	UVehicleWheelComponent* TargetWheelComponent)
{
	float HalfWidth = TargetWheelComponent->WheelConfig.Width * 0.5f;

	FVector LocalImpactPoint = Ctx.RayCastTransform.InverseTransformPositionNoScale(Ctx.HitStruct.ImpactPoint);

	return FMath::Abs(LocalImpactPoint.Y) > HalfWidth || LocalImpactPoint.Z > TargetWheelComponent->SuspensionKinematicsConfig.Stroke;
}

static void ComputeHitDistance(
	FVehicleSuspensionSimContext& Ctx,
	float WheelRadius,
	float EquivalentSphereTraceRadius)
{
	Ctx.HitDistance = Ctx.bHitGround ?
		Ctx.HitStruct.Distance + EquivalentSphereTraceRadius : Ctx.RayCastLength + WheelRadius;
	float HitDistanceNoBias = FMath::Max(0.f, Ctx.HitDistance - WheelRadius);
	Ctx.SuspensionExtensionRatio = SafeDivide(HitDistanceNoBias, Ctx.RayCastLength);
}

static void SuspensionLineTrace(
	FVehicleSuspensionSimContext& Ctx,
	UVehicleWheelComponent* TargetWheelComponent,
	FCollisionQueryParams& QueryParams)
{
	float WheelRadius = TargetWheelComponent->WheelConfig.Radius;
	float HalfWidth = TargetWheelComponent->WheelConfig.Width * 0.5f;

	FVector WheelRightVector = Ctx.CarbodyWorldTransform.TransformRotation(
		FQuat(Ctx.WheelRelativeTransform.GetRotation())).GetRightVector();
	FVector WheelOuterSideToCenter = HalfWidth * Ctx.WheelPos * WheelRightVector;
	FVector Start = Ctx.RayCastStartPos + WheelOuterSideToCenter;
	FVector End = Ctx.RayCastEndPos + WheelOuterSideToCenter;

	FVector LineTraceEnd = End - Ctx.ComponentUpVector * WheelRadius;

	Ctx.bRayCastRefined = false;

	/**************RaycastSingle**************/
	Ctx.bHitGround = FPhysicsInterface::RaycastSingle(TargetWheelComponent->GetWorld(), Ctx.HitStruct,
		Start, LineTraceEnd,
		TargetWheelComponent->SuspensionKinematicsConfig.TraceChannel, QueryParams, FCollisionResponseParams::DefaultResponseParam, FCollisionObjectQueryParams::DefaultObjectQueryParam);

	ComputeHitDistance(Ctx, WheelRadius, 0.f);
}

static void SuspensionSphereTrace(
	FVehicleSuspensionSimContext& Ctx,
	UVehicleWheelComponent* TargetWheelComponent,
	FCollisionQueryParams& QueryParams)
{
	FVehicleSuspensionKinematicsConfig& Config = TargetWheelComponent->SuspensionKinematicsConfig;
	float WheelRadius = TargetWheelComponent->WheelConfig.Radius;

	Ctx.bRayCastRefined = false;
	Ctx.bHitGround = FPhysicsInterface::GeomSweepSingle(
		TargetWheelComponent->GetWorld(), FCollisionShape::MakeSphere(WheelRadius), FQuat::Identity, Ctx.HitStruct,
		Ctx.RayCastStartPos, Ctx.RayCastEndPos, TargetWheelComponent->SuspensionKinematicsConfig.TraceChannel, QueryParams,
		FCollisionResponseParams::DefaultResponseParam, FCollisionObjectQueryParams::DefaultObjectQueryParam);

	if (Ctx.bHitGround)
	{
		float HalfWidth = TargetWheelComponent->WheelConfig.Width * 0.5;

		Ctx.bRayCastRefined = ShouldDoRefinedTrace(Ctx, TargetWheelComponent);

		if (Ctx.bRayCastRefined)
		{
			FVector SecondStepRayCastStart = Ctx.RayCastStartPos + Ctx.ComponentUpVector * (HalfWidth - WheelRadius);
			FVector SecondStepRayCastEnd = SecondStepRayCastStart - Ctx.ComponentUpVector * Ctx.RayCastLength;

			Ctx.bHitGround = FPhysicsInterface::GeomSweepSingle(
				TargetWheelComponent->GetWorld(), FCollisionShape::MakeSphere(HalfWidth), FQuat::Identity, Ctx.HitStruct,
				SecondStepRayCastStart, SecondStepRayCastEnd, TargetWheelComponent->SuspensionKinematicsConfig.TraceChannel, QueryParams,
				FCollisionResponseParams::DefaultResponseParam, FCollisionObjectQueryParams::DefaultObjectQueryParam);
		}
	}

	ComputeHitDistance(Ctx, WheelRadius, WheelRadius);
}

static void SuspensionBoxTrace(
	FVehicleSuspensionSimContext& Ctx,
	UVehicleWheelComponent* TargetWheelComponent,
	FCollisionQueryParams& QueryParams)
{
	Ctx.bRayCastRefined = false;

	float WheelRadius = TargetWheelComponent->WheelConfig.Radius;
	float WheelHalfWidth = TargetWheelComponent->WheelConfig.Width * 0.5;

	FVector HalfSize = FVector(WheelRadius, WheelHalfWidth, WheelRadius) * 0.707;
	FQuat Orientation = Ctx.RayCastTransform.TransformRotation(
		FQuat(0.0f, 0.38268343f, 0.0f, 0.92387953f));	// this is Rotator(0.0, 45.0, 0.0)

	/*****GeomSweepSingle*****/
	Ctx.bHitGround = FPhysicsInterface::GeomSweepSingle(
		TargetWheelComponent->GetWorld(),
		FCollisionShape::MakeBox(HalfSize),
		Orientation,
		Ctx.HitStruct,
		Ctx.RayCastStartPos,
		Ctx.RayCastEndPos,
		TargetWheelComponent->SuspensionKinematicsConfig.TraceChannel,
		QueryParams,
		FCollisionResponseParams::DefaultResponseParam,
		FCollisionObjectQueryParams::DefaultObjectQueryParam);

	ComputeHitDistance(Ctx, WheelRadius, WheelRadius);

	if (Ctx.bHitGround)
	{
		// try to maintain the impact point in the middle of the shape so that it will be more stable
		FVector NormalVec = Ctx.CarbodyWorldTransform.GetRotation().GetRightVector();
		FVector RelativeBias = Ctx.HitStruct.ImpactPoint - Ctx.HitStruct.Location;
		FVector BiasProjection = FVector::VectorPlaneProject(RelativeBias, NormalVec);
		Ctx.HitStruct.ImpactPoint = Ctx.HitStruct.Location + BiasProjection;
	}
}

static void SuspensionSphereTraceNoRefinement(
	FVehicleSuspensionSimContext& Ctx,
	UVehicleWheelComponent* TargetWheelComponent,
	FCollisionQueryParams& QueryParams)
{
	Ctx.bRayCastRefined = false;

	float WheelRadius = TargetWheelComponent->WheelConfig.Radius;
	float WheelHalfWidth = TargetWheelComponent->WheelConfig.Width * 0.5;
	float ValidRadius = FMath::Min(WheelRadius, WheelHalfWidth);
	float RadiusDifference = WheelRadius - ValidRadius;
	FVector TraceEnd = Ctx.RayCastEndPos - Ctx.ComponentUpVector * RadiusDifference;

	Ctx.bHitGround = FPhysicsInterface::GeomSweepSingle(
		TargetWheelComponent->GetWorld(), FCollisionShape::MakeSphere(ValidRadius), FQuat::Identity, Ctx.HitStruct,
		Ctx.RayCastStartPos, TraceEnd, TargetWheelComponent->SuspensionKinematicsConfig.TraceChannel, QueryParams,
		FCollisionResponseParams::DefaultResponseParam, FCollisionObjectQueryParams::DefaultObjectQueryParam);

	ComputeHitDistance(Ctx, WheelRadius, ValidRadius);
}

static void SuspensionMultiSphereTrace(
	FVehicleSuspensionSimContext& Ctx,
	UVehicleWheelComponent* TargetWheelComponent,
	FCollisionQueryParams& QueryParams)
{
	FVehicleSuspensionKinematicsConfig& Config = TargetWheelComponent->SuspensionKinematicsConfig;
	float WheelRadius = TargetWheelComponent->WheelConfig.Radius;

	Ctx.bRayCastRefined = false;
	Ctx.bHitGround = FPhysicsInterface::GeomSweepSingle(
		TargetWheelComponent->GetWorld(), FCollisionShape::MakeSphere(WheelRadius), FQuat::Identity, Ctx.HitStruct,
		Ctx.RayCastStartPos, Ctx.RayCastEndPos, TargetWheelComponent->SuspensionKinematicsConfig.TraceChannel, QueryParams,
		FCollisionResponseParams::DefaultResponseParam, FCollisionObjectQueryParams::DefaultObjectQueryParam);

	if (Ctx.bHitGround)
	{
		float HalfWidth = TargetWheelComponent->WheelConfig.Width * 0.5;

		Ctx.bRayCastRefined = ShouldDoRefinedTrace(Ctx, TargetWheelComponent);

		if (Ctx.bRayCastRefined)
		{
			// first test with a bigger box
			FVector HalfSize = FVector(WheelRadius, HalfWidth, WheelRadius);
			bool bShouldMultiSphereTrace = FPhysicsInterface::GeomSweepSingle(
				TargetWheelComponent->GetWorld(),
				FCollisionShape::MakeBox(HalfSize),
				Ctx.RayCastTransform.GetRotation(),
				Ctx.HitStruct,
				Ctx.RayCastStartPos,
				Ctx.RayCastEndPos,
				TargetWheelComponent->SuspensionKinematicsConfig.TraceChannel,
				QueryParams,
				FCollisionResponseParams::DefaultResponseParam,
				FCollisionObjectQueryParams::DefaultObjectQueryParam);

			// then if there is someting inside the box, we do multi-sphere trace
			if (bShouldMultiSphereTrace)
			{
				TArray<FHitResult> MultiHitResults;
				Ctx.bHitGround = FPhysicsInterface::GeomSweepMulti(
					TargetWheelComponent->GetWorld(),
					FCollisionShape::MakeSphere(WheelRadius),
					FQuat::Identity,
					MultiHitResults,
					Ctx.RayCastStartPos,
					Ctx.RayCastEndPos,
					TargetWheelComponent->SuspensionKinematicsConfig.TraceChannel,
					QueryParams,
					FCollisionResponseParams::DefaultResponseParam,
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

	ComputeHitDistance(Ctx, WheelRadius, WheelRadius);
}

static void SuspensionRayCast(
	FVehicleSuspensionSimContext& Ctx,
	UVehicleWheelComponent* TargetWheelComponent,
	FCollisionQueryParams& QueryParams)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UpdateVehicleSuspensionRayCast);

	// savety check
	if (Ctx.RayCastStartPos.ContainsNaN()
		|| Ctx.RayCastEndPos.ContainsNaN()
		|| !IsValid(TargetWheelComponent->GetWorld()))
	{
		// reset the suspension
		Ctx.bHitGround = false;
		ComputeHitDistance(Ctx, TargetWheelComponent->WheelConfig.Radius, 0.f);
		return;
	}

	switch (TargetWheelComponent->SuspensionKinematicsConfig.RayCastMode)
	{
	case ESuspensionRayCastMode::LineTrace:
		SuspensionLineTrace(Ctx, TargetWheelComponent, QueryParams);
		break;
	case ESuspensionRayCastMode::SphereTrace:
		SuspensionSphereTrace(Ctx, TargetWheelComponent, QueryParams);
		break;
	case ESuspensionRayCastMode::BoxTrace:
		SuspensionBoxTrace(Ctx, TargetWheelComponent, QueryParams);
		break;
	case ESuspensionRayCastMode::SphereTraceNoRefinement:
		SuspensionSphereTraceNoRefinement(Ctx, TargetWheelComponent, QueryParams);
		break;
	case ESuspensionRayCastMode::MultiSphereTrace:
		SuspensionMultiSphereTrace(Ctx, TargetWheelComponent, QueryParams);
		break;
	default:
		SuspensionLineTrace(Ctx, TargetWheelComponent, QueryParams);
		break;
	}
}

static void ComputeStraightSuspension(
	FVehicleSuspensionSimContext& Ctx,
	UVehicleWheelComponent* TargetWheelComponent)
{
	FVehicleSuspensionKinematicsConfig& Config = TargetWheelComponent->SuspensionKinematicsConfig;

	//Compute the position of the Knuckle on the suspension plane
	float WheelRadius = TargetWheelComponent->WheelConfig.Radius;
	Ctx.KnucklePos2D.X = Ctx.RayCastStart2D.X - FMath::Max(0.f, Ctx.HitDistance - WheelRadius);
	Ctx.KnucklePos2D.Y = Config.ArmLength;

	Ctx.KnuckleRelativePos = Ctx.RelativeTransform.TransformPositionNoScale(
		FVehicleSuspensionSolver::SuspensionPlaneToZYPlane(Ctx.KnucklePos2D, Ctx.WheelPos));

	Ctx.StrutRelativeDirection = Ctx.RelativeTransform.GetRotation().GetUpVector();

	// the kingpin(steering axis) is the up vector
	FQuat4f SteeringBiasRotation = FQuat4f(Ctx.StrutRelativeDirection, FMath::DegreesToRadians(Ctx.SteeringAngle));

	FQuat4f InitialWheelRelativeRotation = FQuat4f(FRotator3f(0.f, Config.BaseToe * Ctx.WheelPos, Config.BaseCamber * Ctx.WheelPos));
	Ctx.WheelRelativeTransform.SetRotation(SteeringBiasRotation * InitialWheelRelativeRotation);
	FVector3f WheelRelativeRightVec = Ctx.WheelRelativeTransform.GetRotation().GetRightVector();

	Ctx.WheelCenterToKnuckle = Config.AxialHubOffset * Ctx.WheelPos * WheelRelativeRightVec;
	Ctx.WheelRelativeTransform.SetLocation(Ctx.KnuckleRelativePos + Ctx.WheelCenterToKnuckle);

	Ctx.WheelRightVector = Ctx.CarbodyWorldTransform.TransformVectorNoScale(FVector(WheelRelativeRightVec));

	Ctx.WheelWorldPos = Ctx.CarbodyWorldTransform.TransformPositionNoScale(FVector(Ctx.WheelRelativeTransform.GetLocation()));
}

static void ComputeMacpherson(
	FVehicleSuspensionSimContext& Ctx,
	UVehicleWheelComponent* TargetWheelComponent)
{
	FVehicleSuspensionKinematicsConfig& Config = TargetWheelComponent->SuspensionKinematicsConfig;

	//Compute the position of the Knuckle on the suspension plane
	float WheelRadius = TargetWheelComponent->WheelConfig.Radius;
	Ctx.KnucklePos2D.X = Ctx.RayCastStart2D.X - FMath::Max(0.f, Ctx.HitDistance - WheelRadius);
	Ctx.KnucklePos2D.X = FMath::Clamp(Ctx.KnucklePos2D.X, -Config.ArmLength, Config.ArmLength);
	Ctx.KnucklePos2D.Y = FMath::Sqrt(FMath::Square(Config.ArmLength) - FMath::Square(Ctx.KnucklePos2D.X));

	Ctx.KnuckleRelativePos = Ctx.RelativeTransform.TransformPositionNoScale(
		FVehicleSuspensionSolver::SuspensionPlaneToZYPlane(Ctx.KnucklePos2D, Ctx.WheelPos));

	FVector3f InitialKnuckleRelativePos = Ctx.RelativeTransform.TransformPositionNoScale(
		FVehicleSuspensionSolver::SuspensionPlaneToZYPlane(FVector2f(0, Config.ArmLength), Ctx.WheelPos));
	FVector3f InitialStrutDirection = (Ctx.TopMountRelativePos - InitialKnuckleRelativePos).GetSafeNormal();
	Ctx.StrutRelativeDirection = (Ctx.TopMountRelativePos - Ctx.KnuckleRelativePos).GetSafeNormal();
	FQuat4f StrutBiasRotation = FQuat4f::FindBetweenNormals(InitialStrutDirection, Ctx.StrutRelativeDirection);

	// the kingpin(steering axis) is the strut, for simplification
	FQuat4f SteeringBiasRotation = FQuat4f(Ctx.StrutRelativeDirection, FMath::DegreesToRadians(Ctx.SteeringAngle));

	FQuat4f InitialWheelRelativeRotation = FQuat4f(FRotator3f(0, Config.BaseToe * Ctx.WheelPos, Config.BaseCamber * Ctx.WheelPos));
	Ctx.WheelRelativeTransform.SetRotation(SteeringBiasRotation * StrutBiasRotation * InitialWheelRelativeRotation);
	FVector3f WheelRelativeRightVec = Ctx.WheelRelativeTransform.GetRotation().GetRightVector();

	Ctx.WheelCenterToKnuckle = Config.AxialHubOffset * Ctx.WheelPos * WheelRelativeRightVec;
	Ctx.WheelRelativeTransform.SetLocation(Ctx.KnuckleRelativePos + Ctx.WheelCenterToKnuckle);

	Ctx.WheelRightVector = Ctx.CarbodyWorldTransform.TransformVectorNoScale(FVector(WheelRelativeRightVec));

	Ctx.WheelWorldPos = Ctx.CarbodyWorldTransform.TransformPositionNoScale(FVector(Ctx.WheelRelativeTransform.GetLocation()));
}

static void ComputeDoubleWishbone(
	FVehicleSuspensionSimContext& Ctx,
	UVehicleWheelComponent* TargetWheelComponent)
{
	FVehicleSuspensionKinematicsConfig& Config = TargetWheelComponent->SuspensionKinematicsConfig;

	//Compute the position of the Knuckle on the suspension plane
	float WheelRadius = TargetWheelComponent->WheelConfig.Radius;
	Ctx.KnucklePos2D.X = Ctx.RayCastStart2D.X - FMath::Max(0.f, Ctx.HitDistance - WheelRadius);
	Ctx.KnucklePos2D.X = FMath::Clamp(Ctx.KnucklePos2D.X, -Config.ArmLength, Config.ArmLength);
	Ctx.KnucklePos2D.Y = FMath::Sqrt(FMath::Square(Config.ArmLength) - FMath::Square(Ctx.KnucklePos2D.X));

	Ctx.KnuckleRelativePos = Ctx.RelativeTransform.TransformPositionNoScale(
		FVehicleSuspensionSolver::SuspensionPlaneToZYPlane(Ctx.KnucklePos2D, Ctx.WheelPos));
	Ctx.TopMountRelativePos = Ctx.RelativeTransform.TransformPositionNoScale(
		FVehicleSuspensionSolver::SuspensionPlaneToZYPlane(Ctx.TopMountPos2D, Ctx.WheelPos));

	Ctx.StrutRelativeDirection = (Ctx.TopMountRelativePos - Ctx.KnuckleRelativePos).GetSafeNormal();

	FVector3f WheelAlignmentEuler =
		FVehicleSuspensionSolver::GetCamberToeCasterFromCurve(1.f - Ctx.SuspensionExtensionRatio, Ctx.WheelPos, Config);
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

static void UpdateImpactPointWorldVelocity(
	FVehicleSuspensionSimContext& Ctx,
	UVehicleWheelComponent* TargetWheelComponent)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UpdateVehicleWheelLinearVelocity);

	if (Ctx.HitStruct.bBlockingHit)
	{
		FVector LinVelWorldA = UAsyncTickFunctions::ATP_GetLinearVelocityAtPoint(
			TargetWheelComponent->GetCarbody(),
			Ctx.HitStruct.ImpactPoint,
			"NONE"
		);
		FVector LinVelWorldB = FVector(0.f);

		if (IsValid(Ctx.HitStruct.GetComponent()) && Ctx.HitStruct.GetComponent()->Mobility != EComponentMobility::Static)
		{
			LinVelWorldB = UAsyncTickFunctions::ATP_GetLinearVelocityAtPoint(
				Ctx.HitStruct.GetComponent(),
				Ctx.HitStruct.ImpactPoint,
				Ctx.HitStruct.BoneName
			);
		}

		Ctx.ImpactPointWorldVelocity = FVector3f(0.01 * (LinVelWorldA - LinVelWorldB));
	}
	else
	{
		Ctx.ImpactPointWorldVelocity = FVector3f(0.f);
	}
}

static void ComputeSuspensionForce(
	FVehicleSuspensionSimContext& Ctx,
	UVehicleWheelComponent* TargetWheelComponent)
{
	FVehicleSuspensionSpringConfig& Config = TargetWheelComponent->SuspensionSpringConfig;

	float SuspensionStroke = TargetWheelComponent->SuspensionKinematicsConfig.Stroke;
	float WheelRadius = TargetWheelComponent->WheelConfig.Radius;

	float LastLength = Ctx.SuspensionCurrentLength;

	Ctx.SuspensionCurrentLength = SuspensionStroke * Ctx.SuspensionExtensionRatio;

	ComputeValidPreload(Ctx, TargetWheelComponent);

	float DeltaTimeInv = SafeDivide(1.f, Ctx.PhysicsDelatTime);

	float SpringForce = Config.SpringStiffness * (SuspensionStroke - Ctx.SuspensionCurrentLength) + Ctx.ValidPreload + Ctx.SwaybarForce;

	float DampingRatio = (Ctx.SuspensionCurrentLength > LastLength) ? Config.ReboundDampingRatio : Config.CompressionDampingRatio;
	float DamperStiffness = Ctx.CriticalDamping * DampingRatio;

	float DampingForce = DamperStiffness * (LastLength - Ctx.SuspensionCurrentLength) * DeltaTimeInv;

	Ctx.SuspensionForce = SpringForce + DampingForce;

	Ctx.StrutDirection = Ctx.CarbodyWorldTransform.TransformVectorNoScale((FVector)Ctx.StrutRelativeDirection);
	FVector SuspensionForceVector = Ctx.StrutDirection * Ctx.SuspensionForce;

	// if suspension is compeletly compressed
	float ForceToHoldCar = 0.f;
	if (Ctx.SuspensionCurrentLength < SMALL_NUMBER)
	{
		// try to stop the car immediately
		float VelocityIntoSurface = FVector::DotProduct(Ctx.HitStruct.ImpactNormal, (FVector)Ctx.ImpactPointWorldVelocity);
		float ForceToBringToStop = -VelocityIntoSurface * DeltaTimeInv * Ctx.SprungMass;
		ForceToHoldCar = ForceToBringToStop * Config.BottomOutStiffness;

		// the spring system in another direction (normal of impact surface)
		FVector WheelCenterToImpactPoint = Ctx.WheelWorldPos - Ctx.HitStruct.ImpactPoint;
		float DistanceToSurface = FVector::DotProduct(Ctx.HitStruct.ImpactNormal, WheelCenterToImpactPoint);
		SpringForce = (WheelRadius - DistanceToSurface) * Config.SpringStiffness;
		DampingForce = -VelocityIntoSurface * DamperStiffness;
		ForceToHoldCar += SpringForce + DampingForce;
	}

	float SuspensionForceProj = FVector::DotProduct(SuspensionForceVector, Ctx.HitStruct.ImpactNormal);
	Ctx.ForceAlongImpactNormal = SuspensionForceProj + ForceToHoldCar;

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

bool FVehicleSuspensionSolver::Initialize(UVehicleWheelComponent* InTargetWheelComponent)
{
	TargetWheelComponent = InTargetWheelComponent;
	
	if (IsValid(TargetWheelComponent))
	{
		QueryParams.AddIgnoredActor(TargetWheelComponent->GetOwner());
		QueryParams.bReturnPhysicalMaterial = true;
		QueryParams.bReturnFaceIndex = false;
		QueryParams.bTraceComplex = false;
		
		float WheelPos = FMath::Sign(TargetWheelComponent->GetRelativeTransform().GetLocation().Y);
		State.bIsRightWheel = WheelPos >= 0.f;
		
		ApplySuspensionStateDirect();

		//initialize the cache, so that damping will not be Computed twice when game starts
		CachedSpringStiffness  = TargetWheelComponent->SuspensionSpringConfig.SpringStiffness;
		
		return true;
	}
	else
	{
		return false;
	}
}

void FVehicleSuspensionSolver::SetSprungMass(float NewSprungMass)
{
	if (NewSprungMass <= 0)
	{
		State.SprungMass = 0;
	}
	else
	{
		State.SprungMass = NewSprungMass;
	}

	ComputeCriticalDamping();
}

float FVehicleSuspensionSolver::ComputeCriticalDamping()
{
	if (State.SprungMass <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("VehicleSuspensionSolver: SprungMass not valid!"));
		State.CriticalDamping = 0.1 * TargetWheelComponent->SuspensionSpringConfig.SpringStiffness;
		return State.CriticalDamping;
	}
	else
	{
		float NaturalFrequency = FMath::Sqrt(TargetWheelComponent->SuspensionSpringConfig.SpringStiffness * 100 / State.SprungMass);
		State.CriticalDamping = 0.02 * State.SprungMass * NaturalFrequency;
		return State.CriticalDamping;
	}
}

void FVehicleSuspensionSolver::UpdateSuspension(
	float InDeltaTime,
	float InSteeringAngle, 
	float InSwaybarForce)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UpdateVehicleSuspensionSolver);

	if (!IsValid(TargetWheelComponent))return;

	FVehicleSuspensionSimContext Ctx;

	CopyStateToContext(Ctx, State);

	Ctx.PhysicsDelatTime = InDeltaTime;
	Ctx.SteeringAngle = InSteeringAngle;
	Ctx.SwaybarForce = InSwaybarForce;

	PrepareSimulation(Ctx, TargetWheelComponent);
	ComputeRayCastLocation(Ctx, TargetWheelComponent);
	SuspensionRayCast(Ctx, TargetWheelComponent, QueryParams);

	switch (TargetWheelComponent->SuspensionKinematicsConfig.SuspensionType)
	{
	case ESuspensionType::StraightLine:
		ComputeStraightSuspension(Ctx, TargetWheelComponent);
		break;
	case ESuspensionType::Macpherson:
		ComputeMacpherson(Ctx, TargetWheelComponent);
		break;
	case ESuspensionType::DoubleWishbone:
		ComputeDoubleWishbone(Ctx, TargetWheelComponent);
		break;
	default:
		ComputeStraightSuspension(Ctx, TargetWheelComponent);
		break;
	}

	UpdateImpactPointWorldVelocity(Ctx, TargetWheelComponent);
	ComputeSuspensionForce(Ctx, TargetWheelComponent);
	CopyContextToState(State, Ctx);
}

void FVehicleSuspensionSolver::StartUpdateSolidAxle(
	float InSteeringAngle,
	FVector& OutApporximatedWheelWorldPos,
	FVehicleSuspensionSimContext& Ctx)
{
	if (!IsValid(TargetWheelComponent))return;

	CopyStateToContext(Ctx, State);

	Ctx.SteeringAngle = InSteeringAngle;
	PrepareSimulation(Ctx, TargetWheelComponent);
	ComputeRayCastLocation(Ctx, TargetWheelComponent);
	SuspensionRayCast(Ctx, TargetWheelComponent, QueryParams);

	float DistanceToRayCastStart = FMath::Max(0.f, Ctx.HitDistance - TargetWheelComponent->WheelConfig.Radius);
	FVector OffsetToRayCastStart = Ctx.ComponentUpVector * DistanceToRayCastStart;
	OutApporximatedWheelWorldPos = Ctx.RayCastStartPos - OffsetToRayCastStart;

	return;
}

void FVehicleSuspensionSolver::FinalizeUpdateSolidAxle(
	float InDeltaTime, 
	float InSwaybarForce,
	FVehicleSuspensionSimContext& Ctx,
	const FVector& InKnuckleWorldPos,
	const FVector& InAxleWorldDirection)
{
	if (!IsValid(TargetWheelComponent))return;

	Ctx.PhysicsDelatTime = InDeltaTime;
	Ctx.SwaybarForce = InSwaybarForce;

	// get relative position of the ball joint
	Ctx.KnuckleRelativePos = (FVector3f)Ctx.CarbodyWorldTransform.InverseTransformPositionNoScale(InKnuckleWorldPos);
	
	// the relative direction of the axle, which is also the right vector of the wheel
	// because solid axle usually does not have camber/toe
	FVector3f AxleDirection = (FVector3f)Ctx.CarbodyWorldTransform.InverseTransformVectorNoScale(InAxleWorldDirection);
	Ctx.WheelCenterToKnuckle = TargetWheelComponent->SuspensionKinematicsConfig.AxialHubOffset * Ctx.WheelPos * AxleDirection;
	
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
	Ctx.KnucklePos2D = ZYPlaneToSuspensionPlane(KnuckleLocalPos, Ctx.WheelPos);

	// update velocity
	UpdateImpactPointWorldVelocity(Ctx, TargetWheelComponent);

	// get suspension force
	ComputeSuspensionForce(Ctx, TargetWheelComponent);

	CopyContextToState(State, Ctx);
}

void FVehicleSuspensionSolver::ApplySuspensionStateDirect(float InExtensionRatio, float InSteeringAngle)
{
	if (!IsValid(TargetWheelComponent))return;

	FVehicleSuspensionSimContext Ctx;
	CopyStateToContext(Ctx, State);
	Ctx.SuspensionExtensionRatio = InExtensionRatio;
	Ctx.SteeringAngle = InSteeringAngle;

	PrepareSimulation(Ctx, TargetWheelComponent);
	ComputeRayCastLocation(Ctx, TargetWheelComponent);

	Ctx.HitDistance = InExtensionRatio * Ctx.RayCastLength + TargetWheelComponent->WheelConfig.Radius;

	switch (TargetWheelComponent->SuspensionKinematicsConfig.SuspensionType)
	{
	case ESuspensionType::StraightLine:
		ComputeStraightSuspension(Ctx, TargetWheelComponent);
		break;
	case ESuspensionType::Macpherson:
		ComputeMacpherson(Ctx, TargetWheelComponent);
		break;
	case ESuspensionType::DoubleWishbone:
		ComputeDoubleWishbone(Ctx, TargetWheelComponent);
		break;
	default:
		ComputeStraightSuspension(Ctx, TargetWheelComponent);
		break;
	}

	CopyContextToState(State, Ctx);
}

void FVehicleSuspensionSolver::DrawSuspension(float Duration, float Thickness, bool bDrawSuspension, bool bDrawWheel, bool bDrawRayCast)
{
	if (!IsValid(TargetWheelComponent))return;

	UWorld* TempWorld = TargetWheelComponent->GetWorld();
	if (!IsValid(TempWorld))return;

	const FTransform& CarbodyWorldTrans = TargetWheelComponent->GetCarbodyAsyncWorldTransform();

	FVector WheelRelativePos = FVector(State.KnuckleRelativePos + State.WheelCenterToKnuckle);
	FVector WheelWorldPos = CarbodyWorldTrans.TransformPositionNoScale(WheelRelativePos);
	
	float WheelPos = State.bIsRightWheel ? 1.f : -1.f;

	if (bDrawSuspension)
	{
		FVector TempKnuckleWorldPos = CarbodyWorldTrans.TransformPositionNoScale((FVector)State.KnuckleRelativePos);
		//draw arm
		FTransform WorldTrans = TargetWheelComponent->GetRelativeTransform() * CarbodyWorldTrans;
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
		FVector W = FVector(State.WheelRightVector) * TargetWheelComponent->WheelConfig.Width * 0.5;
		DrawDebugCylinder(TempWorld, WheelWorldPos + W, WheelWorldPos - W,
			TargetWheelComponent->WheelConfig.Radius, 8, FColor(128, 128, 128), false, Duration, 0, Thickness);
	}

	if (bDrawRayCast)
	{
# if 0
		float TempR = TargetWheelComponent->WheelConfig.Radius;
		float TempHalfWidth = TargetWheelComponent->WheelConfig.Width * 0.5;
		float ValidR = FMath::Min(TempR, TempHalfWidth);
		FVector HalfSize = FVector(TempR, TempHalfWidth, TempR) * 0.707;
		FQuat Orientation = Ctx.RayCastTransform.TransformRotation(
			FQuat(0.0f, 0.38268343f, 0.0f, 0.92387953f));	// this is Rotator(0.0, 45.0, 0.0)
		switch (TargetWheelComponent->SuspensionKinematicsConfig.RayCastMode)
		{
		case ESuspensionRayCastMode::LineTrace:
			DrawDebugLine(TempWorld, Ctx.HitStruct.TraceStart, Ctx.HitStruct.TraceEnd, FColor(0, 255, 0), false, Duration, 0, Thickness);
			if (Ctx.bHitGround)DrawDebugPoint(TempWorld, Ctx.HitStruct.Location, Thickness, FColor(255, 0, 0), false, Duration, Thickness);
			break;
		case ESuspensionRayCastMode::SphereTrace:
			//draw capsule
			if (Ctx.bRayCastRefined)TempR = TargetWheelComponent->WheelConfig.Width * 0.5;
			DrawDebugCapsule(TempWorld, (Ctx.HitStruct.TraceStart + Ctx.HitStruct.TraceEnd) * 0.5,
				Ctx.RayCastLength * 0.5 + TempR, TempR, Ctx.RayCastTransform.GetRotation().GetNormalized(), FColor(0, 255, 0), false, Duration, 0, Thickness);
			if (Ctx.bHitGround)DrawDebugSphere(TempWorld, Ctx.HitStruct.Location, TempR, 8, FColor(255, 0, 0), false, Duration, 0, Thickness);
			break;
		case ESuspensionRayCastMode::BoxTrace:
			if (Ctx.bHitGround)DrawDebugBox(TempWorld, Ctx.HitStruct.Location, HalfSize, Orientation, FColor(0, 255, 0), false, Duration, 0, Thickness);
			break;
		case ESuspensionRayCastMode::SphereTraceNoRefinement:
			DrawDebugCapsule(TempWorld, (Ctx.HitStruct.TraceStart + Ctx.HitStruct.TraceEnd) * 0.5,
				FVector::Distance(Ctx.HitStruct.TraceStart, Ctx.HitStruct.TraceEnd) * 0.5 + ValidR,
				ValidR, Ctx.RayCastTransform.GetRotation().GetNormalized(), FColor(0, 255, 0), false, Duration, 0, Thickness);
			if (Ctx.bHitGround)DrawDebugSphere(TempWorld, Ctx.HitStruct.Location, ValidR, 8, FColor(255, 0, 0), false, Duration, 0, Thickness);
			break;
		case ESuspensionRayCastMode::MultiSphereTrace:
			DrawDebugCapsule(TempWorld, (Ctx.HitStruct.TraceStart + Ctx.HitStruct.TraceEnd) * 0.5,
				Ctx.RayCastLength * 0.5 + TempR, TempR, Ctx.RayCastTransform.GetRotation().GetNormalized(), FColor(0, 255, 0), false, Duration, 0, Thickness);
			if (Ctx.bHitGround)DrawDebugSphere(TempWorld, Ctx.HitStruct.Location, TempR, 8, FColor(255, 0, 0), false, Duration, 0, Thickness);
			break;
		default:
			DrawDebugLine(TempWorld, Ctx.HitStruct.TraceStart, Ctx.HitStruct.TraceEnd, FColor(0, 255, 0), false, Duration, 0, Thickness);
			if (Ctx.bHitGround)DrawDebugPoint(TempWorld, Ctx.HitStruct.Location, Thickness, FColor(255, 0, 0), false, Duration, Thickness);
			break;
		}
# endif
	}
}

void FVehicleSuspensionSolver::DrawSuspensionForce(float Duration, float Thickness, float Length)
{
	if (!IsValid(TargetWheelComponent))return;

	UWorld* TempWorld = TargetWheelComponent->GetWorld();
	if (!IsValid(TempWorld))return;

	FColor TempColor = FColor(0, 255, 127);
	if (State.ForceAlongImpactNormal < 0.f)TempColor = FColor(255 - TempColor.R, 255 - TempColor.G, 255 - TempColor.B);

	const FTransform& CarbodyTrans = TargetWheelComponent->GetCarbodyAsyncWorldTransform();

	FVector TempStart = CarbodyTrans.TransformPositionNoScale(FVector(State.TopMountRelativePos));
	FVector TempEnd = TempStart + FVector(State.ImpactNormal) * State.ForceAlongImpactNormal * Length * 0.01;
	DrawDebugLine(TempWorld, TempStart, TempEnd, TempColor, false, Duration, 0, Thickness);
}

bool FVehicleSuspensionSolver::CheckIsDampingDirty()
{
	if (!IsValid(TargetWheelComponent))return false;

	float Stiffness = TargetWheelComponent->SuspensionSpringConfig.SpringStiffness;

	if (CachedSpringStiffness != Stiffness)
	{
		CachedSpringStiffness = Stiffness;
		ComputeCriticalDamping();
		return true;
	}
	else
	{
		return false;
	}
}

bool FVehicleSuspensionSolver::CheckAndFixTriangle()
{
	if(!IsValid(TargetWheelComponent))return false;
	FVehicleSuspensionKinematicsConfig& Config = TargetWheelComponent->SuspensionKinematicsConfig;

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
	float DSqInv = SafeDivide(1, DSq);
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

