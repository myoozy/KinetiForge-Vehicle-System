// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "VehicleDrivetrainStructs.h"

/** Per-step values at the input and two outputs of the existing differential. */
struct KINETIFORGE_API FVehicleDifferentialStepInput
{
	float InputTorque = 0.f;
	float LeftAngularVelocity = 0.f;
	float RightAngularVelocity = 0.f;
	float LeftTotalInertia = 0.f;
	float RightTotalInertia = 0.f;
	float DeltaTime = 0.f;
};

struct KINETIFORGE_API FVehicleDifferentialTorqueOutput
{
	float LeftTorque = 0.f;
	float RightTorque = 0.f;
};

/** Values returned for one driven axle in the existing central split. */
struct KINETIFORGE_API FVehicleTransferCaseAxleOutput
{
	float DriveTorque = 0.f;
	float NormalizedTorqueWeight = 0.f;
};

/**
 * Value-only differential equations. No axle discovery, wheel updates or
 * UObject access. The assembly still supplies the existing fixed topology.
 */
class KINETIFORGE_API FVehicleDifferentialSolver
{
public:
	static FVehicleDifferentialTorqueOutput CalculateOutputTorques(
		const FVehicleLimitedSlipDifferentialConfig& Config, const FVehicleDifferentialStepInput& Input);
	static float CalculateOpenOutputTorque(float InputTorque, float GearRatio);
	static float CalculateInputAngularVelocity(float LeftAngularVelocity, float RightAngularVelocity, float GearRatio);
	static float CalculateReflectedInputInertia(float LeftInertia, float RightInertia, float GearRatio);
	static float CalculateReflectedOutputInertiaPerWheel(float InputReflectedInertia, float GearRatio);

	static float CalculateLockedAngularVelocity(float TotalAngularMomentum, float TotalInertia);
	/** DriveTorque is already multiplied by the transfer-case ratio. */
	static FVehicleTransferCaseAxleOutput CalculateTransferCaseAxleOutput(
		const FVehicleLimitedSlipDifferentialConfig& Config, float DriveTorque,
		float LockedAngularVelocity, float AxleAngularVelocity, float AxleInertia,
		float AxleTorqueWeight, float TotalTorqueWeight, float DeltaTime);
};
