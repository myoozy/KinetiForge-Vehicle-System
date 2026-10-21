#include "AsyncTickManager.h"

#include "PBDRigidsSolver.h"
#include "Physics/Experimental/PhysScene_Chaos.h"

bool FAsyncTickManager::GInitialized  = false;

FDelegateHandle FAsyncTickManager::OnPostWorldInitializationHandle;
FDelegateHandle FAsyncTickManager::OnWorldCleanupHandle;

TMap<FPhysScene*, FAsyncTickManager*> FAsyncTickManager::SceneToPhysicsManagerMap;

FAsyncTickManager::FAsyncTickManager(FPhysScene* PhysScene)
	: Scene(*PhysScene)
	, AsyncObject(nullptr)
{
	if(!GInitialized)
	{
		OnPostWorldInitializationHandle = FWorldDelegates::OnPostWorldInitialization.AddStatic(&FAsyncTickManager::OnPostWorldInitialization);
		OnWorldCleanupHandle = FWorldDelegates::OnWorldCleanup.AddStatic(&FAsyncTickManager::OnWorldCleanup);

		GInitialized = true;
	}

	// BAD IF 2!
	ensure(FAsyncTickManager::SceneToPhysicsManagerMap.Find(PhysScene) == nullptr);

	FAsyncTickManager::SceneToPhysicsManagerMap.Add(PhysScene, this);
}

FAsyncTickManager::~FAsyncTickManager()
{
	// Remove the lookup before destroying this manager, including shutdown paths
	// where the physics-scene termination delegate was not the caller.
	DetachFromPhysScene(&Scene);
	// FPhysScene_Chaos broadcasts OnPhysSceneTerm while its solver still exists;
	// FAsyncTickPhysics deletes this manager from that notification.
	UnregisterCallbacks();
	Pawns.Reset();
	ActorComponents.Reset();
}

void FAsyncTickManager::OnPostWorldInitialization(UWorld* InWorld, const UWorld::InitializationValues)
{
	FAsyncTickManager* AsyncTickManager = FAsyncTickManager::GetPhysicsManagerFromScene(InWorld->GetPhysicsScene());

	if(AsyncTickManager != nullptr)
	{
		AsyncTickManager->RegisterCallbacks();
	}
}

void FAsyncTickManager::OnWorldCleanup(UWorld* InWorld, bool bSessionEnded, bool bCleanupResource)
{
	FAsyncTickManager* AsyncTickManager = FAsyncTickManager::GetPhysicsManagerFromScene(InWorld->GetPhysicsScene());

	if(AsyncTickManager != nullptr)
	{
		AsyncTickManager->UnregisterCallbacks();
	}
}

FAsyncTickManager* FAsyncTickManager::GetPhysicsManagerFromScene(FPhysScene* PhysScene)
{
	FAsyncTickManager* Manager = nullptr;
	FAsyncTickManager** ManagerPtr = FAsyncTickManager::SceneToPhysicsManagerMap.Find(PhysScene);

	if(ManagerPtr != nullptr)
	{
		Manager = *ManagerPtr;
	}

	return Manager;
}

void FAsyncTickManager::DetachFromPhysScene(FPhysScene* PhysScene)
{
	if (FAsyncTickManager** Existing = SceneToPhysicsManagerMap.Find(PhysScene);
		Existing != nullptr && *Existing == this)
	{
		SceneToPhysicsManagerMap.Remove(PhysScene);
	}
	UnregisterWorldDelegatesIfUnused();
}

void FAsyncTickManager::UnregisterWorldDelegatesIfUnused()
{
	if (!SceneToPhysicsManagerMap.IsEmpty())
	{
		return;
	}

	if (OnPostWorldInitializationHandle.IsValid())
	{
		FWorldDelegates::OnPostWorldInitialization.Remove(OnPostWorldInitializationHandle);
		OnPostWorldInitializationHandle.Reset();
	}
	if (OnWorldCleanupHandle.IsValid())
	{
		FWorldDelegates::OnWorldCleanup.Remove(OnWorldCleanupHandle);
		OnWorldCleanupHandle.Reset();
	}
	GInitialized = false;
}

