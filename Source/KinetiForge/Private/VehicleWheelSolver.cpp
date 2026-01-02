// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)


#include "VehicleWheelSolver.h"
#include "VehicleWheelComponent.h"
#include "AsyncTickFunctions.h"

FVehicleWheelSolver::FVehicleWheelSolver()
{
}

FVehicleWheelSolver::~FVehicleWheelSolver()
{
}

bool FVehicleWheelSolver::Initialize(UVehicleWheelComponent* InTargetWheelComponent)
{
	TargetWheelComponent = InTargetWheelComponent;
	return IsValid(TargetWheelComponent);
}

void FVehicleWheelSolver::UpdateWheel(
	float InPhysicsDeltaTime,
	float InDriveTorque,
	float InBrakeTorque,
	float InHandbrakeTorque,
	float InReflectedInertia,
	const FVehicleSuspensionSimData& SuspensionSimData)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UpdateVehicleWheelSolver);

	if (!IsValid(TargetWheelComponent)) return;

	FVehicleWheelConfig& Config = TargetWheelComponent->WheelConfig;

	//divide something...	to avoid 0
	SimData.PhysicsDeltaTime = InPhysicsDeltaTime;
	SimData.PhysicsDeltaTimeInv = SafeDivide(1, SimData.PhysicsDeltaTime);
	SimData.DriveTorque = InDriveTorque + SimData.P4MotorTorque;
	SimData.R = Config.Radius * 0.01;
	SimData.RInv = SafeDivide(1, SimData.R);
	SimData.ReflectedInertia = InReflectedInertia;
	SimData.TotalInertia = Config.Inertia + SimData.ReflectedInertia;
	SimData.TotalInertiaInv = SafeDivide(1, SimData.TotalInertia);
	
	// get target brake torque
	// brake torque from esp can be negative (which means to reduce brake torque)
	// but the target total brake torque should not be negative (the brake should not accelerate the wheel)
	float TargetBrakeTorque = FMath::Max(0.f, FMath::Abs(InBrakeTorque) + SimData.BrakeTorqueFromESP);	
	
	// update abs
	UpdateABS(TargetBrakeTorque, SuspensionSimData.bHitGround);

	// get total brake torque
	SimData.BrakeTorqueFromHandbrake = FMath::Abs(InHandbrakeTorque);
	SimData.BrakeTorque = SimData.BrakeTorqueFromBrake + Config.RollingRisistance + SimData.BrakeTorqueFromHandbrake;

	UpdateDynamicFrictionMultiplier(SuspensionSimData.HitStruct);

	//get the direction of longitudinal and lateral force in world space
	//if the vectors are not normalized, the force can be affected by camber
	FVector LongForceDirUnNorm = FVector::CrossProduct(SuspensionSimData.WheelRightVector, SuspensionSimData.HitStruct.ImpactNormal);
	FVector LatForceDirUnNorm = FVector::VectorPlaneProject(SuspensionSimData.WheelRightVector, SuspensionSimData.HitStruct.ImpactNormal);

	FVector LongForceDir = LongForceDirUnNorm.GetSafeNormal();
	FVector LatForceDir = LatForceDirUnNorm.GetSafeNormal();

	UpdateLinearVelocity(LongForceDir, LatForceDir, SuspensionSimData.ImpactPointWorldVelocity);

	WheelAcceleration(LongForceDir);

	float ForceIntoSurface = FMath::Max(0.f, SuspensionSimData.ForceAlongImpactNormal);

	UpdateGravityCompensationOnSlope(
		ForceIntoSurface,
		SuspensionSimData.bHitGround,
		LongForceDir,
		LatForceDir
	);

	UpdateTireForce(
		SuspensionSimData.SprungMass,
		ForceIntoSurface,
		SuspensionSimData.bHitGround,
		LongForceDirUnNorm,
		LatForceDirUnNorm
	);
}

