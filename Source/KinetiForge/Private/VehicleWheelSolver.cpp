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

bool FVehicleWheelSolver::Initialize(UVehicleWheelComponent* InTargetWheelComponent, UPrimitiveComponent* InCarbody)
{
	TargetWheelComponent = InTargetWheelComponent;
	Carbody = InCarbody;
	return TargetWheelComponent && Carbody;
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

	if (!TargetWheelComponent || !Carbody) return;

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

	UpdateABS(FMath::Abs(InBrakeTorque), SuspensionSimData.bHitGround);
	SimData.BrakeTorqueFromHandbrake = FMath::Abs(InHandbrakeTorque);
	SimData.BrakeTorque = SimData.BrakeTorqueFromBrake + Config.RollingRisistance + SimData.BrakeTorqueFromHandbrake;

	ComputeDynamicFrictionMultiplier(SuspensionSimData.HitStruct);

	//get the direction of longitudinal and lateral force in world space
	//if the vectors are not normalized, the force can be affected by camber
	FVector LongForceDirUnNorm = FVector::CrossProduct(SuspensionSimData.WheelRightVector, SuspensionSimData.HitStruct.ImpactNormal);
	FVector LatForceDirUnNorm = FVector::VectorPlaneProject(SuspensionSimData.WheelRightVector, SuspensionSimData.HitStruct.ImpactNormal);

	FVector LongForceDir = LongForceDirUnNorm.GetSafeNormal();
	FVector LatForceDir = LatForceDirUnNorm.GetSafeNormal();

	ComputeLinearVelocity(LongForceDir, LatForceDir, SuspensionSimData.HitStruct);

	WheelAcceleration();

	FVector2D GravProj = ComputeGravityProjectionOnSlope(SuspensionSimData.PositiveSuspensionForce, LongForceDir, LatForceDir);

	ComputeTireForce(
		SuspensionSimData.SprungMass,
		SuspensionSimData.PositiveSuspensionForce,
		SuspensionSimData.bHitGround,
		LongForceDirUnNorm,
		LatForceDirUnNorm, 
		GravProj);

	ApplyTireForce(SuspensionSimData);
}

void FVehicleWheelSolver::DrawWheelForce(UWorld* InCurrentWorld, const FVehicleSuspensionSimData& InSuspensionSimData, float Duration, float Thickness, float Length, bool bDrawVelocity, bool bDrawSlip, bool bDrawInertia)
{
	if (!InCurrentWorld || !TargetWheelComponent)return;

	FVehicleTireConfig& TireConfig = TargetWheelComponent->TireConfig;

	FVector TempForward = FVector::CrossProduct(InSuspensionSimData.WheelRightVector, InSuspensionSimData.HitStruct.Normal);
	FVector TempRight = FVector::VectorPlaneProject(InSuspensionSimData.WheelRightVector, InSuspensionSimData.HitStruct.Normal);
	FVector TempUp = InSuspensionSimData.HitStruct.Normal;
	FVector TempImpactPoint = InSuspensionSimData.HitStruct.ImpactPoint;
	FRotator TempRot = FRotationMatrix::MakeFromYZ(TempRight, TempForward).Rotator();
	FVector TempScale = FVector(TireConfig.MaxFx, TireConfig.MaxFy, 1.f);
	FTransform TempTrans = FTransform(TempRot, TempImpactPoint, TempScale);

	Length *= 0.01;
	float ScaledSuspForce = Length * InSuspensionSimData.PositiveSuspensionForce;

	//draw grip circle
	FColor GripCircleColor = FColor(0, 191, 255);
	DrawDebugCircle(InCurrentWorld, TempTrans.ToMatrixWithScale(), Length * InSuspensionSimData.PositiveSuspensionForce * SimData.DynFrictionMultiplier, 16, GripCircleColor,
		false, Duration, 0, Thickness, false);

	//draw raw force
	FVector Long = TempForward * SimData.LongForce * Length;
	FVector Lat = TempRight * SimData.LatForce * Length;
	FColor RawForceColor = FColor(0, 255, 191);
	DrawDebugLine(InCurrentWorld, TempImpactPoint, TempImpactPoint + Long, RawForceColor, false, Duration, 0, Thickness);
	DrawDebugLine(InCurrentWorld, TempImpactPoint, TempImpactPoint + Lat, RawForceColor, false, Duration, 0, Thickness);

	//draw final force
	DrawDebugLine(InCurrentWorld, TempImpactPoint, SimData.TyreForce * Length + TempImpactPoint, GripCircleColor, false, Duration, 0, Thickness);

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
		DrawDebugString(InCurrentWorld, TempWheelLocation + TempOffset, TempString, 0, VelociyColor, 0, true);
	}

	if (bDrawSlip)
	{
		FColor SlipColor = FColor(127, 63, 255);
		FVector TempWheelLocation = WheelTrans.GetLocation();
		FVector TempDrawOffset = TargetWheelComponent->WheelConfig.Radius * WheelTrans.GetRotation().GetUpVector();
		FString TextSlipRatio = FString(TEXT("SlipRatio = ")) + FString::SanitizeFloat(SimData.SlipRatio);
		FString TextSlipAngle = FString(TEXT("SlipAngle = ")) + FString::SanitizeFloat(SimData.SlipAngle);
		DrawDebugString(InCurrentWorld, TempWheelLocation + TempDrawOffset, TextSlipRatio, 0, SlipColor, 0, true, Length * 100);
		DrawDebugString(InCurrentWorld, TempWheelLocation - TempDrawOffset, TextSlipAngle, 0, SlipColor, 0, true, Length * 100);
	}

	if (bDrawInertia)
	{
		FColor InertiaColor = FColor(255, 255, 255);
		FVector TempWheelLocation = WheelTrans.GetLocation();
		FString TextInertia = FString(TEXT("Inertia = ")) + FString::SanitizeFloat(SimData.TotalInertia);
		DrawDebugString(InCurrentWorld, TempWheelLocation, TextInertia, 0, InertiaColor, 0, true, Length * 100);
	}
}

