// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)


#include "VehicleWheelSolver.h"
#include "VehicleWheelComponent.h"
#include "AsyncTickFunctions.h"
#include "VehicleUtilities.h"

FVehicleWheelSolver::FVehicleWheelSolver()
{
}

FVehicleWheelSolver::~FVehicleWheelSolver()
{
}

void FVehicleWheelSolver::Initialize(const FVehicleTireConfig& TireConfig)
{
	UpdateCachedLUTs(TireConfig);
}

void FVehicleWheelSolver::PreStep(
	float InMacroDeltaTime,
	const FTransform& AsyncChassisWorldTransform,
	const FVehicleSuspensionSimState& SuspensionState, 
	const FVehicleWheelConfig& WheelConfig,
	const FVehicleTireConfig& TireConfig)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_Wheel_WheelSolver_UpdateWheel);

	FVehicleWheelSimContext& Context = CurrentContext;
	FVehicleWheelSimState& LocalState = State;

	Context.MacroDeltaTime = InMacroDeltaTime;
	Context.MacroDeltaTimeInv = UVehicleUtilities::SafeDivide(1.f, InMacroDeltaTime);

	Context.R = WheelConfig.Radius * 0.01f;
	Context.RInv = UVehicleUtilities::SafeDivide(1.f, Context.R);

	const FVector3f WheelRightVec = SuspensionState.WheelWorldRightVector;
	const FVector3f ImpactNormal = SuspensionState.ImpactWorldNormal;

	const FVector3f LongForceDirUnNorm = FVector3f::CrossProduct(WheelRightVec, ImpactNormal);
	const FVector3f LatForceDirUnNorm = FVector3f::VectorPlaneProject(WheelRightVec, ImpactNormal);
	Context.LongForceDir = LongForceDirUnNorm.GetSafeNormal();
	Context.LatForceDir = LatForceDirUnNorm.GetSafeNormal();

	// 这里的点乘其实就是获取向量长度
	// 我使用点乘只是为了提醒自己，我在用车轮转轴方向在地面上的投影长度来近似模拟camber造成的抓地力变化
	Context.LongForceScale = FVector3f::DotProduct(LongForceDirUnNorm, Context.LongForceDir);
	Context.LatForceScale = FVector3f::DotProduct(LatForceDirUnNorm, Context.LatForceDir);

	UpdateLinearVelocity(LocalState, Context.LongForceDir, Context.LatForceDir, SuspensionState.ImpactWorldVelocity);

	UpdateDynamicFrictionMultiplier(LocalState, Context, TireConfig, SuspensionState.ImpactFriction);

	LocalState.WheelLoad = FMath::Max(0.f, SuspensionState.ForceAlongImpactNormal);

	Context.GravityComp2D = CalculateGravityCompensationOnSlope(
		LocalState, Context,
		LocalState.WheelLoad,
		SuspensionState.bWheelOnGround,
		Context.LongForceDir,
		Context.LatForceDir
	);

	// get wheel load
	Context.AvailableGrip = CalculateAvailableGrip(
		LocalState.DynFrictionMultiplier,
		SuspensionState.StaticSprungMass,
		LocalState.WheelLoad,
		TireConfig.WheelLoadInfluenceFactor
	);

	// get camber
	Context.CamberLateralDrift = CalculateCamberLateralDrift(
		SuspensionState,
		AsyncChassisWorldTransform,
		WheelConfig,
		CachedLUTs,
		LocalState.SignedCamberDegree
	);

	// clear tire force
	Context.AccumulateTireImpulse2D = FVector2f(0.f);
}

