// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)


#include "VehicleSuspensionSolver.h"
#include "VehicleWheelComponent.h"
#include "AsyncTickFunctions.h"

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
		
		SimData.WheelPos = FMath::Sign(TargetWheelComponent->GetRelativeTransform().GetLocation().Y);
		
		ApplySuspensionStateDirect();

		//initialize the cache, so that damping will not be Computed twice when game starts
		CachedSpringStiffness  = TargetWheelComponent->SuspensionSpringConfig.SpringStiffness;
	}
	return TargetWheelComponent != nullptr;
}

void FVehicleSuspensionSolver::SetSprungMass(float NewSprungMass)
{
	if (NewSprungMass <= 0)
	{
		SimData.SprungMass = 0;
	}
	else
	{
		SimData.SprungMass = NewSprungMass;
	}

	ComputeCriticalDamping();
}

float FVehicleSuspensionSolver::ComputeCriticalDamping()
{
	if (SimData.SprungMass <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("VehicleSuspensionSolver: SprungMass not valid!"));
		SimData.CriticalDamping = 0.1 * TargetWheelComponent->SuspensionSpringConfig.SpringStiffness;
		return SimData.CriticalDamping;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("VehicleSuspensionSolver: Calculating spring damping!"));
		// critical damping is at point when damping ratio is 1.0
		float NaturalFrequency = FMath::Sqrt(TargetWheelComponent->SuspensionSpringConfig.SpringStiffness * 100 / SimData.SprungMass);
		SimData.CriticalDamping = 0.02 * SimData.SprungMass * NaturalFrequency;
		return SimData.CriticalDamping;
	}
}

void FVehicleSuspensionSolver::UpdateSuspension(
	float InDeltaTime,
	float InSteeringAngle, 
	float InSwaybarForce)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UpdateVehicleSuspensionSolver);

	if (!IsValid(TargetWheelComponent))return;

	SimData.PhysicsDelatTime = InDeltaTime;
	SimData.SteeringAngle = InSteeringAngle;
	SimData.SwaybarForce = InSwaybarForce;

	PrepareSimulation();
	ComputeRayCastLocation();
	SuspensionRayCast();

	switch (TargetWheelComponent->SuspensionKinematicsConfig.SuspensionType)
	{
	case ESuspensionType::StraightLine:
		ComputeStraightSuspension();
		break;
	case ESuspensionType::Macpherson:
		ComputeMacpherson();
		break;
	case ESuspensionType::DoubleWishbone:
		ComputeDoubleWishbone();
		break;
	default:
		ComputeStraightSuspension();
		break;
	}

	ComputeSuspensionForce();
}

void FVehicleSuspensionSolver::ApplySuspensionStateDirect(float InExtensionRatio, float InSteeringAngle)
{
	SimData.SuspensionExtensionRatio = InExtensionRatio;
	SimData.SteeringAngle = InSteeringAngle;

	PrepareSimulation();
	ComputeRayCastLocation();

	SimData.HitDistance = InExtensionRatio * SimData.RayCastLength + TargetWheelComponent->WheelConfig.Radius;

	switch (TargetWheelComponent->SuspensionKinematicsConfig.SuspensionType)
	{
	case ESuspensionType::StraightLine:
		ComputeStraightSuspension();
		break;
	case ESuspensionType::Macpherson:
		ComputeMacpherson();
		break;
	case ESuspensionType::DoubleWishbone:
		ComputeDoubleWishbone();
		break;
	default:
		ComputeStraightSuspension();
		break;
	}
}

