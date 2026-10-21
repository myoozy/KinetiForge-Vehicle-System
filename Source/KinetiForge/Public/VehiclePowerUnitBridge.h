// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "HAL/CriticalSection.h"
#include "VehiclePowerUnitRuntime.h"
#include "VehicleInputSolver.h"
#include "VehicleDrivetrainStructs.h"

/** Persistent macrostep feedback and published vehicle telemetry. Velocities use m/s. */
struct KINETIFORGE_API FVehicleDriveTelemetry
{
	float PhysicsDeltaTime = 0.f;
	int32 NumOfWheelsOnGround = 0;
	int32 NumOfDriveAxles = 0;
	bool bIsInAir = false;
	FVector3f AbsoluteWorldLinearVelocity = FVector3f(0.f);
	FVector3f LocalLinearVelocity = FVector3f(0.f); // Relative to ground.
	FVector3f WorldLinearVelocity = FVector3f(0.f); // Relative to ground.
	FVector3f LocalVelocityClamped = FVector3f(0.f);
	FVector3f WorldAcceleration = FVector3f(0.f);
	FVector3f LocalAcceleration = FVector3f(0.f);
};

/** One simulation owner; settings are replaced only at macrostep boundaries. */
struct KINETIFORGE_API FVehicleDriveInputRuntime
{
	FVehicleInputRuntimeSettings Settings;
	FVehicleInputPipeline InputValues;
	FVehicleDriveTelemetry Telemetry;
};

/** Game-thread preparation strips curve UObject references from engine settings. */
struct KINETIFORGE_API FVehicleEngineRuntimeSettings
{
	FVehicleEngineParameters Parameters;
	FVehicleEngineTurboConfig TurboConfig;
	FVehicleEngineExhaustConfig ExhaustConfig;
	FVehicleLUT<32> TorqueLUT = FVehicleLUT<32>(1.f);
};

/** Published values, never references into the integrating runtime. */
struct KINETIFORGE_API FVehiclePowerUnitResult
{
	FVehicleEngineSimState EngineState;
	FVehicleClutchSimState ClutchState;
	FVehicleInputPipeline InputValues;
	FVehicleDriveTelemetry Telemetry;
};

/**
 * Narrow mailbox for the power unit. One simulation consumer owns Runtime;
 * game-thread producers submit settings/commands and read published copies.
 * Locks cover data transfer only, never equations or downstream wheel updates.
 * This does not make the containing UObject callback or its inputs thread-safe.
 */
class KINETIFORGE_API FVehiclePowerUnitBridge
{
public:
	explicit FVehiclePowerUnitBridge(FVehiclePowerUnitRuntime InitialRuntime,
		FVehicleDriveInputRuntime InitialDriveInput = FVehicleDriveInputRuntime(),
		FVehicleLimitedSlipDifferentialConfig InitialTransferCaseConfig = FVehicleLimitedSlipDifferentialConfig());
	void SubmitInputSettings(FVehicleInputRuntimeSettings Settings);
	void RequestInput(const FVehicleInputCommand& Command);
	void SubmitEngineSettings(FVehicleEngineRuntimeSettings Settings);
	void SubmitClutchSettings(const FVehicleClutchConfig& Config);
	/** Legacy World timer remains authoritative for gear selection in this stage. */
	void SubmitGearboxSettings(const FVehicleGearboxConfig& Config, const FVehicleGearboxRuntimeState& State);
	/** Submit the transfer-case setup for the next macrostep. */
	void SubmitTransferCaseConfig(const FVehicleLimitedSlipDifferentialConfig& Config);
	void RequestEngineStart();
	void RequestEngineShutdown();
	void RequestP1MotorTorque(float Torque);
	void ReadEngineResult(FVehicleEngineSimState& State, bool& bTurboEvent, bool& bBackfireEvent);
	void ReadClutchResult(FVehicleClutchSimState& State) const;
	FVehiclePowerUnitResult ReadResult() const;

	/** Call once at the macrostep boundary; only the simulation consumer may call. */
	void ConsumeInputs();
	FVehiclePowerUnitRuntime& GetRuntimeForSimulation() { return Runtime; }
	FVehicleDriveInputRuntime& GetDriveInputForSimulation() { return DriveInput; }
	/** Simulation-consumer snapshot; valid only on the owning simulation thread. */
	const FVehicleLimitedSlipDifferentialConfig& GetTransferCaseConfigForSimulation() const { return TransferCaseConfig; }
	void PublishResult();

private:
	enum class EEngineCommand : uint8 { Start, Shutdown, P1MotorTorque };
	struct FEngineCommand
	{
		EEngineCommand Type;
		float MotorTorque = 0.f;
	};
	void EnqueueCommand(EEngineCommand Type, float MotorTorque = 0.f);
	FVehiclePowerUnitRuntime Runtime;
	FVehicleDriveInputRuntime DriveInput;
	mutable FCriticalSection TransferMutex;
	FVehicleEngineRuntimeSettings PendingEngineSettings;
	FVehicleClutchConfig PendingClutchSettings;
	FVehicleGearboxConfig PendingGearboxConfig;
	FVehicleGearboxRuntimeState PendingGearboxState;
	FVehicleLimitedSlipDifferentialConfig TransferCaseConfig;
	FVehicleLimitedSlipDifferentialConfig PendingTransferCaseConfig;
	TArray<FEngineCommand> PendingCommands;
	TOptional<FVehicleInputRuntimeSettings> PendingInputSettings;
	TArray<FVehicleInputCommand> PendingInputCommands;
	bool bEngineSettingsPending = false;
	bool bClutchSettingsPending = false;
	bool bGearboxSettingsPending = false;
	bool bTransferCaseConfigPending = false;
	FVehiclePowerUnitResult PublishedResult;
	bool bPendingTurboEvent = false;
	bool bPendingBackfireEvent = false;
};