void FVehicleWheelSolver::UpdateABS(float TargetBrakeTorque, bool bHitGround)
{
	FVehicleABSConfig& ABSConfig = TargetWheelComponent->ABSConfig;

	if (!ABSConfig.bAntiBrakeSystemEnabled || !bHitGround)
	{
		SimData.BrakeTorqueFromBrake = TargetBrakeTorque;
		SimData.bABSTriggered = 0;
	}
	else
	{
		SimData.bABSTriggered = FMath::Abs(SimData.SlipRatio) > ABSConfig.MaxSlipRatio
			&& FMath::Abs(SimData.LocalLinearVelocity.X) > ABSConfig.ActivationSpeed
			&& TargetBrakeTorque > 0;

		if (SimData.bABSTriggered)TargetBrakeTorque = 0;

		SimData.BrakeTorqueFromBrake = FMath::FInterpTo(SimData.BrakeTorqueFromBrake, TargetBrakeTorque, SimData.PhysicsDeltaTime, ABSConfig.InterpSpeed);
	}
}

void FVehicleWheelSolver::ComputeDynamicFrictionMultiplier(const FHitResult& HitStruct)
{
	FVehicleTireConfig& TireConfig = TargetWheelComponent->TireConfig;

	//if physics mat is valid
	if (UPhysicalMaterial* TempPhysMat = HitStruct.PhysMaterial.Get())
	{
		float MuContact = TempPhysMat->Friction;

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

void FVehicleWheelSolver::ComputeLinearVelocity(FVector LongForceDir, FVector LatForceDir, const FHitResult& HitStruct)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UpdateVehicleWheelLinearVelocity);

	if (HitStruct.bBlockingHit)
	{
		FVector LinVelWorldA = UAsyncTickFunctions::ATP_GetLinearVelocityAtPoint(Carbody, HitStruct.ImpactPoint, "NONE");
		FVector LinVelWorldB = FVector(0.f);

		if (IsValid(HitStruct.GetComponent()) && HitStruct.GetComponent()->Mobility != EComponentMobility::Static)
		{
			LinVelWorldB = UAsyncTickFunctions::ATP_GetLinearVelocityAtPoint(HitStruct.GetComponent(), HitStruct.ImpactPoint, HitStruct.BoneName);
		}

		SimData.WorldLinearVelocity = 0.01 * (LinVelWorldA - LinVelWorldB);
		SimData.LocalLinearVelocity.X = FVector::DotProduct(LongForceDir, SimData.WorldLinearVelocity);
		SimData.LocalLinearVelocity.Y = FVector::DotProduct(LatForceDir, SimData.WorldLinearVelocity);
	}
	else
	{
		SimData.WorldLinearVelocity = FVector(0.f);
		SimData.LocalLinearVelocity = FVector2D(0.f);
	}
}

