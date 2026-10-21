// Copyright Epic Games, Inc. All Rights Reserved.
// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "KinetiForge.h"
#include "Engine/World.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "PhysicsPublic.h"

#define LOCTEXT_NAMESPACE "FKinetiForgeModule"

void FKinetiForgeModule::StartupModule()
{
	check(IsInGameThread());
	bShuttingDown = false;
	if (!PostWorldInitHandle.IsValid())
	{
		PostWorldInitHandle = FWorldDelegates::OnPostWorldInitialization.AddRaw(this, &FKinetiForgeModule::OnWorldPostInit);
	}
	if (!WorldCleanupHandle.IsValid())
	{
		WorldCleanupHandle = FWorldDelegates::OnWorldCleanup.AddRaw(this, &FKinetiForgeModule::OnWorldCleanup);
	}
	if (!PhysSceneTermHandle.IsValid())
	{
		PhysSceneTermHandle = FPhysicsDelegates::OnPhysSceneTerm.AddRaw(this, &FKinetiForgeModule::OnPhysSceneTerm);
	}
}

void FKinetiForgeModule::ShutdownModule()
{
	check(IsInGameThread());
	bShuttingDown = true;
	if (PostWorldInitHandle.IsValid()) FWorldDelegates::OnPostWorldInitialization.Remove(PostWorldInitHandle);
	if (WorldCleanupHandle.IsValid()) FWorldDelegates::OnWorldCleanup.Remove(WorldCleanupHandle);
	if (PhysSceneTermHandle.IsValid()) FPhysicsDelegates::OnPhysSceneTerm.Remove(PhysSceneTermHandle);
	PostWorldInitHandle.Reset();
	WorldCleanupHandle.Reset();
	PhysSceneTermHandle.Reset();
	TArray<TSharedPtr<FVehiclePhysicsScene, ESPMode::ThreadSafe>> RetiredHosts;
	SceneHosts.GenerateValueArray(RetiredHosts);
	SceneHosts.Reset();
	for (const TSharedPtr<FVehiclePhysicsScene, ESPMode::ThreadSafe>& Host : RetiredHosts)
	{
		if (Host) Host->Close();
	}
}

TSharedPtr<FVehiclePhysicsScene, ESPMode::ThreadSafe> FKinetiForgeModule::AcquirePhysicsScene(FPhysScene_Chaos* Scene)
{
	check(IsInGameThread());
	if (bShuttingDown || !Scene || !Scene->GetSolver()) return nullptr;
	if (TSharedPtr<FVehiclePhysicsScene, ESPMode::ThreadSafe>* Existing = SceneHosts.Find(Scene))
	{
		return (*Existing && !(*Existing)->IsClosed()) ? *Existing : nullptr;
	}
	TSharedPtr<FVehiclePhysicsScene, ESPMode::ThreadSafe> Host(new FVehiclePhysicsScene(*Scene));
	SceneHosts.Add(Scene, Host);
	return Host;
}

TSharedPtr<FVehiclePhysicsScene, ESPMode::ThreadSafe> FKinetiForgeModule::FindPhysicsScene(FPhysScene_Chaos* Scene) const
{
	check(IsInGameThread());
	if (const auto* Host = SceneHosts.Find(Scene)) return *Host;
	return nullptr;
}

void FKinetiForgeModule::OnWorldPostInit(UWorld* World, const UWorld::InitializationValues IVS)
{
	check(IsInGameThread());
	if (bShuttingDown || !World) return;
	FPhysScene_Chaos* Scene = World->GetPhysicsScene();
	if (TSharedPtr<FVehiclePhysicsScene, ESPMode::ThreadSafe>* Existing = SceneHosts.Find(Scene))
	{
		if (*Existing && (*Existing)->IsClosed()) SceneHosts.Remove(Scene);
	}
}

void FKinetiForgeModule::OnWorldCleanup(UWorld* World, bool, bool)
{
	check(IsInGameThread());
	if (bShuttingDown || !World) return;
	if (TSharedPtr<FVehiclePhysicsScene, ESPMode::ThreadSafe>* Host = SceneHosts.Find(World->GetPhysicsScene()))
	{
		if (*Host) (*Host)->Close();
	}
}

void FKinetiForgeModule::OnPhysSceneTerm(FPhysScene_Chaos* Scene)
{
	check(IsInGameThread());
	TSharedPtr<FVehiclePhysicsScene, ESPMode::ThreadSafe> Host;
	if (SceneHosts.RemoveAndCopyValue(Scene, Host) && Host) Host->Close();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FKinetiForgeModule, KinetiForge)
