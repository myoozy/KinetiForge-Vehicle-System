// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehiclePowerUnitRuntime.h"

bool FVehiclePowerUnitRuntime::Initialize()
{
	// reset rev-limiter
	EngineState.RevLimiterTimer = EngineParameters.RevLimiterTime;
	return FVehicleGearboxSolver::CalculateGearRatios(GearboxConfig, ForwardGearRatios, ReverseGearRatios);
}

bool FVehiclePowerUnitRuntime::ShiftToGear(int32 TargetGear, bool bImmediate)
{
	if (!FVehicleGearboxSolver::BeginShift(GearboxConfig, GearboxState,
		ForwardGearRatios.Num(), ReverseGearRatios.Num(), TargetGear)) return false;
	if (bImmediate || GearboxConfig.ShiftDelay < SMALL_NUMBER)
	{
		FVehicleGearboxSolver::CompleteShift(GearboxConfig, GearboxState, ForwardGearRatios, ReverseGearRatios);
	}
	return true;
}

bool FVehiclePowerUnitRuntime::AdvanceShift(float DeltaTime)
{
	return FVehicleGearboxSolver::AdvanceShift(GearboxConfig, GearboxState,
		ForwardGearRatios, ReverseGearRatios, DeltaTime);
}

float FVehiclePowerUnitRuntime::UpdateEngineAndCalculateOutputTorque(
	float DeltaTime, float Throttle, bool bHasDrivenAxles)
{
	// update engine: use the cached clutch torque, as in DriveAssembly.
	const float CachedClutchTorque = bHasDrivenAxles ? ClutchState.ClutchTorque : 0.f;
	// sequential transmission disables spark when shifting up
	FVehicleEngineSolver::UpdatePhysics(EngineParameters, TurboConfig, ExhaustConfig, TorqueLUT,
		EngineState, DeltaTime, Throttle, CachedClutchTorque, GearboxState.bShouldCutSpark,
		bShouldTriggerTurboBlowOffCallback, bShouldTriggerBackfiringCallback);

	// get gearbox output torque; P2 stays upstream of the selected gear ratio.
	return FVehicleGearPathSolver::CalculateOutputTorque(CachedClutchTorque + GearboxState.P2MotorTorque,
		GearboxState.CurrentGearRatio, GearboxConfig.Efficiency);
}

void FVehiclePowerUnitRuntime::UpdateClutchFromOutputShaft(float DeltaTime, float ClutchPedalValue,
	float OutputAngularVelocity, float OutputInertia, float OutputDriveShaftStiffness)
{
	// get gearbox inputshaft angular velocity
	const FVehicleGearPathReflection Reflection = FVehicleGearPathSolver::ReflectOutputShaft(
		OutputAngularVelocity, OutputInertia, OutputDriveShaftStiffness, GearboxState.CurrentGearRatio);
	// get clutch torque for next frame
	FVehicleClutchStepInput Input;
	Input.DeltaTime = DeltaTime;
	Input.ClutchPedalValue = ClutchPedalValue;
	Input.GearboxInputAngularVelocity = Reflection.InputAngularVelocity;
	Input.GearboxReflectedInertia = Reflection.ReflectedInertia;
	Input.GearRatio = GearboxState.CurrentGearRatio;
	Input.ReflectedDriveShaftStiffness = Reflection.ReflectedDriveShaftStiffness;
	Input.EngineAngularVelocity = EngineState.EngineAngularVelocity;
	Input.EngineInertia = EngineParameters.EngineInertia;
	Input.EngineMaxTorque = FVehicleEngineSolver::CalculateMaxTorque(EngineParameters, TurboConfig);
	FVehicleClutchSolver::Update(ClutchConfig, ClutchState, Input);
}
