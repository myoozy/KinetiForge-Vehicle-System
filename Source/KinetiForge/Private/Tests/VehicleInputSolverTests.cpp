// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehicleDriveAssemblyComponent.h"
#include "VehicleEngineComponent.h"
#include "VehicleGearboxComponent.h"
#include "VehicleInputSolver.h"
#include "VehiclePowerUnitBridge.h"
#include "VehicleFrozenBaseline.h"
#include "Curves/CurveFloat.h"
#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS
struct FVehicleDriveInputRegressionProbe
{
	static void Bind(UVehicleDriveAssemblyComponent& Drive, UVehicleEngineComponent* Engine,
		UVehicleGearboxComponent* Gearbox)
	{
		Drive.Engine = Engine;
		Drive.Gearbox = Gearbox;
	}

	static void SetPipeline(UVehicleDriveAssemblyComponent& Drive, const FVehicleInputPipeline& Pipeline)
	{
		Drive.InputValues = Pipeline;
	}

	static FVehicleInputPipeline GetPipeline(const UVehicleDriveAssemblyComponent& Drive)
	{
		return Drive.InputValues;
	}

	static void Update(UVehicleDriveAssemblyComponent& Drive, float DeltaTime, float Velocity, bool bInAir, FAutomationTestBase& Test, const FVehicleFrozenBaseline& Baseline, int32& ReferenceRow)
	{
		Drive.LocalLinearVelocity = FVector3f(Velocity, 0.f, 0.f);
		Drive.bIsInAir = bInAir;
		Drive.UpdateInput(DeltaTime);
		const auto& Output = Drive.InputValues;
		Baseline.CheckRow(Test, ReferenceRow++, {
			Output.Smoothened.Throttle, Output.Smoothened.Brake, Output.Smoothened.Clutch,
			Output.Smoothened.Steering, Output.Smoothened.Handbrake,
			Output.Final.Throttle, Output.Final.Brake, Output.Final.Clutch,
			Output.Final.Steering, Output.Final.Handbrake, double(Output.bSwitchThrottleAndBrake)}, {1.e-5,1.e-5,1.e-5,1.e-5,1.e-5,1.e-5,1.e-5,1.e-5,1.e-5,1.e-5,0});
	}

	static void UpdateAdapter(UVehicleDriveAssemblyComponent& Drive, float DeltaTime)
	{
		Drive.UpdateInput(DeltaTime);
	}

	static void UpdateAdapterAxes(UVehicleDriveAssemblyComponent& Drive, float DeltaTime)
	{
		Drive.UpdateThrottle(DeltaTime);
		Drive.UpdateBrake(DeltaTime);
		Drive.UpdateClutch(DeltaTime);
		Drive.UpdateSteering(DeltaTime);
		Drive.UpdateHandbrake(DeltaTime);
	}

	static FVehicleInputRuntimeSettings Capture(const UVehicleDriveAssemblyComponent& Drive)
	{
		return Drive.CaptureInputRuntimeSettings();
	}

	static void BindBridge(UVehicleDriveAssemblyComponent& Drive,
		TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> Bridge)
	{
		Drive.PowerUnitBridge = MoveTemp(Bridge);
		Drive.bPowerUnitBridgeRequested = true;
	}

	static void ApplyCommand(UVehicleDriveAssemblyComponent& Drive, EVehicleInputAxis Axis, float Input, bool bDirect)
	{
		Drive.ApplyInputCommand(Axis, Input, bDirect);
	}

	static void RefreshBridge(UVehicleDriveAssemblyComponent& Drive)
	{
		Drive.RefreshPowerUnitBridge();
	}
};

namespace
{
	bool NearlyEqual(float A, float B, float Tolerance = 1.e-5f)
	{
		return FMath::IsNearlyEqual(A, B, Tolerance);
	}

