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
	const float WheelInertia,
	const FVector3f& TireForce,
	const FVehicleSuspensionKinematicsConfig& KineConfig,
	const FVehicleSuspensionSpringConfig& SpringConfig,
	const FTransform& ComponentRelativeTransform,
	const FTransform& AsyncChassisWorldTransform,
	const FVehicleChassisSimState& ChassisState,
	const UWorld* CurrentWorld,
	const float InDeltaTime,
	const float InSteeringAngle,
	const float ActiveSwaybarStiffness,
	const float OtherHubChassisZ)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_Wheel_SuspensionSolver_UpdateSuspension);

	FVehicleSuspensionSimContext Ctx;

	CopyStateToContext(State, Ctx);

	Ctx.PhysicsDeltaTime = InDeltaTime;
	Ctx.SteeringAngle = InSteeringAngle;

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
		WheelInertia,
		QueryParams, 
		ResponseParams, 
		KineConfig
	);

	CalculateImpactPointWorldVelocity(
		Ctx,
		ChassisState
	);

	UpdateStrutLength(Ctx, ChassisState, 
		WheelRadius, WheelInertia, 
		KineConfig, SpringConfig, CachedLUTs,
		ActiveSwaybarStiffness, OtherHubChassisZ);

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

	ComputeAntiPitchRollGeometry(
		Ctx,
		ChassisState,
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
		CachedLUTs,
		ActiveSwaybarStiffness,
		OtherHubChassisZ
	);

	CopyContextToState(Ctx, State);
	CopyContextToHitResult(Ctx, RayCastResult);
}

void FVehicleSuspensionSolver::StartUpdateSolidAxle(
	const float WheelRadius,
	const float WheelWidth,
	const float WheelInertia,
	const FVehicleSuspensionKinematicsConfig& KineConfig,
	const FVehicleSuspensionSpringConfig& SpringConfig,
	const FTransform& ComponentRelativeTransform,
	const FTransform& AsyncChassisWorldTransform,
	const FVehicleChassisSimState& ChassisState,
	const UWorld* CurrentWorld,
	const float InSteeringAngle,
	const float ActiveSwaybarStiffness,
	const float OtherHubChassisZ,
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
	SuspensionRayCast(Ctx, CurrentWorld, WheelRadius, WheelWidth * 0.5f, WheelInertia, QueryParams, ResponseParams, KineConfig);

	CalculateImpactPointWorldVelocity(
		Ctx,
		ChassisState
	);

	UpdateStrutLength(Ctx, ChassisState, 
		WheelRadius, WheelInertia,
		KineConfig, SpringConfig, CachedLUTs, 
		ActiveSwaybarStiffness, OtherHubChassisZ);
}

void FVehicleSuspensionSolver::FinalizeUpdateSolidAxle(
	const float WheelRadius,
	const FVehicleSuspensionKinematicsConfig& KineConfig,
	const FVehicleSuspensionSpringConfig& SpringConfig,
	const FTransform& AsyncChassisWorldTransform,
	const FVehicleChassisSimState& ChassisState,
	float InDeltaTime,
	const float ActiveSwaybarStiffness,
	const float OtherHubChassisZ,
	const float AxleHalfWidth,
	const FVector3f& AxleChassisCenter,
	const FQuat4f& AxleChassisRotation,
	const FVector3f& TireForce,
	FVehicleSuspensionSimContext& Ctx)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_Wheel_SuspensionSolver_UpdateSuspension);

	Ctx.PhysicsDeltaTime = InDeltaTime;

	ComputeSolidAxle(
		Ctx,
		WheelRadius,
		KineConfig,
		AxleHalfWidth,
		AxleChassisCenter,
		AxleChassisRotation
	);

	ComputeAntiPitchRollGeometry(
		Ctx,
		ChassisState,
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
		CachedLUTs,
		ActiveSwaybarStiffness,
		OtherHubChassisZ
	);

	CopyContextToState(Ctx, State);
	CopyContextToHitResult(Ctx, RayCastResult);
}

void FVehicleSuspensionSolver::RoughlyInitializeState(const FTransform& ComponentRelativeTransform, const FVehicleSuspensionKinematicsConfig& KineConfig, FVehicleSuspensionSimState& InState)
{
	float SideSign = ComponentRelativeTransform.GetLocation().Y >= 0.f ? 1.f : -1.f;

	// 1. 在组件本地空间定义塔顶
	FVector3f TopMountLocalPos = KineConfig.TopMountLocalLocation;
	TopMountLocalPos.Y *= SideSign;

	// 2. 沿着组件自身的局部 Up 轴（或者摆臂轨迹面）向下做初始估计，而不是底盘 Z 轴
	float GuessStrutLen = KineConfig.MinStrutLength + KineConfig.Stroke;
	FVector3f InitialLowerBallJointLocal = TopMountLocalPos - FVector3f(0.f, 0.f, GuessStrutLen);

	// 3. 一次性统一转换到底盘空间，保证纵向 X_c 的线性同步，绝不污染方向向量
	FTransform3f CompToChassis = (FTransform3f)ComponentRelativeTransform;
	InState.LowerBallJointChassisLocation = CompToChassis.TransformPositionNoScale(InitialLowerBallJointLocal);

	FQuat4f SpindleMountRotation = GetSpindleMountQuat(
		KineConfig.StaticSpindleRotation,
		SideSign
	);

	FQuat4f HubChassisRot = (SpindleMountRotation).GetNormalized();
	InState.HubChassisRotation = HubChassisRot;

	// 4. 把 HubOffset 也跟着旋转过去，作为 Hub 的粗略位置
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
	Ctx.CurrentExtensionRatio = InExtensionRatio;
	Ctx.StrutCurrentLength = InExtensionRatio * KineConfig.Stroke;
	Ctx.StrutCurrentVelocity = 0.f;
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
	const FVehicleSuspensionSimState* PrevState,
	const float WheelRadius,
	const FVehicleSuspensionKinematicsConfig& KineConfig,
	const FTransform& ComponentRelativeTransform,
	float InExtensionRatio, 
	float InSteeringAngle,
	FVehicleSuspensionSimContext& Ctx)
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

	CopyStateToContext(NewState, Ctx);
	Ctx.CurrentExtensionRatio = InExtensionRatio;
	Ctx.StrutCurrentLength = InExtensionRatio * KineConfig.Stroke;
	Ctx.StrutCurrentVelocity = 0.f;
	Ctx.SteeringAngle = InSteeringAngle;

	FTransform ChassisWorldTransform = FTransform::Identity;

	PrepareSimulation(
		Ctx, 
		ComponentRelativeTransform, 
		ChassisWorldTransform,
		KineConfig
	);
	ComputeRayCastLocation(Ctx, KineConfig);
}

