// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)


#include "VehicleSuspensionSolver.h"
#include "VehicleWheelComponent.h"
#include "AsyncTickFunctions.h"
#include "VehicleUtilities.h"

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
		UpdateCachedLUTs(WheelComponent->GetSuspensionKinematicsConfig());

		QueryParams.AddIgnoredActor(WheelComponent->GetOwner());
		QueryParams.bReturnPhysicalMaterial = true;
		QueryParams.bReturnFaceIndex = false;
		QueryParams.bTraceComplex = false;
		
		State.bIsRightWheel = WheelComponent->GetRelativeTransform().GetLocation().Y >= 0.f;
		
		State = SolveKinematicsAtExtension(
			WheelComponent->GetWheelRadius(),
			WheelComponent->GetSuspensionKinematicsConfig(),
			WheelComponent->GetRelativeTransform(),
			1.f,
			0.f,
			2
		);

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
		State.StaticSprungMass = 0;
	}
	else
	{
		State.StaticSprungMass = NewSprungMass;
	}
}

void FVehicleSuspensionSolver::UpdateSuspension(
	const float WheelRadius,
	const float WheelWidth,
	const FVector3f& TireForce,
	const FVehicleSuspensionKinematicsConfig& KineConfig,
	const FVehicleSuspensionSpringConfig& SpringConfig,
	const FTransform& ComponentRelativeTransform,
	const FTransform& AsyncChassisWorldTransform,
	const UWorld* CurrentWorld,
	Chaos::FRigidBodyHandle_Internal* ChassisHandle,
	float InDeltaTime,
	float InSteeringAngle, 
	float InSwaybarForce)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_Wheel_SuspensionSolver_UpdateSuspension);

	FVehicleSuspensionSimContext Ctx;

	CopyStateToContext(State, Ctx);

	Ctx.PhysicsDeltaTime = InDeltaTime;
	Ctx.SteeringAngle = InSteeringAngle;
	Ctx.SwaybarForce = InSwaybarForce;

	PrepareSimulation(
		Ctx,
		ComponentRelativeTransform,
		AsyncChassisWorldTransform,
		KineConfig
	);

	ComputeRayCastLocation(Ctx, KineConfig);

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
	default:
	case EVehicleIndependentSuspensionType::StraightLine:
		ComputeStraightSuspension(Ctx, WheelRadius, KineConfig, CachedLUTs);
		break;
	case EVehicleIndependentSuspensionType::Macpherson:
		ComputeMacpherson(Ctx, WheelRadius, KineConfig, CachedLUTs);
		break;
	case EVehicleIndependentSuspensionType::DoubleWishbone:
		ComputeDoubleWishbone(Ctx, WheelRadius, KineConfig, CachedLUTs);
		break;
	}

	FVehicleChassisSimState ChassisState;

	FetchChassisPhysicsState(
		ChassisHandle,
		ChassisState
	);

	CalculateImpactPointWorldVelocity(
		Ctx, 
		ChassisState
	);

	ComputeAntiPitchRollGeometry(
		Ctx,
		ChassisHandle,
		false,
		KineConfig.SuspensionType,
		WheelRadius,
		CachedLUTs,
		AsyncChassisWorldTransform,
		TireForce
	);

	ComputeSuspensionForce(
		Ctx, 
		WheelRadius,
		ChassisState,
		SpringConfig, 
		KineConfig, 
		CachedLUTs
	);

	CopyContextToState(Ctx, State);
	CopyContextToHitResult(Ctx, RayCastResult);
}

void FVehicleSuspensionSolver::StartUpdateSolidAxle(
	const float WheelRadius,
	const float WheelWidth,
	const FVehicleSuspensionKinematicsConfig& KineConfig,
	const FTransform& ComponentRelativeTransform,
	const FTransform& AsyncChassisWorldTransform,
	const UWorld* CurrentWorld,
	float InSteeringAngle,
	FVector& OutHitWorldLocation,
	FVehicleSuspensionSimContext& Ctx)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_Wheel_SuspensionSolver_UpdateSuspension);

	CopyStateToContext(State, Ctx);

	Ctx.SteeringAngle = InSteeringAngle;

	PrepareSimulation(
		Ctx,
		ComponentRelativeTransform,
		AsyncChassisWorldTransform,
		KineConfig
	);

	ComputeRayCastLocation(Ctx, KineConfig);
	SuspensionRayCast(Ctx, CurrentWorld, WheelRadius, WheelWidth * 0.5f, QueryParams, ResponseParams, KineConfig);

	float DistanceToRayCastStart = FMath::Max(0.f, Ctx.HitDistance - WheelRadius);
	FVector RayDir = (Ctx.RayCastStartWorldLocation - Ctx.RayCastEndWorldLocation).GetSafeNormal();
	FVector HubOffsetWorld = AsyncChassisWorldTransform.TransformVectorNoScale((FVector)Ctx.HubOffsetFromLowerJointChassis);
	OutHitWorldLocation = Ctx.RayCastStartWorldLocation - RayDir * DistanceToRayCastStart - HubOffsetWorld;
	
	return;
}

void FVehicleSuspensionSolver::FinalizeUpdateSolidAxle(
	const float WheelRadius,
	const FVehicleSuspensionKinematicsConfig& KineConfig,
	const FVehicleSuspensionSpringConfig& SpringConfig,
	const FTransform& AsyncChassisWorldTransform,
	Chaos::FRigidBodyHandle_Internal* ChassisHandle,
	float InDeltaTime, 
	float InSwaybarForce,
	FVehicleSuspensionSimContext& Ctx,
	const float InTrackWidth,
	const FVector& InThisWheelHitWorldLocation,
	const FVector& InOtherWheelHitWorldLocation,
	const FVector3f& TireForce)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_Wheel_SuspensionSolver_UpdateSuspension);

	Ctx.PhysicsDeltaTime = InDeltaTime;
	Ctx.SwaybarForce = InSwaybarForce;

	ComputeSolidAxle(
		Ctx,
		WheelRadius,
		KineConfig,
		InTrackWidth,
		InThisWheelHitWorldLocation,
		InOtherWheelHitWorldLocation
	);

	FVehicleChassisSimState ChassisState;

	FetchChassisPhysicsState(
		ChassisHandle,
		ChassisState
	);

	CalculateImpactPointWorldVelocity(
		Ctx,
		ChassisState
	);

	ComputeAntiPitchRollGeometry(
		Ctx,
		ChassisHandle,
		true,
		KineConfig.SuspensionType,
		WheelRadius,
		CachedLUTs,
		AsyncChassisWorldTransform,
		TireForce
	);

	// get suspension force
	ComputeSuspensionForce(
		Ctx, 
		WheelRadius, 
		ChassisState,
		SpringConfig, 
		KineConfig, 
		CachedLUTs
	);

	CopyContextToState(Ctx, State);
	CopyContextToHitResult(Ctx, RayCastResult);
}

void FVehicleSuspensionSolver::RoughlyInitializeState(const FTransform& ComponentRelativeTransform, const FVehicleSuspensionKinematicsConfig& KineConfig, FVehicleSuspensionSimState& InState)
{
	float SideSign = ComponentRelativeTransform.GetLocation().Y >= 0.f ? 1.f : -1.f;

	// 1. 粗略算一下塔顶当前的真实底盘坐标
	FVector3f TopMountLocalPos = KineConfig.TopMountLocalLocation;
	TopMountLocalPos.Y *= SideSign;
	FVector3f TopMountChassis = (FVector3f)ComponentRelativeTransform.TransformPositionNoScale((FVector)TopMountLocalPos);

	// 2. 假设下球头就在塔顶的"正下方" (减震器的当前估算长度)
	float GuessStrutLen = KineConfig.MinStrutLength + KineConfig.Stroke;
	InState.LowerBallJointChassisLocation = TopMountChassis - FVector3f(0.f, 0.f, GuessStrutLen);

	FQuat4f SpindleMountRotation = GetSpindleMountQuat(
		KineConfig.StaticSpindleRotation,
		SideSign
	);

	FQuat4f HubChassisRot = (SpindleMountRotation).GetNormalized();
	InState.HubChassisRotation = HubChassisRot;

	// 3. 把 HubOffset 也跟着旋转过去，作为 Hub 的粗略位置
	FVector3f HubOffset = KineConfig.HubOffsetFromLowerJoint;
	HubOffset.Y *= SideSign;
	FVector3f HubOffsetFromLowerJointChassis = HubChassisRot.RotateVector(HubOffset);
	InState.HubChassisLocation = InState.LowerBallJointChassisLocation + HubOffsetFromLowerJointChassis;
}

FVehicleSuspensionSimState FVehicleSuspensionSolver::SolveKinematicsAtExtension(
	const float WheelRadius,
	const FVehicleSuspensionKinematicsConfig& KineConfig,
	const FTransform& ComponentRelativeTransform,
	float InExtensionRatio,
	float InSteeringAngle,
	int32 Iteration,
	const FVehicleSuspensionCachedLUTs* LUTs,
	const FVehicleSuspensionSimState* PrevState)
{
	FVehicleSuspensionSimState NewState;
	
	if (PrevState != nullptr)
	{
		NewState = *PrevState;
	}
	else
	{
		RoughlyInitializeState(ComponentRelativeTransform, KineConfig, NewState);
	}

	FVehicleSuspensionSimContext Ctx;
	CopyStateToContext(NewState, Ctx);
	Ctx.SuspensionExtensionRatio = InExtensionRatio;
	Ctx.SteeringAngle = InSteeringAngle;

	FTransform ChassisWorldTransform = FTransform::Identity;

	for (int32 i = 0; i < Iteration; i++)
	{
		PrepareSimulation(
			Ctx,
			ComponentRelativeTransform,
			ChassisWorldTransform,
			KineConfig
		);
		ComputeRayCastLocation(Ctx, KineConfig);

		Ctx.HitDistance = InExtensionRatio * Ctx.RayCastLength + WheelRadius;
		
		FVehicleSuspensionCachedLUTs RealLUTs;
		if (LUTs != nullptr)RealLUTs = *LUTs;

		switch (KineConfig.SuspensionType)
		{
		default:
		case EVehicleIndependentSuspensionType::StraightLine:
			ComputeStraightSuspension(Ctx, WheelRadius, KineConfig, RealLUTs);
			break;
		case EVehicleIndependentSuspensionType::Macpherson:
			ComputeMacpherson(Ctx, WheelRadius, KineConfig, RealLUTs);
			break;
		case EVehicleIndependentSuspensionType::DoubleWishbone:
			ComputeDoubleWishbone(Ctx, WheelRadius, KineConfig, RealLUTs);
			break;
		}
	}

	CopyContextToState(Ctx, NewState);
	return NewState;
}

