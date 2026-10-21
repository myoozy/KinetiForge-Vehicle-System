// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "VehicleWheelBridge.h"
#include "Async/Async.h"
#include "HAL/Event.h"
#include "HAL/PlatformProcess.h"
#include "HAL/ThreadSafeBool.h"
#include "Misc/AutomationTest.h"

namespace VehicleWheelBridgeTest
{
	FVehicleWheelRuntimeSettings MakeSettings(float Radius, float Friction)
	{
		FVehicleWheelConfig Wheel;
		Wheel.Radius = Radius;
		FVehicleTireConfig Tire;
		Tire.FrictionMultiplier = Friction;
		FVehicleABSConfig ABS;
		FVehicleSuspensionKinematicsConfig Kinematics;
		FVehicleSuspensionSpringConfig Spring;
		FVehicleWheelCachedLUTs TireLUTs;
		FVehicleSuspensionCachedLUTs SuspensionLUTs;
		return FVehicleWheelRuntimeSettings::FromCachedConfig(
			Wheel, Tire, ABS, Kinematics, Spring, TireLUTs, SuspensionLUTs);
	}

	FVehicleWheelRuntime MakeRuntime()
	{
		FVehicleWheelRuntime Runtime;
		Runtime.WheelState.AngularVelocity = 21.f;
		Runtime.WheelState.AngularAcceleration = -3.f;
		Runtime.WheelState.P4MotorTorque = 2.f;
		Runtime.WheelState.BrakeTorqueFromESP = 4.f;
		Runtime.WheelState.TireForce = FVector3f(9.f, 8.f, 7.f);
		Runtime.SuspensionState.StaticSprungMass = 250.f;
		Runtime.SuspensionState.ImpactWorldLocation = FVector(1.f, 2.f, 3.f);
		Runtime.SuspensionState.StrutCurrentLength = 44.f;
		Runtime.WheelContext.MacroDeltaTime = 0.017f;
		Runtime.WheelContext.AccumulateTireImpulse2D = FVector2f(5.f, -6.f);
		return Runtime;
	}

	FVehicleWheelRuntime MakeConcurrentRuntime(float StateValue)
	{
		FVehicleWheelRuntime Runtime;
		Runtime.WheelState.AngularVelocity = StateValue;
		Runtime.SuspensionState.ImpactWorldLocation.X = StateValue * 10.f;
		Runtime.SuspensionState.StaticSprungMass = StateValue + 200.f;
		Runtime.WheelContext.MacroDeltaTime = (StateValue + 1.f) * 1.e-6f;
		return Runtime;
	}

	bool IsConsistent(const FVehicleWheelResult& R, int32 Offset, FString& OutReason)
	{
		const float V = R.WheelState.AngularVelocity;
		const float Step = V - static_cast<float>(Offset);
		if (!FMath::IsNearlyEqual(R.SuspensionState.ImpactWorldLocation.X, V * 10.f, 1.e-3f))
		{
			OutReason = TEXT("wheel and suspension snapshots came from different publishes");
			return false;
		}
		if (!FMath::IsNearlyEqual(R.SuspensionState.StaticSprungMass, V + 200.f, 1.e-3f))
		{
			OutReason = TEXT("mass and wheel state came from different publishes");
			return false;
		}
		const float ExpectedDt = (Step + 1.f + static_cast<float>(Offset)) * 1.e-6f;
		if (!FMath::IsNearlyEqual(R.MacroDeltaTime, ExpectedDt, 1.e-7f))
		{
			OutReason = TEXT("macro timestep and wheel state came from different publishes");
			return false;
		}
		return true;
	}

