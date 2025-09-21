#pragma once

#include "CoreMinimal.h"
#include "AsyncTickPawn.h"
#include "AsyncTickActorComponent.h"
#include "PhysicsPublic.h"
#include "Chaos/SimCallbackInput.h"
#include "Chaos/SimCallbackObject.h"
#include "Chaos/GeometryParticlesfwd.h"
#include "PhysicsProxy/SingleParticlePhysicsProxyFwd.h"

// Data that will be used inside the physics thread
struct FAsyncPhysicsInput : public Chaos::FSimCallbackInput
{
	TArray<TWeakObjectPtr<AAsyncTickPawn>> Pawns;
	TArray<TWeakObjectPtr<UAsyncTickActorComponent>> ActorComponents;
	
	TWeakObjectPtr<UWorld> World;
	bool DebugMessages;

	// Required method
	void Reset()
	{
		Pawns.Reset();
		ActorComponents.Reset();
		World.Reset();
	}
};

// Output from the physics thread
struct FAsyncPhysicsOutput : public Chaos::FSimCallbackOutput
{
	// Required method
	void Reset()
	{
	}
};

class FAsyncPhysicsCallback : public Chaos::TSimCallbackObject<FAsyncPhysicsInput, FAsyncPhysicsOutput> {
	// This is the function in which we will do our physics STUFF!
	virtual void OnPreSimulate_Internal() override;

	// Not used.
	virtual void OnContactModification_Internal(Chaos::FCollisionContactModifier& Modifier) override;
};
