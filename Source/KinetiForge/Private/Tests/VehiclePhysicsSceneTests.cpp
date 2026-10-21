// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "KinetiForge.h"
#include "Chaos/PBDRigidsEvolution.h"
#include "Engine/World.h"
#include "Misc/AutomationTest.h"
#include "Modules/ModuleManager.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "PBDRigidsSolver.h"
#include "UObject/GarbageCollection.h"

namespace VehiclePhysicsSceneTests
{
	struct FWorldOwner
	{
		UWorld* World = nullptr;
		~FWorldOwner() { if (World) World->DestroyWorld(false); }
	};

	UWorld* CreateWorld(FAutomationTestBase& Test, FWorldOwner& Owner, const FName Name)
	{
		Owner.World = UWorld::CreateWorld(EWorldType::Game, false, Name, nullptr, true, ERHIFeatureLevel::Num, nullptr, true);
		if (!Test.TestNotNull(TEXT("Scene host test world created"), Owner.World)) return nullptr;
		Owner.World->InitWorld(UWorld::InitializationValues().AllowAudioPlayback(false)
			.CreatePhysicsScene(true).RequiresHitProxies(false).CreateNavigation(false)
			.CreateAISystem(false).ShouldSimulatePhysics(false).SetTransactional(false));
		if (!Test.TestNotNull(TEXT("Scene host test world has Chaos scene"), Owner.World->GetPhysicsScene())) return nullptr;
		Owner.World->GetPhysicsScene()->GetSolver()->DisableAsyncMode();
		Owner.World->GetPhysicsScene()->GetSolver()->SetThreadingMode_External(Chaos::EThreadingModeTemp::SingleThread);
		return Owner.World;
	}

	void Flush(UWorld& World)
	{
		const FVector Gravity = FVector::ZeroVector;
		World.GetPhysicsScene()->SetUpForFrame(&Gravity, 1.f / 60.f, 0.f, 1.f / 60.f, 1.f / 60.f, 1, false);
		World.GetPhysicsScene()->StartFrame();
		World.GetPhysicsScene()->EndFrame();
		World.GetPhysicsScene()->WaitPhysScenes();
	}

