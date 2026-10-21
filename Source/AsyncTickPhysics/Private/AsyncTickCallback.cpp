#include "AsyncTickCallback.h"
#include "AsyncTickPhysicsExecution.h"

namespace
{
	thread_local Chaos::FPhysicsSolverBase* CurrentAsyncTickSolver = nullptr;
}

Chaos::FPhysicsSolverBase* AsyncTickPhysics::GetCurrentSolver()
{
	return CurrentAsyncTickSolver;
}

void FAsyncPhysicsCallback::OnPreSimulate_Internal()
{
	using namespace Chaos;

	const FAsyncPhysicsInput* Input = GetConsumerInput_Internal();

	if(Input == nullptr)
		return;

	UWorld* World = Input->World.Get();

	if(World == nullptr)
		return;

	// Expose only the solver that invoked this callback, during legacy event dispatch.
	TGuardValue<Chaos::FPhysicsSolverBase*> SolverScope(CurrentAsyncTickSolver, GetSolver());
	const TArray<TWeakObjectPtr<AAsyncTickPawn>>& Pawns = Input->Pawns;
	const TArray<TWeakObjectPtr<UAsyncTickActorComponent>>& ActorComponents = Input->ActorComponents;
	
	for(const TWeakObjectPtr<AAsyncTickPawn> Pawn : Pawns)
	{
		if(!Pawn.IsValid())
			continue;
		
		Pawn->NativeAsyncTick(GetDeltaTime_Internal());
	}

		
	for(const TWeakObjectPtr<UAsyncTickActorComponent> ActorComponent : ActorComponents)
	{
		if(!ActorComponent.IsValid())
			continue;
		
		ActorComponent->NativeAsyncTick(GetDeltaTime_Internal());
	}
}

void FAsyncPhysicsCallback::OnContactModification_Internal(Chaos::FCollisionContactModifier& Modifier)
{
}
