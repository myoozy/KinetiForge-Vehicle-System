#include "AsyncTickActorComponent.h"

#include "AsyncTickManager.h"

void UAsyncTickActorComponent::NativeAsyncTick(float DeltaTime)
{
	AsyncTick(DeltaTime);
}

void UAsyncTickActorComponent::BeginPlay()
{
	Super::BeginPlay();

	FAsyncTickManager* AsyncTickManager = FAsyncTickManager::GetPhysicsManagerFromScene(this->GetWorld()->GetPhysicsScene());
	AsyncTickManager->AddActorComponent(this);
}

void UAsyncTickActorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	FAsyncTickManager* AsyncTickManager = FAsyncTickManager::GetPhysicsManagerFromScene(this->GetWorld()->GetPhysicsScene());
	AsyncTickManager->RemoveActorComponent(this);
}