void FVehicleWheelSolver::DrawWheelForce(
	UWorld* InCurrentWorld, 
	const FVehicleSuspensionSimData& InSuspensionSimData, 
	float Duration, 
	float Thickness, 
	float Length, 
	bool bDrawVelocity, 
	bool bDrawSlip, 
	bool bDrawInertia)
{
	if (!InCurrentWorld || !IsValid(TargetWheelComponent))return;

	FVehicleTireConfig& TireConfig = TargetWheelComponent->TireConfig;

	FVector TempForward = FVector::CrossProduct(InSuspensionSimData.WheelRightVector, InSuspensionSimData.HitStruct.Normal);
	FVector TempRight = FVector::VectorPlaneProject(InSuspensionSimData.WheelRightVector, InSuspensionSimData.HitStruct.Normal);
	FVector TempUp = InSuspensionSimData.HitStruct.Normal;
	FVector TempImpactPoint = InSuspensionSimData.HitStruct.ImpactPoint;
	FRotator TempRot = FRotationMatrix::MakeFromYZ(TempRight, TempForward).Rotator();
	FVector TempScale = FVector(TireConfig.MaxFx, TireConfig.MaxFy, 1.f);
	FTransform TempTrans = FTransform(TempRot, TempImpactPoint, TempScale);

	Length *= 0.01;
	float ForceIntoSurface = FMath::Max(0.f, InSuspensionSimData.ForceAlongImpactNormal);
	float ScaledWheelLoad = CalculateScaledWheelLoad(
		InSuspensionSimData.SprungMass, 
		ForceIntoSurface,
		TireConfig.WheelLoadInfluenceFactor);
	float AvailableGrip = Length * SimData.DynFrictionMultiplier * ScaledWheelLoad;

	//draw grip circle
	FColor GripCircleColor = FColor(0, 191, 255);
	DrawDebugCircle(InCurrentWorld, TempTrans.ToMatrixWithScale(), AvailableGrip, 16, GripCircleColor,
		false, Duration, 0, Thickness, false);

	//draw raw force
	FVector Long = TempForward * SimData.TireForce2D.X * Length;
	FVector Lat = TempRight * SimData.TireForce2D.Y * Length;
	FColor RawForceColor = FColor(0, 255, 191);
	DrawDebugLine(InCurrentWorld, TempImpactPoint, TempImpactPoint + Long, RawForceColor, false, Duration, 0, Thickness);
	DrawDebugLine(InCurrentWorld, TempImpactPoint, TempImpactPoint + Lat, RawForceColor, false, Duration, 0, Thickness);

	//draw final force
	DrawDebugLine(InCurrentWorld, TempImpactPoint, SimData.TireForce * Length + TempImpactPoint, GripCircleColor, false, Duration, 0, Thickness);

	FTransform WheelTrans = InSuspensionSimData.WheelRelativeTransform * InSuspensionSimData.CarbodyWorldTransform;

	if (bDrawVelocity)
	{
		FColor VelociyColor = FColor(127, 255, 191);
		FColor StringColor = FColor(255 - VelociyColor.R, 255 - VelociyColor.G, 255 - VelociyColor.B);
		FVector TempOffset = InSuspensionSimData.WheelRightVector * InSuspensionSimData.WheelPos * FMath::Abs(SimData.AngularVelocity) * Length * 100;
		FVector TempWheelLocation = WheelTrans.GetLocation();
		DrawDebugLine(InCurrentWorld, TempWheelLocation, TempWheelLocation + TempOffset, VelociyColor, false, Duration, 0, Thickness);

		FString TempString = FString::SanitizeFloat(SimData.AngularVelocity);
		TempString = FString(TEXT("Omega = ")) + TempString;
		DrawDebugString(InCurrentWorld, TempWheelLocation + TempOffset, TempString, 0, VelociyColor, Duration, true);
	}

	if (bDrawSlip)
	{
		FColor SlipColor = FColor(127, 63, 255);
		FVector TempWheelLocation = WheelTrans.GetLocation();
		FVector TempDrawOffset = TargetWheelComponent->WheelConfig.Radius * WheelTrans.GetRotation().GetUpVector();
		FString TextSlipRatio = FString(TEXT("SlipRatio = ")) + FString::SanitizeFloat(SimData.SlipRatio);
		FString TextSlipAngle = FString(TEXT("SlipAngle = ")) + FString::SanitizeFloat(SimData.SlipAngle);
		DrawDebugString(InCurrentWorld, TempWheelLocation + TempDrawOffset, TextSlipRatio, 0, SlipColor, Duration, true, Length * 100);
		DrawDebugString(InCurrentWorld, TempWheelLocation - TempDrawOffset, TextSlipAngle, 0, SlipColor, Duration, true, Length * 100);
	}

	if (bDrawInertia)
	{
		FColor InertiaColor = FColor(255, 255, 255);
		FVector TempWheelLocation = WheelTrans.GetLocation();
		FString TextInertia = FString(TEXT("Inertia = ")) + FString::SanitizeFloat(SimData.TotalInertia);
		DrawDebugString(InCurrentWorld, TempWheelLocation, TextInertia, 0, InertiaColor, Duration, true, Length * 100);
	}
}