FVehicleSuspensionSimState FVehicleSuspensionSolver::FinalizeSolveSolidAxleAtExtension(
	const float WheelRadius,
	const FVehicleSuspensionKinematicsConfig& KineConfig,
	FVehicleSuspensionSimContext& Ctx,
	const float AxleHalfWidth,
	const FVector3f& AxleChassisCenter,
	const FQuat4f& AxleChassisRotation)
{
	ComputeSolidAxle(
		Ctx,
		WheelRadius,
		KineConfig,
		AxleHalfWidth,
		AxleChassisCenter,
		AxleChassisRotation
	);

	Ctx.StrutCurrentVelocity = 0.f;

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
	bool bDrawRayCast,
	bool bSolidAxle,
	UVehicleWheelComponent* OtherWheel)
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
		bSolidAxle = bSolidAxle && IsValid(OtherWheel);

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

		// draw strut
		DrawDebugLine(TempWorld, TopMountLocation, LowerBallJointLocation, FColor(255, 255, 0), false, Duration, 0, Thickness);

		// draw knuckle
		if (KineConfig.SuspensionType == EVehicleIndependentSuspensionType::DoubleWishbone && !bSolidAxle)
		{
			DrawDebugLine(TempWorld, LowerBallJointLocation, UpperBallJointLocation, FColor(0, 255, 255), false, Duration, 0, Thickness);
		}		
		DrawDebugLine(TempWorld, LowerBallJointLocation, HubWorldPos, FColor(0, 255, 255), false, Duration, 0, Thickness);

		// draw arm
		if (bSolidAxle)
		{
			FVector OtherBallJointLocation = 
				OtherWheel->GetChassisAsyncWorldTransform().TransformPositionNoScale(
				(FVector)OtherWheel->GetLowerBallJointChassisLocation());
			
			DrawDebugLine(TempWorld, LowerBallJointLocation, OtherBallJointLocation, FColor(0, 0, 255), false, Duration, 0, Thickness);
		}
		else
		{
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
		}
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
	NewState.bWheelOnGround = Context.bWheelOnGround;
	NewState.SteeringAngle = Context.SteeringAngle;
	NewState.StrutCurrentLength = Context.StrutCurrentLength;
	NewState.StrutCurrentVelocity = Context.StrutCurrentVelocity;
	NewState.VirtualUnsprungMass = Context.VirtualUnsprungMass;
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
	Context.StrutCurrentLength = PrevState.StrutCurrentLength;
	Context.StrutCurrentVelocity = PrevState.StrutCurrentVelocity;
	Context.StaticSprungMass = PrevState.StaticSprungMass;
	Context.HubChassisTransform.SetRotation(PrevState.HubChassisRotation);
	Context.HubChassisTransform.SetLocation(PrevState.HubChassisLocation);
	Context.LowerBallJointChassisLocation = PrevState.LowerBallJointChassisLocation;
	Context.UpperBallJointChassisLocation = PrevState.UpperBallJointChassisLocation;
	Context.ImpactWorldVelocity = PrevState.ImpactWorldVelocity;
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

void FVehicleSuspensionSolver::SolveSolidAxlePosture(
	const FVector3f& LeftTopMountChassis,
	const FVector3f& RightTopMountChassis,
	const float LeftStrutLength,
	const float RightStrutLength,
	const float AxleHalfWidth,
	FVector3f& OutAxleCenter,
	FQuat4f& OutAxleRotation)
{
	// 确定 YZ 解算平面的横向原点 (完美居中于两边塔顶)
	const float AxleCenterY = (LeftTopMountChassis.Y + RightTopMountChassis.Y) * 0.5f;
	const float AxleCenterX = (LeftTopMountChassis.X + RightTopMountChassis.X) * 0.5f;

	// 初始猜测值
	FVector3f GuessBallJoint_L = LeftTopMountChassis - FVector3f::UpVector * LeftStrutLength;
	FVector3f GuessBallJoint_R = RightTopMountChassis - FVector3f::UpVector * RightStrutLength;
	float CurrentAxleCenterZ = (GuessBallJoint_L.Z + GuessBallJoint_R.Z) * 0.5f;

	float GuessDeltaY = GuessBallJoint_L.Y - GuessBallJoint_L.Y;
	float GuessDeltaZ = GuessBallJoint_L.Z - GuessBallJoint_L.Z;
	float CurrentRollAngle = 0.f /* FMath::Atan2(GuessDeltaY, GuessDeltaZ) <-有奇点，不如直接把初始值设为 0 */;

	const float TargetLeftStrutSq = LeftStrutLength * LeftStrutLength;
	const float TargetRightStrutSq = RightStrutLength * RightStrutLength;

	// 最多 5 次迭代
	for (int32 Iteration = 0; Iteration < 5; ++Iteration)
	{
		float SinTheta = FMath::Sin(CurrentRollAngle);
		float CosTheta = FMath::Cos(CurrentRollAngle);

		// 基于当前猜测值，计算左右下球头的坐标
		float LeftBallJointY = AxleCenterY - AxleHalfWidth * CosTheta;
		float LeftBallJointZ = CurrentAxleCenterZ - AxleHalfWidth * SinTheta;

		float RightBallJointY = AxleCenterY + AxleHalfWidth * CosTheta;
		float RightBallJointZ = CurrentAxleCenterZ + AxleHalfWidth * SinTheta;

		// 计算塔顶到当前球头的空间向量差值
		float DeltaY_Left = LeftTopMountChassis.Y - LeftBallJointY;
		float DeltaZ_Left = LeftTopMountChassis.Z - LeftBallJointZ;

		float DeltaY_Right = RightTopMountChassis.Y - RightBallJointY;
		float DeltaZ_Right = RightTopMountChassis.Z - RightBallJointZ;

		// 计算残差 (Residuals)，目标是让它们逼近 0
		float Residual_Left = (DeltaY_Left * DeltaY_Left) + (DeltaZ_Left * DeltaZ_Left) - TargetLeftStrutSq;
		float Residual_Right = (DeltaY_Right * DeltaY_Right) + (DeltaZ_Right * DeltaZ_Right) - TargetRightStrutSq;

		// 偏导数/雅可比矩阵元素 (Jacobian Elements)
		// J11: Left 残差对 CenterZ 的导数
		float J11 = -2.0f * DeltaZ_Left;
		// J12: Left 残差对 RollAngle 的导数
		float J12 = -2.0f * DeltaY_Left * (AxleHalfWidth * SinTheta) + 2.0f * DeltaZ_Left * (AxleHalfWidth * CosTheta);

		// J21: Right 残差对 CenterZ 的导数
		float J21 = -2.0f * DeltaZ_Right;
		// J22: Right 残差对 RollAngle 的导数
		float J22 = 2.0f * DeltaY_Right * (AxleHalfWidth * SinTheta) - 2.0f * DeltaZ_Right * (AxleHalfWidth * CosTheta);

		// 矩阵行列式
		float Determinant = (J11 * J22) - (J12 * J21);

		// 如果遭遇数学奇点 Determinant = 0，说明姿态崩溃，SafeDivide默认返回0，会清除StepCenterZ和StepRollAngle
		float DeterminantInv = UVehicleUtilities::SafeDivide(1.f, Determinant);

		// 克莱姆法则求解增量
		float StepCenterZ = (J22 * Residual_Left - J12 * Residual_Right) * DeterminantInv;
		float StepRollAngle = (-J21 * Residual_Left + J11 * Residual_Right) * DeterminantInv;

		CurrentAxleCenterZ -= StepCenterZ;
		CurrentRollAngle -= StepRollAngle;

		// 收敛判断
		if (FMath::Abs(StepCenterZ) < SMALL_NUMBER && FMath::Abs(StepRollAngle) < SMALL_NUMBER)
		{
			break;
		}
	}

	OutAxleCenter = FVector3f(AxleCenterX, AxleCenterY, CurrentAxleCenterZ);
	OutAxleRotation = FQuat4f(FVector3f::ForwardVector, CurrentRollAngle);
}