void FVehicleSuspensionSolver::StartSolveSolidAxleAtExtension(
	const FVehicleSuspensionSimState& PrevState,
	const float WheelRadius,
	const FVehicleSuspensionKinematicsConfig& KineConfig,
	const FTransform& ComponentRelativeTransform,
	float InExtensionRatio, 
	float InSteeringAngle,
	FVector& OutHitWorldLocation,
	FVehicleSuspensionSimContext& Ctx)
{
	CopyStateToContext(PrevState, Ctx);
	Ctx.SuspensionExtensionRatio = InExtensionRatio;
	Ctx.SteeringAngle = InSteeringAngle;

	FTransform ChassisWorldTransform = FTransform::Identity;

	PrepareSimulation(
		Ctx, 
		ComponentRelativeTransform, 
		ChassisWorldTransform,
		KineConfig
	);
	ComputeRayCastLocation(Ctx, KineConfig);

	Ctx.HitDistance = InExtensionRatio * Ctx.RayCastLength + WheelRadius;
	float DistanceToRayCastStart = FMath::Max(0.f, Ctx.HitDistance - WheelRadius);
	FVector RayDir = (Ctx.RayCastStartWorldLocation - Ctx.RayCastEndWorldLocation).GetSafeNormal();
	OutHitWorldLocation = Ctx.RayCastStartWorldLocation - RayDir * DistanceToRayCastStart;
}

