// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehicleClutchComponent.h"
#include "VehicleGearboxComponent.h"
#include "VehicleTransmissionSolver.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "UObject/StrongObjectPtr.h"

namespace VehicleDrivetrainExtractionTests
{
	struct FClutchExpectedStep
	{
		float Torque;
		float AngleDiff;
	};

	// Captured from the pre-extraction component implementation in
	// Saved/DrivetrainBeforeExtraction.log (ClutchApi, fixed inputs, 8 steps).
	static constexpr FClutchExpectedStep ExpectedClutchTraces[3][8] =
	{
		{ // ConstraintLock (enum value 2)
			{181.818192f, 0.f}, {181.818192f, 0.f}, {181.818192f, 0.f}, {181.818192f, 0.f},
			{181.818192f, 0.f}, {181.818192f, 0.f}, {181.818192f, 0.f}, {181.818192f, 0.f}
		},
		{ // FrictionClutch (enum value 0)
			{137.273056f, 0.099999994f}, {259.772186f, 0.199999988f}, {382.271271f, 0.299999982f},
			{504.770386f, 0.399999976f}, {600.f, 0.399999976f}, {600.f, 0.399999976f},
			{600.f, 0.399999976f}, {600.f, 0.399999976f}
		},
		{ // FluidCoupling (enum value 1)
			{153.846161f, 0.f}, {153.846161f, 0.f}, {153.846161f, 0.f}, {153.846161f, 0.f},
			{153.846161f, 0.f}, {153.846161f, 0.f}, {153.846161f, 0.f}, {153.846161f, 0.f}
		}
	};

	static const TCHAR* ClutchModeNames[] = { TEXT("ConstraintLock"), TEXT("FrictionClutch"), TEXT("FluidCoupling") };

	static FVehicleClutchConfig MakeClutchConfig(EClutchSimMode Mode)
	{
		FVehicleClutchConfig Config;
		Config.SimMode = Mode;
		Config.Capacity = 1.5f;
		Config.TorsionalStiffness = 10000.f;
		Config.DampingRatio = 0.1f;
		Config.ViscousDamping = 100.f;
		return Config;
	}

	static void ConfigureClutch(UVehicleClutchComponent& Clutch, EClutchSimMode Mode)
	{
		Clutch.SetConfig(MakeClutchConfig(Mode));
	}

	static void ClutchStep(UVehicleClutchComponent& Clutch, float EngineOmega, float InputOmega,
		float ClutchValue = 0.f, float GearRatio = 1.f)
	{
		Clutch.UpdatePhysics(0.01f, ClutchValue, InputOmega, 2.f, GearRatio,
			10000.f, EngineOmega, 0.2f, 400.f);
	}