void FVehicleSuspensionSolver::PrepareSimulation(
	FVehicleSuspensionSimContext& Ctx,
	const FTransform& ComponentRelativeTransform,
	const FTransform& AsyncChassisWorldTransform,
	const FVehicleSuspensionKinematicsConfig& Config)
{
	Ctx.WheelCompToChassisTransform = FTransform3f(ComponentRelativeTransform);
	if (!Ctx.WheelCompToChassisTransform.IsRotationNormalized())
	{
		Ctx.WheelCompToChassisTransform.NormalizeRotation();
	}
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
	FVector3f RayDirChassis = Ctx.WheelCompToChassisTransform.GetRotation().GetUpVector();
	Ctx.StrutChassisDirection = Ctx.WheelCompToChassisTransform.GetRotation().GetUpVector();
	Ctx.RayCastLength = Config.Stroke;

	Ctx.HubOffsetFromLowerJointChassis = Ctx.HubChassisTransform.GetLocation() - Ctx.LowerBallJointChassisLocation;
	Ctx.RayCastStartChassisLocation = Ctx.TopMountChassisLocation - RayDirChassis * Config.MinStrutLength + Ctx.HubOffsetFromLowerJointChassis;
	Ctx.RayCastStartWorldLocation = Ctx.ChassisWorldTransform.TransformPositionNoScale((FVector)Ctx.RayCastStartChassisLocation);

	FVector3f RayCastEndChassis = Ctx.RayCastStartChassisLocation - Ctx.RayCastLength * RayDirChassis;
	Ctx.RayCastEndWorldLocation = Ctx.ChassisWorldTransform.TransformPositionNoScale((FVector)RayCastEndChassis);

	FQuat SteeringRot = FQuat((FVector)RayDirChassis, FMath::DegreesToRadians(Ctx.SteeringAngle));
	FQuat RayCastRot = Ctx.ChassisWorldTransform.GetRotation() * SteeringRot * (FQuat)Ctx.WheelCompToChassisTransform.GetRotation();

	Ctx.RayCastWorldTransform = FTransform(RayCastRot, Ctx.RayCastEndWorldLocation, FVector(1.f));
	Ctx.RayCastDirectionWorld = Ctx.ChassisWorldTransform.TransformVectorNoScale((FVector)RayDirChassis);
}

void FVehicleSuspensionSolver::ComputeWishboneRayCastLocation(
	FVehicleSuspensionSimContext& Ctx,
	const FVehicleSuspensionKinematicsConfig& Config)
{
	FVector3f BaseUp = FVector3f(0.f, 0.f, 1.f);
	// Relative to chassis, sorry this is actually the opposite direction of the ray
	FVector3f RayDirChassis = Ctx.WheelCompToChassisTransform.TransformVectorNoScale(BaseUp);

	Ctx.HubOffsetFromLowerJointChassis = Ctx.HubChassisTransform.GetLocation() - Ctx.LowerBallJointChassisLocation;

	FVector3f TopMountToLowerBallJoint = Ctx.TopMountChassisLocation - Ctx.LowerBallJointChassisLocation;
	Ctx.StrutChassisDirection = TopMountToLowerBallJoint.GetSafeNormal();

	// parallel to ray
	float StrutProjOnRay = FVector3f::DotProduct(Ctx.StrutChassisDirection, RayDirChassis);
	float ReservedLength = FMath::Abs(StrutProjOnRay) * Config.MinStrutLength;
	Ctx.RayCastLength = FMath::Abs(StrutProjOnRay) * Config.Stroke;

	// vertical to ray
	FVector3f StrutVerticalToRay = FVector3f::VectorPlaneProject(TopMountToLowerBallJoint, RayDirChassis);

	FVector3f RayOffset = ReservedLength * -RayDirChassis - StrutVerticalToRay + Ctx.HubOffsetFromLowerJointChassis;

	Ctx.RayCastStartChassisLocation = Ctx.TopMountChassisLocation + RayOffset;
	Ctx.RayCastStartWorldLocation = Ctx.ChassisWorldTransform.TransformPositionNoScale((FVector)Ctx.RayCastStartChassisLocation);

	FVector3f RayCastEndChassis = Ctx.RayCastStartChassisLocation - Ctx.RayCastLength * RayDirChassis;
	Ctx.RayCastEndWorldLocation = Ctx.ChassisWorldTransform.TransformPositionNoScale((FVector)RayCastEndChassis);

	FQuat SteeringRot = FQuat((FVector)RayDirChassis, FMath::DegreesToRadians(Ctx.SteeringAngle));
	FQuat RayCastRot = Ctx.ChassisWorldTransform.GetRotation() * SteeringRot * (FQuat)Ctx.WheelCompToChassisTransform.GetRotation();

	Ctx.RayCastWorldTransform = FTransform(RayCastRot, Ctx.RayCastEndWorldLocation, FVector(1.f));
	// sorry this is actually the opposite direction of the ray
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
}