FVehicleSuspensionSimState FVehicleSuspensionSolver::FinalizeSolveSolidAxleAtExtension(
	const float WheelRadius,
	const FVehicleSuspensionKinematicsConfig& KineConfig, 
	FVehicleSuspensionSimContext& Ctx,
	const float InTrackWidth,
	const FVector& InThisWheelHitWorldLocation,
	const FVector& InOtherWheelHitWorldLocation)
{
	ComputeSolidAxle(
		Ctx,
		WheelRadius,
		KineConfig,
		InTrackWidth,
		InThisWheelHitWorldLocation,
		InOtherWheelHitWorldLocation
	);

	FVehicleSuspensionSimState NewState;
	CopyContextToState(Ctx, NewState);
	return NewState;
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

	const FTransform& ChassisWorldTrans = WheelComponent->GetChassisAsyncWorldTransform();
	const FVehicleSuspensionKinematicsConfig& KineConfig = WheelComponent->GetSuspensionKinematicsConfig();

	FVector HubRelativePos = FVector(State.HubChassisLocation);
	FVector HubWorldPos = ChassisWorldTrans.TransformPositionNoScale(HubRelativePos);
	
	float WheelPos = State.bIsRightWheel ? 1.f : -1.f;

	if (bDrawSuspension)
	{
		FVector LowerPivotChassis, LowerAxisChassis, LowerBallJointChassis, 
			UpperPivotChassis, UpperAxisChassis, UpperBallJointChassis;
		WheelComponent->GetLowerWishboneState(LowerPivotChassis, LowerAxisChassis, LowerBallJointChassis);
		WheelComponent->GetUpperWishboneState(UpperPivotChassis, UpperAxisChassis, UpperBallJointChassis);

		FVector LowerPivotLocation = ChassisWorldTrans.TransformPositionNoScale(LowerPivotChassis);
		FVector UpperPivotLocation = ChassisWorldTrans.TransformPositionNoScale(UpperPivotChassis);
		FVector LowerAxisDirection = ChassisWorldTrans.TransformVectorNoScale(LowerAxisChassis);
		FVector UpperAxisDirection = ChassisWorldTrans.TransformVectorNoScale(UpperAxisChassis);
		FVector LowerBallJointLocation = ChassisWorldTrans.TransformPositionNoScale(LowerBallJointChassis);
		FVector UpperBallJointLocation = ChassisWorldTrans.TransformPositionNoScale(UpperBallJointChassis);
		FVector TopMountLocation = ChassisWorldTrans.TransformPositionNoScale((FVector)State.TopMountChassisLocation);

		// draw arm
		if (KineConfig.SuspensionType != EVehicleIndependentSuspensionType::StraightLine)
		{
			DrawDebugLine(TempWorld, LowerPivotLocation, LowerBallJointLocation, FColor(0, 0, 255), false, Duration, 0, Thickness);
			
			// draw axis
			float AxisLength = WheelComponent->GetSuspensionKinematicsConfig().LowerWishbone.Length * 0.5f;
			DrawDebugLine(TempWorld,
				LowerPivotLocation + LowerAxisDirection * AxisLength,
				LowerPivotLocation - LowerAxisDirection * AxisLength,
				FColor(0, 0, 255), false, Duration, 0, Thickness);
			
			if (KineConfig.SuspensionType == EVehicleIndependentSuspensionType::DoubleWishbone)
			{
				DrawDebugLine(TempWorld, UpperPivotLocation, UpperBallJointLocation, FColor(0, 0, 255), false, Duration, 0, Thickness);

				AxisLength = WheelComponent->GetSuspensionKinematicsConfig().UpperWishbone.Length * 0.5f;
				DrawDebugLine(TempWorld,
					UpperPivotLocation + UpperAxisDirection * AxisLength,
					UpperPivotLocation - UpperAxisDirection * AxisLength,
					FColor(0, 0, 255), false, Duration, 0, Thickness);
			}
		}
		
		// draw strut
		DrawDebugLine(TempWorld, TopMountLocation, LowerBallJointLocation, FColor(255, 255, 0), false, Duration, 0, Thickness);

		// draw knuckle
		if (KineConfig.SuspensionType == EVehicleIndependentSuspensionType::DoubleWishbone)
		{
			DrawDebugLine(TempWorld, LowerBallJointLocation, UpperBallJointLocation, FColor(0, 255, 255), false, Duration, 0, Thickness);
		}
		
		DrawDebugLine(TempWorld, LowerBallJointLocation, HubWorldPos, FColor(0, 255, 255), false, Duration, 0, Thickness);
	}

	if (bDrawWheel)
	{
		//draw cylinder
		FVector W = FVector(State.WheelWorldRightVector) * WheelComponent->GetWheelWidth() * 0.5;
		DrawDebugCylinder(TempWorld, HubWorldPos + W, HubWorldPos - W,
			WheelComponent->GetWheelRadius(), 8, FColor(128, 128, 128), false, Duration, 0, Thickness);
	}

	if (bDrawRayCast)
	{
		float TempR = WheelComponent->GetWheelRadius();
		float TempHalfWidth = WheelComponent->GetWheelWidth() * 0.5;
		float ValidR = FMath::Min(TempR, TempHalfWidth);
		FVector HalfSize = FVector(TempR, TempHalfWidth, TempR) * 0.707;
		FQuat Orientation = FQuat(RayCastResult.TraceRot *
			FQuat4f(0.0f, 0.38268343f, 0.0f, 0.92387953f));	// this is Rotator(0.0, 45.0, 0.0)
		float SphereTraceR = (State.ImpactWorldLocation - RayCastResult.Location).Length();
		float RayCastLength = (RayCastResult.TraceStart - RayCastResult.TraceEnd).Length();
		switch (KineConfig.RayCastMode)
		{
		case EVehicleSuspensionRayCastMode::LineTrace:
			DrawDebugLine(TempWorld, RayCastResult.TraceStart, RayCastResult.TraceEnd, FColor(0, 255, 0), false, Duration, 0, Thickness);
			if (RayCastResult.bBlockingHit)DrawDebugPoint(TempWorld, RayCastResult.Location, Thickness, FColor(255, 0, 0), false, Duration, Thickness);
			break;
		case EVehicleSuspensionRayCastMode::SphereTrace:
			//draw capsule
			DrawDebugCapsule(TempWorld, (RayCastResult.TraceStart + RayCastResult.TraceEnd) * 0.5,
				RayCastLength * 0.5 + SphereTraceR, SphereTraceR, (FQuat)RayCastResult.TraceRot.GetNormalized(), FColor(0, 255, 0), false, Duration, 0, Thickness);
			if (RayCastResult.bBlockingHit)DrawDebugSphere(TempWorld, RayCastResult.Location, SphereTraceR, 8, FColor(255, 0, 0), false, Duration, 0, Thickness);
			break;
		case EVehicleSuspensionRayCastMode::BoxTrace:
			if (RayCastResult.bBlockingHit)DrawDebugBox(TempWorld, RayCastResult.Location, HalfSize, Orientation, FColor(0, 255, 0), false, Duration, 0, Thickness);
			break;
		case EVehicleSuspensionRayCastMode::SphereTraceNoRefinement:
			DrawDebugCapsule(TempWorld, (RayCastResult.TraceStart + RayCastResult.TraceEnd) * 0.5,
				RayCastLength * 0.5 + ValidR,
				ValidR, (FQuat)RayCastResult.TraceRot.GetNormalized(), FColor(0, 255, 0), false, Duration, 0, Thickness);
			if (RayCastResult.bBlockingHit)DrawDebugSphere(TempWorld, RayCastResult.Location, ValidR, 8, FColor(255, 0, 0), false, Duration, 0, Thickness);
			break;
		case EVehicleSuspensionRayCastMode::MultiSphereTrace:
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

	const FTransform& ChassisTrans = WheelComponent->GetChassisAsyncWorldTransform();

	FVector TempStart = ChassisTrans.TransformPositionNoScale(FVector(WheelComponent->GetTopMountChassisLocation()));
	FVector TempEnd = TempStart + FVector(State.ImpactWorldNormal) * State.ForceAlongImpactNormal * Length * 0.01;
	DrawDebugLine(TempWorld, TempStart, TempEnd, TempColor, false, Duration, 0, Thickness);
}

void FVehicleSuspensionSolver::UpdateCachedLUTs(
	const FVehicleSuspensionKinematicsConfig& KineConfig)
{
	CacheLUTs(CachedLUTs, KineConfig);
}

void FVehicleSuspensionSolver::CacheLUTs(FVehicleSuspensionCachedLUTs& LUTs, const FVehicleSuspensionKinematicsConfig& KineConfig)
{
	const FVehicleSuspensionKinematicsConfig& Config = KineConfig;

	if (IsValid(Config.CamberCurve))
	{
		LUTs.CamberCurve.CopyFromRichCurve(Config.CamberCurve->FloatCurve);
	}
	else
	{
		LUTs.CamberCurve.SetAllTo(0.f);
	}
	if (IsValid(Config.ToeCurve))
	{
		LUTs.ToeCurve.CopyFromRichCurve(Config.ToeCurve->FloatCurve);
	}
	else
	{
		LUTs.ToeCurve.SetAllTo(0.f);
	}
	if (IsValid(Config.CasterCurve))
	{
		LUTs.CasterCurve.CopyFromRichCurve(Config.CasterCurve->FloatCurve);
	}
	else
	{
		LUTs.CasterCurve.SetAllTo(0.f);
	}
	if (IsValid(Config.AntiDiveCurve))
	{
		LUTs.AntiDiveCurve.CopyFromRichCurve(Config.AntiDiveCurve->FloatCurve);
	}
	else
	{
		LUTs.AntiDiveCurve.SetAllTo(0.f);
	}
	if (IsValid(Config.AntiSquatCurve))
	{
		LUTs.AntiSquatCurve.CopyFromRichCurve(Config.AntiSquatCurve->FloatCurve);
	}
	else
	{
		LUTs.AntiSquatCurve.SetAllTo(0.f);
	}
	if (IsValid(Config.AntiRollCurve))
	{
		LUTs.AntiRollCurve.CopyFromRichCurve(Config.AntiRollCurve->FloatCurve);
	}
	else
	{
		LUTs.AntiRollCurve.SetAllTo(0.f);
	}
	if (IsValid(Config.SpringMotionRatioCurve))
	{
		LUTs.MotionRatioCurve.CopyFromRichCurve(Config.SpringMotionRatioCurve->FloatCurve);
	}
	else
	{
		LUTs.MotionRatioCurve.SetAllTo(1.f);
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
	float DSqInv = UVehicleUtilities::SafeDivide(1.f, DSq);
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

void FVehicleSuspensionSolver::CopyContextToState(
	const FVehicleSuspensionSimContext& Context, 
	FVehicleSuspensionSimState& NewState)
{
	NewState.bIsRightWheel = Context.WheelSideSign >= 0.f;
	NewState.bHitGround = Context.bHitGround;
	NewState.SteeringAngle = Context.SteeringAngle;
	NewState.SuspensionCurrentLength = Context.SuspensionCurrentLength;
	NewState.EffectiveSprungMassNormal = Context.EffectiveSprungMassNormal;
	NewState.EffectiveSprungMassLong = Context.EffectiveSprungMassLong;
	NewState.EffectiveSprungMassLat = Context.EffectiveSprungMassLat;
	NewState.ForceAlongImpactNormal = Context.ForceAlongImpactNormal;
	NewState.ImpactFriction = Context.ImpactFriction;
	NewState.AntiPitchScale = Context.AntiPitchScale;
	NewState.AntiRollScale = Context.AntiRollScale;
	NewState.WheelWorldRightVector = FVector3f(Context.WheelWorldRightVector);
	NewState.HubChassisLocation = Context.HubChassisTransform.GetLocation();
	NewState.HubChassisRotation = FQuat4f(Context.HubChassisTransform.GetRotation());
	NewState.ImpactWorldVelocity = Context.ImpactWorldVelocity;
	NewState.ImpactWorldNormal = FVector3f(Context.HitResult.Normal);
	NewState.ImpactWorldLocation = Context.HitResult.ImpactPoint;
	
	NewState.TopMountChassisLocation = Context.TopMountChassisLocation;
	NewState.LowerBallJointChassisLocation = Context.LowerBallJointChassisLocation;
	NewState.UpperBallJointChassisLocation = Context.UpperBallJointChassisLocation;
	NewState.SteerAxisChassisDirection = Context.SteerAxisChassisDirection;
}

void FVehicleSuspensionSolver::CopyContextToHitResult(
	const FVehicleSuspensionSimContext& Context, 
	FVehicleSuspensionHitResult& NewHitResult)
{
	// copy hitresult
	NewHitResult = FVehicleSuspensionHitResult(
		Context.HitResult.PhysMaterial,
		Context.HitResult.Component,
		Context.HitResult.BoneName,
		Context.HitResult.bBlockingHit,
		Context.HitResult.TraceStart,
		Context.HitResult.TraceEnd,
		Context.HitResult.Location,
		(FQuat4f)Context.RayCastWorldTransform.GetRotation()
	);
}

void FVehicleSuspensionSolver::CopyStateToContext(
	const FVehicleSuspensionSimState& PrevState, 
	FVehicleSuspensionSimContext& Context)
{
	Context.SuspensionCurrentLength = PrevState.SuspensionCurrentLength;
	Context.StaticSprungMass = PrevState.StaticSprungMass;
	Context.HubChassisTransform.SetRotation(PrevState.HubChassisRotation);
	Context.HubChassisTransform.SetLocation(PrevState.HubChassisLocation);
	Context.LowerBallJointChassisLocation = PrevState.LowerBallJointChassisLocation;
	Context.UpperBallJointChassisLocation = PrevState.UpperBallJointChassisLocation;
}

FRotator3f FVehicleSuspensionSolver::GetCamberToeCasterFromLUTs(
	const FVehicleSuspensionCachedLUTs& LUTs,
	float CompressionRatio,
	float WheelYPosSign,
	float BaseCamber,
	float BaseToe,
	float BaseCaster)
{
	FRotator3f r;
	r.Pitch = BaseCaster;
	r.Yaw = BaseToe;
	r.Roll = BaseCamber;

	r.Yaw += LUTs.ToeCurve.FastEval(CompressionRatio).Value;
	r.Roll += LUTs.CamberCurve.FastEval(CompressionRatio).Value;

	// flip the caster and toe if necessary
	r *= WheelYPosSign;

	r.Pitch += LUTs.CasterCurve.FastEval(CompressionRatio).Value;

	return r;
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

FVector3f FVehicleSuspensionSolver::GetTopMountChassisLocation(
	const FVehicleSuspensionKinematicsConfig& Config, 
	const FTransform3f& WheelComponentRelativeTransform)
{
	FVector3f TopMountLocalPos = Config.TopMountLocalLocation;
	TopMountLocalPos.Y *= FMath::Sign(WheelComponentRelativeTransform.GetLocation().Y);
	return WheelComponentRelativeTransform.TransformPositionNoScale(TopMountLocalPos);
}

void FVehicleSuspensionSolver::GetLowerWishboneState(
	const FVehicleSuspensionKinematicsConfig& Config, 
	const FVehicleSuspensionSimState& SuspensionState, 
	const FTransform3f& WheelComponentRelativeTransform, 
	FVector3f& OutPivotChassisLocation, 
	FVector3f& OutAxisChassisDirection,
	FVector3f& OutBallJointChassisLocation)
{
	OutBallJointChassisLocation = SuspensionState.LowerBallJointChassisLocation;

	FVector3f PivotLocal = Config.LowerWishbone.MountLocalLocation;
	OutPivotChassisLocation = WheelComponentRelativeTransform.TransformPositionNoScale(PivotLocal);

	FVector3f AxisLocal = Config.LowerWishbone.RotationLocalAxis;
	OutAxisChassisDirection = WheelComponentRelativeTransform.TransformVectorNoScale(AxisLocal);
}

void FVehicleSuspensionSolver::GetUpperWishboneState(
	const FVehicleSuspensionKinematicsConfig& Config, 
	const FVehicleSuspensionSimState& SuspensionState, 
	const FTransform3f& WheelComponentRelativeTransform, 
	FVector3f& OutPivotChassisLocation, 
	FVector3f& OutAxisChassisDirection,
	FVector3f& OutBallJointChassisLocation)
{
	OutBallJointChassisLocation = SuspensionState.UpperBallJointChassisLocation;

	FVector3f PivotLocal = Config.UpperWishbone.MountLocalLocation;
	OutPivotChassisLocation = WheelComponentRelativeTransform.TransformPositionNoScale(PivotLocal);

	FVector3f AxisLocal = Config.UpperWishbone.RotationLocalAxis;
	OutAxisChassisDirection = WheelComponentRelativeTransform.TransformVectorNoScale(AxisLocal);
}

void FVehicleSuspensionSolver::PrepareSimulation(
	FVehicleSuspensionSimContext& Ctx,
	const FTransform& ComponentRelativeTransform,
	const FTransform& AsyncChassisWorldTransform,
	const FVehicleSuspensionKinematicsConfig& Config)
{
	Ctx.WheelCompToChassisTransform = (FTransform3f)ComponentRelativeTransform;
	Ctx.ChassisWorldTransform = AsyncChassisWorldTransform;

	//dealing with transforms
	Ctx.WheelSideSign = Ctx.WheelCompToChassisTransform.GetLocation().Y >= 0.f ? 1.f : -1.f;
	FVector3f TopMountLocalPos = Config.TopMountLocalLocation;
	TopMountLocalPos.Y *= Ctx.WheelSideSign;
	Ctx.TopMountChassisLocation = Ctx.WheelCompToChassisTransform.TransformPositionNoScale(TopMountLocalPos);
}

void FVehicleSuspensionSolver::ComputeStraightRayCastLocation(
	FVehicleSuspensionSimContext& Ctx, 
	const FVehicleSuspensionKinematicsConfig& Config)
{
	/*
	* 
	* 这里还没完工
	* 
	*/
	FVector3f RayDirChassis = Ctx.WheelCompToChassisTransform.GetRotation().GetUpVector();
	Ctx.StrutChassisDirection = Ctx.WheelCompToChassisTransform.GetRotation().GetUpVector();
	Ctx.RayCastLength = Config.Stroke;

	Ctx.HubOffsetFromLowerJointChassis = Ctx.HubChassisTransform.GetLocation() - Ctx.LowerBallJointChassisLocation;
	Ctx.RayCastStartChassisLocation = Ctx.TopMountChassisLocation - RayDirChassis * Config.MinStrutLength + Ctx.HubOffsetFromLowerJointChassis;
	Ctx.RayCastStartWorldLocation = Ctx.ChassisWorldTransform.TransformPositionNoScale((FVector)Ctx.RayCastStartChassisLocation);

	FVector3f RayCastEndChassis = Ctx.RayCastStartChassisLocation - Ctx.RayCastLength * RayDirChassis;
	Ctx.RayCastEndWorldLocation = Ctx.ChassisWorldTransform.TransformPositionNoScale((FVector)RayCastEndChassis);

	FQuat SteeringRot = FQuat((FVector)RayDirChassis, FMath::DegreesToRadians(Ctx.SteeringAngle));
	FQuat RayCastRot = Ctx.ChassisWorldTransform.GetRotation() * SteeringRot * (FQuat)Ctx.WheelCompToChassisTransform.GetRotation() * (FQuat)Ctx.LowerWishboneLocalRotation;

	Ctx.RayCastWorldTransform = FTransform(RayCastRot, Ctx.RayCastEndWorldLocation, FVector(1.f));
	Ctx.RayCastDirectionWorld = Ctx.ChassisWorldTransform.TransformVectorNoScale((FVector)RayDirChassis);
}

void FVehicleSuspensionSolver::ComputeWishboneRayCastLocation(
	FVehicleSuspensionSimContext& Ctx,
	const FVehicleSuspensionKinematicsConfig& Config)
{
	FVector3f BaseForward = FVector3f(1.f, 0.f, 0.f);
	FVector3f LowerArmAxisLocal = Config.LowerWishbone.RotationLocalAxis.GetSafeNormal(); // Relative to the wheel component
	LowerArmAxisLocal.Y *= Ctx.WheelSideSign;
	Ctx.LowerWishboneLocalRotation = FQuat4f::FindBetweenNormals(BaseForward, LowerArmAxisLocal); // Relative to the wheel component

	FVector3f BaseUp = FVector3f(0.f, 0.f, 1.f);
	FVector3f BiasedUp = Ctx.LowerWishboneLocalRotation.RotateVector(BaseUp); // The direction to shoot the ray, relative to the wheel component

	FVector3f RayDirChassis = Ctx.WheelCompToChassisTransform.TransformVectorNoScale(BiasedUp); // Relative to chassis

	Ctx.HubOffsetFromLowerJointChassis = Ctx.HubChassisTransform.GetLocation() - Ctx.LowerBallJointChassisLocation;

	Ctx.StrutChassisDirection = (Ctx.TopMountChassisLocation - Ctx.LowerBallJointChassisLocation).GetSafeNormal();

	float StrutProjOnRay = FVector3f::DotProduct(Ctx.StrutChassisDirection, RayDirChassis);
	Ctx.RayCastLength = FMath::Abs(StrutProjOnRay) * Config.Stroke;

	FVector3f LowerPivotLocationLocal = Config.LowerWishbone.MountLocalLocation;
	LowerPivotLocationLocal.Y *= Ctx.WheelSideSign;
	Ctx.LowerPivotChassisLocation = Ctx.WheelCompToChassisTransform.TransformPositionNoScale(LowerPivotLocationLocal);
	FVector3f StrutStart = Ctx.TopMountChassisLocation - Ctx.StrutChassisDirection * Config.MinStrutLength; // The start of the compressible part of the strut
	FVector3f StrutStartFromMount = StrutStart - Ctx.LowerPivotChassisLocation;

	Ctx.LowerWishboneChassisAxis = Ctx.WheelCompToChassisTransform.TransformVectorNoScale(LowerArmAxisLocal);

	FVector3f ProjectedFromMount = FVector3f::VectorPlaneProject(StrutStartFromMount, Ctx.LowerWishboneChassisAxis);
	FVector3f StrutStartProjOnAxis = Ctx.LowerPivotChassisLocation + ProjectedFromMount; // The start of the compressible part of the strut, projected onto the plane of the ray

	Ctx.RayCastStartChassisLocation = StrutStartProjOnAxis + Ctx.HubOffsetFromLowerJointChassis;
	Ctx.RayCastStartWorldLocation = Ctx.ChassisWorldTransform.TransformPositionNoScale((FVector)Ctx.RayCastStartChassisLocation);

	FVector3f RayCastEndChassis = Ctx.RayCastStartChassisLocation - Ctx.RayCastLength * RayDirChassis;
	Ctx.RayCastEndWorldLocation = Ctx.ChassisWorldTransform.TransformPositionNoScale((FVector)RayCastEndChassis);

	FQuat SteeringRot = FQuat((FVector)RayDirChassis, FMath::DegreesToRadians(Ctx.SteeringAngle));
	FQuat RayCastRot = Ctx.ChassisWorldTransform.GetRotation() * SteeringRot * (FQuat)Ctx.WheelCompToChassisTransform.GetRotation() * (FQuat)Ctx.LowerWishboneLocalRotation;

	Ctx.RayCastWorldTransform = FTransform(RayCastRot, Ctx.RayCastEndWorldLocation, FVector(1.f));
	Ctx.RayCastDirectionWorld = Ctx.ChassisWorldTransform.TransformVectorNoScale((FVector)RayDirChassis);
}

void FVehicleSuspensionSolver::ComputeRayCastLocation(
	FVehicleSuspensionSimContext& Ctx, 
	const FVehicleSuspensionKinematicsConfig& Config)
{
	switch (Config.SuspensionType)
	{
	default:
	case EVehicleIndependentSuspensionType::StraightLine:
		ComputeStraightRayCastLocation(Ctx, Config);
		break;
	case EVehicleIndependentSuspensionType::Macpherson:
	case EVehicleIndependentSuspensionType::DoubleWishbone:
		ComputeWishboneRayCastLocation(Ctx, Config);
		break;
	}
}

bool FVehicleSuspensionSolver::ShouldDoRefinedTrace(
	FVehicleSuspensionSimContext& Ctx,
	const float HalfWheelWidth,
	const FVehicleSuspensionKinematicsConfig& Config)
{
	FVector LocalImpactPoint = Ctx.RayCastWorldTransform.InverseTransformPositionNoScale(Ctx.HitResult.ImpactPoint);

	return FMath::Abs(LocalImpactPoint.Y) > HalfWheelWidth || LocalImpactPoint.Z > Config.Stroke;
}

void FVehicleSuspensionSolver::ComputeHitDistance(
	FVehicleSuspensionSimContext& Ctx,
	const float WheelRadius,
	const float EquivalentSphereTraceRadius)
{
	Ctx.HitDistance = Ctx.bHitGround ?
		Ctx.HitResult.Distance + EquivalentSphereTraceRadius : Ctx.RayCastLength + WheelRadius;
	float HitDistanceNoBias = FMath::Max(0.f, Ctx.HitDistance - WheelRadius);
	Ctx.SuspensionExtensionRatio = UVehicleUtilities::SafeDivide(HitDistanceNoBias, Ctx.RayCastLength);
}

void FVehicleSuspensionSolver::CacheImpactFriction(
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
float FVehicleSuspensionSolver::SuspensionLineTrace(
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
float FVehicleSuspensionSolver::SuspensionSphereTrace(
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
float FVehicleSuspensionSolver::SuspensionBoxTrace(
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
float FVehicleSuspensionSolver::SuspensionSphereTraceNoRefinement(
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
float FVehicleSuspensionSolver::SuspensionMultiSphereTrace(
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
			bool bShouldMultiSphereTrace = FPhysicsInterface::GeomSweepSingle(
				World,
				FCollisionShape::MakeBox(HalfSize),
				Ctx.RayCastWorldTransform.GetRotation(),
				Ctx.HitResult,
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

				if (Ctx.bHitGround)
				{
					int32 ArrayLength = MultiHitResults.Num();
					Ctx.HitResult = MultiHitResults[ArrayLength - 1];
				}
			}
		}
	}

	return WheelRadius;
}

void FVehicleSuspensionSolver::SuspensionRayCast(
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

void FVehicleSuspensionSolver::SolveLowerWishbone(
	FVehicleSuspensionSimContext& Ctx,
	const float WheelRadius,
	const FVehicleSuspensionKinematicsConfig& Config)
{
	float LowerArmLength = Config.LowerWishbone.Length;

	FVector3f LowerPivotLocationLocal = Config.LowerWishbone.MountLocalLocation;
	LowerPivotLocationLocal.Y *= Ctx.WheelSideSign;
	FTransform3f LowerWishboneLocalTransform = FTransform3f(Ctx.LowerWishboneLocalRotation, LowerPivotLocationLocal, FVector3f(1.f));
	FTransform3f LowerWishboneToChassis = LowerWishboneLocalTransform * Ctx.WheelCompToChassisTransform;

	// get rid of hub offset
	FVector3f RayCastStartLocal = LowerWishboneToChassis.InverseTransformPositionNoScale(Ctx.RayCastStartChassisLocation - Ctx.HubOffsetFromLowerJointChassis);
	FVector2f RayCastStart2D = Coord3DTo2D(RayCastStartLocal, Ctx.WheelSideSign);

	// Compute the position of the lower wishbone on the suspension plane
	FVector2f LowerWishboneEnd2D = FVector2f(0.f);
	LowerWishboneEnd2D.X = RayCastStart2D.X - FMath::Max(0.f, Ctx.HitDistance - WheelRadius);
	float SqrDist = FMath::Square(LowerArmLength) - FMath::Square(LowerWishboneEnd2D.X);
	LowerWishboneEnd2D.Y = FMath::Sqrt(FMath::Max(0.f, SqrDist));
	FVector3f LowerWishboneEndLocal = Coord2DTo3D(LowerWishboneEnd2D, Ctx.WheelSideSign);
	Ctx.LowerBallJointChassisLocation = LowerWishboneToChassis.TransformPositionNoScale(LowerWishboneEndLocal);

	Ctx.StrutChassisDirection = (Ctx.TopMountChassisLocation - Ctx.LowerBallJointChassisLocation).GetSafeNormal();

	Ctx.bValidKinematicsConfig = Ctx.bValidKinematicsConfig && SqrDist >= 0.f;
}

void FVehicleSuspensionSolver::ComputeStraightSuspension(
	FVehicleSuspensionSimContext& Ctx,
	const float WheelRadius,
	const FVehicleSuspensionKinematicsConfig& Config,
	const FVehicleSuspensionCachedLUTs& LUTs)
{
	/* Kinematics From Look Up Tables */
	float CompressionRatio = 1.f - Ctx.SuspensionExtensionRatio;
	FQuat4f RotLUT = FQuat4f(GetCamberToeCasterFromLUTs(LUTs, CompressionRatio, Ctx.WheelSideSign));

	/* Kinematics From Geometry */
	FQuat4f SpindleMountRotation = GetSpindleMountQuat(
		Config.StaticSpindleRotation,
		Ctx.WheelSideSign
	);

	Ctx.SteerAxisChassisDirection = Ctx.StrutChassisDirection;
	FQuat4f SteeringBiasRotation = FQuat4f(Ctx.SteerAxisChassisDirection, FMath::DegreesToRadians(Ctx.SteeringAngle));

	FQuat4f HubChassisRot = (SteeringBiasRotation * RotLUT * SpindleMountRotation).GetNormalized();
	Ctx.HubChassisTransform.SetRotation(HubChassisRot);

	FVector3f HubOffset = Config.HubOffsetFromLowerJoint;
	HubOffset.Y *= Ctx.WheelSideSign;
	Ctx.HubOffsetFromLowerJointChassis = HubChassisRot.RotateVector(HubOffset);

	Ctx.LowerBallJointChassisLocation = Ctx.TopMountChassisLocation - Ctx.StrutChassisDirection * (Config.MinStrutLength + FMath::Max(0.f, Ctx.HitDistance - WheelRadius));
	Ctx.HubChassisTransform.SetLocation(Ctx.LowerBallJointChassisLocation + Ctx.HubOffsetFromLowerJointChassis);

	FVector3f WheelChassisRightVec = Ctx.HubChassisTransform.GetRotation().GetRightVector();
	Ctx.WheelWorldRightVector = Ctx.ChassisWorldTransform.TransformVectorNoScale(FVector(WheelChassisRightVec));

	Ctx.HubWorldLocation = Ctx.ChassisWorldTransform.TransformPositionNoScale(FVector(Ctx.HubChassisTransform.GetLocation()));
}

void FVehicleSuspensionSolver::ComputeMacpherson(
	FVehicleSuspensionSimContext& Ctx,
	const float WheelRadius,
	const FVehicleSuspensionKinematicsConfig& Config,
	const FVehicleSuspensionCachedLUTs& LUTs)
{
	/* Kinematics From Look Up Tables */
	float CompressionRatio = 1.f - Ctx.SuspensionExtensionRatio;
	FQuat4f RotLUT = FQuat4f(GetCamberToeCasterFromLUTs(LUTs, CompressionRatio, Ctx.WheelSideSign));

	/* Kinematics From Geometry */
	SolveLowerWishbone(Ctx, WheelRadius, Config);

	// The steer axis (kingpin) is the strut for MacPherson, for simplification
	Ctx.SteerAxisChassisDirection = Ctx.StrutChassisDirection;

	FVector3f BaseUp = FVector3f(0.f, 0.f, 1.f);
	FQuat4f SteerAxisBiasRotation = FQuat4f::FindBetweenNormals(BaseUp, Ctx.SteerAxisChassisDirection);
	FQuat4f SteeringBiasRotation = FQuat4f(Ctx.SteerAxisChassisDirection, FMath::DegreesToRadians(Ctx.SteeringAngle));

	FQuat4f SpindleMountRotation = GetSpindleMountQuat(
		Config.StaticSpindleRotation,
		Ctx.WheelSideSign
	);

	FQuat4f HubChassisRot = (SteeringBiasRotation * SteerAxisBiasRotation * RotLUT * SpindleMountRotation).GetNormalized();
	Ctx.HubChassisTransform.SetRotation(HubChassisRot);

	FVector3f HubOffset = Config.HubOffsetFromLowerJoint;
	HubOffset.Y *= Ctx.WheelSideSign;
	Ctx.HubOffsetFromLowerJointChassis = HubChassisRot.RotateVector(HubOffset);

	Ctx.HubChassisTransform.SetLocation(Ctx.LowerBallJointChassisLocation + Ctx.HubOffsetFromLowerJointChassis);

	FVector3f WheelChassisRightVec = Ctx.HubChassisTransform.GetRotation().GetRightVector();
	Ctx.WheelWorldRightVector = Ctx.ChassisWorldTransform.TransformVectorNoScale(FVector(WheelChassisRightVec));

	Ctx.HubWorldLocation = Ctx.ChassisWorldTransform.TransformPositionNoScale(FVector(Ctx.HubChassisTransform.GetLocation()));
}

bool FVehicleSuspensionSolver::SolveUpperWishbone(
	const FVector3f& LowerBallPos,
	float KnuckleLength,
	const FVector3f& UpperPivotPos,
	const FVector3f& UpperPivotAxis,
	const FVector3f& StrutDir,
	float UpperArmLength,
	FVector3f& OutUpperBallPos)
{
	FVector3f N = UpperPivotAxis.GetSafeNormal();
	FVector3f PivotToLower = LowerBallPos - UpperPivotPos;
	float Dist3D = PivotToLower.Size();

	// Lambda 闭包依然极其好用
	auto ApplyGracefulDegradation = [&]() {
		FVector3f DirToLower = PivotToLower.GetSafeNormal();
		if (DirToLower.IsNearlyZero()) { DirToLower = FVector3f(0.f, 0.f, -1.f); }

		OutUpperBallPos = UpperPivotPos + DirToLower * UpperArmLength;
		return false;
		};

	if (Dist3D >= UpperArmLength + KnuckleLength || Dist3D <= FMath::Abs(UpperArmLength - KnuckleLength))
		return ApplyGracefulDegradation();

	float d = FVector3f::DotProduct(PivotToLower, N);
	float KnuckleSq = FMath::Square(KnuckleLength);
	float dSq = FMath::Square(d);

	if (dSq > KnuckleSq)
		return ApplyGracefulDegradation();

	FVector3f Cp = LowerBallPos - d * N;
	float rpSq = KnuckleSq - dSq;
	float rp = FMath::Sqrt(FMath::Max(0.f, rpSq));

	FVector3f Vdiff = Cp - UpperPivotPos;
	float D = Vdiff.Size();

	if (D > UpperArmLength + rp || D < FMath::Abs(UpperArmLength - rp))
		return ApplyGracefulDegradation();

	float UpperArmSq = FMath::Square(UpperArmLength);
	float a = (UpperArmSq - rpSq + D * D) / (2.0f * D);
	float h = FMath::Sqrt(FMath::Max(0.0f, UpperArmSq - a * a));

	FVector3f VdiffDir = Vdiff / D;
	FVector3f Pmid = UpperPivotPos + a * VdiffDir;
	FVector3f DirPerp = FVector3f::CrossProduct(N, VdiffDir);

	FVector3f OutPos1 = Pmid + h * DirPerp;
	FVector3f OutPos2 = Pmid - h * DirPerp;

	FVector3f Pos1Dir = (OutPos1 - LowerBallPos).GetSafeNormal();
	FVector3f Pos2Dir = (OutPos2 - LowerBallPos).GetSafeNormal();

	// 输出
	OutUpperBallPos = FVector3f::DotProduct(Pos1Dir, StrutDir) > FVector3f::DotProduct(Pos2Dir, StrutDir) ? OutPos1 : OutPos2;

	return true;
}

void FVehicleSuspensionSolver::ComputeDoubleWishbone(
	FVehicleSuspensionSimContext& Ctx,
	const float WheelRadius,
	const FVehicleSuspensionKinematicsConfig& Config,
	const FVehicleSuspensionCachedLUTs& LUTs)
{
	/* Kinematics From Look Up Tables */
	float CompressionRatio = 1.f - Ctx.SuspensionExtensionRatio;
	FQuat4f RotLUT = FQuat4f(GetCamberToeCasterFromLUTs(LUTs, CompressionRatio, Ctx.WheelSideSign));

	/* Kinematics From Geometry */
	SolveLowerWishbone(Ctx, WheelRadius, Config);

	FVector3f UpperPivotLocationLocal = Config.UpperWishbone.MountLocalLocation;
	UpperPivotLocationLocal.Y *= Ctx.WheelSideSign;
	Ctx.UpperPivotChassisLocation =
		Ctx.WheelCompToChassisTransform.TransformPositionNoScale(UpperPivotLocationLocal);

	FVector3f UpperWishboneAxisLocal = Config.UpperWishbone.RotationLocalAxis.GetSafeNormal();
	UpperWishboneAxisLocal.Y *= Ctx.WheelSideSign;
	Ctx.UpperWishboneChassisAxis =
		Ctx.WheelCompToChassisTransform.TransformVectorNoScale(UpperWishboneAxisLocal);

	bool ValidUpperWishbone = SolveUpperWishbone(
		Ctx.LowerBallJointChassisLocation,
		Config.KnuckleLength,
		Ctx.UpperPivotChassisLocation,
		Ctx.UpperWishboneChassisAxis,
		Ctx.StrutChassisDirection,
		Config.UpperWishbone.Length,
		Ctx.UpperBallJointChassisLocation
	);

	Ctx.bValidKinematicsConfig = Ctx.bValidKinematicsConfig && ValidUpperWishbone;

	Ctx.SteerAxisChassisDirection = (Ctx.UpperBallJointChassisLocation - Ctx.LowerBallJointChassisLocation).GetSafeNormal();

	FVector3f BaseUp = FVector3f(0.f, 0.f, 1.f);
	FQuat4f SteerAxisBiasRotation = FQuat4f::FindBetweenNormals(BaseUp, Ctx.SteerAxisChassisDirection);
	FQuat4f SteeringBiasRotation = FQuat4f(Ctx.SteerAxisChassisDirection, FMath::DegreesToRadians(Ctx.SteeringAngle));

	FQuat4f SpindleMountRotation = GetSpindleMountQuat(
		Config.StaticSpindleRotation,
		Ctx.WheelSideSign
	);

	FQuat4f HubChassisRot = (SteeringBiasRotation * SteerAxisBiasRotation * RotLUT * SpindleMountRotation).GetNormalized();
	Ctx.HubChassisTransform.SetRotation(HubChassisRot);

	FVector3f HubOffset = Config.HubOffsetFromLowerJoint;
	HubOffset.Y *= Ctx.WheelSideSign;
	Ctx.HubOffsetFromLowerJointChassis = HubChassisRot.RotateVector(HubOffset);

	Ctx.HubChassisTransform.SetLocation(Ctx.LowerBallJointChassisLocation + Ctx.HubOffsetFromLowerJointChassis);

	FVector3f WheelChassisRightVec = Ctx.HubChassisTransform.GetRotation().GetRightVector();
	Ctx.WheelWorldRightVector = Ctx.ChassisWorldTransform.TransformVectorNoScale(FVector(WheelChassisRightVec));

	Ctx.HubWorldLocation = Ctx.ChassisWorldTransform.TransformPositionNoScale(FVector(Ctx.HubChassisTransform.GetLocation()));
}

void FVehicleSuspensionSolver::ComputeSolidAxle(
	FVehicleSuspensionSimContext& Ctx,
	const float WheelRadius,
	const FVehicleSuspensionKinematicsConfig& Config,
	const float TrackWidth,
	const FVector ThisWheelHitWorldLocation,
	const FVector OtherWheelHitWorldLocation)
{
	FVector AxleCenterWorld = (ThisWheelHitWorldLocation + OtherWheelHitWorldLocation) * 0.5f;
	FVector AxleDirectionWorld = (ThisWheelHitWorldLocation - OtherWheelHitWorldLocation).GetSafeNormal();
	AxleDirectionWorld *= Ctx.WheelSideSign;

	FVector3f AxleCenterChassis = (FVector3f)Ctx.ChassisWorldTransform.InverseTransformPositionNoScale(AxleCenterWorld);
	AxleCenterChassis.Y = 0.f;
	FVector3f AxleDirectionChassis = (FVector3f)Ctx.ChassisWorldTransform.InverseTransformVectorNoScale(AxleDirectionWorld);

	// 1. 获取下球头（车桥末端的主销点）的底盘局部坐标
	Ctx.LowerBallJointChassisLocation = AxleCenterChassis + AxleDirectionChassis * TrackWidth * 0.5f * Ctx.WheelSideSign;

	// 3. 计算车桥的侧倾旋转 (Roll)
	FVector3f DefaultRight = FVector3f(0.f, 1.f, 0.f);
	FQuat4f AxleRollRotation = FQuat4f::FindBetweenNormals(DefaultRight, AxleDirectionChassis);

	// 4. 计算转向轴 (Steer Axis / Kingpin)
	FVector3f ForwardChassis = Ctx.WheelCompToChassisTransform.GetRotation().GetForwardVector();
	Ctx.SteerAxisChassisDirection = FVector3f::CrossProduct(ForwardChassis, AxleDirectionChassis).GetSafeNormal();

	// 5. 应用转向角
	FQuat4f SteeringBiasRotation = FQuat4f(Ctx.SteerAxisChassisDirection, FMath::DegreesToRadians(Ctx.SteeringAngle));

	// 6. 最终的 Hub 旋转 = 转向旋转 * 车桥侧倾旋转
	FQuat4f HubChassisRot = SteeringBiasRotation * AxleRollRotation;
	Ctx.HubChassisTransform.SetRotation(HubChassisRot);

	// 7. 计算 Hub Offset 并应用
	FVector3f HubOffset = Config.HubOffsetFromLowerJoint;
	HubOffset.Y *= Ctx.WheelSideSign;
	Ctx.HubOffsetFromLowerJointChassis = HubChassisRot.RotateVector(HubOffset);

	// 8. 得到最终的车轮中心位置
	Ctx.HubChassisTransform.SetLocation(Ctx.LowerBallJointChassisLocation + Ctx.HubOffsetFromLowerJointChassis);

	// 9. 算出世界空间的 Right Vector 和 Hub Location 供后续物理受力和渲染使用
	FVector3f WheelChassisRightVec = Ctx.HubChassisTransform.GetRotation().GetRightVector();
	Ctx.WheelWorldRightVector = Ctx.ChassisWorldTransform.TransformVectorNoScale((FVector)WheelChassisRightVec);

	Ctx.HubWorldLocation = Ctx.ChassisWorldTransform.TransformPositionNoScale((FVector)Ctx.HubChassisTransform.GetLocation());

	// 10. 计算减震器方向（用于算弹簧力）
	Ctx.StrutChassisDirection = (Ctx.TopMountChassisLocation - Ctx.LowerBallJointChassisLocation).GetSafeNormal();

	// 11. (可选) 反推一个本地空间的点用于下一帧的初始参考
	FVector3f KnuckleLocalLocation = Ctx.WheelCompToChassisTransform.InverseTransformPositionNoScale(Ctx.LowerBallJointChassisLocation);
}

bool FVehicleSuspensionSolver::Solve2DLineIntersection(
	const FVector2f& P1, 
	const FVector2f& P2, 
	const FVector2f& P3, 
	const FVector2f& P4, 
	FVector2f& OutIntersection)
{
	float Denom = (P1.X - P2.X) * (P3.Y - P4.Y) - (P1.Y - P2.Y) * (P3.X - P4.X);

	// if parallel
	if (FMath::Abs(Denom) < 0.001f)
	{
		return false;
	}

	float t1 = P1.X * P2.Y - P1.Y * P2.X;
	float t2 = P3.X * P4.Y - P3.Y * P4.X;

	OutIntersection.X = (t1 * (P3.X - P4.X) - (P1.X - P2.X) * t2) / Denom;
	OutIntersection.Y = (t1 * (P3.Y - P4.Y) - (P1.Y - P2.Y) * t2) / Denom;

	return true;
}

float FVehicleSuspensionSolver::SolveSwingArmSlope2D(
	const FVector2f& LowerInner, 
	const FVector2f& LowerOuter, 
	const FVector2f& UpperInner, 
	const FVector2f& UpperOuter, 
	const FVector2f& ContactPatch)
{
	FVector2f IC;
	float Slope = 0.f;
	float DeltaX = 0.f;

	// if not parallel
	if (Solve2DLineIntersection(LowerInner, LowerOuter, UpperInner, UpperOuter, IC))
	{
		DeltaX = IC.X - ContactPatch.X;
		if (FMath::Abs(DeltaX) > 0.001f)
		{
			Slope = (IC.Y - ContactPatch.Y) / DeltaX;
		}
	}
	else
	{
		// use slope of lower wishbone if parallel
		DeltaX = LowerInner.X - LowerOuter.X;
		if (FMath::Abs(DeltaX) > 0.001f)
		{
			Slope = (LowerInner.Y - LowerOuter.Y) / DeltaX;
		}
	}
	return Slope;
}

float FVehicleSuspensionSolver::CalculateMacPhersonAntiPitchScale(
	const FVector3f& TopMount, const FVector3f& StrutDir,
	const FVector3f& LowerPivot, const FVector3f& LowerBallJoint, const FVector3f& LowerAxis,
	const FVector3f& HubLocation, float WheelRadius)
{
	// 1. 下摆臂的终极物理约束：计算球头瞬时速度
	FVector3f RadiusVec = LowerBallJoint - LowerPivot;
	FVector3f Vel = FVector3f::CrossProduct(LowerAxis, RadiusVec);

	// 速度向量 (Vx, Vz) 的 2D 垂直法线永远是 (-Vz, Vx)
	FVector2f P1(LowerBallJoint.X, LowerBallJoint.Z);
	FVector2f P2(P1.X - Vel.Z, P1.Y + Vel.X);

	// 2. 塔顶约束不变
	FVector2f Top2D(TopMount.X, TopMount.Z);
	FVector2f TopNormal2D(Top2D.X - StrutDir.Z * 100.f, Top2D.Y + StrutDir.X * 100.f);

	float Slope = SolveSwingArmSlope2D(P1, P2, Top2D, TopNormal2D, FVector2f(HubLocation.X, HubLocation.Z - WheelRadius));
	return Slope; // 纯斜率，不上绝对值
}

float FVehicleSuspensionSolver::CalculateMacPhersonRollCenterHeight(
	const FVector3f& TopMount, const FVector3f& StrutDir,
	const FVector3f& LowerPivot, const FVector3f& LowerBallJoint, const FVector3f& LowerAxis,
	const FVector3f& HubLocation, float WheelRadius)
{
	FVector3f RadiusVec = LowerBallJoint - LowerPivot;
	FVector3f Vel = FVector3f::CrossProduct(LowerAxis, RadiusVec);

	// 速度向量在 Y-Z 平面的垂直法线：(-Vz, Vy)
	FVector2f P1(LowerBallJoint.Y, LowerBallJoint.Z);
	FVector2f P2(P1.X - Vel.Z, P1.Y + Vel.Y);

	FVector2f Top2D(TopMount.Y, TopMount.Z);
	FVector2f TopNormal2D(Top2D.X - StrutDir.Z * 100.f, Top2D.Y + StrutDir.Y * 100.f);

	FVector2f Contact2D(HubLocation.Y, HubLocation.Z - WheelRadius);
	float Slope = SolveSwingArmSlope2D(P1, P2, Top2D, TopNormal2D, Contact2D);

	return Contact2D.Y - Slope * Contact2D.X;
}

float FVehicleSuspensionSolver::CalculateDoubleWishboneAntiPitchScale(
	const FVector3f& UpperPivot, const FVector3f& UpperBallJoint, const FVector3f& UpperAxis,
	const FVector3f& LowerPivot, const FVector3f& LowerBallJoint, const FVector3f& LowerAxis,
	const FVector3f& HubLocation, float WheelRadius)
{
	// 下摆臂 3D 速度投影
	FVector3f VelLower = FVector3f::CrossProduct(LowerAxis, LowerBallJoint - LowerPivot);
	FVector2f P1(LowerBallJoint.X, LowerBallJoint.Z);
	FVector2f P2(P1.X - VelLower.Z, P1.Y + VelLower.X);

	// 上摆臂 3D 速度投影
	FVector3f VelUpper = FVector3f::CrossProduct(UpperAxis, UpperBallJoint - UpperPivot);
	FVector2f P3(UpperBallJoint.X, UpperBallJoint.Z);
	FVector2f P4(P3.X - VelUpper.Z, P3.Y + VelUpper.X);

	float Slope = SolveSwingArmSlope2D(P1, P2, P3, P4, FVector2f(HubLocation.X, HubLocation.Z - WheelRadius));
	return Slope;
}

float FVehicleSuspensionSolver::CalculateDoubleWishboneRollCenterHeight(
	const FVector3f& UpperPivot, const FVector3f& UpperBallJoint, const FVector3f& UpperAxis,
	const FVector3f& LowerPivot, const FVector3f& LowerBallJoint, const FVector3f& LowerAxis,
	const FVector3f& HubLocation, float WheelRadius)
{
	FVector3f VelLower = FVector3f::CrossProduct(LowerAxis, LowerBallJoint - LowerPivot);
	FVector2f P1(LowerBallJoint.Y, LowerBallJoint.Z);
	FVector2f P2(P1.X - VelLower.Z, P1.Y + VelLower.Y); // (-Vz, Vy)

	FVector3f VelUpper = FVector3f::CrossProduct(UpperAxis, UpperBallJoint - UpperPivot);
	FVector2f P3(UpperBallJoint.Y, UpperBallJoint.Z);
	FVector2f P4(P3.X - VelUpper.Z, P3.Y + VelUpper.Y);

	FVector2f Contact2D(HubLocation.Y, HubLocation.Z - WheelRadius);
	float Slope = SolveSwingArmSlope2D(P1, P2, P3, P4, Contact2D);

	return Contact2D.Y - Slope * Contact2D.X;
}

void FVehicleSuspensionSolver::ComputeAntiPitchRollGeometry(
	FVehicleSuspensionSimContext& Ctx,
	Chaos::FRigidBodyHandle_Internal* ChassisHandle,
	const bool bOnlyFromLUTs,
	const EVehicleIndependentSuspensionType SuspensionType,
	const float WheelRadius,
	const FVehicleSuspensionCachedLUTs& LUTs,
	const FTransform& AsyncChassisWorldTransform,
	const FVector3f& TireForce)
{
	if (!Ctx.bHitGround)
	{
		Ctx.JackingForce = 0.f;
		return;
	}

	float Compression = 1.f - Ctx.SuspensionExtensionRatio;

	// get world com
	Chaos::FVec3 ChassisWorldCOM = ChassisHandle != nullptr ?
		Chaos::FParticleUtilitiesGT::GetCoMWorldPosition(ChassisHandle) : AsyncChassisWorldTransform.GetLocation();

	// get arm
	Chaos::FVec3 LeverArmVec = Ctx.HitResult.ImpactPoint - ChassisWorldCOM;
	float LeverArmLengthSq = LeverArmVec.SquaredLength();

	// get torque caused by tire force
	Chaos::FVec3 InducedTorqueWorld = Chaos::FVec3::CrossProduct(LeverArmVec, TireForce);

	// transform torque into Chassis space
	Chaos::FVec3 InducedTorqueLocal = AsyncChassisWorldTransform.GetRotation().UnrotateVector(InducedTorqueWorld);

	float GeomAntiPitch = 0.f;
	float GeomAntiRoll = 0.f;
	if (!bOnlyFromLUTs)
	{
		FVector LeverArmVecChassis = AsyncChassisWorldTransform.InverseTransformVectorNoScale(LeverArmVec);

		float GroundZ = Ctx.HubChassisTransform.GetLocation().Z - WheelRadius;

		float COM_Z = AsyncChassisWorldTransform.InverseTransformPositionNoScale(ChassisWorldCOM).Z;

		float TrueCOMHeight = COM_Z - GroundZ;

		float RollCenterHeight = 0.f;
		float GeomSlope = 0.f;
		float TrueRCHeight = 0.f;
		switch (SuspensionType)
		{
		case EVehicleIndependentSuspensionType::StraightLine:
			break;
		case EVehicleIndependentSuspensionType::Macpherson:
			GeomSlope = CalculateMacPhersonAntiPitchScale(
				Ctx.TopMountChassisLocation,
				Ctx.StrutChassisDirection,
				Ctx.LowerPivotChassisLocation,
				Ctx.LowerBallJointChassisLocation,
				Ctx.LowerWishboneChassisAxis,
				Ctx.HubChassisTransform.GetLocation(),
				WheelRadius
			);
			GeomAntiPitch = GeomSlope * UVehicleUtilities::SafeDivide(LeverArmVecChassis.X, LeverArmVecChassis.Z);
			RollCenterHeight = CalculateMacPhersonRollCenterHeight(
				Ctx.TopMountChassisLocation,
				Ctx.StrutChassisDirection,
				Ctx.LowerPivotChassisLocation,
				Ctx.LowerBallJointChassisLocation,
				Ctx.LowerWishboneChassisAxis,
				Ctx.HubChassisTransform.GetLocation(),
				WheelRadius
			);
			TrueRCHeight = RollCenterHeight - GroundZ;
			GeomAntiRoll = UVehicleUtilities::SafeDivide(TrueRCHeight, TrueCOMHeight);
			break;
		case EVehicleIndependentSuspensionType::DoubleWishbone:
			GeomSlope = CalculateDoubleWishboneAntiPitchScale(
				Ctx.UpperPivotChassisLocation,
				Ctx.UpperBallJointChassisLocation,
				Ctx.UpperWishboneChassisAxis,
				Ctx.LowerPivotChassisLocation,
				Ctx.LowerBallJointChassisLocation,
				Ctx.LowerWishboneChassisAxis,
				Ctx.HubChassisTransform.GetLocation(),
				WheelRadius
			);
			GeomAntiPitch = GeomSlope * UVehicleUtilities::SafeDivide(LeverArmVecChassis.X, LeverArmVecChassis.Z);
			RollCenterHeight = CalculateDoubleWishboneRollCenterHeight(
				Ctx.UpperPivotChassisLocation,
				Ctx.UpperBallJointChassisLocation,
				Ctx.UpperWishboneChassisAxis,
				Ctx.LowerPivotChassisLocation,
				Ctx.LowerBallJointChassisLocation,
				Ctx.LowerWishboneChassisAxis,
				Ctx.HubChassisTransform.GetLocation(),
				WheelRadius
			);
			TrueRCHeight = RollCenterHeight - GroundZ;
			GeomAntiRoll = UVehicleUtilities::SafeDivide(TrueRCHeight, TrueCOMHeight);
			break;
		default:
			break;
		}
	}
	
	// anti-pitch from LUT
	bool bIsDiving = InducedTorqueLocal.Y > 0.f;
	float LUT_AntiPitch = bIsDiving ?
		LUTs.AntiDiveCurve.FastEval(Compression).Value :
		LUTs.AntiSquatCurve.FastEval(Compression).Value;

	// anti-roll from LUT
	float LUT_AntiRoll = LUTs.AntiRollCurve.FastEval(Compression).Value;

	// combine together
	Ctx.AntiPitchScale = GeomAntiPitch + LUT_AntiPitch;
	Ctx.AntiRollScale = GeomAntiRoll + LUT_AntiRoll;

	// get counter torque
	Chaos::FVec3 TargetCounterTorqueLocal =
		-Chaos::FVec3(InducedTorqueLocal.X * Ctx.AntiRollScale, InducedTorqueLocal.Y * Ctx.AntiPitchScale, 0.f);

	// get counter torque in world space
	Chaos::FVec3 TargetCounterTorqueWorld = AsyncChassisWorldTransform.GetRotation().RotateVector(TargetCounterTorqueLocal);

	// get normal of ground
	FVector ImpactNormal = Ctx.HitResult.Normal;

	// Torque Axis for Normal Force (1N)
	Chaos::FVec3 NormalTorqueAxis = Chaos::FVec3::CrossProduct(LeverArmVec, ImpactNormal);
	float EffectiveLeverArmSq = NormalTorqueAxis.SizeSquared();

	float MinLeverArmSq = FMath::Square(WheelRadius);
	EffectiveLeverArmSq = FMath::Max(EffectiveLeverArmSq, MinLeverArmSq);

	// get jacking force
	Ctx.JackingForce = UVehicleUtilities::SafeDivide(
		(float)Chaos::FVec3::DotProduct(TargetCounterTorqueWorld, NormalTorqueAxis),
		EffectiveLeverArmSq
	);

	Ctx.ForceAlongImpactNormal += Ctx.JackingForce;
}

void FVehicleSuspensionSolver::FetchChassisPhysicsState(Chaos::FRigidBodyHandle_Internal* ChassisHandle, FVehicleChassisSimState& OutState)
{
	if (!ChassisHandle || !ChassisHandle->CanTreatAsKinematic()) return;

	const bool bIsRigid = ChassisHandle->CanTreatAsRigid();

	OutState.CoMWorldLocation = bIsRigid ?
		Chaos::FParticleUtilitiesGT::GetCoMWorldPosition(ChassisHandle) :
		Chaos::FParticleUtilitiesGT::GetActorWorldTransform(ChassisHandle).GetTranslation();

	OutState.LinearVelocity = ChassisHandle->V();
	OutState.AngularVelocity = ChassisHandle->W();
	OutState.Mass = ChassisHandle->M();

	OutState.WorldInvInertiaTensor = Chaos::FParticleUtilitiesGT::GetWorldInvInertia(ChassisHandle);
}

void FVehicleSuspensionSolver::CalculateImpactPointWorldVelocity(
	FVehicleSuspensionSimContext& Ctx,
	const FVehicleChassisSimState& ChassisState)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_Wheel_SuspensionSolver_GetVelocity);

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
				LinVelWorldB = UAsyncTickFunctions::ATP_GetLinearVelocityAtPoint(
					HitComponent,
					Ctx.HitResult.ImpactPoint,
					Ctx.HitResult.BoneName
				);
			}
		}

		Ctx.ImpactWorldVelocity = FVector3f(0.01 * (LinVelWorldA - LinVelWorldB));
	}
	else
	{
		Ctx.ImpactWorldVelocity = FVector3f(0.f);
	}
}

Chaos::FVec3 FVehicleSuspensionSolver::CalculatePointEffectiveMass3D(
	const Chaos::FReal TotalMass, 
	const Chaos::FMatrix33& WorldInvInertiaTensor, 
	const Chaos::FVec3& CoM_WorldLocation, 
	const Chaos::FVec3& Contact_WorldLocation, 
	Chaos::FVec3 ImpactNormal, 
	Chaos::FVec3 WheelRight)
{
	const Chaos::FReal LinearInvMass =
		(Chaos::FReal)1.0 /
		FMath::Max(TotalMass, UE_SMALL_NUMBER);

	if (!ImpactNormal.Normalize())
	{
		return Chaos::FVec3(0);
	}

	if (!WheelRight.Normalize())
	{
		return Chaos::FVec3(0);
	}

	// LatDir = wheel axis projected onto contact plane
	Chaos::FVec3 LatDir =
		WheelRight -
		Chaos::FVec3::DotProduct(WheelRight, ImpactNormal)
		* ImpactNormal;

	if (!LatDir.Normalize())
	{
		const Chaos::FVec3 Ref =
			(FMath::Abs(ImpactNormal.Z) < (Chaos::FReal)0.999)
			? Chaos::FVec3(0, 0, 1)
			: Chaos::FVec3(1, 0, 0);

		LatDir = Chaos::FVec3::CrossProduct(Ref, ImpactNormal);

		if (!LatDir.Normalize())
		{
			return Chaos::FVec3(0);
		}
	}

	// Longitudinal direction
	Chaos::FVec3 LongDir =
		Chaos::FVec3::CrossProduct(WheelRight, ImpactNormal);

	if (!LongDir.Normalize())
	{
		LongDir =
			Chaos::FVec3::CrossProduct(LatDir, ImpactNormal);

		if (!LongDir.Normalize())
		{
			return Chaos::FVec3(0);
		}
	}

	const Chaos::FVec3 r =
		Contact_WorldLocation - CoM_WorldLocation;

	const auto ComputeEffectiveMass =
		[&](const Chaos::FVec3& Dir)
		{
			const Chaos::FVec3 RxD =
				Chaos::FVec3::CrossProduct(r, Dir);

			// 3x3 matrix multiply
			const Chaos::FVec3 IInvRxD =
				WorldInvInertiaTensor * RxD;

			const Chaos::FReal InvMass =
				LinearInvMass +
				Chaos::FVec3::DotProduct(RxD, IInvRxD);

			return (Chaos::FReal)1.0 /
				FMath::Max(InvMass, UE_SMALL_NUMBER);
		};

	return Chaos::FVec3(
		ComputeEffectiveMass(LongDir),
		ComputeEffectiveMass(LatDir),
		ComputeEffectiveMass(ImpactNormal));
}

float FVehicleSuspensionSolver::GetCriticalDamping(
	const float SpringStiffness,
	const float StaticSprungMass)
{
	if (StaticSprungMass <= 0.f)
	{
		//UE_LOG(LogTemp, Warning, TEXT("VehicleSuspensionSolver: SprungMass not valid!"));
		return 0.1f * SpringStiffness;
	}
	else
	{
		float NaturalFrequency = FMath::Sqrt(SpringStiffness * 100.f / StaticSprungMass);
		return 0.02f * StaticSprungMass * NaturalFrequency;
	}
}

void FVehicleSuspensionSolver::ComputeSuspensionForce(
	FVehicleSuspensionSimContext& Ctx,
	const float WheelRadius,
	const FVehicleChassisSimState& ChassisState,
	const FVehicleSuspensionSpringConfig& SpringConfig,
	const FVehicleSuspensionKinematicsConfig& KineConfig,
	const FVehicleSuspensionCachedLUTs& LUTs)
{
	const Chaos::FVec3 EffectiveMass = CalculatePointEffectiveMass3D(
		ChassisState.Mass,
		ChassisState.WorldInvInertiaTensor,
		ChassisState.CoMWorldLocation,
		Ctx.HitResult.ImpactPoint,
		Ctx.HitResult.Normal,
		Ctx.WheelWorldRightVector
	);
	Ctx.EffectiveSprungMassNormal = EffectiveMass.Z;

	float DeltaTimeInv = UVehicleUtilities::SafeDivide(1.f, Ctx.PhysicsDeltaTime);
	const float LastLength = Ctx.SuspensionCurrentLength;
	Ctx.SuspensionCurrentLength = KineConfig.Stroke * Ctx.SuspensionExtensionRatio;
	Ctx.StrutWorldDirection = Ctx.ChassisWorldTransform.TransformVectorNoScale((FVector)Ctx.StrutChassisDirection);

	if (!Ctx.bHitGround)
	{
		Ctx.ForceAlongImpactNormal = 0.f;
		Ctx.SuspensionForce = 0.f;
		Ctx.EffectiveSprungMassLong = EffectiveMass.X;
		Ctx.EffectiveSprungMassLat = EffectiveMass.Y;
		return;
	}

	const float CompressionRatio = 1.f - Ctx.SuspensionExtensionRatio;
	float MotionRatio = LUTs.MotionRatioCurve.FastEval(CompressionRatio).Value;

	const float EquivSpringStiffness = SpringConfig.SpringStiffness * MotionRatio * MotionRatio;
	const float MaxSpring = 4.f * Ctx.EffectiveSprungMassNormal * DeltaTimeInv * DeltaTimeInv;
	const float ActiveSpring = FMath::Min(MaxSpring, EquivSpringStiffness);
	float SpringForce = ActiveSpring * (KineConfig.Stroke - Ctx.SuspensionCurrentLength);

	float DamperStiffness = (Ctx.SuspensionCurrentLength > LastLength) ?
		SpringConfig.ReboundDamping : SpringConfig.CompressionDamping;
	if (SpringConfig.bUseDampingRatio)DamperStiffness *= GetCriticalDamping(SpringConfig.SpringStiffness, Ctx.StaticSprungMass);
	
	const float EquivDamperStiffness = DamperStiffness * MotionRatio * MotionRatio;
	const float MaxDamping = Ctx.EffectiveSprungMassNormal * DeltaTimeInv; // damper required to flip the sign of velocity in one frame
	const float ActiveDamping = FMath::Min(MaxDamping, EquivDamperStiffness);
	float DampingForce = ActiveDamping * (LastLength - Ctx.SuspensionCurrentLength) * DeltaTimeInv;

	Ctx.SuspensionForce = SpringForce + DampingForce;

	FVector ChassisUpVector = Ctx.ChassisWorldTransform.GetRotation().GetUpVector();
	FVector SwaybarForceVector = Ctx.SwaybarForce * ChassisUpVector;
	FVector SuspensionForceVector = Ctx.StrutWorldDirection * Ctx.SuspensionForce + SwaybarForceVector;
	float SuspensionForceProj = FVector::DotProduct(SuspensionForceVector, Ctx.HitResult.Normal);

	// some limits of constraint
	const float ConstraintScale = 0.99f;
	float VelocityAlongNormal = FVector::DotProduct(Ctx.HitResult.Normal, FVector(Ctx.ImpactWorldVelocity));
	float ImpulseAlongNormal = VelocityAlongNormal * Ctx.EffectiveSprungMassNormal;
	float ForceToBringToStop = FMath::Max(0.f, -ImpulseAlongNormal * DeltaTimeInv);
	float DynSprungMassGravity = Ctx.WorldGravityZ * Ctx.EffectiveSprungMassNormal;

	// if suspension is compeletly compressed
	if (Ctx.SuspensionCurrentLength < SMALL_NUMBER)
	{
		// try to stop the car immediately, only in the ray cast direction
		float ForceToHoldCar = ForceToBringToStop * ConstraintScale * SpringConfig.BottomOutStiffness;

		// the spring system in another direction (normal of impact surface)
		FVector WheelCenterToImpactPoint = Ctx.HubWorldLocation - Ctx.HitResult.ImpactPoint;
		float DistanceToSurface = FVector::DotProduct(Ctx.HitResult.Normal, WheelCenterToImpactPoint);
		SpringForce = (WheelRadius - DistanceToSurface) * ActiveSpring;
		DampingForce = -VelocityAlongNormal * ActiveDamping;
		ForceToHoldCar += SpringForce + DampingForce;
		SuspensionForceProj += ForceToHoldCar;
	}
	Ctx.ForceAlongImpactNormal += SuspensionForceProj;

	// spring preload
	float StrutProjOnNormal = FVector::DotProduct(Ctx.StrutWorldDirection, Ctx.HitResult.Normal);
	float NormalProjOnWorldUp = FVector::DotProduct(FVector::UpVector, Ctx.HitResult.Normal);

	float RawPreloadAlongSpring = SpringConfig.SpringPreload * MotionRatio;
	float RawPreloadAlongNormal = StrutProjOnNormal * RawPreloadAlongSpring;

	float MaxStaticPreloadAlongNormal = UVehicleUtilities::SafeDivide(DynSprungMassGravity, NormalProjOnWorldUp);

	float ImpulseScale = FMath::Clamp(FMath::Abs(CompressionRatio * 10.f), 0.f, 1.f);
	float MaxPreloadAlongNormal = MaxStaticPreloadAlongNormal - ImpulseAlongNormal * DeltaTimeInv * ImpulseScale - Ctx.ForceAlongImpactNormal;
	float ValidPreloadAlongNormal = FMath::Max(0.f, FMath::Min(MaxPreloadAlongNormal * ConstraintScale, RawPreloadAlongNormal));

	Ctx.ForceAlongImpactNormal += ValidPreloadAlongNormal;

	// get effective mass in other directions
	const float ForceIntoSurface = FMath::Max(Ctx.ForceAlongImpactNormal, 0.f);
	const float DynLoadMass = ForceIntoSurface / Ctx.WorldGravityZ;
	const float LoadFactor = UVehicleUtilities::SafeDivide(DynLoadMass, ChassisState.Mass);
	Ctx.EffectiveSprungMassLong = EffectiveMass.X * LoadFactor;
	Ctx.EffectiveSprungMassLat = EffectiveMass.Y * LoadFactor;
}
