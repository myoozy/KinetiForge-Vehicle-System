// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)


#include "VehicleWheelSolver.h"
#include "VehicleWheelComponent.h"
#include "AsyncTickFunctions.h"
#include "VehicleUtil.h"

FVehicleWheelSolver::FVehicleWheelSolver()
{
}

FVehicleWheelSolver::~FVehicleWheelSolver()
{
}

bool FVehicleWheelSolver::Initialize(UVehicleWheelComponent* WheelComponent)
{
	if (WheelComponent != nullptr)
	{
		UpdateCachedRichCurves(WheelComponent->TireConfig);
		return true;
	}
	return false;
}

void FVehicleWheelSolver::UpdateWheel(
	float InPhysicsDeltaTime,
	float InDriveTorque,
	float InBrakeTorque,
	float InHandbrakeTorque,
	float InReflectedInertia,
	const FVehicleWheelConfig& WheelConfig,
	const FVehicleTireConfig& TireConfig,
	const FVehicleABSConfig& ABSConfig,
	const FVehicleSuspensionSimState& SuspensionState)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UpdateVehicleWheelSolver);

	const FVehicleWheelConfig& Config = WheelConfig;

	//divide something...	to avoid 0
	State.PhysicsDeltaTime = InPhysicsDeltaTime;
	State.PhysicsDeltaTimeInv = VehicleUtil::SafeDivide(1.f, State.PhysicsDeltaTime);
	State.R = Config.Radius * 0.01;
	State.RInv = VehicleUtil::SafeDivide(1.f, State.R);
	State.TotalInertia = Config.Inertia + InReflectedInertia;
	State.DriveTorque = InDriveTorque + State.P4MotorTorque;

	// get the direction of longitudinal and lateral force in world space
	// if the vectors are not normalized, the force can be affected by camber
	const FVector3f WheelRightVec = SuspensionState.WheelRightVector;
	const FVector3f ImpactNormal = SuspensionState.ImpactNormal;
	FVector3f LongForceDirUnNorm = FVector3f::CrossProduct(WheelRightVec, ImpactNormal);
	FVector3f LatForceDirUnNorm = FVector3f::VectorPlaneProject(WheelRightVec, ImpactNormal);
	FVector3f LongForceDir = LongForceDirUnNorm.GetSafeNormal();
	FVector3f LatForceDir = LatForceDirUnNorm.GetSafeNormal();

	UpdateLinearVelocity(LongForceDir, LatForceDir, SuspensionState.ImpactPointWorldVelocity);

	// get target brake torque
	// brake torque from esp can be negative (which means to reduce brake torque)
	// but the target total brake torque should not be negative (the brake should not accelerate the wheel)
	float TargetBrakeTorque = FMath::Max(0.f, FMath::Abs(InBrakeTorque) + State.BrakeTorqueFromESP);	
	
	// update abs
	PredictSlipAndUpdateABS(ABSConfig, TargetBrakeTorque, SuspensionState.bHitGround);

	// get total brake torque
	State.BrakeTorqueFromHandbrake = FMath::Abs(InHandbrakeTorque);
	State.BrakeTorque = State.BrakeTorqueFromBrake + Config.RollingRisistance + State.BrakeTorqueFromHandbrake;

	UpdateDynamicFrictionMultiplier(TireConfig, SuspensionState.ImpactFriction);

	const float SlipVelocityTolerance = 0.1f;
	WheelAcceleration(LongForceDir, SlipVelocityTolerance);

	float ForceIntoSurface = FMath::Max(0.f, SuspensionState.ForceAlongImpactNormal);

	UpdateGravityCompensationOnSlope(
		ForceIntoSurface,
		SuspensionState.bHitGround,
		LongForceDir,
		LatForceDir
	);

	UpdateTireForce(
		SuspensionState.SprungMass,
		ForceIntoSurface,
		SuspensionState.bHitGround,
		LongForceDirUnNorm,
		LatForceDirUnNorm,
		TireConfig
	);
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

	FVehicleTireConfig& TireConfig = WheelComponent->TireConfig;
	FVector WheelRightVec = FVector(SuspensionState.WheelRightVector);
	FVector ImpactNormal = FVector(SuspensionState.ImpactNormal);

	FVector TempForward = FVector::CrossProduct(WheelRightVec, ImpactNormal);
	FVector TempRight = FVector::VectorPlaneProject(WheelRightVec, ImpactNormal);
	FVector TempUp = ImpactNormal;
	FVector TempImpactPoint = SuspensionState.ImpactPoint;
	FRotator TempRot = FRotationMatrix::MakeFromYZ(TempRight, TempForward).Rotator();
	FVector TempScale = FVector(TireConfig.MaxFx, TireConfig.MaxFy, 1.f);
	FTransform TempTrans = FTransform(TempRot, TempImpactPoint, TempScale);

	Length *= 0.01;
	float ForceIntoSurface = FMath::Max(0.f, SuspensionState.ForceAlongImpactNormal);
	float AvailableGrip = Length * State.DynFrictionMultiplier * State.WheelLoad;

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

	FTransform WheelRelativeTrans = FTransform(WheelComponent->GetWheelRelativeTransform());
	FTransform WheelTrans = WheelRelativeTrans * WheelComponent->GetCarbodyAsyncWorldTransform();

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
		FVector TempDrawOffset = WheelComponent->WheelConfig.Radius * WheelTrans.GetRotation().GetUpVector();
		FString TextSlipRatio = FString(TEXT("SlipRatio = ")) + FString::SanitizeFloat(State.SlipRatio);
		FString TextSlipAngle = FString(TEXT("SlipAngle = ")) + FString::SanitizeFloat(State.SlipAngle);
		DrawDebugString(CurrentWorld, TempWheelLocation + TempDrawOffset, TextSlipRatio, 0, SlipColor, Duration, true, Length * 100);
		DrawDebugString(CurrentWorld, TempWheelLocation - TempDrawOffset, TextSlipAngle, 0, SlipColor, Duration, true, Length * 100);
	}

	if (bDrawInertia)
	{
		FColor InertiaColor = FColor(255, 255, 255);
		FVector TempWheelLocation = WheelTrans.GetLocation();
		FString TextInertia = FString(TEXT("Inertia = ")) + FString::SanitizeFloat(State.TotalInertia);
		DrawDebugString(CurrentWorld, TempWheelLocation, TextInertia, 0, InertiaColor, Duration, true, Length * 100);
	}
}

