// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehicleTransmissionSolver.h"
#include "VehicleDrivetrainMath.h"

void FVehicleClutchSolver::Update(const FVehicleClutchConfig& Config,
	FVehicleClutchSimState& State, const FVehicleClutchStepInput& Input)
{
	const float ClutchPedalValue = FMath::Clamp(Input.ClutchPedalValue, 0.f, 1.f);
	State.MaxClutchTorque = Input.EngineMaxTorque * Config.Capacity;
	State.ClutchLock = FMath::Clamp(float(Input.GearRatio != 0.f) - ClutchPedalValue, 0.f, 1.f);
	const float ClutchSlip = Input.EngineAngularVelocity - Input.GearboxInputAngularVelocity;

	switch (Config.SimMode)
	{
	default:
	case EClutchSimMode::ConstraintLock:
		SolveConstraintTorque(State, Input.DeltaTime, ClutchSlip,
			Input.EngineInertia, Input.GearboxReflectedInertia);
		break;
	case EClutchSimMode::FrictionClutch:
		SolveSpringTorque(Config, State, Input.DeltaTime, ClutchSlip,
			Input.EngineInertia, Input.GearboxReflectedInertia, Input.ReflectedDriveShaftStiffness);
		break;
	case EClutchSimMode::FluidCoupling:
		SolveDampingTorque(Config, State, Input.DeltaTime, ClutchSlip,
			Input.EngineInertia, Input.GearboxReflectedInertia);
		break;
	}
}

float FVehicleClutchSolver::SolveSpringTorque(const FVehicleClutchConfig& Config,
	FVehicleClutchSimState& State, float DeltaTime, float ClutchSlip,
	float EngineInertia, float GearboxReflectedInertia, float ReflectedDriveShaftStiffness)
{
	const float ReducedInertia = VehicleDrivetrainMath::DivideOrDefault(GearboxReflectedInertia * EngineInertia,
		GearboxReflectedInertia + EngineInertia);
	// to simulate torson spring on drive shaft
	// to simulate torson spring on clutch
	// The shaft and clutch stiffnesses form the same series spring as before.
	const float SeriesStiffness = VehicleDrivetrainMath::DivideOrDefault(ReflectedDriveShaftStiffness * Config.TorsionalStiffness,
		ReflectedDriveShaftStiffness + Config.TorsionalStiffness);
	const float CriticalDamping = 2.f * FMath::Sqrt(SeriesStiffness * ReducedInertia);
	const float EffectiveDamping = CriticalDamping * Config.DampingRatio;
	const float EngagedClutchSlip = ClutchSlip * State.ClutchLock; // ClutchSlip * ClutchLock
	const float ImplicitVelocityCoefficient = SeriesStiffness * DeltaTime + EffectiveDamping;
	const float TorqueNumerator = SeriesStiffness * State.AngleDiff
		+ ImplicitVelocityCoefficient * EngagedClutchSlip;
	const float TorqueDenominator = 1.f + VehicleDrivetrainMath::DivideOrDefault(ImplicitVelocityCoefficient * DeltaTime, ReducedInertia);
	float ClutchTorque = TorqueNumerator / TorqueDenominator;
	const float CurrentCapacity = State.MaxClutchTorque * State.ClutchLock;

	if (FMath::Abs(ClutchTorque) > CurrentCapacity)
	{
		ClutchTorque = FMath::Sign(ClutchTorque) * CurrentCapacity;
	}
	else
	{
		State.AngleDiff += EngagedClutchSlip * DeltaTime;
	}
	State.AngleDiff *= State.ClutchLock;
	return State.ClutchTorque = ClutchTorque;
}

float FVehicleClutchSolver::SolveDampingTorque(const FVehicleClutchConfig& Config,
	FVehicleClutchSimState& State, float DeltaTime, float ClutchSlip,
	float EngineInertia, float GearboxReflectedInertia)
{
	State.AngleDiff = 0.f;
	const float ReducedInertia = VehicleDrivetrainMath::DivideOrDefault(GearboxReflectedInertia * EngineInertia,
		GearboxReflectedInertia + EngineInertia);
	const float TorqueNumerator = Config.ViscousDamping * ClutchSlip;
	const float TorqueDenominator = 1.f + VehicleDrivetrainMath::DivideOrDefault(Config.ViscousDamping * DeltaTime, ReducedInertia);
	const float ClutchTorque = TorqueNumerator / TorqueDenominator;
	const float CurrentCapacity = State.MaxClutchTorque * State.ClutchLock;
	return State.ClutchTorque = FMath::Clamp(ClutchTorque, -CurrentCapacity, CurrentCapacity);
}

float FVehicleClutchSolver::SolveConstraintTorque(FVehicleClutchSimState& State,
	float DeltaTime, float ClutchSlip, float EngineInertia, float GearboxReflectedInertia)
{
	const float ReducedInertia = VehicleDrivetrainMath::DivideOrDefault(GearboxReflectedInertia * EngineInertia,
		GearboxReflectedInertia + EngineInertia);
	// Retain the legacy positive-step requirement and AngleDiff history.
	const float LockTorque = (ClutchSlip * ReducedInertia) / DeltaTime;
	const float CurrentCapacity = State.MaxClutchTorque * State.ClutchLock;
	return State.ClutchTorque = FMath::Clamp(LockTorque, -CurrentCapacity, CurrentCapacity);
}

float FVehicleGearPathSolver::CalculateOutputTorque(float InputTorque, float GearRatio, float Efficiency)
{
	return InputTorque * GearRatio * Efficiency;
}

FVehicleGearPathReflection FVehicleGearPathSolver::ReflectOutputShaft(float OutputAngularVelocity,
	float OutputInertia, float OutputDriveShaftStiffness, float GearRatio)
{
	FVehicleGearPathReflection Reflection;
	Reflection.InputAngularVelocity = OutputAngularVelocity * GearRatio;
	const float InverseGearRatioSquared = VehicleDrivetrainMath::DivideOrDefault(1.f, GearRatio * GearRatio);
	Reflection.ReflectedInertia = OutputInertia * InverseGearRatioSquared;
	Reflection.ReflectedDriveShaftStiffness = OutputDriveShaftStiffness * InverseGearRatioSquared;
	return Reflection;
}
