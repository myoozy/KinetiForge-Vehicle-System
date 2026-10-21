// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehicleDifferentialSolver.h"
#include "VehicleDrivetrainMath.h"

FVehicleDifferentialTorqueOutput FVehicleDifferentialSolver::CalculateOutputTorques(
	const FVehicleLimitedSlipDifferentialConfig& Config, const FVehicleDifferentialStepInput& Input)
{
	const float OpenDiffTorque = CalculateOpenOutputTorque(Input.InputTorque, Config.GearRatio);
	const float AverageAngularVelocity = 0.5 * (Input.LeftAngularVelocity + Input.RightAngularVelocity);
	const bool bIsDrive = (OpenDiffTorque * AverageAngularVelocity) >= 0.f;
	const float CurrentLockRatio = bIsDrive ? Config.DriveLockRatio : Config.CoastLockRatio;

	//calculate the torque required for both sides to match the average angular velocity
	const float ReducedInertia = VehicleDrivetrainMath::DivideOrDefault(Input.LeftTotalInertia * Input.RightTotalInertia,
		Input.LeftTotalInertia + Input.RightTotalInertia);
	const float AngularVelocityDifference = Input.LeftAngularVelocity - Input.RightAngularVelocity;
	const float MaxTransferTorque = VehicleDrivetrainMath::DivideOrDefault(ReducedInertia * AngularVelocityDifference, Input.DeltaTime);
	const float TransferTorque = MaxTransferTorque * CurrentLockRatio;

	FVehicleDifferentialTorqueOutput Output;
	Output.LeftTorque = OpenDiffTorque - TransferTorque;
	Output.RightTorque = OpenDiffTorque + TransferTorque;
	return Output;
}

float FVehicleDifferentialSolver::CalculateOpenOutputTorque(float InputTorque, float GearRatio)
{
	return 0.5 * GearRatio * InputTorque;
}

float FVehicleDifferentialSolver::CalculateInputAngularVelocity(
	float LeftAngularVelocity, float RightAngularVelocity, float GearRatio)
{
	return (LeftAngularVelocity + RightAngularVelocity) * 0.5 * GearRatio;
}

float FVehicleDifferentialSolver::CalculateReflectedInputInertia(
	float LeftInertia, float RightInertia, float GearRatio)
{
	return VehicleDrivetrainMath::DivideOrDefault(LeftInertia + RightInertia, GearRatio * GearRatio);
}

float FVehicleDifferentialSolver::CalculateReflectedOutputInertiaPerWheel(
	float InputReflectedInertia, float GearRatio)
{
	return 0.5 * InputReflectedInertia * GearRatio * GearRatio;
}

float FVehicleDifferentialSolver::CalculateLockedAngularVelocity(float TotalAngularMomentum, float TotalInertia)
{
	// Calculate the physically correct locked angular velocity (momentum-weighted average)
	return VehicleDrivetrainMath::DivideOrDefault(TotalAngularMomentum, TotalInertia);
}

FVehicleTransferCaseAxleOutput FVehicleDifferentialSolver::CalculateTransferCaseAxleOutput(
	const FVehicleLimitedSlipDifferentialConfig& Config, float DriveTorque,
	float LockedAngularVelocity, float AxleAngularVelocity, float AxleInertia,
	float AxleTorqueWeight, float TotalTorqueWeight, float DeltaTime)
{
	// central diff locking logic
	// Use TargetLockedAngVel instead of simple arithmetic average
	const float AngularVelocityDifference = LockedAngularVelocity - AxleAngularVelocity;
	const bool bIsDrive = (DriveTorque * LockedAngularVelocity) >= 0.f;
	const float CurrentLockRatio = bIsDrive ? Config.DriveLockRatio : Config.CoastLockRatio;

	// The calculated TorqueBias is now guaranteed to sum to exactly 0 across all axles
	// This legacy comment describes the analytic balance; float sums can have roundoff.
	const float TorqueBias = VehicleDrivetrainMath::DivideOrDefault(AngularVelocityDifference * AxleInertia * CurrentLockRatio, DeltaTime);
	FVehicleTransferCaseAxleOutput Output;
	Output.NormalizedTorqueWeight = VehicleDrivetrainMath::DivideOrDefault(AxleTorqueWeight, TotalTorqueWeight);

	// Combine mechanical static split + LSD clutch pack transfer
	Output.DriveTorque = DriveTorque * Output.NormalizedTorqueWeight + TorqueBias;
	return Output;
}