void FVehicleWheelSolver::Substep(
	float InSubstepDeltaTime,
	float InDriveTorque,
	float InBrakeTorque,
	float InHandbrakeTorque,
	float InReflectedInertia,
	const FVehicleWheelConfig& WheelConfig,
	const FVehicleTireConfig& TireConfig,
	const FVehicleABSConfig& ABSConfig,
	const FVehicleSuspensionSimState& SuspensionState)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_Wheel_WheelSolver_UpdateWheel);

	const FVehicleWheelConfig& Config = WheelConfig;
	FVehicleWheelSimContext& Context = CurrentContext;
	FVehicleWheelSimState& LocalState = State;

	Context.SubstepDeltaTime = InSubstepDeltaTime;
	Context.SubstepDeltaTimeInv = UVehicleUtilities::SafeDivide(1.f, InSubstepDeltaTime);

	LocalState.EffectiveInertia = Config.Inertia + InReflectedInertia;
	LocalState.DriveTorque = InDriveTorque + LocalState.P4MotorTorque;

	float TargetBrakeTorque = FMath::Max(0.f, FMath::Abs(InBrakeTorque) + LocalState.BrakeTorqueFromESP);
	PredictSlipAndUpdateABS(LocalState, Context, ABSConfig, TargetBrakeTorque, SuspensionState.bWheelOnGround);

	LocalState.BrakeTorqueFromHandbrake = FMath::Abs(InHandbrakeTorque);
	LocalState.BrakeTorque = LocalState.BrakeTorqueFromBrake + Config.RollingRisistance + LocalState.BrakeTorqueFromHandbrake;

	// =========================================================
	// 1. Get tire force (using wheel speed from last frame)
	// =========================================================
	float ForceIntoSurface = FMath::Max(0.f, SuspensionState.ForceAlongImpactNormal);
	FVector2f SubstepForce2D = SolveTireForce(
		LocalState, Context,
		SuspensionState.StaticSprungMass,
		SuspensionState.EffectiveSprungMassLong,
		SuspensionState.EffectiveSprungMassLat,
		ForceIntoSurface,
		SuspensionState.bWheelOnGround,
		TireConfig,
		CachedLUTs
	);

	// =========================================================
	// 2. Get wheel speed
	// =========================================================
	const float SlipVelocityTolerance = 0.1f;
	WheelAcceleration(LocalState, Context, SubstepForce2D.X, SuspensionState.bWheelOnGround, SlipVelocityTolerance);

	Context.AccumulateTireImpulse2D += SubstepForce2D * Context.SubstepDeltaTime;
}

void FVehicleWheelSolver::PostStep()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_Wheel_WheelSolver_UpdateWheel);

	FVehicleWheelSimContext& Context = CurrentContext;
	FVehicleWheelSimState& LocalState = State;
	LocalState.TireForce2D = Context.AccumulateTireImpulse2D * Context.MacroDeltaTimeInv;
	LocalState.TireForce =
		LocalState.TireForce2D.X * Context.LongForceDir +
		LocalState.TireForce2D.Y * Context.LatForceDir;
}

void FVehicleWheelSolver::DrawWheelForce(
	UVehicleWheelComponent* WheelComponent,
	const FVehicleSuspensionSimState& SuspensionState,
	float Duration, 
	float Thickness, 
	float Length, 
	bool bDrawVelocity, 
	bool bDrawSlip, 
	bool bDrawInertia)
{
	if (!WheelComponent)return;

	UWorld* CurrentWorld = WheelComponent->GetWorld();

	const FVehicleTireConfig& TireConfig = WheelComponent->GetTireConfig();
	FVector WheelRightVec = FVector(SuspensionState.WheelWorldRightVector);
	FVector ImpactNormal = FVector(SuspensionState.ImpactWorldNormal);

	FVector TempForward = FVector::CrossProduct(WheelRightVec, ImpactNormal);
	FVector TempRight = FVector::VectorPlaneProject(WheelRightVec, ImpactNormal);
	FVector TempUp = ImpactNormal;
	FVector TempImpactPoint = SuspensionState.ImpactWorldLocation;
	FRotator TempRot = FRotationMatrix::MakeFromYZ(TempRight, TempForward).Rotator();
	const float CamberCos = FMath::Abs(TempRight.Size());
	FVector TempScale = FVector(TireConfig.MaxFx, TireConfig.MaxFy, 1.f) * CamberCos;
	FTransform TempTrans = FTransform(TempRot, TempImpactPoint, TempScale);

	Length *= 0.01;
	float AvailableGrip = Length * CalculateAvailableGrip(
		State.DynFrictionMultiplier,
		SuspensionState.StaticSprungMass,
		State.WheelLoad,
		TireConfig.WheelLoadInfluenceFactor
	);

	//draw grip circle
	FColor GripCircleColor = FColor(0, 191, 255);
	DrawDebugCircle(CurrentWorld, TempTrans.ToMatrixWithScale(), AvailableGrip, 16, GripCircleColor,
		false, Duration, 0, Thickness, false);

	//draw raw force
	FVector Long = TempForward * State.TireForce2D.X * Length;
	FVector Lat = TempRight * State.TireForce2D.Y * Length;
	FColor RawForceColor = FColor(0, 255, 191);
	DrawDebugLine(CurrentWorld, TempImpactPoint, TempImpactPoint + Long, RawForceColor, false, Duration, 0, Thickness);
	DrawDebugLine(CurrentWorld, TempImpactPoint, TempImpactPoint + Lat, RawForceColor, false, Duration, 0, Thickness);

	//draw final force
	DrawDebugLine(CurrentWorld, TempImpactPoint, (FVector)State.TireForce * Length + TempImpactPoint, GripCircleColor, false, Duration, 0, Thickness);

	FTransform HubChassisTransform = FTransform(WheelComponent->GetHubChassisTransform());
	FTransform WheelTrans = HubChassisTransform * WheelComponent->GetChassisAsyncWorldTransform();

	if (bDrawVelocity)
	{
		FColor VelociyColor = FColor(127, 255, 191);
		FColor StringColor = FColor(255 - VelociyColor.R, 255 - VelociyColor.G, 255 - VelociyColor.B);
		FVector TempOffset = WheelRightVec * (SuspensionState.bIsRightWheel ? 1.f : -1.f) * FMath::Abs(State.AngularVelocity) * Length * 100;
		FVector TempWheelLocation = WheelTrans.GetLocation();
		DrawDebugLine(CurrentWorld, TempWheelLocation, TempWheelLocation + TempOffset, VelociyColor, false, Duration, 0, Thickness);

		FString TempString = FString::SanitizeFloat(State.AngularVelocity);
		TempString = FString(TEXT("Omega = ")) + TempString;
		DrawDebugString(CurrentWorld, TempWheelLocation + TempOffset, TempString, 0, VelociyColor, Duration, true);
	}

	if (bDrawSlip)
	{
		FColor SlipColor = FColor(127, 63, 255);
		FVector TempWheelLocation = WheelTrans.GetLocation();
		FVector TempDrawOffset = WheelComponent->GetWheelConfig().Radius * WheelTrans.GetRotation().GetUpVector();
		FString TextSlipRatio = FString(TEXT("SlipRatio = ")) + FString::SanitizeFloat(State.SlipRatio);
		FString TextSlipAngle = FString(TEXT("SlipAngle = ")) + FString::SanitizeFloat(State.SlipAngle);
		DrawDebugString(CurrentWorld, TempWheelLocation + TempDrawOffset, TextSlipRatio, 0, SlipColor, Duration, true, Length * 100);
		DrawDebugString(CurrentWorld, TempWheelLocation - TempDrawOffset, TextSlipAngle, 0, SlipColor, Duration, true, Length * 100);
	}

	if (bDrawInertia)
	{
		FColor InertiaColor = FColor(255, 255, 255);
		FVector TempWheelLocation = WheelTrans.GetLocation();
		FString TextInertia = FString(TEXT("Inertia = ")) + FString::SanitizeFloat(State.EffectiveInertia);
		DrawDebugString(CurrentWorld, TempWheelLocation, TextInertia, 0, InertiaColor, Duration, true, Length * 100);
	}
}

