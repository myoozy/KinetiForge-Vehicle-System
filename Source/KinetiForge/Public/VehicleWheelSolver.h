// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "VehicleWheelStructs.h"
#include "VehicleSuspensionStructs.h"

class UVehicleWheelComponent;
class UCurveFloat;

/**
 * 
 */
class KINETIFORGE_API FVehicleWheelSolver
{
public:
	FVehicleWheelSolver();
	~FVehicleWheelSolver();

	void Initialize(const FVehicleTireConfig& TireConfig);
	void UpdateWheel(
		float InPhysicsDeltaTime,
		float InDriveTorque,
		float InBrakeTorque,
		float InHandbrakeTorque,
		float InReflectedInertia,
		const FVehicleWheelConfig& WheelConfig,
		const FVehicleTireConfig& TireConfig,
		const FVehicleABSConfig& ABSConfig,
		const FVehicleSuspensionSimState& SuspensionState);
	void DrawWheelForce(
		UVehicleWheelComponent* WheelComponent,
		const FVehicleSuspensionSimState& SuspensionState,
		float Duration = -1,
		float Thickness = 5,
		float Length = 1,
		bool bDrawVelocity = true,
		bool bDrawSlip = true,
		bool bDrawInertia = true);
	void SmoothenSlip(float InDeltaTime, float InInterpSpeed);
	void UpdateCachedLUTs(const FVehicleTireConfig& Config);

	static float GetTangentAtOrigin(const FRichCurve& Curve);

	// all data during simulation
	FVehicleWheelSimState State;
	FVehicleWheelCachedLUTs CachedLUTs;

	// e.g. for skid mark / sound
	float SmoothenedSlipRatio = 0.f;
	// e.g. for skid mark / sound
	float SmoothenedSlipAngle = 0.f;

protected:

private:
	void PredictSlipAndUpdateABS(
		const FVehicleABSConfig& ABSConfig,
		const float TargetBrakeTorque,
		const bool bHitGround);
	void UpdateDynamicFrictionMultiplier(
		const FVehicleTireConfig& TireConfig,
		const float ImpactFriction);
	void UpdateLinearVelocity(
		const FVector3f& LongForceDir,
		const FVector3f& LatForceDir,
		const FVector3f& ImpactPointWorldVelocity);
	void WheelAcceleration(
		const FVector3f& LongForceDir,
		float SlipVelocityTolerance = 0.1f);
	void UpdateSlipAngle(bool bHitGround);
	void UpdateSlipRatio(bool bHitGround);
	/**Returns transient slip ratio and slip angle (normalized)*/
	FVector2f UpdateTransientSlip(
		const bool bHitGround,
		const FVector2f& RelaxationLength);
	float CalculateConstraintLongForce(float EffectiveSprungMass);
	float CalculateConstraintLatForce(float EffectiveSprungMassy);
	FVector2f CalculateGravityCompensationOnSlope(
		float PositiveForceIntoSurface,
		bool bHitGround,
		const FVector3f& LongForceDir,
		const FVector3f& LatForceDir);
	float CalculateScaledWheelLoad(
		float StaticSprungMass,
		float WheelLoad,
		float Saturation);
	void UpdateTireForce(
		float StaticSprungMass,
		float EffectiveSprungMassLong,
		float EffectiveSprungMassLat,
		float PositiveForceIntoSurface,
		bool bHitGround,
		const FVector3f& LongForceDirUnNorm,
		const FVector3f& LatForceDirUnNorm,
		const FVector3f& LongForceDir,
		const FVector3f& LatForceDir,
		const FVehicleTireConfig& TireConfig);
};
