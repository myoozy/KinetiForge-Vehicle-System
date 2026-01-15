// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

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

	bool Initialize(UVehicleWheelComponent* InTargetWheelComponent);
	void UpdateWheel(
		float InPhysicsDeltaTime,
		float InDriveTorque,
		float InBrakeTorque,
		float InHandbrakeTorque,
		float InReflectedInertia,
		const FVehicleSuspensionSimState& SuspensionState);
	void DrawWheelForce(
		UWorld* InCurrentWorld,
		const FVehicleSuspensionSimState& SuspensionState,
		float Duration = -1,
		float Thickness = 5,
		float Length = 1,
		bool bDrawVelocity = true,
		bool bDrawSlip = true,
		bool bDrawInertia = true);
	void SmoothenSlip(float InDeltaTime, float InInterpSpeed);
	void UpdateCachedRichCurves();

	static float GetTangentAtOrigin(const FRichCurve& Curve);

	// all data during simulation
	FVehicleWheelSimState State;
	FVehicleWheelCachedRichCurves CachedCurves;

	// e.g. for skid mark / sound
	float SmoothenedSlipRatio = 0.f;
	// e.g. for skid mark / sound
	float SmoothenedSlipAngle = 0.f;

protected:
	UVehicleWheelComponent* TargetWheelComponent;

private:
	void PredictSlipAndUpdateABS(float TargetBrakeTorque, bool bHitGround);
	void UpdateDynamicFrictionMultiplier(float ImpactFriction);
	void UpdateLinearVelocity(
		const FVector3f& LongForceDir,
		const FVector3f& LatForceDir,
		const FVector3f& ImpactPointWorldVelocity);
	void WheelAcceleration(
		const FVector3f& LongForceDir,
		float SlipVelocityTolerance = 0.1f);
	void UpdateSlipAngle(bool bHitGround);
	void UpdateSlipRatio(bool bHitGround);
	float CalculateConstraintLongForce(float SprungMass);
	float CalculateConstraintLatForce(float SprungMass);
	void UpdateGravityCompensationOnSlope(
		float ForceIntoSurface,
		bool bHitGround,
		const FVector3f& LongForceDir,
		const FVector3f& LatForceDir);
	float CalculateScaledWheelLoad(
		float SprungMass,
		float WheelLoad,
		float Saturation);
	void UpdateTireForce(
		float SprungMass,
		float PositiveSuspensionForce,
		bool bHitGround,
		const FVector3f& LongForceDirUnNorm,
		const FVector3f& LatForceDirUnNorm);
};