void FVehicleWheelSolver::UpdateCachedLUTs(const FVehicleTireConfig& Config)
{
	if (IsValid(Config.Fx))
	{
		CachedLUTs.Fx.BuildFromCurve(Config.Fx->FloatCurve);
	}
	else
	{
		CachedLUTs.Fx.SetAllTo(1.f);
	}
	if (IsValid(Config.Fy))
	{
		CachedLUTs.Fy.BuildFromCurve(Config.Fy->FloatCurve, FVector2f(0.f, 90.f));
	}
	else
	{
		CachedLUTs.Fy.SetAllTo(1.f);
	}
	if (IsValid(Config.CamberToLateralDrift))
	{
		CachedLUTs.CamberToLateralDrift.CopyFromRichCurve(Config.CamberToLateralDrift->FloatCurve, FVector2f(0.f, 90.f));
	}
	else
	{
		CachedLUTs.CamberToLateralDrift.SetAllTo(0.f);
	}
}

float FVehicleWheelSolver::GetTangentAtOrigin(const FRichCurve& Curve)
{
	if (Curve.Keys.Num() == 0) return 10.f;

	const auto& Key0 = Curve.Keys[0];

	return FMath::IsNearlyZero(Key0.Time) ? Key0.LeaveTangent : 0.f;
}

void FVehicleWheelSolver::PredictSlipAndUpdateABS(
	FVehicleWheelSimState& LocalState,
	const FVehicleWheelSimContext& Context,
	const FVehicleABSConfig& ABSConfig,
	const float TargetBrakeTorque, 
	const bool bOnGround)
{
	// predict angular velocity
	float PredictedOmega = LocalState.AngularVelocity + LocalState.AngularAcceleration * Context.SubstepDeltaTime;
	float PredictedVSlip = PredictedOmega * Context.R - LocalState.LocalLinearVelocity.X;
	PredictedVSlip *= bOnGround;
	float Denominator = FMath::Max(FMath::Max(FMath::Abs(LocalState.LocalLinearVelocity.X), FMath::Abs(PredictedOmega * Context.R)), 1.f);
	LocalState.PredictedSlipRatio = PredictedVSlip / Denominator;

	float AbsolutSlip = FMath::Abs(LocalState.PredictedSlipRatio);
	// only activate abs when the sign of slip and the sign of velocity is different
	bool bDifferentSign = LocalState.PredictedSlipRatio * LocalState.LocalLinearVelocity.X < 0.f;

	LocalState.bABSTriggered = 
		ABSConfig.bAntiBrakeSystemEnabled
		&& TargetBrakeTorque > SMALL_NUMBER
		&& bOnGround
		&& FMath::Abs(LocalState.LocalLinearVelocity.X) > ABSConfig.ActivationSpeed
		&& AbsolutSlip > ABSConfig.OptimalSlip
		&& bDifferentSign;

	if (LocalState.bABSTriggered)
	{
		float Error = AbsolutSlip - ABSConfig.OptimalSlip;
		float AbsFactor = 1.0f - (Error * ABSConfig.Sensitivity);
		AbsFactor = FMath::Clamp(AbsFactor, 0.0f, 1.0f);
		LocalState.BrakeTorqueFromBrake = TargetBrakeTorque * AbsFactor;
		
		return;
	}

	LocalState.BrakeTorqueFromBrake = TargetBrakeTorque;
}

