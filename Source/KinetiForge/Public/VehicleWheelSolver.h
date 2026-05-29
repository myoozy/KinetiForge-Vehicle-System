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

	void PreStep(
		float InMacroDeltaTime,
		const FVehicleSuspensionSimState& SuspensionState,
		const FVehicleWheelConfig& WheelConfig,
		const FVehicleTireConfig& TireConfig
	);

	void Substep(
		float InSubstepDeltaTime,
		float InDriveTorque,
		float InBrakeTorque,
		float InHandbrakeTorque,
		float InReflectedInertia,
		const FVehicleWheelConfig& WheelConfig,
		const FVehicleTireConfig& TireConfig,
		const FVehicleABSConfig& ABSConfig,
		const FVehicleSuspensionSimState& SuspensionState
	);

	void PostStep(

	);

	void DrawWheelForce(
		UVehicleWheelComponent* WheelComponent,
		const FVehicleSuspensionSimState& SuspensionState,
		float Duration = -1,
		float Thickness = 5,
		float Length = 1,
		bool bDrawVelocity = true,
		bool bDrawSlip = true,
		bool bDrawInertia = true);
	void UpdateCachedLUTs(const FVehicleTireConfig& Config);

	static float GetTangentAtOrigin(const FRichCurve& Curve);

	// all data during simulation
	FVehicleWheelSimState State;
	FVehicleWheelSimContext CurrentContext;
	FVehicleWheelCachedLUTs CachedLUTs;

protected:

private:
	static void PredictSlipAndUpdateABS(
		FVehicleWheelSimState& LocalState,
		const FVehicleWheelSimContext& Context,
		const FVehicleABSConfig& ABSConfig,
		const float TargetBrakeTorque,
		const bool bHitGround);
	static void UpdateDynamicFrictionMultiplier(
		FVehicleWheelSimState& LocalState,
		const FVehicleWheelSimContext& Context,
		const FVehicleTireConfig& TireConfig,
		const float ImpactFriction);
	static void UpdateLinearVelocity(
		FVehicleWheelSimState& LocalState,
		const FVector3f& LongForceDir,
		const FVector3f& LatForceDir,
		const FVector3f& ImpactPointWorldVelocity);
	static void WheelAcceleration(
		FVehicleWheelSimState& LocalState,
		const FVehicleWheelSimContext& Context,
		const float LastTireLongitudinalForce,
		const float SlipVelocityTolerance = 0.1f);
	static void UpdateSlipAngle(
		FVehicleWheelSimState& LocalState,
		const bool bHitGround);
	static void UpdateSlipRatio(
		FVehicleWheelSimState& LocalState,
		const FVehicleWheelSimContext& Context,
		const bool bHitGround);
	/**Returns transient slip ratio and slip angle (normalized)*/
	static FVector2f UpdateTransientSlip(
		FVehicleWheelSimState& LocalState,
		const FVehicleWheelSimContext& Context,
		const bool bHitGround,
		const FVector2f& RelaxationLength);
	static float CalculateConstraintLongForce(
		FVehicleWheelSimState& LocalState,
		const FVehicleWheelSimContext& Context,
		const float EffectiveSprungMass);
	static float CalculateConstraintLatForce(
		FVehicleWheelSimState& LocalState,
		const FVehicleWheelSimContext& Context,
		const float EffectiveSprungMassy);
	static FVector2f CalculateGravityCompensationOnSlope(
		FVehicleWheelSimState& LocalState,
		FVehicleWheelSimContext& Context,
		const float PositiveForceIntoSurface,
		const bool bHitGround,
		const FVector3f& LongForceDir,
		const FVector3f& LatForceDir);
	static float CalculateScaledWheelLoad(
		const float StaticSprungMass,
		const float WheelLoad,
		const float Saturation);
	static FVector2f SolveTireForce(
		FVehicleWheelSimState& LocalState,
		const FVehicleWheelSimContext& Context,
		const float StaticSprungMass,
		const float EffectiveSprungMassLong,
		const float EffectiveSprungMassLat,
		const float PositiveForceIntoSurface,
		const bool bHitGround,
		const FVector3f& LongForceDirUnNorm,
		const FVector3f& LatForceDirUnNorm,
		const FVector3f& LongForceDir,
		const FVector3f& LatForceDir,
		const FVehicleTireConfig& TireConfig,
		const FVehicleWheelCachedLUTs& TireLUTs);
};