void FVehicleWheelSolver::WheelAcceleration(float AccelerationTolerance)
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
	float FrictionTorque = SimData.LongForce * SimData.R;
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

void FVehicleWheelSolver::ComputeSlipAngle(bool bHitGround)
{
	//get velocity2d
	FVector2D Velocity2DNormalized = SimData.LocalLinearVelocity.GetSafeNormal();

	//calculate lateral slip
	//get slip angle
	float SlipAngleRaw = FMath::Asin(-Velocity2DNormalized.Y);
	SlipAngleRaw = FMath::RadiansToDegrees(SlipAngleRaw);

	//approximate low speed slip angle
	//-V.y = |V| * sin(SlipAngle) �� |V| * SlipAngle �� SlipAngle; when slip angle is small and |v| --> 1.f
	float LowSpeedSlipAngle = -FMath::RadiansToDegrees(SimData.LocalLinearVelocity.Y);

	//get alpha for lerp, lerp between high speed and low speed
	float Alpha = FMath::GetMappedRangeValueClamped(FVector2D(0.5, 1.f), FVector2D(0.f, 1.f), SimData.LocalLinearVelocity.SquaredLength());

	//combine low speed and high speed
	SimData.SlipAngle = FMath::Lerp(LowSpeedSlipAngle, SlipAngleRaw, Alpha) * bHitGround;
	SimData.SlipAngle = FMath::Clamp(SimData.SlipAngle, -90.f, 90.f);
}

void FVehicleWheelSolver::ComputeSlipRatio(bool bHitGround)
{
	//Calculate longitudinal slip
	SimData.LongSlipVelocity = SimData.AngularVelocity * SimData.R - SimData.LocalLinearVelocity.X;
	SimData.LongSlipVelocity *= bHitGround;

	//calculate slip ratio
	SimData.SlipRatio = SafeDivide(SimData.LongSlipVelocity, FMath::Max(FMath::Abs(SimData.LocalLinearVelocity.X), 1.f));
}

float FVehicleWheelSolver::ComputeRigidLongForce(float SprungMass)
{
	float ForceRequiredToBringToStop = FMath::Abs(SimData.LocalLinearVelocity.X * SimData.PhysicsDeltaTimeInv * SprungMass);

	//get linear brake force
	float SignedBrakeTorque = SimData.BrakeTorque * (-FMath::Sign(SimData.LocalLinearVelocity.X));
	SignedBrakeTorque = FMath::Clamp(SignedBrakeTorque, -ForceRequiredToBringToStop, ForceRequiredToBringToStop);

	//torque from ground interaction is the torque required to make angularvelocity == linearvelocity / radius
	float GroundAngularVelocity = SimData.LocalLinearVelocity.X * SimData.RInv;
	SimData.TorqueFromGroundInteraction = SimData.PhysicsDeltaTimeInv * SimData.TotalInertia * (SimData.AngularVelocity - GroundAngularVelocity);

	//get longitudinal force
	return (SimData.DriveTorque + SignedBrakeTorque + SimData.TorqueFromGroundInteraction) * SimData.RInv;
}

float FVehicleWheelSolver::ComputeRigidLatForce(float SprungMass)
{
	float ForceRequiredToBringToStop = -SimData.LocalLinearVelocity.Y * SimData.PhysicsDeltaTimeInv * SprungMass;
	
	//more stable lateral force at low speed
	float Scale = FMath::Clamp(FMath::Abs(SimData.LocalLinearVelocity.Y), 0.5, 1.f);
	
	return ForceRequiredToBringToStop * Scale;
}

