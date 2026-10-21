// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "VehicleDrivetrainStructs.h"

/** Value inputs for one clutch step. Angular velocities are rad/s, torques Nm. */
struct KINETIFORGE_API FVehicleClutchStepInput
{
	/** Legacy equations require a positive step, in seconds. */
	float DeltaTime = 0.f;
	/** Pedal convention: 0 engaged, 1 released. */
	float ClutchPedalValue = 0.f;
	float GearboxInputAngularVelocity = 0.f;
	float GearboxReflectedInertia = 0.f;
	float GearRatio = 0.f;
	float ReflectedDriveShaftStiffness = 0.f;
	float EngineAngularVelocity = 0.f;
	float EngineInertia = 0.f;
	float EngineMaxTorque = 0.f;
};

/**
 * Clutch equations with caller-owned state. No UObject or shared mutable state
 * is accessed. Each future transmission path must supply its own clutch state.
 * Shared engine/output dynamics remain the responsibility of the assembly.
 */
class KINETIFORGE_API FVehicleClutchSolver
{
public:
	static void Update(const FVehicleClutchConfig& Config, FVehicleClutchSimState& State,
		const FVehicleClutchStepInput& Input);

	// Kept separate for the existing component's protected C++ helper interface.
	static float SolveSpringTorque(const FVehicleClutchConfig& Config, FVehicleClutchSimState& State,
		float DeltaTime, float ClutchSlip, float EngineInertia,
		float GearboxReflectedInertia, float ReflectedDriveShaftStiffness);
	static float SolveDampingTorque(const FVehicleClutchConfig& Config, FVehicleClutchSimState& State,
		float DeltaTime, float ClutchSlip, float EngineInertia, float GearboxReflectedInertia);
	static float SolveConstraintTorque(FVehicleClutchSimState& State,
		float DeltaTime, float ClutchSlip, float EngineInertia, float GearboxReflectedInertia);
};

/** Output-side quantities reflected to the input of one selected gear path. */
struct KINETIFORGE_API FVehicleGearPathReflection
{
	float InputAngularVelocity = 0.f;
	float ReflectedInertia = 0.f;
	float ReflectedDriveShaftStiffness = 0.f;
};

/**
 * Stateless gear-path relations. Ratio = input speed / output speed, including
 * reverse sign. No current gear or shift timer is assumed, so different paths
 * can use different ratios. This does not solve a dual-clutch transmission.
 */
class KINETIFORGE_API FVehicleGearPathSolver
{
public:
	static float CalculateOutputTorque(float InputTorque, float GearRatio, float Efficiency);
	static FVehicleGearPathReflection ReflectOutputShaft(float OutputAngularVelocity,
		float OutputInertia, float OutputDriveShaftStiffness, float GearRatio);
};
