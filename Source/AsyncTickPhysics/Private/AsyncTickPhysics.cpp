// Copyright Epic Games, Inc. All Rights Reserved.

#include "AsyncTickPhysics.h"
#include "AsyncTickManager.h"

#define LOCTEXT_NAMESPACE "FAsyncTickPhysicsModule"

void FAsyncTickPhysicsModule::StartupModule()
{
	this->OnPhysSceneInit = FPhysicsDelegates::OnPhysSceneInit.AddRaw(this, &FAsyncTickPhysicsModule::PhysScene_OnPhysSceneInit);
	this->OnPhysSceneTerm = FPhysicsDelegates::OnPhysSceneTerm.AddRaw(this, &FAsyncTickPhysicsModule::PhysScene_OnPhysSceneTerm);
}

void FAsyncTickPhysicsModule::ShutdownModule()
{
	FPhysicsDelegates::OnPhysSceneInit.Remove(this->OnPhysSceneInit);
	FPhysicsDelegates::OnPhysSceneTerm.Remove(this->OnPhysSceneTerm);
}

// Create the new tick manager
void FAsyncTickPhysicsModule::PhysScene_OnPhysSceneInit(FPhysScene* Scene)
{
	new FAsyncTickManager(Scene);
}

// Delete the current tick manager
void FAsyncTickPhysicsModule::PhysScene_OnPhysSceneTerm(FPhysScene* Scene)
{
	FAsyncTickManager* PhysManager = FAsyncTickManager::GetPhysicsManagerFromScene(Scene);

	if(PhysManager != nullptr)
	{
		PhysManager->DetachFromPhysScene(Scene);
		delete PhysManager;
		PhysManager = nullptr;
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAsyncTickPhysicsModule, AsyncTickPhysics)