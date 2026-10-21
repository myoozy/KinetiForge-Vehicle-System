#pragma once

#include "CoreMinimal.h"
#include "AsyncTickActorComponent.h"
#include <atomic>
#include "VehicleATPExecutionTestComponent.generated.h"

UCLASS()
class UVehicleATPExecutionTestComponent final : public UAsyncTickActorComponent
{
	GENERATED_BODY()

public:
	virtual void NativeAsyncTick(float DeltaTime) override;

	std::atomic<void*> ObservedSolver = nullptr;
	std::atomic<int32> NativeCallCount = 0;
	std::atomic<bool> bObservedGameThread = false;
};
