// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "VehicleAxleAssemblyComponent.h"
#include "VehicleDifferentialComponent.h"
#include "VehicleAxleSolver.h"
#include "VehicleWheelRuntime.h"
#include "VehicleWheelSolver.h"
#include "VehicleSuspensionSolver.h"
#include "VehicleWheelComponent.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/AutomationTest.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "UObject/StrongObjectPtr.h"

struct FVehicleAxleRuntimeProbe
{
	static void ConfigureWheel(UVehicleWheelComponent& Wheel, const bool bOnGround,
		const float InitialOmega, const float PredictedSlip)
	{
		Wheel.WheelConfig.Radius = 34.f;
		Wheel.WheelConfig.Width = 22.f;
		Wheel.WheelConfig.Inertia = 1.4f;
		Wheel.WheelConfig.RollingRisistance = 7.f;
		Wheel.TireConfig.FrictionMultiplier = 1.2f;
		Wheel.TireConfig.RelaxationLength = FVector2f(0.16f, 0.27f);
		Wheel.TireConfig.MaxFx = 1.15f;
		Wheel.TireConfig.MaxFy = 1.05f;
		Wheel.ABSConfig.bAntiBrakeSystemEnabled = false;
		FVehicleWheelSolver::CacheLUTs(Wheel.CachedTireLUTs, Wheel.TireConfig);
		FVehicleSuspensionSolver::CacheLUTs(Wheel.CachedSuspensionLUTs, Wheel.SuspensionKinematicsConfig);
		Wheel.SubmitWheelRuntimeSettings();

		FVehicleSuspensionSimState Suspension;
		Suspension.bHitGround = bOnGround;
		Suspension.bWheelOnGround = bOnGround;
		Suspension.StaticSprungMass = 300.f;
		Suspension.EffectiveSprungMassNormal = 300.f;
		Suspension.EffectiveSprungMassLong = 300.f;
		Suspension.EffectiveSprungMassLat = 300.f;
		Suspension.ForceAlongImpactNormal = bOnGround ? 4200.f : 0.f;
		Suspension.ImpactFriction = 0.75f;
		Suspension.ImpactWorldNormal = FVector3f::UpVector;
		Suspension.WheelWorldRightVector = FVector3f::RightVector;
		Suspension.ImpactWorldVelocity = FVector3f(12.f, 0.6f, 0.f);
		Wheel.SubmitWheelEnvironment();
		Wheel.GetWheelBridge()->ConsumeInputs();
		auto& Runtime = Wheel.GetWheelBridge()->GetRuntimeForSimulation();
		Runtime.SuspensionState = Suspension;
		Runtime.WheelState = FVehicleWheelSimState();
		Runtime.WheelState.AngularVelocity = InitialOmega;
		Runtime.WheelState.EffectiveInertia = Wheel.WheelConfig.Inertia;
		Runtime.WheelState.PredictedSlipRatio = PredictedSlip;
	}

	static void Connect(UVehicleAxleAssemblyComponent& Axle,
		UVehicleWheelComponent* Left, UVehicleWheelComponent* Right,
		UVehicleDifferentialComponent* Differential, EVehicleAxleLayout Layout,
		float TorqueWeight, float P3MotorTorque, bool bTcsEnabled)
	{
		Axle.LeftWheel = Left;
		Axle.RightWheel = Right;
		Axle.Differential = Differential;
		Axle.AxleLayout = Layout;
		Axle.AxleConfig.DriveShaftInertia = 0.35f;
		Axle.AxleConfig.DriveShaftTorsionalStiffness = 5200.f;
		Axle.AxleConfig.TorqueWeight = TorqueWeight;
		Axle.AxleConfig.MaxBrakeTorque = 900.f;
		Axle.AxleConfig.MaxHandbrakeTorque = 650.f;
		Axle.AxleConfig.bAffectedByHandbrake = true;
		Axle.TCSConfig.bTractionControlSystemEnabled = bTcsEnabled;
		Axle.TCSConfig.OptimalSlip = 0.2f;
		Axle.TCSConfig.ActivationSpeed = 1.f;
		Axle.TCSConfig.Sensitivity = 0.4f;
		Axle.State = FVehicleAxleSimState();
		Axle.State.AxleAngularVelocity = 11.f;
		Axle.State.AxleEffectiveInertia = 0.35f;
		Axle.State.P3MotorTorque = P3MotorTorque;
		Axle.SubmitRuntimeSettings();
		Axle.GetAxleBridge()->ConsumeInputs();
		Axle.GetAxleBridge()->GetRuntimeForSimulation().State = Axle.State;
	}