void FVehicleSuspensionSolver::DrawSuspension(float Duration, float Thickness, bool bDrawSuspension, bool bDrawWheel, bool bDrawRayCast)
{
	if (!IsValid(TargetWheelComponent))return;

	UWorld* TempWorld = TargetWheelComponent->GetWorld();
	if (!IsValid(TempWorld))return;

	const FTransform& CarbodyWorldTrans = SimData.CarbodyWorldTransform;

	FTransform WheelTrans = SimData.WheelRelativeTransform * CarbodyWorldTrans;

	if (bDrawSuspension)
	{
		FVector TempBallJointWorldPos = CarbodyWorldTrans.TransformPositionNoScale(SimData.BallJointRelativePos);
		//draw arm
		FTransform WorldTrans = SimData.RelativeTransform * CarbodyWorldTrans;
		FVector PivotPos = WorldTrans.TransformPositionNoScale(SuspensionPlaneToZYPlane(FVector2D(0.f)));
		DrawDebugLine(TempWorld, PivotPos, TempBallJointWorldPos, FColor(0, 0, 255), false, Duration, 0, Thickness);
		//draw strut
		DrawDebugLine(TempWorld, CarbodyWorldTrans.TransformPositionNoScale(SimData.TopMountRelativePos), TempBallJointWorldPos, FColor(255, 255, 0), false, Duration, 0, Thickness);
		//draw wheel offset
		DrawDebugLine(TempWorld, TempBallJointWorldPos, WheelTrans.GetLocation(), FColor(0, 255, 255), false, Duration, 0, Thickness);
	}

	if (bDrawWheel)
	{
		//draw cylinder
		FVector W = SimData.WheelRightVector * TargetWheelComponent->WheelConfig.Width * 0.5;
		DrawDebugCylinder(TempWorld, WheelTrans.GetLocation() + W, WheelTrans.GetLocation() - W,
			TargetWheelComponent->WheelConfig.Radius, 8, FColor(128, 128, 128), false, Duration, 0, Thickness);
	}

	if (bDrawRayCast)
	{
		float TempR = TargetWheelComponent->WheelConfig.Radius;
		float TempHalfWidth = TargetWheelComponent->WheelConfig.Width * 0.5;
		float ValidR = FMath::Min(TempR, TempHalfWidth);
		FVector HalfSize = FVector(TempR, TempHalfWidth, TempR) * 0.707;
		FQuat Orientation = SimData.RayCastTransform.TransformRotation(
			FQuat(0.0f, 0.38268343f, 0.0f, 0.92387953f));	// this is Rotator(0.0, 45.0, 0.0)
		switch (TargetWheelComponent->SuspensionKinematicsConfig.RayCastMode)
		{
		case ESuspensionRayCastMode::LineTrace:
			DrawDebugLine(TempWorld, SimData.HitStruct.TraceStart, SimData.HitStruct.TraceEnd, FColor(0, 255, 0), false, Duration, 0, Thickness);
			if (SimData.bHitGround)DrawDebugPoint(TempWorld, SimData.HitStruct.Location, Thickness, FColor(255, 0, 0), false, Duration, Thickness);
			break;
		case ESuspensionRayCastMode::SphereTrace:
			//draw capsule
			if (SimData.bRayCastRefined)TempR = TargetWheelComponent->WheelConfig.Width * 0.5;
			DrawDebugCapsule(TempWorld, (SimData.HitStruct.TraceStart + SimData.HitStruct.TraceEnd) * 0.5,
				SimData.RayCastLength * 0.5 + TempR, TempR, SimData.RayCastTransform.GetRotation().GetNormalized(), FColor(0, 255, 0), false, Duration, 0, Thickness);
			if (SimData.bHitGround)DrawDebugSphere(TempWorld, SimData.HitStruct.Location, TempR, 8, FColor(255, 0, 0), false, Duration, 0, Thickness);
			break;
		case ESuspensionRayCastMode::BoxTrace:
			if (SimData.bHitGround)DrawDebugBox(TempWorld, SimData.HitStruct.Location, HalfSize, Orientation, FColor(0, 255, 0), false, Duration, 0, Thickness);
			break;
		case ESuspensionRayCastMode::SphereTraceNoRefinement:
			DrawDebugCapsule(TempWorld, (SimData.HitStruct.TraceStart + SimData.HitStruct.TraceEnd) * 0.5,
				FVector::Distance(SimData.HitStruct.TraceStart, SimData.HitStruct.TraceEnd) * 0.5 + ValidR,
				ValidR, SimData.RayCastTransform.GetRotation().GetNormalized(), FColor(0, 255, 0), false, Duration, 0, Thickness);
			if (SimData.bHitGround)DrawDebugSphere(TempWorld, SimData.HitStruct.Location, ValidR, 8, FColor(255, 0, 0), false, Duration, 0, Thickness);
			break;
		default:
			DrawDebugLine(TempWorld, SimData.HitStruct.TraceStart, SimData.HitStruct.TraceEnd, FColor(0, 255, 0), false, Duration, 0, Thickness);
			if (SimData.bHitGround)DrawDebugPoint(TempWorld, SimData.HitStruct.Location, Thickness, FColor(255, 0, 0), false, Duration, Thickness);
			break;
		}
	}
}

