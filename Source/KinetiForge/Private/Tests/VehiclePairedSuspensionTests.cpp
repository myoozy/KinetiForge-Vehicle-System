// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "VehicleSuspensionSolver.h"
#include "Misc/AutomationTest.h"

namespace VehiclePairedSuspensionTest
{
	struct FWheel
	{
		FVehicleSuspensionSimContext Context;
		FVehicleChassisSimState Chassis;
		FVehicleSuspensionKinematicsConfig Kinematics;
		FVehicleSuspensionSpringConfig Spring;
		FVehicleSuspensionCachedLUTs LUTs;
		float Projection = 1.f;
		float Radius = 30.f;
		float Inertia = 0.f;

		FWheel(float Length = 500.f, float Mass = 40.f)
		{
			Kinematics.Stroke = 1000.f;
			Kinematics.SuspensionAndBrakeMass = Mass;
			Spring.SpringStiffness = 0.f;
			Spring.SpringPreload = 0.f;
			Spring.ReboundDamping = Spring.CompressionDamping = 0.f;
			Spring.bUseDampingRatio = false;
			Context.StrutCurrentLength = Length;
			Context.TransientStrutLengthLimit = Kinematics.Stroke;
			Context.HitDistance = Radius + Kinematics.Stroke;
			Context.RayCastLength = Kinematics.Stroke;
			Context.ChassisWorldTransform = FTransform::Identity;
			Context.StaticSprungMass = 300.f;
			Context.bHitGround = false;
			Chassis.LinearAcceleration = Chaos::FVec3(0.0);
			Chassis.AngularAcceleration = Chaos::FVec3(0.0);
			Chassis.AngularVelocity = Chaos::FVec3(0.0);
			Chassis.CoMWorldLocation = Chaos::FVec3(0.0);
			Prepare(1.f / 60.f);
		}

		void Prepare(float Dt)
		{
			Context.PhysicsDeltaTime = Dt;
			Context.CurrentExtensionRatio = Context.StrutCurrentLength / Kinematics.Stroke;
			Context.StrutChassisDirection = FVector3f(FMath::Sqrt(1.f - Projection * Projection), 0.f, Projection);
			Context.HubChassisTransform = FTransform3f(FQuat4f::Identity,
				FVector3f(0.f, 0.f, -Context.StrutCurrentLength * Projection));
		}
		FVehicleSuspensionIntegrationInput Input()
		{
			return { Context, Chassis, 0.f, Radius, Inertia, Kinematics, Spring, LUTs };
		}
	};

	void Step(FWheel& Left, FWheel& Right, float Dt, float Bar)
	{
		Left.Prepare(Dt);
		Right.Prepare(Dt);
		FVehicleSuspensionSolver::IntegratePairedIndependentSuspension(Left.Input(), Right.Input(), Bar);
	}

	double Energy(const FWheel& Left, const FWheel& Right, double Bar)
	{
		const double Difference = Left.Context.StrutCurrentLength * Left.Projection
			- Right.Context.StrutCurrentLength * Right.Projection;
		return 0.00005 * (Left.Kinematics.SuspensionAndBrakeMass * FMath::Square(double(Left.Context.StrutCurrentVelocity))
			+ Right.Kinematics.SuspensionAndBrakeMass * FMath::Square(double(Right.Context.StrutCurrentVelocity)))
			+ 0.005 * Bar * Difference * Difference;
	}