void FVehicleWheelSolver::SmoothenSlip(float InDeltaTime, float InInterpSpeed
)
{
	SmoothenedSlipRatio = FMath::FInterpTo(
		SmoothenedSlipRatio,
		SimData.SlipRatio,
		InDeltaTime,
		InInterpSpeed
	);

	SmoothenedSlipAngle = FMath::FInterpTo(
		SmoothenedSlipAngle,
		SimData.SlipAngle,
		InDeltaTime,
		InInterpSpeed
	);
}

void FVehicleWheelSolver::UpdateABS(float TargetBrakeTorque, bool bHitGround)
{
	FVehicleABSConfig& ABSConfig = TargetWheelComponent->ABSConfig;

	float AbsolutSlip = FMath::Abs(SimData.SlipRatio);

	SimData.bABSTriggered = 
		ABSConfig.bAntiBrakeSystemEnabled
		&& TargetBrakeTorque > SMALL_NUMBER
		&& bHitGround
		&& FMath::Abs(SimData.LocalLinearVelocity.X) > ABSConfig.ActivationSpeed
		&& AbsolutSlip > ABSConfig.OptimalSlip;

	if (SimData.bABSTriggered)
	{
		float Error = AbsolutSlip - ABSConfig.OptimalSlip;
		float AbsFactor = 1.0f - (Error * ABSConfig.Sensitivity);
		AbsFactor = FMath::Clamp(AbsFactor, 0.0f, 1.0f);
		SimData.BrakeTorqueFromBrake = TargetBrakeTorque * AbsFactor;
		
		return;
	}

	SimData.BrakeTorqueFromBrake = TargetBrakeTorque;
}

void FVehicleWheelSolver::UpdateDynamicFrictionMultiplier(const FHitResult& HitStruct)
{
	FVehicleTireConfig& TireConfig = TargetWheelComponent->TireConfig;

	//if physics mat is valid
	if (UPhysicalMaterial* HitPhysMat = HitStruct.PhysMaterial.Get())
	{
		float MuContact = HitPhysMat->Friction;

		switch (TireConfig.TireFrictionCombineMode)
		{
		case ETireFrictionCombineMode::Constant:
			SimData.DynFrictionMultiplier = TireConfig.FrictionMultiplier;
			break;
		case ETireFrictionCombineMode::Average:
			SimData.DynFrictionMultiplier = 0.5 * (TireConfig.FrictionMultiplier + MuContact);
			break;
		case ETireFrictionCombineMode::Multiply:
			SimData.DynFrictionMultiplier = TireConfig.FrictionMultiplier * MuContact;
			break;
		case ETireFrictionCombineMode::Min:
			SimData.DynFrictionMultiplier = FMath::Min(TireConfig.FrictionMultiplier, MuContact);
			break;
		case ETireFrictionCombineMode::Max:
			SimData.DynFrictionMultiplier = FMath::Max(TireConfig.FrictionMultiplier, MuContact);
			break;
		default:
			SimData.DynFrictionMultiplier = 0.5 * (TireConfig.FrictionMultiplier + MuContact);
			break;
		}
	}
	else
	{
		SimData.DynFrictionMultiplier = TireConfig.FrictionMultiplier;
	}
}

