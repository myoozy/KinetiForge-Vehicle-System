// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "VehicleDriveAssemblyComponent.h"
#include "VehicleAsyncTickComponent.h"
#include "VehicleAxleAssemblyComponent.h"
#include "VehicleWheelComponent.h"
#include "VehicleWheelBridge.h"
#include "VehicleEngineComponent.h"
#include "VehicleGearboxComponent.h"
#include "KinetiForge.h"
#include "VehiclePhysicsScene.h"
#include "VehiclePhysicsStep.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "HAL/Event.h"
#include "HAL/PlatformAtomics.h"
#include "HAL/PlatformProcess.h"
#include "HAL/PlatformTime.h"
#include "HAL/PlatformTLS.h"
#include "Misc/AutomationTest.h"
#include "Misc/ScopeExit.h"
#include "Modules/ModuleManager.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "PBDRigidsSolver.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"
#include "Chaos/Particle/ParticleUtilities.h"
#include "UObject/GarbageCollection.h"

struct FVehicleFixedDriveAsyncLifecycleProbe
{
	static TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe> Step(UVehicleDriveAssemblyComponent& Drive)
	{
		return Drive.GetPhysicsStep();
	}
	static TSharedPtr<FVehiclePhysicsScene, ESPMode::ThreadSafe> Scene(UVehicleDriveAssemblyComponent& Drive)
	{
		return Drive.PhysicsScene;
	}
	static void Tick(UVehicleDriveAssemblyComponent& Drive)
	{
		Drive.TickComponent(1.f / 60.f, LEVELTICK_All, nullptr);
	}
};

namespace VehicleFixedDriveAsyncLifecycleTests
{
	constexpr uint32 WaitTimeoutMs = 5000;

	struct FWorldOwner
	{
		UWorld* World = nullptr;
		~FWorldOwner() { if (World) World->DestroyWorld(false); }
	};

	UWorld* CreateWorld(FAutomationTestBase& Test, FWorldOwner& Owner, const FName Name)
	{
		Owner.World = UWorld::CreateWorld(EWorldType::Game, false, Name, nullptr,
			true, ERHIFeatureLevel::Num, nullptr, true);
		if (!Test.TestNotNull(TEXT("TaskGraph physics world created"), Owner.World)) return nullptr;
		Owner.World->InitWorld(UWorld::InitializationValues().AllowAudioPlayback(false)
			.CreatePhysicsScene(true).RequiresHitProxies(false).CreateNavigation(false)
			.CreateAISystem(false).ShouldSimulatePhysics(true).SetTransactional(false));
		if (!Test.TestNotNull(TEXT("TaskGraph world has Chaos scene"), Owner.World->GetPhysicsScene())) return nullptr;
		auto* Solver = Owner.World->GetPhysicsScene()->GetSolver();
		Solver->DisableAsyncMode(); // Keep frame handoff deterministic while Chaos advances on TaskGraph workers.
		Solver->SetThreadingMode_External(Chaos::EThreadingModeTemp::TaskGraph);
		return Owner.World;
	}

	void BeginFrame(UWorld& World)
	{
		const FVector Gravity(0.f, 0.f, -980.f);
		World.GetPhysicsScene()->SetUpForFrame(&Gravity, 1.f / 60.f, 0.f, 1.f / 60.f, 1.f / 60.f, 1, false);
		World.GetPhysicsScene()->StartFrame();
	}

	void Flush(UWorld& World)
	{
		BeginFrame(World);
		World.GetPhysicsScene()->WaitPhysScenes();
		World.GetPhysicsScene()->EndFrame();
	}

	struct FVehicle
	{
		APawn* Actor = nullptr;
		UBoxComponent* Chassis = nullptr;
		UVehicleAxleAssemblyComponent* Axle = nullptr;
		UVehicleDriveAssemblyComponent* Drive = nullptr;
	};