void FVehicleSuspensionSolver::DrawSuspensionForce(float Duration, float Thickness, float Length)
{
	if (!IsValid(TargetWheelComponent))return;

	UWorld* TempWorld = TargetWheelComponent->GetWorld();
	if (!IsValid(TempWorld))return;

	FColor TempColor = FColor(0, 255, 127);
	if (SimData.SuspensionForce < 0)TempColor = FColor(255 - TempColor.R, 255 - TempColor.G, 255 - TempColor.B);

	const FTransform& CarbodyTrans = SimData.CarbodyWorldTransform;

	FVector TempStart = CarbodyTrans.TransformPositionNoScale(SimData.TopMountRelativePos);
	FVector TempEnd = TempStart + SimData.SuspensionForceVector * Length * 0.01;
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

float FVehicleSuspensionSolver::SafeDivide(float a, float b)
{
	return (FMath::IsNearlyZero(b)) ? 0.0f : a / b;
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

FQuat FVehicleSuspensionSolver::MakeQuatFrom2DVectors(const FVector2D From, const FVector2D To, const FVector Axis)
{
	FVector2D A = From.GetSafeNormal();
	FVector2D B = To.GetSafeNormal();

	float cosTheta = FVector2D::DotProduct(A, B);              // cosθ
	float sinTheta = A.X * B.Y - A.Y * B.X;                    // sinθ（2D 叉积）

	// Clamp for numerical stability
	cosTheta = FMath::Clamp(cosTheta, -1.0f, 1.0f);

	float cosHalf = FMath::Sqrt((1.0f + cosTheta) * 0.5f);    // cos(θ/2)
	float sinHalf = FMath::Sqrt((1.0f - cosTheta) * 0.5f);    // sin(θ/2)
	sinHalf *= FMath::Sign(sinTheta);                         // 保持方向

	FVector axisNorm = Axis.GetSafeNormal();
	return FQuat(
		axisNorm.X * sinHalf,
		axisNorm.Y * sinHalf,
		axisNorm.Z * sinHalf,
		cosHalf
	);
}

bool FVehicleSuspensionSolver::SingleSphereTrace(FVector Start, FVector End, float SphereRadius, FHitResult& OutHit)
{
	/*****GeomSweepSingle*****/
	return FPhysicsInterface::GeomSweepSingle(
		TargetWheelComponent->GetWorld(), FCollisionShape::MakeSphere(SphereRadius), FQuat::Identity, OutHit,
		Start, End, TargetWheelComponent->SuspensionKinematicsConfig.TraceChannel, QueryParams,
		FCollisionResponseParams::DefaultResponseParam, FCollisionObjectQueryParams::DefaultObjectQueryParam);
}

FVector FVehicleSuspensionSolver::SuspensionPlaneToZYPlane(FVector2D V2D)
{
	FVehicleSuspensionKinematicsConfig& Config = TargetWheelComponent->SuspensionKinematicsConfig;
	return FVector(0, (V2D.Y - Config.ArmLength) * SimData.WheelPos, V2D.X);
}

FVector2D FVehicleSuspensionSolver::ZYPlaneToSuspensionPlane(FVector V3D)
{
	FVehicleSuspensionKinematicsConfig& Config = TargetWheelComponent->SuspensionKinematicsConfig;
	return FVector2D(V3D.Z, V3D.Y * SimData.WheelPos + Config.ArmLength);
}

FVector FVehicleSuspensionSolver::GetCamberCasterToeFromCurve()
{
	FVehicleSuspensionKinematicsConfig& Config = TargetWheelComponent->SuspensionKinematicsConfig;

	FVector v;
	v.X = 0.f;
	v.Y = Config.BaseToe;
	v.Z = Config.BaseCamber;

	float Compression = 1 - SimData.SuspensionExtensionRatio;
	if (Config.CasterCurve)v.X += Config.CasterCurve->GetFloatValue(Compression);
	if (Config.ToeCurve)v.Y += Config.ToeCurve->GetFloatValue(Compression);
	if (Config.CamberCurve)v.Z += Config.CamberCurve->GetFloatValue(Compression);
	
	v *= SimData.WheelPos;
	return v;
}

void FVehicleSuspensionSolver::PrepareSimulation()
{
	SimData.RelativeTransform = TargetWheelComponent->GetRelativeTransform();
	SimData.CarbodyWorldTransform = UAsyncTickFunctions::ATP_GetTransform(TargetWheelComponent->GetCarbody());

	//dealing with transforms
	SimData.WheelPos = FMath::Sign(SimData.RelativeTransform.GetLocation().Y);
	SimData.WheelPos += SimData.WheelPos == 0.f;	//if(wheelpos == 0) wheelpos = 1;
	FVector2D TopMountConfig = TargetWheelComponent->SuspensionKinematicsConfig.TopMountPosition;
	SimData.TopMountPos2D = FVector2D(TopMountConfig.Y, TopMountConfig.X + TargetWheelComponent->SuspensionKinematicsConfig.ArmLength);
	SimData.ComponentRelativeForwardVector = SimData.RelativeTransform.GetRotation().GetForwardVector();
}

float FVehicleSuspensionSolver::ComputeValidPreload()
{
	//check suspension preload
	//preload should not be greater than gravity force on the wheel
	float Proj = FVector::DotProduct(FVector(0, 0, 1), SimData.StrutRelativeDirection);
	float MaxPreload = SafeDivide(0.99 * SimData.WorldGravityZ * SimData.SprungMass, Proj);

	return SimData.ValidPreload = FMath::Min(TargetWheelComponent->SuspensionSpringConfig.SpringPreload, MaxPreload);
}

void FVehicleSuspensionSolver::ComputeRayCastLocation()
{
	SimData.StrutDirection2D = (SimData.TopMountPos2D - SimData.BallJointPos2D).GetSafeNormal();
	SimData.RayCastLength = FMath::Abs(SimData.StrutDirection2D.X * TargetWheelComponent->SuspensionKinematicsConfig.Stroke);

	SimData.RayCastStart2D.X = SimData.TopMountPos2D.X - SimData.StrutDirection2D.X * TargetWheelComponent->SuspensionKinematicsConfig.MinStrutLength;
	SimData.RayCastStart2D.Y = SimData.BallJointPos2D.Y;

	FVector RayCastStartLocal = SuspensionPlaneToZYPlane(SimData.RayCastStart2D);
	FVector RayCastStartRelative = SimData.RelativeTransform.TransformPositionNoScale(RayCastStartLocal) + SimData.WheelOffsetToBallJoint;
	SimData.RayCastStartPos = SimData.CarbodyWorldTransform.TransformPositionNoScale(RayCastStartRelative);

	FTransform WorldTrans = SimData.RelativeTransform * SimData.CarbodyWorldTransform;
	SimData.ComponentUpVector = WorldTrans.GetRotation().GetUpVector();
	SimData.RayCastEndPos = SimData.RayCastStartPos - SimData.RayCastLength * SimData.ComponentUpVector;

	FQuat SteeringRot = FQuat(SimData.ComponentUpVector, FMath::DegreesToRadians(SimData.SteeringAngle));
	FQuat RayCastRot = SteeringRot * WorldTrans.GetRotation();

	SimData.RayCastTransform = FTransform(RayCastRot, SimData.RayCastEndPos, FVector(1, 1, 1));
}

void FVehicleSuspensionSolver::SuspensionRayCast()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UpdateVehicleSuspensionRayCast);

	//switch to line trace if the suspension is completly compressed
	//or if the raycastlength is too small
	if (SimData.RayCastLength < 1.f || SimData.HitDistance <= TargetWheelComponent->WheelConfig.Radius)
	{
		SuspensionLineTrace();
	}
	else
	{
		switch (TargetWheelComponent->SuspensionKinematicsConfig.RayCastMode)
		{
		case ESuspensionRayCastMode::LineTrace:
			SuspensionLineTrace();
			break;
		case ESuspensionRayCastMode::SphereTrace:
			SuspensionSphereTrace();
			break;
		case ESuspensionRayCastMode::BoxTrace:
			SuspensionBoxTrace();
			break;
		case ESuspensionRayCastMode::SphereTraceNoRefinement:
			SuspensionSphereTraceNoRefinement();
			break;
		default:
			SuspensionLineTrace();
			break;
		}
	}
}

