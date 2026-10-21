// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "AsyncTickManager.h"
#include "AsyncTickPawn.h"
#include "AsyncTickActorComponent.h"
#include "Engine/World.h"
#include "Misc/AutomationTest.h"
#include "PBDRigidsSolver.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "UObject/GarbageCollection.h"
#include "UObject/StrongObjectPtr.h"

struct FAsyncTickManagerLifecycleProbe
{
	static int32 PawnCount(const FAsyncTickManager& Manager) { return Manager.Pawns.Num(); }
	static int32 ComponentCount(const FAsyncTickManager& Manager) { return Manager.ActorComponents.Num(); }
	static FDelegateHandle PreTickHandle(const FAsyncTickManager& Manager) { return Manager.OnPhysScenePreTickHandle; }
	static FAsyncPhysicsCallback* AsyncObject(const FAsyncTickManager& Manager) { return Manager.AsyncObject; }
	static FDelegateHandle PostWorldHandle() { return FAsyncTickManager::OnPostWorldInitializationHandle; }
	static FDelegateHandle WorldCleanupHandle() { return FAsyncTickManager::OnWorldCleanupHandle; }
	static int32 ManagerCount() { return FAsyncTickManager::SceneToPhysicsManagerMap.Num(); }
};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleAsyncManagerLifecycleTest,
	"KinetiForge.Lifecycle.AsyncManagerCallbacks",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleAsyncManagerLifecycleTest::RunTest(const FString& Parameters)
{
	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true,
		ERHIFeatureLevel::Num, nullptr, true);
	if (!TestNotNull(TEXT("Test world created"), World)) return false;
	World->InitWorld(UWorld::InitializationValues().AllowAudioPlayback(false)
		.CreatePhysicsScene(true).RequiresHitProxies(false).CreateNavigation(false)
		.CreateAISystem(false).ShouldSimulatePhysics(false).SetTransactional(false));
	struct FWorldCleanup
	{
		UWorld* World;
		~FWorldCleanup() { if (World) World->DestroyWorld(false); }
	} Cleanup{World};
	World->GetPhysicsScene()->GetSolver()->DisableAsyncMode();
	World->GetPhysicsScene()->GetSolver()->SetThreadingMode_External(Chaos::EThreadingModeTemp::SingleThread);

	FPhysScene* Scene = World->GetPhysicsScene();
	FAsyncTickManager* Manager = FAsyncTickManager::GetPhysicsManagerFromScene(Scene);
	if (!TestNotNull(TEXT("World initialization registered its physics manager"), Manager)) return false;
	Manager->RegisterCallbacks(); // Also covers callers that explicitly register after world init.
	FDelegateHandle Handle = FAsyncTickManagerLifecycleProbe::PreTickHandle(*Manager);
	FAsyncPhysicsCallback* Callback = FAsyncTickManagerLifecycleProbe::AsyncObject(*Manager);
	if (!TestTrue(TEXT("First callback registration is active"), Handle.IsValid() && Callback != nullptr)) return false;
	Manager->RegisterCallbacks();
	TestTrue(TEXT("Repeated registration keeps one pre-tick delegate handle"),
		FAsyncTickManagerLifecycleProbe::PreTickHandle(*Manager) == Handle);
	TestTrue(TEXT("Repeated registration does not allocate another sim callback"),
		FAsyncTickManagerLifecycleProbe::AsyncObject(*Manager) == Callback);

	TStrongObjectPtr<UAsyncTickActorComponent> Component(NewObject<UAsyncTickActorComponent>());
	TWeakObjectPtr<UAsyncTickActorComponent> StaleComponent(Component.Get());
	Manager->AddActorComponent(Component.Get());
	Manager->AddActorComponent(Component.Get());
	TestEqual(TEXT("Repeated component add stores one weak entry"),
		FAsyncTickManagerLifecycleProbe::ComponentCount(*Manager), 1);
	Component.Reset();
	CollectGarbage(RF_NoFlags);
	TestFalse(TEXT("Component weak identity becomes invalid after collection"), StaleComponent.IsValid());
	Manager->RemoveActorComponent(StaleComponent);
	TestEqual(TEXT("Invalid component weak entry is still removable"),
		FAsyncTickManagerLifecycleProbe::ComponentCount(*Manager), 0);

	AAsyncTickPawn* Pawn = World->SpawnActor<AAsyncTickPawn>();
	if (!TestNotNull(TEXT("Pawn spawned in the test world"), Pawn)) return false;
	TWeakObjectPtr<AAsyncTickPawn> StalePawn(Pawn);
	Manager->AddPawn(Pawn);
	Manager->AddPawn(Pawn);
	TestEqual(TEXT("Repeated pawn add stores one weak entry"),
		FAsyncTickManagerLifecycleProbe::PawnCount(*Manager), 1);
	World->DestroyActor(Pawn);
	CollectGarbage(RF_NoFlags);
	TestFalse(TEXT("Destroyed pawn weak identity becomes invalid"), StalePawn.IsValid());
	Manager->RemovePawn(StalePawn);
	TestEqual(TEXT("Invalid pawn weak entry is still removable"),
		FAsyncTickManagerLifecycleProbe::PawnCount(*Manager), 0);

	Manager->UnregisterCallbacks();
	Manager->UnregisterCallbacks();
	TestFalse(TEXT("Repeated unregister clears the pre-tick handle"),
		FAsyncTickManagerLifecycleProbe::PreTickHandle(*Manager).IsValid());
	TestNull(TEXT("Repeated unregister clears the callback pointer"),
		FAsyncTickManagerLifecycleProbe::AsyncObject(*Manager));
	Manager->RegisterCallbacks();
	TestTrue(TEXT("Callbacks can be registered again after unregister"),
		FAsyncTickManagerLifecycleProbe::PreTickHandle(*Manager).IsValid()
		&& FAsyncTickManagerLifecycleProbe::AsyncObject(*Manager) != nullptr);

	FPhysScene* DestroyedScene = Scene;
	World->DestroyWorld(false);
	Cleanup.World = nullptr;
	if (FAsyncTickManager* RemainingManager = FAsyncTickManager::GetPhysicsManagerFromScene(DestroyedScene))
	{
		TestFalse(TEXT("World cleanup unregisters the pre-tick delegate even if the scene outlives DestroyWorld"),
			FAsyncTickManagerLifecycleProbe::PreTickHandle(*RemainingManager).IsValid());
		TestNull(TEXT("World cleanup unregisters its solver callback even if the scene outlives DestroyWorld"),
			FAsyncTickManagerLifecycleProbe::AsyncObject(*RemainingManager));
	}
	CollectGarbage(RF_NoFlags);
	TestNull(TEXT("Scene teardown eventually removes its manager"),
		FAsyncTickManager::GetPhysicsManagerFromScene(DestroyedScene));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleAsyncManagerWorldOwnershipTest,
	"KinetiForge.Lifecycle.AsyncManagerWorldOwnership",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleAsyncManagerWorldOwnershipTest::RunTest(const FString& Parameters)
{
	const int32 InitialManagerCount = FAsyncTickManagerLifecycleProbe::ManagerCount();
	const FDelegateHandle InitialPostWorldHandle = FAsyncTickManagerLifecycleProbe::PostWorldHandle();
	const FDelegateHandle InitialCleanupHandle = FAsyncTickManagerLifecycleProbe::WorldCleanupHandle();
	auto CreateWorld = [](const TCHAR* Name, bool bCreatePhysicsScene) -> UWorld*
	{
		UWorld* World = UWorld::CreateWorld(EWorldType::Game, false, FName(Name), nullptr, true,
			ERHIFeatureLevel::Num, nullptr, true);
		if (World != nullptr)
		{
			World->InitWorld(UWorld::InitializationValues().AllowAudioPlayback(false)
				.CreatePhysicsScene(bCreatePhysicsScene).RequiresHitProxies(false).CreateNavigation(false)
				.CreateAISystem(false).ShouldSimulatePhysics(false).SetTransactional(false));
		}
		return World;
	};
	auto ConfigureSolver = [](UWorld* World)
	{
		if (FPhysScene* Scene = World ? World->GetPhysicsScene() : nullptr)
		{
			if (Chaos::FPhysicsSolver* Solver = Scene->GetSolver())
			{
				Solver->DisableAsyncMode();
				Solver->SetThreadingMode_External(Chaos::EThreadingModeTemp::SingleThread);
			}
		}
	};

	TStrongObjectPtr<UWorld> First(CreateWorld(TEXT("AsyncManagerFirstWorld"), true));
	if (!TestNotNull(TEXT("First world created"), First.Get())) return false;
	ConfigureSolver(First.Get());
	FPhysScene* FirstScene = First->GetPhysicsScene();
	FAsyncTickManager* FirstManager = FAsyncTickManager::GetPhysicsManagerFromScene(FirstScene);
	if (!TestNotNull(TEXT("First world manager registered"), FirstManager)) return false;
	FDelegateHandle PostWorldHandle = FAsyncTickManagerLifecycleProbe::PostWorldHandle();
	FDelegateHandle CleanupHandle = FAsyncTickManagerLifecycleProbe::WorldCleanupHandle();
	TestTrue(TEXT("World delegate registrations remain active while managers exist"),
		FAsyncTickManager::GInitialized && PostWorldHandle.IsValid() && CleanupHandle.IsValid());

	TStrongObjectPtr<UWorld> Second(CreateWorld(TEXT("AsyncManagerSecondWorld"), true));
	if (!TestNotNull(TEXT("Second world created"), Second.Get())) return false;
	ConfigureSolver(Second.Get());
	FPhysScene* SecondScene = Second->GetPhysicsScene();
	FAsyncTickManager* SecondManager = FAsyncTickManager::GetPhysicsManagerFromScene(SecondScene);
	if (!TestNotNull(TEXT("Second world manager registered"), SecondManager)) return false;
	TestEqual(TEXT("Two fixture worlds add two manager entries"), FAsyncTickManagerLifecycleProbe::ManagerCount(), InitialManagerCount + 2);
	TestTrue(TEXT("Second world does not duplicate global delegate registration"),
		FAsyncTickManagerLifecycleProbe::PostWorldHandle() == PostWorldHandle
		&& FAsyncTickManagerLifecycleProbe::WorldCleanupHandle() == CleanupHandle);

	First->DestroyWorld(false);
	if (FAsyncTickManager* RemainingManager = FAsyncTickManager::GetPhysicsManagerFromScene(FirstScene))
	{
		TestFalse(TEXT("First world cleanup unregisters callbacks before scene destruction"),
			FAsyncTickManagerLifecycleProbe::PreTickHandle(*RemainingManager).IsValid());
		TestNull(TEXT("First world cleanup clears its callback object"), FAsyncTickManagerLifecycleProbe::AsyncObject(*RemainingManager));
	}
	First.Reset();
	CollectGarbage(RF_NoFlags);
	TestNull(TEXT("First scene teardown removes its manager"), FAsyncTickManager::GetPhysicsManagerFromScene(FirstScene));
	TestEqual(TEXT("First termination removes only its entry"), FAsyncTickManagerLifecycleProbe::ManagerCount(), InitialManagerCount + 1);
	TestTrue(TEXT("Other managers keep global delegates registered"), FAsyncTickManager::GInitialized
		&& FAsyncTickManagerLifecycleProbe::PostWorldHandle() == PostWorldHandle
		&& FAsyncTickManagerLifecycleProbe::WorldCleanupHandle() == CleanupHandle);

	Second->DestroyWorld(false);
	if (FAsyncTickManager* RemainingManager = FAsyncTickManager::GetPhysicsManagerFromScene(SecondScene))
	{
		TestFalse(TEXT("Second world cleanup unregisters callbacks before scene destruction"),
			FAsyncTickManagerLifecycleProbe::PreTickHandle(*RemainingManager).IsValid());
		TestNull(TEXT("Second world cleanup clears its callback object"), FAsyncTickManagerLifecycleProbe::AsyncObject(*RemainingManager));
	}
	Second.Reset();
	CollectGarbage(RF_NoFlags);
	TestNull(TEXT("Second scene teardown removes its manager"), FAsyncTickManager::GetPhysicsManagerFromScene(SecondScene));
	TestEqual(TEXT("Fixture termination restores the initial manager count"), FAsyncTickManagerLifecycleProbe::ManagerCount(), InitialManagerCount);
	if (InitialManagerCount == 0)
	{
		TestFalse(TEXT("Last fixture world resets world delegate ownership"), FAsyncTickManager::GInitialized);
		TestFalse(TEXT("Last fixture world clears post-init delegate handle"),
			FAsyncTickManagerLifecycleProbe::PostWorldHandle().IsValid());
		TestFalse(TEXT("Last fixture world clears cleanup delegate handle"),
			FAsyncTickManagerLifecycleProbe::WorldCleanupHandle().IsValid());

		// Exercise explicit normal-shutdown cleanup only when no host/editor manager exists.
		TStrongObjectPtr<UWorld> ShutdownWorld(CreateWorld(TEXT("AsyncManagerShutdownWorld"), true));
		if (!TestNotNull(TEXT("Shutdown fixture world created"), ShutdownWorld.Get())) return false;
		ConfigureSolver(ShutdownWorld.Get());
		FPhysScene* ShutdownScene = ShutdownWorld->GetPhysicsScene();
		TestNotNull(TEXT("Shutdown fixture manager registered"), FAsyncTickManager::GetPhysicsManagerFromScene(ShutdownScene));
		FAsyncTickManager::ShutdownAllManagers();
		TestEqual(TEXT("Explicit shutdown removes all manager entries"), FAsyncTickManagerLifecycleProbe::ManagerCount(), 0);
		TestFalse(TEXT("Explicit shutdown releases world delegates"), FAsyncTickManager::GInitialized);
		FAsyncTickManager::ShutdownAllManagers();
		TestNull(TEXT("Repeated shutdown remains idempotent"), FAsyncTickManager::GetPhysicsManagerFromScene(ShutdownScene));
		ShutdownWorld->DestroyWorld(false);
	}
	else
	{
		AddWarning(TEXT("Skipped global ShutdownAllManagers verification because the automation host has live managers; fixture cleanup preserved them."));
	}

	TStrongObjectPtr<UWorld> Third(CreateWorld(TEXT("AsyncManagerThirdWorld"), true));
	if (!TestNotNull(TEXT("Third world created after cleanup"), Third.Get())) return false;
	ConfigureSolver(Third.Get());
	FPhysScene* ThirdScene = Third->GetPhysicsScene();
	FAsyncTickManager* ThirdManager = FAsyncTickManager::GetPhysicsManagerFromScene(ThirdScene);
	if (!TestNotNull(TEXT("A later world can register a fresh manager"), ThirdManager)) return false;
	delete ThirdManager;
	TestNull(TEXT("Manager can be absent while the physics scene remains alive"),
		FAsyncTickManager::GetPhysicsManagerFromScene(ThirdScene));
	AAsyncTickPawn* PawnWithoutManager = Third->SpawnActor<AAsyncTickPawn>();
	if (!TestNotNull(TEXT("Pawn spawned in a world without a manager"), PawnWithoutManager)) return false;
	PawnWithoutManager->DispatchBeginPlay();
	Third->DestroyActor(PawnWithoutManager);
	TestTrue(TEXT("Pawn BeginPlay and EndPlay tolerate an absent manager"), true);
	Third->DestroyWorld(false);
	Third.Reset();
	CollectGarbage(RF_NoFlags);
	TestNull(TEXT("Third scene teardown removes its manager"), FAsyncTickManager::GetPhysicsManagerFromScene(ThirdScene));

	// A world without Chaos must not make Pawn lifecycle code dereference a null manager.
	TStrongObjectPtr<UWorld> NoPhysics(CreateWorld(TEXT("AsyncPawnNoPhysicsWorld"), false));
	if (!TestNotNull(TEXT("No-physics world created"), NoPhysics.Get())) return false;
	TestNull(TEXT("No-physics world has no physics scene"), NoPhysics->GetPhysicsScene());
	AAsyncTickPawn* Pawn = NoPhysics->SpawnActor<AAsyncTickPawn>();
	if (!TestNotNull(TEXT("Pawn spawned without a physics scene"), Pawn)) return false;
	Pawn->DispatchBeginPlay();
	NoPhysics->DestroyActor(Pawn);
	TestTrue(TEXT("Pawn BeginPlay and EndPlay tolerate a missing physics scene"), true);
	NoPhysics->DestroyWorld(false);
	NoPhysics.Reset();
	CollectGarbage(RF_NoFlags);

	TestEqual(TEXT("Test restores preexisting manager count"), FAsyncTickManagerLifecycleProbe::ManagerCount(), InitialManagerCount);
	if (InitialManagerCount > 0)
	{
		TestTrue(TEXT("Test preserves host world delegate registrations"), FAsyncTickManager::GInitialized
			&& FAsyncTickManagerLifecycleProbe::PostWorldHandle() == InitialPostWorldHandle
			&& FAsyncTickManagerLifecycleProbe::WorldCleanupHandle() == InitialCleanupHandle);
	}
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