	static void PrimeForMacroStep(UVehicleAxleAssemblyComponent& Axle,
		UVehicleWheelComponent* Left, UVehicleWheelComponent* Right,
		const float MacroDt, const bool bOnGround, const float PredictedSlip)
	{
		if (Left)
		{
			auto& Runtime = Left->GetWheelBridge()->GetRuntimeForSimulation();
			Runtime.SuspensionState.bWheelOnGround = bOnGround;
			Runtime.SuspensionState.ForceAlongImpactNormal = bOnGround ? 4200.f : 0.f;
			Runtime.PrepareWheel(MacroDt, FTransform::Identity);
			Runtime.WheelState.PredictedSlipRatio = PredictedSlip;
		}
		if (Right)
		{
			auto& Runtime = Right->GetWheelBridge()->GetRuntimeForSimulation();
			Runtime.SuspensionState.bWheelOnGround = bOnGround;
			Runtime.SuspensionState.ForceAlongImpactNormal = bOnGround ? 4200.f : 0.f;
			Runtime.PrepareWheel(MacroDt, FTransform::Identity);
			Runtime.WheelState.PredictedSlipRatio = PredictedSlip;
		}
		auto& RuntimeState = Axle.GetAxleBridge()->GetRuntimeForSimulation().State;
		RuntimeState.NumOfWheels = static_cast<int32>(Left != nullptr) + static_cast<int32>(Right != nullptr);
		RuntimeState.NumOfWheelOnGround = bOnGround ? RuntimeState.NumOfWheels : 0;
		RuntimeState.LocalLinearVelocity = FVector3f(15.f, 0.f, 0.f);
	}

	static const FVehicleAxleSimState& GetState(const UVehicleAxleAssemblyComponent& Axle)
	{
		return Axle.GetAxleBridge()->GetRuntimeForSimulation().State;
	}
	static void SetInitialTcsFlag(UVehicleAxleAssemblyComponent& Axle) { Axle.GetAxleBridge()->GetRuntimeForSimulation().State.bTCSTriggered = true; }

	static FVehicleWheelSimState GetWheelState(const UVehicleWheelComponent* Wheel)
	{
		return Wheel ? Wheel->GetWheelBridge()->GetRuntimeForSimulation().WheelState : FVehicleWheelSimState();
	}
	static FVehicleSuspensionSimState GetSuspensionState(const UVehicleWheelComponent* Wheel)
	{
		return Wheel ? Wheel->GetWheelBridge()->GetRuntimeForSimulation().SuspensionState : FVehicleSuspensionSimState();
	}
	static FVehicleAxleConfig GetAxleConfig(const UVehicleAxleAssemblyComponent& Axle) { return Axle.AxleConfig; }
	static FVehicleTCSConfig GetTcsConfig(const UVehicleAxleAssemblyComponent& Axle) { return Axle.TCSConfig; }
	static EVehicleAxleLayout GetLayout(const UVehicleAxleAssemblyComponent& Axle) { return Axle.AxleLayout; }
};

namespace VehicleAxleRuntimeTest
{
	struct FScenario
	{
		const TCHAR* Name;
		bool bLeftWheel;
		bool bRightWheel;
		bool bHasDifferential;
		EVehicleAxleLayout Layout;
		float TorqueWeight;
		float P3Torque;
		bool bGrounded;
		bool bTcsEnabled;
		float PredictedSlip;
		float LongitudinalVelocity;
	};

	struct FCapture
	{
		float AxleTorque;
		float AxleInertia;
		float AxleOmega;
		float ShaftStiffness;
		float LeftOmega;
		float RightOmega;
		float LeftInertia;
		float RightInertia;
		bool bTcs;
	};

	FCapture Capture(const FVehicleAxleSimState& State,
		const UVehicleWheelComponent* Left, const UVehicleWheelComponent* Right,
		float OutInertia, float OutOmega, float OutStiffness)
	{
		const FVehicleWheelSimState LeftState = FVehicleAxleRuntimeProbe::GetWheelState(Left);
		const FVehicleWheelSimState RightState = FVehicleAxleRuntimeProbe::GetWheelState(Right);
		return { State.AxleDriveTorque, OutInertia, OutOmega, OutStiffness,
			LeftState.AngularVelocity,
			RightState.AngularVelocity,
			LeftState.EffectiveInertia,
			RightState.EffectiveInertia,
			State.bTCSTriggered };
	}

