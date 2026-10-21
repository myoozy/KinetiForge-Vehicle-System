// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehicleEngineComponent.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "HAL/FileManager.h"
#include "Misc/AutomationTest.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "UObject/StrongObjectPtr.h"

// Calls production code on an unregistered transient component. No world, asset
// edits, copied simulation equations, or public state setters are required.
struct FVehicleEngineLegacyProbe
{
	static FVehicleEngineSimState& State(UVehicleEngineComponent& Engine) { return Engine.State; }
	static void ExhaustStep(UVehicleEngineComponent& Engine, float Dt) { Engine.UpdateExhaust(Dt); }
	static void PinRPM(UVehicleEngineComponent& Engine, float RPM)
	{
		Engine.State.EngineRPM = RPM;
		Engine.State.EngineAngularVelocity = RPM * PI / 30.f;
	}
};

namespace VehicleEngineLegacyBench
{
	constexpr float ReferenceRPM = 3500.f;
	constexpr float ReferenceHeat = ReferenceRPM / 6000.f;

	static void Configure(UVehicleEngineComponent& Engine)
	{
		// Freeze protocol-v1 values so future defaults cannot silently change the bench.
		FVehicleNaturallyAspiratedEngineConfig NA;
		NA.MaxEngineTorque = 400.f;
		NA.EngineTorqueCurve = nullptr; // Explicit flat LUT; independent of packaged curves.
		NA.EngineIdleRPM = 900.f;
		NA.EngineMaxRPM = 6000.f;
		NA.EngineInertia = 0.2f;
		NA.StartFriction = 50.f;
		NA.FrictionCoefficient = 0.005f;
		NA.PumpingLossCoefficient = 0.005f;
		NA.IdleThrottleInterpSpeed = 1.f;
		NA.RevLimiterTime = 0.05f;
		NA.MaxPowerLambda = 0.85f;
		NA.DeceleratingLambda = 0.9f;
		NA.UnburntFuelAccumulationRate = 2.f;
		Engine.SetNaturallyAspiratedEngineConfig(NA);
		FVehicleEngineTurboConfig Turbo;
		Turbo.MaxBoostPressure = 1.f; // Default configs otherwise describe an NA engine.
		Turbo.StaticIntakeRestriction = -0.2f;
		Turbo.SpoolStartRPM = 1200.f;
		Turbo.FullBoostRPM = 3500.f;
		Turbo.SpoolUpDuration = 0.5f;
		Turbo.PressureDecayDuration = 0.2f;
		Turbo.BoostEfficiency = 0.7f;
		Turbo.bEnableAntiLag = false;
		Turbo.AntiLagMinRPM = 2000.f;
		Turbo.AntiLagTargetPressureRatio = 0.9f;
		Engine.SetTurboConfig(Turbo);
		FVehicleEngineExhaustConfig Exhaust;
		Exhaust.ExhaustScavengingStrength = 2.f;
		Exhaust.HeatUpRate = 0.5f;
		Exhaust.CoolDownRate = 0.2f;
		Exhaust.FlashPoint = 0.5f;
		Exhaust.IgnitionProbability = 0.f; // Deterministic response captures.
		Exhaust.PopFuelThreshold = 0.2f;
		Exhaust.FlameFuelThreshold = 0.25f;
		Exhaust.BackfireHeatSpike = 0.15f;
		Engine.SetExhaustConfig(Exhaust);
		auto& State = FVehicleEngineLegacyProbe::State(Engine);
		State = FVehicleEngineSimState();
		State.OperationMode = EVehicleEngineOperationMode::On;
		State.RevLimiterTimer = NA.RevLimiterTime;
		FVehicleEngineLegacyProbe::PinRPM(Engine, ReferenceRPM);
	}

	static void DynoStep(UVehicleEngineComponent& Engine, float Dt, float Throttle, bool bDisableSpark = false)
	{
		// Ideal speed controller: pin before update and before recording. The
		// exhaust sees the one-step crank response inside UpdatePhysics, as in production.
		FVehicleEngineLegacyProbe::PinRPM(Engine, ReferenceRPM);
		Engine.UpdatePhysics(Dt, Throttle, 0.f, bDisableSpark);
		FVehicleEngineLegacyProbe::PinRPM(Engine, ReferenceRPM);
	}

	static void WarmBoost(UVehicleEngineComponent& Engine, float Dt)
	{
		for (int32 I = 0; I < FMath::RoundToInt(5.f / Dt); ++I)
		{
			DynoStep(Engine, Dt, 1.f);
		}
	}

	static FString Row(double Time, UVehicleEngineComponent& Engine)
	{
		const auto& S = Engine.GetEngineState();
		return FString::Printf(TEXT("%.9f,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%d,%d,%d,%d,%.9g,%.9g\n"),
			Time, S.EngineRPM, S.RawThrottleInput, S.RealThrottle, S.TurboSpool,
			S.TurboPressure, S.ExhaustHeat, S.UnburntFuelBuffer, int32(S.bFuelInjection),
			int32(S.bSpark), int32(S.bIsAntiLagTriggered), int32(S.bIsTurboBlowingOff),
			S.EffectiveTorque, S.BackfireIntensity);
	}

