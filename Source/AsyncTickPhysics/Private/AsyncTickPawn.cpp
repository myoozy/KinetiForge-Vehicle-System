#include "AsyncTickPawn.h"

#include "AsyncTickManager.h"

void AAsyncTickPawn::NativeAsyncTick(float DeltaTime)
{
	AsyncTick(DeltaTime);
}

void AAsyncTickPawn::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}
	FPhysScene* PhysicsScene = World->GetPhysicsScene();
	if (PhysicsScene == nullptr)
	{
		return;
	}
	if (FAsyncTickManager* AsyncTickManager = FAsyncTickManager::GetPhysicsManagerFromScene(PhysicsScene))
	{
		AsyncTickManager->AddPawn(this);
	}
}

void AAsyncTickPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}
	FPhysScene* PhysicsScene = World->GetPhysicsScene();
	if (PhysicsScene == nullptr)
	{
		return;
	}
	if (FAsyncTickManager* AsyncTickManager = FAsyncTickManager::GetPhysicsManagerFromScene(PhysicsScene))
	{
		AsyncTickManager->RemovePawn(this);
	}
}