void FVehicleWheelSolver::UpdateLinearVelocity(
	const FVector& LongForceDir, 
	const FVector& LatForceDir, 
	const FVector& ImpactPointWorldVelocity)
{
	SimData.LocalLinearVelocity.X = FVector::DotProduct(LongForceDir, ImpactPointWorldVelocity);
	SimData.LocalLinearVelocity.Y = FVector::DotProduct(LatForceDir, ImpactPointWorldVelocity);
}

void FVehicleWheelSolver::WheelAcceleration(
	const FVector& LongForceDir,
	float AccelerationTolerance)
{
	//friction torque should not flip the sign of the relative rotation to the ground
	//but there should be tolerance, because even when there is no drive torque or brake torque, the wheel should not always be completely sticked to the road
	//allow small angular acceleration tolerance to prevent sticky behavior when slip ~ 0 (empirical value)
	float ToleranceTorque = SimData.TotalInertia * AccelerationTolerance * SimData.PhysicsDeltaTimeInv;

	//Get the torque required to flip the sign of relative rotation between ground and wheel
	float AngularLongSlip = SimData.AngularVelocity - SimData.LocalLinearVelocity.X * SimData.RInv;
	float MaxFrictionTorque = AngularLongSlip * SimData.TotalInertia * SimData.PhysicsDeltaTimeInv;

	//drive torque must be considered, but till now we cannot define the direction of the brake torque, so just donot take brake torque into account
	MaxFrictionTorque += SimData.DriveTorque;
	MaxFrictionTorque = FMath::Abs(MaxFrictionTorque);

	//if there is no tolerance, there will be a bug when braking: the brake force will not be released since the longsilpvelocity == 0 and the smoothing factor == 0, so the longitudinal force will not reduce even when brake is released
	MaxFrictionTorque += ToleranceTorque;

	//clamp the friction torque, friction torque should not flip the sign of the relative rotation to the ground
	float FrictionTorque = FVector::DotProduct(SimData.TireForce, LongForceDir) * SimData.R;
	float ClampedFrictionTorque = FMath::Clamp(FrictionTorque, -MaxFrictionTorque, MaxFrictionTorque);

	//since the brake torque is not considered when calculating max friction torque, we have to get the excess friction torque to deal with brake torque
	//if there is no excess friction torque, slip ratio will be very high when braking, since brake torque is not affected by friction
	//and again, the excess friction torque should not be greater than brake torque, since the friction torque should not flip the sign of the relative rotation to the ground
	float ExcessFrictionTorque = FMath::Clamp(FrictionTorque - ClampedFrictionTorque, -SimData.BrakeTorque, SimData.BrakeTorque);

	//get the angular velocity without braking
	SimData.AngularVelocity += SimData.PhysicsDeltaTime * SimData.TotalInertiaInv * (SimData.DriveTorque - ClampedFrictionTorque);
	float AngVelSignIfNotBraking = FMath::Sign(SimData.AngularVelocity);

	//finally the sign of brake torque can be defined
	float ActuralBrakingTorque = SimData.BrakeTorque * (-AngVelSignIfNotBraking);
	SimData.AngularVelocity += SimData.PhysicsDeltaTime * SimData.TotalInertiaInv * (ActuralBrakingTorque - ExcessFrictionTorque);

	//zero cross check
	//if the wheel is locked, the angular velocity should be 0
	SimData.bIsLocked = AngVelSignIfNotBraking != FMath::Sign(SimData.AngularVelocity);
	SimData.AngularVelocity *= !SimData.bIsLocked;
}

