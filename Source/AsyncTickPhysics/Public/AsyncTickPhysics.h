// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FAsyncTickPhysicsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	// Delegates for the Physics Scene init and termination.
	void PhysScene_OnPhysSceneInit(FPhysScene* Scene);
	void PhysScene_OnPhysSceneTerm(FPhysScene* Scene);

	// Delegate Handles
	FDelegateHandle OnPhysSceneInit;
	FDelegateHandle OnPhysSceneTerm;
};