void FVehicleWheelSolver::UpdateDynamicFrictionMultiplier(
	FVehicleWheelSimState& LocalState,
	const FVehicleWheelSimContext& Context,
	const FVehicleTireConfig& TireConfig,
	const float ImpactFriction)
{
	switch (TireConfig.TireFrictionCombineMode)
	{
	case ETireFrictionCombineMode::Constant:
		LocalState.DynFrictionMultiplier = TireConfig.FrictionMultiplier;
		break;
	case ETireFrictionCombineMode::Average:
		LocalState.DynFrictionMultiplier = 0.5 * (TireConfig.FrictionMultiplier + ImpactFriction);
		break;
	case ETireFrictionCombineMode::Multiply:
		LocalState.DynFrictionMultiplier = TireConfig.FrictionMultiplier * ImpactFriction;
		break;
	case ETireFrictionCombineMode::Min:
		LocalState.DynFrictionMultiplier = FMath::Min(TireConfig.FrictionMultiplier, ImpactFriction);
		break;
	case ETireFrictionCombineMode::Max:
		LocalState.DynFrictionMultiplier = FMath::Max(TireConfig.FrictionMultiplier, ImpactFriction);
		break;
	default:
		LocalState.DynFrictionMultiplier = 0.5 * (TireConfig.FrictionMultiplier + ImpactFriction);
		break;
	}
}

void FVehicleWheelSolver::UpdateLinearVelocity(
	FVehicleWheelSimState& LocalState,
	const FVector3f& LongForceDir, 
	const FVector3f& LatForceDir, 
	const FVector3f& ImpactPointWorldVelocity)
{
	LocalState.LocalLinearVelocity.X = FVector3f::DotProduct(LongForceDir, ImpactPointWorldVelocity);
	LocalState.LocalLinearVelocity.Y = FVector3f::DotProduct(LatForceDir, ImpactPointWorldVelocity);
}

void FVehicleWheelSolver::WheelAcceleration(
	FVehicleWheelSimState& LocalState,
	const FVehicleWheelSimContext& Context,
	const float LastTireLongitudinalForce,
	const bool bOnGround,
	const float SlipVelocityTolerance)
{
	float LastAngularVelocity = LocalState.AngularVelocity;

	//friction torque should not flip the sign of the relative rotation to the ground
	//but there should be tolerance, because even when there is no drive torque or brake torque, the wheel should not always be completely sticked to the road
	//allow small angular acceleration tolerance to prevent sticky behavior when slip ~ 0 (empirical value)
	float ToleranceTorque = LocalState.EffectiveInertia * SlipVelocityTolerance * Context.SubstepDeltaTimeInv;

	//Get the torque required to flip the sign of relative rotation between ground and wheel
	float AngularLongSlip = LocalState.AngularVelocity - LocalState.LocalLinearVelocity.X * Context.RInv;
	float MaxFrictionTorque = AngularLongSlip * LocalState.EffectiveInertia * Context.SubstepDeltaTimeInv;

	//drive torque must be considered, but till now we cannot define the direction of the brake torque, so just donot take brake torque into account
	MaxFrictionTorque += LocalState.DriveTorque;
	MaxFrictionTorque = FMath::Abs(MaxFrictionTorque);

	//if there is no tolerance, there will be a bug when braking: the brake force will not be released since the longsilpvelocity == 0 and the smoothing factor == 0, so the longitudinal force will not reduce even when brake is released
	MaxFrictionTorque += ToleranceTorque;

	//clamp the friction torque, friction torque should not flip the sign of the relative rotation to the ground
	float FrictionTorque = LastTireLongitudinalForce * Context.R;
	float ClampedFrictionTorque = FMath::Clamp(FrictionTorque, -MaxFrictionTorque, MaxFrictionTorque);

	//since the brake torque is not considered when calculating max friction torque, we have to get the excess friction torque to deal with brake torque
	//if there is no excess friction torque, slip ratio will be very high when braking, since brake torque is not affected by friction
	//and again, the excess friction torque should not be greater than brake torque, since the friction torque should not flip the sign of the relative rotation to the ground
	float ExcessFrictionTorque = FMath::Clamp(FrictionTorque - ClampedFrictionTorque, -LocalState.BrakeTorque, LocalState.BrakeTorque);

	// avoid divided by 0
	float EffectiveInertiaInv = UVehicleUtilities::SafeDivide(1.f, LocalState.EffectiveInertia);

	//get the angular velocity without braking
	LocalState.AngularVelocity += Context.SubstepDeltaTime * EffectiveInertiaInv * (LocalState.DriveTorque - ClampedFrictionTorque);
	float AngVelSignIfNotBraking = FMath::Sign(LocalState.AngularVelocity);

	//finally the sign of brake torque can be defined
	float ActuralBrakingTorque = LocalState.BrakeTorque * (-AngVelSignIfNotBraking);
	LocalState.AngularVelocity += Context.SubstepDeltaTime * EffectiveInertiaInv * (ActuralBrakingTorque - ExcessFrictionTorque);

	//zero cross check
	//if the wheel is locked, the angular velocity should be 0
	LocalState.bIsLocked = AngVelSignIfNotBraking * LocalState.AngularVelocity <= 0.f && LocalState.BrakeTorque > SMALL_NUMBER;
	LocalState.AngularVelocity *= !LocalState.bIsLocked;

	// get angular acceleration
	LocalState.AngularAcceleration = (LocalState.AngularVelocity - LastAngularVelocity) * Context.SubstepDeltaTimeInv;
}