void FVehicleWheelSolver::UpdateSlipAngle(bool bHitGround)
{
	//get velocity2d
	FVector2D Velocity2DNormalized = SimData.LocalLinearVelocity.GetSafeNormal();

	//calculate lateral slip
	//get slip angle
	float SlipAngleRaw = FMath::Asin(-Velocity2DNormalized.Y);
	SlipAngleRaw = FMath::RadiansToDegrees(SlipAngleRaw);

	//approximate low speed slip angle
	//-V.y = |V| * sin(SlipAngle) ¡Ö |V| * SlipAngle ¡Ö SlipAngle; when slip angle is small and |v| --> 1.f
	float LowSpeedSlipAngle = -FMath::RadiansToDegrees(SimData.LocalLinearVelocity.Y);

	//get alpha for lerp, lerp between high speed and low speed
	float Alpha = FMath::GetMappedRangeValueClamped(FVector2D(0.5, 1.f), FVector2D(0.f, 1.f), SimData.LocalLinearVelocity.SquaredLength());

	//combine low speed and high speed
	SimData.SlipAngle = FMath::Lerp(LowSpeedSlipAngle, SlipAngleRaw, Alpha) * bHitGround;
	SimData.SlipAngle = FMath::Clamp(SimData.SlipAngle, -90.f, 90.f);
}

void FVehicleWheelSolver::UpdateSlipRatio(bool bHitGround)
{
	//Calculate longitudinal slip
	SimData.LongSlipVelocity = SimData.AngularVelocity * SimData.R - SimData.LocalLinearVelocity.X;
	SimData.LongSlipVelocity *= bHitGround;

	//calculate slip ratio
	SimData.SlipRatio = SafeDivide(SimData.LongSlipVelocity, FMath::Max(FMath::Abs(SimData.LocalLinearVelocity.X), 1.f));
}

float FVehicleWheelSolver::CalculateConstraintLongForce(float SprungMass)
{
	float ForceRequiredToBringToStop = FMath::Abs(SimData.LocalLinearVelocity.X * SimData.PhysicsDeltaTimeInv * SprungMass);
	ForceRequiredToBringToStop += FMath::Abs(SimData.DriveTorque * SimData.RInv);

	//get linear brake force
	float SignedBrakeTorque = SimData.BrakeTorque * (-FMath::Sign(SimData.LocalLinearVelocity.X));
	SignedBrakeTorque = FMath::Clamp(SignedBrakeTorque, -ForceRequiredToBringToStop, ForceRequiredToBringToStop);

	//torque from ground interaction is the torque required to make angularvelocity == linearvelocity / radius
	float GroundAngularVelocity = SimData.LocalLinearVelocity.X * SimData.RInv;
	SimData.TorqueFromGroundInteraction = SimData.PhysicsDeltaTimeInv * SimData.TotalInertia * (SimData.AngularVelocity - GroundAngularVelocity);

	//get longitudinal force
	return (SimData.DriveTorque + SignedBrakeTorque + SimData.TorqueFromGroundInteraction) * SimData.RInv;
}

float FVehicleWheelSolver::CalculateConstraintLatForce(float SprungMass)
{
	float ForceRequiredToBringToStop = -SimData.LocalLinearVelocity.Y * SimData.PhysicsDeltaTimeInv * SprungMass;
	
	//more stable lateral force at low speed
	float Scale = FMath::Clamp(FMath::Abs(SimData.LocalLinearVelocity.Y), 0.5, 1.f);
	
	return ForceRequiredToBringToStop * Scale;
}

