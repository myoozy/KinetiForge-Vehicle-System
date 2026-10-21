#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "AsyncTickCallback.h"
#include "AsyncTickPawn.h"
#include "AsyncTickActorComponent.h"

class ASYNCTICKPHYSICS_API FAsyncTickManager
{
	friend struct FAsyncTickManagerLifecycleProbe;
public:
	FAsyncTickManager(FPhysScene* PhysScene);
	~FAsyncTickManager();

	static bool GInitialized;
	
	static void OnPostWorldInitialization(UWorld* InWorld, const UWorld::InitializationValues);
	static void OnWorldCleanup(UWorld* InWorld, bool bSessionEnded, bool bCleanupResource);

	FPhysScene_Chaos& GetScene() const { return Scene; }

	static FAsyncTickManager* GetPhysicsManagerFromScene(FPhysScene* PhysScene);
	// Called on the game thread during normal module shutdown. This does not drain
	// callbacks already queued on a solver; the module remains resident until process exit.
	static void ShutdownAllManagers();

	void DetachFromPhysScene(FPhysScene* PhysScene);
	
	void AddPawn(TWeakObjectPtr<AAsyncTickPawn> Pawn);
	void RemovePawn(TWeakObjectPtr<AAsyncTickPawn> Pawn);

	void AddActorComponent(TWeakObjectPtr<UAsyncTickActorComponent> ActorComponent);
	void RemoveActorComponent(TWeakObjectPtr<UAsyncTickActorComponent> ActorComponent);

	void RegisterCallbacks();
	void UnregisterCallbacks();

	void ScenePreTick(FPhysScene* PhysScene, float DeltaTime);
private:
	static void UnregisterWorldDelegatesIfUnused();
	static TMap<FPhysScene*, FAsyncTickManager*> SceneToPhysicsManagerMap;

	TArray<TWeakObjectPtr<AAsyncTickPawn>> Pawns;
	TArray<TWeakObjectPtr<UAsyncTickActorComponent>> ActorComponents;
	
	FDelegateHandle OnPhysScenePreTickHandle;

	static FDelegateHandle OnPostWorldInitializationHandle;
	static FDelegateHandle OnWorldCleanupHandle;
	
	FPhysScene_Chaos& Scene;
	
	FAsyncPhysicsCallback* AsyncObject;
};