void FVehicleSuspensionSolver::SuspensionLineTrace()
{
	float WheelRadius = TargetWheelComponent->WheelConfig.Radius;

	FVector LineTraceEnd = SimData.RayCastEndPos - SimData.ComponentUpVector * WheelRadius;

	SimData.bRayCastRefined = false;

	/**************RaycastSingle**************/
	SimData.bHitGround = FPhysicsInterface::RaycastSingle(TargetWheelComponent->GetWorld(), SimData.HitStruct,
		SimData.RayCastStartPos, LineTraceEnd,
		TargetWheelComponent->SuspensionKinematicsConfig.TraceChannel, QueryParams, FCollisionResponseParams::DefaultResponseParam, FCollisionObjectQueryParams::DefaultObjectQueryParam);

	ComputeHitDistance();
}

void FVehicleSuspensionSolver::SuspensionSphereTrace()
{
	FVehicleSuspensionKinematicsConfig& Config = TargetWheelComponent->SuspensionKinematicsConfig;
	float WheelRadius = TargetWheelComponent->WheelConfig.Radius;

	SimData.bRayCastRefined = false;
	SimData.bHitGround = SingleSphereTrace(SimData.RayCastStartPos, SimData.RayCastEndPos, WheelRadius, SimData.HitStruct);

	if (SimData.bHitGround)
	{
		float HalfWidth = TargetWheelComponent->WheelConfig.Width * 0.5;

		FVector LocalImpactPoint = SimData.RayCastTransform.InverseTransformPositionNoScale(SimData.HitStruct.ImpactPoint);
		SimData.bRayCastRefined = FMath::Abs(LocalImpactPoint.Y) > HalfWidth || LocalImpactPoint.Z > Config.Stroke;

		if (SimData.bRayCastRefined)
		{
			FVector SecondStepRayCastStart = SimData.RayCastStartPos + SimData.ComponentUpVector * (HalfWidth - WheelRadius);
			FVector SecondStepRayCastEnd = SecondStepRayCastStart - SimData.ComponentUpVector * SimData.RayCastLength;

			SimData.bHitGround = SingleSphereTrace(SecondStepRayCastStart, SecondStepRayCastEnd, HalfWidth, SimData.HitStruct);
		}
	}

	ComputeHitDistance(WheelRadius);
}

