// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "VehicleWheelSolver.h"
#include "VehicleWheelRuntime.h"
#include "VehicleWheelComponent.h"
#include "VehicleSuspensionSolver.h"
#include "Curves/CurveFloat.h"
#include "Misc/AutomationTest.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"
#include "UObject/StrongObjectPtr.h"

namespace VehicleWheelRuntimeTest
{
	struct FWheelCapture
	{
		float AngularVelocity = 0.f;
		float AngularAcceleration = 0.f;
		float TireForceX = 0.f;
		float TireForceY = 0.f;
		float SlipRatio = 0.f;
		float PredictedSlipRatio = 0.f;
		float LongSlipVelocity = 0.f;
		float LatSlipVelocity = 0.f;
		float TransientSlipX = 0.f;
		float TransientSlipY = 0.f;
		float WheelLoad = 0.f;
		float DriveTorque = 0.f;
		float BrakeTorque = 0.f;
		float BrakeFromESP = 0.f;
		float BrakeFromHandbrake = 0.f;
		float EffectiveInertia = 0.f;
		float SignedCamber = 0.f;
		float Friction = 0.f;
		bool bABS = false;
	};

	bool IsFinite(const FWheelCapture& C)
	{
		return FMath::IsFinite(C.AngularVelocity) && FMath::IsFinite(C.AngularAcceleration)
			&& FMath::IsFinite(C.TireForceX) && FMath::IsFinite(C.TireForceY)
			&& FMath::IsFinite(C.SlipRatio) && FMath::IsFinite(C.PredictedSlipRatio)
			&& FMath::IsFinite(C.LongSlipVelocity) && FMath::IsFinite(C.LatSlipVelocity)
			&& FMath::IsFinite(C.TransientSlipX) && FMath::IsFinite(C.TransientSlipY)
			&& FMath::IsFinite(C.WheelLoad) && FMath::IsFinite(C.DriveTorque)
			&& FMath::IsFinite(C.BrakeTorque) && FMath::IsFinite(C.BrakeFromESP)
			&& FMath::IsFinite(C.BrakeFromHandbrake) && FMath::IsFinite(C.EffectiveInertia)
			&& FMath::IsFinite(C.SignedCamber) && FMath::IsFinite(C.Friction);
	}

	FVehicleSuspensionSimState MakeSuspensionInput(const bool bOnGround)
	{
		FVehicleSuspensionSimState S;
		S.bHitGround = bOnGround;
		S.bWheelOnGround = bOnGround;
		S.StaticSprungMass = 300.f;
		S.VirtualUnsprungMass = 40.f;
		S.EffectiveSprungMassNormal = 300.f;
		S.EffectiveSprungMassLong = 300.f;
		S.EffectiveSprungMassLat = 300.f;
		S.ForceAlongImpactNormal = bOnGround ? 4200.f : 0.f;
		S.ImpactFriction = 0.73f;
		S.ImpactWorldNormal = bOnGround ? FVector3f(0.f, 0.0871557f, 0.996195f) : FVector3f::UpVector;
		S.WheelWorldRightVector = FVector3f(0.f, 0.996195f, 0.0871557f);
		S.ImpactWorldVelocity = FVector3f(12.f, 0.8f, 0.f);
		return S;
	}

	FWheelCapture Capture(const FVehicleWheelSolver& Solver)
	{
		const FVehicleWheelSimState& S = Solver.State;
		return { S.AngularVelocity, S.AngularAcceleration, S.TireForce2D.X, S.TireForce2D.Y,
			S.SlipRatio, S.PredictedSlipRatio, S.LongSlipVelocity, S.LatSlipVelocity,
			S.TransientSlip.X, S.TransientSlip.Y, S.WheelLoad, S.DriveTorque, S.BrakeTorque,
			S.BrakeTorqueFromESP, S.BrakeTorqueFromHandbrake, S.EffectiveInertia,
			S.SignedCamberDegree, S.DynFrictionMultiplier, S.bABSTriggered };
	}