	FVehicle CreateVehicle(UWorld& World)
	{
		FVehicle Vehicle;
		Vehicle.Actor = World.SpawnActor<APawn>(FVector(0.f, 0.f, 1000.f), FRotator::ZeroRotator);
		if (!Vehicle.Actor) return Vehicle;
		Vehicle.Chassis = NewObject<UBoxComponent>(Vehicle.Actor, TEXT("AsyncLifecycleChassis"));
		Vehicle.Actor->AddInstanceComponent(Vehicle.Chassis);
		Vehicle.Actor->SetRootComponent(Vehicle.Chassis);
		Vehicle.Chassis->SetBoxExtent(FVector(50.f));
		Vehicle.Chassis->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Vehicle.Chassis->SetSimulatePhysics(true);
		Vehicle.Chassis->RegisterComponent();
		Vehicle.Chassis->RecreatePhysicsState();

		Vehicle.Axle = NewObject<UVehicleAxleAssemblyComponent>(Vehicle.Actor, TEXT("AsyncLifecycleAxle"));
		Vehicle.Actor->AddInstanceComponent(Vehicle.Axle);
		Vehicle.Axle->SetupAttachment(Vehicle.Chassis);
		Vehicle.Axle->RegisterComponent();

		Vehicle.Drive = NewObject<UVehicleDriveAssemblyComponent>(Vehicle.Actor, TEXT("AsyncLifecycleDrive"));
		Vehicle.Actor->AddInstanceComponent(Vehicle.Drive);
		Vehicle.Drive->SetupAttachment(Vehicle.Chassis);
		// Exercise the production default rather than opting the fixture into it.
		Vehicle.Drive->bUpdatePhysicsAutomatically = true;
		Vehicle.Drive->AutoGearboxConfig.bAutomaticGearbox = false;
		FVehicleAxleSpawnTemplate AxleTemplate;
		AxleTemplate.bUseExistingComponent = true;
		AxleTemplate.AxleComponentName = Vehicle.Axle->GetFName();
		Vehicle.Drive->AxleTemplates = { AxleTemplate };
		Vehicle.Drive->RegisterComponent();
		return Vehicle;
	}

	bool StartVehicle(FAutomationTestBase& Test, FVehicle& Vehicle)
	{
		if (!Test.TestTrue(TEXT("Vehicle components registered"), Vehicle.Actor && Vehicle.Chassis && Vehicle.Axle && Vehicle.Drive)) return false;
		if (!Test.TestTrue(TEXT("New Drive uses the whole-vehicle runtime by default"), Vehicle.Drive->bUseIsolatedVehicleRuntime)) return false;
		Vehicle.Actor->DispatchBeginPlay();
		FVehicleFixedDriveAsyncLifecycleProbe::Tick(*Vehicle.Drive);
		const auto Entry = FVehicleFixedDriveAsyncLifecycleProbe::Step(*Vehicle.Drive);
		if (!Test.TestTrue(TEXT("First eligible GT refresh publishes an active default runtime"),
			Entry.IsValid() && !Entry->IsStopped())) return false;
		Test.TestFalse(TEXT("Default runtime cannot be advanced by a context-free legacy step call"), Entry->TryExecute(1.f / 60.f));
		Vehicle.Drive->InputThrottle(0.7f, false);
		Vehicle.Drive->GetEngine()->StartVehicleEngine();
		Vehicle.Drive->GetGearbox()->ShiftToTargetGear(1, true);
		return true;
	}

	struct FGateEvents
	{
		FEvent* Entered = FPlatformProcess::GetSynchEventFromPool(true);
		FEvent* Release = FPlatformProcess::GetSynchEventFromPool(true);
		FEvent* ReleaserDone = FPlatformProcess::GetSynchEventFromPool(true);
		~FGateEvents()
		{
			Release->Trigger();
			for (FEvent* Event : {Entered, Release, ReleaserDone}) FPlatformProcess::ReturnSynchEventToPool(Event);
		}
	};