	void TestPipelineEqual(FAutomationTestBase& Test, const FVehicleInputPipeline& Actual,
		const FVehicleInputPipeline& Expected, const TCHAR* Context)
	{
		Test.TestTrue(FString::Printf(TEXT("%s throttle smooth"), Context), NearlyEqual(Actual.Smoothened.Throttle, Expected.Smoothened.Throttle));
		Test.TestTrue(FString::Printf(TEXT("%s brake smooth"), Context), NearlyEqual(Actual.Smoothened.Brake, Expected.Smoothened.Brake));
		Test.TestTrue(FString::Printf(TEXT("%s clutch smooth"), Context), NearlyEqual(Actual.Smoothened.Clutch, Expected.Smoothened.Clutch));
		Test.TestTrue(FString::Printf(TEXT("%s steering smooth"), Context), NearlyEqual(Actual.Smoothened.Steering, Expected.Smoothened.Steering));
		Test.TestTrue(FString::Printf(TEXT("%s handbrake smooth"), Context), NearlyEqual(Actual.Smoothened.Handbrake, Expected.Smoothened.Handbrake));
		Test.TestTrue(FString::Printf(TEXT("%s throttle final"), Context), NearlyEqual(Actual.Final.Throttle, Expected.Final.Throttle));
		Test.TestTrue(FString::Printf(TEXT("%s brake final"), Context), NearlyEqual(Actual.Final.Brake, Expected.Final.Brake));
		Test.TestTrue(FString::Printf(TEXT("%s clutch final"), Context), NearlyEqual(Actual.Final.Clutch, Expected.Final.Clutch));
		Test.TestTrue(FString::Printf(TEXT("%s steering final"), Context), NearlyEqual(Actual.Final.Steering, Expected.Final.Steering));
		Test.TestTrue(FString::Printf(TEXT("%s handbrake final"), Context), NearlyEqual(Actual.Final.Handbrake, Expected.Final.Handbrake));
		Test.TestEqual(FString::Printf(TEXT("%s switch state"), Context), Actual.bSwitchThrottleAndBrake, Expected.bSwitchThrottleAndBrake);
	}