void FVehicleSuspensionSolver::SuspensionBoxTrace()
{
	SimData.bRayCastRefined = false;

	float WheelRadius = TargetWheelComponent->WheelConfig.Radius;
	float WheelHalfWidth = TargetWheelComponent->WheelConfig.Width * 0.5;

	FVector HalfSize = FVector(WheelRadius, WheelHalfWidth, WheelRadius) * 0.707;
	FQuat Orientation = SimData.RayCastTransform.TransformRotation(
		FQuat(0.0f, 0.38268343f, 0.0f, 0.92387953f));	// this is Rotator(0.0, 45.0, 0.0)
	
	/*****GeomSweepSingle*****/
	SimData.bHitGround = FPhysicsInterface::GeomSweepSingle(
		TargetWheelComponent->GetWorld(), 
		FCollisionShape::MakeBox(HalfSize), 
		Orientation, 
		SimData.HitStruct,
		SimData.RayCastStartPos,
		SimData.RayCastEndPos, 
		TargetWheelComponent->SuspensionKinematicsConfig.TraceChannel, 
		QueryParams,
		FCollisionResponseParams::DefaultResponseParam, 
		FCollisionObjectQueryParams::DefaultObjectQueryParam);

	ComputeHitDistance(WheelRadius);

	if (SimData.bHitGround)
	{
		// try to maintain the impact point in the middle of the shape so that it will be more stable
		FVector NormalVec = SimData.CarbodyWorldTransform.GetRotation().GetRightVector();
		FVector RelativeBias = SimData.HitStruct.ImpactPoint - SimData.HitStruct.Location;
		FVector BiasProjection = FVector::VectorPlaneProject(RelativeBias, NormalVec);
		SimData.HitStruct.ImpactPoint = SimData.HitStruct.Location + BiasProjection;
	}
}