void FVehicleWheelSolver::UpdateSlipVelocity(
	FVehicleWheelSimState& LocalState,
	const FVehicleWheelSimContext& Context,
	const bool bOnGround
)
{
	const float Vx = LocalState.LocalLinearVelocity.X;
	const float Vy = LocalState.LocalLinearVelocity.Y;
	const float OmegaR = LocalState.AngularVelocity * Context.R;
	const float AbsOmegaR = FMath::Abs(OmegaR);

	const float q = bOnGround ? Context.CamberLateralDrift : 0.f;
	const float Norm = FMath::Sqrt(1.f + q * q);

	// Soft patch target. This does not mean rim direction changes.
	const float PatchVx = OmegaR / Norm;
	const float PatchVy = AbsOmegaR * q / Norm;

	LocalState.LongSlipVelocity = (PatchVx - Vx) * bOnGround;
	LocalState.LatSlipVelocity = (PatchVy - Vy) * bOnGround;

}

void FVehicleWheelSolver::UpdateSlipAngle(
	FVehicleWheelSimState& LocalState,
	const bool bOnGround)
{
	//get velocity2d
	FVector2f Velocity2DNormalized = LocalState.LocalLinearVelocity.GetSafeNormal();

	//calculate lateral slip
	//get slip angle
	float SlipAngleRaw = FMath::Asin(-Velocity2DNormalized.Y);
	SlipAngleRaw = FMath::RadiansToDegrees(SlipAngleRaw);

	//approximate low speed slip angle
	//-V.y = |V| * sin(SlipAngle) ≈ |V| * SlipAngle ≈ SlipAngle; when slip angle is small and |v| --> 1.f
	float LowSpeedSlipAngle = -FMath::RadiansToDegrees(LocalState.LocalLinearVelocity.Y);

	//get alpha for lerp, lerp between high speed and low speede
	float Alpha = FMath::GetMappedRangeValueClamped(FVector2f(0.01f, 0.1f), FVector2f(0.f, 1.f), LocalState.LocalLinearVelocity.SquaredLength());

	//combine low speed and high speed
	LocalState.SlipAngle = FMath::Lerp(LowSpeedSlipAngle, SlipAngleRaw, Alpha) * bOnGround;
	LocalState.SlipAngle = FMath::Clamp(LocalState.SlipAngle, -90.f, 90.f);
}

void FVehicleWheelSolver::UpdateSlipRatio(
	FVehicleWheelSimState& LocalState,
	const FVehicleWheelSimContext& Context,
	const bool bOnGround)
{
	float KinematicsLongSlipVelocity = LocalState.AngularVelocity * Context.R - LocalState.LocalLinearVelocity.X;
	KinematicsLongSlipVelocity *= bOnGround;
	float Denominator = FMath::Max(FMath::Max(FMath::Abs(LocalState.LocalLinearVelocity.X), FMath::Abs(LocalState.AngularVelocity * Context.R)), 1.f);
	LocalState.SlipRatio = KinematicsLongSlipVelocity / Denominator;
}

