// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "AsyncTickCallback.h"
#include "AsyncTickPhysicsExecution.h"
#include "Chaos/PBDRigidsEvolution.h"
#include "Engine/World.h"
#include "Misc/AutomationTest.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "PBDRigidsSolver.h"
#include "UObject/StrongObjectPtr.h"
#include "VehicleATPExecutionTestComponent.h"

namespace VehicleATPExecutionContextTests
{
	struct FWorldOwner
	{
		UWorld* World = nullptr;
		~FWorldOwner()
		{
			if (World)
			{
				World->DestroyWorld(false);
			}
		}
	};

	UWorld* CreateWorld(FAutomationTestBase& Test, FWorldOwner& Owner, const FName Name)
	{
		Owner.World = UWorld::CreateWorld(EWorldType::Game, false, Name, nullptr, true,
			ERHIFeatureLevel::Num, nullptr, true);
		if (!Test.TestNotNull(TEXT("ATP context test world created"), Owner.World)) return nullptr;

		Owner.World->InitWorld(UWorld::InitializationValues().AllowAudioPlayback(false)
			.CreatePhysicsScene(true).RequiresHitProxies(false).CreateNavigation(false)
			.CreateAISystem(false).ShouldSimulatePhysics(false).SetTransactional(false));
		if (!Test.TestNotNull(TEXT("ATP context test world has a Chaos scene"), Owner.World->GetPhysicsScene())) return nullptr;

		Owner.World->GetPhysicsScene()->GetSolver()->DisableAsyncMode();
		Owner.World->GetPhysicsScene()->GetSolver()->SetThreadingMode_External(Chaos::EThreadingModeTemp::TaskGraph);
		return Owner.World;
	}

	void Flush(UWorld& World)
	{
		const FVector Gravity = FVector::ZeroVector;
		World.GetPhysicsScene()->SetUpForFrame(&Gravity, 1.f / 60.f, 0.f, 1.f / 60.f, 1.f / 60.f, 1, false);
		World.GetPhysicsScene()->StartFrame();
		World.GetPhysicsScene()->WaitPhysScenes();
		World.GetPhysicsScene()->EndFrame();
	}

	void Submit(FAsyncPhysicsCallback& Callback, UWorld& World, UAsyncTickActorComponent& Component)
	{
		FAsyncPhysicsInput* Input = Callback.GetProducerInputData_External();
		Input->Reset();
		Input->World = &World;
		Input->ActorComponents.Add(&Component);
		Input->DebugMessages = false;
	}

	struct FCallbackOwner
	{
		Chaos::FPhysicsSolver* Solver = nullptr;
		FAsyncPhysicsCallback* Callback = nullptr;

		~FCallbackOwner()
		{
			if (Solver && Callback)
			{
				Solver->UnregisterAndFreeSimCallbackObject_External(Callback);
			}
		}
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleATPExecutionContextScopeTest,
	"KinetiForge.AsyncTickPhysics.ExecutionContext.RealCallbackScope",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleATPExecutionContextScopeTest::RunTest(const FString& Parameters)
{
	using namespace VehicleATPExecutionContextTests;

	FWorldOwner WorldAOwner;
	FWorldOwner WorldBOwner;
	UWorld* WorldA = CreateWorld(*this, WorldAOwner, TEXT("ATPExecutionContextA"));
	UWorld* WorldB = CreateWorld(*this, WorldBOwner, TEXT("ATPExecutionContextB"));
	if (!WorldA || !WorldB) return false;

	Chaos::FPhysicsSolver* SolverA = WorldA->GetPhysicsScene()->GetSolver();
	Chaos::FPhysicsSolver* SolverB = WorldB->GetPhysicsScene()->GetSolver();
	if (!TestNotNull(TEXT("World A solver exists"), SolverA)
		|| !TestNotNull(TEXT("World B solver exists"), SolverB)) return false;
	if (!TestTrue(TEXT("Worlds own distinct solvers"), SolverA != SolverB)) return false;

	TStrongObjectPtr<UVehicleATPExecutionTestComponent> ComponentA(NewObject<UVehicleATPExecutionTestComponent>(WorldA));
	TStrongObjectPtr<UVehicleATPExecutionTestComponent> ComponentB(NewObject<UVehicleATPExecutionTestComponent>(WorldB));
	if (!TestNotNull(TEXT("World A ATP probe component created"), ComponentA.Get())
		|| !TestNotNull(TEXT("World B ATP probe component created"), ComponentB.Get())) return false;

	FCallbackOwner CallbackAOwner{ SolverA,
		SolverA->CreateAndRegisterSimCallbackObject_External<FAsyncPhysicsCallback>() };
	FCallbackOwner CallbackBOwner{ SolverB,
		SolverB->CreateAndRegisterSimCallbackObject_External<FAsyncPhysicsCallback>() };
	FAsyncPhysicsCallback* CallbackA = CallbackAOwner.Callback;
	FAsyncPhysicsCallback* CallbackB = CallbackBOwner.Callback;
	if (!TestNotNull(TEXT("World A real ATP sim callback registered"), CallbackA)
		|| !TestNotNull(TEXT("World B real ATP sim callback registered"), CallbackB)) return false;

	Submit(*CallbackA, *WorldA, *ComponentA);
	Submit(*CallbackB, *WorldB, *ComponentB);
	Flush(*WorldA);
	Flush(*WorldB);

	TestEqual(TEXT("Legacy NativeAsyncTick is still dispatched once in World A"),
		ComponentA->NativeCallCount.load(std::memory_order_acquire), 1);
	TestEqual(TEXT("Legacy NativeAsyncTick is still dispatched once in World B"),
		ComponentB->NativeCallCount.load(std::memory_order_acquire), 1);
	TestTrue(TEXT("World A callback exposes its own active solver"),
		ComponentA->ObservedSolver.load(std::memory_order_acquire) == SolverA);
	TestTrue(TEXT("World B callback exposes its own active solver"),
		ComponentB->ObservedSolver.load(std::memory_order_acquire) == SolverB);
	TestFalse(TEXT("ATP callback dispatch runs away from the game thread"),
		ComponentA->bObservedGameThread.load(std::memory_order_acquire)
			|| ComponentB->bObservedGameThread.load(std::memory_order_acquire));
	TestNull(TEXT("Current-solver accessor has no value on the game thread"),
		AsyncTickPhysics::GetCurrentSolver());

	Submit(*CallbackA, *WorldA, *ComponentA);
	Flush(*WorldA);
	TestEqual(TEXT("Repeated callback still dispatches legacy component code"),
		ComponentA->NativeCallCount.load(std::memory_order_acquire), 2);
	TestTrue(TEXT("Solver scope is re-established for each callback invocation"),
		ComponentA->ObservedSolver.load(std::memory_order_acquire) == SolverA);
	TestNull(TEXT("No solver scope is visible on the game thread after repeated callback"),
		AsyncTickPhysics::GetCurrentSolver());

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