	struct FObservedStep
	{
		TArray<int32> Order;
		TArray<Chaos::FPBDRigidsSolver*> Solvers;
		int32 LegacyCalls = 0;
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehiclePhysicsSceneHostTest,
	"KinetiForge.Physics.SceneHost.LazyIsolationAndClose",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehiclePhysicsSceneHostTest::RunTest(const FString& Parameters)
{
	using namespace VehiclePhysicsSceneTests;
	FKinetiForgeModule& Module = FModuleManager::Get().GetModuleChecked<FKinetiForgeModule>(TEXT("KinetiForge"));
	FWorldOwner WorldAOwner;
	FWorldOwner WorldBOwner;
	FWorldOwner WorldCOwner;
	UWorld* WorldA = CreateWorld(*this, WorldAOwner, TEXT("VehicleSceneHostA"));
	UWorld* WorldB = CreateWorld(*this, WorldBOwner, TEXT("VehicleSceneHostB"));
	UWorld* WorldC = CreateWorld(*this, WorldCOwner, TEXT("VehicleSceneHostC"));
	if (!WorldA || !WorldB || !WorldC) return false;
	FPhysScene_Chaos* SceneA = WorldA->GetPhysicsScene();
	FPhysScene_Chaos* SceneB = WorldB->GetPhysicsScene();
	FPhysScene_Chaos* SceneC = WorldC->GetPhysicsScene();
	if (!TestNotNull(TEXT("Scene A exists"), SceneA) || !TestNotNull(TEXT("Scene B exists"), SceneB)
		|| !TestNotNull(TEXT("Scene C exists"), SceneC)) return false;
	TestNull(TEXT("World creation alone does not acquire a scene host"), Module.FindPhysicsScene(SceneA).Get());
	TestNull(TEXT("World creation alone does not acquire another scene host"), Module.FindPhysicsScene(SceneB).Get());

	TSharedPtr<FVehiclePhysicsScene, ESPMode::ThreadSafe> HostA = Module.AcquirePhysicsScene(SceneA);
	TSharedPtr<FVehiclePhysicsScene, ESPMode::ThreadSafe> HostB = Module.AcquirePhysicsScene(SceneB);
	if (!TestTrue(TEXT("Each scene lazily acquires one host"), HostA.IsValid() && HostB.IsValid())) return false;
	TestTrue(TEXT("Repeated acquire reuses same-scene host"), Module.AcquirePhysicsScene(SceneA) == HostA);
	TestTrue(TEXT("Different scenes have independent hosts"), HostA != HostB);
	int32 LegacyCalls = 0;
	FVehiclePhysicsStep LegacyStep([&LegacyCalls](float) { ++LegacyCalls; });
	TestTrue(TEXT("Legacy entry retains its original no-context manual API"), LegacyStep.TryExecute(1.f / 60.f)
		&& LegacyStep.TryExecute(1.f / 60.f) && LegacyCalls == 2);
	FVehiclePhysicsStep ContextStep([](float, const FVehiclePhysicsExecutionContext&) {});
	TestFalse(TEXT("Context-required entry rejects legacy invocation"), ContextStep.TryExecute(1.f / 60.f));

	FObservedStep ObservedA;
	FObservedStep ObservedB;
	auto StepA1 = HostA->RegisterStep([&ObservedA](float, const FVehiclePhysicsExecutionContext&)
	{
		ObservedA.Order.Add(1);
	});
	auto StepA2 = HostA->RegisterStep([&ObservedA](float, const FVehiclePhysicsExecutionContext& Context)
	{
		ObservedA.Order.Add(2);
		ObservedA.Solvers.Add(&Context.GetSolver());
		auto NestedLegacy = MakeShared<FVehiclePhysicsStep, ESPMode::ThreadSafe>([&ObservedA](float) { ++ObservedA.LegacyCalls; });
		FVehiclePhysicsSchedule NestedSchedule;
		FVehiclePhysicsSchedule::FStepArray NestedSteps;
		NestedSteps.Add(NestedLegacy);
		FVehiclePhysicsSchedule::FStepArray Retired = NestedSchedule.PublishSteps(MoveTemp(NestedSteps));
		FVehiclePhysicsSchedule::ExecuteCapturedSteps(NestedSchedule.CaptureSteps(), 1.f / 60.f, Context, true);
	});
	auto StepB = HostB->RegisterStep([&ObservedB](float, const FVehiclePhysicsExecutionContext& Context)
	{
		ObservedB.Order.Add(3);
		ObservedB.Solvers.Add(&Context.GetSolver());
	});
	if (!TestTrue(TEXT("Hosts create their own context-aware steps"), StepA1.IsValid() && StepA2.IsValid() && StepB.IsValid())) return false;
	Flush(*WorldA);
	Flush(*WorldB);
	TestTrue(TEXT("Callback executes ordered steps from its captured schedule"), ObservedA.Order == TArray<int32>({ 1, 2 }));
	TestTrue(TEXT("Context-required scene step receives the current scene solver"), ObservedA.Solvers.Num() == 1
		&& ObservedA.Solvers[0] == SceneA->GetSolver());
	TestEqual(TEXT("Context schedule also dispatches legacy float-only entries"), ObservedA.LegacyCalls, 1);
	TestTrue(TEXT("Scene B callback receives only its own current solver"), ObservedB.Solvers.Num() == 1
		&& ObservedB.Solvers[0] == SceneB->GetSolver());

	const int32 CallsBeforeUnregister = ObservedA.Order.Num();
	TestFalse(TEXT("Host rejects a step owned by a different scene"), HostA->UnregisterStep(StepB));
	TestFalse(TEXT("Foreign unregister does not stop the owning step"), StepB->IsStopped());
	TestTrue(TEXT("Registered step can be removed"), HostA->UnregisterStep(StepA1));
	TestTrue(TEXT("Removal permanently stops the old entry"), StepA1->IsStopped());
	Flush(*WorldA);
	TestTrue(TEXT("Removed entry is absent from later callbacks"), ObservedA.Order.Num() == CallsBeforeUnregister + 1
		&& ObservedA.Order.Last() == 2);

	// Cover callback create followed by close before the first solver frame consumes its input.
	FObservedStep PendingObserved;
	auto PendingHost = Module.AcquirePhysicsScene(SceneC);
	auto PendingStep = PendingHost->RegisterStep([&PendingObserved](float, const FVehiclePhysicsExecutionContext&)
	{
		PendingObserved.Order.Add(1);
	});
	TestNotNull(TEXT("Pending create/close step registered"), PendingStep.Get());
	PendingHost->Close();
	TestTrue(TEXT("Close invalidates the pending step before solver flush"), PendingHost->IsClosed() && PendingStep->IsStopped());
	TestNull(TEXT("Closed host marker prevents teardown-time reacquisition"), Module.AcquirePhysicsScene(SceneC).Get());
	Flush(*WorldC);
	TestTrue(TEXT("Pending callback input cannot revive the old entry"), PendingObserved.Order.IsEmpty());
	TestTrue(TEXT("Retained closed host is safe and idempotent"), PendingHost->UnregisterStep(PendingStep) == false);
	PendingHost->Close();

	// WorldCleanup closes a retained host even if the user has not explicitly closed it.
	FObservedStep CleanupObserved;
	auto CleanupHost = Module.AcquirePhysicsScene(SceneB);
	auto CleanupStep = CleanupHost->RegisterStep([&CleanupObserved](float, const FVehiclePhysicsExecutionContext&)
	{
		CleanupObserved.Order.Add(1);
	});
	WorldBOwner.World->DestroyWorld(false);
	WorldBOwner.World = nullptr;
	TestTrue(TEXT("WorldCleanup closes its scene host and stops entries"), CleanupHost->IsClosed() && CleanupStep->IsStopped());
	CleanupHost->Close();
	CollectGarbage(RF_NoFlags);
	TestNull(TEXT("SceneTerm removes its host registry entry after world GC"), Module.FindPhysicsScene(SceneB).Get());
	TestTrue(TEXT("Retained host remains inert after world cleanup and GC"), CleanupHost->IsClosed()
		&& !CleanupHost->UnregisterStep(CleanupStep));

	HostA->Close();
	TestTrue(TEXT("Explicit close permanently stops scene A remaining steps"), HostA->IsClosed() && StepA2->IsStopped());
	Flush(*WorldA);
	TestTrue(TEXT("Closed scene A no longer executes prior steps"), ObservedA.Order.Num() == CallsBeforeUnregister + 1);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