	struct FDrainObservation
	{
		FGateEvents Events;
		volatile int32 CallbackThreadId = -1;
		volatile int32 SawStopBeforeRelease = 0;
		volatile int32 UnregisterReturned = 0;
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleFixedDriveTaskGraphLifecycleTest,
	"KinetiForge.Drivetrain.FixedDrive.TaskGraphLifecycle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleFixedDriveTaskGraphLifecycleTest::RunTest(const FString& Parameters)
{
	using namespace VehicleFixedDriveAsyncLifecycleTests;
	FWorldOwner Owner;
	UWorld* World = CreateWorld(*this, Owner, TEXT("VehicleFixedDriveTaskGraphLifecycle"));
	if (!World) return false;
	FVehicle Vehicle = CreateVehicle(*World);
	Flush(*World);
	if (!StartVehicle(*this, Vehicle)) return false;
	Flush(*World); // Ensure the registered Drive callback and its captured body identities are consumed.
	FVehicleFixedDriveAsyncLifecycleProbe::Tick(*Vehicle.Drive);
	auto OldStep = FVehicleFixedDriveAsyncLifecycleProbe::Step(*Vehicle.Drive);
	auto Host = FVehicleFixedDriveAsyncLifecycleProbe::Scene(*Vehicle.Drive);
	if (!TestTrue(TEXT("Isolated Drive has active scene step"), OldStep.IsValid() && !OldStep->IsStopped())
		|| !TestNotNull(TEXT("Drive owns scene host"), Host.Get())) return false;

	volatile int32 WorkerCalls = 0;
	volatile int32 WorkerThreadId = -1;
	const int32 GameThreadId = static_cast<int32>(FPlatformTLS::GetCurrentThreadId());
	auto Witness = Host->RegisterStep([&WorkerCalls, &WorkerThreadId](float, const FVehiclePhysicsExecutionContext&)
	{
		FPlatformAtomics::InterlockedIncrement(&WorkerCalls);
		FPlatformAtomics::InterlockedCompareExchange(&WorkerThreadId,
			static_cast<int32>(FPlatformTLS::GetCurrentThreadId()), -1);
	});
	if (!TestNotNull(TEXT("Post-Drive TaskGraph witness step registered"), Witness.Get())) return false;
	ON_SCOPE_EXIT { Host->UnregisterStep(Witness); };
	TArray<UVehicleWheelComponent*> Wheels = Vehicle.Drive->GetWheels();
	if (!TestEqual(TEXT("Runtime observer has two wheels"), Wheels.Num(), 2)) return false;
	const auto WheelBridge = Wheels[0]->GetWheelBridge();
	if (!TestNotNull(TEXT("Wheel result bridge is available"), WheelBridge.Get())) return false;
	uint64 PreviousPublication = WheelBridge->ReadResult().PublicationSequence;
	for (int32 Frame = 0; Frame < 4; ++Frame)
	{
		Flush(*World);
		Vehicle.Drive->GetEngine()->RefreshPublishedRuntimeState();
		const uint64 NewPublication = WheelBridge->ReadResult().PublicationSequence;
		TestEqual(FString::Printf(TEXT("Wheel publishes once on TaskGraph frame %d"), Frame),
			NewPublication, PreviousPublication + 1);
		PreviousPublication = NewPublication;
	}
	TestTrue(TEXT("Chaos invoked the registered callback on a non-GT TaskGraph worker"),
		FPlatformAtomics::InterlockedCompareExchange(&WorkerCalls, 0, 0) >= 4
		&& FPlatformAtomics::InterlockedCompareExchange(&WorkerThreadId, -1, -1) != GameThreadId);
	TestTrue(TEXT("The actual fixed Drive advanced its engine through the scene callback"), Vehicle.Drive->GetEngine()->GetRPM() > 0.f);

	const float RPMBeforeBodyRebuild = Vehicle.Drive->GetEngine()->GetRPM();
	// Old serialized switches must not restore the removed production component path,
	// including when a subsequent body recreation rebuilds the complete entry.
	Vehicle.Drive->bUseIsolatedVehicleRuntime = false;
	Vehicle.Drive->bUseIsolatedPowerUnitRuntime = false;
	Vehicle.Chassis->RecreatePhysicsState();
	Flush(*World);
	Vehicle.Drive->GetEngine()->RefreshPublishedRuntimeState();
	FVehicleFixedDriveAsyncLifecycleProbe::Tick(*Vehicle.Drive);
	auto RebuiltStep = FVehicleFixedDriveAsyncLifecycleProbe::Step(*Vehicle.Drive);
	TestTrue(TEXT("Body recreation closes old entry and publishes a new one"), OldStep->IsStopped()
		&& RebuiltStep.IsValid() && RebuiltStep != OldStep && !RebuiltStep->IsStopped());
	TestFalse(TEXT("Retained old Drive entry cannot restart"), OldStep->TryExecute(1.f / 60.f));
	Flush(*World);
	Vehicle.Drive->GetEngine()->RefreshPublishedRuntimeState();
	TestTrue(TEXT("Drive state survives body rebinding and resumes on TaskGraph"),
		Vehicle.Drive->GetEngine()->GetRPM() > 0.f && RPMBeforeBodyRebuild > 0.f);

	const auto BeforeWheelUnregister = FVehicleFixedDriveAsyncLifecycleProbe::Step(*Vehicle.Drive);
	Wheels = Vehicle.Drive->GetWheels();
	if (!TestEqual(TEXT("Lifecycle fixture has two wheels"), Wheels.Num(), 2)) return false;
	Wheels[1]->UnregisterComponent();
	TestTrue(TEXT("Wheel unregister invalidates the currently shared entry"), BeforeWheelUnregister->IsStopped());
	FVehicleFixedDriveAsyncLifecycleProbe::Tick(*Vehicle.Drive);
	TestTrue(TEXT("Unregistered dependency does not let old entry resume"),
		FVehicleFixedDriveAsyncLifecycleProbe::Step(*Vehicle.Drive)->IsStopped());
	Wheels[1]->RegisterComponent();
	FVehicleFixedDriveAsyncLifecycleProbe::Tick(*Vehicle.Drive);
	auto RecoveredStep = FVehicleFixedDriveAsyncLifecycleProbe::Step(*Vehicle.Drive);
	TestTrue(TEXT("Wheel registration publishes a fresh runnable entry"), RecoveredStep.IsValid()
		&& RecoveredStep != BeforeWheelUnregister && !RecoveredStep->IsStopped());
	Flush(*World);
	Vehicle.Drive->GetEngine()->RefreshPublishedRuntimeState();

	Wheels = Vehicle.Drive->GetWheels();
	Wheels[1]->DestroyComponent();
	TestTrue(TEXT("Destroying one wheel closes the old pair entry"), RecoveredStep->IsStopped());
	FVehicleFixedDriveAsyncLifecycleProbe::Tick(*Vehicle.Drive);
	auto SingleWheelStep = FVehicleFixedDriveAsyncLifecycleProbe::Step(*Vehicle.Drive);
	TestTrue(TEXT("Remaining wheel is rebound through a fresh entry"), SingleWheelStep.IsValid()
		&& SingleWheelStep != RecoveredStep && !SingleWheelStep->IsStopped());
	Flush(*World);
	Vehicle.Drive->GetEngine()->RefreshPublishedRuntimeState();
	TestTrue(TEXT("TaskGraph callback continues after single-wheel degradation"),
		FPlatformAtomics::InterlockedCompareExchange(&WorkerCalls, 0, 0) >= 7);

	Vehicle.Drive->UnregisterComponent();
	TestTrue(TEXT("Drive unregister drains the retained step"), SingleWheelStep->IsStopped());
	Vehicle.Drive->RegisterComponent();
	FVehicleFixedDriveAsyncLifecycleProbe::Tick(*Vehicle.Drive);
	auto ReRegisteredStep = FVehicleFixedDriveAsyncLifecycleProbe::Step(*Vehicle.Drive);
	TestTrue(TEXT("Drive re-register creates a fresh entry"), ReRegisteredStep.IsValid()
		&& ReRegisteredStep != SingleWheelStep && !ReRegisteredStep->IsStopped());
	Flush(*World);

	TestTrue(TEXT("Drive entry is active immediately before world cleanup"), !ReRegisteredStep->IsStopped());
	FPhysScene_Chaos* SceneIdentity = World->GetPhysicsScene();
	Owner.World->DestroyWorld(false);
	Owner.World = nullptr;
	TestTrue(TEXT("WorldCleanup closes retained Drive and witness entries and host"),
		ReRegisteredStep->IsStopped() && Witness->IsStopped() && Host->IsClosed());
	TestNull(TEXT("Closed host refuses a replacement entry"), Host->RegisterStep(
		[](float, const FVehiclePhysicsExecutionContext&) {}).Get());
	CollectGarbage(RF_NoFlags);
	FKinetiForgeModule& Module = FModuleManager::Get().GetModuleChecked<FKinetiForgeModule>(TEXT("KinetiForge"));
	TestNull(TEXT("SceneTerm removes its host registry entry after GC"), Module.FindPhysicsScene(SceneIdentity).Get());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleFixedDriveTaskGraphDrainTest,
	"KinetiForge.Drivetrain.FixedDrive.TaskGraphDrain",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleFixedDriveTaskGraphDrainTest::RunTest(const FString& Parameters)
{
	using namespace VehicleFixedDriveAsyncLifecycleTests;
	FWorldOwner Owner;
	UWorld* World = CreateWorld(*this, Owner, TEXT("VehicleFixedDriveTaskGraphDrain"));
	if (!World) return false;
	FKinetiForgeModule& Module = FModuleManager::Get().GetModuleChecked<FKinetiForgeModule>(TEXT("KinetiForge"));
	auto Host = Module.AcquirePhysicsScene(World->GetPhysicsScene());
	if (!TestNotNull(TEXT("Direct TaskGraph host acquired"), Host.Get())) return false;
	TSharedRef<FDrainObservation, ESPMode::ThreadSafe> Observation = MakeShared<FDrainObservation, ESPMode::ThreadSafe>();
	FVehiclePhysicsScene::FStepPtr Step;
	bool bFrameStarted = false;
	ON_SCOPE_EXIT
	{
		Observation->Events.Release->Trigger();
		if (Step) Host->UnregisterStep(Step);
		if (bFrameStarted)
		{
			World->GetPhysicsScene()->WaitPhysScenes();
			World->GetPhysicsScene()->EndFrame();
		}
		Host->Close();
	};
	// First consume callback creation with an empty schedule. Registering the gate before
	// this flush would block Flush itself and leave a stale Entered signal.
	Flush(*World);
	Step = Host->RegisterStep([Observation](float, const FVehiclePhysicsExecutionContext&)
	{
		FPlatformAtomics::InterlockedCompareExchange(&Observation->CallbackThreadId,
			static_cast<int32>(FPlatformTLS::GetCurrentThreadId()), -1);
		Observation->Events.Entered->Trigger();
		Observation->Events.Release->Wait(WaitTimeoutMs * 2);
	});
	if (!TestNotNull(TEXT("Blocking context step registered"), Step.Get())) return false;
	const FVector Gravity(0.f, 0.f, -980.f);
	World->GetPhysicsScene()->SetUpForFrame(&Gravity, 1.f / 60.f, 0.f, 1.f / 60.f, 1.f / 60.f, 1, false);
	World->GetPhysicsScene()->StartFrame();
	bFrameStarted = true;
	// The solver may need its frozen-GT pre-simulate tasks before it can enter the
	// worker gate. Service those tasks while waiting; a blocking GT event wait would
	// prevent the prerequisite from running and manufacture a timeout.
	const double EnterDeadline = FPlatformTime::Seconds() + double(WaitTimeoutMs) / 1000.0;
	bool bCallbackEntered = Observation->Events.Entered->Wait(0);
	while (!bCallbackEntered && FPlatformTime::Seconds() < EnterDeadline)
	{
		FTaskGraphInterface::Get().ProcessThreadUntilIdle(ENamedThreads::GameThread);
		FPlatformProcess::Sleep(0.001f);
		bCallbackEntered = Observation->Events.Entered->Wait(0);
	}
	if (!bCallbackEntered)
	{
		TestFalse(TEXT("Worker callback entered before timeout"), true);
		return false;
	}
	TestTrue(TEXT("Drain callback is executing away from GT"),
		FPlatformAtomics::InterlockedCompareExchange(&Observation->CallbackThreadId, -1, -1)
		!= static_cast<int32>(FPlatformTLS::GetCurrentThreadId()));

	auto Releaser = Async(EAsyncExecution::Thread, [Observation, Step]()
	{
		const double Deadline = FPlatformTime::Seconds() + double(WaitTimeoutMs) / 1000.0;
		while ((!Step->IsStopped() || FPlatformAtomics::InterlockedCompareExchange(&Observation->UnregisterReturned, 0, 0) != 0)
			&& FPlatformTime::Seconds() < Deadline)
			FPlatformProcess::Sleep(0.001f);
		if (Step->IsStopped() && FPlatformAtomics::InterlockedCompareExchange(&Observation->UnregisterReturned, 0, 0) == 0)
			FPlatformAtomics::InterlockedExchange(&Observation->SawStopBeforeRelease, 1);
		Observation->Events.Release->Trigger(); // Always release, even when stop admission was not observed in time.
		Observation->Events.ReleaserDone->Trigger();
	});
	const bool bRemoved = Host->UnregisterStep(Step); // GT blocks here until the in-flight callback exits.
	FPlatformAtomics::InterlockedExchange(&Observation->UnregisterReturned, 1);
	const bool bReleaserFinished = Observation->Events.ReleaserDone->Wait(WaitTimeoutMs);
	if (!bReleaserFinished) Observation->Events.Release->Trigger();
	Releaser.Wait(); // Joined before returning pooled events or leaving the host/world scope.
	World->GetPhysicsScene()->WaitPhysScenes();
	World->GetPhysicsScene()->EndFrame(); // Chaos requires completion events to be done before EndFrame.
	bFrameStarted = false;
	TestTrue(TEXT("Unregister removed the active step"), bRemoved);
	TestTrue(TEXT("Releaser observed closed admission before unblocking callback"),
		FPlatformAtomics::InterlockedCompareExchange(&Observation->SawStopBeforeRelease, 0, 0) == 1);
	TestTrue(TEXT("Stop drained the callback and closed the old entry"), Step->IsStopped());
	Host->Close();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleFixedDriveMultipleBodiesTest,
	"KinetiForge.Drivetrain.FixedDrive.MultipleBodies",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleFixedDriveMultipleBodiesTest::RunTest(const FString& Parameters)
{
	using namespace VehicleFixedDriveAsyncLifecycleTests;
	FWorldOwner Owner;
	UWorld* World = CreateWorld(*this, Owner, TEXT("VehicleFixedDriveMultipleBodies"));
	if (!World) return false;
	FVehicle Vehicle = CreateVehicle(*World);
	if (!Vehicle.Drive) return false;
	Vehicle.Drive->UnregisterComponent();

	// Two independent bodies under one Actor/Drive. This tests drivetrain binding
	// across body boundaries, without adding an articulation constraint or new topology.
	UBoxComponent* RearBody = NewObject<UBoxComponent>(Vehicle.Actor, TEXT("RearSectionBody"));
	Vehicle.Actor->AddInstanceComponent(RearBody);
	RearBody->SetBoxExtent(FVector(50.f));
	RearBody->SetWorldLocationAndRotation(FVector(500.f, 0.f, 1200.f), FRotator(0.f, 35.f, 0.f));
	RearBody->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RearBody->SetSimulatePhysics(true);
	RearBody->RegisterComponent();
	RearBody->RecreatePhysicsState();
	UVehicleAxleAssemblyComponent* RearAxle = NewObject<UVehicleAxleAssemblyComponent>(Vehicle.Actor, TEXT("RearSectionAxle"));
	Vehicle.Actor->AddInstanceComponent(RearAxle);
	RearAxle->SetupAttachment(RearBody);
	RearAxle->RegisterComponent();
	FVehicleAxleSpawnTemplate RearTemplate;
	RearTemplate.bUseExistingComponent = true;
	RearTemplate.AxleComponentName = RearAxle->GetFName();
	Vehicle.Drive->AxleTemplates.Add(RearTemplate);
	Vehicle.Drive->RegisterComponent();
	Flush(*World);
	if (!StartVehicle(*this, Vehicle)) return false;
	if (!TestTrue(TEXT("Multi-body fixture has distinct body positions"),
		FVector::Dist(RearBody->GetComponentLocation(), Vehicle.Chassis->GetComponentLocation()) > 400.f)
		|| !TestFalse(TEXT("Multi-body fixture has distinct body rotations"),
			RearBody->GetComponentQuat().Equals(Vehicle.Chassis->GetComponentQuat(), 1.e-3f))) return false;
	const TArray<UVehicleWheelComponent*> Wheels = Vehicle.Drive->GetWheels();
	if (!TestEqual(TEXT("One Drive binds four wheels across two bodies"), Wheels.Num(), 4)) return false;
	auto Host = FVehicleFixedDriveAsyncLifecycleProbe::Scene(*Vehicle.Drive);
	if (!TestNotNull(TEXT("Multi-body Drive has a scene host"), Host.Get())) return false;

	struct FWheelBodyObservation
	{
		TSharedPtr<FVehicleWheelBridge, ESPMode::ThreadSafe> Bridge;
		FVehiclePhysicsBodyIdentity BodyIdentity;
	};
	volatile int32 MismatchedFrames = 0;
	volatile int32 ObservedFrames = 0;
	volatile int32 WorkerThreadId = -1;
	FVehiclePhysicsScene::FStepPtr Witness;
	ON_SCOPE_EXIT { if (Witness) Host->UnregisterStep(Witness); };
	const auto RegisterWitness = [&]()
	{
		TArray<FWheelBodyObservation> Observations;
		for (UVehicleAxleAssemblyComponent* Axle : {Vehicle.Axle, RearAxle})
		{
			UVehicleWheelComponent* Left = nullptr;
			UVehicleWheelComponent* Right = nullptr;
			Axle->GetWheels(Left, Right);
			UBoxComponent* Body = Axle == RearAxle ? RearBody : Vehicle.Chassis;
			for (UVehicleWheelComponent* Wheel : {Left, Right})
			{
				if (Wheel) Observations.Add({Wheel->GetWheelBridge(), FVehiclePhysicsBodyIdentity::Capture(Body->GetBodyInstance())});
			}
		}
		// Registered after Drive, so published wheel frames must match their own
		// body's current PT transform in the same pre-simulate phase.
		return Host->RegisterStep([Observations, &MismatchedFrames, &ObservedFrames, &WorkerThreadId]
			(float, const FVehiclePhysicsExecutionContext& Context)
		{
			FPlatformAtomics::InterlockedExchange(&WorkerThreadId, static_cast<int32>(FPlatformTLS::GetCurrentThreadId()));
			for (const auto& Observation : Observations)
			{
				auto* Body = Context.ResolveBody(Observation.BodyIdentity);
				if (!Body || !Observation.Bridge || !Observation.Bridge->ReadResult().ChassisWorldTransform.Equals(
					Chaos::FParticleUtilitiesGT::GetActorWorldTransform(Body), 1.e-3f))
					FPlatformAtomics::InterlockedIncrement(&MismatchedFrames);
			}
			FPlatformAtomics::InterlockedIncrement(&ObservedFrames);
		});
	};
	Witness = RegisterWitness();
	const auto VerifyAdvance = [&]()
	{
		TArray<uint64> PreviousSequences;
		for (auto* Wheel : Wheels) PreviousSequences.Add(Wheel->GetWheelBridge()->ReadResult().PublicationSequence);
		Flush(*World);
		for (int32 WheelIndex = 0; WheelIndex < Wheels.Num(); ++WheelIndex)
			TestEqual(TEXT("Each section's wheel publishes exactly once"),
				Wheels[WheelIndex]->GetWheelBridge()->ReadResult().PublicationSequence, PreviousSequences[WheelIndex] + 1);
	};
	for (int32 Frame = 0; Frame < 3; ++Frame) VerifyAdvance();
	TestTrue(TEXT("Multi-body witness executes off the game thread"),
		FPlatformAtomics::InterlockedCompareExchange(&WorkerThreadId, -1, -1)
		!= static_cast<int32>(FPlatformTLS::GetCurrentThreadId()));

	Host->UnregisterStep(Witness);
	const auto PreviousDriveStep = FVehicleFixedDriveAsyncLifecycleProbe::Step(*Vehicle.Drive);
	RearBody->RecreatePhysicsState();
	FVehicleFixedDriveAsyncLifecycleProbe::Tick(*Vehicle.Drive);
	TestTrue(TEXT("Recreating only the rear body closes the previous whole-Drive entry"), PreviousDriveStep->IsStopped());
	Flush(*World);
	FVehicleFixedDriveAsyncLifecycleProbe::Tick(*Vehicle.Drive);
	const auto ReboundDriveStep = FVehicleFixedDriveAsyncLifecycleProbe::Step(*Vehicle.Drive);
	if (!TestTrue(TEXT("Rear body recreation publishes a fresh active Drive entry"), ReboundDriveStep.IsValid()
		&& ReboundDriveStep != PreviousDriveStep && !ReboundDriveStep->IsStopped())) return false;
	TestTrue(TEXT("Body rebinding preserves all four wheel component identities"), Vehicle.Drive->GetWheels() == Wheels);
	Witness = RegisterWitness();
	for (int32 Frame = 0; Frame < 3; ++Frame) VerifyAdvance();
	TestEqual(TEXT("Both body bindings are checked before and after rear recreation"),
		FPlatformAtomics::InterlockedCompareExchange(&ObservedFrames, 0, 0), 6);
	TestEqual(TEXT("Every wheel uses its own body's current position and rotation"),
		FPlatformAtomics::InterlockedCompareExchange(&MismatchedFrames, 0, 0), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleFixedDriveFixedAsyncTest,
	"KinetiForge.Drivetrain.FixedDrive.FixedAsyncFrameBoundary",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

static bool RunFixedAsyncFrameBoundary(FAutomationTestBase& Test, float FixedDeltaTime)
{
	using namespace VehicleFixedDriveAsyncLifecycleTests;
	FWorldOwner Owner;
	UWorld* World = CreateWorld(Test, Owner, TEXT("VehicleFixedDriveFixedAsync"));
	if (!World) return false;
	FVehicle Vehicle = CreateVehicle(*World);
	Flush(*World);
	if (!StartVehicle(Test, Vehicle)) return false;
	Flush(*World);
	FVehicleFixedDriveAsyncLifecycleProbe::Tick(*Vehicle.Drive);
	auto Host = FVehicleFixedDriveAsyncLifecycleProbe::Scene(*Vehicle.Drive);
	if (!Test.TestNotNull(TEXT("Fixed async Drive has a scene host"), Host.Get())) return false;
	const auto Wheels = Vehicle.Drive->GetWheels();
	if (!Test.TestEqual(TEXT("Fixed async fixture has two wheels"), Wheels.Num(), 2)) return false;
	const auto WheelBridge = Wheels[0]->GetWheelBridge();
	if (!Test.TestNotNull(TEXT("Fixed async wheel bridge exists"), WheelBridge.Get())) return false;
	auto* Solver = World->GetPhysicsScene()->GetSolver();
	Solver->EnableAsyncMode(FixedDeltaTime);
	Test.TestTrue(TEXT("Solver really uses fixed asynchronous results"), Solver->IsUsingAsyncResults());

	const auto AdvanceGameFrame = [World, FixedDeltaTime](float GameDeltaTime)
	{
		const FVector Gravity(0.f, 0.f, -980.f);
		World->GetPhysicsScene()->SetUpForFrame(&Gravity, GameDeltaTime, 0.f, 1.f / 15.f,
			FixedDeltaTime, 8, false);
		World->GetPhysicsScene()->StartFrame();
		// In fixed async mode this waits the engine's frame-completion prerequisites,
		// not necessarily the current frame's last solver task.
		World->GetPhysicsScene()->WaitPhysScenes();
		World->GetPhysicsScene()->EndFrame();
	};
	// Settle mode transition and registration before installing a controlled worker gate.
	AdvanceGameFrame(FixedDeltaTime);
	Solver->WaitOnPendingTasks_External();
	const uint64 InitialPublication = WheelBridge->ReadResult().PublicationSequence;
	TSharedRef<FDrainObservation, ESPMode::ThreadSafe> Observation = MakeShared<FDrainObservation, ESPMode::ThreadSafe>();
	volatile int32 PhysicsCalls = 0;
	volatile int32 IncorrectDeltaTimes = 0;
	volatile int32 BlockNextStep = 1;
	auto Witness = Host->RegisterStep([&PhysicsCalls, &IncorrectDeltaTimes, &BlockNextStep, Observation, FixedDeltaTime]
		(float DeltaTime, const FVehiclePhysicsExecutionContext&)
	{
		FPlatformAtomics::InterlockedIncrement(&PhysicsCalls);
		if (!FMath::IsNearlyEqual(DeltaTime, FixedDeltaTime, 1.e-6f))
			FPlatformAtomics::InterlockedIncrement(&IncorrectDeltaTimes);
		FPlatformAtomics::InterlockedExchange(&Observation->CallbackThreadId,
			static_cast<int32>(FPlatformTLS::GetCurrentThreadId()));
		if (FPlatformAtomics::InterlockedCompareExchange(&BlockNextStep, 0, 1) == 1)
		{
			Observation->Events.Entered->Trigger();
			Observation->Events.Release->Wait(WaitTimeoutMs * 2);
		}
	});
	if (!Test.TestNotNull(TEXT("Fixed async worker witness registered"), Witness.Get())) return false;
	ON_SCOPE_EXIT
	{
		Observation->Events.Release->Trigger();
		if (Owner.World) Solver->WaitOnPendingTasks_External();
		Host->UnregisterStep(Witness);
	};

	AdvanceGameFrame(FixedDeltaTime);
	const double EnterDeadline = FPlatformTime::Seconds() + double(WaitTimeoutMs) / 1000.0;
	bool bEntered = Observation->Events.Entered->Wait(0);
	while (!bEntered && FPlatformTime::Seconds() < EnterDeadline)
	{
		FTaskGraphInterface::Get().ProcessThreadUntilIdle(ENamedThreads::GameThread);
		FPlatformProcess::Sleep(0.001f);
		bEntered = Observation->Events.Entered->Wait(0);
	}
	if (!Test.TestTrue(TEXT("Actual worker enters the controlled fixed-async callback"), bEntered)) return false;
	Test.TestFalse(TEXT("Game frame returned while its current solver task is still executing"), Solver->IsPendingTasksComplete());
	Test.TestTrue(TEXT("Fixed async callback executes away from GT"),
		FPlatformAtomics::InterlockedCompareExchange(&Observation->CallbackThreadId, -1, -1)
		!= static_cast<int32>(FPlatformTLS::GetCurrentThreadId()));
	const uint64 CommandSequence = WheelBridge->RequestP4MotorTorque(25.f);
	Test.TestTrue(TEXT("GT submits a mailbox command while current solver task is still blocked"),
		WheelBridge->ReadResult().AppliedP4Sequence < CommandSequence);
	Observation->Events.Release->Trigger();
	Solver->WaitOnPendingTasks_External();

	bool bObservedNoPhysicsStep = false;
	bool bObservedMultiplePhysicsSteps = false;
	for (float GameDeltaTime : {FixedDeltaTime * 0.5f, FixedDeltaTime * 0.5f, 1.f / 30.f, FixedDeltaTime * 0.5f, FixedDeltaTime * 0.5f, 1.f / 60.f})
	{
		const int32 PreviousCalls = FPlatformAtomics::InterlockedCompareExchange(&PhysicsCalls, 0, 0);
		AdvanceGameFrame(GameDeltaTime);
		// Drain only for deterministic observations; the first gated frame above proved
		// real overlap across the game-frame completion boundary.
		Solver->WaitOnPendingTasks_External();
		const int32 CurrentCalls = FPlatformAtomics::InterlockedCompareExchange(&PhysicsCalls, 0, 0);
		bObservedNoPhysicsStep |= CurrentCalls == PreviousCalls;
		bObservedMultiplePhysicsSteps |= CurrentCalls - PreviousCalls > 1;
		for (auto* Wheel : Wheels)
			Test.TestEqual(TEXT("Each wheel publishes once per physics callback, independent of GT frame count"),
				Wheel->GetWheelBridge()->ReadResult().PublicationSequence, InitialPublication + uint64(CurrentCalls));
	}
	Test.TestTrue(TEXT("Short game frames can produce zero physics steps"), bObservedNoPhysicsStep);
	Test.TestTrue(TEXT("Long game frames can produce multiple physics steps"), bObservedMultiplePhysicsSteps);
	Test.TestEqual(TEXT("All physics callbacks receive fixed dt despite varying GT dt"),
		FPlatformAtomics::InterlockedCompareExchange(&IncorrectDeltaTimes, 0, 0), 0);
	Test.TestEqual(TEXT("Mailbox command is consumed by subsequent fixed physics steps"),
		WheelBridge->ReadResult().AppliedP4Sequence, CommandSequence);
	const auto RetainedDriveStep = FVehicleFixedDriveAsyncLifecycleProbe::Step(*Vehicle.Drive);
	// Leave completion to normal World teardown after a final catch-up frame. No audio device
	// is created here; this checks the vehicle/Chaos exit path, not the reported audio shutdown.
	AdvanceGameFrame(1.f / 30.f);
	Owner.World->DestroyWorld(false);
	Owner.World = nullptr;
	Test.TestTrue(TEXT("Fixed async world teardown closes Drive, witness and host"),
		RetainedDriveStep->IsStopped() && Witness->IsStopped() && Host->IsClosed());
	return true;
}

bool FVehicleFixedDriveFixedAsyncTest::RunTest(const FString& Parameters)
{
	return RunFixedAsyncFrameBoundary(*this, 1.f / 120.f);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleFixedDrive1000HzExitTest,
	"KinetiForge.Drivetrain.FixedDrive.FixedAsync1000HzExit",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleFixedDrive1000HzExitTest::RunTest(const FString& Parameters)
{
	return RunFixedAsyncFrameBoundary(*this, 0.001f);
}

#endif // WITH_DEV_AUTOMATION_TESTS