void FVehicleSuspensionSolver::SuspensionSphereTraceNoRefinement()
{
	SimData.bRayCastRefined = false;

	float WheelRadius = TargetWheelComponent->WheelConfig.Radius;
	float WheelHalfWidth = TargetWheelComponent->WheelConfig.Width * 0.5;
	float ValidRadius = FMath::Min(WheelRadius, WheelHalfWidth);
	float RadiusDifference = WheelRadius - ValidRadius;
	FVector TraceEnd = SimData.RayCastEndPos - SimData.ComponentUpVector * RadiusDifference;

	SimData.bHitGround = SingleSphereTrace(SimData.RayCastStartPos, TraceEnd, ValidRadius, SimData.HitStruct);

	ComputeHitDistance(ValidRadius);
}

void FVehicleSuspensionSolver::ComputeHitDistance(float EquivalentSphereTraceRadius)
{
	float WheelRadius = TargetWheelComponent->WheelConfig.Radius;
	float TargetHitDistance = SimData.RayCastLength + WheelRadius;

	if (SimData.bHitGround)
	{
		TargetHitDistance = SimData.HitStruct.Distance + EquivalentSphereTraceRadius;
	}

	float LastHitDistance = SimData.HitDistance;
	SimData.HitDistance = TargetHitDistance;
	SimData.HitDistanceRate = SafeDivide(SimData.HitDistance - LastHitDistance, SimData.PhysicsDelatTime);
	float HitDistanceNoBias = FMath::Max(0.f, SimData.HitDistance - WheelRadius);
	SimData.SuspensionExtensionRatio = SafeDivide(HitDistanceNoBias, SimData.RayCastLength);
}

void FVehicleSuspensionSolver::ComputeStraightSuspension()
{
	FVehicleSuspensionKinematicsConfig& Config = TargetWheelComponent->SuspensionKinematicsConfig;

	//Compute the position of the BallJoint on the suspension plane
	float WheelRadius = TargetWheelComponent->WheelConfig.Radius;
	SimData.BallJointPos2D.X = SimData.RayCastStart2D.X - FMath::Max(0.f, SimData.HitDistance - WheelRadius);
	SimData.BallJointPos2D.Y = Config.ArmLength;

	SimData.BallJointRelativePos = SimData.RelativeTransform.TransformPositionNoScale(SuspensionPlaneToZYPlane(SimData.BallJointPos2D));
	SimData.TopMountRelativePos = SimData.RelativeTransform.TransformPositionNoScale(SuspensionPlaneToZYPlane(SimData.TopMountPos2D));

	SimData.StrutRelativeDirection = SimData.RelativeTransform.GetRotation().GetUpVector();
	FQuat SteeringBiasRotation = FQuat(SimData.StrutRelativeDirection, FMath::DegreesToRadians(SimData.SteeringAngle));

	FQuat InitialWheelRelativeRotation = FQuat(FRotator(0.f, Config.BaseToe * SimData.WheelPos, Config.BaseCamber * SimData.WheelPos));
	SimData.WheelRelativeTransform.SetRotation(SteeringBiasRotation * InitialWheelRelativeRotation);
	FVector WheelRelativeRightVec = SimData.WheelRelativeTransform.GetRotation().GetRightVector();

	SimData.WheelOffsetToBallJoint = Config.SteeringAxleOffset * SimData.WheelPos * WheelRelativeRightVec;
	SimData.WheelRelativeTransform.SetLocation(SimData.BallJointRelativePos + SimData.WheelOffsetToBallJoint);

	FVector RelativeRightVec = SimData.WheelRelativeTransform.GetRotation().GetRightVector();
	SimData.WheelRightVector = SimData.CarbodyWorldTransform.TransformVectorNoScale(RelativeRightVec);

	SimData.WheelWorldPos = SimData.CarbodyWorldTransform.TransformPositionNoScale(SimData.WheelRelativeTransform.GetLocation());
}

