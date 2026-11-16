// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehicleWheelRuntime.h"

FVehicleWheelRuntimeSettings FVehicleWheelRuntimeSettings::FromCachedConfig(
	const FVehicleWheelConfig& WheelConfig, const FVehicleTireConfig& TireConfig,
	const FVehicleABSConfig& ABSConfig,
	const FVehicleSuspensionKinematicsConfig& KinematicsConfig,
	const FVehicleSuspensionSpringConfig& SpringConfig,
	const FVehicleWheelCachedLUTs& TireLUTs,
	const FVehicleSuspensionCachedLUTs& SuspensionLUTs)
{
	FVehicleWheelRuntimeSettings Settings;
	Settings.WheelConfig = WheelConfig;
	Settings.TireConfig = TireConfig;
	Settings.ABSConfig = ABSConfig;
	Settings.KinematicsConfig = KinematicsConfig;
	Settings.SpringConfig = SpringConfig;
	Settings.TireLUTs = TireLUTs;
	Settings.SuspensionLUTs = SuspensionLUTs;
	Settings.TireConfig.Fx = nullptr;
	Settings.TireConfig.Fy = nullptr;
	Settings.TireConfig.CamberToLateralDrift = nullptr;
	Settings.KinematicsConfig.CamberCurve = nullptr;
	Settings.KinematicsConfig.ToeCurve = nullptr;
	Settings.KinematicsConfig.CasterCurve = nullptr;
	Settings.KinematicsConfig.AntiDiveCurve = nullptr;
	Settings.KinematicsConfig.AntiSquatCurve = nullptr;
	Settings.KinematicsConfig.AntiRollCurve = nullptr;
	Settings.KinematicsConfig.SpringMotionRatioCurve = nullptr;
	return Settings;
}

void FVehicleWheelRuntime::ApplySettings(FVehicleWheelRuntimeSettings NewSettings)
{
	Settings = MoveTemp(NewSettings);
}

void FVehicleWheelRuntime::SetSprungMass(float NewSprungMass)
{
	FVehicleSuspensionSolver::SetSprungMass(SuspensionState, NewSprungMass);
}

void FVehicleWheelRuntime::PrepareWheel(float MacroDeltaTime, const FTransform& ChassisWorldTransform)
{
	WheelContext.SpinReactionAngularImpulse = FVector::ZeroVector;
	StepSpinAxis = FVector(SuspensionState.WheelWorldRightVector).GetSafeNormal();
	StepWheelInertia = FMath::Max(0.f, Settings.WheelConfig.Inertia);
	bSpinStepPrepared = MacroDeltaTime > SMALL_NUMBER && !StepSpinAxis.IsNearlyZero();
	if (!bSpinStepPrepared) PreviousSpinAxis = FVector::ZeroVector;
	// Positive wheel speed rotates about +wheel-right (the same convention as wheel animation).
	// First use seeds orientation history without inventing a gyro impulse for an existing spin.
	const FVector InitialAxis = PreviousSpinAxis.IsNearlyZero() ? StepSpinAxis : PreviousSpinAxis;
	StepInitialSpinMomentum = InitialAxis * (StepWheelInertia * WheelState.AngularVelocity);
	FVehicleWheelSolver::PrepareWheel(WheelState, WheelContext, Settings.TireLUTs,
		MacroDeltaTime, ChassisWorldTransform, SuspensionState, Settings.WheelConfig, Settings.TireConfig);
}

void FVehicleWheelRuntime::IntegrateWheel(float SubstepDeltaTime, float DriveTorque, float BrakeTorque,
	float HandbrakeTorque, float ReflectedInertia)
{
	FVehicleWheelSolver::IntegrateWheel(WheelState, WheelContext, Settings.TireLUTs,
		SubstepDeltaTime, DriveTorque, BrakeTorque, HandbrakeTorque, ReflectedInertia,
		Settings.WheelConfig, Settings.TireConfig, Settings.ABSConfig, SuspensionState);
}

void FVehicleWheelRuntime::FinalizeWheel()
{
	FVehicleWheelSolver::FinalizeWheel(WheelState, WheelContext);
	if (!bSpinStepPrepared) return;
	// Use the complete macrostep speed change, not AngularAcceleration (last tire substep).
	// World-axis change includes chassis rotation, steering and suspension articulation.
	// Only actual wheel inertia belongs here; reflected drivetrain inertia has no defined 3D axis.
	WheelContext.SpinReactionAngularImpulse = StepInitialSpinMomentum
		- StepSpinAxis * (StepWheelInertia * WheelState.AngularVelocity);
	PreviousSpinAxis = StepSpinAxis;
	bSpinStepPrepared = false;
}

void FVehicleWheelRuntime::ResetSpinMomentumHistory()
{
	PreviousSpinAxis = FVector::ZeroVector;
	bSpinStepPrepared = false;
	WheelContext.SpinReactionAngularImpulse = FVector::ZeroVector;
}
