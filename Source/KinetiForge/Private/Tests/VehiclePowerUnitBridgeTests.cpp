// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehiclePowerUnitBridge.h"
#include "VehicleEngineComponent.h"
#include "VehicleClutchComponent.h"
#include "VehicleGearboxComponent.h"
#include "VehicleDifferentialComponent.h"
#include "VehicleAxleAssemblyComponent.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Async/Async.h"
#include "HAL/Event.h"
#include "HAL/PlatformProcess.h"
#include "Misc/AutomationTest.h"
#include "UObject/StrongObjectPtr.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehiclePowerUnitBridgeCommandTest,
	"KinetiForge.Drivetrain.Bridge.CommandsAndPublication",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehiclePowerUnitBridgeCommandTest::RunTest(const FString& Parameters)
{
	FVehiclePowerUnitRuntime InitialRuntime;
	InitialRuntime.EngineState.OperationMode = EVehicleEngineOperationMode::Starting;
	InitialRuntime.EngineState.EngineAngularVelocity = 23.f;
	InitialRuntime.EngineState.P1MotorTorque = 2.f;
	TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> Bridge =
		MakeShared<FVehiclePowerUnitBridge, ESPMode::ThreadSafe>(MoveTemp(InitialRuntime));

	const FVehiclePowerUnitResult InitialResult = Bridge->ReadResult();
	FVehicleEngineRuntimeSettings NewSettings;
	NewSettings.Parameters.MaxEngineTorque = 725.f;
	Bridge->SubmitEngineSettings(NewSettings);
	Bridge->ConsumeInputs();
	FVehiclePowerUnitRuntime& Runtime = Bridge->GetRuntimeForSimulation();
	TestEqual(TEXT("Applying new settings preserves engine angular velocity"),
		Runtime.EngineState.EngineAngularVelocity, 23.f);
	TestTrue(TEXT("Applying new settings preserves operation mode"),
		Runtime.EngineState.OperationMode == EVehicleEngineOperationMode::Starting);
	TestEqual(TEXT("Engine parameters update independently of state"), Runtime.EngineParameters.MaxEngineTorque, 725.f);

	Bridge->RequestEngineStart();
	Bridge->RequestEngineShutdown();
	Bridge->RequestP1MotorTorque(11.f);
	Bridge->RequestP1MotorTorque(-17.f);
	Bridge->ConsumeInputs();
	TestTrue(TEXT("Ordered start then shutdown preserves legacy Starting behavior"),
		Runtime.EngineState.OperationMode == EVehicleEngineOperationMode::Starting);
	TestEqual(TEXT("Last ordered P1 command wins"), Runtime.EngineState.P1MotorTorque, -17.f);

	Runtime.EngineState.EngineAngularVelocity = 91.f;
	TestEqual(TEXT("Result remains a published snapshot until PublishResult"),
		Bridge->ReadResult().EngineState.EngineAngularVelocity, InitialResult.EngineState.EngineAngularVelocity);
	Runtime.bShouldTriggerTurboBlowOffCallback = true;
	Bridge->PublishResult();
	Runtime.bShouldTriggerTurboBlowOffCallback = true;
	Bridge->PublishResult();
	FVehicleEngineSimState PresentedEngineState;
	bool bTurboEvent = false;
	bool bBackfireEvent = false;
	Bridge->ReadEngineResult(PresentedEngineState, bTurboEvent, bBackfireEvent);
	TestEqual(TEXT("Published engine state advances only at publication"), PresentedEngineState.EngineAngularVelocity, 91.f);
	TestTrue(TEXT("Events coalesce until one consumer read"), bTurboEvent);
	bTurboEvent = false;
	Bridge->ReadEngineResult(PresentedEngineState, bTurboEvent, bBackfireEvent);
	TestFalse(TEXT("Coalesced event is consumed only once"), bTurboEvent);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehiclePowerUnitBridgeComponentTest,
	"KinetiForge.Drivetrain.Bridge.ComponentAdapters",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehiclePowerUnitBridgeComponentTest::RunTest(const FString& Parameters)
{
	TStrongObjectPtr<UVehicleEngineComponent> Engine(NewObject<UVehicleEngineComponent>());
	TStrongObjectPtr<UVehicleClutchComponent> Clutch(NewObject<UVehicleClutchComponent>());
	TStrongObjectPtr<UVehicleGearboxComponent> Gearbox(NewObject<UVehicleGearboxComponent>());
	TStrongObjectPtr<UVehicleEngineComponent> UnboundEngine(NewObject<UVehicleEngineComponent>());
	TStrongObjectPtr<UVehicleClutchComponent> UnboundClutch(NewObject<UVehicleClutchComponent>());

	FVehicleNaturallyAspiratedEngineConfig EngineConfig;
	EngineConfig.EngineTorqueCurve = nullptr;
	EngineConfig.MaxEngineTorque = 0.f;
	EngineConfig.StartFriction = 1.f;
	EngineConfig.FrictionCoefficient = 0.f;
	EngineConfig.PumpingLossCoefficient = 0.f;
	EngineConfig.UnburntFuelAccumulationRate = 0.f;
	Engine->SetNaturallyAspiratedEngineConfig(EngineConfig);
	UnboundEngine->SetNaturallyAspiratedEngineConfig(EngineConfig);
	TestTrue(TEXT("Unbound components remain eligible for legacy operation"),
		Engine->CanBindPowerUnitBridge() && Clutch->CanBindPowerUnitBridge());
	UnboundEngine->StartVehicleEngine();
	UnboundEngine->UpdatePhysics(0.01f, 0.f, 0.f);
	TestTrue(TEXT("Unbound engine retains legacy manual stepping"),
		UnboundEngine->GetEngineState().StarterMotorTorque > 0.f);
	UnboundClutch->UpdatePhysics(0.01f, 0.f, 0.f, 2.f, 1.f, 10000.f, 20.f, 2.f, 400.f);
	TestTrue(TEXT("Unbound clutch retains legacy manual stepping"),
		UnboundClutch->GetClutchState().ClutchTorque > 0.f);

	FVehiclePowerUnitRuntime InitialRuntime;
	InitialRuntime.EngineParameters = FVehicleEngineParameters::FromConfig(EngineConfig);
	InitialRuntime.EngineState = Engine->GetEngineState();
	InitialRuntime.ClutchConfig = Clutch->GetConfig();
	TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> Bridge =
		MakeShared<FVehiclePowerUnitBridge, ESPMode::ThreadSafe>(MoveTemp(InitialRuntime));
	TestTrue(TEXT("Engine binds once"), Engine->BindPowerUnitBridge(Bridge));
	TestFalse(TEXT("Engine refuses a second runtime binding"), Engine->CanBindPowerUnitBridge());
	TestFalse(TEXT("Engine rejects another bridge"), Engine->BindPowerUnitBridge(
		MakeShared<FVehiclePowerUnitBridge, ESPMode::ThreadSafe>(FVehiclePowerUnitRuntime{})));
	TestTrue(TEXT("Clutch binds once"), Clutch->BindPowerUnitBridge(Bridge));
	TestTrue(TEXT("Game-thread gearbox authority binds once"), Gearbox->BindPowerUnitBridge(Bridge));
	TestFalse(TEXT("Gearbox refuses a second runtime binding"), Gearbox->CanBindPowerUnitBridge());
	Gearbox->ShiftToTargetGear(1, true);
	Gearbox->SetP2MotorTorque(32.f);
	TestFalse(TEXT("Clutch refuses a second runtime binding"), Clutch->CanBindPowerUnitBridge());
	TestFalse(TEXT("Clutch rejects another bridge"), Clutch->BindPowerUnitBridge(
		MakeShared<FVehiclePowerUnitBridge, ESPMode::ThreadSafe>(FVehiclePowerUnitRuntime{})));

	Engine->StartVehicleEngine();
	Engine->ShutVehicleEngine();
	Engine->SetP1MotorTorque(-5.f);
	Engine->RefreshPublishedRuntimeState();
	TestEqual(TEXT("An older published result does not undo the current GT P1 command"),
		Engine->GetP1MotorTorque(), -5.f);
	Bridge->ConsumeInputs();
	TestTrue(TEXT("Bound component routes ordered lifecycle commands to runtime"),
		Bridge->GetRuntimeForSimulation().EngineState.OperationMode == EVehicleEngineOperationMode::Starting);
	TestEqual(TEXT("Bound component routes P1 command to runtime"),
		Bridge->GetRuntimeForSimulation().EngineState.P1MotorTorque, -5.f);
	TestEqual(TEXT("Game-thread immediate shift publishes its completed gear"),
		Bridge->GetRuntimeForSimulation().GearboxState.CurrentGear, 1);
	TestEqual(TEXT("Game-thread gear snapshot preserves authoritative ratio"),
		Bridge->GetRuntimeForSimulation().GearboxState.CurrentGearRatio, Gearbox->GetCurrentGearRatio());
	TestEqual(TEXT("Game-thread gearbox routes P2 torque through the mailbox"),
		Bridge->GetRuntimeForSimulation().GearboxState.P2MotorTorque, 32.f);

	FVehicleGearboxConfig ReconfiguredGearbox = Gearbox->GetConfig();
	ReconfiguredGearbox.FirstGear = 4.2f;
	ReconfiguredGearbox.TopGear = 0.7f;
	Gearbox->SetConfig(ReconfiguredGearbox);
	TestTrue(TEXT("Engaged gearbox config edit immediately recalculates its selected ratio"),
		FMath::IsNearlyEqual(Gearbox->GetCurrentGearRatio(), 4.2f));
	Bridge->ConsumeInputs();
	TestTrue(TEXT("Gearbox config edit reaches bound power-unit snapshot without shifting"),
		FMath::IsNearlyEqual(Bridge->GetRuntimeForSimulation().GearboxState.CurrentGearRatio,
			Gearbox->GetCurrentGearRatio()));
	Gearbox->ShiftToTargetGear(0, true);
	TestTrue(TEXT("Neutral remains zero after gearbox reconfiguration"),
		FMath::IsNearlyZero(Gearbox->GetCurrentGearRatio()));
	Gearbox->ShiftToTargetGear(-1, true);
	TestTrue(TEXT("Reverse uses the recalculated reverse gear ratio"),
		FMath::IsNearlyEqual(Gearbox->GetCurrentGearRatio(), Gearbox->GetGearRatio(-1)));

	const float EngineOmegaBefore = Engine->GetAngularVelocity();
	Engine->UpdatePhysics(0.01f, 1.f, 0.f);
	TestEqual(TEXT("Bound engine manual update does not double-step display state"),
		Engine->GetAngularVelocity(), EngineOmegaBefore);
	const float ClutchTorqueBefore = Clutch->GetClutchState().ClutchTorque;
	Clutch->UpdatePhysics(0.01f, 0.f, 0.f, 2.f, 1.f, 10000.f, 100.f, 2.f, 400.f);
	TestEqual(TEXT("Bound clutch manual update does not double-step display state"),
		Clutch->GetClutchState().ClutchTorque, ClutchTorqueBefore);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehiclePowerUnitBridgeConcurrentTest,
	"KinetiForge.Drivetrain.Bridge.ConcurrentMailbox",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehiclePowerUnitBridgeConcurrentTest::RunTest(const FString& Parameters)
{
	TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> Bridge =
		MakeShared<FVehiclePowerUnitBridge, ESPMode::ThreadSafe>(FVehiclePowerUnitRuntime{});
	FEvent* SimStepReady = FPlatformProcess::GetSynchEventFromPool(false);
	FEvent* SimStepComplete = FPlatformProcess::GetSynchEventFromPool(false);
	TFuture<void> Consumer = Async(EAsyncExecution::ThreadPool, [Bridge, SimStepReady, SimStepComplete]()
	{
		for (int32 Step = 0; Step < 100; ++Step)
		{
			if (!SimStepReady->Wait(10000)) return;
			Bridge->ConsumeInputs();
			FVehiclePowerUnitRuntime& Runtime = Bridge->GetRuntimeForSimulation();
			Runtime.EngineState.EngineAngularVelocity += 1.f;
			FVehicleDriveInputRuntime& DriveInput = Bridge->GetDriveInputForSimulation();
			DriveInput.InputValues.Smoothened.Steering += 0.001f;
			DriveInput.Telemetry.LocalLinearVelocity.X += 1.f;
			Bridge->PublishResult();
			SimStepComplete->Trigger();
		}
	});

	for (int32 Step = 0; Step < 100; ++Step)
	{
		Bridge->RequestP1MotorTorque(static_cast<float>(Step));
		FVehicleInputRuntimeSettings InputSettings;
		InputSettings.Steering.InterpSpeed.X = static_cast<float>(Step);
		Bridge->SubmitInputSettings(MoveTemp(InputSettings));
		Bridge->RequestInput({EVehicleInputAxis::Steering, Step / 100.f, false, false});
		(void)Bridge->ReadResult();
		SimStepReady->Trigger();
		// Exercise reads after waking the worker, while it may be publishing.
		for (int32 ReadIndex = 0; ReadIndex < 32; ++ReadIndex) (void)Bridge->ReadResult();
		if (!SimStepComplete->Wait(10000))
		{
			AddError(FString::Printf(TEXT("Simulation consumer did not finish step %d"), Step));
			break;
		}
	}
	Consumer.Wait();
	FPlatformProcess::ReturnSynchEventToPool(SimStepReady);
	FPlatformProcess::ReturnSynchEventToPool(SimStepComplete);
	const FVehiclePowerUnitResult Result = Bridge->ReadResult();
	TestEqual(TEXT("Concurrent mailbox preserves the last ordered P1 command"),
		Result.EngineState.P1MotorTorque, 99.f);
	TestTrue(TEXT("Simulation consumer publishes while GT reads results"),
		FMath::IsNearlyEqual(Result.EngineState.EngineAngularVelocity, 100.f));
	TestEqual(TEXT("Concurrent input commands preserve latest raw input"), Result.InputValues.Raw.Steering, 0.99f);
	TestTrue(TEXT("Settings never rewind simulation smoothing history"),
		FMath::IsNearlyEqual(Result.InputValues.Smoothened.Steering, 0.1f, 1.e-6f));
	TestEqual(TEXT("Telemetry has a simulation owner and a published copy"), Result.Telemetry.LocalLinearVelocity.X, 100.f);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleTransferCaseSnapshotTest,
	"KinetiForge.Drivetrain.Bridge.TransferCaseSnapshot",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleTransferCaseSnapshotTest::RunTest(const FString& Parameters)
{
	FVehicleLimitedSlipDifferentialConfig InitialConfig;
	InitialConfig.GearRatio = 3.f;
	InitialConfig.DriveLockRatio = 0.5f;
	InitialConfig.CoastLockRatio = 0.25f;
	FVehicleLimitedSlipDifferentialConfig SubmittedConfig = InitialConfig;
	SubmittedConfig.GearRatio = 4.f;
	FVehiclePowerUnitRuntime InitialRuntime;
	InitialRuntime.EngineState.EngineAngularVelocity = 41.f;
	InitialRuntime.ClutchState.AngleDiff = 0.125f;
	TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> Bridge =
		MakeShared<FVehiclePowerUnitBridge, ESPMode::ThreadSafe>(MoveTemp(InitialRuntime),
			FVehicleDriveInputRuntime(), InitialConfig);

	Bridge->SubmitTransferCaseConfig(SubmittedConfig);
	TestEqual(TEXT("Transfer-case submission waits for the next macrostep"),
		Bridge->GetTransferCaseConfigForSimulation().GearRatio, 3.f);
	Bridge->ConsumeInputs();
	TestEqual(TEXT("Macrostep consumption advances the independent config snapshot"),
		Bridge->GetTransferCaseConfigForSimulation().GearRatio, 4.f);
	TestEqual(TEXT("Differential config update does not reset engine state"),
		Bridge->GetRuntimeForSimulation().EngineState.EngineAngularVelocity, 41.f);
	TestEqual(TEXT("Differential config update does not reset clutch history"),
		Bridge->GetRuntimeForSimulation().ClutchState.AngleDiff, 0.125f);

	TStrongObjectPtr<UVehicleDifferentialComponent> Differential(NewObject<UVehicleDifferentialComponent>());
	TStrongObjectPtr<UVehicleAxleAssemblyComponent> Axle(NewObject<UVehicleAxleAssemblyComponent>());
	FVehicleAxleConfig AxleConfig = Axle->GetAxleConfig();
	AxleConfig.TorqueWeight = 1.f;
	Axle->SetAxleConfig(AxleConfig);
	FVehicleAxleSimState& AxleState = Axle->GetSimulationState();
	AxleState.AxleEffectiveInertia = 2.f;
	AxleState.AxleAngularVelocity = 10.f;
	UVehicleAxleAssemblyComponent* AxleArray[] = {Axle.Get()};
	float LegacyOmega = 0.f, LegacyInertia = 0.f, LegacyStiffness = 0.f;
	AxleState = FVehicleAxleSimState();
	AxleState.AxleEffectiveInertia = 2.f;
	AxleState.AxleAngularVelocity = 10.f;
	const int32 LegacyDriveAxles = Differential->SubstepTransferCase(AxleArray, 0.01f, 100.f,
		0.f, 0.f, false, LegacyOmega, LegacyInertia, LegacyStiffness);
	const FVehicleLimitedSlipDifferentialConfig ComponentConfig = Differential->GetConfig();
	FVehicleLimitedSlipDifferentialConfig AlternateConfig = ComponentConfig;
	AlternateConfig.GearRatio = 4.f;
	Differential->SetConfig(AlternateConfig); // Simulate a GT edit after the owned snapshot was captured.
	float StaticOmega = 0.f, StaticInertia = 0.f, StaticStiffness = 0.f;
	AxleState = FVehicleAxleSimState();
	AxleState.AxleEffectiveInertia = 2.f;
	AxleState.AxleAngularVelocity = 10.f;
	const int32 StaticDriveAxles = UVehicleDifferentialComponent::SubstepTransferCaseWithConfig(
		ComponentConfig, AxleArray, 0.01f, 100.f, 0.f, 0.f, false,
		StaticOmega, StaticInertia, StaticStiffness);
	TestEqual(TEXT("Legacy component wrapper and static snapshot visit the same drive axles"),
		StaticDriveAxles, LegacyDriveAxles);
	TestTrue(TEXT("Static snapshot wrapper preserves legacy shaft outputs"),
		FMath::IsNearlyEqual(StaticOmega, LegacyOmega)
		&& FMath::IsNearlyEqual(StaticInertia, LegacyInertia)
		&& FMath::IsNearlyEqual(StaticStiffness, LegacyStiffness));

	AxleState = FVehicleAxleSimState();
	AxleState.AxleEffectiveInertia = 2.f;
	AxleState.AxleAngularVelocity = 10.f;
	float AlternateOmega = 0.f, AlternateInertia = 0.f, AlternateStiffness = 0.f;
	UVehicleDifferentialComponent::SubstepTransferCaseWithConfig(AlternateConfig, AxleArray, 0.01f,
		100.f, 0.f, 0.f, false, AlternateOmega, AlternateInertia, AlternateStiffness);
	TestTrue(TEXT("A distinct owned gear ratio produces distinct nonzero shaft output"),
		FMath::Abs(StaticOmega) > SMALL_NUMBER && !FMath::IsNearlyEqual(StaticOmega, AlternateOmega));

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
