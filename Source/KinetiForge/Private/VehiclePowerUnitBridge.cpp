// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehiclePowerUnitBridge.h"
#include "ProfilingDebugging/CpuProfilerTrace.h"
#include "Misc/ScopeLock.h"

FVehiclePowerUnitBridge::FVehiclePowerUnitBridge(FVehiclePowerUnitRuntime InitialRuntime,
	FVehicleDriveInputRuntime InitialDriveInput,
	FVehicleLimitedSlipDifferentialConfig InitialTransferCaseConfig)
	: Runtime(MoveTemp(InitialRuntime)), DriveInput(MoveTemp(InitialDriveInput)),
	  TransferCaseConfig(MoveTemp(InitialTransferCaseConfig))
{
	PublishResult();
}

void FVehiclePowerUnitBridge::SubmitInputSettings(FVehicleInputRuntimeSettings Settings)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_Input_SubmitSettings);
	FScopeLock Lock(&TransferMutex);
	PendingInputSettings.Emplace(MoveTemp(Settings));
}

void FVehiclePowerUnitBridge::RequestInput(const FVehicleInputCommand& Command)
{
	FScopeLock Lock(&TransferMutex);
	PendingInputCommands.Add(Command);
}

void FVehiclePowerUnitBridge::SubmitEngineSettings(FVehicleEngineRuntimeSettings Settings)
{
	FScopeLock Lock(&TransferMutex);
	PendingEngineSettings = MoveTemp(Settings);
	bEngineSettingsPending = true;
}

void FVehiclePowerUnitBridge::SubmitClutchSettings(const FVehicleClutchConfig& Config)
{
	FScopeLock Lock(&TransferMutex);
	PendingClutchSettings = Config;
	bClutchSettingsPending = true;
}

void FVehiclePowerUnitBridge::SubmitGearboxSettings(const FVehicleGearboxConfig& Config,
	const FVehicleGearboxRuntimeState& State)
{
	FScopeLock Lock(&TransferMutex);
	PendingGearboxConfig = Config;
	PendingGearboxState = State;
	bGearboxSettingsPending = true;
}

void FVehiclePowerUnitBridge::SubmitTransferCaseConfig(const FVehicleLimitedSlipDifferentialConfig& Config)
{
	FScopeLock Lock(&TransferMutex);
	PendingTransferCaseConfig = Config;
	bTransferCaseConfigPending = true;
}

void FVehiclePowerUnitBridge::EnqueueCommand(EEngineCommand Type, float MotorTorque)
{
	FScopeLock Lock(&TransferMutex);
	PendingCommands.Add({Type, MotorTorque});
}

void FVehiclePowerUnitBridge::RequestEngineStart() { EnqueueCommand(EEngineCommand::Start); }
void FVehiclePowerUnitBridge::RequestEngineShutdown() { EnqueueCommand(EEngineCommand::Shutdown); }
void FVehiclePowerUnitBridge::RequestP1MotorTorque(float Torque) { EnqueueCommand(EEngineCommand::P1MotorTorque, Torque); }