void FVehicleWheelSolver::UpdateGravityCompensationOnSlope(
	float ForceIntoSurface,
	bool bHitGround,
	const FVector& LongForceDir, 
	const FVector& LatForceDir)
{
	if (!bHitGround)
	{
		SimData.GravityCompensationForce = FVector2D(0.f);
		return;
	}

	FVector2D GravityComp = FVector2D(0.f, 0.f);

	// sin(theta): LatForceDir.Z or LongForceDir.Z
	// ExtraGravityForce = Fz * TanTheta; 
	// TanTheta ¡Ö SinTheta, when theta is small; 
	// when theta is large, the vehicle maybe should not be able to hold itself on the slope; 
	// so there might be no need to calculate tan(theta)
	   
	// the vehicle is not able to stop on the slope
	// even if the slope is not steep
	// so we add a little extra friction force
	float Mu = SimData.DynFrictionMultiplier;

	//Lateral:
	float SlipSign = FMath::Sign(-SimData.LocalLinearVelocity.Y);
	GravityComp.Y = (LatForceDir.Z + FMath::Abs(LatForceDir.Z) * SlipSign * Mu) * ForceIntoSurface;

	// longitudinal:
	if (SimData.bIsLocked)
	{
		SlipSign = FMath::Sign(SimData.LongSlipVelocity);
		GravityComp.X = (LongForceDir.Z + FMath::Abs(LongForceDir.Z) * SlipSign * Mu) * ForceIntoSurface;

		//consider brake force?
		float BrakeForce = SimData.BrakeTorque * SimData.RInv;	//SimData.BrakeTorque is always positive
		GravityComp.X = FMath::Clamp(GravityComp.X, -BrakeForce, BrakeForce);
	}

	// in the lateral direction, the wheel can be treated as it is always braking

	// then do some smoothing
	float Scale = TargetWheelComponent->TireConfig.ParkingResponseSpeed;
	FVector2D Smoothing = FVector2D(FMath::Abs(SimData.LongSlipVelocity), FMath::Abs(SimData.LocalLinearVelocity.Y));
	Smoothing *= Scale;

	// clamp between 0 to 1
	Smoothing.X = FMath::Clamp(Smoothing.X, 0.f, 1.f);
	Smoothing.Y = FMath::Clamp(Smoothing.Y, 0.f, 1.f);

	SimData.GravityCompensationForce.X += (GravityComp.X - SimData.GravityCompensationForce.X) * Smoothing.X;
	SimData.GravityCompensationForce.Y += (GravityComp.Y - SimData.GravityCompensationForce.Y) * Smoothing.Y;
}

float FVehicleWheelSolver::CalculateScaledWheelLoad(float SprungMass, float WheelLoad, float Saturation)
{
	float NormWheelLoad = SprungMass * 9.8;
	float LoadRatio = SafeDivide(WheelLoad, NormWheelLoad);
	float b = (1.f - Saturation) / (2.f + 2.f * Saturation);
	float LoadScale = LoadRatio / (1.f + b * LoadRatio);
	return LoadScale * NormWheelLoad;
}

