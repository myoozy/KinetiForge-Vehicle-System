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
	void UpdateCachedLUTs(const FVehicleTireConfig& Config);

	static float GetTangentAtOrigin(const FRichCurve& Curve);

	// all data during simulation
	FVehicleWheelSimState State;
	FVehicleWheelCachedLUTs CachedLUTs;

protected:

private:
	static void PredictSlipAndUpdateABS(
		FVehicleWheelSimState& LocalState,
		FVehicleWheelSimContext& Ctx,
		const FVehicleABSConfig& ABSConfig,
		const float TargetBrakeTorque,
		const bool bHitGround);
	static void UpdateDynamicFrictionMultiplier(
		FVehicleWheelSimState& LocalState,
		FVehicleWheelSimContext& Ctx,
		const FVehicleTireConfig& TireConfig,
		const float ImpactFriction);
	static void UpdateLinearVelocity(
		FVehicleWheelSimState& LocalState,
		const FVector3f& LongForceDir,
		const FVector3f& LatForceDir,
		const FVector3f& ImpactPointWorldVelocity);
	static void WheelAcceleration(
		FVehicleWheelSimState& LocalState,
		FVehicleWheelSimContext& Ctx,
		const FVector3f& LongForceDir,
		float SlipVelocityTolerance = 0.1f);
	static void UpdateSlipAngle(
		FVehicleWheelSimState& LocalState,
		bool bHitGround);
	static void UpdateSlipRatio(
		FVehicleWheelSimState& LocalState,
		FVehicleWheelSimContext& Ctx, 
		bool bHitGround);
	/**Returns transient slip ratio and slip angle (normalized)*/
	static FVector2f UpdateTransientSlip(
		FVehicleWheelSimState& LocalState,
		FVehicleWheelSimContext& Ctx,
		const bool bHitGround,
		const FVector2f& RelaxationLength);
	static float CalculateConstraintLongForce(
		FVehicleWheelSimState& LocalState,
		FVehicleWheelSimContext& Ctx, 
		float EffectiveSprungMass);
	static float CalculateConstraintLatForce(
		FVehicleWheelSimState& LocalState,
		FVehicleWheelSimContext& Ctx, 
		float EffectiveSprungMassy);
	static FVector2f CalculateGravityCompensationOnSlope(
		FVehicleWheelSimState& LocalState,
		FVehicleWheelSimContext& Ctx,
		float PositiveForceIntoSurface,
		bool bHitGround,
		const FVector3f& LongForceDir,
		const FVector3f& LatForceDir);
	static float CalculateScaledWheelLoad(
		float StaticSprungMass,
		float WheelLoad,
		float Saturation);
	static void UpdateTireForce(
		FVehicleWheelSimState& LocalState,
		FVehicleWheelSimContext& Ctx,
		float StaticSprungMass,
		float EffectiveSprungMassLong,
		float EffectiveSprungMassLat,
		float PositiveForceIntoSurface,
		bool bHitGround,
		const FVector3f& LongForceDirUnNorm,
		const FVector3f& LatForceDirUnNorm,
		const FVector3f& LongForceDir,
		const FVector3f& LatForceDir,
		const FVehicleTireConfig& TireConfig,
		const FVehicleWheelCachedLUTs& TireLUTs);
};