void FVehicleWheelSolver::SmoothenSlip(float InDeltaTime, float InInterpSpeed)
{
	SmoothenedSlipRatio = FMath::FInterpTo(
		SmoothenedSlipRatio,
		State.SlipRatio,
		InDeltaTime,
		InInterpSpeed
	);

	SmoothenedSlipAngle = FMath::FInterpTo(
		SmoothenedSlipAngle,
		State.SlipAngle,
		InDeltaTime,
		InInterpSpeed
	);
}

void FVehicleWheelSolver::UpdateCachedRichCurves(const FVehicleTireConfig& Config)
{
	if (IsValid(Config.Fx))
	{
		CachedCurves.Fx = Config.Fx->FloatCurve;
	}
	else
	{
		CachedCurves.Fx.Reset();
	}
	if (IsValid(Config.Fy))
	{
		CachedCurves.Fy = Config.Fy->FloatCurve;
	}
	else
	{
		CachedCurves.Fy.Reset();
	}
}

float FVehicleWheelSolver::GetTangentAtOrigin(const FRichCurve& Curve)
{
	if (Curve.Keys.Num() == 0) return 10.f;

	const auto& Key0 = Curve.Keys[0];

	return FMath::IsNearlyZero(Key0.Time) ? Key0.LeaveTangent : 0.f;
}