	static bool IsFinite(const FVehicleEngineSimState& S)
	{
		return FMath::IsFinite(S.EngineRPM) && FMath::IsFinite(S.TurboSpool)
			&& FMath::IsFinite(S.TurboPressure) && FMath::IsFinite(S.ExhaustHeat)
			&& FMath::IsFinite(S.UnburntFuelBuffer) && FMath::IsFinite(S.EffectiveTorque)
			&& FMath::IsFinite(S.BackfireIntensity);
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleEngineLegacySemanticsTest,
	"KinetiForge.Engine.Legacy.Semantics",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleEngineLegacySemanticsTest::RunTest(const FString& Parameters)
{
	using namespace VehicleEngineLegacyBench;
	TStrongObjectPtr<UVehicleEngineComponent> Engine(NewObject<UVehicleEngineComponent>());
	Configure(*Engine);
	constexpr float Dt = 1.f / 120.f;
	for (int32 I = 0; I < 60; ++I) DynoStep(*Engine, Dt, 1.f);
	TestTrue(TEXT("Duration describes spool, not 95% boost"),
		Engine->GetTurboSpool() > 0.95f && Engine->GetTurboPressure() < 0.95f);

	Configure(*Engine);
	WarmBoost(*Engine, Dt);
	DynoStep(*Engine, Dt, 0.f);
	TestTrue(TEXT("Lift opens legacy BOV"), Engine->GetEngineState().bIsTurboBlowingOff);
	TestTrue(TEXT("Lift cuts injection above idle"), !Engine->GetEngineState().bFuelInjection);
	for (int32 I = 0; I < 240; ++I) DynoStep(*Engine, Dt, 0.f);
	TestTrue(TEXT("Lift retains idle-biased spool target"), Engine->GetTurboSpool() > 0.1f);
	TestTrue(TEXT("Legacy open BOV removes vacuum target"), FMath::Abs(Engine->GetTurboPressure()) < 0.001f);

	Configure(*Engine);
	WarmBoost(*Engine, Dt);
	auto Turbo = Engine->GetTurboConfig();
	Turbo.bEnableAntiLag = true;
	Engine->SetTurboConfig(Turbo);
	DynoStep(*Engine, Dt, 0.f);
	TestTrue(TEXT("ALS triggers on lift"), Engine->GetEngineState().bIsAntiLagTriggered);
	TestTrue(TEXT("ALS prevents BOV"), !Engine->GetEngineState().bIsTurboBlowingOff);
	TestTrue(TEXT("ALS forces injection"), Engine->GetEngineState().bFuelInjection);

	Configure(*Engine);
	DynoStep(*Engine, Dt, 1.f, true);
	TestTrue(TEXT("Spark cut retains injection"), Engine->GetEngineState().bFuelInjection);
	TestTrue(TEXT("Spark cut sends fuel to buffer"), Engine->GetEngineState().UnburntFuelBuffer > 0.f);
	TestTrue(TEXT("Spark cut still animates spool"), Engine->GetTurboSpool() > 0.f);

	Configure(*Engine);
	Turbo = Engine->GetTurboConfig();
	Turbo.SpoolUpDuration = 0.f;
	Engine->SetTurboConfig(Turbo);
	DynoStep(*Engine, Dt, 1.f);
	TestEqual(TEXT("Zero duration snaps spool to target"), Engine->GetTurboSpool(), 1.f);

	Configure(*Engine);
	auto& S = FVehicleEngineLegacyProbe::State(*Engine);
	S.bFuelInjection = false;
	S.RealThrottle = 1.f;
	FVehicleEngineLegacyProbe::ExhaustStep(*Engine, 1.f);
	TestTrue(TEXT("Legacy heat rises even without fuel or spark"), S.ExhaustHeat > 0.f);
	S.RealThrottle = 0.f;
	S.ExhaustHeat = 0.f;
	S.UnburntFuelBuffer = 1.f;
	FVehicleEngineLegacyProbe::ExhaustStep(*Engine, 2.f);
	TestEqual(TEXT("Large explicit decay step clamps fuel to zero"), S.UnburntFuelBuffer, 0.f);

	Configure(*Engine);
	auto Exhaust = Engine->GetExhaustConfig();
	Exhaust.ExhaustScavengingStrength = 0.f;
	Exhaust.HeatUpRate = Exhaust.CoolDownRate = 0.f;
	Exhaust.IgnitionProbability = 1.f;
	Engine->SetExhaustConfig(Exhaust);
	FVehicleEngineLegacyProbe::PinRPM(*Engine, 3600.f);
	S.RealThrottle = 1.f; // Keep target heat at the initial heat.
	S.ExhaustHeat = 0.6f;
	S.UnburntFuelBuffer = Exhaust.FlameFuelThreshold;
	FVehicleEngineLegacyProbe::ExhaustStep(*Engine, Dt);
	TestTrue(TEXT("Threshold event has unit intensity"), FMath::IsNearlyEqual(S.BackfireIntensity, 1.f));
	TestTrue(TEXT("Unit event consumes whole buffer"), FMath::IsNearlyZero(S.UnburntFuelBuffer));
	TestTrue(TEXT("Event adds normalized heat spike"), FMath::IsNearlyEqual(S.ExhaustHeat, 0.75f));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleEngineLegacyCaptureTest,
	"KinetiForge.Engine.Legacy.Capture",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleEngineLegacyCaptureTest::RunTest(const FString& Parameters)
{
	using namespace VehicleEngineLegacyBench;
	const FString OutputDir = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Automation/KinetiForge/EngineLegacy"));
	if (!IFileManager::Get().MakeDirectory(*OutputDir, true))
	{
		AddError(TEXT("Cannot create legacy capture directory"));
		return false;
	}
	const TCHAR* Scenarios[] = { TEXT("boost_rise"), TEXT("throttle_lift"), TEXT("antilag_lift"),
		TEXT("spark_cut"), TEXT("heat_rise"), TEXT("heat_cool"), TEXT("fuel_decay"),
		TEXT("backfire_impulse"), TEXT("free_rev"), TEXT("rev_limiter") };
	for (const int32 Hz : { 60, 120, 240 })
	{
		const float Dt = 1.f / Hz;
		for (const TCHAR* Scenario : Scenarios)
		{
			const FString Name(Scenario);
			TStrongObjectPtr<UVehicleEngineComponent> Engine(NewObject<UVehicleEngineComponent>());
			Configure(*Engine);
			auto& S = FVehicleEngineLegacyProbe::State(*Engine);
			const bool bLift = Name == TEXT("throttle_lift") || Name == TEXT("antilag_lift");
			const bool bExhaustOnly = Name.StartsWith(TEXT("heat_"))
				|| Name == TEXT("fuel_decay") || Name == TEXT("backfire_impulse");
			if (bLift || Name == TEXT("spark_cut")) WarmBoost(*Engine, Dt);
			if (Name == TEXT("antilag_lift"))
			{
				auto Turbo = Engine->GetTurboConfig();
				Turbo.bEnableAntiLag = true;
				Engine->SetTurboConfig(Turbo);
			}
			if (bExhaustOnly)
			{
				S.bFuelInjection = S.bSpark = false;
				S.RawThrottleInput = 0.f;
				S.RealThrottle = Name == TEXT("heat_rise") ? 1.f : 0.f;
				S.ExhaustHeat = Name == TEXT("heat_cool") ? ReferenceHeat : 0.f;
				S.UnburntFuelBuffer = Name == TEXT("fuel_decay") ? 1.f : 0.f;
			}
			if (Name == TEXT("backfire_impulse"))
			{
				auto Exhaust = Engine->GetExhaustConfig();
				Exhaust.ExhaustScavengingStrength = 0.f;
				Exhaust.IgnitionProbability = 1.f;
				S.UnburntFuelBuffer = Exhaust.FlameFuelThreshold;
				S.ExhaustHeat = 0.6f;
				Engine->SetExhaustConfig(Exhaust);
			}
			if (Name == TEXT("rev_limiter")) FVehicleEngineLegacyProbe::PinRPM(*Engine, 6100.f);
			const bool bFreeCrank = Name == TEXT("free_rev") || Name == TEXT("rev_limiter");
			const int32 Steps = (bExhaustOnly ? 30 : 5) * Hz;
			FString Csv(TEXT("time_s,rpm,raw_throttle,real_throttle,spool,legacy_pressure,legacy_heat,legacy_fuel_buffer,injection,spark,antilag,bov,effective_torque,backfire_intensity\n"));
			Csv += Row(0.0, *Engine);
			bool bFinite = true;
			for (int32 Step = 1; Step <= Steps; ++Step)
			{
				if (bExhaustOnly) FVehicleEngineLegacyProbe::ExhaustStep(*Engine, Dt);
				else if (bFreeCrank) Engine->UpdatePhysics(Dt, 1.f, 0.f);
				else DynoStep(*Engine, Dt, bLift ? 0.f : 1.f, Name == TEXT("spark_cut"));
				bFinite &= IsFinite(S);
				Csv += Row(double(Step) / Hz, *Engine);
			}
			TestTrue(FString::Printf(TEXT("%s at %d Hz stays finite"), Scenario, Hz), bFinite);
			const FString Path = FPaths::Combine(OutputDir, FString::Printf(TEXT("%s_%dHz.csv"), Scenario, Hz));
			TestTrue(FString::Printf(TEXT("Write %s"), *Path), FFileHelper::SaveStringToFile(Csv, *Path, FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM));
		}
	}
	AddInfo(FString::Printf(TEXT("Legacy reference captures: %s. Protocol/config are defined in this test fixture."), *OutputDir));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
