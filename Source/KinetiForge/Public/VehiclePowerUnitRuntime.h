// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "VehicleEngineSolver.h"
#include "VehicleGearboxSolver.h"
#include "VehicleTransmissionSolver.h"
#include "VehicleUtilities.h"

/**
 * Single-clutch power unit with owned value data. Does not discover
 * components, schedule physics, dispatch delegates, or own chassis/wheels.
 * DriveAssembly will bind and drive it in stage C. Its two step functions keep
 * the existing downstream update between engine/output and clutch feedback.
 */
struct KINETIFORGE_API FVehiclePowerUnitRuntime
{
	FVehicleEngineParameters EngineParameters;
	FVehicleEngineTurboConfig TurboConfig;
	FVehicleEngineExhaustConfig ExhaustConfig;
	FVehicleLUT<32> TorqueLUT = FVehicleLUT<32>(1.f);
	FVehicleEngineSimState EngineState;
	FVehicleClutchConfig ClutchConfig;
	FVehicleClutchSimState ClutchState;
	FVehicleGearboxConfig GearboxConfig;
	FVehicleGearboxRuntimeState GearboxState;
	TArray<float> ForwardGearRatios;
	TArray<float> ReverseGearRatios;
	bool bShouldTriggerTurboBlowOffCallback = false;
	bool bShouldTriggerBackfiringCallback = false;

	/** Call after supplying configuration/LUT, before stepping a new runtime. */
	bool Initialize();
	bool ShiftToGear(int32 TargetGear, bool bImmediate = false);
	/** Explicit standalone clock; legacy component timing has not moved here. */
	bool AdvanceShift(float DeltaTime);
	float UpdateEngineAndCalculateOutputTorque(float DeltaTime, float Throttle, bool bHasDrivenAxles);
	void UpdateClutchFromOutputShaft(float DeltaTime, float ClutchPedalValue,
		float OutputAngularVelocity, float OutputInertia, float OutputDriveShaftStiffness);
};