void FVehicleWheelSolver::PredictSlipAndUpdateABS(
	const FVehicleABSConfig& ABSConfig,
	const float TargetBrakeTorque, 
	const bool bHitGround)
{
	// predict angular velocity
	float PredictedOmega = State.AngularVelocity + State.AngularAcceleration * State.PhysicsDeltaTime;
	float PredictedVSlip = PredictedOmega * State.R - State.LocalLinearVelocity.X;
	PredictedVSlip *= bHitGround;
	float Denominator = FMath::Max(FMath::Max(FMath::Abs(State.LocalLinearVelocity.X), FMath::Abs(PredictedOmega * State.R)), 1.f);
	State.PredictedSlipRatio = PredictedVSlip / Denominator;

	float AbsolutSlip = FMath::Abs(State.PredictedSlipRatio);
	// only activate abs when the sign of slip and the sign of velocity is different
	bool bDifferentSign = State.PredictedSlipRatio * State.LocalLinearVelocity.X < 0.f;

	State.bABSTriggered = 
		ABSConfig.bAntiBrakeSystemEnabled
		&& TargetBrakeTorque > SMALL_NUMBER
		&& bHitGround
		&& FMath::Abs(State.LocalLinearVelocity.X) > ABSConfig.ActivationSpeed
		&& AbsolutSlip > ABSConfig.OptimalSlip
		&& bDifferentSign;

	if (State.bABSTriggered)
	{
		float Error = AbsolutSlip - ABSConfig.OptimalSlip;
		float AbsFactor = 1.0f - (Error * ABSConfig.Sensitivity);
		AbsFactor = FMath::Clamp(AbsFactor, 0.0f, 1.0f);
		State.BrakeTorqueFromBrake = TargetBrakeTorque * AbsFactor;
		
		return;
	}

	State.BrakeTorqueFromBrake = TargetBrakeTorque;
}

void FVehicleWheelSolver::UpdateDynamicFrictionMultiplier(
	const FVehicleTireConfig& TireConfig,
	const float ImpactFriction)
{
	switch (TireConfig.TireFrictionCombineMode)
	{
	case ETireFrictionCombineMode::Constant:
		State.DynFrictionMultiplier = TireConfig.FrictionMultiplier;
		break;
	case ETireFrictionCombineMode::Average:
		State.DynFrictionMultiplier = 0.5 * (TireConfig.FrictionMultiplier + ImpactFriction);
		break;
	case ETireFrictionCombineMode::Multiply:
		State.DynFrictionMultiplier = TireConfig.FrictionMultiplier * ImpactFriction;
		break;
	case ETireFrictionCombineMode::Min:
		State.DynFrictionMultiplier = FMath::Min(TireConfig.FrictionMultiplier, ImpactFriction);
		break;
	case ETireFrictionCombineMode::Max:
		State.DynFrictionMultiplier = FMath::Max(TireConfig.FrictionMultiplier, ImpactFriction);
		break;
	default:
		State.DynFrictionMultiplier = 0.5 * (TireConfig.FrictionMultiplier + ImpactFriction);
		break;
	}
}

void FVehicleWheelSolver::UpdateLinearVelocity(
	const FVector3f& LongForceDir, 
	const FVector3f& LatForceDir, 
	const FVector3f& ImpactPointWorldVelocity)
{
	State.LocalLinearVelocity.X = FVector3f::DotProduct(LongForceDir, ImpactPointWorldVelocity);
	State.LocalLinearVelocity.Y = FVector3f::DotProduct(LatForceDir, ImpactPointWorldVelocity);
}