void FVehiclePowerUnitBridge::ConsumeInputs()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_PowerUnit_ConsumeInputs);
	TOptional<FVehicleEngineRuntimeSettings> EngineSettings;
	FVehicleClutchConfig ClutchSettings;
	FVehicleGearboxConfig GearboxSettings;
	FVehicleGearboxRuntimeState GearboxState;
	FVehicleLimitedSlipDifferentialConfig TransferCaseSettings;
	TArray<FEngineCommand> Commands;
	TOptional<FVehicleInputRuntimeSettings> InputSettings;
	TArray<FVehicleInputCommand> InputCommands;
	bool bApplyEngineSettings, bApplyClutchSettings, bApplyGearboxSettings, bApplyTransferCaseSettings;
	{
		FScopeLock Lock(&TransferMutex);
		bApplyEngineSettings = bEngineSettingsPending;
		bApplyClutchSettings = bClutchSettingsPending;
		bApplyGearboxSettings = bGearboxSettingsPending;
		bApplyTransferCaseSettings = bTransferCaseConfigPending;
		if (bApplyEngineSettings) EngineSettings.Emplace(MoveTemp(PendingEngineSettings));
		if (bApplyClutchSettings) ClutchSettings = PendingClutchSettings;
		if (bApplyGearboxSettings)
		{
			GearboxSettings = PendingGearboxConfig;
			GearboxState = PendingGearboxState;
		}
		if (bApplyTransferCaseSettings) TransferCaseSettings = PendingTransferCaseConfig;
		Commands = MoveTemp(PendingCommands);
		InputSettings = MoveTemp(PendingInputSettings);
		PendingInputSettings.Reset();
		InputCommands = MoveTemp(PendingInputCommands);
		bEngineSettingsPending = bClutchSettingsPending = bGearboxSettingsPending = bTransferCaseConfigPending = false;
	}
	if (InputSettings.IsSet()) DriveInput.Settings = MoveTemp(InputSettings.GetValue());
	for (const FVehicleInputCommand& Command : InputCommands)
	{
		FVehicleInputSolver::ApplyCommand(DriveInput.InputValues, Command);
	}
	// Only the simulation thread touches live state; no lock covers solver work.
	if (bApplyEngineSettings)
	{
		Runtime.EngineParameters = EngineSettings->Parameters;
		Runtime.TurboConfig = EngineSettings->TurboConfig;
		Runtime.ExhaustConfig = EngineSettings->ExhaustConfig;
		Runtime.TorqueLUT = MoveTemp(EngineSettings->TorqueLUT);
	}
	if (bApplyClutchSettings) Runtime.ClutchConfig = ClutchSettings;
	if (bApplyGearboxSettings)
	{
		Runtime.GearboxConfig = GearboxSettings;
		FVehicleGearboxSolver::CalculateGearRatios(Runtime.GearboxConfig, Runtime.ForwardGearRatios, Runtime.ReverseGearRatios);
		Runtime.GearboxState = GearboxState;
	}
	if (bApplyTransferCaseSettings) TransferCaseConfig = TransferCaseSettings;
	// Do not reconstruct commands from published state: that would rewind On/Off transitions.
	for (const FEngineCommand& Command : Commands)
	{
		switch (Command.Type)
		{
		case EEngineCommand::Start: FVehicleEngineSolver::StartVehicleEngine(Runtime.EngineState); break;
		case EEngineCommand::Shutdown: FVehicleEngineSolver::ShutdownVehicleEngine(Runtime.EngineState); break;
		case EEngineCommand::P1MotorTorque: Runtime.EngineState.P1MotorTorque = Command.MotorTorque; break;
		}
	}
}

void FVehiclePowerUnitBridge::PublishResult()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_PowerUnit_PublishResult);
	FScopeLock Lock(&TransferMutex);
	PublishedResult.EngineState = Runtime.EngineState;
	PublishedResult.ClutchState = Runtime.ClutchState;
	PublishedResult.InputValues = DriveInput.InputValues;
	PublishedResult.Telemetry = DriveInput.Telemetry;
	// Coalesce events exactly as the old pending bool flags did, until game Tick consumes.
	bPendingTurboEvent |= Runtime.bShouldTriggerTurboBlowOffCallback;
	bPendingBackfireEvent |= Runtime.bShouldTriggerBackfiringCallback;
	Runtime.bShouldTriggerTurboBlowOffCallback = false;
	Runtime.bShouldTriggerBackfiringCallback = false;
}

void FVehiclePowerUnitBridge::ReadEngineResult(FVehicleEngineSimState& State, bool& bTurboEvent, bool& bBackfireEvent)
{
	FScopeLock Lock(&TransferMutex);
	State = PublishedResult.EngineState;
	bTurboEvent |= bPendingTurboEvent;
	bBackfireEvent |= bPendingBackfireEvent;
	bPendingTurboEvent = bPendingBackfireEvent = false;
}

void FVehiclePowerUnitBridge::ReadClutchResult(FVehicleClutchSimState& State) const
{
	FScopeLock Lock(&TransferMutex);
	State = PublishedResult.ClutchState;
}

FVehiclePowerUnitResult FVehiclePowerUnitBridge::ReadResult() const
{
	FScopeLock Lock(&TransferMutex);
	return PublishedResult;
}
