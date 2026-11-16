// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehicleWheelRuntime.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "VehicleWheelBridge.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleWheelSpinMomentumTest,
	"KinetiForge.Wheel.Momentum.Exchange",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleWheelSpinMomentumTest::RunTest(const FString& Parameters)
{
	const float Dt = 1.f / 120.f;
	FVehicleWheelRuntime Wheel;
	Wheel.Settings.WheelConfig.Inertia = 2.f;
	Wheel.Settings.WheelConfig.RollingRisistance = 0.f;
	Wheel.SuspensionState.bHitGround = false;
	Wheel.SuspensionState.bWheelOnGround = false;
	Wheel.SuspensionState.ImpactWorldNormal = FVector3f::UpVector;
	Wheel.SuspensionState.WheelWorldRightVector = FVector3f::RightVector;
	Wheel.WheelState.AngularVelocity = 100.f;
	Wheel.PrepareWheel(Dt, FTransform::Identity);
	Wheel.FinalizeWheel();
	TestTrue(TEXT("Initial spinning wheel seeds orientation without a kick"),
		Wheel.WheelContext.SpinReactionAngularImpulse.IsNearlyZero());

	// Two unequal substeps: last-substep acceleration is deliberately not the macro average.
	Wheel.PrepareWheel(Dt, FTransform::Identity);
	Wheel.IntegrateWheel(Dt * 0.25f, 60.f, 0.f, 0.f, 8.f);
	Wheel.IntegrateWheel(Dt * 0.75f, 20.f, 0.f, 0.f, 8.f);
	const float SpeedAfterDrive = Wheel.WheelState.AngularVelocity;
	Wheel.FinalizeWheel();
	const FVector ExpectedDriveReaction(0., -2. * (SpeedAfterDrive - 100.f), 0.);
	TestTrue(TEXT("Drive reaction balances actual wheel momentum, excluding reflected inertia"),
		Wheel.WheelContext.SpinReactionAngularImpulse.Equals(ExpectedDriveReaction, 1.e-6));
	TestEqual(TEXT("AngularAcceleration only describes the final substep"),
		Wheel.WheelState.AngularAcceleration, 2.f, 0.01f);

	Wheel.PrepareWheel(Dt, FTransform::Identity);
	Wheel.IntegrateWheel(Dt, 0.f, 100000.f, 0.f, 0.f);
	Wheel.FinalizeWheel();
	TestTrue(TEXT("Brake lock transfers the remaining wheel spin to the chassis"),
		Wheel.WheelState.AngularVelocity == 0.f
		&& Wheel.WheelContext.SpinReactionAngularImpulse.Equals(
			FVector(0., 2. * SpeedAfterDrive, 0.), 1.e-5));

	// Both signs and arbitrary 3D axis motion, even with no ground contact.
	for (const float Speed : { -90.f, 90.f })
	{
		Wheel.ResetSpinMomentumHistory();
		Wheel.WheelState.AngularVelocity = Speed;
		Wheel.SuspensionState.WheelWorldRightVector = FVector3f::RightVector;
		Wheel.PrepareWheel(Dt, FTransform::Identity);
		Wheel.FinalizeWheel();
		const FVector OldMomentum = FVector::RightVector * (2. * Speed);
		const FVector NewAxis = FVector(0.2, 0.9, 0.3).GetSafeNormal();
		Wheel.SuspensionState.WheelWorldRightVector = FVector3f(NewAxis);
		Wheel.PrepareWheel(Dt, FTransform::Identity);
		Wheel.FinalizeWheel();
		const FVector NewMomentum = FVector(Wheel.SuspensionState.WheelWorldRightVector).GetSafeNormal() * (2. * Speed);
		TestTrue(TEXT("Axis swing conserves sampled total angular momentum in flight"),
			(OldMomentum - NewMomentum - Wheel.WheelContext.SpinReactionAngularImpulse).IsNearlyZero(1.e-5));
	}

	Wheel.ResetSpinMomentumHistory();
	Wheel.SuspensionState.WheelWorldRightVector = FVector3f::UpVector;
	Wheel.PrepareWheel(Dt, FTransform::Identity);
	Wheel.FinalizeWheel();
	TestTrue(TEXT("Pose reset discards old gyro history"), Wheel.WheelContext.SpinReactionAngularImpulse.IsNearlyZero());
	Wheel.SuspensionState.WheelWorldRightVector = FVector3f::ZeroVector;
	Wheel.PrepareWheel(Dt, FTransform::Identity);
	Wheel.FinalizeWheel();
	TestTrue(TEXT("Uninitialized axis produces no impulse"), Wheel.WheelContext.SpinReactionAngularImpulse.IsNearlyZero());

	FVehicleWheelBridge Bridge(Wheel);
	Bridge.RequestSuspensionReset(Wheel.SuspensionState);
	Bridge.ConsumeInputs();
	FVehicleWheelRuntime& ResetWheel = Bridge.GetRuntimeForSimulation();
	ResetWheel.SuspensionState.WheelWorldRightVector = FVector3f::ForwardVector;
	ResetWheel.PrepareWheel(Dt, FTransform::Identity);
	ResetWheel.FinalizeWheel();
	TestTrue(TEXT("Bridge suspension reset also resets spin history"), ResetWheel.WheelContext.SpinReactionAngularImpulse.IsNearlyZero());
	return true;
}

#endif
