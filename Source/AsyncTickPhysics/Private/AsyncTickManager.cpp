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
	while(Pawns.Num() > 0)
	{
		this->RemovePawn(Pawns.Last());
	}
	while(ActorComponents.Num() > 0)
	{
		this->RemoveActorComponent(ActorComponents.Last());
	}
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
	FAsyncTickManager::SceneToPhysicsManagerMap.Remove(PhysScene);
}

void FAsyncTickManager::AddPawn(TWeakObjectPtr<AAsyncTickPawn> Pawn)
{
	if(Pawn.IsValid())
	{
		Pawns.Add(Pawn);
	}
}

void FAsyncTickManager::RemovePawn(TWeakObjectPtr<AAsyncTickPawn> Pawn)
{
	if(Pawn.IsValid())
	{
		Pawns.Remove(Pawn);
	}
}
void FAsyncTickManager::AddActorComponent(TWeakObjectPtr<UAsyncTickActorComponent> ActorComponent)
{
	if(ActorComponent.IsValid())
	{
		ActorComponents.Add(ActorComponent);
	}
}

void FAsyncTickManager::RemoveActorComponent(TWeakObjectPtr<UAsyncTickActorComponent> ActorComponent)
{
	if(ActorComponent.IsValid())
	{
		ActorComponents.Remove(ActorComponent);
	}
}

void FAsyncTickManager::RegisterCallbacks()
{
	OnPhysScenePreTickHandle = Scene.OnPhysScenePreTick.AddRaw(this, &FAsyncTickManager::ScenePreTick);

	if(AsyncObject == nullptr)
	{
		AsyncObject = Scene.GetSolver()->CreateAndRegisterSimCallbackObject_External<FAsyncPhysicsCallback>();
	}
}

void FAsyncTickManager::UnregisterCallbacks()
{
	Scene.OnPhysScenePreTick.Remove(OnPhysScenePreTickHandle);

	if(AsyncObject != nullptr)
	{
		Scene.GetSolver()->UnregisterAndFreeSimCallbackObject_External(AsyncObject);
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
