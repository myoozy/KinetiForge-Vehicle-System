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
		QueryParams.bReturnFaceIndex = false;
		QueryParams.bTraceComplex = false;
		
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

void FVehicleSuspensionSolver::StartUpdateSolidAxle(float InSteeringAngle)
{
	SimData.SteeringAngle = InSteeringAngle;
	PrepareSimulation();
	ComputeRayCastLocation();
	SuspensionRayCast();
}

void FVehicleSuspensionSolver::FinalizeUpdateSolidAxle(
	float InDeltaTime, 
	float InSwaybarForce,
	const FVector& InKnuckleWorldPos,
	const FVector& InAxleWorldDirection)
{
	SimData.PhysicsDelatTime = InDeltaTime;
	SimData.SwaybarForce = InSwaybarForce;

	// get relative position of the ball joint
	SimData.KnuckleRelativePos = SimData.CarbodyWorldTransform.InverseTransformPositionNoScale(InKnuckleWorldPos);
	
	// the relative direction of the axle, which is also the right vector of the wheel
	// because solid axle usually does not have camber/toe
	FVector AxleDirection = SimData.CarbodyWorldTransform.InverseTransformVectorNoScale(InAxleWorldDirection);
	SimData.WheelOffsetToKnuckle = TargetWheelComponent->SuspensionKinematicsConfig.AxialHubOffset * SimData.WheelPos * AxleDirection;
	
	// get relative position of the wheel
	FVector WheelRelativePos = SimData.KnuckleRelativePos + SimData.WheelOffsetToKnuckle;
	
	// get the relative rotation of the wheel
	FVector DefaultRight = FVector(0.f, 1.f, 0.f);
	FQuat AxleRotation = FQuat::FindBetweenNormals(DefaultRight, AxleDirection);
	FVector SteeringAxle = FVector::CrossProduct(SimData.ComponentRelativeForwardVector, AxleDirection);
	FQuat SteeringBiasRotation = FQuat(SteeringAxle, FMath::DegreesToRadians(SimData.SteeringAngle));

	SimData.WheelRelativeTransform = FTransform(SteeringBiasRotation * AxleRotation, WheelRelativePos);
	FVector WheelRelativeRightVec = SimData.WheelRelativeTransform.GetRotation().GetRightVector();
	SimData.WheelRightVector = SimData.CarbodyWorldTransform.TransformVectorNoScale(WheelRelativeRightVec);

	SimData.WheelWorldPos = SimData.CarbodyWorldTransform.TransformPositionNoScale(SimData.WheelRelativeTransform.GetLocation());

	// the relative direction of the strut
	SimData.StrutRelativeDirection = (SimData.TopMountRelativePos - SimData.KnuckleRelativePos).GetSafeNormal();

	// have to calculate the 2d position of the ball joint, to decide the raycast start for next frame
	FVector KnuckleLocalPos = SimData.RelativeTransform.InverseTransformPositionNoScale(SimData.KnuckleRelativePos);
	SimData.KnucklePos2D = ZYPlaneToSuspensionPlane(KnuckleLocalPos);

	// get suspension force
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
		FVector TempKnuckleWorldPos = CarbodyWorldTrans.TransformPositionNoScale(SimData.KnuckleRelativePos);
		//draw arm
		FTransform WorldTrans = SimData.RelativeTransform * CarbodyWorldTrans;
		FVector PivotPos = WorldTrans.TransformPositionNoScale(SuspensionPlaneToZYPlane(FVector2D(0.f)));
		DrawDebugLine(TempWorld, PivotPos, TempKnuckleWorldPos, FColor(0, 0, 255), false, Duration, 0, Thickness);
		//draw strut
		DrawDebugLine(TempWorld, CarbodyWorldTrans.TransformPositionNoScale(SimData.TopMountRelativePos), TempKnuckleWorldPos, FColor(255, 255, 0), false, Duration, 0, Thickness);
		//draw wheel offset
		DrawDebugLine(TempWorld, TempKnuckleWorldPos, WheelTrans.GetLocation(), FColor(0, 255, 255), false, Duration, 0, Thickness);
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
		case ESuspensionRayCastMode::MultiSphereTrace:
			DrawDebugCapsule(TempWorld, (SimData.HitStruct.TraceStart + SimData.HitStruct.TraceEnd) * 0.5,
				SimData.RayCastLength * 0.5 + TempR, TempR, SimData.RayCastTransform.GetRotation().GetNormalized(), FColor(0, 255, 0), false, Duration, 0, Thickness);
			if (SimData.bHitGround)DrawDebugSphere(TempWorld, SimData.HitStruct.Location, TempR, 8, FColor(255, 0, 0), false, Duration, 0, Thickness);
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
	return FVector(0, V2D.Y * SimData.WheelPos, V2D.X);
}