	bool IsFinite(const FCapture& C)
	{
		return FMath::IsFinite(C.AxleTorque) && FMath::IsFinite(C.AxleInertia)
			&& FMath::IsFinite(C.AxleOmega) && FMath::IsFinite(C.ShaftStiffness)
			&& FMath::IsFinite(C.LeftOmega) && FMath::IsFinite(C.RightOmega)
			&& FMath::IsFinite(C.LeftInertia) && FMath::IsFinite(C.RightInertia);
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleAxleRuntimeLegacyCaptureTest,
	"KinetiForge.AxleRuntime.LegacyCapture",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleAxleRuntimeLegacyCaptureTest::RunTest(const FString& Parameters)
{
	using namespace VehicleAxleRuntimeTest;
	(void)Parameters;
	const FScenario Scenarios[] = {
		{ TEXT("dual_tcs_ground"), true, true, true, EVehicleAxleLayout::TwoWheels, 1.f, 0.f, true, true, 0.9f, 15.f },
		{ TEXT("dual_tcs_disabled"), true, true, true, EVehicleAxleLayout::TwoWheels, 1.f, 0.f, true, false, 0.9f, 15.f },
		{ TEXT("dual_airborne"), true, true, true, EVehicleAxleLayout::TwoWheels, 1.f, 0.f, false, true, 0.9f, 15.f },
		{ TEXT("dual_reverse_drive"), true, true, true, EVehicleAxleLayout::TwoWheels, 1.f, 0.f, true, false, 0.f, 15.f },
		{ TEXT("single_left_open_half"), true, false, true, EVehicleAxleLayout::TwoWheels, 1.f, 0.f, true, false, 0.f, 15.f },
		{ TEXT("single_right_full_torque"), false, true, true, EVehicleAxleLayout::SingleRight, 1.f, 0.f, true, false, 0.f, 15.f },
		{ TEXT("missing_diff"), true, true, false, EVehicleAxleLayout::TwoWheels, 1.f, 0.f, true, true, 0.f, 15.f },
		{ TEXT("p3_powered_zero_weight"), true, true, true, EVehicleAxleLayout::TwoWheels, 0.f, 35.f, true, false, 0.f, 15.f },
		{ TEXT("nonpowered_axle"), true, true, true, EVehicleAxleLayout::TwoWheels, 0.f, 0.f, true, false, 0.f, 15.f },
		{ TEXT("no_wheels"), false, false, true, EVehicleAxleLayout::TwoWheels, 1.f, 0.f, false, false, 0.f, 15.f },
	};
	const float MacroDts[] = { 1.f / 60.f, 1.f / 120.f, 1.f / 240.f };
	FString Csv = TEXT("scenario,macro_dt,macro_step,axle_torque,axle_inertia,axle_omega,shaft_stiffness,left_omega,right_omega,left_effective_inertia,right_effective_inertia,tcs\n");
	int32 CaptureCount = 0;
	int32 TcsTriggeredSteps = 0;
	int32 DisabledTcsTriggeredSteps = 0;
	int32 DualTorqueSteps = 0;
	int32 OpenHalfSteps = 0;
	int32 SingleRightSteps = 0;
	int32 RetainedTcsFlagSteps = 0;

	for (const FScenario& Scenario : Scenarios)
	{
		for (const float MacroDt : MacroDts)
		{
			TStrongObjectPtr<UVehicleWheelComponent> Left(Scenario.bLeftWheel ? NewObject<UVehicleWheelComponent>() : nullptr);
			TStrongObjectPtr<UVehicleWheelComponent> Right(Scenario.bRightWheel ? NewObject<UVehicleWheelComponent>() : nullptr);
			TStrongObjectPtr<UVehicleAxleAssemblyComponent> Axle(NewObject<UVehicleAxleAssemblyComponent>());
			TStrongObjectPtr<UVehicleDifferentialComponent> Differential(
				Scenario.bHasDifferential ? NewObject<UVehicleDifferentialComponent>() : nullptr);
			if (!TestNotNull(TEXT("Axle object created"), Axle.Get())) return false;
			if (Scenario.bLeftWheel && !TestNotNull(TEXT("Left wheel created"), Left.Get())) return false;
			if (Scenario.bRightWheel && !TestNotNull(TEXT("Right wheel created"), Right.Get())) return false;
			if (Scenario.bHasDifferential && !TestNotNull(TEXT("Differential created"), Differential.Get())) return false;

			if (Left) FVehicleAxleRuntimeProbe::ConfigureWheel(*Left, Scenario.bGrounded, 5.f, Scenario.PredictedSlip);
			if (Right) FVehicleAxleRuntimeProbe::ConfigureWheel(*Right, Scenario.bGrounded, 8.f, Scenario.PredictedSlip);
			if (Differential)
			{
				FVehicleLimitedSlipDifferentialConfig DiffConfig;
				DiffConfig.GearRatio = 3.f;
				DiffConfig.DriveLockRatio = 0.6f;
				DiffConfig.CoastLockRatio = 0.35f;
				Differential->SetConfig(DiffConfig);
			}
			FVehicleAxleRuntimeProbe::Connect(*Axle, Left.Get(), Right.Get(), Differential.Get(),
				Scenario.Layout, Scenario.TorqueWeight, Scenario.P3Torque, Scenario.bTcsEnabled);
			if (FCString::Strcmp(Scenario.Name, TEXT("missing_diff")) == 0
				|| FCString::Strcmp(Scenario.Name, TEXT("nonpowered_axle")) == 0)
			{
				FVehicleAxleRuntimeProbe::SetInitialTcsFlag(*Axle);
			}

			int32 ScenarioTcsSteps = 0;
			for (int32 MacroStep = 0; MacroStep < 4; ++MacroStep)
			{
				const bool bGrounded = Scenario.bGrounded;
				FVehicleAxleRuntimeProbe::PrimeForMacroStep(*Axle, Left.Get(), Right.Get(),
					MacroDt, bGrounded, Scenario.PredictedSlip);
				const float DriveSign = FCString::Strcmp(Scenario.Name, TEXT("dual_reverse_drive")) == 0 ? -1.f : 1.f;
				const float DriveTorque = DriveSign * (480.f + 40.f * MacroStep);
				float OutInertia = 0.f;
				float OutOmega = 0.f;
				float OutStiffness = 0.f;
				Axle->SubstepAxle(MacroDt, DriveTorque, 0.25f, 0.2f,
					OutInertia, OutOmega, OutStiffness);
				const FCapture C = Capture(FVehicleAxleRuntimeProbe::GetState(*Axle),
					Left.Get(), Right.Get(), OutInertia, OutOmega, OutStiffness);
				TestTrue(FString::Printf(TEXT("%s finite at %d Hz step %d"), Scenario.Name,
					FMath::RoundToInt(1.f / MacroDt), MacroStep), IsFinite(C));
				CaptureCount++;
				ScenarioTcsSteps += C.bTcs ? 1 : 0;
				TcsTriggeredSteps += C.bTcs ? 1 : 0;
				DisabledTcsTriggeredSteps += (FCString::Strcmp(Scenario.Name, TEXT("dual_tcs_disabled")) == 0 && C.bTcs) ? 1 : 0;
				DualTorqueSteps += (FCString::Strcmp(Scenario.Name, TEXT("dual_tcs_ground")) == 0
					&& FMath::Abs(C.AxleTorque) > 0.f) ? 1 : 0;
				OpenHalfSteps += (FCString::Strcmp(Scenario.Name, TEXT("single_left_open_half")) == 0
					&& Left && FMath::IsFinite(C.LeftOmega)) ? 1 : 0;
				SingleRightSteps += (FCString::Strcmp(Scenario.Name, TEXT("single_right_full_torque")) == 0
					&& Right && FMath::IsFinite(C.RightOmega)) ? 1 : 0;
				RetainedTcsFlagSteps += ((FCString::Strcmp(Scenario.Name, TEXT("missing_diff")) == 0
					|| FCString::Strcmp(Scenario.Name, TEXT("nonpowered_axle")) == 0) && C.bTcs) ? 1 : 0;
				Csv += FString::Printf(TEXT("%s,%.9g,%d,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%d\n"),
					Scenario.Name, MacroDt, MacroStep, C.AxleTorque, C.AxleInertia, C.AxleOmega,
					C.ShaftStiffness, C.LeftOmega, C.RightOmega, C.LeftInertia, C.RightInertia,
					C.bTcs ? 1 : 0);
			}
			if (FCString::Strcmp(Scenario.Name, TEXT("dual_tcs_ground")) == 0)
			{
				TestTrue(TEXT("Grounded dual-wheel case exercises TCS"), ScenarioTcsSteps > 0);
			}
		}
	}

	TestEqual(TEXT("All axle adapter captures emitted"), CaptureCount, 120);
	TestTrue(TEXT("At least one grounded dual-wheel TCS step triggers"), TcsTriggeredSteps > 0);
	TestEqual(TEXT("Disabled TCS stays disabled"), DisabledTcsTriggeredSteps, 0);
	TestTrue(TEXT("Driven dual-wheel path exercised"), DualTorqueSteps > 0);
	TestEqual(TEXT("Single-left open-diff path captured"), OpenHalfSteps, 12);
	TestEqual(TEXT("Single-right full-torque path captured"), SingleRightSteps, 12);
	TestEqual(TEXT("Missing-differential/nonpowered branches retain an existing TCS flag"), RetainedTcsFlagSteps, 24);

	const FString OutputDir = FPaths::ProjectSavedDir() / TEXT("Automation/KinetiForge/AxleRuntime");
	IFileManager::Get().MakeDirectory(*OutputDir, true);
	const FString OutputPath = OutputDir / TEXT("axle.csv");
	TestTrue(TEXT("Legacy axle capture CSV saved"), FFileHelper::SaveStringToFile(Csv, *OutputPath));
	AddInfo(FString::Printf(TEXT("Axle adapter capture saved to %s"), *OutputPath));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleAxleRuntimeOwnedCompatibilityTest,
	"KinetiForge.AxleRuntime.OwnedCompatibility",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleAxleRuntimeOwnedCompatibilityTest::RunTest(const FString& Parameters)
{
	using namespace VehicleAxleRuntimeTest;
	(void)Parameters;
	struct FPair
	{
		TStrongObjectPtr<UVehicleWheelComponent> Left;
		TStrongObjectPtr<UVehicleWheelComponent> Right;
		TStrongObjectPtr<UVehicleAxleAssemblyComponent> Axle;
		TStrongObjectPtr<UVehicleDifferentialComponent> Diff;
		FVehicleWheelRuntime LeftRuntime;
		FVehicleWheelRuntime RightRuntime;
		FVehicleAxleSimState RuntimeState;
		FVehicleAxleConfig AxleConfig;
		FVehicleTCSConfig TcsConfig;
		FVehicleLimitedSlipDifferentialConfig DiffConfig;
		EVehicleAxleLayout Layout = EVehicleAxleLayout::TwoWheels;
		bool bHasDiff = false;
	};
	const FScenario Scenarios[] = {
		{ TEXT("dual"), true, true, true, EVehicleAxleLayout::TwoWheels, 1.f, 0.f, true, true, 0.9f, 15.f },
		{ TEXT("single_left_open_half"), true, false, true, EVehicleAxleLayout::TwoWheels, 1.f, 0.f, true, false, 0.f, 15.f },
		{ TEXT("single_right_full"), false, true, true, EVehicleAxleLayout::SingleRight, 1.f, 0.f, true, false, 0.f, 15.f },
		{ TEXT("missing_diff"), true, true, false, EVehicleAxleLayout::TwoWheels, 1.f, 0.f, true, false, 0.f, 15.f },
		{ TEXT("zero_weight_p3"), true, true, true, EVehicleAxleLayout::TwoWheels, 0.f, 35.f, true, false, 0.f, 15.f },
		{ TEXT("no_wheels"), false, false, true, EVehicleAxleLayout::TwoWheels, 1.f, 0.f, false, false, 0.f, 15.f },
	};
	TArray<FPair> Pairs;
	Pairs.Reserve(UE_ARRAY_COUNT(Scenarios));
	for (const FScenario& Scenario : Scenarios)
	{
		FPair Pair;
		Pair.Left.Reset(Scenario.bLeftWheel ? NewObject<UVehicleWheelComponent>() : nullptr);
		Pair.Right.Reset(Scenario.bRightWheel ? NewObject<UVehicleWheelComponent>() : nullptr);
		Pair.Axle.Reset(NewObject<UVehicleAxleAssemblyComponent>());
		Pair.Diff.Reset(Scenario.bHasDifferential ? NewObject<UVehicleDifferentialComponent>() : nullptr);
		if (!Pair.Axle.IsValid() || (Scenario.bLeftWheel && !Pair.Left.IsValid())
			|| (Scenario.bRightWheel && !Pair.Right.IsValid()) || (Scenario.bHasDifferential && !Pair.Diff.IsValid()))
		{
			AddError(FString::Printf(TEXT("Could not create fixture for %s"), Scenario.Name));
			return false;
		}
		if (Pair.Left) FVehicleAxleRuntimeProbe::ConfigureWheel(*Pair.Left, Scenario.bGrounded, 5.f, Scenario.PredictedSlip);
		if (Pair.Right) FVehicleAxleRuntimeProbe::ConfigureWheel(*Pair.Right, Scenario.bGrounded, 8.f, Scenario.PredictedSlip);
		if (Pair.Diff)
		{
			Pair.DiffConfig.GearRatio = 3.f;
			Pair.DiffConfig.DriveLockRatio = 0.6f;
			Pair.DiffConfig.CoastLockRatio = 0.35f;
			Pair.Diff->SetConfig(Pair.DiffConfig);
		}
		FVehicleAxleRuntimeProbe::Connect(*Pair.Axle, Pair.Left.Get(), Pair.Right.Get(), Pair.Diff.Get(),
			Scenario.Layout, Scenario.TorqueWeight, Scenario.P3Torque, Scenario.bTcsEnabled);
		Pair.AxleConfig = FVehicleAxleRuntimeProbe::GetAxleConfig(*Pair.Axle);
		Pair.TcsConfig = FVehicleAxleRuntimeProbe::GetTcsConfig(*Pair.Axle);
		Pair.Layout = FVehicleAxleRuntimeProbe::GetLayout(*Pair.Axle);
		Pair.bHasDiff = Pair.Diff.IsValid();
		Pair.RuntimeState = FVehicleAxleRuntimeProbe::GetState(*Pair.Axle);
		if (Pair.Left)
		{
			Pair.LeftRuntime.Settings = Pair.Left->CaptureRuntimeSettings();
			Pair.LeftRuntime.WheelState = FVehicleAxleRuntimeProbe::GetWheelState(Pair.Left.Get());
			Pair.LeftRuntime.SuspensionState = FVehicleAxleRuntimeProbe::GetSuspensionState(Pair.Left.Get());
		}
		if (Pair.Right)
		{
			Pair.RightRuntime.Settings = Pair.Right->CaptureRuntimeSettings();
			Pair.RightRuntime.WheelState = FVehicleAxleRuntimeProbe::GetWheelState(Pair.Right.Get());
			Pair.RightRuntime.SuspensionState = FVehicleAxleRuntimeProbe::GetSuspensionState(Pair.Right.Get());
		}
		Pairs.Add(MoveTemp(Pair));
	}

	const float Dt = 1.f / 120.f;
	for (int32 Step = 0; Step < 8; ++Step)
	{
		// Interleave different owned states so no fixture can inherit another axle's feedback.
		for (int32 PairVisit = 0; PairVisit < Pairs.Num(); ++PairVisit)
		{
			const int32 PairIndex = (Step + PairVisit) % Pairs.Num();
			FPair& Pair = Pairs[PairIndex];
			const FScenario& Scenario = Scenarios[PairIndex];
			FVehicleAxleRuntimeProbe::PrimeForMacroStep(*Pair.Axle, Pair.Left.Get(), Pair.Right.Get(),
				Dt, Scenario.bGrounded, Scenario.PredictedSlip);
			if (Pair.Left) Pair.LeftRuntime.PrepareWheel(Dt, FTransform::Identity);
			if (Pair.Right) Pair.RightRuntime.PrepareWheel(Dt, FTransform::Identity);
			// Match the fixture's explicit slip injection on the component path.
			if (Pair.Left) Pair.LeftRuntime.WheelState.PredictedSlipRatio = Scenario.PredictedSlip;
			if (Pair.Right) Pair.RightRuntime.WheelState.PredictedSlipRatio = Scenario.PredictedSlip;
			Pair.RuntimeState.NumOfWheels = (Pair.Left ? 1 : 0) + (Pair.Right ? 1 : 0);
			Pair.RuntimeState.NumOfWheelOnGround = Scenario.bGrounded ? Pair.RuntimeState.NumOfWheels : 0;
			Pair.RuntimeState.LocalLinearVelocity = FVector3f(15.f, 0.f, 0.f);
			const FVehicleAxleWheelInput PreLeft = Pair.Left
				? FVehicleAxleWheelInput{Pair.LeftRuntime.WheelState.AngularVelocity, Pair.LeftRuntime.WheelState.EffectiveInertia,
					Pair.LeftRuntime.Settings.WheelConfig.Inertia, Pair.LeftRuntime.WheelState.PredictedSlipRatio} : FVehicleAxleWheelInput();
			const FVehicleAxleWheelInput PreRight = Pair.Right
				? FVehicleAxleWheelInput{Pair.RightRuntime.WheelState.AngularVelocity, Pair.RightRuntime.WheelState.EffectiveInertia,
					Pair.RightRuntime.Settings.WheelConfig.Inertia, Pair.RightRuntime.WheelState.PredictedSlipRatio} : FVehicleAxleWheelInput();
			const float DriveTorque = 480.f + 40.f * Step;
			const bool bNotDriven = FVehicleAxleSolver::PrepareTransmissionStep(Pair.RuntimeState,
				Pair.AxleConfig, Pair.TcsConfig, Pair.Layout, Pair.bHasDiff ? &Pair.DiffConfig : nullptr,
				Pair.Left ? &PreLeft : nullptr, Pair.Right ? &PreRight : nullptr,
				Dt, DriveTorque, 0.25f, 0.2f);
			if (Pair.Left) Pair.LeftRuntime.IntegrateWheel(Dt, Pair.RuntimeState.LeftDriveTorque,
				Pair.RuntimeState.BrakeTorque, Pair.RuntimeState.HandbrakeTorque, Pair.RuntimeState.ReflectedInertiaOnWheel);
			if (Pair.Right) Pair.RightRuntime.IntegrateWheel(Dt, Pair.RuntimeState.RightDriveTorque,
				Pair.RuntimeState.BrakeTorque, Pair.RuntimeState.HandbrakeTorque, Pair.RuntimeState.ReflectedInertiaOnWheel);
			const FVehicleAxleWheelInput PostLeft = Pair.Left
				? FVehicleAxleWheelInput{Pair.LeftRuntime.WheelState.AngularVelocity, Pair.LeftRuntime.WheelState.EffectiveInertia,
					Pair.LeftRuntime.Settings.WheelConfig.Inertia, Pair.LeftRuntime.WheelState.PredictedSlipRatio} : FVehicleAxleWheelInput();
			const FVehicleAxleWheelInput PostRight = Pair.Right
				? FVehicleAxleWheelInput{Pair.RightRuntime.WheelState.AngularVelocity, Pair.RightRuntime.WheelState.EffectiveInertia,
					Pair.RightRuntime.Settings.WheelConfig.Inertia, Pair.RightRuntime.WheelState.PredictedSlipRatio} : FVehicleAxleWheelInput();
			const float RuntimeStiffness = FVehicleAxleSolver::FinalizeTransmissionStep(Pair.RuntimeState,
				Pair.AxleConfig, Pair.bHasDiff ? &Pair.DiffConfig : nullptr,
				Pair.Left ? &PostLeft : nullptr, Pair.Right ? &PostRight : nullptr, bNotDriven);

			float LegacyInertia = 0.f, LegacyOmega = 0.f, LegacyStiffness = 0.f;
			Pair.Axle->SubstepAxle(Dt, DriveTorque, 0.25f, 0.2f, LegacyInertia, LegacyOmega, LegacyStiffness);
			const FVehicleAxleSimState& LegacyState = FVehicleAxleRuntimeProbe::GetState(*Pair.Axle);
			const auto Near = [this, &Scenario, Step](const TCHAR* Label, const float A, const float B)
			{
				TestTrue(FString::Printf(TEXT("%s step %d: %s"), Scenario.Name, Step, Label),
					FMath::IsNearlyEqual(A, B, 1.e-4f));
			};
			Near(TEXT("axle torque matches adapter"), Pair.RuntimeState.AxleDriveTorque, LegacyState.AxleDriveTorque);
			Near(TEXT("axle omega matches adapter"), Pair.RuntimeState.AxleAngularVelocity, LegacyOmega);
			Near(TEXT("axle inertia matches adapter"), Pair.RuntimeState.AxleEffectiveInertia, LegacyInertia);
			Near(TEXT("shaft stiffness matches adapter"), RuntimeStiffness, LegacyStiffness);
			Near(TEXT("left torque matches adapter"), Pair.RuntimeState.LeftDriveTorque, LegacyState.LeftDriveTorque);
			Near(TEXT("right torque matches adapter"), Pair.RuntimeState.RightDriveTorque, LegacyState.RightDriveTorque);
			Near(TEXT("per-wheel reflected inertia matches adapter"), Pair.RuntimeState.ReflectedInertiaOnWheel, LegacyState.ReflectedInertiaOnWheel);
			Near(TEXT("brake torque matches adapter"), Pair.RuntimeState.BrakeTorque, LegacyState.BrakeTorque);
			Near(TEXT("handbrake torque matches adapter"), Pair.RuntimeState.HandbrakeTorque, LegacyState.HandbrakeTorque);
			TestEqual(TEXT("TCS state matches adapter"), Pair.RuntimeState.bTCSTriggered, LegacyState.bTCSTriggered);
			if (Pair.Left)
			{
				const FVehicleWheelSimState LegacyWheel = FVehicleAxleRuntimeProbe::GetWheelState(Pair.Left.Get());
				Near(TEXT("left wheel omega matches adapter"), Pair.LeftRuntime.WheelState.AngularVelocity, LegacyWheel.AngularVelocity);
				Near(TEXT("left wheel inertia matches adapter"), Pair.LeftRuntime.WheelState.EffectiveInertia, LegacyWheel.EffectiveInertia);
				Near(TEXT("left wheel torque matches adapter"), Pair.LeftRuntime.WheelState.DriveTorque, LegacyWheel.DriveTorque);
			}
			if (Pair.Right)
			{
				const FVehicleWheelSimState LegacyWheel = FVehicleAxleRuntimeProbe::GetWheelState(Pair.Right.Get());
				Near(TEXT("right wheel omega matches adapter"), Pair.RightRuntime.WheelState.AngularVelocity, LegacyWheel.AngularVelocity);
				Near(TEXT("right wheel inertia matches adapter"), Pair.RightRuntime.WheelState.EffectiveInertia, LegacyWheel.EffectiveInertia);
				Near(TEXT("right wheel torque matches adapter"), Pair.RightRuntime.WheelState.DriveTorque, LegacyWheel.DriveTorque);
			}
		}
	}

	// Value-stage semantics: Finalize consumes current post-wheel inputs, and a missing wheel
	// in an open two-wheel layout receives half torque while a single-right layout gets full.
	FVehicleAxleConfig Config;
	Config.DriveShaftInertia = 0.35f;
	Config.DriveShaftTorsionalStiffness = 5200.f;
	Config.TorqueWeight = 1.f;
	FVehicleTCSConfig Tcs;
	Tcs.bTractionControlSystemEnabled = false;
	FVehicleLimitedSlipDifferentialConfig Diff;
	Diff.GearRatio = 3.f;
	FVehicleAxleWheelInput WheelIn{5.f, 1.4f, 1.4f, 0.f};
	FVehicleAxleSimState OpenState;
	OpenState.NumOfWheels = 1;
	OpenState.NumOfWheelOnGround = 1;
	FVehicleAxleSolver::PrepareTransmissionStep(OpenState, Config, Tcs, EVehicleAxleLayout::TwoWheels,
		&Diff, &WheelIn, nullptr, 1.f / 120.f, 100.f, 0.f, 0.f);
	const float OpenTorque = OpenState.LeftDriveTorque;
	FVehicleAxleSimState SingleState;
	SingleState.NumOfWheels = 1;
	SingleState.NumOfWheelOnGround = 1;
	FVehicleAxleSolver::PrepareTransmissionStep(SingleState, Config, Tcs, EVehicleAxleLayout::SingleRight,
		&Diff, nullptr, &WheelIn, 1.f / 120.f, 100.f, 0.f, 0.f);
	TestTrue(TEXT("Single-right layout allocates more torque than open half-torque"), SingleState.RightDriveTorque > OpenTorque);
	FVehicleAxleSimState FeedbackA;
	FVehicleAxleSimState FeedbackB;
	FeedbackA.NumOfWheels = FeedbackB.NumOfWheels = 1;
	FVehicleAxleWheelInput FeedbackInputA{3.f, 1.4f, 1.4f, 0.f};
	FVehicleAxleWheelInput FeedbackInputB{9.f, 1.4f, 1.4f, 0.f};
	FVehicleAxleSolver::FinalizeTransmissionStep(FeedbackA, Config, &Diff, &FeedbackInputA, nullptr, false);
	FVehicleAxleSolver::FinalizeTransmissionStep(FeedbackB, Config, &Diff, &FeedbackInputB, nullptr, false);
	TestTrue(TEXT("Finalize feedback observes the current post-wheel angular velocity"),
		!FMath::IsNearlyEqual(FeedbackA.AxleAngularVelocity, FeedbackB.AxleAngularVelocity));
	FVehicleAxleSimState NoDriveState;
	FVehicleAxleSolver::PrepareTransmissionStep(NoDriveState, Config, Tcs, EVehicleAxleLayout::TwoWheels,
		nullptr, nullptr, nullptr, 1.f / 120.f, 100.f, 0.f, 0.f);
	const float NoDriveStiffness = FVehicleAxleSolver::FinalizeTransmissionStep(NoDriveState, Config,
		nullptr, nullptr, nullptr, true);
	TestEqual(TEXT("Undriven path has zero shaft stiffness"), NoDriveStiffness, 0.f);

	FVehicleWheelRuntime SettingsRuntime;
	SettingsRuntime.WheelState.AngularVelocity = 23.f;
	SettingsRuntime.WheelState.P4MotorTorque = 7.f;
	SettingsRuntime.SuspensionState.StaticSprungMass = 80.f;
	SettingsRuntime.WheelContext.MacroDeltaTime = 0.02f;
	FVehicleWheelRuntimeSettings UpdatedSettings;
	UpdatedSettings.WheelConfig.Radius = 51.f;
	SettingsRuntime.ApplySettings(UpdatedSettings);
	TestEqual(TEXT("Applying axle-owned wheel settings preserves caller omega"), SettingsRuntime.WheelState.AngularVelocity, 23.f);
	TestEqual(TEXT("Applying axle-owned wheel settings preserves caller P4 command"), SettingsRuntime.WheelState.P4MotorTorque, 7.f);
	TestEqual(TEXT("Applying axle-owned wheel settings preserves suspension state"), SettingsRuntime.SuspensionState.StaticSprungMass, 80.f);
	TestEqual(TEXT("Applying axle-owned wheel settings preserves wheel context"), SettingsRuntime.WheelContext.MacroDeltaTime, 0.02f);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
