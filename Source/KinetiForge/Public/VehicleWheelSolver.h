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

	bool Initialize(TWeakObjectPtr<UVehicleWheelComponent> InTargetWheelComponent);
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

	FVehicleWheelSimData SimData;

protected:
	TWeakObjectPtr<UVehicleWheelComponent> TargetWheelComponent;

private:
	void UpdateABS(float TargetBrakeTorque, bool bHitGround);
	void ComputeDynamicFrictionMultiplier(const FHitResult& HitStruct);
	void ComputeLinearVelocity(
		FVector LongForceDir,
		FVector LatForceDir,
		const FHitResult& HitStruct );
	void WheelAcceleration(float AccelerationTolerance = 0.1);
	void ComputeSlipAngle(bool bHitGround);
	void ComputeSlipRatio(bool bHitGround);
	float ComputeRigidLongForce(float SprungMass);
	float ComputeRigidLatForce(float SprungMass);
	FVector2D ComputeGravityProjectionOnSlope(
		float PositiveSuspensionForce,
		FVector LongForceDir,
		FVector LatForceDir);
	FVector2D ComputeTireSmoothingFactor();
	void ComputeTireForce(
		float SprungMass,
		float PositiveSuspensionForce,
		bool bHitGround,
		FVector LongForceDirUnNorm,
		FVector LatForceDirUnNorm,
		FVector2D GravityProjOnSlope);
	void ApplyTireForce(const FVehicleSuspensionSimData& SuspensionSimData);
	float SafeDivide(float a, float b);
};