FVector2D FVehicleWheelSolver::ComputeGravityProjectionOnSlope(float PositiveSuspensionForce, FVector LongForceDir, FVector LatForceDir)
{
	FVector2D GravityProj = FVector2D(0.f, 0.f);

	//sin(theta): LatForceDir.Z or LongForceDir.Z
	//ExtraGravityForce = Fz * TanTheta; 
	//TanTheta �� SinTheta, when theta is small; 
	//when theta is large, the vehicle maybe should not be able to hold itself on the slope; 
	//so there might be no need to calculate tan(theta)

	//the vehicle is not able to stop on the slope
	//even if the slope is not steep
	//so we add a little extra friction force
	float Mu = 2.f * SimData.DynFrictionMultiplier;

	//Lateral:
	float SlipSign = FMath::Sign(-SimData.LocalLinearVelocity.Y);
	GravityProj.Y = (LatForceDir.Z + FMath::Abs(LatForceDir.Z) * SlipSign * Mu) * PositiveSuspensionForce;

	//longitudinal:
	SlipSign = FMath::Sign(SimData.LongSlipVelocity);
	GravityProj.X = (LongForceDir.Z + FMath::Abs(LongForceDir.Z) * SlipSign * Mu) * PositiveSuspensionForce;

	//consider brake force?
	float BrakeForce = SimData.BrakeTorque * SimData.RInv;	//SimData.BrakeTorque is always positive
	GravityProj.X = FMath::Clamp(GravityProj.X, -BrakeForce, BrakeForce);

	//in the lateral direction, the wheel can be treated as it is always braking

	return GravityProj;
}

FVector2D FVehicleWheelSolver::ComputeTireSmoothingFactor()
{
	FVehicleTireConfig& TireConfig = TargetWheelComponent->TireConfig;

	FVector2D Result = FVector2D(0.f, 0.f);
	Result.X = SafeDivide(SimData.PhysicsDeltaTime * FMath::Abs(SimData.LongSlipVelocity), TireConfig.RelaxationLength.X);
	Result.X = FMath::Clamp(Result.X, 0.f, 1.f);

	Result.Y = SafeDivide(SimData.PhysicsDeltaTime * FMath::Abs(SimData.LocalLinearVelocity.Y), TireConfig.RelaxationLength.Y);
	Result.Y = FMath::Clamp(Result.Y, 0.f, 1.f);

	return Result;
}

