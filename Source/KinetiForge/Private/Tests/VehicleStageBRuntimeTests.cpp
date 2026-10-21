// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehicleEngineSolver.h"
#include "VehicleGearboxSolver.h"
#include "VehicleUtilities.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

namespace VehicleStageBRuntimeTests
{
	static FVehicleEngineParameters MakeEngineParameters()
	{
		FVehicleNaturallyAspiratedEngineConfig Config;
		Config.MaxEngineTorque = 0.f;
		Config.EngineIdleRPM = 900.f;
		Config.EngineMaxRPM = 6000.f;
		Config.EngineInertia = 0.2f;
		Config.StartFriction = 0.f;
		Config.FrictionCoefficient = 0.f;
		Config.PumpingLossCoefficient = 0.f;
		Config.IdleThrottleInterpSpeed = 1.f;
		Config.RevLimiterTime = 0.05f;
		Config.UnburntFuelAccumulationRate = 0.f;
		return FVehicleEngineParameters::FromConfig(Config);
	}

	static FVehicleGearboxConfig MakeGearboxConfig()
	{
		FVehicleGearboxConfig Config;
		Config.FirstGear = 3.f;
		Config.TopGear = 1.f;
		Config.NumberOfGears = 4;
		Config.NumOfReverseGears = 2;
		Config.GearRatioBias = 0.5f;
		Config.ShiftDelay = 0.2f;
		return Config;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleGearboxRuntimeStateTest,
	"KinetiForge.Drivetrain.Runtime.GearboxState",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleGearboxRuntimeStateTest::RunTest(const FString& Parameters)
{
	using namespace VehicleStageBRuntimeTests;
	FVehicleGearboxConfig Config = MakeGearboxConfig();
	TArray<float> ForwardRatios;
	TArray<float> ReverseRatios;
	TestTrue(TEXT("Configured forward and reverse ratios generate"),
		FVehicleGearboxSolver::CalculateGearRatios(Config, ForwardRatios, ReverseRatios));

	FVehicleGearboxRuntimeState State;
	TestFalse(TEXT("Current gear is rejected"), FVehicleGearboxSolver::BeginShift(
		Config, State, ForwardRatios.Num(), ReverseRatios.Num(), 0));
	TestFalse(TEXT("Gear above available forwards is rejected"), FVehicleGearboxSolver::BeginShift(
		Config, State, ForwardRatios.Num(), ReverseRatios.Num(), 5));
	TestFalse(TEXT("Gear below available reverse range is rejected"), FVehicleGearboxSolver::BeginShift(
		Config, State, ForwardRatios.Num(), ReverseRatios.Num(), -3));

	Config.bSequentialGearbox = true;
	State.CurrentGear = 1;
	State.CurrentGearRatio = ForwardRatios[0];
	TestTrue(TEXT("Sequential upshift is limited to one gear"), FVehicleGearboxSolver::BeginShift(
		Config, State, ForwardRatios.Num(), ReverseRatios.Num(), 4));
	TestEqual(TEXT("Sequential target is the next gear"), State.TargetGear, 2);
	TestTrue(TEXT("Sequential upshift requests spark cut above first gear"), State.bShouldCutSpark);
	TestEqual(TEXT("Ratio is zero during an active shift"), State.CurrentGearRatio, 0.f);
	TestFalse(TEXT("A second shift is rejected before completion"), FVehicleGearboxSolver::BeginShift(
		Config, State, ForwardRatios.Num(), ReverseRatios.Num(), 3));

	TestFalse(TEXT("Shift remains active before its delay"), FVehicleGearboxSolver::AdvanceShift(
		Config, State, ForwardRatios, ReverseRatios, 0.19f));
	TestTrue(TEXT("Shift completes once elapsed time exceeds the delay"), FVehicleGearboxSolver::AdvanceShift(
		Config, State, ForwardRatios, ReverseRatios, 0.02f));
	TestEqual(TEXT("Completed shift commits the target gear"), State.CurrentGear, 2);
	TestTrue(TEXT("Completed shift restores selected ratio"), FMath::IsNearlyEqual(State.CurrentGearRatio, ForwardRatios[1]));
	TestTrue(TEXT("Completion clears shift flags and target"), State.bIsInGear && !State.bShouldCutSpark
		&& !State.bShouldRevMatch && State.TargetGear == 0);
	TestFalse(TEXT("Completion is reported only once"), FVehicleGearboxSolver::AdvanceShift(
		Config, State, ForwardRatios, ReverseRatios, 0.2f));

	TestTrue(TEXT("Sequential downshift is limited to one gear"), FVehicleGearboxSolver::BeginShift(
		Config, State, ForwardRatios.Num(), ReverseRatios.Num(), -1));
	TestEqual(TEXT("Downshift target steps toward neutral"), State.TargetGear, 1);
	TestFalse(TEXT("First gear upshift does not cut spark"), State.bShouldCutSpark);
	FVehicleGearboxSolver::CompleteShift(Config, State, ForwardRatios, ReverseRatios);
	State.CurrentGear = 3;
	State.CurrentGearRatio = ForwardRatios[2];
	TestTrue(TEXT("Downshift sets rev-match while engaged"), FVehicleGearboxSolver::BeginShift(
		Config, State, ForwardRatios.Num(), ReverseRatios.Num(), 2));
	TestTrue(TEXT("Non-neutral downshift requests rev match"), State.bShouldRevMatch);
	FVehicleGearboxSolver::CompleteShift(Config, State, ForwardRatios, ReverseRatios);
	Config.bSequentialGearbox = false;
	TestTrue(TEXT("Shift to neutral is accepted"), FVehicleGearboxSolver::BeginShift(
		Config, State, ForwardRatios.Num(), ReverseRatios.Num(), 0));
	TestFalse(TEXT("Neutral shift does not request rev match"), State.bShouldRevMatch);
	FVehicleGearboxSolver::CompleteShift(Config, State, ForwardRatios, ReverseRatios);
	TestEqual(TEXT("Completed neutral shift has zero ratio"), State.CurrentGearRatio, 0.f);

	FVehicleGearboxConfig SingleConfig = MakeGearboxConfig();
	SingleConfig.NumberOfGears = 1;
	SingleConfig.NumOfReverseGears = 1;
	SingleConfig.FirstGear = 2.5f;
	SingleConfig.TopGear = 0.5f;
	TArray<float> SingleForward;
	TArray<float> SingleReverse;
	TestTrue(TEXT("Single forward and reverse gear arrays generate"),
		FVehicleGearboxSolver::CalculateGearRatios(SingleConfig, SingleForward, SingleReverse));
	TestEqual(TEXT("Single forward gear uses first ratio"), SingleForward[0], 2.5f);
	TestEqual(TEXT("Single reverse gear uses reverse sign"), SingleReverse[0], -2.5f);

	FVehicleGearboxConfig ReverseHeavyConfig = MakeGearboxConfig();
	ReverseHeavyConfig.NumberOfGears = 1;
	ReverseHeavyConfig.NumOfReverseGears = 3;
	ReverseHeavyConfig.FirstGear = -4.f;
	ReverseHeavyConfig.TopGear = -1.f;
	ReverseHeavyConfig.GearRatioBias = 2.f;
	TArray<float> ReverseHeavyForward;
	TArray<float> ReverseHeavyReverse;
	TestTrue(TEXT("More reverse gears than forward gears generate"),
		FVehicleGearboxSolver::CalculateGearRatios(ReverseHeavyConfig,
			ReverseHeavyForward, ReverseHeavyReverse));
	TestTrue(TEXT("Gear generation normalizes first, top and bias"),
		FMath::IsNearlyEqual(ReverseHeavyConfig.FirstGear, 4.f)
		&& FMath::IsNearlyEqual(ReverseHeavyConfig.TopGear, 1.f)
		&& FMath::IsNearlyEqual(ReverseHeavyConfig.GearRatioBias, 1.f));
	TestTrue(TEXT("Reverse-heavy ratios retain reverse signs and normalized ordering"),
		// Preserve legacy Pow(0, 0) at bias=1: even the first ratio becomes TopGear.
		FMath::IsNearlyEqual(ReverseHeavyReverse[0], -1.f)
		&& FMath::IsNearlyEqual(ReverseHeavyReverse[1], -1.f)
		&& FMath::IsNearlyEqual(ReverseHeavyReverse[2], -1.f)
		&& FMath::IsNearlyEqual(ReverseHeavyForward[0], 1.f));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleEngineRuntimeStateTest,
	"KinetiForge.Drivetrain.Runtime.EngineState",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleEngineRuntimeStateTest::RunTest(const FString& Parameters)
{
	using namespace VehicleStageBRuntimeTests;
	const FVehicleEngineParameters ParametersValue = MakeEngineParameters();
	FVehicleEngineTurboConfig TurboConfig;
	FVehicleEngineExhaustConfig ExhaustConfig;
	ExhaustConfig.IgnitionProbability = 0.f;
	FVehicleLUT<32> TorqueLUT(1.f);

	FVehicleEngineSimState State;
	State.OperationMode = EVehicleEngineOperationMode::Off;
	TestTrue(TEXT("Start command transitions Off to Starting"),
		FVehicleEngineSolver::StartVehicleEngine(State) == EVehicleEngineOperationMode::Starting);
	TestTrue(TEXT("Shutdown command leaves Starting unchanged"),
		FVehicleEngineSolver::ShutdownVehicleEngine(State) == EVehicleEngineOperationMode::Starting);

	State.OperationMode = EVehicleEngineOperationMode::Starting;
	State.EngineRPM = 1400.f;
	State.EngineAngularVelocity = 1400.f * PI / 30.f;
	State.StarterMotorTorque = 20.f;
	bool bStartTurboEvent = false;
	bool bStartBackfireEvent = false;
	FVehicleEngineSolver::UpdatePhysics(ParametersValue, TurboConfig, ExhaustConfig, TorqueLUT,
		State, 0.01f, 0.f, 0.f, false, bStartTurboEvent, bStartBackfireEvent);
	TestTrue(TEXT("Push-start speed transitions Starting to On"), State.OperationMode == EVehicleEngineOperationMode::On);
	TestEqual(TEXT("Push start clears starter torque"), State.StarterMotorTorque, 0.f);
	TestTrue(TEXT("Shutdown command transitions On to Shutting"),
		FVehicleEngineSolver::ShutdownVehicleEngine(State) == EVehicleEngineOperationMode::Shutting);

	FVehicleEngineSimState IdleState;
	IdleState.OperationMode = EVehicleEngineOperationMode::On;
	IdleState.EngineRPM = 700.f; // Below idle, but above the legacy 630 RPM stall threshold.
	IdleState.EngineAngularVelocity = 700.f * PI / 30.f;
	bool bTurboEvent = false;
	bool bBackfireEvent = false;
	FVehicleEngineSolver::UpdatePhysics(ParametersValue, TurboConfig, ExhaustConfig, TorqueLUT,
		IdleState, 0.01f, 0.f, 0.f, false, bTurboEvent, bBackfireEvent);
	TestTrue(TEXT("Low-RPM idle control opens throttle and enables fuel"),
		IdleState.RealThrottle > 0.f && IdleState.bFuelInjection && IdleState.bSpark);

	FVehicleEngineSimState RevLimitState;
	RevLimitState.OperationMode = EVehicleEngineOperationMode::On;
	RevLimitState.EngineRPM = 6200.f;
	RevLimitState.EngineAngularVelocity = 6200.f * PI / 30.f;
	FVehicleEngineSolver::UpdatePhysics(ParametersValue, TurboConfig, ExhaustConfig, TorqueLUT,
		RevLimitState, 0.01f, 1.f, 0.f, false, bTurboEvent, bBackfireEvent);
	TestFalse(TEXT("Rev limiter suppresses spark above maximum RPM"), RevLimitState.bSpark);

	FVehicleEngineTurboConfig BlowerTurboConfig;
	BlowerTurboConfig.MaxBoostPressure = 1.f;
	BlowerTurboConfig.FullBoostRPM = 3500.f;
	BlowerTurboConfig.SpoolStartRPM = 1200.f;
	FVehicleEngineSimState TurboState;
	TurboState.EngineRPM = 3000.f;
	TurboState.EngineAngularVelocity = 3000.f * PI / 30.f;
	TurboState.TurboSpool = 0.8f;
	TurboState.RawThrottleInput = 0.f;
	TurboState.RealThrottle = 0.f;
	FVehicleEngineSolver::EngineAcceleration(ParametersValue, BlowerTurboConfig, TorqueLUT,
		TurboState, 0.01f, bTurboEvent);
	TestTrue(TEXT("Turbo acceleration marks blow-off event for component Tick"), bTurboEvent);

	FVehicleEngineSimState PositiveP1State;
	FVehicleEngineSimState NegativeP1State;
	PositiveP1State.OperationMode = NegativeP1State.OperationMode = EVehicleEngineOperationMode::On;
	PositiveP1State.P1MotorTorque = 100.f;
	NegativeP1State.P1MotorTorque = -100.f;
	const FVehicleEngineParameters MotorParameters = []()
	{
		FVehicleNaturallyAspiratedEngineConfig Config;
		Config.EngineIdleRPM = 0.f;
		Config.MaxEngineTorque = 0.f;
		Config.EngineInertia = 0.2f;
		Config.StartFriction = Config.FrictionCoefficient = Config.PumpingLossCoefficient = 0.f;
		Config.UnburntFuelAccumulationRate = 0.f;
		return FVehicleEngineParameters::FromConfig(Config);
	}();
	FVehicleEngineSolver::UpdatePhysics(MotorParameters, TurboConfig, ExhaustConfig, TorqueLUT,
		PositiveP1State, 0.01f, 0.f, 0.f, false, bTurboEvent, bBackfireEvent);
	TestTrue(TEXT("Positive P1 torque spins its caller-owned engine state forward"),
		PositiveP1State.EngineAngularVelocity > 0.f);
	TestEqual(TEXT("Interleaved engine state remains untouched until stepped"),
		NegativeP1State.EngineAngularVelocity, 0.f);
	FVehicleEngineSolver::UpdatePhysics(MotorParameters, TurboConfig, ExhaustConfig, TorqueLUT,
		NegativeP1State, 0.01f, 0.f, 0.f, false, bTurboEvent, bBackfireEvent);
	TestTrue(TEXT("Negative P1 torque spins its caller-owned engine state backward"),
		NegativeP1State.EngineAngularVelocity < 0.f);
	TestTrue(TEXT("Separate engine states remain distinct after interleaved updates"),
		PositiveP1State.EngineAngularVelocity > 0.f && NegativeP1State.EngineAngularVelocity < 0.f);

	bool bExistingTurboEvent = true;
	bool bExistingBackfireEvent = true;
	FVehicleEngineSimState NoNewEventsState;
	NoNewEventsState.OperationMode = EVehicleEngineOperationMode::Off;
	FVehicleEngineSolver::UpdatePhysics(ParametersValue, TurboConfig, ExhaustConfig, TorqueLUT,
		NoNewEventsState, 0.01f, 0.f, 0.f, false, bExistingTurboEvent, bExistingBackfireEvent);
	TestTrue(TEXT("Solver leaves pending discrete event flags for component Tick to clear"),
		bExistingTurboEvent && bExistingBackfireEvent);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