FVector2D FVehicleSuspensionSolver::ZYPlaneToSuspensionPlane(FVector V3D)
{
	return FVector2D(V3D.Z, V3D.Y * SimData.WheelPos);
}

FVector FVehicleSuspensionSolver::GetCamberToeCasterFromCurve()
{
	FVehicleSuspensionKinematicsConfig& Config = TargetWheelComponent->SuspensionKinematicsConfig;

	FVector v;
	v.X = 0.f;
	v.Y = Config.BaseToe;
	v.Z = Config.BaseCamber;

	float Compression = 1 - SimData.SuspensionExtensionRatio;
	if (Config.ToeCurve)v.Y += Config.ToeCurve->GetFloatValue(Compression);
	if (Config.CamberCurve)v.Z += Config.CamberCurve->GetFloatValue(Compression);
	
	// flip the caster and toe if necessary
	v *= SimData.WheelPos;

	if (Config.CasterCurve) v.X = Config.CasterCurve->GetFloatValue(Compression);

	return v;
}

void FVehicleSuspensionSolver::PrepareSimulation()
{
	SimData.RelativeTransform = TargetWheelComponent->GetRelativeTransform();
	SimData.CarbodyWorldTransform = UAsyncTickFunctions::ATP_GetTransform(TargetWheelComponent->GetCarbody());

	//dealing with transforms
	SimData.WheelPos = FMath::Sign(SimData.RelativeTransform.GetLocation().Y);
	SimData.WheelPos += SimData.WheelPos == 0.f;	//if(wheelpos == 0) wheelpos = 1;
	FVector TopMountLocalPos = TargetWheelComponent->SuspensionKinematicsConfig.TopMountPosition;
	TopMountLocalPos.Y *= SimData.WheelPos;
	SimData.TopMountPos2D = ZYPlaneToSuspensionPlane(TopMountLocalPos);
	SimData.TopMountRelativePos = SimData.RelativeTransform.TransformPositionNoScale(TopMountLocalPos);
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
	SimData.StrutDirection2D = (SimData.TopMountPos2D - SimData.KnucklePos2D).GetSafeNormal();
	SimData.RayCastLength = FMath::Abs(SimData.StrutDirection2D.X * TargetWheelComponent->SuspensionKinematicsConfig.Stroke);

	SimData.RayCastStart2D.X = SimData.TopMountPos2D.X - SimData.StrutDirection2D.X * TargetWheelComponent->SuspensionKinematicsConfig.MinStrutLength;
	SimData.RayCastStart2D.Y = SimData.KnucklePos2D.Y;

	FVector RayCastStartLocal = SuspensionPlaneToZYPlane(SimData.RayCastStart2D);
	FVector RayCastStartRelative = SimData.RelativeTransform.TransformPositionNoScale(RayCastStartLocal) + SimData.WheelOffsetToKnuckle;
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

	// savety check
	if (SimData.RayCastStartPos.ContainsNaN() 
		|| SimData.RayCastEndPos.ContainsNaN()
		|| !IsValid(TargetWheelComponent->GetWorld()))
	{
		// reset the suspension
		SimData.bHitGround = false;
		ComputeHitDistance();
		return;
	}

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
		case ESuspensionRayCastMode::MultiSphereTrace:
			SuspensionMultiSphereTrace();
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

void FVehicleSuspensionSolver::SuspensionMultiSphereTrace()
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
			// first test with a bigger box
			FVector HalfSize = FVector(WheelRadius, HalfWidth, WheelRadius);
			bool bShouldMultiSphereTrace = FPhysicsInterface::GeomSweepSingle(
				TargetWheelComponent->GetWorld(),
				FCollisionShape::MakeBox(HalfSize),
				SimData.RayCastTransform.GetRotation(),
				SimData.HitStruct,
				SimData.RayCastStartPos,
				SimData.RayCastEndPos,
				TargetWheelComponent->SuspensionKinematicsConfig.TraceChannel,
				QueryParams,
				FCollisionResponseParams::DefaultResponseParam,
				FCollisionObjectQueryParams::DefaultObjectQueryParam);

			// then if there is someting inside the box, we do multi-sphere trace
			if (bShouldMultiSphereTrace)
			{
				TArray<FHitResult> MultiHitResults;
				SimData.bHitGround = FPhysicsInterface::GeomSweepMulti(
					TargetWheelComponent->GetWorld(),
					FCollisionShape::MakeSphere(WheelRadius),
					FQuat::Identity,
					MultiHitResults,
					SimData.RayCastStartPos,
					SimData.RayCastEndPos,
					TargetWheelComponent->SuspensionKinematicsConfig.TraceChannel,
					QueryParams,
					FCollisionResponseParams::DefaultResponseParam,
					FCollisionObjectQueryParams::DefaultObjectQueryParam
				);

				if (SimData.bHitGround)
				{
					int32 ArrayLength = MultiHitResults.Num();
					SimData.HitStruct = MultiHitResults[ArrayLength - 1];
				}
			}			
		}
	}

	ComputeHitDistance(WheelRadius);
}