	static void ConfigureGearbox(UVehicleGearboxComponent& Gearbox)
	{
		FVehicleGearboxConfig Config;
		Config.FirstGear = 3.f;
		Config.TopGear = 1.f;
		Config.NumberOfGears = 2;
		Config.NumOfReverseGears = 1;
		Config.GearRatioBias = 0.5f;
		Config.Efficiency = 0.9f;
		Config.ShiftDelay = 0.2f;
		Gearbox.SetConfig(Config);
		Gearbox.CalculateGearRatios();
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleClutchLegacyApiTest,
	"KinetiForge.Drivetrain.Legacy.ClutchApi",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleClutchLegacyApiTest::RunTest(const FString& Parameters)
{
	using namespace VehicleDrivetrainExtractionTests;
	TStrongObjectPtr<UVehicleClutchComponent> Clutch(NewObject<UVehicleClutchComponent>());

	// Fixed protocol inputs and expected values are captured from the pre-extraction UE5.3 baseline.
	const EClutchSimMode Modes[] = { EClutchSimMode::ConstraintLock,
		EClutchSimMode::FrictionClutch, EClutchSimMode::FluidCoupling };
	const float TorqueToleranceNm = 0.001f;
	const float AngleToleranceRad = 0.000001f;
	for (int32 ModeIndex = 0; ModeIndex < UE_ARRAY_COUNT(Modes); ++ModeIndex)
	{
		const EClutchSimMode Mode = Modes[ModeIndex];
		TStrongObjectPtr<UVehicleClutchComponent> TrajectoryClutch(NewObject<UVehicleClutchComponent>());
		ConfigureClutch(*TrajectoryClutch, Mode);
		FVehicleClutchSimState SolverState;
		const FVehicleClutchConfig SolverConfig = MakeClutchConfig(Mode);
		for (int32 Step = 1; Step <= 8; ++Step)
		{
			ClutchStep(*TrajectoryClutch, 100.f, 90.f);
			FVehicleClutchStepInput Input;
			Input.DeltaTime = 0.01f;
			Input.ClutchPedalValue = 0.f;
			Input.GearboxInputAngularVelocity = 90.f;
			Input.GearboxReflectedInertia = 2.f;
			Input.GearRatio = 1.f;
			Input.ReflectedDriveShaftStiffness = 10000.f;
			Input.EngineAngularVelocity = 100.f;
			Input.EngineInertia = 0.2f;
			Input.EngineMaxTorque = 400.f;
			FVehicleClutchSolver::Update(SolverConfig, SolverState, Input);

			const FClutchExpectedStep& Expected = ExpectedClutchTraces[ModeIndex][Step - 1];
			const FString Label = FString::Printf(TEXT("%s step %d"), ClutchModeNames[ModeIndex], Step);
			TestTrue(Label + TEXT(" component torque matches pre-extraction capture"),
				FMath::IsNearlyEqual(TrajectoryClutch->GetCluchTorque(), Expected.Torque, TorqueToleranceNm));
			TestTrue(Label + TEXT(" component angle matches pre-extraction capture"),
				FMath::IsNearlyEqual(TrajectoryClutch->GetClutchState().AngleDiff, Expected.AngleDiff, AngleToleranceRad));
			TestTrue(Label + TEXT(" pure solver torque independently matches pre-extraction capture"),
				FMath::IsNearlyEqual(SolverState.ClutchTorque, Expected.Torque, TorqueToleranceNm));
			TestTrue(Label + TEXT(" pure solver angle independently matches pre-extraction capture"),
				FMath::IsNearlyEqual(SolverState.AngleDiff, Expected.AngleDiff, AngleToleranceRad));
		}

		ConfigureClutch(*Clutch, Mode);
		ClutchStep(*Clutch, 100.f, 90.f);
		const float FirstPositiveSlipTorque = Clutch->GetCluchTorque();
		TestTrue(TEXT("Each engaged mode responds to positive slip"), FirstPositiveSlipTorque > 0.f);
		TestTrue(TEXT("Torque remains within the configured clutch capacity"),
			FMath::Abs(FirstPositiveSlipTorque) <= 600.f + KINDA_SMALL_NUMBER);
		ClutchStep(*Clutch, 80.f, 90.f);
		TestTrue(TEXT("Reversing slip reverses clutch torque"), Clutch->GetCluchTorque() < 0.f);
	}

	ConfigureClutch(*Clutch, EClutchSimMode::FrictionClutch);
	ClutchStep(*Clutch, 100.f, 90.f);
	TestTrue(TEXT("FrictionClutch accumulates angle history"),
		FMath::IsNearlyEqual(Clutch->GetClutchState().AngleDiff, 0.1f));
	ConfigureClutch(*Clutch, EClutchSimMode::ConstraintLock);
	ClutchStep(*Clutch, 100.f, 90.f);
	TestTrue(TEXT("ConstraintLock preserves existing FrictionClutch angle history"),
		FMath::IsNearlyEqual(Clutch->GetClutchState().AngleDiff, 0.1f));
	ClutchStep(*Clutch, 100.f, 90.f, 1.f);
	TestEqual(TEXT("Fully separated clutch transmits no torque"), Clutch->GetCluchTorque(), 0.f);
	TestTrue(TEXT("ConstraintLock preserves angle history while fully separated"),
		FMath::IsNearlyEqual(Clutch->GetClutchState().AngleDiff, 0.1f));
	ClutchStep(*Clutch, 100.f, 90.f, 0.f, 0.f);
	TestEqual(TEXT("Neutral ratio disengages the clutch"), Clutch->GetClutchState().ClutchLock, 0.f);
	TestEqual(TEXT("Neutral ratio transmits no torque"), Clutch->GetCluchTorque(), 0.f);

	// FluidCoupling explicitly clears the legacy torsional history.
	ConfigureClutch(*Clutch, EClutchSimMode::FluidCoupling);
	ClutchStep(*Clutch, 100.f, 90.f);
	TestEqual(TEXT("FluidCoupling resets AngleDiff"), Clutch->GetClutchState().AngleDiff, 0.f);

	AddInfo(TEXT("Clutch numeric baselines captured from Saved/DrivetrainBeforeExtraction.log, ClutchApi; torque abs tolerance 0.001 Nm, angle tolerance 1e-6 rad."));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleTransmissionRuntimeIsolationTest,
	"KinetiForge.Drivetrain.Runtime.IsolatedPaths",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleTransmissionRuntimeIsolationTest::RunTest(const FString& Parameters)
{
	using namespace VehicleDrivetrainExtractionTests;
	FVehicleClutchSimState OddClutchState;
	FVehicleClutchSimState EvenClutchState;
	const FVehicleClutchConfig OddConfig = MakeClutchConfig(EClutchSimMode::FrictionClutch);
	const FVehicleClutchConfig EvenConfig = MakeClutchConfig(EClutchSimMode::ConstraintLock);
	FVehicleClutchStepInput Input;
	Input.DeltaTime = 0.01f;
	Input.GearboxInputAngularVelocity = 90.f;
	Input.GearboxReflectedInertia = 2.f;
	Input.GearRatio = 1.f;
	Input.ReflectedDriveShaftStiffness = 10000.f;
	Input.EngineAngularVelocity = 100.f;
	Input.EngineInertia = 0.2f;
	Input.EngineMaxTorque = 400.f;
	for (int32 Step = 0; Step < 8; ++Step)
	{
		FVehicleClutchSolver::Update(OddConfig, OddClutchState, Input);
		FVehicleClutchSolver::Update(EvenConfig, EvenClutchState, Input);
		const FClutchExpectedStep& OddExpected = ExpectedClutchTraces[1][Step];
		const FClutchExpectedStep& EvenExpected = ExpectedClutchTraces[0][Step];
		TestTrue(FString::Printf(TEXT("Interleaved odd clutch trace step %d"), Step + 1),
			FMath::IsNearlyEqual(OddClutchState.ClutchTorque, OddExpected.Torque, 0.001f)
			&& FMath::IsNearlyEqual(OddClutchState.AngleDiff, OddExpected.AngleDiff, 0.000001f));
		TestTrue(FString::Printf(TEXT("Interleaved even clutch trace step %d"), Step + 1),
			FMath::IsNearlyEqual(EvenClutchState.ClutchTorque, EvenExpected.Torque, 0.001f)
			&& FMath::IsNearlyEqual(EvenClutchState.AngleDiff, EvenExpected.AngleDiff, 0.000001f));
	}
	TestTrue(TEXT("Interleaved paths retain independent angle histories"),
		!FMath::IsNearlyEqual(OddClutchState.AngleDiff, EvenClutchState.AngleDiff, 0.000001f));

	const float OddRatio = 3.f;
	const float EvenRatio = -2.f;
	const float OddOutputTorque = FVehicleGearPathSolver::CalculateOutputTorque(15.f, OddRatio, 0.9f);
	const float EvenOutputTorque = FVehicleGearPathSolver::CalculateOutputTorque(15.f, EvenRatio, 0.9f);
	const auto OddReflection = FVehicleGearPathSolver::ReflectOutputShaft(12.f, 18.f, 900.f, OddRatio);
	const auto EvenReflection = FVehicleGearPathSolver::ReflectOutputShaft(12.f, 18.f, 900.f, EvenRatio);
	TestTrue(TEXT("Distinct path ratios produce independent output torques"),
		FMath::IsNearlyEqual(OddOutputTorque, 40.5f) && FMath::IsNearlyEqual(EvenOutputTorque, -27.f));
	TestTrue(TEXT("Path outputs sum from their separately selected ratios"),
		FMath::IsNearlyEqual(OddOutputTorque + EvenOutputTorque, 13.5f));
	TestTrue(TEXT("Forward path reflection uses its ratio"),
		FMath::IsNearlyEqual(OddReflection.InputAngularVelocity, 36.f)
		&& FMath::IsNearlyEqual(OddReflection.ReflectedInertia, 2.f)
		&& FMath::IsNearlyEqual(OddReflection.ReflectedDriveShaftStiffness, 100.f));
	TestTrue(TEXT("Reverse path reflects angular velocity sign but squares ratio for inertia"),
		FMath::IsNearlyEqual(EvenReflection.InputAngularVelocity, -24.f)
		&& FMath::IsNearlyEqual(EvenReflection.ReflectedInertia, 4.5f)
		&& FMath::IsNearlyEqual(EvenReflection.ReflectedDriveShaftStiffness, 225.f));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleGearboxLegacyApiTest,
	"KinetiForge.Drivetrain.Legacy.GearboxApi",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleGearboxLegacyApiTest::RunTest(const FString& Parameters)
{
	using namespace VehicleDrivetrainExtractionTests;
	TStrongObjectPtr<UVehicleGearboxComponent> Gearbox(NewObject<UVehicleGearboxComponent>());
	ConfigureGearbox(*Gearbox);

	Gearbox->ShiftToTargetGear(1, true);
	TestEqual(TEXT("First forward gear is selected"), Gearbox->GetCurrentGear(), 1);
	TestEqual(TEXT("First forward gear ratio"), Gearbox->GetCurrentGearRatio(), 3.f);
	float OutputTorque = 0.f;
	Gearbox->UpdateOutputShaft(10.f, OutputTorque);
	TestTrue(TEXT("Forward output includes clutch torque and efficiency"),
		FMath::IsNearlyEqual(OutputTorque, 27.f));

	Gearbox->SetP2MotorTorque(5.f);
	Gearbox->UpdateOutputShaft(10.f, OutputTorque);
	TestTrue(TEXT("P2 torque is added before gear ratio and efficiency"),
		FMath::IsNearlyEqual(OutputTorque, 40.5f));

	float ClutchVelocity = 0.f;
	float ReflectedInertia = 0.f;
	float ReflectedStiffness = 0.f;
	float CurrentRatio = 0.f;
	Gearbox->UpdateInputShaft(12.f, 18.f, 900.f,
		ClutchVelocity, ReflectedInertia, ReflectedStiffness, CurrentRatio);
	TestTrue(TEXT("Forward ratio reflects input angular velocity"), FMath::IsNearlyEqual(ClutchVelocity, 36.f));
	TestTrue(TEXT("Forward ratio reflects inertia"), FMath::IsNearlyEqual(ReflectedInertia, 2.f));
	TestTrue(TEXT("Forward ratio reflects shaft stiffness"), FMath::IsNearlyEqual(ReflectedStiffness, 100.f));
	TestEqual(TEXT("Forward ratio is returned to the clutch"), CurrentRatio, 3.f);

	Gearbox->ShiftToTargetGear(2, true);
	TestEqual(TEXT("Top forward gear ratio"), Gearbox->GetCurrentGearRatio(), 1.f);
	Gearbox->ShiftToTargetGear(-1, true);
	TestEqual(TEXT("Reverse gear ratio has reverse sign"), Gearbox->GetCurrentGearRatio(), -3.f);
	Gearbox->UpdateOutputShaft(10.f, OutputTorque);
	TestTrue(TEXT("Reverse output torque has reverse sign and includes P2 torque"), FMath::IsNearlyEqual(OutputTorque, -40.5f));

	Gearbox->ShiftToTargetGear(0, true);
	TestEqual(TEXT("Neutral selects zero gear ratio"), Gearbox->GetCurrentGearRatio(), 0.f);
	Gearbox->UpdateOutputShaft(10.f, OutputTorque);
	TestEqual(TEXT("Neutral transmits no gearbox output torque"), OutputTorque, 0.f);
	Gearbox->UpdateInputShaft(12.f, 18.f, 900.f,
		ClutchVelocity, ReflectedInertia, ReflectedStiffness, CurrentRatio);
	TestEqual(TEXT("Neutral reports zero clutch input velocity"), ClutchVelocity, 0.f);
	TestEqual(TEXT("Neutral reports zero reflected inertia"), ReflectedInertia, 0.f);
	TestEqual(TEXT("Neutral reports zero reflected shaft stiffness"), ReflectedStiffness, 0.f);
	TestEqual(TEXT("Neutral returns zero ratio"), CurrentRatio, 0.f);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