void FAsyncTickManager::ShutdownAllManagers()
{
	// Remove all lookup entries before deleting managers so callbacks cannot find
	// a partially dismantled manager during normal game-thread shutdown.
	TArray<FAsyncTickManager*> Managers;
	SceneToPhysicsManagerMap.GenerateValueArray(Managers);
	SceneToPhysicsManagerMap.Reset();
	UnregisterWorldDelegatesIfUnused();

	for (FAsyncTickManager* Manager : Managers)
	{
		if (Manager != nullptr)
		{
			Manager->UnregisterCallbacks();
			delete Manager;
		}
	}
}

void FAsyncTickManager::AddPawn(TWeakObjectPtr<AAsyncTickPawn> Pawn)
{
	if(Pawn.IsValid())
	{
		Pawns.AddUnique(Pawn);
	}
}

void FAsyncTickManager::RemovePawn(TWeakObjectPtr<AAsyncTickPawn> Pawn)
{
	// Remove by weak identity even after the UObject has become invalid.
	Pawns.Remove(Pawn);
}
void FAsyncTickManager::AddActorComponent(TWeakObjectPtr<UAsyncTickActorComponent> ActorComponent)
{
	if(ActorComponent.IsValid())
	{
		ActorComponents.AddUnique(ActorComponent);
	}
}

void FAsyncTickManager::RemoveActorComponent(TWeakObjectPtr<UAsyncTickActorComponent> ActorComponent)
{
	// Remove by weak identity even after the UObject has become invalid.
	ActorComponents.Remove(ActorComponent);
}

void FAsyncTickManager::RegisterCallbacks()
{
	Chaos::FPhysicsSolver* Solver = Scene.GetSolver();
	if (Solver == nullptr) return;
	if (!OnPhysScenePreTickHandle.IsValid())
	{
		OnPhysScenePreTickHandle = Scene.OnPhysScenePreTick.AddRaw(this, &FAsyncTickManager::ScenePreTick);
	}
	if (AsyncObject == nullptr)
	{
		AsyncObject = Solver->CreateAndRegisterSimCallbackObject_External<FAsyncPhysicsCallback>();
	}
}

void FAsyncTickManager::UnregisterCallbacks()
{
	if (OnPhysScenePreTickHandle.IsValid())
	{
		Scene.OnPhysScenePreTick.Remove(OnPhysScenePreTickHandle);
		OnPhysScenePreTickHandle.Reset();
	}

	if(AsyncObject != nullptr)
	{
		FAsyncPhysicsCallback* CallbackToRelease = AsyncObject;
		AsyncObject = nullptr;
		// OnPhysSceneTerm is broadcast from FPhysScene_Chaos's destructor before
		// FChaosScene's base destructor destroys SceneSolver.
		if (Chaos::FPhysicsSolver* Solver = Scene.GetSolver())
		{
			Solver->UnregisterAndFreeSimCallbackObject_External(CallbackToRelease);
		}
	}
}

void FAsyncTickManager::ScenePreTick(FPhysScene* PhysScene, float DeltaTime)
{
	if(AsyncObject == nullptr)
		return;

	FAsyncPhysicsInput* AsyncInput = AsyncObject->GetProducerInputData_External();

	if(AsyncInput == nullptr)
		return;

	UWorld* World = PhysScene->GetOwningWorld();

	if(World == nullptr)
		return;

	AsyncInput->Reset();

	AsyncInput->World = World;
	// no realloc )))
	AsyncInput->Pawns.Reserve(Pawns.Num());
	AsyncInput->ActorComponents.Reserve(ActorComponents.Num());

	for(const TWeakObjectPtr<AAsyncTickPawn> Pawn : Pawns)
	{
		AsyncInput->Pawns.Add(Pawn);
	}
	for(const TWeakObjectPtr<UAsyncTickActorComponent> ActorComponent : ActorComponents)
	{
		AsyncInput->ActorComponents.Add(ActorComponent);
	}
}
