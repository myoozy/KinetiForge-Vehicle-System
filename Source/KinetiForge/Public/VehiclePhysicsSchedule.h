// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "HAL/CriticalSection.h"
#include "VehiclePhysicsStep.h"

/** Ordered, UObject-free snapshot of Drive physics steps. This is a scheduling
 * primitive only; callers still own component and borrowed-body lifetimes.
 */
class KINETIFORGE_API FVehiclePhysicsSchedule
{
public:
	using FStepPtr = TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe>;
	using FStepArray = TArray<FStepPtr>;

	// Replaces the current snapshot and returns the previous one. The returned
	// references must be released after caller-owned registration locks are dropped.
	FStepArray PublishSteps(FStepArray NewSteps);
	FStepArray CaptureSteps() const;
	static void ExecuteCapturedSteps(const FStepArray& Steps, float DeltaTime, bool bAutomaticCall = true);
	static void ExecuteCapturedSteps(const FStepArray& Steps, float DeltaTime,
		const FVehiclePhysicsExecutionContext& Context, bool bAutomaticCall = true);
	void ClearAndStop();

private:
	mutable FCriticalSection ScheduleMutex;
	FStepArray Steps;
};