void FVehicleSuspensionSolver::UpdateStrutLength(
	FVehicleSuspensionSimContext& Ctx,
	const FVehicleChassisSimState& ChassisState,
	const float WheelRadius,
	const float WheelInertia,
	const FVehicleSuspensionKinematicsConfig& KineConfig,
	const FVehicleSuspensionSpringConfig& SpringConfig,
	const FVehicleSuspensionCachedLUTs& LUTs,
	const float ActiveSwaybarStiffness,
	const float OtherHubChassisZ)
{
	const float HitDistanceNoBias = FMath::Max(0.f, Ctx.HitDistance - WheelRadius);

	const float MaxExtension = FMath::Clamp(UVehicleUtilities::SafeDivide(HitDistanceNoBias, Ctx.RayCastLength), 0.f, 1.f);
	const float MaxCurrentLength = MaxExtension * KineConfig.Stroke;
	const float StrutLastLength = Ctx.StrutCurrentLength;
	Ctx.StrutLastLength = StrutLastLength;
	const float MacroDtInv = UVehicleUtilities::SafeDivide(1.f, Ctx.PhysicsDeltaTime);
	const float RayStrutVelocity = (MaxCurrentLength - StrutLastLength) * MacroDtInv;

	bool bSimulateUnsprungMass = KineConfig.SuspensionAndBrakeMass > SMALL_NUMBER;
	if (bSimulateUnsprungMass)
	{
		const float TargetSubDt = 1.f / 480.f;
		int32 Substeps = FMath::CeilToInt32(Ctx.PhysicsDeltaTime / TargetSubDt);
		Substeps = FMath::Max(Substeps, 1);
		const float SubDt = Ctx.PhysicsDeltaTime / (float)Substeps;

		// integrate unsprung mass position
		const float EstimatedWheelMass = UVehicleUtilities::SafeDivide(2.0f * WheelInertia, WheelRadius * WheelRadius * 0.0001f); // cm to m
		Ctx.VirtualUnsprungMass = EstimatedWheelMass + KineConfig.SuspensionAndBrakeMass;
		const float VirtualUnsprungMassInv = UVehicleUtilities::SafeDivide(1.f, Ctx.VirtualUnsprungMass);
		Ctx.StrutWorldDirection = Ctx.ChassisWorldTransform.TransformVectorNoScale((FVector)Ctx.StrutChassisDirection);
		const float GravityForce = Ctx.VirtualUnsprungMass * FMath::Abs(Ctx.WorldGravityZ) * FMath::Abs(Ctx.StrutWorldDirection.Z);

		const float CompressionRatio = 1.f - Ctx.CurrentExtensionRatio;
		float MotionRatio = LUTs.MotionRatioCurve.FastEval(CompressionRatio).Value;

		float Preload = SpringConfig.SpringPreload * MotionRatio;
		float EquivSpring = SpringConfig.SpringStiffness * MotionRatio * MotionRatio;
		float ReboundDamp = SpringConfig.ReboundDamping * MotionRatio * MotionRatio;
		float CompDamp = SpringConfig.CompressionDamping * MotionRatio * MotionRatio;

		const float ThisWheelChassisZ = Ctx.HubChassisTransform.GetLocation().Z;

		// 1. 获取质心到挂载点的世界空间向量 (r)
		FVector MountWorldPos = Ctx.ChassisWorldTransform.TransformPositionNoScale(FVector(Ctx.TopMountChassisLocation));
		FVector RadiusVec = MountWorldPos - FVector(ChassisState.CoMWorldLocation);

		// 2. 提取底盘状态
		FVector A_com = ChassisState.LinearAcceleration;
		FVector Alpha = ChassisState.AngularAcceleration;
		FVector Omega = ChassisState.AngularVelocity;

		// 3. 计算切向加速度: Alpha x r
		FVector TangentialAccel = FVector::CrossProduct(Alpha, RadiusVec);

		// 4. 计算向心加速度: Omega x (Omega x r)
		FVector CentripetalAccel = FVector::CrossProduct(Omega, FVector::CrossProduct(Omega, RadiusVec));

		// 5. 挂载点总绝对加速度
		FVector MountWorldAcceleration = A_com + TangentialAccel + CentripetalAccel;

		// 6. 投影到减震器（Strut）方向
		float StrutAcceleration = FVector::DotProduct(MountWorldAcceleration, Ctx.StrutWorldDirection);

		// 7. 计算伪惯性力 (F = -m * a)。单位：N
		float FictitiousForce = Ctx.VirtualUnsprungMass * StrutAcceleration * 0.01f;

		if (SpringConfig.bUseDampingRatio)
		{
			float CriticalDamping = GetCriticalDamping(SpringConfig.SpringStiffness, Ctx.StaticSprungMass);
			ReboundDamp *= CriticalDamping;
			CompDamp *= CriticalDamping;
		}

		float CurrentLength = Ctx.StrutCurrentLength;
		float CurrentVelocity = Ctx.StrutCurrentVelocity;
		float CurrentForce = 0.f;

		float StrutZProj = FMath::Abs(Ctx.StrutChassisDirection.Z);
		float EffectiveSwaybarStiffness = ActiveSwaybarStiffness * StrutZProj * StrutZProj;
		float TotalStiffness = EquivSpring + EffectiveSwaybarStiffness;

		const float m_to_cm = 100.f;

		for (int32 i = 0; i < Substeps; ++i)
		{
			float CurrentSubstepTime = (i + 1) * SubDt;
			float InterpolatedGroundLimit = StrutLastLength + RayStrutVelocity * CurrentSubstepTime;

			// 1. 计算当前的显式静态力 
			float SpringCompression = KineConfig.Stroke - CurrentLength;
			float SpringForce = EquivSpring * SpringCompression;

			float LengthDelta = CurrentLength - StrutLastLength;
			float SimulatedChassisZ = ThisWheelChassisZ - LengthDelta * StrutZProj;
			float ZDifference = SimulatedChassisZ - OtherHubChassisZ;
			float SwaybarForce = ActiveSwaybarStiffness * ZDifference * StrutZProj;

			float StaticForce = SpringForce + GravityForce + Preload + SwaybarForce + FictitiousForce;

			// 2. 基于显式力预测速度 ( 100.f 单位换算)
			float PredictedVelocity = CurrentVelocity + (StaticForce * VirtualUnsprungMassInv) * m_to_cm * SubDt;

			// 3. 全隐式求解分母 (Implicit Denominator)
			float EquivDamp = (PredictedVelocity > 0.f) ? ReboundDamp : CompDamp;

			// 隐式阻尼项: C * dt / m
			float ImplicitDampingTerm = EquivDamp * SubDt * VirtualUnsprungMassInv;

			// 隐式刚度项: K * dt^2 / m (需要带上和加速度相同的 100.f 单位换算)
			float ImplicitSpringTerm = TotalStiffness * VirtualUnsprungMassInv * SubDt * SubDt * m_to_cm;

			// 核心魔法：把刚度项加入分母，彻底消除震荡
			float DampingDenominator = 1.f + ImplicitDampingTerm + ImplicitSpringTerm;

			// 4. 更新最终速度与位置
			CurrentVelocity = PredictedVelocity / DampingDenominator;
			CurrentLength += CurrentVelocity * SubDt;

			// 运动学约束
			if (CurrentLength > InterpolatedGroundLimit)
			{
				CurrentLength = InterpolatedGroundLimit;
				if (CurrentVelocity > RayStrutVelocity)
				{
					CurrentVelocity = RayStrutVelocity;
				}
			}
			else if (CurrentLength <= 0.f)
			{
				CurrentLength = 0.f;
				CurrentVelocity = 0.f;
			}
		}

		Ctx.bWheelOnGround = CurrentLength >= MaxCurrentLength && Ctx.bHitGround;

		Ctx.StrutCurrentLength = CurrentLength;
		Ctx.StrutCurrentVelocity = (CurrentLength - StrutLastLength) * MacroDtInv;
		Ctx.CurrentExtensionRatio = UVehicleUtilities::SafeDivide(CurrentLength, KineConfig.Stroke);
	}
	else
	{
		Ctx.VirtualUnsprungMass = 0.f;
		Ctx.bWheelOnGround = Ctx.bHitGround;
		Ctx.StrutCurrentLength = MaxCurrentLength;
		Ctx.StrutCurrentVelocity = RayStrutVelocity;
		Ctx.CurrentExtensionRatio = MaxExtension;
	}
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

void FVehicleSuspensionSolver::SuspensionRayCast(
	FVehicleSuspensionSimContext& Ctx,
	const UWorld* World,
	const float WheelRadius,
	const float HalfWheelWidth,
	const float WheelInertia,
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
	FVector3f LowerPivotLocationLocal = Config.LowerWishbone.MountLocalLocation;
	LowerPivotLocationLocal.Y *= Ctx.WheelSideSign;
	Ctx.LowerPivotChassisLocation = Ctx.WheelCompToChassisTransform.TransformPositionNoScale(LowerPivotLocationLocal);

	FVector3f LowerArmAxisLocal = Config.LowerWishbone.RotationLocalAxis; // Relative to the wheel component, unnormalized
	LowerArmAxisLocal.Y *= Ctx.WheelSideSign;
	Ctx.LowerWishboneChassisAxis = Ctx.WheelCompToChassisTransform.TransformVectorNoScale(LowerArmAxisLocal).GetSafeNormal();

	// 1. 获取目标减震器长度（球面半径 L_s）
	float TargetStrutLength = Config.MinStrutLength + Ctx.StrutCurrentLength;
	float StrutLenSq = TargetStrutLength * TargetStrutLength;

	// 2. 获取摆臂参数（圆半径 R_arm）
	float RArm = Config.LowerWishbone.Length;
	float RArmSq = RArm * RArm;

	// 3. 提取三个已知的三维坐标/向量
	FVector3f Pivot = Ctx.LowerPivotChassisLocation;
	FVector3f Axis = Ctx.LowerWishboneChassisAxis;
	FVector3f TM = Ctx.TopMountChassisLocation;

	// D 是从塔顶指向下摆臂转轴的向量
	FVector3f D = Pivot - TM;
	float DSq = D.SquaredLength();

	// 4. 空间解析几何核心方程：(Ls^2 - R^2 - ||D||^2) / 2
	float K = (StrutLenSq - RArmSq - DSq) * 0.5f;

	// D 在垂直于摆臂旋转轴平面上的投影
	FVector3f DProj = D - FVector3f::DotProduct(D, Axis) * Axis;
	float DProjLenSq = DProj.SquaredLength();

	bool bValidIntersection = false;

	// 防止塔顶恰好在摆臂旋转轴上导致除以零（现实中减震器不可能这样安装）
	if (DProjLenSq > SMALL_NUMBER)
	{
		float DProjLen = FMath::Sqrt(DProjLenSq);
		FVector3f U = DProj / DProjLen; // 投影面上的局部基底 X 轴

		float Du = K / DProjLen;

		// 判别式：检查减震器是否能够触及摆臂的运动圆环
		if (Du * Du <= RArmSq)
		{
			bValidIntersection = true;

			// Dv 是正交方向的分量
			float Dv = FMath::Sqrt(RArmSq - Du * Du);
			FVector3f VDir = FVector3f::CrossProduct(Axis, U).GetSafeNormal(); // 投影面上的局部基底 Y 轴

			// 得出两个绝对精确的解析解
			FVector3f P1 = Pivot + Du * U + Dv * VDir;
			FVector3f P2 = Pivot + Du * U - Dv * VDir;

			// 对比上一帧的下球头位置，选择距离最近的解，保持空间运动的连续性
			float Dist1Sq = (P1 - Ctx.LowerBallJointChassisLocation).SquaredLength();
			float Dist2Sq = (P2 - Ctx.LowerBallJointChassisLocation).SquaredLength();

			Ctx.LowerBallJointChassisLocation = (Dist1Sq < Dist2Sq) ? P1 : P2;
		}
		else
		{
			// 安全降级：当减震器过长或过短，导致球面和圆没有交点时。
			// 我们将坐标钳制（Clamp）在摆臂圆周上距离减震器目标球体最近的那个物理极值点。
			Ctx.LowerBallJointChassisLocation = Pivot + RArm * FMath::Sign(Du) * U;
		}
	}
	else
	{
		// 极其罕见的奇异状态兜底：保持上一次的方向并限制在摆臂长度上
		FVector3f OldV = Ctx.LowerBallJointChassisLocation - Pivot;
		FVector3f OldVProj = OldV - FVector3f::DotProduct(OldV, Axis) * Axis;
		if (OldVProj.SquaredLength() > SMALL_NUMBER)
		{
			Ctx.LowerBallJointChassisLocation = Pivot + OldVProj.GetSafeNormal() * RArm;
		}
	}

	// 5. 更新上下文状态
	Ctx.StrutChassisDirection = (Ctx.TopMountChassisLocation - Ctx.LowerBallJointChassisLocation).GetSafeNormal();
	Ctx.bValidKinematicsConfig = Ctx.bValidKinematicsConfig && bValidIntersection;
}

void FVehicleSuspensionSolver::ComputeStraightSuspension(
	FVehicleSuspensionSimContext& Ctx,
	const float WheelRadius,
	const FVehicleSuspensionKinematicsConfig& Config,
	const FVehicleSuspensionCachedLUTs& LUTs)
{
	/* Kinematics From Look Up Tables */
	float CompressionRatio = 1.f - Ctx.CurrentExtensionRatio;
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

	Ctx.LowerBallJointChassisLocation = Ctx.TopMountChassisLocation - Ctx.StrutChassisDirection * (Config.MinStrutLength + Ctx.StrutCurrentLength);
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
	float CompressionRatio = 1.f - Ctx.CurrentExtensionRatio;
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
	float a = UVehicleUtilities::SafeDivide(UpperArmSq - rpSq + D * D, 2.0f * D);
	float h = FMath::Sqrt(FMath::Max(0.0f, UpperArmSq - a * a));

	FVector3f VdiffDir = UVehicleUtilities::SafeDivide(Vdiff, D);
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
	float CompressionRatio = 1.f - Ctx.CurrentExtensionRatio;
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
	const float AxleHalfWidth,
	const FVector3f& AxleChassisCenter,
	const FQuat4f& AxleChassisRotation)
{
	// 4. 转向轴 (Steer Axis / Kingpin)
	FVector3f DefaultForward = FVector3f(1.f, 0.f, 0.f);
	FVector3f AxleDirectionChassis = AxleChassisRotation.RotateVector(FVector3f::RightVector);
	Ctx.LowerBallJointChassisLocation = AxleChassisCenter + AxleDirectionChassis * AxleHalfWidth * Ctx.WheelSideSign;
	Ctx.SteerAxisChassisDirection = FVector3f::CrossProduct(DefaultForward, AxleDirectionChassis).GetSafeNormal();

	// 5. 应用转向角
	FQuat4f SteeringBiasRotation = FQuat4f(Ctx.SteerAxisChassisDirection, FMath::DegreesToRadians(Ctx.SteeringAngle));

	// 6. 组合 Hub 旋转
	FQuat4f HubChassisRot = SteeringBiasRotation * AxleChassisRotation;
	Ctx.HubChassisTransform.SetRotation(HubChassisRot);

	// 7. 偏移到轮心
	FVector3f HubOffset = Config.HubOffsetFromLowerJoint;
	HubOffset.Y *= Ctx.WheelSideSign;
	Ctx.HubOffsetFromLowerJointChassis = HubChassisRot.RotateVector(HubOffset);

	Ctx.HubChassisTransform.SetLocation(Ctx.LowerBallJointChassisLocation + Ctx.HubOffsetFromLowerJointChassis);

	// 8. 缓存世界空间向量与减震器受力方向
	FVector3f WheelChassisRightVec = Ctx.HubChassisTransform.GetRotation().GetRightVector();
	Ctx.WheelWorldRightVector = Ctx.ChassisWorldTransform.TransformVectorNoScale((FVector)WheelChassisRightVec);
	Ctx.HubWorldLocation = Ctx.ChassisWorldTransform.TransformPositionNoScale((FVector)Ctx.HubChassisTransform.GetLocation());
	Ctx.StrutChassisDirection = (Ctx.TopMountChassisLocation - Ctx.LowerBallJointChassisLocation).GetSafeNormal();
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
	if (!(FMath::Abs(Denom) > SMALL_NUMBER))
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

	// get direction
	FVector2f DirLower = (LowerOuter - LowerInner).GetSafeNormal();
	FVector2f DirUpper = (UpperOuter - UpperInner).GetSafeNormal();

	// cross product
	float CrossProd = DirLower.X * DirUpper.Y - DirLower.Y * DirUpper.X;

	if (FMath::Abs(CrossProd) < KINDA_SMALL_NUMBER)
	{
		// [Parallel Reduction]
		// If the arms are nearly parallel, the instantaneous center (IC) lies at infinity.
		// The slope of the line from the point of contact to infinity is equal to the average slope of the arms.
		FVector2f AvgDir = (DirLower + DirUpper).GetSafeNormal();

		if (FMath::Abs(AvgDir.X) > KINDA_SMALL_NUMBER)
		{
			Slope = AvgDir.Y / AvgDir.X;
		}
		else
		{
			// If the control arm is completely perpendicular to the ground 
			// (an almost impossible suspension configuration)
			// Slope will be INF, so I skip the calculation
		}
	}
	else
	{
		if (Solve2DLineIntersection(LowerInner, LowerOuter, UpperInner, UpperOuter, IC))
		{
			DeltaX = IC.X - ContactPatch.X;

			if (FMath::Abs(DeltaX) > KINDA_SMALL_NUMBER)
			{
				Slope = (IC.Y - ContactPatch.Y) / DeltaX;
			}
			else
			{
				// The IC is directly above/below the contact point
				// Slope will be INF, so I skip the calculation
			}
		}
	}
	return Slope;
}

float FVehicleSuspensionSolver::CalculateMacPhersonPitchSlope(
	const FVector3f& TopMount, const FVector3f& StrutDir,
	const FVector3f& LowerPivot, const FVector3f& LowerBallJoint, const FVector3f& LowerAxis,
	const FVector3f& ImpactPointChassisLocation)
{	
	// Calculate the instantaneous velocity of the ball head
	FVector3f RadiusVec = LowerBallJoint - LowerPivot;
	FVector3f Vel = FVector3f::CrossProduct(LowerAxis, RadiusVec);

	// The 2D perpendicular normal to the velocity vectors (Vx, Vz) is always (-Vz, Vx)
	FVector2f P1(LowerBallJoint.X, LowerBallJoint.Z);
	FVector2f P2(P1.X - Vel.Z, P1.Y + Vel.X);

	// top mount constraint
	FVector2f Top2D(TopMount.X, TopMount.Z);
	FVector2f TopNormal2D(Top2D.X - StrutDir.Z * 100.f, Top2D.Y + StrutDir.X * 100.f);

	float Slope = SolveSwingArmSlope2D(P1, P2, Top2D, TopNormal2D, FVector2f(ImpactPointChassisLocation.X, ImpactPointChassisLocation.Z));
	return Slope;
}

float FVehicleSuspensionSolver::CalculateMacPhersonRollSlope(
	const FVector3f& TopMount, const FVector3f& StrutDir,
	const FVector3f& LowerPivot, const FVector3f& LowerBallJoint, const FVector3f& LowerAxis,
	const FVector3f& ImpactPointChassisLocation)
{
	FVector3f RadiusVec = LowerBallJoint - LowerPivot;
	FVector3f Vel = FVector3f::CrossProduct(LowerAxis, RadiusVec);

	FVector2f P1(LowerBallJoint.Y, LowerBallJoint.Z);
	FVector2f P2(P1.X - Vel.Z, P1.Y + Vel.Y);

	FVector2f Top2D(TopMount.Y, TopMount.Z);
	FVector2f TopNormal2D(Top2D.X - StrutDir.Z * 100.f, Top2D.Y + StrutDir.Y * 100.f);

	FVector2f Contact2D(ImpactPointChassisLocation.Y, ImpactPointChassisLocation.Z);
	return SolveSwingArmSlope2D(P1, P2, Top2D, TopNormal2D, Contact2D);
}

float FVehicleSuspensionSolver::CalculateDoubleWishbonePitchSlope(
	const FVector3f& UpperPivot, const FVector3f& UpperBallJoint, const FVector3f& UpperAxis,
	const FVector3f& LowerPivot, const FVector3f& LowerBallJoint, const FVector3f& LowerAxis,
	const FVector3f& ImpactPointChassisLocation)
{
	// Lower Control Arm 3D Velocity Projection
	FVector3f VelLower = FVector3f::CrossProduct(LowerAxis, LowerBallJoint - LowerPivot);
	FVector2f P1(LowerBallJoint.X, LowerBallJoint.Z);
	FVector2f P2(P1.X - VelLower.Z, P1.Y + VelLower.X);

	// Upper control arm 3D velocity projection
	FVector3f VelUpper = FVector3f::CrossProduct(UpperAxis, UpperBallJoint - UpperPivot);
	FVector2f P3(UpperBallJoint.X, UpperBallJoint.Z);
	FVector2f P4(P3.X - VelUpper.Z, P3.Y + VelUpper.X);

	float Slope = SolveSwingArmSlope2D(P1, P2, P3, P4, FVector2f(ImpactPointChassisLocation.X, ImpactPointChassisLocation.Z));
	return Slope;
}

float FVehicleSuspensionSolver::CalculateDoubleWishboneRollSlope(
	const FVector3f& UpperPivot, const FVector3f& UpperBallJoint, const FVector3f& UpperAxis,
	const FVector3f& LowerPivot, const FVector3f& LowerBallJoint, const FVector3f& LowerAxis,
	const FVector3f& ImpactPointChassisLocation)
{
	FVector3f VelLower = FVector3f::CrossProduct(LowerAxis, LowerBallJoint - LowerPivot);
	FVector2f P1(LowerBallJoint.Y, LowerBallJoint.Z);
	FVector2f P2(P1.X - VelLower.Z, P1.Y + VelLower.Y);

	FVector3f VelUpper = FVector3f::CrossProduct(UpperAxis, UpperBallJoint - UpperPivot);
	FVector2f P3(UpperBallJoint.Y, UpperBallJoint.Z);
	FVector2f P4(P3.X - VelUpper.Z, P3.Y + VelUpper.Y);

	FVector2f Contact2D(ImpactPointChassisLocation.Y, ImpactPointChassisLocation.Z);
	return SolveSwingArmSlope2D(P1, P2, P3, P4, Contact2D);
}

void FVehicleSuspensionSolver::ComputeAntiPitchRollGeometry(
	FVehicleSuspensionSimContext& Ctx,
	const FVehicleChassisSimState& ChassisState,
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

	float Compression = 1.f - Ctx.CurrentExtensionRatio;

	// get world com
	const FVector& ChassisWorldCOM = ChassisState.CoMWorldLocation;

	// tire force in chassis space
	FVector TireForceChassis = AsyncChassisWorldTransform.InverseTransformVectorNoScale((FVector)TireForce);

	// get arm and com height
	FVector LeverArmVecChassis = AsyncChassisWorldTransform.InverseTransformVectorNoScale(Ctx.HitResult.ImpactPoint - ChassisWorldCOM);
	float DynamicLx = FMath::Abs(LeverArmVecChassis.X);
	float DynamicLy = FMath::Abs(LeverArmVecChassis.Y);
	FVector3f ImpactPointChassis = (FVector3f)AsyncChassisWorldTransform.InverseTransformPositionNoScale(Ctx.HitResult.ImpactPoint);
	float GroundZ = ImpactPointChassis.Z;
	float COM_Z = AsyncChassisWorldTransform.InverseTransformPositionNoScale(ChassisWorldCOM).Z;
	float TrueCOMHeight = FMath::Abs(COM_Z - GroundZ);

	float GeomPitchSlope = 0.f;
	float GeomRollSlope = 0.f;
	if (!bOnlyFromLUTs)
	{
		switch (SuspensionType)
		{
		case EVehicleIndependentSuspensionType::StraightLine:
			break;
		case EVehicleIndependentSuspensionType::Macpherson:
			GeomPitchSlope = CalculateMacPhersonPitchSlope(
				Ctx.TopMountChassisLocation,
				Ctx.StrutChassisDirection,
				Ctx.LowerPivotChassisLocation,
				Ctx.LowerBallJointChassisLocation,
				Ctx.LowerWishboneChassisAxis,
				ImpactPointChassis
			);
			GeomRollSlope = CalculateMacPhersonRollSlope(
				Ctx.TopMountChassisLocation,
				Ctx.StrutChassisDirection,
				Ctx.LowerPivotChassisLocation,
				Ctx.LowerBallJointChassisLocation,
				Ctx.LowerWishboneChassisAxis,
				ImpactPointChassis
			);
			break;
		case EVehicleIndependentSuspensionType::DoubleWishbone:
			GeomPitchSlope = CalculateDoubleWishbonePitchSlope(
				Ctx.UpperPivotChassisLocation,
				Ctx.UpperBallJointChassisLocation,
				Ctx.UpperWishboneChassisAxis,
				Ctx.LowerPivotChassisLocation,
				Ctx.LowerBallJointChassisLocation,
				Ctx.LowerWishboneChassisAxis,
				ImpactPointChassis
			);
			GeomRollSlope = CalculateDoubleWishboneRollSlope(
				Ctx.UpperPivotChassisLocation,
				Ctx.UpperBallJointChassisLocation,
				Ctx.UpperWishboneChassisAxis,
				Ctx.LowerPivotChassisLocation,
				Ctx.LowerBallJointChassisLocation,
				Ctx.LowerWishboneChassisAxis,
				ImpactPointChassis
			);
			break;
		default:
			break;
		}
	}

	float GeomAntiPitchRatio = GeomPitchSlope * UVehicleUtilities::SafeDivide(DynamicLx, TrueCOMHeight);
	float GeomAntiRollRatio = GeomRollSlope * UVehicleUtilities::SafeDivide(DynamicLy, TrueCOMHeight);
		
	// anti-pitch from LUT
	bool bIsBraking = TireForceChassis.X < 0.f;
	float LUT_AntiPitchRatio = bIsBraking ?
		LUTs.AntiDiveCurve.FastEval(Compression).Value :
		LUTs.AntiSquatCurve.FastEval(Compression).Value;

	// anti-roll from LUT
	float LUT_AntiRollRatio = LUTs.AntiRollCurve.FastEval(Compression).Value;

	float FinalAntiPitchRatio = GeomAntiPitchRatio + LUT_AntiPitchRatio;
	float FinalAntiRollRatio = GeomAntiRollRatio + LUT_AntiRollRatio;
	Ctx.AntiPitchScale = FinalAntiPitchRatio;
	Ctx.AntiRollScale = FinalAntiRollRatio;

	float FinalPitchSlope = FinalAntiPitchRatio * UVehicleUtilities::SafeDivide(TrueCOMHeight, DynamicLx);
	float FinalRollSlope = FinalAntiRollRatio * UVehicleUtilities::SafeDivide(TrueCOMHeight, DynamicLy);

	float JackingForceChassisZ = (TireForceChassis.X * FinalPitchSlope) + (TireForceChassis.Y * FinalRollSlope);

	FVector ChassisUpWorld = AsyncChassisWorldTransform.GetRotation().GetUpVector();
	float UpDotNormal = FMath::Max(0.1f, FVector::DotProduct(ChassisUpWorld, Ctx.HitResult.Normal));

	Ctx.JackingForce = JackingForceChassisZ * UpDotNormal;
	Ctx.ForceAlongImpactNormal += Ctx.JackingForce;
}

void FVehicleSuspensionSolver::CalculateImpactPointWorldVelocity(
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
	if (StaticSprungMass > SMALL_NUMBER)
	{
		float NaturalFrequency = FMath::Sqrt(SpringStiffness * 100.f / StaticSprungMass);
		return 0.02f * StaticSprungMass * NaturalFrequency;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("VehicleSuspensionSolver: SprungMass not valid!"));
		return 0.1f * SpringStiffness;
	}
}

