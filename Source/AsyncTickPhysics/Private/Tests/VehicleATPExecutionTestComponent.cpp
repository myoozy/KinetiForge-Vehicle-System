#include "VehicleATPExecutionTestComponent.h"

#include "AsyncTickPhysicsExecution.h"

void UVehicleATPExecutionTestComponent::NativeAsyncTick(float DeltaTime)
{
	ObservedSolver.store(AsyncTickPhysics::GetCurrentSolver(), std::memory_order_release);
	bObservedGameThread.store(IsInGameThread(), std::memory_order_release);
	NativeCallCount.fetch_add(1, std::memory_order_release);
	Super::NativeAsyncTick(DeltaTime);
}
