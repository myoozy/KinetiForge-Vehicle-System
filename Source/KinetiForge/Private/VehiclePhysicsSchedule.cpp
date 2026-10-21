// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehiclePhysicsSchedule.h"
#include "Misc/ScopeLock.h"

FVehiclePhysicsSchedule::FStepArray FVehiclePhysicsSchedule::PublishSteps(FStepArray NewSteps)
{
	FStepArray PreviousSteps;
	{
		FScopeLock Lock(&ScheduleMutex);
		PreviousSteps = MoveTemp(Steps);
		Steps = MoveTemp(NewSteps);
	}
	return PreviousSteps;
}

FVehiclePhysicsSchedule::FStepArray FVehiclePhysicsSchedule::CaptureSteps() const
{
	FScopeLock Lock(&ScheduleMutex);
	return Steps;
}

void FVehiclePhysicsSchedule::ExecuteCapturedSteps(const FStepArray& CapturedSteps, float DeltaTime, bool bAutomaticCall)
{
	for (const FStepPtr& Step : CapturedSteps)
	{
		if (Step)
		{
			Step->TryExecute(DeltaTime, bAutomaticCall);
		}
	}
}

void FVehiclePhysicsSchedule::ExecuteCapturedSteps(const FStepArray& CapturedSteps, float DeltaTime,
	const FVehiclePhysicsExecutionContext& Context, bool bAutomaticCall)
{
	for (const FStepPtr& Step : CapturedSteps)
	{
		if (Step)
		{
			Step->TryExecute(DeltaTime, Context, bAutomaticCall);
		}
	}
}

void FVehiclePhysicsSchedule::ClearAndStop()
{
	FStepArray RetiredSteps = PublishSteps({});
	for (const FStepPtr& Step : RetiredSteps)
	{
		if (Step)
		{
			Step->StopAndWait();
		}
	}
}
