// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehiclePhysicsBodyBinding.h"
#include "ProfilingDebugging/CpuProfilerTrace.h"

#include "Chaos/Framework/PhysicsProxyBase.h"
#include "Chaos/PBDRigidsEvolution.h"
#include "Chaos/PBDRigidsSOAs.h"
#include "Chaos/ParticleHandle.h"
#include "PhysicsEngine/BodyInstance.h"
#include "PhysicsProxy/SingleParticlePhysicsProxy.h"
#include "PBDRigidsSolver.h"
#include "VehiclePhysicsStep.h"

FVehiclePhysicsBodyIdentity FVehiclePhysicsBodyIdentity::Capture(const FBodyInstance* BodyInstance)
{
	check(IsInGameThread());
	FVehiclePhysicsBodyIdentity Identity;
	if (!BodyInstance) return Identity;
	const FPhysicsActorHandle& ActorHandle = BodyInstance->GetPhysicsActorHandle();
	if (ActorHandle)
	{
		Identity.Timestamp = ActorHandle->GetSyncTimestamp();
	}
	return Identity;
}

Chaos::FRigidBodyHandle_Internal* FVehiclePhysicsExecutionContext::ResolveBody(const FVehiclePhysicsBodyIdentity& Identity) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_ResolveBody);
	if (!Identity.Timestamp.IsValid()) return nullptr;
	Chaos::FPBDRigidsSolver& CurrentSolver = GetSolver();
	Chaos::FPBDRigidsEvolution* Evolution = CurrentSolver.GetEvolution();
	if (!Evolution) return nullptr;

	const auto& ParticleHandles = Evolution->GetParticles().GetParticleHandles();
	const int32 NumParticleHandles = static_cast<int32>(ParticleHandles.Size());
	for (int32 ParticleIndex = 0; ParticleIndex < NumParticleHandles; ++ParticleIndex)
	{
		Chaos::FGeometryParticleHandle* Particle = ParticleHandles.Handle(ParticleIndex).Get();
		if (!Particle) continue;
		IPhysicsProxyBase* Proxy = Particle->PhysicsProxy();
		if (!Proxy || Proxy->GetType() != EPhysicsProxyType::SingleParticleProxy
			|| Proxy->GetSyncTimestamp().Get() != Identity.Timestamp.Get()) continue;

		auto* SingleProxy = static_cast<Chaos::FSingleParticlePhysicsProxy*>(Proxy);
		if (SingleProxy->GetHandle_LowLevel() != Particle) continue;
		Chaos::FRigidBodyHandle_Internal* PhysicsHandle = SingleProxy->GetPhysicsThreadAPI();
		if (PhysicsHandle) return PhysicsHandle;
	}
	return nullptr;
}

Chaos::FRigidBodyHandle_Internal* FVehiclePhysicsMacrostepBodyResolver::ResolveBody(
	const FVehiclePhysicsBodyIdentity& Identity)
{
	for (const FEntry& Entry : Entries)
	{
		if (Entry.Identity == Identity) return Entry.Handle;
	}
	++UnderlyingResolveCount;
	Chaos::FRigidBodyHandle_Internal* Handle = Context.ResolveBody(Identity);
	Entries.Add({ Identity, Handle });
	return Handle;
}
