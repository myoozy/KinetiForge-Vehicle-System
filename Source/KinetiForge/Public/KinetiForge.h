// Copyright Epic Games, Inc. All Rights Reserved.
// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Engine/World.h"
#include "VehiclePhysicsScene.h"

class FPhysScene_Chaos;
class UWorld;

class FKinetiForgeModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override { return false; }

	/** Explicit, lazy opt-in. Merely creating a world does not install a solver callback. */
	KINETIFORGE_API TSharedPtr<FVehiclePhysicsScene, ESPMode::ThreadSafe> AcquirePhysicsScene(FPhysScene_Chaos* Scene);
	KINETIFORGE_API TSharedPtr<FVehiclePhysicsScene, ESPMode::ThreadSafe> FindPhysicsScene(FPhysScene_Chaos* Scene) const;

private:
	void OnWorldPostInit(UWorld* World, const UWorld::InitializationValues IVS);
	void OnWorldCleanup(UWorld* World, bool bSessionEnded, bool bCleanupResources);
	void OnPhysSceneTerm(FPhysScene_Chaos* Scene);

	TMap<FPhysScene_Chaos*, TSharedPtr<FVehiclePhysicsScene, ESPMode::ThreadSafe>> SceneHosts;
	FDelegateHandle PostWorldInitHandle;
	FDelegateHandle WorldCleanupHandle;
	FDelegateHandle PhysSceneTermHandle;
	bool bShuttingDown = false;
};