void FVehicleSuspensionSolver::ComputeSuspensionForce(
	FVehicleSuspensionSimContext& Ctx,
	const float WheelRadius,
	const FVehicleChassisSimState& ChassisState,
	const FVehicleSuspensionSpringConfig& SpringConfig,
	const FVehicleSuspensionKinematicsConfig& KineConfig,
	const FVehicleSuspensionCachedLUTs& LUTs,
	const float ActiveSwaybarStiffness,
	const float OtherHubChassisZ)
{
	Ctx.StrutWorldDirection = Ctx.ChassisWorldTransform.TransformVectorNoScale((FVector)Ctx.StrutChassisDirection);
	float DeltaTimeInv = UVehicleUtilities::SafeDivide(1.f, Ctx.PhysicsDeltaTime);

	const Chaos::FVec3 EffectiveMass = CalculatePointEffectiveMass3D(
		ChassisState.Mass,
		ChassisState.WorldInvInertiaTensor,
		ChassisState.CoMWorldLocation,
		Ctx.HitResult.ImpactPoint,
		Ctx.HitResult.Normal,
		Ctx.WheelWorldRightVector
	);
	Ctx.EffectiveSprungMassNormal = EffectiveMass.Z;

	if (!Ctx.bWheelOnGround)
	{
		Ctx.ForceAlongImpactNormal = 0.f;
		Ctx.EffectiveSprungMassLong = EffectiveMass.X ;
		Ctx.EffectiveSprungMassLat = EffectiveMass.Y;
	}

	const float MaxSpring = 4.f * Ctx.EffectiveSprungMassNormal * DeltaTimeInv * DeltaTimeInv;
	// damper required to flip the sign of velocity in one frame
	const float MaxDamping = Ctx.EffectiveSprungMassNormal * DeltaTimeInv;

	const float CompressionRatio = 1.f - Ctx.CurrentExtensionRatio;
	const float SpringCompression = KineConfig.Stroke - Ctx.StrutCurrentLength;
	float MotionRatio = LUTs.MotionRatioCurve.FastEval(CompressionRatio).Value;

	const float EquivSpringStiffness = SpringConfig.SpringStiffness * MotionRatio * MotionRatio;
	const float ActiveSpring = FMath::Min(MaxSpring, EquivSpringStiffness);
	float SpringForce = ActiveSpring * SpringCompression;

	float DamperStiffness = (Ctx.StrutCurrentVelocity > 0.f) ?
		SpringConfig.ReboundDamping : SpringConfig.CompressionDamping;
	if (SpringConfig.bUseDampingRatio)DamperStiffness *= GetCriticalDamping(SpringConfig.SpringStiffness, Ctx.StaticSprungMass);
	
	const float EquivDamperStiffness = DamperStiffness * MotionRatio * MotionRatio;
	const float ActiveDamping = FMath::Min(MaxDamping, EquivDamperStiffness);
	float DampingForce = ActiveDamping * (-Ctx.StrutCurrentVelocity);

	Ctx.StrutForce = SpringForce + DampingForce;
	FVector StrutForceVector = Ctx.StrutWorldDirection * Ctx.StrutForce;

	// sway bar
	Ctx.SwaybarForce = (Ctx.HubChassisTransform.GetLocation().Z - OtherHubChassisZ) * ActiveSwaybarStiffness;
	FVector ChassisUpVector = Ctx.ChassisWorldTransform.GetRotation().GetUpVector();
	FVector SwaybarForceVector = Ctx.SwaybarForce * ChassisUpVector;

	Ctx.ForceAlongImpactNormal += FVector::DotProduct(StrutForceVector + SwaybarForceVector, Ctx.HitResult.Normal);

	// some limits of constraint
	const float ConstraintScale = 0.99f;
	const float MaxPenetration = 0.1f;
	const float ImpulseConstraintScale = FMath::Clamp(CompressionRatio / MaxPenetration, 0.f, 1.f);
	float VelocityAlongNormal = FVector::DotProduct(Ctx.HitResult.Normal, FVector(Ctx.ImpactWorldVelocity));
	float ImpulseAlongNormal = VelocityAlongNormal * Ctx.EffectiveSprungMassNormal;
	float NormalProjOnWorldUp = FVector::DotProduct(FVector::UpVector, Ctx.HitResult.Normal);
	float DynSprungMassGravity = Ctx.WorldGravityZ * Ctx.EffectiveSprungMassNormal;
	float ForceToCancelOutSprungWeight = UVehicleUtilities::SafeDivide(DynSprungMassGravity, NormalProjOnWorldUp);
	float ForceToStopSprungMass = -ImpulseAlongNormal * DeltaTimeInv;
	float NormalForceToHoldCar = ForceToCancelOutSprungWeight + ForceToStopSprungMass * ImpulseConstraintScale;
	NormalForceToHoldCar *= ConstraintScale;

	// spring preload
	float FullPreloadAlongSpring = SpringConfig.SpringPreload * MotionRatio;
	float StrutProjOnNormal = FVector::DotProduct(Ctx.StrutWorldDirection, Ctx.HitResult.Normal);
	float RawPreloadAlongNormal = StrutProjOnNormal * FullPreloadAlongSpring;
	float ValidPreloadAlongNormal = FMath::Max(0.f, FMath::Min(NormalForceToHoldCar, RawPreloadAlongNormal));

	Ctx.ForceAlongImpactNormal += ValidPreloadAlongNormal;

	// if suspension is compeletly compressed
	if (Ctx.StrutCurrentLength < SMALL_NUMBER)
	{
		// try to stop the car immediately, only in the ray cast direction
		float ForceToHoldCar = FMath::Max(0.f, ForceToStopSprungMass) * ConstraintScale * SpringConfig.BottomOutStiffness;

		// the spring system in another direction (normal of impact surface)
		FVector WheelCenterToImpactPoint = Ctx.HubWorldLocation - Ctx.HitResult.ImpactPoint;
		float DistanceToSurface = FVector::DotProduct(Ctx.HitResult.Normal, WheelCenterToImpactPoint);
		SpringForce = (WheelRadius - DistanceToSurface) * ActiveSpring;
		DampingForce = -VelocityAlongNormal * ActiveDamping;
		ForceToHoldCar += SpringForce + DampingForce;
		Ctx.ForceAlongImpactNormal += ForceToHoldCar;
	}

	// get effective mass in other directions
	const float ForceIntoSurface = FMath::Max(Ctx.ForceAlongImpactNormal, 0.f);
	const float DynLoadMass = ForceIntoSurface / Ctx.WorldGravityZ;
	const float LoadFactor = UVehicleUtilities::SafeDivide(DynLoadMass, ChassisState.Mass);
	Ctx.EffectiveSprungMassLong = EffectiveMass.X * LoadFactor;
	Ctx.EffectiveSprungMassLat = EffectiveMass.Y * LoadFactor;
}