float FVehicleWheelSolver::CalculateCamberLateralDrift(
	const FVehicleSuspensionSimState& SuspensionState,
	const FTransform& AsyncChassisWorldTransform,
	const FVehicleWheelConfig& Config,
	const FVehicleWheelCachedLUTs& TireLUTs,
	float& OutSignedCamberDeg)
{
	if (!SuspensionState.bWheelOnGround)
	{
		OutSignedCamberDeg = 0.f;
		return 0.f;
	}

	const FVector3f WheelRight =
		SuspensionState.WheelWorldRightVector.GetSafeNormal();

	const FVector3f GroundNormal =
		SuspensionState.ImpactWorldNormal.GetSafeNormal();

	if (WheelRight.IsNearlyZero() || GroundNormal.IsNearlyZero())
	{
		OutSignedCamberDeg = 0.f;
		return 0.f;
	}

	// Raw camber sign follows the tire local axis convention:
//     +sin(camber) = wheel right vector points into the ground normal.
// This is intentionally not flipped by left/right side.
	float SinCamber = FVector3f::DotProduct(WheelRight, GroundNormal);
	SinCamber = FMath::Clamp(SinCamber, -1.f, 1.f);

	const float CamberRad = FMath::Asin(SinCamber);
	const float CamberDeg = FMath::RadiansToDegrees(CamberRad);

	// Human-readable signed camber: negative camber has the same sign on both sides.
	OutSignedCamberDeg = SuspensionState.bIsRightWheel ? -CamberDeg : CamberDeg;

	// Curve input:
	//     abs camber angle in degrees
	//
	// Curve output:
	//     q_gamma = dy / dx
	float Drift = TireLUTs.CamberToLateralDrift.FastEval(FMath::Abs(CamberDeg) / 90.f).Value;

	// Just in case the user draws a negative curve by accident.
	Drift = FMath::Max(0.f, Drift);

	// Convert the unsigned LUT value into the tire local lateral drift sign.
	float SignedDrift = CamberDeg > 0.f ? -Drift : Drift;

	// Geometric safety cap.
	//
	// Do not reconstruct contact patch length from ImpactWorldLocation here.
	// With LineTrace starting from the outer tire side, the reconstructed patch
	// length can become camber-sign dependent and clamp one side to zero.
	//
	// q_gamma = dy / dx is slope-like, so abs(tan(camber)) is the geometry-only
	// upper bound available without a real contact patch model.
	const float CosCamberAbs = FMath::Sqrt(FMath::Max(
		SMALL_NUMBER,
		1.f - SinCamber * SinCamber
	));

	const float DriftLimit =
		FMath::Abs(SinCamber) / CosCamberAbs;

	return FMath::Clamp(SignedDrift, -DriftLimit, DriftLimit);
}

FVector2f FVehicleWheelSolver::UpdateTransientSlip(
	FVehicleWheelSimState& LocalState,
	const FVehicleWheelSimContext& Context,
	const bool bOnGround, 
	const FVector2f& RelaxationLength)
{
	if (bOnGround)
	{
		const FVector2f SafeRelaxationLength = FVector2f::Max(RelaxationLength, FVector2f(SMALL_NUMBER));
		const FVector2f RelaxationLengthInv = FVector2f(1.f) / SafeRelaxationLength;

		const FVector2f SlipVelocity =
			FVector2f(LocalState.LongSlipVelocity, LocalState.LatSlipVelocity);

		// This is only for the relaxation denominator.
		// It is a numerical regularization, not a real transport speed.
		float AbsVx = FMath::Abs(LocalState.LocalLinearVelocity.X);
		float AbsOmegaR = FMath::Abs(LocalState.AngularVelocity * Context.R);
		FVector2f AbsVx2D = FVector2f(FMath::Max(AbsVx, AbsOmegaR), AbsVx);

		float MinVx = 0.1f;
		AbsVx2D = FVector2f::Max(AbsVx2D, FVector2f(MinVx));

		LocalState.TransientSlip =
			(LocalState.TransientSlip + (SlipVelocity * Context.SubstepDeltaTime) * RelaxationLengthInv) /
			(FVector2f(1.f, 1.f) + (AbsVx2D * Context.SubstepDeltaTime) * RelaxationLengthInv);

		float TransientSlipRatio = LocalState.TransientSlip.X;
		float TransientSlipAngle = FMath::Atan(LocalState.TransientSlip.Y) / (0.5f * PI);

		return FVector2f(TransientSlipRatio, TransientSlipAngle);
	}
	else
	{
		LocalState.TransientSlip = FVector2f(0.f, 0.f);
		return FVector2f(0.f, 0.f);
	}
}