void FVehicleWheelSolver::UpdateTireForce(
	float SprungMass, 
	float WheelLoad,	
	bool bHitGround,
	const FVector& LongForceDirUnNorm,
	const FVector& LatForceDirUnNorm)
{
	UpdateSlipRatio(bHitGround);
	UpdateSlipAngle(bHitGround);

	if (!bHitGround)
	{
		SimData.MFTireForce2D = FVector2D(0.f);
		SimData.TireForce2D = FVector2D(0.f);
		SimData.TireForce = FVector(0.f);
		return;
	}

	FVehicleTireConfig& TireConfig = TargetWheelComponent->TireConfig;

	// Constraint tire force
	FVector2D ConstraintForce = FVector2D(
		CalculateConstraintLongForce(SprungMass),
		CalculateConstraintLatForce(SprungMass)
	);

	FVector2D TargetTireForce = ConstraintForce;

	// get wheel load
	float ScaledWheelLoad = CalculateScaledWheelLoad(SprungMass, WheelLoad, TireConfig.WheelLoadInfluenceFactor);
	float AvailableGrip = SimData.DynFrictionMultiplier * ScaledWheelLoad;
	float SlipInputScale = SafeDivide(TireConfig.FrictionMultiplier, SimData.DynFrictionMultiplier);

	// if the user has only setup one of the Fx or Fy curve, the Constraint force on the other direction should be cut, before computing combined slip
	bool bUseFxCurve = (TireConfig.Fx != nullptr);
	bool bUseFyCurve = (TireConfig.Fy != nullptr);

	// magic formula
	float MaxFx = TireConfig.MaxFx * AvailableGrip;
	float MaxFy = TireConfig.MaxFy * AvailableGrip;
	if (bUseFxCurve)
	{
		float Fx = MaxFx * TireConfig.Fx->GetFloatValue(FMath::Abs(SimData.SlipRatio * SlipInputScale));
		TargetTireForce.X = FMath::Clamp(TargetTireForce.X, -Fx, Fx);
	}
	if (bUseFyCurve)
	{
		float Fy = MaxFy * TireConfig.Fy->GetFloatValue(FMath::Abs(SimData.SlipAngle * SlipInputScale));
		TargetTireForce.Y = FMath::Clamp(TargetTireForce.Y, -Fy, Fy);
	}
	if (bUseFxCurve != bUseFyCurve) {
		if (!bUseFxCurve) TargetTireForce.X = FMath::Clamp(TargetTireForce.X, -MaxFx, MaxFx);
		if (!bUseFyCurve) TargetTireForce.Y = FMath::Clamp(TargetTireForce.Y, -MaxFy, MaxFy);
	}

	// combined slip, if the user has setup curves
	float Gx = 1.f;
	float Gy = 1.f;
	if (TireConfig.Gx)Gx = TireConfig.Gx->GetFloatValue(FMath::Abs(SimData.SlipAngle * SlipInputScale));
	if (TireConfig.Gy)Gy = TireConfig.Gy->GetFloatValue(FMath::Abs(SimData.SlipRatio * SlipInputScale));
	TargetTireForce.X *= Gx;
	TargetTireForce.Y *= Gy;

	// lerp between constraint force and magic formula force
	FVector2D V = FVector2D(0.f);
	V.X = FMath::Max(FMath::Abs(SimData.LocalLinearVelocity.X), FMath::Abs(SimData.AngularVelocity * SimData.R));
	V.Y = FMath::Abs(SimData.LocalLinearVelocity.Y);
	float ScalarV = V.Length();
	float Alpha = FMath::GetMappedRangeValueClamped(
		TireConfig.StictionSpeedThreshold,
		FVector2D(0.f, 1.f),
		ScalarV
	);
	TargetTireForce = FMath::Lerp(ConstraintForce, TargetTireForce, Alpha);

	// the friction ellipse
	FVector2D MaxForceInv = FVector2D(SafeDivide(1.f, MaxFx), SafeDivide(1.f, MaxFy));
	FVector2D NormalizedForce = TargetTireForce * MaxForceInv;
	if (NormalizedForce.SquaredLength() > 1.f)
	{
		NormalizedForce = NormalizedForce.GetSafeNormal();
		TargetTireForce.X = NormalizedForce.X * MaxFx;
		TargetTireForce.Y = NormalizedForce.Y * MaxFy;
	}

	// relaxation length
	float Distance = SimData.PhysicsDeltaTime * ScalarV;
	FVector2D RelaxationLengthSmoothing = FVector2D(Distance) / FVector2D::Max(TireConfig.RelaxationLength, FVector2D(SMALL_NUMBER));

	// smoothing factor under stiction condition (low speed)
	float Scale = TireConfig.ParkingResponseSpeed;
	FVector2D StictionSmoothing = FVector2D(FMath::Abs(SimData.LongSlipVelocity), FMath::Abs(SimData.LocalLinearVelocity.Y));
	StictionSmoothing *= Scale;

	// lerp and smoothen
	FVector2D SmoothingFactor = FMath::Lerp(StictionSmoothing, RelaxationLengthSmoothing, Alpha);
	SmoothingFactor.X = FMath::Clamp(SmoothingFactor.X, 0.f, 1.f);
	SmoothingFactor.Y = FMath::Clamp(SmoothingFactor.Y, 0.f, 1.f);

	SimData.MFTireForce2D += (TargetTireForce - SimData.MFTireForce2D) * SmoothingFactor;

	// final tire force, magic formula + gravity
	SimData.TireForce2D = SimData.MFTireForce2D + SimData.GravityCompensationForce;

	// cut with friction ellipse again to prevent overshoot
	NormalizedForce = SimData.TireForce2D * MaxForceInv;
	if (NormalizedForce.SquaredLength() > 1.f)
	{
		NormalizedForce = NormalizedForce.GetSafeNormal();
		SimData.TireForce2D.X = NormalizedForce.X * MaxFx;
		SimData.TireForce2D.Y = NormalizedForce.Y * MaxFy;
	}

	SimData.TireForce = SimData.TireForce2D.X * LongForceDirUnNorm + SimData.TireForce2D.Y * LatForceDirUnNorm;
}

float FVehicleWheelSolver::SafeDivide(float a, float b)
{
	return (FMath::IsNearlyZero(b)) ? 0.0f : a / b;
}