void FVehicleWheelSolver::WheelAcceleration(
	const FVector3f& LongForceDir,
	float SlipVelocityTolerance)
{
	float LastAngularVelocity = State.AngularVelocity;

	//friction torque should not flip the sign of the relative rotation to the ground
	//but there should be tolerance, because even when there is no drive torque or brake torque, the wheel should not always be completely sticked to the road
	//allow small angular acceleration tolerance to prevent sticky behavior when slip ~ 0 (empirical value)
	float ToleranceTorque = State.TotalInertia * SlipVelocityTolerance * State.PhysicsDeltaTimeInv;

	//Get the torque required to flip the sign of relative rotation between ground and wheel
	float AngularLongSlip = State.AngularVelocity - State.LocalLinearVelocity.X * State.RInv;
	float MaxFrictionTorque = AngularLongSlip * State.TotalInertia * State.PhysicsDeltaTimeInv;

	//drive torque must be considered, but till now we cannot define the direction of the brake torque, so just donot take brake torque into account
	MaxFrictionTorque += State.DriveTorque;
	MaxFrictionTorque = FMath::Abs(MaxFrictionTorque);

	//if there is no tolerance, there will be a bug when braking: the brake force will not be released since the longsilpvelocity == 0 and the smoothing factor == 0, so the longitudinal force will not reduce even when brake is released
	MaxFrictionTorque += ToleranceTorque;

	//clamp the friction torque, friction torque should not flip the sign of the relative rotation to the ground
	float FrictionTorque = FVector3f::DotProduct(State.TireForce, LongForceDir) * State.R;
	float ClampedFrictionTorque = FMath::Clamp(FrictionTorque, -MaxFrictionTorque, MaxFrictionTorque);

	//since the brake torque is not considered when calculating max friction torque, we have to get the excess friction torque to deal with brake torque
	//if there is no excess friction torque, slip ratio will be very high when braking, since brake torque is not affected by friction
	//and again, the excess friction torque should not be greater than brake torque, since the friction torque should not flip the sign of the relative rotation to the ground
	float ExcessFrictionTorque = FMath::Clamp(FrictionTorque - ClampedFrictionTorque, -State.BrakeTorque, State.BrakeTorque);

	// avoid divided by 0
	float TotalInertiaInv = VehicleUtil::SafeDivide(1.f, State.TotalInertia);

	//get the angular velocity without braking
	State.AngularVelocity += State.PhysicsDeltaTime * TotalInertiaInv * (State.DriveTorque - ClampedFrictionTorque);
	float AngVelSignIfNotBraking = FMath::Sign(State.AngularVelocity);

	//finally the sign of brake torque can be defined
	float ActuralBrakingTorque = State.BrakeTorque * (-AngVelSignIfNotBraking);
	State.AngularVelocity += State.PhysicsDeltaTime * TotalInertiaInv * (ActuralBrakingTorque - ExcessFrictionTorque);

	//zero cross check
	//if the wheel is locked, the angular velocity should be 0
	State.bIsLocked = AngVelSignIfNotBraking * State.AngularVelocity <= 0.f && State.BrakeTorque > SMALL_NUMBER;
	State.AngularVelocity *= !State.bIsLocked;

	// get angular acceleration
	State.AngularAcceleration = (State.AngularVelocity - LastAngularVelocity) * State.PhysicsDeltaTimeInv;
}

void FVehicleWheelSolver::UpdateSlipAngle(bool bHitGround)
{
	//get velocity2d
	FVector2f Velocity2DNormalized = State.LocalLinearVelocity.GetSafeNormal();

	//calculate lateral slip
	//get slip angle
	float SlipAngleRaw = FMath::Asin(-Velocity2DNormalized.Y);
	SlipAngleRaw = FMath::RadiansToDegrees(SlipAngleRaw);

	//approximate low speed slip angle
	//-V.y = |V| * sin(SlipAngle) ¡Ö |V| * SlipAngle ¡Ö SlipAngle; when slip angle is small and |v| --> 1.f
	float LowSpeedSlipAngle = -FMath::RadiansToDegrees(State.LocalLinearVelocity.Y);

	//get alpha for lerp, lerp between high speed and low speede
	float Alpha = FMath::GetMappedRangeValueClamped(FVector2f(0.01f, 0.1f), FVector2f(0.f, 1.f), State.LocalLinearVelocity.SquaredLength());

	//combine low speed and high speed
	State.SlipAngle = FMath::Lerp(LowSpeedSlipAngle, SlipAngleRaw, Alpha) * bHitGround;
	State.SlipAngle = FMath::Clamp(State.SlipAngle, -90.f, 90.f);
}

void FVehicleWheelSolver::UpdateSlipRatio(bool bHitGround)
{
	//Calculate longitudinal slip
	State.LongSlipVelocity = State.AngularVelocity * State.R - State.LocalLinearVelocity.X;
	State.LongSlipVelocity *= bHitGround;

	//calculate slip ratio
	float Denominator = FMath::Max(FMath::Max(FMath::Abs(State.LocalLinearVelocity.X), FMath::Abs(State.AngularVelocity * State.R)), 1.f);
	State.SlipRatio = State.LongSlipVelocity / Denominator;
}