	FVehicleInputStepContext MakeContext(UVehicleGearboxComponent* Gearbox, UVehicleEngineComponent* Engine,
		float Velocity, bool bInAir)
	{
		FVehicleInputStepContext Context;
		Context.bHasGearbox = Gearbox != nullptr;
		Context.bHasEngine = Engine != nullptr;
		if (Gearbox)
		{
			Context.GearboxConfig = Gearbox->GetConfig();
			Context.GearboxState = Gearbox->CaptureRuntimeSettingsState();
		}
		if (Engine) Context.EngineRPM = Engine->GetRPM();
		Context.LocalLongitudinalVelocity = Velocity;
		Context.bIsInAir = bInAir;
		return Context;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleInputSolverFrozenCompatibilityTest,
	"KinetiForge.Drivetrain.Input.FrozenCompatibility",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleInputSolverFrozenCompatibilityTest::RunTest(const FString& Parameters)
{
	FVehicleFrozenBaseline Baseline;
	if (!Baseline.Load(*this, TEXT("input-compatibility.csv"), 18, 11)) return false;
	int32 ReferenceRow = 0;
	TStrongObjectPtr<UVehicleDriveAssemblyComponent> Drive(NewObject<UVehicleDriveAssemblyComponent>());
	TStrongObjectPtr<UVehicleGearboxComponent> Gearbox(NewObject<UVehicleGearboxComponent>());
	TStrongObjectPtr<UVehicleEngineComponent> Engine(NewObject<UVehicleEngineComponent>());
	FVehicleGearboxConfig GearboxConfig;
	GearboxConfig.NumberOfGears = 4;
	GearboxConfig.NumOfReverseGears = 1;
	Gearbox->SetConfig(GearboxConfig);
	FVehicleDriveInputRegressionProbe::Bind(*Drive, Engine.Get(), Gearbox.Get());

	// Curves cover constant, linear and cubic interpolation through the same copied FRichCurve path.
	TStrongObjectPtr<UCurveFloat> ThrottleCurve(NewObject<UCurveFloat>());
	ThrottleCurve->FloatCurve.AddKey(0.f, 0.f);
	ThrottleCurve->FloatCurve.AddKey(1.f, 1.f);
	TStrongObjectPtr<UCurveFloat> BrakeCurve(NewObject<UCurveFloat>());
	const FKeyHandle BrakeConstantKey = BrakeCurve->FloatCurve.AddKey(0.f, 0.2f);
	BrakeCurve->FloatCurve.AddKey(1.f, 0.8f);
	BrakeCurve->FloatCurve.SetKeyInterpMode(BrakeConstantKey, ERichCurveInterpMode::RCIM_Constant);
	TStrongObjectPtr<UCurveFloat> SteeringCurve(NewObject<UCurveFloat>());
	const FKeyHandle SteeringCubicKey = SteeringCurve->FloatCurve.AddKey(0.f, 0.f);
	SteeringCurve->FloatCurve.AddKey(1.f, 1.f);
	SteeringCurve->FloatCurve.SetKeyInterpMode(SteeringCubicKey, ERichCurveInterpMode::RCIM_Cubic);
	Drive->InputConfig.Throttle.ResponseCurve = ThrottleCurve.Get();
	Drive->InputConfig.Brake.ResponseCurve = BrakeCurve.Get();
	Drive->InputConfig.Steering.ResponseCurve = SteeringCurve.Get();
	Drive->InputConfig.HighSpeedSteeringScale = NewObject<UCurveFloat>();
	const FKeyHandle SteeringScaleLinearKey = Drive->InputConfig.HighSpeedSteeringScale->FloatCurve.AddKey(0.f, 1.f);
	Drive->InputConfig.HighSpeedSteeringScale->FloatCurve.AddKey(20.f, 0.5f);
	Drive->InputConfig.HighSpeedSteeringScale->FloatCurve.SetKeyInterpMode(SteeringScaleLinearKey, ERichCurveInterpMode::RCIM_Linear);
	Drive->InputAssistConfig.bAutomaticClutch = false;
	Drive->AutoGearboxConfig.bAutomaticGearbox = true;
	Drive->AutoGearboxConfig.bArcadeAutoGearbox = true;

	FVehicleInputRuntimeSettings Settings = FVehicleDriveInputRegressionProbe::Capture(*Drive);
	FVehicleInputPipeline SolverPipeline;
	SolverPipeline.Raw.Throttle = 0.75f;
	SolverPipeline.Raw.Brake = 0.25f;
	SolverPipeline.Raw.Clutch = 0.4f;
	SolverPipeline.Raw.Steering = 0.1f;
	SolverPipeline.Raw.Handbrake = 0.8f;
	SolverPipeline.Smoothened.Throttle = 0.1f;
	SolverPipeline.Smoothened.Brake = 0.2f;
	SolverPipeline.Smoothened.Clutch = 0.3f;
	SolverPipeline.Smoothened.Steering = -0.2f;
	SolverPipeline.Smoothened.Handbrake = 0.1f;
	FVehicleDriveInputRegressionProbe::SetPipeline(*Drive, SolverPipeline);

	const float Velocities[] = { 12.f, -4.f, 0.f, 1.f };
	const int32 Gears[] = { 1, -1, 0, 2 };
	for (int32 Scenario = 0; Scenario < UE_ARRAY_COUNT(Gears); ++Scenario)
	{
		Gearbox->ShiftToTargetGear(Gears[Scenario], true);
		for (int32 Substep = 0; Substep < 4; ++Substep)
		{
			const FVehicleInputPipeline BeforeStep = SolverPipeline;
			FVehicleDriveInputRegressionProbe::SetPipeline(*Drive, SolverPipeline);
			FVehicleDriveInputRegressionProbe::Update(*Drive, 0.016f, Velocities[Scenario], Scenario == 3, *this, Baseline, ReferenceRow);
			const FVehicleInputPipeline AdapterResult = FVehicleDriveInputRegressionProbe::GetPipeline(*Drive);
			const FVehicleInputStepContext Context = MakeContext(Gearbox.Get(), Engine.Get(), Velocities[Scenario], Scenario == 3);
			FVehicleInputSolver::Update(Settings, SolverPipeline, Context, 0.016f);
			TestPipelineEqual(*this, SolverPipeline, AdapterResult,
				*FString::Printf(TEXT("gear %d substep %d"), Gears[Scenario], Substep));
			FVehicleDriveInputRegressionProbe::SetPipeline(*Drive, BeforeStep);
			FVehicleDriveInputRegressionProbe::UpdateAdapter(*Drive, 0.016f);
			TestPipelineEqual(*this, FVehicleDriveInputRegressionProbe::GetPipeline(*Drive), AdapterResult,
				TEXT("GT component adapter retains historical input behavior"));
			// Individual protected helpers retain their old mapping semantics: callers select it first.
			FVehicleInputPipeline AxisInput = BeforeStep;
			AxisInput.bSwitchThrottleAndBrake = AdapterResult.bSwitchThrottleAndBrake;
			FVehicleDriveInputRegressionProbe::SetPipeline(*Drive, AxisInput);
			FVehicleDriveInputRegressionProbe::UpdateAdapterAxes(*Drive, 0.016f);
			TestPipelineEqual(*this, FVehicleDriveInputRegressionProbe::GetPipeline(*Drive), AdapterResult,
				TEXT("GT per-axis adapters retain historical input behavior"));
		}
	}

	// The previous switch value is retained when a gearbox is present but arcade auto is disabled.
	Drive->AutoGearboxConfig.bAutomaticGearbox = false;
	Gearbox->ShiftToTargetGear(-1, true);
	SolverPipeline.bSwitchThrottleAndBrake = true;
	FVehicleDriveInputRegressionProbe::SetPipeline(*Drive, SolverPipeline);
	FVehicleDriveInputRegressionProbe::Update(*Drive, 0.016f, -1.f, false, *this, Baseline, ReferenceRow);
	const FVehicleInputPipeline AdapterStaleSwitch = FVehicleDriveInputRegressionProbe::GetPipeline(*Drive);
	Settings = FVehicleDriveInputRegressionProbe::Capture(*Drive);
	FVehicleInputSolver::Update(Settings, SolverPipeline, MakeContext(Gearbox.Get(), Engine.Get(), -1.f, false), 0.016f);
	TestPipelineEqual(*this, SolverPipeline, AdapterStaleSwitch, TEXT("auto disabled retains switch"));

	// Without either component, the legacy path clears the stale switch before updating the five axes.
	TStrongObjectPtr<UVehicleDriveAssemblyComponent> StandaloneDrive(NewObject<UVehicleDriveAssemblyComponent>());
	FVehicleInputPipeline StandalonePipeline;
	StandalonePipeline.bSwitchThrottleAndBrake = true;
	StandalonePipeline.Raw.Throttle = 0.5f;
	StandalonePipeline.Raw.Brake = 0.2f;
	FVehicleDriveInputRegressionProbe::SetPipeline(*StandaloneDrive, StandalonePipeline);
	const FVehicleInputRuntimeSettings StandaloneSettings = FVehicleDriveInputRegressionProbe::Capture(*StandaloneDrive);
	FVehicleDriveInputRegressionProbe::Update(*StandaloneDrive, 0.02f, 0.f, false, *this, Baseline, ReferenceRow);
	const FVehicleInputPipeline AdapterStandalone = FVehicleDriveInputRegressionProbe::GetPipeline(*StandaloneDrive);
	FVehicleInputSolver::Update(StandaloneSettings, StandalonePipeline, FVehicleInputStepContext(), 0.02f);
	TestPipelineEqual(*this, StandalonePipeline, AdapterStandalone, TEXT("no engine or gearbox"));

	// A captured settings copy remains independent from later source UObject edits.
	const FVehicleInputPipeline BeforeMutation = SolverPipeline;
	FVehicleInputPipeline CapturedSettingsResult = BeforeMutation;
	FVehicleInputSolver::Update(Settings, CapturedSettingsResult, MakeContext(Gearbox.Get(), Engine.Get(), 8.f, false), 0.016f);
	BrakeCurve->FloatCurve.Reset();
	BrakeCurve->FloatCurve.AddKey(0.f, 1.f);
	BrakeCurve->FloatCurve.AddKey(1.f, 1.f);
	FVehicleInputPipeline EditedSourceSettingsResult = BeforeMutation;
	const FVehicleInputRuntimeSettings EditedSourceSettings = FVehicleDriveInputRegressionProbe::Capture(*Drive);
	FVehicleInputPipeline RetainedSettingsResult = BeforeMutation;
	FVehicleInputSolver::Update(Settings, RetainedSettingsResult,
		MakeContext(Gearbox.Get(), Engine.Get(), 8.f, false), 0.016f);
	FVehicleInputSolver::Update(EditedSourceSettings, EditedSourceSettingsResult,
		MakeContext(Gearbox.Get(), Engine.Get(), 8.f, false), 0.016f);
	TestPipelineEqual(*this, RetainedSettingsResult, CapturedSettingsResult, TEXT("curve snapshot preserves full result after source edit"));
	TestTrue(TEXT("captured curve copy keeps old response after source edit"),
		!NearlyEqual(CapturedSettingsResult.Final.Brake, EditedSourceSettingsResult.Final.Brake));

	TestEqual(TEXT("Every frozen compatibility row was exercised"), ReferenceRow, 18);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleInputSolverAssistAndMailboxTest,
	"KinetiForge.Drivetrain.Input.AssistAndMailbox",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleInputSolverAssistAndMailboxTest::RunTest(const FString& Parameters)
{
	FVehicleFrozenBaseline Baseline;
	if (!Baseline.Load(*this, TEXT("input-assist.csv"), 3, 11)) return false;
	int32 ReferenceRow = 0;
	TStrongObjectPtr<UVehicleDriveAssemblyComponent> Drive(NewObject<UVehicleDriveAssemblyComponent>());
	TStrongObjectPtr<UVehicleGearboxComponent> Gearbox(NewObject<UVehicleGearboxComponent>());
	TStrongObjectPtr<UVehicleEngineComponent> Engine(NewObject<UVehicleEngineComponent>());
	FVehicleDriveInputRegressionProbe::Bind(*Drive, Engine.Get(), Gearbox.Get());
	FVehicleGearboxConfig GearboxConfig = Gearbox->GetConfig();
	GearboxConfig.ShiftDelay = 0.2f;
	Gearbox->SetConfig(GearboxConfig);

	Drive->InputAssistConfig.bAutomaticClutch = true;
	Drive->InputAssistConfig.bEVClutchLogic = false;
	Drive->InputAssistConfig.bRevMatching = true;
	Drive->InputAssistConfig.RevMatchMaxThrottle = 0.6f;
	Gearbox->ShiftToTargetGear(1, true);
	FVehicleInputRuntimeSettings Settings = FVehicleDriveInputRegressionProbe::Capture(*Drive);
	FVehicleInputPipeline Pipeline;
	Pipeline.Raw.Throttle = 0.1f;
	Pipeline.Raw.Clutch = 0.2f;
	FVehicleDriveInputRegressionProbe::SetPipeline(*Drive, Pipeline);
	FVehicleDriveInputRegressionProbe::Update(*Drive, 0.01f, 2.f, false, *this, Baseline, ReferenceRow);
	const FVehicleInputPipeline AdapterAutomaticClutch = FVehicleDriveInputRegressionProbe::GetPipeline(*Drive);
	FVehicleInputSolver::Update(Settings, Pipeline, MakeContext(Gearbox.Get(), Engine.Get(), 2.f, false), 0.01f);
	TestPipelineEqual(*this, Pipeline, AdapterAutomaticClutch, TEXT("automatic clutch assist"));

	// Use a value context for shift-rev-match state that cannot be held on a world-less component after immediate finalize.
	FVehicleInputStepContext RevMatchContext = MakeContext(Gearbox.Get(), Engine.Get(), 2.f, false);
	RevMatchContext.GearboxState.bShouldRevMatch = true;
	FVehicleInputSolver::Update(Settings, Pipeline, RevMatchContext, 0.01f);
	TestTrue(TEXT("rev-match advances throttle toward configured cap"), Pipeline.Smoothened.Throttle > 0.f && Pipeline.Smoothened.Throttle <= 0.6f);

	Settings.Assist.bEVClutchLogic = true;
	Drive->InputAssistConfig.bEVClutchLogic = true;
	Pipeline.Smoothened.Clutch = 0.7f;
	FVehicleDriveInputRegressionProbe::SetPipeline(*Drive, Pipeline);
	FVehicleDriveInputRegressionProbe::Update(*Drive, 0.01f, 2.f, false, *this, Baseline, ReferenceRow);
	const FVehicleInputPipeline AdapterEV = FVehicleDriveInputRegressionProbe::GetPipeline(*Drive);
	FVehicleInputSolver::Update(Settings, Pipeline, MakeContext(Gearbox.Get(), Engine.Get(), 2.f, false), 0.01f);
	TestTrue(TEXT("EV clutch logic forces smooth clutch to zero"), NearlyEqual(Pipeline.Smoothened.Clutch, 0.f));
	TestPipelineEqual(*this, Pipeline, AdapterEV, TEXT("EV clutch assist"));

	Settings.Assist.bEVClutchLogic = false;
	Drive->InputAssistConfig.bEVClutchLogic = false;
	Settings.Assist.bAutoHold = true;
	Pipeline.Raw.Throttle = 0.f;
	Pipeline.Raw.Handbrake = 0.f;
	FVehicleDriveInputRegressionProbe::SetPipeline(*Drive, Pipeline);
	FVehicleDriveInputRegressionProbe::Update(*Drive, 0.1f, 0.f, false, *this, Baseline, ReferenceRow);
	const FVehicleInputPipeline AdapterAutoHold = FVehicleDriveInputRegressionProbe::GetPipeline(*Drive);
	FVehicleInputSolver::Update(Settings, Pipeline, MakeContext(Gearbox.Get(), Engine.Get(), 0.f, false), 0.1f);
	TestTrue(TEXT("autohold raises handbrake at rest"), Pipeline.Smoothened.Handbrake > 0.f);
	TestPipelineEqual(*this, Pipeline, AdapterAutoHold, TEXT("autohold legacy comparison"));

	FVehicleDriveInputRuntime InitialDriveInput;
	InitialDriveInput.InputValues.Smoothened.Throttle = 0.4f;
	InitialDriveInput.InputValues.Smoothened.Brake = 0.3f;
	InitialDriveInput.InputValues.Final.Steering = 0.25f;
	FVehiclePowerUnitRuntime InitialRuntime;
	TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> Bridge = MakeShared<FVehiclePowerUnitBridge, ESPMode::ThreadSafe>(
		MoveTemp(InitialRuntime), MoveTemp(InitialDriveInput));
	const FVehiclePowerUnitResult InitiallyPublished = Bridge->ReadResult();

	FVehicleInputRuntimeSettings NewSettings;
	NewSettings.Throttle.InterpSpeed = FVector2f(9.f, 7.f);
	Bridge->SubmitInputSettings(NewSettings);
	FVehicleInputCommand ThrottleCommand;
	ThrottleCommand.Axis = EVehicleInputAxis::Throttle;
	ThrottleCommand.Input = 0.8f;
	ThrottleCommand.bDirectInput = true;
	ThrottleCommand.bSwitchThrottleAndBrake = true;
	Bridge->RequestInput(ThrottleCommand);
	FVehicleInputCommand BrakeCommand;
	BrakeCommand.Axis = EVehicleInputAxis::Brake;
	BrakeCommand.Input = 0.6f;
	BrakeCommand.bDirectInput = true;
	BrakeCommand.bSwitchThrottleAndBrake = true;
	Bridge->RequestInput(BrakeCommand);
	Bridge->ConsumeInputs();

	FVehicleDriveInputRuntime& RuntimeInput = Bridge->GetDriveInputForSimulation();
	TestTrue(TEXT("mailbox applies direct throttle and brake commands to switched axes"),
		NearlyEqual(RuntimeInput.InputValues.Smoothened.Throttle, 0.6f) && NearlyEqual(RuntimeInput.InputValues.Smoothened.Brake, 0.8f));
	TestTrue(TEXT("direct commands leave final presentation values untouched"),
		NearlyEqual(RuntimeInput.InputValues.Final.Steering, 0.25f));
	TestTrue(TEXT("settings and commands do not immediately rewrite published values"),
		NearlyEqual(Bridge->ReadResult().InputValues.Smoothened.Throttle, InitiallyPublished.InputValues.Smoothened.Throttle));
	Bridge->PublishResult();
	const FVehiclePowerUnitResult Published = Bridge->ReadResult();
	TestTrue(TEXT("publish exposes mailbox command results"),
		NearlyEqual(Published.InputValues.Raw.Throttle, 0.8f) && NearlyEqual(Published.InputValues.Smoothened.Brake, 0.8f));
	TestTrue(TEXT("published telemetry is preserved"), Published.Telemetry.NumOfWheelsOnGround == InitiallyPublished.Telemetry.NumOfWheelsOnGround);

	TStrongObjectPtr<UVehicleDriveAssemblyComponent> AdapterDrive(NewObject<UVehicleDriveAssemblyComponent>());
	FVehicleDriveInputRuntime AdapterInput;
	AdapterInput.InputValues.Smoothened.Throttle = 0.2f;
	AdapterInput.InputValues.Final.Throttle = 0.4f;
	TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> AdapterBridge = MakeShared<FVehiclePowerUnitBridge, ESPMode::ThreadSafe>(
		FVehiclePowerUnitRuntime(), MoveTemp(AdapterInput));
	FVehicleDriveInputRegressionProbe::BindBridge(*AdapterDrive, AdapterBridge);
	FVehicleDriveInputRegressionProbe::ApplyCommand(*AdapterDrive, EVehicleInputAxis::Throttle, 0.7f, true);
	FVehicleDriveInputRegressionProbe::RefreshBridge(*AdapterDrive);
	TestTrue(TEXT("adapter preserves latest requested raw input across stale published result"),
		NearlyEqual(FVehicleDriveInputRegressionProbe::GetPipeline(*AdapterDrive).Raw.Throttle, 0.7f));
	const FVehicleInputPipeline BoundView = FVehicleDriveInputRegressionProbe::GetPipeline(*AdapterDrive);
	FVehicleDriveInputRegressionProbe::UpdateAdapter(*AdapterDrive, 1.f);
	FVehicleDriveInputRegressionProbe::UpdateAdapterAxes(*AdapterDrive, 1.f);
	TestPipelineEqual(*this, FVehicleDriveInputRegressionProbe::GetPipeline(*AdapterDrive), BoundView,
		TEXT("GT helpers cannot advance an owned pipeline or rewrite its display view"));
	AdapterBridge->ConsumeInputs();
	const FVehicleInputPipeline& ConsumedInput = AdapterBridge->GetDriveInputForSimulation().InputValues;
	TestTrue(TEXT("consumed adapter direct command reaches smooth simulation input"),
		NearlyEqual(ConsumedInput.Smoothened.Throttle, 0.7f));
	TestTrue(TEXT("mailbox consumption leaves prior publication and final view lagging"),
		NearlyEqual(AdapterBridge->ReadResult().InputValues.Smoothened.Throttle, 0.2f)
		&& NearlyEqual(ConsumedInput.Final.Throttle, 0.4f));
	TestEqual(TEXT("Every frozen assist row was exercised"), ReferenceRow, 3);
	return true;
}
#endif // WITH_DEV_AUTOMATION_TESTS