	struct FEventOwner
	{
		FEvent* Ready = FPlatformProcess::GetSynchEventFromPool(false);
		FEvent* Done = FPlatformProcess::GetSynchEventFromPool(false);
		~FEventOwner()
		{
			if (Ready) FPlatformProcess::ReturnSynchEventToPool(Ready);
			if (Done) FPlatformProcess::ReturnSynchEventToPool(Done);
		}
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleWheelBridgeLatestInputsTest,
	"KinetiForge.WheelRuntime.Bridge.LatestInputs",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleWheelBridgeLatestInputsTest::RunTest(const FString& Parameters)
{
	using namespace VehicleWheelBridgeTest;
	(void)Parameters;
	FVehicleWheelBridge Bridge(MakeRuntime());
	const FVehicleWheelResult InitiallyPublished = Bridge.ReadResult();

	Bridge.SubmitSettings(MakeSettings(30.f, 0.8f));
	Bridge.SubmitSettings(MakeSettings(41.f, 1.7f));
	Bridge.RequestP4MotorTorque(11.f);
	Bridge.RequestP4MotorTorque(23.f);
	Bridge.RequestEspBrakeTorque(19.f);
	Bridge.RequestEspBrakeTorque(31.f);
	Bridge.RequestSprungMass(80.f);
	Bridge.RequestSprungMass(-2.f);
	const FVehicleWheelResult BeforeConsume = Bridge.ReadResult();
	TestTrue(TEXT("Submit and Read do not advance or publish simulation state"),
		BeforeConsume.WheelState.AngularVelocity == InitiallyPublished.WheelState.AngularVelocity
		&& BeforeConsume.WheelState.TireForce == InitiallyPublished.WheelState.TireForce
		&& BeforeConsume.SuspensionState.StaticSprungMass == InitiallyPublished.SuspensionState.StaticSprungMass
		&& BeforeConsume.MacroDeltaTime == InitiallyPublished.MacroDeltaTime);

	Bridge.ConsumeInputs();
	FVehicleWheelRuntime& Runtime = Bridge.GetRuntimeForSimulation();
	TestTrue(TEXT("Latest settings win independently"), Runtime.Settings.WheelConfig.Radius == 41.f
		&& Runtime.Settings.TireConfig.FrictionMultiplier == 1.7f);
	TestTrue(TEXT("P4 and ESP command values are independently consumed"),
		Runtime.WheelState.P4MotorTorque == 23.f && Runtime.WheelState.BrakeTorqueFromESP == 31.f);
	TestTrue(TEXT("Nonpositive sprung mass preserves legacy zero behavior"),
		Runtime.SuspensionState.StaticSprungMass == 0.f);
	TestTrue(TEXT("Consuming settings and commands preserves existing history"),
		Runtime.WheelState.AngularVelocity == 21.f && Runtime.WheelState.AngularAcceleration == -3.f
		&& Runtime.WheelState.TireForce == FVector3f(9.f, 8.f, 7.f)
		&& Runtime.WheelContext.MacroDeltaTime == 0.017f
		&& Runtime.WheelContext.AccumulateTireImpulse2D == FVector2f(5.f, -6.f)
		&& Runtime.SuspensionState.ImpactWorldLocation == FVector(1.f, 2.f, 3.f)
		&& Runtime.SuspensionState.StrutCurrentLength == 44.f);
	Bridge.RequestSprungMass(80.f);
	Bridge.ConsumeInputs();
	TestEqual(TEXT("Positive sprung mass is applied"), Runtime.SuspensionState.StaticSprungMass, 80.f);
	Bridge.RequestSprungMass(0.f);
	Bridge.ConsumeInputs();
	TestEqual(TEXT("Zero sprung mass is applied as zero"), Runtime.SuspensionState.StaticSprungMass, 0.f);
	Bridge.RequestSprungMass(-2.f);
	Bridge.ConsumeInputs();
	TestEqual(TEXT("Negative sprung mass is clamped to zero"), Runtime.SuspensionState.StaticSprungMass, 0.f);

	// Advance/publish once, then consume an empty mailbox. The prior published value must
	// not be replayed into simulation state as if it were a new input.
	Runtime.WheelState.AngularVelocity = 52.f;
	Runtime.WheelState.TireForce = FVector3f(12.f, 13.f, 14.f);
	Runtime.WheelContext.MacroDeltaTime = 0.009f;
	Runtime.SuspensionState.ImpactWorldLocation = FVector(6.f, 7.f, 8.f);
	Bridge.PublishResult();
	Runtime.WheelState.AngularVelocity = 53.f;
	Bridge.ConsumeInputs();
	const FVehicleWheelResult AfterEmptyConsume = Bridge.ReadResult();
	TestTrue(TEXT("Empty consume does not rewind from the last published result"),
		Runtime.WheelState.AngularVelocity == 53.f
		&& Runtime.WheelState.P4MotorTorque == 23.f
		&& Runtime.WheelState.BrakeTorqueFromESP == 31.f
		&& Runtime.WheelContext.MacroDeltaTime == 0.009f
		&& AfterEmptyConsume.WheelState.AngularVelocity == 52.f);

	const FVector3f StateForceBeforeSettings = Runtime.WheelState.TireForce;
	Bridge.SubmitSettings(MakeSettings(52.f, 0.9f));
	Bridge.ConsumeInputs();
	TestTrue(TEXT("Applying settings preserves wheel history and commands"),
		Runtime.WheelState.AngularVelocity == 53.f && Runtime.WheelState.TireForce == StateForceBeforeSettings
		&& Runtime.WheelState.P4MotorTorque == 23.f && Runtime.WheelState.BrakeTorqueFromESP == 31.f);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleWheelBridgeConcurrentSnapshotTest,
	"KinetiForge.WheelRuntime.Bridge.ConcurrentSnapshots",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleWheelBridgeConcurrentSnapshotTest::RunTest(const FString& Parameters)
{
	using namespace VehicleWheelBridgeTest;
	(void)Parameters;
	constexpr int32 NumSteps = 64;
	constexpr uint32 TimeoutMs = 5000;
	TSharedPtr<FVehicleWheelBridge, ESPMode::ThreadSafe> BridgeA =
		MakeShared<FVehicleWheelBridge, ESPMode::ThreadSafe>(MakeConcurrentRuntime(-1.f));
	TSharedPtr<FVehicleWheelBridge, ESPMode::ThreadSafe> BridgeB =
		MakeShared<FVehicleWheelBridge, ESPMode::ThreadSafe>(MakeConcurrentRuntime(999.f));
	FEventOwner Events;
	if (!TestNotNull(TEXT("Consumer ready event created"), Events.Ready)
		|| !TestNotNull(TEXT("Consumer completion event created"), Events.Done)) return false;
	FThreadSafeBool bStop(false);
	FThreadSafeBool bWorkerTimedOut(false);
	TFuture<void> Consumer = Async(EAsyncExecution::ThreadPool,
		[BridgeA, BridgeB, &Events, &bStop, &bWorkerTimedOut, NumSteps, TimeoutMs]()
		{
			for (int32 Step = 0; Step < NumSteps; ++Step)
			{
				if (!Events.Ready->Wait(TimeoutMs))
				{
					bWorkerTimedOut = true;
					return;
				}
				if (bStop) return;
				BridgeA->ConsumeInputs();
				BridgeB->ConsumeInputs();
				const float A = static_cast<float>(Step);
				const float B = A + 1000.f;
				auto& RuntimeA = BridgeA->GetRuntimeForSimulation();
				auto& RuntimeB = BridgeB->GetRuntimeForSimulation();
				RuntimeA.WheelState.AngularVelocity = A;
				RuntimeA.SuspensionState.ImpactWorldLocation.X = A * 10.f;
				RuntimeA.SuspensionState.StaticSprungMass = A + 200.f;
				RuntimeA.WheelContext.MacroDeltaTime = (A + 1.f) * 1.e-6f;
				RuntimeB.WheelState.AngularVelocity = B;
				RuntimeB.SuspensionState.ImpactWorldLocation.X = B * 10.f;
				RuntimeB.SuspensionState.StaticSprungMass = B + 200.f;
				RuntimeB.WheelContext.MacroDeltaTime = (B + 1.f) * 1.e-6f;
				BridgeA->PublishResult();
				BridgeB->PublishResult();
				Events.Done->Trigger();
			}
		});

	bool bTimedOut = false;
	for (int32 Step = 0; Step < NumSteps; ++Step)
	{
		BridgeA->RequestP4MotorTorque(static_cast<float>(Step));
		BridgeB->RequestP4MotorTorque(-static_cast<float>(Step));
		BridgeA->RequestSprungMass(300.f + Step);
		BridgeB->RequestSprungMass(600.f + Step);
		BridgeA->SubmitSettings(MakeSettings(30.f + Step, 1.f));
		BridgeB->SubmitSettings(MakeSettings(130.f + Step, 2.f));
		(void)BridgeA->ReadResult();
		(void)BridgeB->ReadResult();
		Events.Ready->Trigger();
		// Read while the worker may be consuming and publishing; each copied result must be coherent.
		for (int32 ReadIndex = 0; ReadIndex < 12; ++ReadIndex)
		{
			FString Reason;
			const FVehicleWheelResult ResultA = BridgeA->ReadResult();
			const FVehicleWheelResult ResultB = BridgeB->ReadResult();
			TestTrue(TEXT("Bridge A publication is a coherent snapshot"), IsConsistent(ResultA, 0, Reason));
			if (!Reason.IsEmpty()) AddError(Reason);
			Reason.Reset();
			TestTrue(TEXT("Bridge B publication is independent and coherent"), IsConsistent(ResultB, 1000, Reason));
			if (!Reason.IsEmpty()) AddError(Reason);
		}
		if (!Events.Done->Wait(TimeoutMs))
		{
			AddError(FString::Printf(TEXT("Simulation consumer timed out at step %d; requesting worker shutdown"), Step));
			bTimedOut = true;
			bStop = true;
			Events.Ready->Trigger();
			break;
		}
	}
	if (bTimedOut)
	{
		// The ready signal releases a worker waiting for the next step; join before returning events to the pool.
		bStop = true;
		Events.Ready->Trigger();
	}
	Consumer.Wait();
	TestFalse(TEXT("Worker did not time out waiting for a producer step"), bWorkerTimedOut);
	if (!bTimedOut)
	{
		const FVehicleWheelResult ResultA = BridgeA->ReadResult();
		const FVehicleWheelResult ResultB = BridgeB->ReadResult();
		FString Reason;
		TestTrue(TEXT("Bridge A has the final coherent state"), IsConsistent(ResultA, 0, Reason));
		if (!Reason.IsEmpty()) AddError(Reason);
		Reason.Reset();
		TestTrue(TEXT("Bridge B has the final independent state"), IsConsistent(ResultB, 1000, Reason));
		if (!Reason.IsEmpty()) AddError(Reason);
		TestEqual(TEXT("Bridge A received its own last P4 command"), ResultA.WheelState.P4MotorTorque, 63.f);
		TestEqual(TEXT("Bridge B received its own last P4 command"), ResultB.WheelState.P4MotorTorque, -63.f);
		TestEqual(TEXT("Bridge A settings remain independent"), BridgeA->GetRuntimeForSimulation().Settings.WheelConfig.Radius, 93.f);
		TestEqual(TEXT("Bridge B settings remain independent"), BridgeB->GetRuntimeForSimulation().Settings.WheelConfig.Radius, 193.f);
	}
	return !bTimedOut;
}

#endif // WITH_DEV_AUTOMATION_TESTS