float FVehicleWheelSolver::CalculateConstraintLongForce(float SprungMass)
{
	float ForceRequiredToBringToStop = FMath::Abs(State.LocalLinearVelocity.X * State.PhysicsDeltaTimeInv * SprungMass);
	ForceRequiredToBringToStop += FMath::Abs(State.DriveTorque * State.RInv);

	//get linear brake force
	float SignedBrakeTorque = State.BrakeTorque * (-FMath::Sign(State.LocalLinearVelocity.X));
	SignedBrakeTorque = FMath::Clamp(SignedBrakeTorque, -ForceRequiredToBringToStop, ForceRequiredToBringToStop);

	//torque from ground interaction is the torque required to make angularvelocity == linearvelocity / radius
	float GroundAngularVelocity = State.LocalLinearVelocity.X * State.RInv;
	State.TorqueFromGroundInteraction = State.PhysicsDeltaTimeInv * State.TotalInertia * (State.AngularVelocity - GroundAngularVelocity);

	//get longitudinal force
	return (State.DriveTorque + SignedBrakeTorque + State.TorqueFromGroundInteraction) * State.RInv;
}

float FVehicleWheelSolver::CalculateConstraintLatForce(float SprungMass)
{
	float ForceRequiredToBringToStop = -State.LocalLinearVelocity.Y * State.PhysicsDeltaTimeInv * SprungMass;
	
	return ForceRequiredToBringToStop;
}

void FVehicleWheelSolver::UpdateGravityCompensationOnSlope(
	float PositiveForceIntoSurface,
	bool bHitGround,
	const FVector3f& LongForceDir, 
	const FVector3f& LatForceDir)
{
	if (!bHitGround)
	{
		State.GravityCompensationForce = FVector2f(0.f);
		return;
	}

	FVector2f GravityComp = FVector2f(0.f, 0.f);

	// sin(theta): LatForceDir.Z or LongForceDir.Z
	// ExtraGravityForce = Fz * TanTheta; 
	// TanTheta ¡Ö SinTheta, when theta is small; 
	// when theta is large, the vehicle maybe should not be able to hold itself on the slope; 
	// so there might be no need to calculate tan(theta)
	   
	// the vehicle is not able to stop on the slope
	// even if the slope is not steep
	// so we add a little extra friction force
	float Mu = 0.1f * State.DynFrictionMultiplier;

	//Lateral:
	float SlipSign = FMath::Sign(-State.LocalLinearVelocity.Y);
	GravityComp.Y = (LatForceDir.Z + FMath::Abs(LatForceDir.Z) * SlipSign * Mu) * PositiveForceIntoSurface;

	// longitudinal:
	if (State.bIsLocked)
	{
		SlipSign = FMath::Sign(State.LongSlipVelocity);
		GravityComp.X = (LongForceDir.Z + FMath::Abs(LongForceDir.Z) * SlipSign * Mu) * PositiveForceIntoSurface;

		//consider brake force?
		float BrakeForce = State.BrakeTorque * State.RInv;	//State.BrakeTorque is always positive
		GravityComp.X = FMath::Clamp(GravityComp.X, -BrakeForce, BrakeForce);
	}

	// in the lateral direction, the wheel can be treated as it is always braking

	// then do some smoothing
	const float Scale = 100.f; // magic number
	FVector2f Smoothing = FVector2f(FMath::Abs(State.LongSlipVelocity), FMath::Abs(State.LocalLinearVelocity.Y));
	Smoothing *= Scale * FMath::Min(State.PhysicsDeltaTime, 1.f / 60.f);

	// clamp between 0 to 1
	Smoothing = Smoothing.ClampAxes(0.f, 1.f);

	State.GravityCompensationForce += (GravityComp - State.GravityCompensationForce) * Smoothing;
}