void FVehicleSuspensionSolver::ComputeHitDistance(float EquivalentSphereTraceRadius)
{
	float WheelRadius = TargetWheelComponent->WheelConfig.Radius;
	float TargetHitDistance = SimData.bHitGround ? 
		SimData.HitStruct.Distance + EquivalentSphereTraceRadius : SimData.RayCastLength + WheelRadius;

	if (TargetHitDistance <= WheelRadius || SimData.HitDistance <= WheelRadius)
	{
		// if the suspension is completely compressed
		// we can smoothen the hit distance
		float UnDampedFrequency = 25.f;
		float DampingRatio = 1.f;
		FMath::SpringDamper(
			SimData.HitDistance,
			SimData.HitDistanceRate,
			TargetHitDistance,
			0.f,
			SimData.PhysicsDelatTime,
			UnDampedFrequency,
			DampingRatio
		);
	}
	else
	{
		float LastHitDistance = SimData.HitDistance;
		SimData.HitDistance = TargetHitDistance;
		SimData.HitDistanceRate = SafeDivide(SimData.HitDistance - LastHitDistance, SimData.PhysicsDelatTime);
	}

	float HitDistanceNoBias = FMath::Max(0.f, SimData.HitDistance - WheelRadius);
	SimData.SuspensionExtensionRatio = SafeDivide(HitDistanceNoBias, SimData.RayCastLength);
}

void FVehicleSuspensionSolver::IterateKnucklePos()
{
	// this function suppose to simulate the unsprung mass
	// but I haven't finished it yet

	int SubSteps = 10;
	float UnSprungMass = 35.f;
	float TireStiffness = 2000.f;
	float TireDamping = 200.f;

	if (SubSteps > 0)
	{
		float SubDeltaTime = SimData.PhysicsDelatTime / SubSteps;
		float WheelRadius = TargetWheelComponent->WheelConfig.Radius;
		float GroundPosisitonX = SimData.RayCastStart2D.X - SimData.HitDistance;
		// the tire displacement and displacement rate is not accurate
		float TireDisplacement = FMath::Max(0.f, GroundPosisitonX + WheelRadius - SimData.KnucklePos2D.X);
		float TireDisplacementRate = SimData.HitDistanceRate;
		float TireForce = TireDisplacement * TireStiffness - TireDisplacementRate * TireDamping;

		FTransform ComponentWorldTransform = SimData.CarbodyWorldTransform * SimData.RelativeTransform;
		FVector KnuckleWorldPos = SimData.CarbodyWorldTransform.TransformPositionNoScale(SimData.KnuckleRelativePos);
		//FVector KnuckleWorldVel = SimData.CarbodyWorldTransform.TransformPositionNoScale(SimData.KnuckleRelativeVel);
		FVector TopMountWorldPos = SimData.CarbodyWorldTransform.TransformPositionNoScale(SimData.TopMountRelativePos);
		FVector PivotWorldPos = ComponentWorldTransform.TransformPositionNoScale(SuspensionPlaneToZYPlane(SimData.TopMountPos2D));

		// take velocity of carbody into account
		UPrimitiveComponent* Carbody = TargetWheelComponent->GetCarbody();
		FVector CarbodyVelocity = UAsyncTickFunctions::ATP_GetLinearVelocityAtPoint(Carbody, KnuckleWorldPos);
		//KnuckleWorldVel += CarbodyVelocity;
		
		// itteration in world space
		for (int i = 0; i < SubSteps; i++)
		{

		}

		//KnuckleWorldVel -= CarbodyVelocity;
		SimData.KnuckleRelativePos = SimData.CarbodyWorldTransform.InverseTransformPositionNoScale(KnuckleWorldPos);
		//SimData.KnuckleRelativeVel = SimData.CarbodyWorldTransform.InverseTransformPositionNoScale(KnuckleWorldVel);
	}
}

