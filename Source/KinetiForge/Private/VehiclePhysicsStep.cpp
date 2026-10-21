// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehiclePhysicsStep.h"
#include "HAL/Event.h"
#include "HAL/PlatformProcess.h"
#include "Misc/ScopeExit.h"
#include "Misc/ScopeLock.h"

namespace
{
	thread_local const FVehiclePhysicsExecutionContext* CurrentVehiclePhysicsContext = nullptr;
}

const FVehiclePhysicsExecutionContext* FVehiclePhysicsExecutionContext::GetCurrent()
{
	return CurrentVehiclePhysicsContext;
}

FVehiclePhysicsExecutionScope::FVehiclePhysicsExecutionScope(const FVehiclePhysicsExecutionContext& Context)
	: PreviousContext(CurrentVehiclePhysicsContext)
{
	CurrentVehiclePhysicsContext = &Context;
}

FVehiclePhysicsExecutionScope::~FVehiclePhysicsExecutionScope()
{
	CurrentVehiclePhysicsContext = PreviousContext;
}

FVehiclePhysicsStep::FVehiclePhysicsStep(TFunction<void(float)> InUpdate)
	: Update(MoveTemp(InUpdate)), IdleEvent(FPlatformProcess::GetSynchEventFromPool(true))
{
	IdleEvent->Trigger();
}

FVehiclePhysicsStep::FVehiclePhysicsStep(TFunction<void(float, const FVehiclePhysicsExecutionContext&)> InUpdate)
	: ContextUpdate(MoveTemp(InUpdate)), bRequiresContext(true), IdleEvent(FPlatformProcess::GetSynchEventFromPool(true))
{
	IdleEvent->Trigger();
}

FVehiclePhysicsStep::~FVehiclePhysicsStep()
{
	check(!bExecuting); // The executing caller must retain its shared entry until TryExecute returns.
	FPlatformProcess::ReturnSynchEventToPool(IdleEvent);
}

bool FVehiclePhysicsStep::TryExecute(float DeltaTime, bool bAutomaticCall)
{
	if (bRequiresContext || !TryBeginExecution(bAutomaticCall)) return false;
	ON_SCOPE_EXIT { FinishExecution(); };
	Update(DeltaTime); // No lock is held while solving, querying or applying impulses.
	return true;
}

bool FVehiclePhysicsStep::TryExecute(float DeltaTime, const FVehiclePhysicsExecutionContext& Context, bool bAutomaticCall)
{
	if (!TryBeginExecution(bAutomaticCall)) return false;
	ON_SCOPE_EXIT { FinishExecution(); };
	if (bRequiresContext)
	{
		ContextUpdate(DeltaTime, Context); // The borrowed solver context is not retained beyond this call.
	}
	else
	{
		Update(DeltaTime);
	}
	return true;
}

bool FVehiclePhysicsStep::TryBeginExecution(bool bAutomaticCall)
{
	{
		FScopeLock Lock(&EntryMutex);
		if (bStopped || bExecuting || (bAutomaticCall && !bAutomaticUpdateEnabled)) return false;
		bExecuting = true;
		IdleEvent->Reset();
	}
	return true;
}

void FVehiclePhysicsStep::FinishExecution()
{
	FScopeLock Lock(&EntryMutex);
	bExecuting = false;
	IdleEvent->Trigger();
}

void FVehiclePhysicsStep::SetAutomaticUpdateEnabled(bool bEnabled)
{
	FScopeLock Lock(&EntryMutex);
	bAutomaticUpdateEnabled = bEnabled;
}

void FVehiclePhysicsStep::StopAndWait()
{
	{
		FScopeLock Lock(&EntryMutex);
		bStopped = true;
		bBindingInvalidated = false; // Explicit stop takes precedence over dependency recovery.
	}
	// Admission is closed before waiting; even a previously copied callback cannot start again.
	IdleEvent->Wait();
}

bool FVehiclePhysicsStep::IsStopped() const
{
	FScopeLock Lock(&EntryMutex);
	return bStopped;
}

void FVehiclePhysicsStep::InvalidateBindingAndWait()
{
	{
		FScopeLock Lock(&EntryMutex);
		if (!bStopped) bBindingInvalidated = true;
		bStopped = true;
	}
	IdleEvent->Wait();
}

bool FVehiclePhysicsStep::IsBindingInvalidated() const
{
	FScopeLock Lock(&EntryMutex);
	return bBindingInvalidated;
}

bool FVehiclePhysicsOwnership::CanClaim(const TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe>& Step) const
{
	if (!Step || Step->IsStopped()) return false;
	FScopeLock Lock(&OwnerMutex);
	const auto Existing = Owner.Pin();
	return !Existing || Existing == Step || Existing->IsStopped();
}

void FVehiclePhysicsOwnership::Claim(const TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe>& Step)
{
	check(IsInGameThread());
	check(Step && !Step->IsStopped());
	FScopeLock Lock(&OwnerMutex);
	const auto Existing = Owner.Pin();
	check(!Existing || Existing == Step || Existing->IsStopped());
	Owner = Step;
}

bool FVehiclePhysicsOwnership::HasActiveOwner() const
{
	FScopeLock Lock(&OwnerMutex);
	const auto Existing = Owner.Pin();
	return Existing && !Existing->IsStopped();
}

void FVehiclePhysicsOwnership::InvalidateOwnerAndWait()
{
	check(IsInGameThread());
	TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe> Existing;
	{
		FScopeLock Lock(&OwnerMutex);
		Existing = Owner.Pin();
	}
	if (Existing) Existing->InvalidateBindingAndWait();
}
