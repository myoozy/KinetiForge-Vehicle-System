// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "VehiclePhysicsSchedule.h"
#include "VehicleAsyncTickComponent.h"
#include "VehicleDriveAssemblyComponent.h"
#include "Async/Async.h"
#include "HAL/Event.h"
#include "HAL/PlatformProcess.h"
#include "HAL/PlatformTime.h"
#include "Misc/AutomationTest.h"
#include "UObject/StrongObjectPtr.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehiclePhysicsScheduleSnapshotTest,
	"KinetiForge.Drivetrain.PhysicsSchedule.Snapshot",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehiclePhysicsScheduleSnapshotTest::RunTest(const FString& Parameters)
{
	FVehiclePhysicsSchedule Schedule;
	TArray<int32> Order;
	FVehiclePhysicsSchedule::FStepArray InitialSteps;
	for (int32 Value : {1, 2, 3})
	{
		InitialSteps.Add(MakeShared<FVehiclePhysicsStep, ESPMode::ThreadSafe>(
			[&Order, Value](float) { Order.Add(Value); }));
	}
	FVehiclePhysicsSchedule::FStepArray Previous = Schedule.PublishSteps(MoveTemp(InitialSteps));
	TestEqual(TEXT("First publish replaces an empty snapshot"), Previous.Num(), 0);
	FVehiclePhysicsSchedule::FStepArray Captured = Schedule.CaptureSteps();
	FVehiclePhysicsSchedule::ExecuteCapturedSteps(Captured, 1.f / 60.f, false);
	TestEqual(TEXT("Captured schedule executes each drive once"), Order.Num(), 3);
	if (Order.Num() == 3)
	{
		TestEqual(TEXT("Drive insertion order is preserved at index zero"), Order[0], 1);
		TestEqual(TEXT("Drive insertion order is preserved at index one"), Order[1], 2);
		TestEqual(TEXT("Drive insertion order is preserved at index two"), Order[2], 3);
	}

	Schedule.ClearAndStop();
	TestEqual(TEXT("Clear publishes an empty schedule"), Schedule.CaptureSteps().Num(), 0);
	FVehiclePhysicsSchedule::ExecuteCapturedSteps(Captured, 1.f / 60.f, false);
	TestEqual(TEXT("Old captured snapshots cannot execute after ClearAndStop"), Order.Num(), 3);

	FEvent* Entered = FPlatformProcess::GetSynchEventFromPool(true);
	FEvent* Release = FPlatformProcess::GetSynchEventFromPool(true);
	FEvent* ClearStarted = FPlatformProcess::GetSynchEventFromPool(true);
	FEvent* ClearDone = FPlatformProcess::GetSynchEventFromPool(true);
	FEvent* PublishDone = FPlatformProcess::GetSynchEventFromPool(true);
	constexpr uint32 CallbackTimeoutMs = 10000;
	constexpr uint32 DrainPublishTimeoutMs = 1000;
	bool bEntered = false;
	bool bStoppedWhileExecuting = false;
	bool bPublishCompletedDuringDrain = false;
	bool bCleared = false;
	TFuture<void> ExecuteFuture;
	TFuture<void> ClearFuture;
	TFuture<void> PublishFuture;
	TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe> BlockingStep = MakeShared<FVehiclePhysicsStep, ESPMode::ThreadSafe>(
		[Entered, Release, CallbackTimeoutMs](float)
		{
			Entered->Trigger();
			Release->Wait(CallbackTimeoutMs);
		});
	FVehiclePhysicsSchedule::FStepArray BlockingSteps;
	BlockingSteps.Add(BlockingStep);
	Schedule.PublishSteps(MoveTemp(BlockingSteps));
	FVehiclePhysicsSchedule::FStepArray BlockingSnapshot = Schedule.CaptureSteps();
	ExecuteFuture = Async(EAsyncExecution::Thread, [BlockingSnapshot]()
	{
		FVehiclePhysicsSchedule::ExecuteCapturedSteps(BlockingSnapshot, 0.02f, false);
	});
	bEntered = Entered->Wait(CallbackTimeoutMs);
	if (bEntered)
	{
		ClearFuture = Async(EAsyncExecution::Thread, [&Schedule, ClearStarted, ClearDone]()
		{
			ClearStarted->Trigger();
			Schedule.ClearAndStop();
			ClearDone->Trigger();
		});
		if (ClearStarted->Wait(CallbackTimeoutMs))
		{
			const double Deadline = FPlatformTime::Seconds() + 2.0;
			while (!BlockingStep->IsStopped() && FPlatformTime::Seconds() < Deadline)
			{
				FPlatformProcess::YieldThread();
			}
			bStoppedWhileExecuting = BlockingStep->IsStopped() && !ClearDone->Wait(0);
			if (bStoppedWhileExecuting)
			{
				TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe> Replacement = MakeShared<FVehiclePhysicsStep, ESPMode::ThreadSafe>([](float) {});
				PublishFuture = Async(EAsyncExecution::Thread, [&Schedule, Replacement, PublishDone]()
				{
					FVehiclePhysicsSchedule::FStepArray NewSteps;
					NewSteps.Add(Replacement);
					Schedule.PublishSteps(MoveTemp(NewSteps));
					PublishDone->Trigger();
				});
				bPublishCompletedDuringDrain = PublishDone->Wait(DrainPublishTimeoutMs) && !ClearDone->Wait(0);
			}
		}
	}
	// Always release the running callback before waiting or returning, even on timeout.
	Release->Trigger();
	if (ExecuteFuture.IsValid()) ExecuteFuture.Wait();
	if (ClearFuture.IsValid()) ClearFuture.Wait();
	if (PublishFuture.IsValid()) PublishFuture.Wait();
	const bool bExecutionCompleted = ExecuteFuture.IsValid();
	const bool bClearCompleted = !ClearFuture.IsValid() || ClearDone->Wait(0);
	const bool bPublisherCompleted = !PublishFuture.IsValid() || PublishDone->Wait(0);
	bCleared = ClearDone->Wait(0);
	TestTrue(TEXT("schedule callback entered"), bEntered);
	TestTrue(TEXT("ClearAndStop closes the active entry and waits for its callback"), bStoppedWhileExecuting);
	TestTrue(TEXT("schedule publication remains available while ClearAndStop drains"), bPublishCompletedDuringDrain);
	TestTrue(TEXT("running captured callback completes after release"), bExecutionCompleted);
	TestTrue(TEXT("ClearAndStop finishes after the active callback drains"), bClearCompleted && bCleared);
	TestTrue(TEXT("replacement snapshot publisher completes"), bPublisherCompleted);

	for (FEvent* Event : {Entered, Release, ClearStarted, ClearDone, PublishDone})
	{
		FPlatformProcess::ReturnSynchEventToPool(Event);
	}
	return true;
}