void FVehicleSuspensionSolver::ComputeMacpherson()
{
	FVehicleSuspensionKinematicsConfig& Config = TargetWheelComponent->SuspensionKinematicsConfig;

	//Compute the position of the BallJoint on the suspension plane
	float WheelRadius = TargetWheelComponent->WheelConfig.Radius;
	SimData.BallJointPos2D.X = SimData.RayCastStart2D.X - FMath::Max(0.f, SimData.HitDistance - WheelRadius);
	SimData.BallJointPos2D.X = FMath::Clamp(SimData.BallJointPos2D.X, -Config.ArmLength, Config.ArmLength);
	SimData.BallJointPos2D.Y = FMath::Sqrt(FMath::Square(Config.ArmLength) - FMath::Square(SimData.BallJointPos2D.X));

	SimData.BallJointRelativePos = SimData.RelativeTransform.TransformPositionNoScale(SuspensionPlaneToZYPlane(SimData.BallJointPos2D));
	SimData.TopMountRelativePos = SimData.RelativeTransform.TransformPositionNoScale(SuspensionPlaneToZYPlane(SimData.TopMountPos2D));

	FVector2D InitialStrutDirection = SimData.TopMountPos2D - FVector2D(0, Config.ArmLength);
	FVector2D CurrentStrutDirection = SimData.TopMountPos2D - SimData.BallJointPos2D;
	InitialStrutDirection.Y *= -SimData.WheelPos;
	CurrentStrutDirection.Y *= -SimData.WheelPos;
	FQuat StrutBiasRotation = MakeQuatFrom2DVectors(InitialStrutDirection, CurrentStrutDirection, SimData.ComponentRelativeForwardVector);

	SimData.StrutRelativeDirection = (SimData.TopMountRelativePos - SimData.BallJointRelativePos).GetSafeNormal();
	FQuat SteeringBiasRotation = FQuat(SimData.StrutRelativeDirection, FMath::DegreesToRadians(SimData.SteeringAngle));

	FQuat InitialWheelRelativeRotation = FQuat(FRotator(0, Config.BaseToe * SimData.WheelPos, Config.BaseCamber * SimData.WheelPos));
	SimData.WheelRelativeTransform.SetRotation(SteeringBiasRotation	* StrutBiasRotation	* InitialWheelRelativeRotation);
	FVector WheelRelativeRightVec = SimData.WheelRelativeTransform.GetRotation().GetRightVector();

	SimData.WheelOffsetToBallJoint = Config.SteeringAxleOffset * SimData.WheelPos * WheelRelativeRightVec;
	SimData.WheelRelativeTransform.SetLocation(SimData.BallJointRelativePos + SimData.WheelOffsetToBallJoint);

	FVector RelativeRightVec = SimData.WheelRelativeTransform.GetRotation().GetRightVector();
	SimData.WheelRightVector = SimData.CarbodyWorldTransform.TransformVectorNoScale(RelativeRightVec);

	SimData.WheelWorldPos = SimData.CarbodyWorldTransform.TransformPositionNoScale(SimData.WheelRelativeTransform.GetLocation());
}