float FVehicleWheelSolver::CalculateConstraintLongForce(
	FVehicleWheelSimState& LocalState,
	const FVehicleWheelSimContext& Context,
	const float EffectiveSprungMass)
{
	const float Vx = LocalState.LocalLinearVelocity.X;
	const float Omega = LocalState.AngularVelocity;
	const float R = Context.R;
	const float RInv = Context.RInv;

	const float DriveForce = LocalState.DriveTorque * RInv;
	const float ForceRequiredToBringToStop = -(Vx * Context.MacroDeltaTimeInv * EffectiveSprungMass + DriveForce);
	
	//get linear brake force
	const float TargetBrakeForce = LocalState.BrakeTorque * RInv;
	const float SignedBrakeForce = FMath::Clamp(ForceRequiredToBringToStop, -TargetBrakeForce, TargetBrakeForce);

	//torque from ground interaction is the torque required to make angularvelocity == linearvelocity / radius
	const float KinematicsSlipVelocity = Omega * R - Vx;
	const float ForceFromGroundInteraction = Context.SubstepDeltaTimeInv * LocalState.EffectiveInertia * KinematicsSlipVelocity;
	LocalState.TorqueFromGroundInteraction = ForceFromGroundInteraction * R;
	
	//get longitudinal force
	float ConstraintForce = DriveForce + SignedBrakeForce + ForceFromGroundInteraction;
	
	return ConstraintForce * KinematicsSlipVelocity > 0.f ? ConstraintForce : 0.f;
}

float FVehicleWheelSolver::CalculateConstraintLatForce(
	FVehicleWheelSimState& LocalState,
	const FVehicleWheelSimContext& Context, 
	const float EffectiveSprungMass)
{
	float ForceRequiredToBringToStop = 
		LocalState.LatSlipVelocity * Context.MacroDeltaTimeInv * EffectiveSprungMass;
	
	return ForceRequiredToBringToStop;
}

FVector2f FVehicleWheelSolver::CalculateGravityCompensationOnSlope(
	FVehicleWheelSimState& LocalState,
	FVehicleWheelSimContext& Context,
	const float PositiveForceIntoSurface,
	const bool bOnGround,
	const FVector3f& LongForceDir, 
	const FVector3f& LatForceDir)
{
	if (!bOnGround)
	{
		return FVector2f(0.f);
	}

	FVector2f GravityComp = FVector2f(0.f, 0.f);

	// sin(theta): LatForceDir.Z or LongForceDir.Z
	// ExtraGravityForce = Fz * TanTheta; 
	// TanTheta ≈ SinTheta, when theta is small; 
	// when theta is large, the vehicle maybe should not be able to hold itself on the slope; 
	// so there might be no need to calculate tan(theta)
	   
	//Lateral:
	float SlipSign = FMath::Sign(LocalState.LatSlipVelocity);
	GravityComp.Y = LatForceDir.Z * PositiveForceIntoSurface;

	// longitudinal:
	if (LocalState.bIsLocked)
	{
		SlipSign = FMath::Sign(LocalState.LongSlipVelocity);
		GravityComp.X = LongForceDir.Z * PositiveForceIntoSurface;

		//consider brake force?
		float BrakeForce = LocalState.BrakeTorque * Context.RInv;	//LocalState.BrakeTorque is always positive
		GravityComp.X = FMath::Clamp(GravityComp.X, -BrakeForce, BrakeForce);
	}

	// in the lateral direction, the wheel can be treated as it is always braking

	Context.GravityComp2D = GravityComp;
	return GravityComp;
}

float FVehicleWheelSolver::CalculateAvailableGrip(
	const float FrictionMultiplier,
	const float StaticSprungMass,
	const float WheelLoad,
	const float Saturation)
{
	const float DefaultGravity = 9.81f;
	float NormWheelLoad = StaticSprungMass * DefaultGravity;
	float LoadRatio = UVehicleUtilities::SafeDivide(WheelLoad, NormWheelLoad);
	float b = (1.f - Saturation) / (2.f + 2.f * Saturation);
	float LoadScale = LoadRatio / (1.f + b * LoadRatio);
	return FrictionMultiplier * LoadScale * NormWheelLoad;
}