void FVehicleSuspensionSolver::ComputeStraightSuspension()
{
	FVehicleSuspensionKinematicsConfig& Config = TargetWheelComponent->SuspensionKinematicsConfig;

	//Compute the position of the Knuckle on the suspension plane
	float WheelRadius = TargetWheelComponent->WheelConfig.Radius;
	SimData.KnucklePos2D.X = SimData.RayCastStart2D.X - FMath::Max(0.f, SimData.HitDistance - WheelRadius);
	SimData.KnucklePos2D.Y = Config.ArmLength;

	SimData.KnuckleRelativePos = SimData.RelativeTransform.TransformPositionNoScale(SuspensionPlaneToZYPlane(SimData.KnucklePos2D));

	SimData.StrutRelativeDirection = SimData.RelativeTransform.GetRotation().GetUpVector();
	
	// the kingpin(steering axis) is the up vector
	FQuat SteeringBiasRotation = FQuat(SimData.StrutRelativeDirection, FMath::DegreesToRadians(SimData.SteeringAngle));

	FQuat InitialWheelRelativeRotation = FQuat(FRotator(0.f, Config.BaseToe * SimData.WheelPos, Config.BaseCamber * SimData.WheelPos));
	SimData.WheelRelativeTransform.SetRotation(SteeringBiasRotation * InitialWheelRelativeRotation);
	FVector WheelRelativeRightVec = SimData.WheelRelativeTransform.GetRotation().GetRightVector();

	SimData.WheelOffsetToKnuckle = Config.AxialHubOffset * SimData.WheelPos * WheelRelativeRightVec;
	SimData.WheelRelativeTransform.SetLocation(SimData.KnuckleRelativePos + SimData.WheelOffsetToKnuckle);

	SimData.WheelRightVector = SimData.CarbodyWorldTransform.TransformVectorNoScale(WheelRelativeRightVec);

	SimData.WheelWorldPos = SimData.CarbodyWorldTransform.TransformPositionNoScale(SimData.WheelRelativeTransform.GetLocation());
}

