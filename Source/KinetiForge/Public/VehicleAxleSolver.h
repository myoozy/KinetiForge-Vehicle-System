// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "VehicleAxleStructs.h"
#include "VehicleDrivetrainStructs.h"
#include "VehicleSuspensionStructs.h"

/** Instantaneous wheel values; read again after integrating the wheel substep.
 * A null pointer in the stages below denotes an absent wheel, never an absent body. */
struct KINETIFORGE_API FVehicleAxleWheelInput
{
	float AngularVelocity = 0.f;
	float EffectiveInertia = 0.f;
	float IntrinsicInertia = 0.f;
	float PredictedSlipRatio = 0.f;
};

/** Axle-level equations only. Wheel integration, suspension and body queries stay separate. */
class KINETIFORGE_API FVehicleAxleSolver
{
public:
	/** Pure axle-level kinematic helpers shared by the legacy component and ordinary runtime. */
	static void UpdateLinearVelocity(FVehicleAxleSimState& State,
		const FVehicleSuspensionSimState* LeftSuspension, const FQuat4f* LeftChassisRotation,
		const FVehicleSuspensionSimState* RightSuspension, const FQuat4f* RightChassisRotation);
	static void UpdateSteeringAssist(FVehicleAxleSimState& State,
		const FVehicleAxleSteeringConfig& SteeringConfig,
		const FVehicleSteeringAssistConfig& AssistConfig, float DeltaTime, float SteeringInput);
	static void UpdateSteering(FVehicleAxleSimState& State,
		const FVehicleAxleSteeringConfig& SteeringConfig, float SteeringInput, float TrackWidth);

	/** Standalone TCS calls require State.NumOfWheels > 0 and matching non-null wheel inputs.
	 * PrepareTransmissionStep establishes this before calling TCS on a driven axle. */
	static void UpdateTCS(FVehicleAxleSimState& State, const FVehicleTCSConfig& Config,
		const FVehicleAxleWheelInput* LeftWheel, const FVehicleAxleWheelInput* RightWheel,
		float TargetDriveTorque);

	/** Computes torques before wheel integration. Returns whether wheels are undriven.
 * Null DifferentialConfig retains the legacy missing-differential behavior. */
	static bool PrepareTransmissionStep(FVehicleAxleSimState& State,
		const FVehicleAxleConfig& AxleConfig, const FVehicleTCSConfig& TCSConfig,
		EVehicleAxleLayout AxleLayout, const FVehicleLimitedSlipDifferentialConfig* DifferentialConfig,
		const FVehicleAxleWheelInput* LeftWheel, const FVehicleAxleWheelInput* RightWheel,
		float SubstepDeltaTime, float DriveTorque, float BrakeInput, float HandbrakeInput);

	/** Uses post-integration wheel values to calculate current input-shaft feedback.
 * Pass the same wheel presence/config and undriven flag as PrepareTransmissionStep.
	 * Driven calls require DifferentialConfig and exactly State.NumOfWheels non-null inputs
	 * (one or two), as established by Prepare. Undriven calls do not dereference those inputs.
	 * Returns effective shaft stiffness; the caller reads angular velocity/inertia from State. */
	static float FinalizeTransmissionStep(FVehicleAxleSimState& State,
		const FVehicleAxleConfig& AxleConfig, const FVehicleLimitedSlipDifferentialConfig* DifferentialConfig,
		const FVehicleAxleWheelInput* LeftWheel, const FVehicleAxleWheelInput* RightWheel,
		bool bWheelNotDriven);
};