FVector2f FVehicleWheelSolver::SolveTireForce(
	FVehicleWheelSimState& LocalState,
	const FVehicleWheelSimContext& Context,
	const float StaticSprungMass,
	const float EffectiveSprungMassLong,
	const float EffectiveSprungMassLat,
	const float PositiveForceIntoSurface,
	const bool bOnGround,
	const FVehicleTireConfig& TireConfig,
	const FVehicleWheelCachedLUTs& TireLUTs)
{
	UpdateSlipVelocity(LocalState, Context, bOnGround);

	// transient slip for tire force
	FVector2f TransientSlip = UpdateTransientSlip(LocalState, Context, bOnGround, TireConfig.RelaxationLength);
	
	// not for tire force now but for abs and tc logic
	UpdateSlipRatio(LocalState, Context, bOnGround);
	UpdateSlipAngle(LocalState, bOnGround);

	if (!bOnGround)
	{
		return FVector2f(0.f);
	}

	// Constraint tire force
	FVector2f ConstraintTireForce = FVector2f(
		CalculateConstraintLongForce(LocalState, Context, EffectiveSprungMassLong),
		CalculateConstraintLatForce(LocalState, Context, EffectiveSprungMassLat)
	);
	
	ConstraintTireForce += Context.GravityComp2D;

	// get stiffness(tangent) of linear region
	FVector2f LinearRegionStiffness = FVector2f(
		TireLUTs.Fx.LinearStiffness,
		TireLUTs.Fy.LinearStiffness
	);
	FVector2f LinearRegionStiffnessInv = FVector2f(
		UVehicleUtilities::SafeDivide(1.f, LinearRegionStiffness.X, 1.f),
		UVehicleUtilities::SafeDivide(1.f, LinearRegionStiffness.Y, 1.f)
	);

	// get absolut slip ratio and slip angle
	FVector2f AbsolutSlip = TransientSlip.GetAbs();

	// normalize slip ratio and slip angle
	FVector2f NormalizedSlip = AbsolutSlip * LinearRegionStiffness;
	float ScalarNormSlip = NormalizedSlip.Length();

	// get combined slip direction
	FVector2f WeightXY = FVector2f(1.f - TireConfig.CombinedSlipBias, TireConfig.CombinedSlipBias);
	WeightXY *= FVector2f(UVehicleUtilities::SafeDivide(1.f, TireConfig.MaxFx), UVehicleUtilities::SafeDivide(1.f, TireConfig.MaxFy));
	FVector2f ScDirection = (NormalizedSlip * WeightXY).GetSafeNormal();

	// the tangent of the linear region should not be changed, if the vehicle is driving on a surface with high friction multiplier
	float SlipInputScale = UVehicleUtilities::SafeDivide(TireConfig.FrictionMultiplier, LocalState.DynFrictionMultiplier);
	FVector2f SlipInput = SlipInputScale * ScalarNormSlip * LinearRegionStiffnessInv;
	
	// if the user has only setup one of the Fx or Fy curve, the Constraint force on the other direction should be cut, before computing combined slip
	bool bUseFxCurve = TireLUTs.Fx.bHasValidStiffness;
	bool bUseFyCurve = TireLUTs.Fy.bHasValidStiffness;

	// magic formula
	float MaxFx = TireConfig.MaxFx * Context.AvailableGrip * Context.LongForceScale;
	float MaxFy = TireConfig.MaxFy * Context.AvailableGrip * Context.LatForceScale;
	FVector2f MFTireForce = ConstraintTireForce;
	if (bUseFxCurve)
	{
		float FxPure = TireLUTs.Fx.FastEval(AbsolutSlip.X).Value;
		float FxCoupled = TireLUTs.Fx.FastEval(SlipInput.X).Value * ScDirection.X;
		float Fx = FMath::Abs(MaxFx * FMath::Lerp(FxPure, FxCoupled, TireConfig.LateralToLongitudinalInterference));
		MFTireForce.X = TransientSlip.X * ConstraintTireForce.X > 0.f ? 
			FMath::Clamp(ConstraintTireForce.X, -Fx, Fx) : 0.f;
	}
	if (bUseFyCurve)
	{
		float FyPure = TireLUTs.Fy.FastEval(AbsolutSlip.Y).Value;
		float FyCoupled = TireLUTs.Fy.FastEval(SlipInput.Y).Value * ScDirection.Y;
		float Fy = FMath::Abs(MaxFy * FMath::Lerp(FyPure, FyCoupled, TireConfig.LongitudinalToLateralInterference));
		MFTireForce.Y = TransientSlip.Y * ConstraintTireForce.Y > 0.f ? 
			FMath::Clamp(ConstraintTireForce.Y, -Fy, Fy) : 0.f;
	}
	if (!bUseFxCurve || !bUseFyCurve)
	{
		if (bUseFxCurve != bUseFyCurve) {
			if (!bUseFxCurve) MFTireForce.X = FMath::Clamp(ConstraintTireForce.X, -MaxFx, MaxFx);
			if (!bUseFyCurve) MFTireForce.Y = FMath::Clamp(ConstraintTireForce.Y, -MaxFy, MaxFy);
		}

		// cut with friction ellipse again to prevent overshoot
		FVector2f MaxForceInv = FVector2f(UVehicleUtilities::SafeDivide(1.f, MaxFx), UVehicleUtilities::SafeDivide(1.f, MaxFy));
		FVector2f NormalizedForce = MFTireForce * MaxForceInv;
		if (NormalizedForce.SquaredLength() > 1.f)
		{
			NormalizedForce = NormalizedForce.GetSafeNormal();
			MFTireForce.X = NormalizedForce.X * MaxFx;
			MFTireForce.Y = NormalizedForce.Y * MaxFy;
		}
	}

	return MFTireForce;
}