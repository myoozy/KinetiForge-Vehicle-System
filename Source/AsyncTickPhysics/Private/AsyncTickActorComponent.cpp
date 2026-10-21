#include "AsyncTickActorComponent.h"

#include "AsyncTickManager.h"

void UAsyncTickActorComponent::NativeAsyncTick(float DeltaTime)
{
	AsyncTick(DeltaTime);
}

void UAsyncTickActorComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		if (FAsyncTickManager* Manager = FAsyncTickManager::GetPhysicsManagerFromScene(World->GetPhysicsScene()))
		{
			Manager->AddActorComponent(this);
		}
	}
}

void UAsyncTickActorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	// World/scene teardown can precede a deferred component BeginDestroy/EndPlay.
	if (UWorld* World = GetWorld())
	{
		if (FAsyncTickManager* Manager = FAsyncTickManager::GetPhysicsManagerFromScene(World->GetPhysicsScene()))
		{
			Manager->RemoveActorComponent(this);
		}
	}
}
