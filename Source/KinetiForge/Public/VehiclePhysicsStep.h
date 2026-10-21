// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "HAL/CriticalSection.h"

class FEvent;
namespace Chaos { class FPBDRigidsSolver; class FRigidBodyHandle_Internal; }
class FVehiclePhysicsBodyIdentity;

/** Borrowed solver context valid only for the callback invocation that created it. */
class KINETIFORGE_API FVehiclePhysicsExecutionContext
{
public:
	explicit FVehiclePhysicsExecutionContext(Chaos::FPBDRigidsSolver& InSolver) : Solver(InSolver) {}
	Chaos::FPBDRigidsSolver& GetSolver() const { return Solver; }
	/** Available only inside a scene callback scope on this thread; never retained between steps. */
	static const FVehiclePhysicsExecutionContext* GetCurrent();
	/** Callback-only resolution; the returned PT handle is borrowed and must not outlive this invocation. */
	Chaos::FRigidBodyHandle_Internal* ResolveBody(const FVehiclePhysicsBodyIdentity& Identity) const;

private:
	Chaos::FPBDRigidsSolver& Solver;
};

/** Publishes a borrowed callback context for legacy manual UpdatePhysics(float) callers.
 * Nested scopes restore their caller's context; this owns neither solver nor scene. */
class KINETIFORGE_API FVehiclePhysicsExecutionScope
{
public:
	explicit FVehiclePhysicsExecutionScope(const FVehiclePhysicsExecutionContext& Context);
	~FVehiclePhysicsExecutionScope();
	FVehiclePhysicsExecutionScope(const FVehiclePhysicsExecutionScope&) = delete;
	FVehiclePhysicsExecutionScope& operator=(const FVehiclePhysicsExecutionScope&) = delete;
private:
	const FVehiclePhysicsExecutionContext* PreviousContext;
};

/** Cancellable DriveAssembly entry shared by manual and automatic callers.
 * StopAndWait is a lifecycle operation, outside the step itself. A stopped entry
 * is never reopened: a new registration gets a new entry so old snapshots stay inert.
 * Child ownership slots can invalidate the entry before their lifecycle changes.
 * Borrowed bodies and callers bypassing this entry still need separate lifetime management.
 */
class KINETIFORGE_API FVehiclePhysicsStep
{
public:
	explicit FVehiclePhysicsStep(TFunction<void(float)> InUpdate);
	explicit FVehiclePhysicsStep(TFunction<void(float, const FVehiclePhysicsExecutionContext&)> InUpdate);
	~FVehiclePhysicsStep();
	FVehiclePhysicsStep(const FVehiclePhysicsStep&) = delete;
	FVehiclePhysicsStep& operator=(const FVehiclePhysicsStep&) = delete;
	bool TryExecute(float DeltaTime, bool bAutomaticCall = false);
	bool TryExecute(float DeltaTime, const FVehiclePhysicsExecutionContext& Context, bool bAutomaticCall = false);
	void SetAutomaticUpdateEnabled(bool bEnabled);
	void StopAndWait();
	void InvalidateBindingAndWait(); // Dependency lifecycle; GT may rebuild a fresh entry later.
	bool IsStopped() const;
	bool IsBindingInvalidated() const;

private:
	TFunction<void(float)> Update;
	TFunction<void(float, const FVehiclePhysicsExecutionContext&)> ContextUpdate;
	bool bRequiresContext = false;
	mutable FCriticalSection EntryMutex;
	FEvent* IdleEvent;
	bool bStopped = false;
	bool bBindingInvalidated = false;
	bool bExecuting = false;
	bool bAutomaticUpdateEnabled = true;
	bool TryBeginExecution(bool bAutomaticCall);
	void FinishExecution();
};

/** Small ownership slot shared by axle/wheel adapters, not a drivetrain topology.
 * Claims are made on GT after old owners have drained and the complete assembly has passed preflight.
 * Manual wrappers may inspect the slot from the simulation thread.
 */
class KINETIFORGE_API FVehiclePhysicsOwnership
{
public:
	bool CanClaim(const TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe>& Step) const;
	void Claim(const TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe>& Step);
	bool HasActiveOwner() const;
	void InvalidateOwnerAndWait();
private:
	mutable FCriticalSection OwnerMutex;
	TWeakPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe> Owner;
};