void FVehicleSuspensionSolver::ComputeDoubleWishbone()
{
	FVehicleSuspensionKinematicsConfig& Config = TargetWheelComponent->SuspensionKinematicsConfig;

	//Compute the position of the BallJoint on the suspension plane
	float WheelRadius = TargetWheelComponent->WheelConfig.Radius;
	SimData.BallJointPos2D.X = SimData.RayCastStart2D.X - FMath::Max(0.f, SimData.HitDistance - WheelRadius);
	SimData.BallJointPos2D.X = FMath::Clamp(SimData.BallJointPos2D.X, -Config.ArmLength, Config.ArmLength);
	SimData.BallJointPos2D.Y = FMath::Sqrt(FMath::Square(Config.ArmLength) - FMath::Square(SimData.BallJointPos2D.X));

	SimData.BallJointRelativePos = SimData.RelativeTransform.TransformPositionNoScale(SuspensionPlaneToZYPlane(SimData.BallJointPos2D));
	SimData.TopMountRelativePos = SimData.RelativeTransform.TransformPositionNoScale(SuspensionPlaneToZYPlane(SimData.TopMountPos2D));

	SimData.StrutRelativeDirection = (SimData.TopMountRelativePos - SimData.BallJointRelativePos).GetSafeNormal();
	FQuat SteeringBiasRotation = FQuat(SimData.RelativeTransform.GetRotation().GetUpVector(), FMath::DegreesToRadians(SimData.SteeringAngle));

	FVector WheelAlignmentEuler = GetCamberCasterToeFromCurve();
	FQuat InitialWheelRelativeRotation = FQuat(FRotator(WheelAlignmentEuler.X, WheelAlignmentEuler.Y, WheelAlignmentEuler.Z));
	SimData.WheelRelativeTransform.SetRotation(SteeringBiasRotation * InitialWheelRelativeRotation);
	FVector WheelRelativeRightVec = SimData.WheelRelativeTransform.GetRotation().GetRightVector();

	SimData.WheelOffsetToBallJoint = Config.SteeringAxleOffset * SimData.WheelPos * WheelRelativeRightVec;
	SimData.WheelRelativeTransform.SetLocation(SimData.BallJointRelativePos + SimData.WheelOffsetToBallJoint);

	FVector RelativeRightVec = SimData.WheelRelativeTransform.GetRotation().GetRightVector();
	SimData.WheelRightVector = SimData.CarbodyWorldTransform.TransformVectorNoScale(RelativeRightVec);

	SimData.WheelWorldPos = SimData.CarbodyWorldTransform.TransformPositionNoScale(SimData.WheelRelativeTransform.GetLocation());
}

void FVehicleSuspensionSolver::ComputeSuspensionForce()
{
	FVehicleSuspensionSpringConfig& Config = TargetWheelComponent->SuspensionSpringConfig;

	float SuspensionStroke = TargetWheelComponent->SuspensionKinematicsConfig.Stroke;
	float WheelRadius = TargetWheelComponent->WheelConfig.Radius;

	float LastLength = SimData.SuspensionCurrentLength;

	SimData.SuspensionCurrentLength = SuspensionStroke * SimData.SuspensionExtensionRatio;

	ComputeValidPreload();

	float Frequency = SafeDivide(1, SimData.PhysicsDelatTime);

	float SpringForce = Config.SpringStiffness * (SuspensionStroke - SimData.SuspensionCurrentLength) + SimData.ValidPreload + SimData.SwaybarForce;

	float DampingRatio = (SimData.SuspensionCurrentLength > LastLength) ? Config.ReboundDampingRatio : Config.CompressionDampingRatio;
	float DamperStiffness = SimData.CriticalDamping * DampingRatio;

	float DampingForce = DamperStiffness * (LastLength - SimData.SuspensionCurrentLength) * Frequency;

	//if the suspension is completely compressed
	if (SimData.HitDistance <= WheelRadius)
	{
		//Compute the max stiffness
		float Omega = Frequency * 0.5;
		float MaxStiffness = Omega * Omega * SimData.SprungMass * 0.01;
		float MaxDamping = 0.02 * Omega * SimData.SprungMass;	//critical damping

		float Stiffness = FMath::Lerp(Config.SpringStiffness, MaxStiffness, Config.BottomOutStiffness);
		float Damping = FMath::Lerp(DamperStiffness, MaxDamping, Config.BottomOutStiffness);

		SpringForce += (WheelRadius - SimData.HitDistance) * Stiffness;
		DampingForce += -SimData.HitDistanceRate * Damping;
	}

	SimData.SuspensionForce = SpringForce + DampingForce;

	SimData.StrutDirection = SimData.CarbodyWorldTransform.TransformVectorNoScale(SimData.StrutRelativeDirection);
	SimData.SuspensionForceVector = SimData.StrutDirection * SimData.SuspensionForce;

	float Fn = FVector::DotProduct(SimData.SuspensionForceVector, SimData.HitStruct.ImpactNormal);
	SimData.WheelLoad = FMath::Max(0, Fn);
	
	if (!SimData.bHitGround)
	{
		SimData.WheelLoad = 0;
	}	
}