void FVehicleWheelSolver::ComputeTireForce(float SprungMass, float PositiveSuspensionForce,	bool bHitGround, FVector LongForceDirUnNorm, FVector LatForceDirUnNorm, FVector2D GravityProjOnSlope)
{
	if (!bHitGround)
	{
		SimData.LongForce = SimData.LatForce = 0.f;
		SimData.TyreForce = FVector(0.f);
	}

	FVehicleTireConfig& TireConfig = TargetWheelComponent->TireConfig;

	ComputeSlipRatio(bHitGround);
	ComputeSlipAngle(bHitGround);

	//rigid tire force
	float TargetLongForce = ComputeRigidLongForce(SprungMass);
	float TargetLatForce = ComputeRigidLatForce(SprungMass);
	float AvailableGrip = SimData.DynFrictionMultiplier * PositiveSuspensionForce;

	//if the user has only setup one of the Fx or Fy curve, the rigid force on the other direction should be cut, before computing combined slip
	bool bUseFxCurve = (TireConfig.Fx != nullptr);
	bool bUseFyCurve = (TireConfig.Fy != nullptr);

	//magic formula
	float MaxFx = TireConfig.MaxFx * AvailableGrip;
	float MaxFy = TireConfig.MaxFy * AvailableGrip;
	if (bUseFxCurve)
	{
		float Fx = MaxFx * TireConfig.Fx->GetFloatValue(FMath::Abs(SimData.SlipRatio));
		TargetLongForce = FMath::Clamp(TargetLongForce, -Fx, Fx);
	}
	if (bUseFyCurve)
	{
		float Fy = MaxFy * TireConfig.Fy->GetFloatValue(FMath::Abs(SimData.SlipAngle));
		TargetLatForce = FMath::Clamp(TargetLatForce, -Fy, Fy);
	}
	if (bUseFxCurve != bUseFyCurve) {
		if (!bUseFxCurve) TargetLongForce = FMath::Clamp(TargetLongForce, -MaxFx, MaxFx);
		if (!bUseFyCurve) TargetLatForce = FMath::Clamp(TargetLatForce, -MaxFy, MaxFy);
	}

	//consider gravity
	TargetLongForce += GravityProjOnSlope.X;
	TargetLatForce += GravityProjOnSlope.Y;

	//combined slip, if the user has setup curves
	float Gx = 1.f;
	float Gy = 1.f;
	if (TireConfig.Gx)Gx = TireConfig.Gx->GetFloatValue(FMath::Abs(SimData.SlipAngle));
	if (TireConfig.Gy)Gy = TireConfig.Gy->GetFloatValue(FMath::Abs(SimData.SlipRatio));
	TargetLongForce *= Gx;
	TargetLatForce  *= Gy;

	//smoothing
	FVector2D SmoothingFactor = ComputeTireSmoothingFactor();
	SimData.LongForce += (TargetLongForce - SimData.LongForce) * SmoothingFactor.X;
	SimData.LatForce += (TargetLatForce - SimData.LatForce) * SmoothingFactor.Y;

	//combined slip
	//the friction circle
	FVector2D NormalizedForce = FVector2D(
		SafeDivide(SimData.LongForce, MaxFx),
		SafeDivide(SimData.LatForce, MaxFy));

	if (NormalizedForce.SquaredLength() > 1.f)
	{
		NormalizedForce = NormalizedForce.GetSafeNormal();
		SimData.LongForce = NormalizedForce.X * MaxFx;
		SimData.LatForce = NormalizedForce.Y * MaxFy;
	}

	SimData.TyreForce = SimData.LongForce * LongForceDirUnNorm + SimData.LatForce * LatForceDirUnNorm;
}

void FVehicleWheelSolver::ApplyTireForce(const FVehicleSuspensionSimData& SuspensionSimData)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UpdateVehicleWheelAddForce);

	if (!SuspensionSimData.HitStruct.bBlockingHit)return;

	FVector TotalImpulse = (SimData.TyreForce + SuspensionSimData.SuspensionForceVector) * SimData.PhysicsDeltaTime;

	FVector WidthBias = FVector(0.f);
	FVector PosToApplyImpulse = FVector(0.f);

	switch (TargetWheelComponent->SuspensionKinematicsConfig.PositionToApplyForce)
	{
	case EPositionToApplyForce::ImpactPoint:
		PosToApplyImpulse = SuspensionSimData.HitStruct.ImpactPoint;
		break;
	case EPositionToApplyForce::ImpactPointWithBias:
		WidthBias = SuspensionSimData.WheelRightVector * (SuspensionSimData.WheelPos * TargetWheelComponent->WheelConfig.Width * 0.5);
		WidthBias = FVector::VectorPlaneProject(WidthBias, SuspensionSimData.HitStruct.ImpactNormal);
		PosToApplyImpulse = SuspensionSimData.HitStruct.ImpactPoint + WidthBias;
		break;
	case EPositionToApplyForce::WheelCenter:
		PosToApplyImpulse = SuspensionSimData.WheelWorldPos;
		break;
	default:
		PosToApplyImpulse = SuspensionSimData.HitStruct.ImpactPoint;
		break;
	}

	UAsyncTickFunctions::ATP_AddImpulseAtPosition(Carbody, PosToApplyImpulse, TotalImpulse * 100, "NONE");

	if (IsValid(SuspensionSimData.HitStruct.GetComponent()) &&
		SuspensionSimData.HitStruct.GetComponent()->IsSimulatingPhysics())
	{
		UAsyncTickFunctions::ATP_AddImpulseAtPosition(SuspensionSimData.HitStruct.GetComponent(), SuspensionSimData.HitStruct.ImpactPoint, TotalImpulse * -100, SuspensionSimData.HitStruct.BoneName);
	}
}

float FVehicleWheelSolver::SafeDivide(float a, float b)
{
	return (FMath::IsNearlyZero(b)) ? 0.0f : a / b;
}