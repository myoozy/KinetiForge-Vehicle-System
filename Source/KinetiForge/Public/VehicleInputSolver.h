// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "Curves/RichCurve.h"
#include "VehicleInputStructs.h"
#include "VehicleGearboxSolver.h"

/** Value copy of a response curve. Disabled curves evaluate as identity. */
struct KINETIFORGE_API FVehicleInputCurve
{
	bool bEnabled = false;
	FRichCurve Curve;

	float Evaluate(float Value) const { return bEnabled ? Curve.Eval(Value) : Value; }
};

struct KINETIFORGE_API FVehicleInputAxisRuntimeConfig
{
	FVector2f InterpSpeed = FVector2f(5.f, 5.f);
	FVehicleInputCurve ResponseCurve;
};

/** UObject-free input configuration copied by the owning game-thread adapter. */
struct KINETIFORGE_API FVehicleInputRuntimeSettings
{
	FVehicleInputAxisRuntimeConfig Throttle;
	FVehicleInputAxisRuntimeConfig Brake;
	FVehicleInputAxisRuntimeConfig Clutch;
	FVehicleInputAxisRuntimeConfig Steering{ FVector2f(2.5f, 2.5f) };
	FVehicleInputAxisRuntimeConfig Handbrake{ FVector2f(15.f, 15.f) };
	FVehicleInputCurve HighSpeedSteeringScale;
	FVehicleInputAssistConfig Assist;
	bool bAutomaticGearbox = true;
	bool bArcadeAutoGearbox = true;
	float SubstepDeltaTime = 1.f / 240.f;
};

struct KINETIFORGE_API FVehicleInputStepContext
{
	bool bHasGearbox = false;
	bool bHasEngine = false;
	FVehicleGearboxConfig GearboxConfig;
	FVehicleGearboxRuntimeState GearboxState;
	float EngineRPM = 0.f;
	float LocalLongitudinalVelocity = 0.f;
	bool bIsInAir = false;
};

enum class EVehicleInputAxis : uint8
{
	Throttle,
	Brake,
	Clutch,
	Steering,
	Handbrake
};

/** A single external input write; direct input also mirrors into its smoothed axis. */
struct KINETIFORGE_API FVehicleInputCommand
{
	EVehicleInputAxis Axis = EVehicleInputAxis::Throttle;
	float Input = 0.f;
	bool bDirectInput = false;
	bool bSwitchThrottleAndBrake = false;
};

/** Reproduces the DriveAssembly input-axis update sequence over caller-owned values. */
class KINETIFORGE_API FVehicleInputSolver
{
public:
	static void Update(const FVehicleInputRuntimeSettings& Settings, FVehicleInputPipeline& Pipeline,
		const FVehicleInputStepContext& Context, float DeltaTime);
	/** Updates only the requested axis; preserves the pipeline's current throttle/brake mapping. */
	static void UpdateAxis(const FVehicleInputRuntimeSettings& Settings, FVehicleInputPipeline& Pipeline,
		const FVehicleInputStepContext& Context, EVehicleInputAxis Axis, float DeltaTime);
	static void ApplyCommand(FVehicleInputPipeline& Pipeline, const FVehicleInputCommand& Command);
};
