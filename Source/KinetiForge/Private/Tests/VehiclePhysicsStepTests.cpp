// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "VehiclePhysicsStep.h"
#include "Async/Async.h"
#include "HAL/Event.h"
#include "HAL/PlatformAtomics.h"
#include "HAL/PlatformProcess.h"
#include "Misc/AutomationTest.h"

namespace
{
	struct FScopedStepEvents
	{
		FEvent* Entered = FPlatformProcess::GetSynchEventFromPool(true);
		FEvent* Release = FPlatformProcess::GetSynchEventFromPool(true);
		FEvent* ExecuteDone = FPlatformProcess::GetSynchEventFromPool(true);
		FEvent* StopStarted = FPlatformProcess::GetSynchEventFromPool(true);
		FEvent* StopDone = FPlatformProcess::GetSynchEventFromPool(true);

		~FScopedStepEvents()
		{
			// Always unblock callbacks before returning pooled events, including assertion failures.
			Release->Trigger();
			for (FEvent* Event : {Entered, Release, ExecuteDone, StopStarted, StopDone})
			{
				FPlatformProcess::ReturnSynchEventToPool(Event);
			}
		}
	};

	constexpr uint32 StepTestTimeoutMs = 5000;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehiclePhysicsStepAdmissionTest,
	"KinetiForge.Drivetrain.PhysicsStepAdmission",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehiclePhysicsStepAdmissionTest::RunTest(const FString& Parameters)
{
	int32 UpdateCount = 0;
	TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe> Step = MakeShared<FVehiclePhysicsStep, ESPMode::ThreadSafe>(
		[&UpdateCount](float) { FPlatformAtomics::InterlockedIncrement(&UpdateCount); });
	Step->SetAutomaticUpdateEnabled(false);
	TestFalse(TEXT("disabled automatic callback is rejected"), Step->TryExecute(0.01f, true));
	TestTrue(TEXT("manual callback remains available"), Step->TryExecute(0.01f, false));
	TestEqual(TEXT("manual callback ran exactly once"), UpdateCount, 1);

	for (bool bInvalidateBinding : {false, true})
	{
		FScopedStepEvents Events;
		TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe> BlockingStep = MakeShared<FVehiclePhysicsStep, ESPMode::ThreadSafe>(
			[&Events](float)
			{
				Events.Entered->Trigger();
				Events.Release->Wait(StepTestTimeoutMs);
			});

		TFuture<bool> ExecuteFuture = Async(EAsyncExecution::Thread, [BlockingStep, &Events]()
		{
			const bool bExecuted = BlockingStep->TryExecute(0.02f);
			Events.ExecuteDone->Trigger();
			return bExecuted;
		});
		if (!Events.Entered->Wait(StepTestTimeoutMs))
		{
			TestFalse(TEXT("step callback entered before timeout"), true);
			Events.Release->Trigger();
			Events.ExecuteDone->Wait(StepTestTimeoutMs);
			ExecuteFuture.Wait();
			return false;
		}

		TestFalse(TEXT("reentrant/manual call is rejected while callback runs"), BlockingStep->TryExecute(0.02f));
		TestFalse(TEXT("automatic call is also rejected while callback runs"), BlockingStep->TryExecute(0.02f, true));

		TFuture<void> StopFuture = Async(EAsyncExecution::Thread, [BlockingStep, &Events, bInvalidateBinding]()
		{
			Events.StopStarted->Trigger();
			if (bInvalidateBinding) BlockingStep->InvalidateBindingAndWait();
			else BlockingStep->StopAndWait();
			Events.StopDone->Trigger();
		});
		if (!Events.StopStarted->Wait(StepTestTimeoutMs))
		{
			TestFalse(TEXT("stop caller started before timeout"), true);
		}
		// The callback is still blocked, so StopAndWait must not complete yet.
		TestFalse(TEXT("stop waits for active callback"), Events.StopDone->Wait(100));

		Events.Release->Trigger();
		TestTrue(TEXT("active execution completed"), Events.ExecuteDone->Wait(StepTestTimeoutMs));
		TestTrue(TEXT("stop completed after callback drained"), Events.StopDone->Wait(StepTestTimeoutMs));
		ExecuteFuture.Wait();
		StopFuture.Wait();
		TestTrue(TEXT("the old entry is stopped"), BlockingStep->IsStopped());
		TestEqual(TEXT("Dependency stop remains eligible for GT recovery"), BlockingStep->IsBindingInvalidated(), bInvalidateBinding);
		BlockingStep->StopAndWait();
		BlockingStep->InvalidateBindingAndWait();
		TestFalse(TEXT("An explicit stop cannot become recoverable again"), BlockingStep->IsBindingInvalidated());
		TestFalse(TEXT("old shared snapshot remains closed"), BlockingStep->TryExecute(0.02f));

		int32 ReplacementCount = 0;
		TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe> Replacement = MakeShared<FVehiclePhysicsStep, ESPMode::ThreadSafe>(
			[&ReplacementCount](float) { FPlatformAtomics::InterlockedIncrement(&ReplacementCount); });
		TestTrue(TEXT("a newly registered entry can execute"), Replacement->TryExecute(0.03f));
		TestEqual(TEXT("replacement callback ran once"), ReplacementCount, 1);
	}
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
