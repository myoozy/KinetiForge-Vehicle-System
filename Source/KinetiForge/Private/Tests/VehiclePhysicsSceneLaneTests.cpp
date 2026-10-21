// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "KinetiForge.h"
#include "VehiclePhysicsScene.h"
#include "VehiclePhysicsStep.h"
#include "Engine/World.h"
#include "HAL/PlatformAtomics.h"
#include "HAL/PlatformTLS.h"
#include "Misc/AutomationTest.h"
#include "Misc/ScopeExit.h"
#include "Modules/ModuleManager.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "PBDRigidsSolver.h"

namespace VehiclePhysicsSceneLaneTests
{
	struct FWorldOwner
	{
		UWorld* World = nullptr;
		~FWorldOwner() { if (World) World->DestroyWorld(false); }
	};

	UWorld* CreateTaskGraphWorld(FAutomationTestBase& Test, FWorldOwner& Owner)
	{
		Owner.World = UWorld::CreateWorld(EWorldType::Game, false, TEXT("VehiclePhysicsSceneLanes"), nullptr,
			true, ERHIFeatureLevel::Num, nullptr, true);
		if (!Test.TestNotNull(TEXT("Scene lane test world created"), Owner.World)) return nullptr;
		Owner.World->InitWorld(UWorld::InitializationValues().AllowAudioPlayback(false)
			.CreatePhysicsScene(true).RequiresHitProxies(false).CreateNavigation(false)
			.CreateAISystem(false).ShouldSimulatePhysics(true).SetTransactional(false));
		if (!Test.TestNotNull(TEXT("Scene lane test has Chaos scene"), Owner.World->GetPhysicsScene())) return nullptr;
		Chaos::FPBDRigidsSolver* Solver = static_cast<Chaos::FPBDRigidsSolver*>(Owner.World->GetPhysicsScene()->GetSolver());
		Solver->DisableAsyncMode();
		Solver->SetThreadingMode_External(Chaos::EThreadingModeTemp::TaskGraph);
		return Owner.World;
	}

	void Flush(UWorld& World)
	{
		const FVector Gravity(0.f, 0.f, -980.f);
		World.GetPhysicsScene()->SetUpForFrame(&Gravity, 1.f / 60.f, 0.f, 1.f / 60.f, 1.f / 60.f, 1, false);
		World.GetPhysicsScene()->StartFrame();
		World.GetPhysicsScene()->WaitPhysScenes();
		World.GetPhysicsScene()->EndFrame();
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehiclePhysicsSceneExecutionLanesTest,
	"KinetiForge.Lifecycle.PhysicsScene.ExecutionLanes",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehiclePhysicsSceneExecutionLanesTest::RunTest(const FString& Parameters)
{
	using namespace VehiclePhysicsSceneLaneTests;
	FWorldOwner Owner;
	UWorld* World = CreateTaskGraphWorld(*this, Owner);
	if (!World) return false;
	FKinetiForgeModule& Module = FModuleManager::Get().GetModuleChecked<FKinetiForgeModule>(TEXT("KinetiForge"));
	auto Host = Module.AcquirePhysicsScene(World->GetPhysicsScene());
	if (!TestNotNull(TEXT("Scene host acquired"), Host.Get())) return false;
	ON_SCOPE_EXIT { Host->Close(); };

	// Preserve the existing eager worker callback behavior while creating the lazy frozen lane on demand.
	Flush(*World);
	volatile int32 WorkerCalls = 0;
	volatile int32 FrozenCalls = 0;
	volatile int32 WorkerThreadId = -1;
	volatile int32 FrozenThreadId = -1;
	const int32 GameThreadId = static_cast<int32>(FPlatformTLS::GetCurrentThreadId());
	auto WorkerStep = Host->RegisterStep([&WorkerCalls, &WorkerThreadId](float, const FVehiclePhysicsExecutionContext&)
	{
		FPlatformAtomics::InterlockedIncrement(&WorkerCalls);
		FPlatformAtomics::InterlockedCompareExchange(&WorkerThreadId,
			static_cast<int32>(FPlatformTLS::GetCurrentThreadId()), -1);
	});
	auto FrozenStep = Host->RegisterStep([&FrozenCalls, &FrozenThreadId](float, const FVehiclePhysicsExecutionContext&)
	{
		FPlatformAtomics::InterlockedIncrement(&FrozenCalls);
		FPlatformAtomics::InterlockedCompareExchange(&FrozenThreadId,
			static_cast<int32>(FPlatformTLS::GetCurrentThreadId()), -1);
	}, true, EVehiclePhysicsExecutionLane::FrozenGameThread);
	if (!TestNotNull(TEXT("Worker lane step registered"), WorkerStep.Get())
		|| !TestNotNull(TEXT("Frozen game-thread lane step registered"), FrozenStep.Get())) return false;
	ON_SCOPE_EXIT
	{
		Host->UnregisterStep(WorkerStep);
		Host->UnregisterStep(FrozenStep);
	};

	Flush(*World);
	TestEqual(TEXT("Worker lane runs exactly once for one solver advance"),
		FPlatformAtomics::InterlockedCompareExchange(&WorkerCalls, 0, 0), 1);
	TestEqual(TEXT("Frozen lane runs exactly once for one solver advance"),
		FPlatformAtomics::InterlockedCompareExchange(&FrozenCalls, 0, 0), 1);
	TestTrue(TEXT("Presimulate worker lane executes off the game thread"),
		FPlatformAtomics::InterlockedCompareExchange(&WorkerThreadId, -1, -1) != GameThreadId);
	TestEqual(TEXT("Frozen presimulate lane executes in the engine frozen game-thread window"),
		FPlatformAtomics::InterlockedCompareExchange(&FrozenThreadId, -1, -1), GameThreadId);

	TestTrue(TEXT("Worker step unregisters from its own lane"), Host->UnregisterStep(WorkerStep));
	Flush(*World);
	TestEqual(TEXT("Unregistering worker step leaves its lane inert"),
		FPlatformAtomics::InterlockedCompareExchange(&WorkerCalls, 0, 0), 1);
	TestEqual(TEXT("Frozen lane remains independent after worker removal"),
		FPlatformAtomics::InterlockedCompareExchange(&FrozenCalls, 0, 0), 2);

	Host->Close();
	TestTrue(TEXT("Close drains retained frozen step"), FrozenStep->IsStopped());
	Flush(*World);
	TestEqual(TEXT("Closed host does not run stale lane snapshots"),
		FPlatformAtomics::InterlockedCompareExchange(&FrozenCalls, 0, 0), 2);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
