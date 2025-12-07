// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "VehicleWheelStructs.h"
#include "VehicleSuspensionStructs.h"

class UVehicleWheelComponent;

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
		const FVehicleSuspensionSimData& SuspensionSimData);
	void DrawWheelForce(
		UWorld* InCurrentWorld,
		const FVehicleSuspensionSimData& InSuspensionSimData,
		float Duration = -1,
		float Thickness = 5,
		float Length = 1,
		bool bDrawVelocity = true,
		bool bDrawSlip = true,
		bool bDrawInertia = true);
	void SmoothenSlip(float InDeltaTime, float InInterpSpeed);

	// all data during simulation
	FVehicleWheelSimData SimData;

	// e.g. for skid mark / sound
	float SmoothenedSlipRatio = 0.f;
	// e.g. for skid mark / sound
	float SmoothenedSlipAngle = 0.f;

protected:
	UVehicleWheelComponent* TargetWheelComponent;

private:
	void UpdateABS(float TargetBrakeTorque, bool bHitGround);
	void ComputeDynamicFrictionMultiplier(const FHitResult& HitStruct);
	void ComputeLinearVelocity(
		const FVector& LongForceDir,
		const FVector& LatForceDir,
		const FHitResult& HitStruct );
	void WheelAcceleration(float AccelerationTolerance = 0.1);
	void ComputeSlipAngle(bool bHitGround);
	void ComputeSlipRatio(bool bHitGround);
	float ComputeConstraintLongForce(float SprungMass);
	float ComputeConstraintLatForce(float SprungMass);
	FVector2D ComputeGravityProjectionOnSlope(
		float PositiveSuspensionForce,
		const FVector& LongForceDir,
		const FVector& LatForceDir);
	FVector2D ComputeTireSmoothingFactor();
	float CalculateScaledWheelLoad(
		float SprungMass,
		float WheelLoad,
		float Saturation);
	void ComputeTireForce(
		float SprungMass,
		float PositiveSuspensionForce,
		bool bHitGround,
		const FVector& LongForceDirUnNorm,
		const FVector& LatForceDirUnNorm,
		const FVector2D& GravityProjOnSlope);
	float SafeDivide(float a, float b);
};
