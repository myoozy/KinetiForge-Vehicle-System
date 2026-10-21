// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehicleDifferentialComponent.h"
#include "VehicleDifferentialSolver.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "UObject/StrongObjectPtr.h"

namespace VehicleDifferentialExtractionTests
{
	static void Configure(UVehicleDifferentialComponent& Differential, float GearRatio = 3.f,
		float DriveLockRatio = 0.5f, float CoastLockRatio = 0.25f)
	{
		FVehicleLimitedSlipDifferentialConfig Config;
		Config.GearRatio = GearRatio;
		Config.DriveLockRatio = DriveLockRatio;
		Config.CoastLockRatio = CoastLockRatio;
		Differential.SetConfig(Config);
	}

	static void Solve(UVehicleDifferentialComponent& Differential, float Torque,
		float LeftOmega, float RightOmega, float LeftInertia, float RightInertia, float DeltaTime,
		float& OutLeftTorque, float& OutRightTorque)
	{
		Differential.GetOutputTorque(Torque, LeftOmega, RightOmega, LeftInertia, RightInertia,
			DeltaTime, OutLeftTorque, OutRightTorque);
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleDifferentialLegacyApiTest,
	"KinetiForge.Drivetrain.Legacy.DifferentialApi",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleDifferentialLegacyApiTest::RunTest(const FString& Parameters)
{
	using namespace VehicleDifferentialExtractionTests;
	TStrongObjectPtr<UVehicleDifferentialComponent> Differential(NewObject<UVehicleDifferentialComponent>());
	Configure(*Differential);

	float LeftTorque = 0.f;
	float RightTorque = 0.f;
	Solve(*Differential, 100.f, 10.f, 20.f, 2.f, 6.f, 0.01f, LeftTorque, RightTorque);
	TestTrue(TEXT("Drive lock frozen left/right torques"),
		FMath::IsNearlyEqual(LeftTorque, 900.f) && FMath::IsNearlyEqual(RightTorque, -600.f));
	TestTrue(TEXT("Limited slip transfer preserves total drive torque"),
		FMath::IsNearlyEqual(LeftTorque + RightTorque, 300.f));
	const float CapturedDriveLeftTorque = LeftTorque;
	const float CapturedDriveRightTorque = RightTorque;

	float LeftWheelReflectedInertia = 0.f;
	Differential->UpdateOutputShaft(100.f, 10.f, 20.f, 2.f, 6.f, 0.01f, 4.f,
		LeftTorque, RightTorque, LeftWheelReflectedInertia);
	TestTrue(TEXT("Output shaft reflects inertia equally through squared ratio"),
		FMath::IsNearlyEqual(LeftWheelReflectedInertia, 18.f));
	const float CapturedReflectedWheelInertia = LeftWheelReflectedInertia;

	Solve(*Differential, -100.f, 10.f, 20.f, 2.f, 6.f, 0.01f, LeftTorque, RightTorque);
	TestTrue(TEXT("Negative drive torque selects coast lock"),
		FMath::IsNearlyEqual(LeftTorque, 225.f) && FMath::IsNearlyEqual(RightTorque, -525.f));
	TestTrue(TEXT("Coast lock transfer preserves total torque"),
		FMath::IsNearlyEqual(LeftTorque + RightTorque, -300.f));
	const float CapturedCoastLeftTorque = LeftTorque;
	const float CapturedCoastRightTorque = RightTorque;

	Solve(*Differential, 0.f, 10.f, 20.f, 2.f, 6.f, 0.01f, LeftTorque, RightTorque);
	TestTrue(TEXT("Zero torque follows the legacy drive-lock branch"),
		FMath::IsNearlyEqual(LeftTorque, 750.f) && FMath::IsNearlyEqual(RightTorque, -750.f));
	const float CapturedZeroTorqueLeft = LeftTorque;
	const float CapturedZeroTorqueRight = RightTorque;

	Solve(*Differential, 100.f, -10.f, -20.f, 2.f, 6.f, 0.01f, LeftTorque, RightTorque);
	TestTrue(TEXT("Reverse wheel rotation selects coast lock"),
		FMath::IsNearlyEqual(LeftTorque, -225.f) && FMath::IsNearlyEqual(RightTorque, 525.f));
	const float CapturedReverseRotationLeft = LeftTorque;
	const float CapturedReverseRotationRight = RightTorque;

	float SwappedLeftTorque = 0.f;
	float SwappedRightTorque = 0.f;
	Solve(*Differential, 100.f, 20.f, 10.f, 6.f, 2.f, 0.01f,
		SwappedLeftTorque, SwappedRightTorque);
	TestTrue(TEXT("Swapping sides swaps output torques"),
		FMath::IsNearlyEqual(SwappedLeftTorque, -600.f)
		&& FMath::IsNearlyEqual(SwappedRightTorque, 900.f));
	TestTrue(TEXT("Swapping sides preserves total torque"),
		FMath::IsNearlyEqual(SwappedLeftTorque + SwappedRightTorque, 300.f));

	Configure(*Differential, 3.f, 0.f, 0.f);
	Solve(*Differential, 100.f, 10.f, 20.f, 2.f, 6.f, 0.01f, LeftTorque, RightTorque);
	TestTrue(TEXT("Open differential splits torque equally without LSD exchange"),
		FMath::IsNearlyEqual(LeftTorque, 150.f) && FMath::IsNearlyEqual(RightTorque, 150.f));

	Configure(*Differential);
	Solve(*Differential, 100.f, 15.f, 15.f, 2.f, 6.f, 0.01f, LeftTorque, RightTorque);
	TestTrue(TEXT("Equal wheel speeds have no limited-slip transfer"),
		FMath::IsNearlyEqual(LeftTorque, 150.f) && FMath::IsNearlyEqual(RightTorque, 150.f));

	Solve(*Differential, 100.f, 10.f, 20.f, 2.f, 6.f, 0.f, LeftTorque, RightTorque);
	TestTrue(TEXT("Zero delta time SafeDivide removes transfer torque"),
		FMath::IsNearlyEqual(LeftTorque, 150.f) && FMath::IsNearlyEqual(RightTorque, 150.f));

	Solve(*Differential, 100.f, 10.f, 20.f, 0.f, 0.f, 0.01f, LeftTorque, RightTorque);
	TestTrue(TEXT("Zero total inertia SafeDivide removes transfer torque"),
		FMath::IsNearlyEqual(LeftTorque, 150.f) && FMath::IsNearlyEqual(RightTorque, 150.f));

	Configure(*Differential, 0.f, 0.5f, 0.25f);
	Solve(*Differential, 100.f, 10.f, 20.f, 2.f, 6.f, 0.01f, LeftTorque, RightTorque);
	TestTrue(TEXT("Zero ratio retains internal drive-lock exchange while open torque is zero"),
		FMath::IsNearlyEqual(LeftTorque, 750.f) && FMath::IsNearlyEqual(RightTorque, -750.f));
	Differential->UpdateOutputShaft(100.f, 10.f, 20.f, 2.f, 6.f, 0.01f, 4.f,
		LeftTorque, RightTorque, LeftWheelReflectedInertia);
	TestEqual(TEXT("Zero ratio reflects zero wheel inertia"), LeftWheelReflectedInertia, 0.f);

	float InputShaftVelocity = 0.f;
	float ReflectedInertia = 0.f;
	Configure(*Differential);
	Differential->UpdateInputShaft(10.f, 20.f, 2.f, 6.f, InputShaftVelocity, ReflectedInertia);
	TestTrue(TEXT("Input shaft speed uses average wheel speed times ratio"),
		FMath::IsNearlyEqual(InputShaftVelocity, 45.f));
	TestTrue(TEXT("Input shaft inertia reflects through squared ratio"),
		FMath::IsNearlyEqual(ReflectedInertia, 8.f / 9.f));
	Configure(*Differential, 0.f);
	Differential->UpdateInputShaft(10.f, 20.f, 2.f, 6.f, InputShaftVelocity, ReflectedInertia);
	TestEqual(TEXT("Zero ratio gives zero input shaft velocity"), InputShaftVelocity, 0.f);
	TestEqual(TEXT("Zero ratio SafeDivide gives zero reflected inertia"), ReflectedInertia, 0.f);

	AddInfo(FString::Printf(TEXT("Differential API captures: drive=(%.3f, %.3f), coast=(%.3f, %.3f), zeroTorque=(%.3f, %.3f), reverseRotation=(%.3f, %.3f), ratio=3 reflected wheel inertia=%.3f."),
		CapturedDriveLeftTorque, CapturedDriveRightTorque,
		CapturedCoastLeftTorque, CapturedCoastRightTorque,
		CapturedZeroTorqueLeft, CapturedZeroTorqueRight,
		CapturedReverseRotationLeft, CapturedReverseRotationRight,
		CapturedReflectedWheelInertia));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleDifferentialRuntimeValuesTest,
	"KinetiForge.Drivetrain.Runtime.DifferentialValues",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleDifferentialRuntimeValuesTest::RunTest(const FString& Parameters)
{
	FVehicleLimitedSlipDifferentialConfig Config;
	Config.GearRatio = 3.f;
	Config.DriveLockRatio = 0.5f;
	Config.CoastLockRatio = 0.25f;
	// Independently check the value-only API against the old component capture.
	FVehicleDifferentialStepInput WheelInput;
	WheelInput.InputTorque = 100.f;
	WheelInput.LeftAngularVelocity = 10.f;
	WheelInput.RightAngularVelocity = 20.f;
	WheelInput.LeftTotalInertia = 2.f;
	WheelInput.RightTotalInertia = 6.f;
	WheelInput.DeltaTime = 0.01f;
	const FVehicleDifferentialTorqueOutput DriveWheelOutput =
		FVehicleDifferentialSolver::CalculateOutputTorques(Config, WheelInput);
	TestTrue(TEXT("Value-only differential matches captured drive outputs"),
		FMath::IsNearlyEqual(DriveWheelOutput.LeftTorque, 900.f)
		&& FMath::IsNearlyEqual(DriveWheelOutput.RightTorque, -600.f));
	WheelInput.InputTorque = -100.f;
	const FVehicleDifferentialTorqueOutput CoastWheelOutput =
		FVehicleDifferentialSolver::CalculateOutputTorques(Config, WheelInput);
	TestTrue(TEXT("Value-only differential matches captured coast outputs"),
		FMath::IsNearlyEqual(CoastWheelOutput.LeftTorque, 225.f)
		&& FMath::IsNearlyEqual(CoastWheelOutput.RightTorque, -525.f));

	const float LockedAngularVelocity = FVehicleDifferentialSolver::CalculateLockedAngularVelocity(140.f, 8.f);
	TestTrue(TEXT("Locked velocity is momentum divided by total inertia"),
		FMath::IsNearlyEqual(LockedAngularVelocity, 17.5f));

	const FVehicleTransferCaseAxleOutput DriveAxle1 =
		FVehicleDifferentialSolver::CalculateTransferCaseAxleOutput(Config, 300.f,
			LockedAngularVelocity, 10.f, 2.f, 1.f, 4.f, 0.01f);
	const FVehicleTransferCaseAxleOutput DriveAxle2 =
		FVehicleDifferentialSolver::CalculateTransferCaseAxleOutput(Config, 300.f,
			LockedAngularVelocity, 20.f, 6.f, 3.f, 4.f, 0.01f);
	TestTrue(TEXT("Drive lock produces frozen 825/-525 axle torques"),
		FMath::IsNearlyEqual(DriveAxle1.DriveTorque, 825.f)
		&& FMath::IsNearlyEqual(DriveAxle2.DriveTorque, -525.f));
	TestTrue(TEXT("Drive torque split uses normalized 1:3 weights"),
		FMath::IsNearlyEqual(DriveAxle1.NormalizedTorqueWeight, 0.25f)
		&& FMath::IsNearlyEqual(DriveAxle2.NormalizedTorqueWeight, 0.75f));
	TestTrue(TEXT("Drive axle outputs sum to the input torque"),
		FMath::IsNearlyEqual(DriveAxle1.DriveTorque + DriveAxle2.DriveTorque, 300.f));

	const FVehicleTransferCaseAxleOutput CoastAxle1 =
		FVehicleDifferentialSolver::CalculateTransferCaseAxleOutput(Config, -300.f,
			LockedAngularVelocity, 10.f, 2.f, 1.f, 4.f, 0.01f);
	const FVehicleTransferCaseAxleOutput CoastAxle2 =
		FVehicleDifferentialSolver::CalculateTransferCaseAxleOutput(Config, -300.f,
			LockedAngularVelocity, 20.f, 6.f, 3.f, 4.f, 0.01f);
	TestTrue(TEXT("Coast lock produces frozen 300/-600 axle torques"),
		FMath::IsNearlyEqual(CoastAxle1.DriveTorque, 300.f)
		&& FMath::IsNearlyEqual(CoastAxle2.DriveTorque, -600.f));
	TestTrue(TEXT("Coast axle outputs sum to input torque"),
		FMath::IsNearlyEqual(CoastAxle1.DriveTorque + CoastAxle2.DriveTorque, -300.f));

	const float ReverseLockedAngularVelocity = FVehicleDifferentialSolver::CalculateLockedAngularVelocity(-140.f, 8.f);
	const FVehicleTransferCaseAxleOutput ReverseAxle1 =
		FVehicleDifferentialSolver::CalculateTransferCaseAxleOutput(Config, -300.f,
			ReverseLockedAngularVelocity, -10.f, 2.f, 1.f, 4.f, 0.01f);
	const FVehicleTransferCaseAxleOutput ReverseAxle2 =
		FVehicleDifferentialSolver::CalculateTransferCaseAxleOutput(Config, -300.f,
			ReverseLockedAngularVelocity, -20.f, 6.f, 3.f, 4.f, 0.01f);
	TestTrue(TEXT("Reverse motion with reverse torque selects drive lock"),
		FMath::IsNearlyEqual(ReverseAxle1.DriveTorque, -825.f)
		&& FMath::IsNearlyEqual(ReverseAxle2.DriveTorque, 525.f));

	const FVehicleTransferCaseAxleOutput ZeroStepAxle1 =
		FVehicleDifferentialSolver::CalculateTransferCaseAxleOutput(Config, 300.f,
			LockedAngularVelocity, 10.f, 2.f, 1.f, 4.f, 0.f);
	const FVehicleTransferCaseAxleOutput ZeroStepAxle2 =
		FVehicleDifferentialSolver::CalculateTransferCaseAxleOutput(Config, 300.f,
			LockedAngularVelocity, 20.f, 6.f, 3.f, 4.f, 0.f);
	TestTrue(TEXT("Zero step SafeDivide falls back to static 75/225 split"),
		FMath::IsNearlyEqual(ZeroStepAxle1.DriveTorque, 75.f)
		&& FMath::IsNearlyEqual(ZeroStepAxle2.DriveTorque, 225.f));

	TestEqual(TEXT("Zero total inertia returns zero locked velocity"),
		FVehicleDifferentialSolver::CalculateLockedAngularVelocity(140.f, 0.f), 0.f);
	const FVehicleTransferCaseAxleOutput ZeroWeight =
		FVehicleDifferentialSolver::CalculateTransferCaseAxleOutput(Config, 300.f,
			0.f, 10.f, 0.f, 1.f, 0.f, 0.f);
	TestEqual(TEXT("Zero total weight returns zero normalized weight"), ZeroWeight.NormalizedTorqueWeight, 0.f);
	TestEqual(TEXT("Zero dt and zero inertia return zero axle torque"), ZeroWeight.DriveTorque, 0.f);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