struct FVehicleAsyncTickBeginDestroyProbe
{
	static void RegisterDriveStep(UVehicleAsyncTickComponent& Ticker, UVehicleDriveAssemblyComponent& Drive,
		const TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe>& Step)
	{
		Ticker.DriveAssemblies.AddUnique(&Drive);
		Ticker.DrivePhysicsSteps.Add(&Drive, Step);
	}
};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleAsyncTickBeginDestroyScheduleTest,
	"KinetiForge.Drivetrain.PhysicsSchedule.TickerBeginDestroy",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleAsyncTickBeginDestroyScheduleTest::RunTest(const FString& Parameters)
{
	TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe> RetainedStep;
	TWeakObjectPtr<UVehicleAsyncTickComponent> CollectedTicker;
	{
		TStrongObjectPtr<UVehicleAsyncTickComponent> Ticker(NewObject<UVehicleAsyncTickComponent>());
		TStrongObjectPtr<UVehicleDriveAssemblyComponent> Drive(NewObject<UVehicleDriveAssemblyComponent>());
		CollectedTicker = Ticker.Get();
		RetainedStep = MakeShared<FVehiclePhysicsStep, ESPMode::ThreadSafe>([](float) {});
		FVehicleAsyncTickBeginDestroyProbe::RegisterDriveStep(*Ticker, *Drive, RetainedStep);
	}

	CollectGarbage(RF_NoFlags);
	TestFalse(TEXT("Unregistered ticker was collected"), CollectedTicker.IsValid());
	TestTrue(TEXT("BeginDestroy stops an entry from the ticker's Drive registration map"), RetainedStep->IsStopped());
	TestFalse(TEXT("Retained registered Drive step cannot execute after ticker destruction"), RetainedStep->TryExecute(1.f / 60.f));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