	FWheelCapture Capture(const FVehicleWheelRuntime& Runtime)
	{
		const FVehicleWheelSimState& S = Runtime.WheelState;
		return { S.AngularVelocity, S.AngularAcceleration, S.TireForce2D.X, S.TireForce2D.Y,
			S.SlipRatio, S.PredictedSlipRatio, S.LongSlipVelocity, S.LatSlipVelocity,
			S.TransientSlip.X, S.TransientSlip.Y, S.WheelLoad, S.DriveTorque, S.BrakeTorque,
			S.BrakeTorqueFromESP, S.BrakeTorqueFromHandbrake, S.EffectiveInertia,
			S.SignedCamberDegree, S.DynFrictionMultiplier, S.bABSTriggered };
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleWheelRuntimeLegacyCaptureTest,
	"KinetiForge.WheelRuntime.LegacyCapture",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleWheelRuntimeLegacyCaptureTest::RunTest(const FString& Parameters)
{
	using namespace VehicleWheelRuntimeTest;
	(void)Parameters;

	TStrongObjectPtr<UCurveFloat> Fx(NewObject<UCurveFloat>());
	TStrongObjectPtr<UCurveFloat> Fy(NewObject<UCurveFloat>());
	TStrongObjectPtr<UCurveFloat> CamberDrift(NewObject<UCurveFloat>());
	if (!TestNotNull(TEXT("Fx curve created"), Fx.Get())
		|| !TestNotNull(TEXT("Fy curve created"), Fy.Get())
		|| !TestNotNull(TEXT("Camber drift curve created"), CamberDrift.Get())) return false;

	const FKeyHandle Fx0 = Fx->FloatCurve.AddKey(0.f, 0.f);
	const FKeyHandle Fx1 = Fx->FloatCurve.AddKey(0.12f, 1.f);
	Fx->FloatCurve.SetKeyInterpMode(Fx0, RCIM_Cubic);
	Fx->FloatCurve.SetKeyInterpMode(Fx1, RCIM_Cubic);
	Fy->FloatCurve.AddKey(0.f, 0.f);
	Fy->FloatCurve.AddKey(18.f, 1.1f);
	CamberDrift->FloatCurve.AddKey(0.f, 0.f);
	CamberDrift->FloatCurve.AddKey(90.f, 0.025f);

	FVehicleWheelConfig WheelConfig;
	WheelConfig.Radius = 34.f;
	WheelConfig.Width = 22.f;
	WheelConfig.Inertia = 1.4f;
	WheelConfig.RollingRisistance = 7.f;

	FVehicleTireConfig TireConfig;
	TireConfig.FrictionMultiplier = 1.25f;
	TireConfig.TireFrictionCombineMode = ETireFrictionCombineMode::Multiply;
	TireConfig.RelaxationLength = FVector2f(0.16f, 0.27f);
	TireConfig.WheelLoadInfluenceFactor = 0.72f;
	TireConfig.CombinedSlipBias = 0.42f;
	TireConfig.LongitudinalToLateralInterference = 0.83f;
	TireConfig.LateralToLongitudinalInterference = 0.91f;
	TireConfig.MaxFx = 1.15f;
	TireConfig.MaxFy = 1.05f;
	TireConfig.Fx = Fx.Get();
	TireConfig.Fy = Fy.Get();
	TireConfig.CamberToLateralDrift = CamberDrift.Get();

	FVehicleABSConfig ABSConfig;
	ABSConfig.bAntiBrakeSystemEnabled = true;
	ABSConfig.OptimalSlip = 0.12f;
	ABSConfig.ActivationSpeed = 2.f;
	ABSConfig.Sensitivity = 2.2f;

	const FTransform ChassisTransform = FTransform::Identity;
	const FVehicleSuspensionSimState GroundInput = MakeSuspensionInput(true);
	const FVehicleSuspensionSimState AirInput = MakeSuspensionInput(false);
	const float MacroDts[] = { 1.f / 60.f, 1.f / 120.f, 1.f / 240.f };
	FString Csv = TEXT("macro_dt,macro_step,on_ground,substeps,omega,alpha,force_x,force_y,slip_ratio,predicted_slip,long_slip,lat_slip,transient_x,transient_y,wheel_load,drive_torque,brake_torque,esp_torque,handbrake_torque,effective_inertia,signed_camber,friction,abs\n");
	int32 GroundForceSteps = 0;
	int32 AirTorqueSteps = 0;
	int32 AbsSteps = 0;
	int32 CaptureCount = 0;

	for (const float MacroDt : MacroDts)
	{
		FVehicleWheelSolver Solver;
		Solver.Initialize(TireConfig);
		Solver.State.AngularVelocity = 0.f;
		Solver.State.AngularAcceleration = 0.f;
		Solver.State.P4MotorTorque = 45.f;
		const FVehicleSuspensionSimState* CurrentSuspension = &GroundInput;

		for (int32 MacroStep = 0; MacroStep < 4; ++MacroStep)
		{
			CurrentSuspension = MacroStep == 3 ? &AirInput : &GroundInput;
			Solver.State.BrakeTorqueFromESP = MacroStep == 1 ? 180.f : 0.f;
			// Synthetic suspension/contact values are explicit test inputs, not raycast validation.
			Solver.PreStep(MacroDt, ChassisTransform, *CurrentSuspension, WheelConfig, TireConfig);
			const float AngularVelocityBefore = Solver.State.AngularVelocity;

			const float DriveTorque[] = { 260.f, 0.f, 80.f, 180.f };
			const float BrakeTorque[] = { 0.f, 1100.f, 0.f, 0.f };
			const float HandbrakeTorque[] = { 0.f, 0.f, 0.f, 35.f };
			const float ReflectedInertia[] = { 0.35f, 0.35f, 0.5f, 0.2f };
			for (int32 Substep = 0; Substep < 4; ++Substep)
			{
				Solver.Substep(MacroDt * 0.25f, DriveTorque[MacroStep], BrakeTorque[MacroStep],
					HandbrakeTorque[MacroStep], ReflectedInertia[MacroStep], WheelConfig,
					TireConfig, ABSConfig, *CurrentSuspension);
			}
			Solver.PostStep();

			const FWheelCapture C = Capture(Solver);
			TestTrue(FString::Printf(TEXT("Finite state at dt %.9g step %d"), MacroDt, MacroStep), IsFinite(C));
			GroundForceSteps += (MacroStep < 3 && (FMath::Abs(C.TireForceX) + FMath::Abs(C.TireForceY) > 0.001f)) ? 1 : 0;
			AirTorqueSteps += (MacroStep == 3 && C.AngularVelocity > AngularVelocityBefore) ? 1 : 0;
			AbsSteps += C.bABS ? 1 : 0;
			++CaptureCount;

			Csv += FString::Printf(TEXT("%.9g,%d,%d,4,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%d\n"),
				MacroDt, MacroStep, MacroStep == 3 ? 0 : 1, C.AngularVelocity, C.AngularAcceleration,
				C.TireForceX, C.TireForceY, C.SlipRatio, C.PredictedSlipRatio, C.LongSlipVelocity,
				C.LatSlipVelocity, C.TransientSlipX, C.TransientSlipY, C.WheelLoad, C.DriveTorque,
				C.BrakeTorque, C.BrakeFromESP, C.BrakeFromHandbrake, C.EffectiveInertia,
				C.SignedCamber, C.Friction, C.bABS ? 1 : 0);
		}
	}

	TestEqual(TEXT("All legacy solver captures emitted"), CaptureCount, 12);
	TestTrue(TEXT("Grounded tire force responds to drive/brake inputs"), GroundForceSteps > 0);
	TestTrue(TEXT("ABS triggers from the synthetic braking slip input"), AbsSteps > 0);
	TestEqual(TEXT("Airborne steps still integrate wheel torque"), AirTorqueSteps, 3);

	const FString OutputDir = FPaths::ProjectSavedDir() / TEXT("Automation/KinetiForge/WheelRuntime");
	IFileManager::Get().MakeDirectory(*OutputDir, true);
	const FString OutputPath = OutputDir / TEXT("wheel-before.csv");
	TestTrue(TEXT("Legacy wheel capture CSV saved"), FFileHelper::SaveStringToFile(Csv, *OutputPath));
	AddInfo(FString::Printf(TEXT("Legacy WheelSolver capture saved to %s"), *OutputPath));
	return true;
}

namespace VehicleWheelRuntimeTest
{
	void CompareCapture(FAutomationTestBase& Test, const FWheelCapture& Expected,
		const FWheelCapture& Actual, const TCHAR* Label)
	{
	const float Tol = 1.e-5f;
	Test.TestTrue(FString::Printf(TEXT("%s wheel state matches legacy"), Label),
		FMath::IsNearlyEqual(Expected.AngularVelocity, Actual.AngularVelocity, Tol)
		&& FMath::IsNearlyEqual(Expected.AngularAcceleration, Actual.AngularAcceleration, Tol)
		&& FMath::IsNearlyEqual(Expected.TireForceX, Actual.TireForceX, Tol)
		&& FMath::IsNearlyEqual(Expected.TireForceY, Actual.TireForceY, Tol)
		&& FMath::IsNearlyEqual(Expected.SlipRatio, Actual.SlipRatio, Tol)
		&& FMath::IsNearlyEqual(Expected.PredictedSlipRatio, Actual.PredictedSlipRatio, Tol)
		&& FMath::IsNearlyEqual(Expected.LongSlipVelocity, Actual.LongSlipVelocity, Tol)
		&& FMath::IsNearlyEqual(Expected.LatSlipVelocity, Actual.LatSlipVelocity, Tol)
		&& FMath::IsNearlyEqual(Expected.TransientSlipX, Actual.TransientSlipX, Tol)
		&& FMath::IsNearlyEqual(Expected.TransientSlipY, Actual.TransientSlipY, Tol)
		&& FMath::IsNearlyEqual(Expected.WheelLoad, Actual.WheelLoad, Tol)
		&& FMath::IsNearlyEqual(Expected.DriveTorque, Actual.DriveTorque, Tol)
		&& FMath::IsNearlyEqual(Expected.BrakeTorque, Actual.BrakeTorque, Tol)
		&& FMath::IsNearlyEqual(Expected.BrakeFromESP, Actual.BrakeFromESP, Tol)
		&& FMath::IsNearlyEqual(Expected.BrakeFromHandbrake, Actual.BrakeFromHandbrake, Tol)
		&& FMath::IsNearlyEqual(Expected.EffectiveInertia, Actual.EffectiveInertia, Tol)
		&& FMath::IsNearlyEqual(Expected.SignedCamber, Actual.SignedCamber, Tol)
		&& FMath::IsNearlyEqual(Expected.Friction, Actual.Friction, Tol)
		&& Expected.bABS == Actual.bABS);
	}

	template <int32 NumSamples>
	bool SameSamples(const FVehicleLUT<NumSamples>& A, const FVehicleLUT<NumSamples>& B)
	{
		for (int32 I = 0; I < NumSamples; ++I)
		{
			const float Progress = static_cast<float>(I) / (NumSamples - 1);
			if (!FMath::IsNearlyEqual(A.FastEval(Progress).Value, B.FastEval(Progress).Value, 1.e-6f)) return false;
		}
		return true;
	}

	bool SameTireLUT(const FVehicleTireLUT<64>& A, const FVehicleTireLUT<64>& B)
	{
		return SameSamples(A, B) && A.bHasValidStiffness == B.bHasValidStiffness
			&& A.OptimalSlipIndex == B.OptimalSlipIndex
			&& FMath::IsNearlyEqual(A.LinearStiffness, B.LinearStiffness, 1.e-6f)
			&& FMath::IsNearlyEqual(A.PeakFriction, B.PeakFriction, 1.e-6f);
	}

	void MakeCurvedSettings(TStrongObjectPtr<UCurveFloat>& Curve, FVehicleTireConfig& Tire,
		FVehicleSuspensionKinematicsConfig& Kinematics)
	{
		Curve.Reset(NewObject<UCurveFloat>());
		const FKeyHandle K0 = Curve->FloatCurve.AddKey(0.f, 0.f);
		const FKeyHandle K1 = Curve->FloatCurve.AddKey(0.2f, 1.f);
		const FKeyHandle K2 = Curve->FloatCurve.AddKey(1.f, 0.7f);
		Curve->FloatCurve.SetKeyInterpMode(K0, RCIM_Cubic);
		Curve->FloatCurve.SetKeyInterpMode(K1, RCIM_Cubic);
		Curve->FloatCurve.SetKeyInterpMode(K2, RCIM_Cubic);
		Tire.Fx = Curve.Get();
		Tire.Fy = Curve.Get();
		Tire.CamberToLateralDrift = Curve.Get();
		Kinematics.CamberCurve = Curve.Get();
		Kinematics.ToeCurve = Curve.Get();
		Kinematics.CasterCurve = Curve.Get();
		Kinematics.AntiDiveCurve = Curve.Get();
		Kinematics.AntiSquatCurve = Curve.Get();
		Kinematics.AntiRollCurve = Curve.Get();
		Kinematics.SpringMotionRatioCurve = Curve.Get();
	}

	bool HasNoCurvePointers(const FVehicleWheelRuntimeSettings& Settings)
	{
		const FVehicleTireConfig& T = Settings.TireConfig;
		const FVehicleSuspensionKinematicsConfig& K = Settings.KinematicsConfig;
		return !T.Fx && !T.Fy && !T.CamberToLateralDrift
			&& !K.CamberCurve && !K.ToeCurve && !K.CasterCurve && !K.AntiDiveCurve
			&& !K.AntiSquatCurve && !K.AntiRollCurve && !K.SpringMotionRatioCurve;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleWheelRuntimeOwnedCompatibilityTest,
	"KinetiForge.WheelRuntime.OwnedCompatibility",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleWheelRuntimeOwnedCompatibilityTest::RunTest(const FString& Parameters)
{
	using namespace VehicleWheelRuntimeTest;
	(void)Parameters;
	TStrongObjectPtr<UCurveFloat> Curve;
	FVehicleTireConfig TireA;
	FVehicleSuspensionKinematicsConfig Kinematics;
	MakeCurvedSettings(Curve, TireA, Kinematics);
	FVehicleTireConfig TireB = TireA;
	TireB.FrictionMultiplier = 0.62f;
	TireB.Fx = nullptr;

	FVehicleWheelConfig WheelConfig;
	WheelConfig.Radius = 34.f;
	WheelConfig.Inertia = 1.4f;
	WheelConfig.RollingRisistance = 7.f;
	FVehicleABSConfig ABS;
	ABS.OptimalSlip = 0.12f;
	ABS.ActivationSpeed = 2.f;
	ABS.Sensitivity = 2.2f;
	FVehicleSuspensionSpringConfig Spring;
	FVehicleWheelCachedLUTs TireLUTA, TireLUTB;
	FVehicleWheelSolver::CacheLUTs(TireLUTA, TireA);
	FVehicleWheelSolver::CacheLUTs(TireLUTB, TireB);
	FVehicleSuspensionCachedLUTs SuspensionLUT;
	FVehicleSuspensionSolver::CacheLUTs(SuspensionLUT, Kinematics);
	const FVehicleWheelRuntimeSettings SettingsA = FVehicleWheelRuntimeSettings::FromCachedConfig(
		WheelConfig, TireA, ABS, Kinematics, Spring, TireLUTA, SuspensionLUT);
	const FVehicleWheelRuntimeSettings SettingsB = FVehicleWheelRuntimeSettings::FromCachedConfig(
		WheelConfig, TireB, ABS, Kinematics, Spring, TireLUTB, SuspensionLUT);
	TestTrue(TEXT("Runtime settings strip every curve UObject pointer"), HasNoCurvePointers(SettingsA)
		&& HasNoCurvePointers(SettingsB));

	FVehicleWheelSolver LegacyA, LegacyB;
	LegacyA.Initialize(TireA);
	LegacyB.Initialize(TireB);
	FVehicleWheelRuntime RuntimeA, RuntimeB;
	RuntimeA.Settings = SettingsA;
	RuntimeB.Settings = SettingsB;
	RuntimeA.SuspensionState = MakeSuspensionInput(true);
	RuntimeB.SuspensionState = MakeSuspensionInput(true);
	RuntimeB.SuspensionState.ImpactWorldVelocity = FVector3f(8.f, -1.2f, 0.f);
	RuntimeB.SuspensionState.ImpactFriction = 0.45f;
	LegacyA.State.P4MotorTorque = RuntimeA.WheelState.P4MotorTorque = 45.f;
	LegacyB.State.P4MotorTorque = RuntimeB.WheelState.P4MotorTorque = -20.f;
	LegacyA.State.BrakeTorqueFromESP = RuntimeA.WheelState.BrakeTorqueFromESP = 120.f;
	LegacyB.State.BrakeTorqueFromESP = RuntimeB.WheelState.BrakeTorqueFromESP = 60.f;
	LegacyA.State.AngularVelocity = RuntimeA.WheelState.AngularVelocity = 0.f;
	LegacyB.State.AngularVelocity = RuntimeB.WheelState.AngularVelocity = 5.f;
	LegacyA.State.AngularAcceleration = RuntimeA.WheelState.AngularAcceleration = 0.f;
	LegacyB.State.AngularAcceleration = RuntimeB.WheelState.AngularAcceleration = 0.f;

	RuntimeA.WheelContext.AccumulateTireImpulse2D = FVector2f(7.f, -3.f);
	RuntimeA.WheelContext.MacroDeltaTime = 0.033f;
	RuntimeA.SuspensionState.StaticSprungMass = 333.f;
	const FWheelCapture BeforeApply = Capture(RuntimeA);
	const float BeforeContextDt = RuntimeA.WheelContext.MacroDeltaTime;
	const FVector2f BeforeAccumulator = RuntimeA.WheelContext.AccumulateTireImpulse2D;
	const FVehicleSuspensionSimState BeforeSuspension = RuntimeA.SuspensionState;
	RuntimeA.ApplySettings(SettingsA);
	const FWheelCapture AfterApply = Capture(RuntimeA);
	CompareCapture(*this, BeforeApply, AfterApply, TEXT("ApplySettings preserves"));
	TestTrue(TEXT("Applying owned settings preserves P4 and ESP commands"),
		RuntimeA.WheelState.P4MotorTorque == 45.f && RuntimeA.WheelState.BrakeTorqueFromESP == 120.f);
	TestTrue(TEXT("Applying owned settings leaves wheel context untouched"),
		RuntimeA.WheelContext.MacroDeltaTime == BeforeContextDt
		&& RuntimeA.WheelContext.AccumulateTireImpulse2D == BeforeAccumulator);
	TestTrue(TEXT("Applying owned settings leaves suspension snapshot untouched"),
		RuntimeA.SuspensionState.StaticSprungMass == BeforeSuspension.StaticSprungMass
		&& RuntimeA.SuspensionState.ImpactWorldVelocity == BeforeSuspension.ImpactWorldVelocity);

	const float MacroDt = 1.f / 120.f;
	const FTransform Chassis = FTransform::Identity;
	int32 AbsCaptures = 0;
	bool bGroundedResponsesDiffer = false;
	for (int32 MacroStep = 0; MacroStep < 4; ++MacroStep)
	{
		if (MacroStep == 3)
		{
			RuntimeA.SuspensionState = MakeSuspensionInput(false);
			RuntimeB.SuspensionState = MakeSuspensionInput(false);
			LegacyA.State.BrakeTorqueFromESP = RuntimeA.WheelState.BrakeTorqueFromESP = 0.f;
			LegacyB.State.BrakeTorqueFromESP = RuntimeB.WheelState.BrakeTorqueFromESP = 0.f;
		}
		const FVehicleSuspensionSimState InputA = RuntimeA.SuspensionState;
		const FVehicleSuspensionSimState InputB = RuntimeB.SuspensionState;
		LegacyA.PreStep(MacroDt, Chassis, InputA, WheelConfig, TireA);
		LegacyB.PreStep(MacroDt, Chassis, InputB, WheelConfig, TireB);
		RuntimeA.PrepareWheel(MacroDt, Chassis);
		RuntimeB.PrepareWheel(MacroDt, Chassis);
		for (int32 Substep = 0; Substep < 4; ++Substep)
		{
			const float Drive = MacroStep == 0 ? 210.f : (MacroStep == 3 ? 100.f : 0.f);
			const float Brake = MacroStep == 1 ? 850.f : 0.f;
			const float Handbrake = MacroStep == 2 ? 280.f : 0.f;
			const float Reflected = MacroStep == 0 ? 0.4f : 0.2f;
			RuntimeA.IntegrateWheel(MacroDt * 0.25f, Drive, Brake, Handbrake, Reflected);
			RuntimeB.IntegrateWheel(MacroDt * 0.25f, Drive, Brake, Handbrake, Reflected);
			LegacyA.Substep(MacroDt * 0.25f, Drive, Brake, Handbrake, Reflected,
				WheelConfig, TireA, ABS, InputA);
			LegacyB.Substep(MacroDt * 0.25f, Drive, Brake, Handbrake, Reflected,
				WheelConfig, TireB, ABS, InputB);
		}
		RuntimeA.FinalizeWheel();
		RuntimeB.FinalizeWheel();
		LegacyA.PostStep();
		LegacyB.PostStep();
		CompareCapture(*this, Capture(LegacyA), Capture(RuntimeA), *FString::Printf(TEXT("runtime A step %d"), MacroStep));
		CompareCapture(*this, Capture(LegacyB), Capture(RuntimeB), *FString::Printf(TEXT("runtime B step %d"), MacroStep));
		AbsCaptures += RuntimeA.WheelState.bABSTriggered ? 1 : 0;
		if (MacroStep < 3)
		{
			bGroundedResponsesDiffer |= !FMath::IsNearlyEqual(
				RuntimeA.WheelState.TireForce.X, RuntimeB.WheelState.TireForce.X, 1.e-5f);
		}
		TestTrue(FString::Printf(TEXT("Runtime A finite at macro step %d"), MacroStep), IsFinite(Capture(RuntimeA)));
		TestTrue(FString::Printf(TEXT("Runtime B finite at macro step %d"), MacroStep), IsFinite(Capture(RuntimeB)));
	}
	TestTrue(TEXT("ABS history is exercised in the runtime comparison"), AbsCaptures > 0);
	TestTrue(TEXT("Interleaved runtime states remain distinct"),
		!FMath::IsNearlyEqual(RuntimeA.WheelState.AngularVelocity, RuntimeB.WheelState.AngularVelocity, 1.e-5f)
		&& bGroundedResponsesDiffer);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleWheelRuntimeCachedSettingsSnapshotTest,
	"KinetiForge.WheelRuntime.CachedSettingsSnapshot",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleWheelRuntimeCachedSettingsSnapshotTest::RunTest(const FString& Parameters)
{
	using namespace VehicleWheelRuntimeTest;
	(void)Parameters;
	TStrongObjectPtr<UCurveFloat> Curve;
	FVehicleTireConfig Tire;
	FVehicleSuspensionKinematicsConfig Kinematics;
	MakeCurvedSettings(Curve, Tire, Kinematics);
	TStrongObjectPtr<UVehicleWheelComponent> Wheel(NewObject<UVehicleWheelComponent>());
	if (!TestNotNull(TEXT("Wheel component created"), Wheel.Get())) return false;
	Wheel->SetTireConfig(Tire);
	Wheel->SetSuspensionKinematicsConfig(Kinematics);

	FVehicleWheelCachedLUTs ExpectedTireLUT;
	FVehicleWheelSolver::CacheLUTs(ExpectedTireLUT, Tire);
	FVehicleSuspensionCachedLUTs ExpectedSuspensionLUT;
	FVehicleSuspensionSolver::CacheLUTs(ExpectedSuspensionLUT, Kinematics);
	FVehicleWheelRuntimeSettings Initial = Wheel->CaptureRuntimeSettings();
	TestTrue(TEXT("Capture contains no tire or suspension curve pointers"), HasNoCurvePointers(Initial));
	TestTrue(TEXT("Captured tire tables match cached curve bake"),
		SameTireLUT(Initial.TireLUTs.Fx, ExpectedTireLUT.Fx)
		&& SameTireLUT(Initial.TireLUTs.Fy, ExpectedTireLUT.Fy)
		&& SameSamples(Initial.TireLUTs.CamberToLateralDrift, ExpectedTireLUT.CamberToLateralDrift));
	TestTrue(TEXT("Captured suspension tables match cached curve bake"),
		SameSamples(Initial.SuspensionLUTs.CamberCurve, ExpectedSuspensionLUT.CamberCurve)
		&& SameSamples(Initial.SuspensionLUTs.ToeCurve, ExpectedSuspensionLUT.ToeCurve)
		&& SameSamples(Initial.SuspensionLUTs.CasterCurve, ExpectedSuspensionLUT.CasterCurve)
		&& SameSamples(Initial.SuspensionLUTs.AntiDiveCurve, ExpectedSuspensionLUT.AntiDiveCurve)
		&& SameSamples(Initial.SuspensionLUTs.AntiSquatCurve, ExpectedSuspensionLUT.AntiSquatCurve)
		&& SameSamples(Initial.SuspensionLUTs.AntiRollCurve, ExpectedSuspensionLUT.AntiRollCurve)
		&& SameSamples(Initial.SuspensionLUTs.MotionRatioCurve, ExpectedSuspensionLUT.MotionRatioCurve));

	const FVehicleWheelRuntimeSettings StableSnapshot = Initial;
	Curve->FloatCurve.UpdateOrAddKey(Curve->FloatCurve.GetLastKey().Time, 0.1f);
	const FVehicleWheelRuntimeSettings WithoutRebake = Wheel->CaptureRuntimeSettings();
	TestTrue(TEXT("Changing resource curve does not change the component's existing cache"),
		SameTireLUT(StableSnapshot.TireLUTs.Fx, WithoutRebake.TireLUTs.Fx)
		&& SameSamples(StableSnapshot.SuspensionLUTs.CamberCurve, WithoutRebake.SuspensionLUTs.CamberCurve));

	Wheel->UpdateCachedLookUpTables();
	const FVehicleWheelRuntimeSettings Rebuilt = Wheel->CaptureRuntimeSettings();
	TestTrue(TEXT("Rebaking changes the component cache but not an older snapshot"),
		!SameTireLUT(StableSnapshot.TireLUTs.Fx, Rebuilt.TireLUTs.Fx)
		&& SameTireLUT(StableSnapshot.TireLUTs.Fx, Initial.TireLUTs.Fx));

	const float OldLinearStiffness = Rebuilt.TireLUTs.Fx.LinearStiffness;
	const float OldPeakFriction = Rebuilt.TireLUTs.Fx.PeakFriction;
	const bool bHadStiffness = Rebuilt.TireLUTs.Fx.bHasValidStiffness;
	Tire.Fx = nullptr;
	Wheel->SetTireConfig(Tire);
	const FVehicleWheelRuntimeSettings RemovedFx = Wheel->CaptureRuntimeSettings();
	bool bAllFxSamplesAreOne = true;
	for (int32 SampleIndex = 0; SampleIndex < RemovedFx.TireLUTs.Fx.Num(); ++SampleIndex)
	{
		const float Sample = RemovedFx.TireLUTs.Fx.FastEval(
			static_cast<float>(SampleIndex) / (RemovedFx.TireLUTs.Fx.Num() - 1)).Value;
		bAllFxSamplesAreOne &= FMath::IsNearlyEqual(Sample, 1.f, 1.e-6f);
	}
	TestTrue(TEXT("Removing Fx retains legacy set-all-to-one behavior"), bAllFxSamplesAreOne);
	TestTrue(TEXT("Removing Fx preserves cached tire LUT metadata"),
		RemovedFx.TireLUTs.Fx.bHasValidStiffness == bHadStiffness
		&& FMath::IsNearlyEqual(RemovedFx.TireLUTs.Fx.LinearStiffness, OldLinearStiffness, 1.e-6f)
		&& FMath::IsNearlyEqual(RemovedFx.TireLUTs.Fx.PeakFriction, OldPeakFriction, 1.e-6f));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