	bool FiniteAndBounded(const FWheel& Wheel)
	{
		return FMath::IsFinite(Wheel.Context.StrutCurrentLength) && FMath::IsFinite(Wheel.Context.StrutCurrentVelocity)
			&& Wheel.Context.StrutCurrentLength >= 0.f
			&& Wheel.Context.StrutCurrentLength <= Wheel.Context.TransientStrutLengthLimit + 1.e-4f;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehiclePairedSuspensionZeroBarTest,
	"KinetiForge.SuspensionPair.ZeroBarScalarEquivalence", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FVehiclePairedSuspensionZeroBarTest::RunTest(const FString& Parameters)
{
	using namespace VehiclePairedSuspensionTest;
	for (const float Mass : { 0.f, 40.f })
	{
		FWheel Left(480.f, Mass), Right(520.f, 65.f);
		Left.Spring.SpringStiffness = 350.f;
		Right.Spring.SpringStiffness = 280.f;
		Left.Spring.ReboundDamping = 600.f;
		Right.Spring.CompressionDamping = 750.f;
		Left.Projection = 0.7f;
		for (int32 Frame = 0; Frame < 60; ++Frame)
		{
			Left.Prepare(1.f / 60.f);
			Right.Prepare(1.f / 60.f);
			FWheel ScalarLeft = Left, ScalarRight = Right;
			for (FWheel* Wheel : { &ScalarLeft, &ScalarRight })
				FVehicleSuspensionSolver::IntegrateSingleWheelSuspension(Wheel->Context, Wheel->Chassis,
					0.f, Wheel->Radius, Wheel->Inertia, Wheel->Kinematics, Wheel->Spring, Wheel->LUTs, 0.f, 0.f);
			FVehicleSuspensionSolver::IntegratePairedIndependentSuspension(Left.Input(), Right.Input(), 0.f);
			TestEqual(TEXT("Zero bar left length is exactly scalar"), Left.Context.StrutCurrentLength, ScalarLeft.Context.StrutCurrentLength);
			TestEqual(TEXT("Zero bar left velocity is exactly scalar"), Left.Context.StrutCurrentVelocity, ScalarLeft.Context.StrutCurrentVelocity);
			TestEqual(TEXT("Zero bar right length is exactly scalar"), Right.Context.StrutCurrentLength, ScalarRight.Context.StrutCurrentLength);
			TestEqual(TEXT("Zero bar right velocity is exactly scalar"), Right.Context.StrutCurrentVelocity, ScalarRight.Context.StrutCurrentVelocity);
		}
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehiclePairedSuspensionEnergyTest,
	"KinetiForge.SuspensionPair.PassivityAndMomentum", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FVehiclePairedSuspensionEnergyTest::RunTest(const FString& Parameters)
{
	using namespace VehiclePairedSuspensionTest;
	for (const float Bar : { 20.f, 10000.f, 1.e7f, 1.e9f })
	for (const float Dt : { 1.f / 30.f, 1.f / 60.f, 1.f / 120.f, 1.f / 240.f })
	{
		FWheel Left(498.f, 40.f), Right(502.f, 65.f);
		Left.Context.StrutCurrentVelocity = 5.f;
		Right.Context.StrutCurrentVelocity = -3.f;
		double PreviousEnergy = Energy(Left, Right, Bar);
		for (int32 Frame = 0; Frame < 120; ++Frame)
		{
			Step(Left, Right, Dt, Bar);
			const double CurrentEnergy = Energy(Left, Right, Bar);
			// Float state publication can round the last few ulps; permit 0.1% plus 1 mJ.
			if (!TestTrue(TEXT("Unforced coupled suspension dissipates rather than creates energy"),
				CurrentEnergy <= PreviousEnergy * 1.001 + 0.001)) return false;
			if (!TestTrue(TEXT("Internal bar conserves total linear momentum"),
				FMath::Abs(40.0 * Left.Context.StrutCurrentVelocity + 65.0 * Right.Context.StrutCurrentVelocity - 5.0) < 0.02)) return false;
			if (!TestTrue(TEXT("Pair remains finite and within stroke"), FiniteAndBounded(Left) && FiniteAndBounded(Right))) return false;
			PreviousEnergy = CurrentEnergy;
		}
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehiclePairedSuspensionSymmetryTest,
	"KinetiForge.SuspensionPair.ExchangeAndDamping", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FVehiclePairedSuspensionSymmetryTest::RunTest(const FString& Parameters)
{
	using namespace VehiclePairedSuspensionTest;
	FWheel Left(480.f, 40.f), Right(620.f, 70.f);
	Left.Projection = 0.8f;
	Right.Projection = 0.6f;
	Left.Spring.ReboundDamping = 800.f;
	Left.Spring.CompressionDamping = 400.f;
	Right.Spring.ReboundDamping = 300.f;
	Right.Spring.CompressionDamping = 900.f;
	FWheel SwappedLeft = Right, SwappedRight = Left;
	double PreviousEnergy = Energy(Left, Right, 1.e6);
	for (int32 Frame = 0; Frame < 240; ++Frame)
	{
		Step(Left, Right, 1.f / 120.f, 1.e6f);
		Step(SwappedLeft, SwappedRight, 1.f / 120.f, 1.e6f);
		const double CurrentEnergy = Energy(Left, Right, 1.e6);
		TestTrue(TEXT("Asymmetric compression/rebound damping remains passive"),
			CurrentEnergy <= PreviousEnergy * 1.001 + 0.001);
		PreviousEnergy = CurrentEnergy;
		TestTrue(TEXT("Exchanging wheel order preserves left solution"),
			FMath::IsNearlyEqual(Left.Context.StrutCurrentLength, SwappedRight.Context.StrutCurrentLength, 0.0001f));
		TestTrue(TEXT("Exchanging wheel order preserves right solution"),
			FMath::IsNearlyEqual(Right.Context.StrutCurrentVelocity, SwappedLeft.Context.StrutCurrentVelocity, 0.0001f));
	}
	FWheel SymmetricLeft, SymmetricRight;
	SymmetricLeft.Context.StrutCurrentVelocity = SymmetricRight.Context.StrutCurrentVelocity = 10.f;
	Step(SymmetricLeft, SymmetricRight, 1.f / 60.f, 1.e9f);
	TestEqual(TEXT("Symmetric motion does not excite the bar"), SymmetricLeft.Context.StrutCurrentLength, SymmetricRight.Context.StrutCurrentLength);
	TestTrue(TEXT("Bar leaves common velocity unchanged"), FMath::IsNearlyEqual(SymmetricLeft.Context.StrutCurrentVelocity, 10.f, 0.0001f));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehiclePairedSuspensionConstraintTest,
	"KinetiForge.SuspensionPair.ContactStrokeAndMassless", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FVehiclePairedSuspensionConstraintTest::RunTest(const FString& Parameters)
{
	using namespace VehiclePairedSuspensionTest;
	FWheel Left(490.f), Right(510.f);
	Left.Context.bHitGround = true;
	Left.Context.TransientStrutLengthLimit = 494.f;
	Left.Context.HitDistance = Left.Radius + 494.f;
	bool bReachedGroundLimit = false;
	for (int32 Frame = 0; Frame < 120; ++Frame)
	{
		const double Before = Energy(Left, Right, 1.e7);
		Step(Left, Right, 1.f / 60.f, 1.e7f);
		bReachedGroundLimit |= Left.Context.bWheelOnGround;
		if (!TestTrue(TEXT("Stationary unilateral contact is passive"), Energy(Left, Right, 1.e7) <= Before * 1.001 + 0.001)) return false;
		TestTrue(TEXT("Single grounded side does not violate either limit"), FiniteAndBounded(Left) && FiniteAndBounded(Right));
		TestFalse(TEXT("Airborne side stays airborne"), Right.Context.bWheelOnGround);
	}
	// With no gravity or preload, the pair may rebound and leave this unilateral surface.
	TestTrue(TEXT("Grounded side reaches its query limit during the transient"), bReachedGroundLimit);
	for (const float OtherMass : { 0.f, 40.f })
	{
		FWheel Prescribed(495.f, 0.f), Dynamic(510.f, OtherMass);
		Prescribed.Context.HitDistance = Prescribed.Radius + 495.f;
		Prescribed.Context.TransientStrutLengthLimit = 495.f;
		for (int32 Frame = 0; Frame < 120; ++Frame)
		{
			Step(Prescribed, Dynamic, 1.f / 60.f, 1.e8f);
			TestEqual(TEXT("Massless side follows geometry without division by mass"), Prescribed.Context.StrutCurrentLength, 495.f);
			TestTrue(TEXT("Mixed or fully massless pair stays finite"), FiniteAndBounded(Prescribed) && FiniteAndBounded(Dynamic));
		}
		TestEqual(TEXT("Massless side retains zero virtual mass"), Prescribed.Context.VirtualUnsprungMass, 0.f);
	}
	// A moving ground limit can demand work; test feasibility, not unforced energy conservation.
	FWheel Bump(1.f), Droop(999.f);
	Bump.Context.StrutCurrentVelocity = -10000.f;
	Droop.Context.StrutCurrentVelocity = 10000.f;
	Step(Bump, Droop, 1.f / 30.f, 100.f);
	TestTrue(TEXT("Bump and droop stops remain feasible"), FiniteAndBounded(Bump) && FiniteAndBounded(Droop));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehiclePairedSuspensionConvergenceTest,
	"KinetiForge.SuspensionPair.AnalyticConvergence", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FVehiclePairedSuspensionConvergenceTest::RunTest(const FString& Parameters)
{
	using namespace VehiclePairedSuspensionTest;
	const double Duration = 0.25;
	const double ExactDifference = 4.0 * FMath::Cos(10.0 * Duration); // sqrt(100*k*(1/mL+1/mR)) = 10 rad/s.
	double PreviousError = 1.e10;
	for (const int32 Frequency : { 480, 960, 1920 })
	{
		FWheel Left(502.f), Right(498.f);
		for (int32 Frame = 0; Frame < Frequency / 4; ++Frame) Step(Left, Right, 1.f / Frequency, 20.f);
		const double Error = FMath::Abs(double(Left.Context.StrutCurrentLength - Right.Context.StrutCurrentLength) - ExactDifference);
		AddInfo(FString::Printf(TEXT("%d Hz analytic relative-displacement error: %.9f cm"), Frequency, Error));
		TestTrue(TEXT("Reducing timestep reduces error against analytic oscillator"), Error < PreviousError * 0.8);
		PreviousError = Error;
	}
	TestTrue(TEXT("Finest solution within 0.03 cm of analytic relative displacement"), PreviousError < 0.03);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
