// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "VehicleWheelSolver.h"
#include "VehicleSuspensionSolver.h"

/** Owned numerical settings. Curve pointers in the copied legacy configs are cleared.
 * Tables must already be prepared by their resource owner; copying them also preserves
 * legacy tire stiffness/peak metadata when a curve is removed. */
struct KINETIFORGE_API FVehicleWheelRuntimeSettings
{
	FVehicleWheelConfig WheelConfig;
	FVehicleTireConfig TireConfig;
	FVehicleABSConfig ABSConfig;
	FVehicleSuspensionKinematicsConfig KinematicsConfig;
	FVehicleSuspensionSpringConfig SpringConfig;
	FVehicleWheelCachedLUTs TireLUTs;
	FVehicleSuspensionCachedLUTs SuspensionLUTs;

	static FVehicleWheelRuntimeSettings FromCachedConfig(
		const FVehicleWheelConfig& WheelConfig, const FVehicleTireConfig& TireConfig,
		const FVehicleABSConfig& ABSConfig,
		const FVehicleSuspensionKinematicsConfig& KinematicsConfig,
		const FVehicleSuspensionSpringConfig& SpringConfig,
		const FVehicleWheelCachedLUTs& TireLUTs,
		const FVehicleSuspensionCachedLUTs& SuspensionLUTs);
};

/** One consumer owns these values. No component, world, query or body binding is stored.
 * Suspension is advanced separately through its D2 stages, then consumed by wheel phases.
 * Components may bind through FVehicleWheelBridge; only the simulation owner advances this core. */
struct KINETIFORGE_API FVehicleWheelRuntime
{
	FVehicleWheelRuntimeSettings Settings;
	FVehicleWheelSimState WheelState;
	FVehicleWheelSimContext WheelContext;
	FVehicleSuspensionSimState SuspensionState;

	/** Simulation-owner operation at a step boundary; does not reset history or command values. */
	void ApplySettings(FVehicleWheelRuntimeSettings NewSettings);
	void SetSprungMass(float NewSprungMass);
	void PrepareWheel(float MacroDeltaTime, const FTransform& ChassisWorldTransform);
	void IntegrateWheel(float SubstepDeltaTime, float DriveTorque, float BrakeTorque,
		float HandbrakeTorque, float ReflectedInertia);
	void FinalizeWheel();
};