void FVehicleSuspensionSolver::ComputeMacpherson()
{
	FVehicleSuspensionKinematicsConfig& Config = TargetWheelComponent->SuspensionKinematicsConfig;

	//Compute the position of the Knuckle on the suspension plane
	float WheelRadius = TargetWheelComponent->WheelConfig.Radius;
	SimData.KnucklePos2D.X = SimData.RayCastStart2D.X - FMath::Max(0.f, SimData.HitDistance - WheelRadius);
	SimData.KnucklePos2D.X = FMath::Clamp(SimData.KnucklePos2D.X, -Config.ArmLength, Config.ArmLength);
	SimData.KnucklePos2D.Y = FMath::Sqrt(FMath::Square(Config.ArmLength) - FMath::Square(SimData.KnucklePos2D.X));

	SimData.KnuckleRelativePos = SimData.RelativeTransform.TransformPositionNoScale(SuspensionPlaneToZYPlane(SimData.KnucklePos2D));

	FVector InitialKnuckleRelativePos = SimData.RelativeTransform.TransformPositionNoScale(SuspensionPlaneToZYPlane(FVector2D(0, Config.ArmLength)));
	FVector InitialStrutDirection = (SimData.TopMountRelativePos - InitialKnuckleRelativePos).GetSafeNormal();
	SimData.StrutRelativeDirection = (SimData.TopMountRelativePos - SimData.KnuckleRelativePos).GetSafeNormal();
	FQuat StrutBiasRotation = FQuat::FindBetweenNormals(InitialStrutDirection, SimData.StrutRelativeDirection);

	// the kingpin(steering axis) is the strut, for simplification
	FQuat SteeringBiasRotation = FQuat(SimData.StrutRelativeDirection, FMath::DegreesToRadians(SimData.SteeringAngle));

	FQuat InitialWheelRelativeRotation = FQuat(FRotator(0, Config.BaseToe * SimData.WheelPos, Config.BaseCamber * SimData.WheelPos));
	SimData.WheelRelativeTransform.SetRotation(SteeringBiasRotation	* StrutBiasRotation	* InitialWheelRelativeRotation);
	FVector WheelRelativeRightVec = SimData.WheelRelativeTransform.GetRotation().GetRightVector();

	SimData.WheelOffsetToKnuckle = Config.AxialHubOffset * SimData.WheelPos * WheelRelativeRightVec;
	SimData.WheelRelativeTransform.SetLocation(SimData.KnuckleRelativePos + SimData.WheelOffsetToKnuckle);

	SimData.WheelRightVector = SimData.CarbodyWorldTransform.TransformVectorNoScale(WheelRelativeRightVec);

	SimData.WheelWorldPos = SimData.CarbodyWorldTransform.TransformPositionNoScale(SimData.WheelRelativeTransform.GetLocation());
}

void FVehicleSuspensionSolver::ComputeDoubleWishbone()
{
	FVehicleSuspensionKinematicsConfig& Config = TargetWheelComponent->SuspensionKinematicsConfig;

	//Compute the position of the Knuckle on the suspension plane
	float WheelRadius = TargetWheelComponent->WheelConfig.Radius;
	SimData.KnucklePos2D.X = SimData.RayCastStart2D.X - FMath::Max(0.f, SimData.HitDistance - WheelRadius);
	SimData.KnucklePos2D.X = FMath::Clamp(SimData.KnucklePos2D.X, -Config.ArmLength, Config.ArmLength);
	SimData.KnucklePos2D.Y = FMath::Sqrt(FMath::Square(Config.ArmLength) - FMath::Square(SimData.KnucklePos2D.X));

	SimData.KnuckleRelativePos = SimData.RelativeTransform.TransformPositionNoScale(SuspensionPlaneToZYPlane(SimData.KnucklePos2D));
	SimData.TopMountRelativePos = SimData.RelativeTransform.TransformPositionNoScale(SuspensionPlaneToZYPlane(SimData.TopMountPos2D));

	SimData.StrutRelativeDirection = (SimData.TopMountRelativePos - SimData.KnuckleRelativePos).GetSafeNormal();

	FVector WheelAlignmentEuler = GetCamberToeCasterFromCurve();
	FQuat BaseWheelRelativeRotation = FQuat(FRotator(WheelAlignmentEuler.X, WheelAlignmentEuler.Y, WheelAlignmentEuler.Z));
	
	// the kingpin(steering axis) is the upvector of BaseWheelRelativeRotation
	FQuat SteeringBiasRotation = FQuat(BaseWheelRelativeRotation.GetUpVector(), FMath::DegreesToRadians(SimData.SteeringAngle));

	
	SimData.WheelRelativeTransform.SetRotation(SteeringBiasRotation * BaseWheelRelativeRotation);
	FVector WheelRelativeRightVec = SimData.WheelRelativeTransform.GetRotation().GetRightVector();

	SimData.WheelOffsetToKnuckle = Config.AxialHubOffset * SimData.WheelPos * WheelRelativeRightVec;
	SimData.WheelRelativeTransform.SetLocation(SimData.KnuckleRelativePos + SimData.WheelOffsetToKnuckle);

	SimData.WheelRightVector = SimData.CarbodyWorldTransform.TransformVectorNoScale(WheelRelativeRightVec);

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
		DampingForce -= SimData.HitDistanceRate * Damping;
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