float FVehicleWheelSolver::CalculateScaledWheelLoad(float SprungMass, float WheelLoad, float Saturation)
{
	float NormWheelLoad = SprungMass * 9.8;
	float LoadRatio = VehicleUtil::SafeDivide(WheelLoad, NormWheelLoad);
	float b = (1.f - Saturation) / (2.f + 2.f * Saturation);
	float LoadScale = LoadRatio / (1.f + b * LoadRatio);
	return LoadScale * NormWheelLoad;
}

void FVehicleWheelSolver::UpdateTireForce(
	float SprungMass, 
	float PositiveForceIntoSurface,
	bool bHitGround,
	const FVector3f& LongForceDirUnNorm,
	const FVector3f& LatForceDirUnNorm,
	const FVehicleTireConfig& TireConfig)
{
	UpdateSlipRatio(bHitGround);
	UpdateSlipAngle(bHitGround);

	if (!bHitGround)
	{
		State.MFTireForce2D = FVector2f(0.f);
		State.TireForce2D = FVector2f(0.f);
		State.TireForce = FVector3f(0.f);
		return;
	}

	// Constraint tire force
	FVector2f ConstraintTireForce = FVector2f(
		CalculateConstraintLongForce(SprungMass),
		CalculateConstraintLatForce(SprungMass)
	);

	// get wheel load
	State.WheelLoad = CalculateScaledWheelLoad(SprungMass, PositiveForceIntoSurface, TireConfig.WheelLoadInfluenceFactor);
	float AvailableGrip = State.DynFrictionMultiplier * State.WheelLoad;

	// get stiffness(tangent) of linear region
	FVector2f LinearRegionStiffness = FVector2f(GetTangentAtOrigin(CachedCurves.Fx), GetTangentAtOrigin(CachedCurves.Fy));

	// get absolut slip ratio and slip angle
	FVector2f AbsolutSlip = FVector2f(FMath::Abs(State.SlipRatio), FMath::Abs(State.SlipAngle));

	// normalize slip ratio and slip angle
	FVector2f NormalizedSlip = AbsolutSlip * LinearRegionStiffness;
	float ScalarNormSlip = NormalizedSlip.Length();

	// get optimal slip
	FVector2f OptimalSlip = LinearRegionStiffness.SquaredLength() > SMALL_NUMBER ? FVector2f(1.f) / LinearRegionStiffness : FVector2f(1.f);
	
	// get combined slip direction
	FVector2f WeightXY = FVector2f(1.f - TireConfig.CombinedSlipBias, TireConfig.CombinedSlipBias);
	WeightXY *= FVector2f(VehicleUtil::SafeDivide(1.f, TireConfig.MaxFx), VehicleUtil::SafeDivide(1.f, TireConfig.MaxFy));
	FVector2f ScDirection = (NormalizedSlip * WeightXY).GetSafeNormal();

	// the tangent of the linear region should not be changed, if the vehicle is driving on a surface with high friction multiplier
	float SlipInputScale = VehicleUtil::SafeDivide(TireConfig.FrictionMultiplier, State.DynFrictionMultiplier);
	FVector2f SlipInput = SlipInputScale * ScalarNormSlip * OptimalSlip;
	
	// if the user has only setup one of the Fx or Fy curve, the Constraint force on the other direction should be cut, before computing combined slip
	bool bUseFxCurve = TireConfig.Fx != nullptr;
	bool bUseFyCurve = TireConfig.Fy != nullptr;

	// magic formula
	float MaxFx = TireConfig.MaxFx * AvailableGrip;
	float MaxFy = TireConfig.MaxFy * AvailableGrip;
	FVector2f MFTireForce = ConstraintTireForce;
	if (bUseFxCurve)
	{
		float FxPure = CachedCurves.Fx.Eval(AbsolutSlip.X);
		float FxCoupled = CachedCurves.Fx.Eval(SlipInput.X) * ScDirection.X;
		float Fx = FMath::Abs(MaxFx * FMath::Lerp(FxPure, FxCoupled, TireConfig.LateralToLongitudinalInterference));
		MFTireForce.X = FMath::Clamp(ConstraintTireForce.X, -Fx, Fx);
	}
	if (bUseFyCurve)
	{
		float FyPure = CachedCurves.Fy.Eval(AbsolutSlip.Y);
		float FyCoupled = CachedCurves.Fy.Eval(SlipInput.Y) * ScDirection.Y;
		float Fy = FMath::Abs(MaxFy * FMath::Lerp(FyPure, FyCoupled, TireConfig.LongitudinalToLateralInterference));
		MFTireForce.Y = FMath::Clamp(ConstraintTireForce.Y, -Fy, Fy);
	}
	if (bUseFxCurve != bUseFyCurve) {
		if (!bUseFxCurve) MFTireForce.X = FMath::Clamp(ConstraintTireForce.X, -MaxFx, MaxFx);
		if (!bUseFyCurve) MFTireForce.Y = FMath::Clamp(ConstraintTireForce.Y, -MaxFy, MaxFy);
	}

	// get the velocity of the spinning wheel
	FVector2f V = FVector2f(0.f);
	V.X = FMath::Max(FMath::Abs(State.LocalLinearVelocity.X), FMath::Abs(State.AngularVelocity * State.R));
	V.Y = FMath::Abs(State.LocalLinearVelocity.Y);
	float ScalarV = V.Length();
	
	// lerp between constraint force (more stable at low speed) and MF force (more accurate at high speed)
	// it is not necessary to use constraint force at low speed, since the MF force is also stable at low speed
	// but to burn out in place, constraint force is necessary
	float Alpha = FMath::Clamp(ScalarV, 0.f, 1.f);	// Alpha = 1 when V = 1 m/s
	FVector2f TargetTireForce = FMath::Lerp(ConstraintTireForce, MFTireForce, Alpha);

	// relaxation length
	const FVector2f RelaxLength = TireConfig.RelaxationLength;
	bool bIsRelaxLengthValid = RelaxLength.SquaredLength() > SMALL_NUMBER;

	// rolling distance
	float Distance = State.PhysicsDeltaTime * ScalarV;
	FVector2f Dist2D = FVector2f(Distance);

	// smoothing factor for relaxation length (implicit)
	FVector2f ImplicitSmoothing = Dist2D / (RelaxLength + Dist2D);
	// it will never reach 1.f but I still want to clamp lol
	ImplicitSmoothing = ImplicitSmoothing.ClampAxes(0.f, 1.f);

	// smoothing factor under stiction condition (low speed)
	FVector2f StictionSmoothing = FVector2f(FMath::Abs(State.LongSlipVelocity), FMath::Abs(State.LocalLinearVelocity.Y));
	const float Scale = 250.f;	// magic number
	StictionSmoothing *= Scale * FMath::Min(State.PhysicsDeltaTime, 1.f / 60.f);
	StictionSmoothing = StictionSmoothing.ClampAxes(0.f, 1.f);

	// lerp and smoothen
	FVector2f SmoothingFactor = FMath::Lerp(StictionSmoothing, ImplicitSmoothing, Alpha * bIsRelaxLengthValid);
	SmoothingFactor = SmoothingFactor.ClampAxes(0.f, 1.f);

	State.MFTireForce2D += (TargetTireForce - State.MFTireForce2D) * SmoothingFactor;

	// final tire force, magic formula + gravity
	State.TireForce2D = State.MFTireForce2D + State.GravityCompensationForce;

	// cut with friction ellipse again to prevent overshoot
	FVector2f MaxForceInv = FVector2f(VehicleUtil::SafeDivide(1.f, MaxFx), VehicleUtil::SafeDivide(1.f, MaxFy));
	FVector2f NormalizedForce = State.TireForce2D * MaxForceInv;
	if (NormalizedForce.SquaredLength() > 1.f)
	{
		NormalizedForce = NormalizedForce.GetSafeNormal();
		State.TireForce2D.X = NormalizedForce.X * MaxFx;
		State.TireForce2D.Y = NormalizedForce.Y * MaxFy;
	}

	State.TireForce = State.TireForce2D.X * LongForceDirUnNorm + State.TireForce2D.Y * LatForceDirUnNorm;
}