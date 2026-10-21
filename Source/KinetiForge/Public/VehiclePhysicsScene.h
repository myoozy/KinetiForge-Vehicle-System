// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "VehiclePhysicsSchedule.h"

class FPhysScene_Chaos;
class FVehicleSceneCallback;
class FVehicleFrozenSceneCallback;
namespace Chaos { class FPBDRigidsSolver; }

enum class EVehiclePhysicsExecutionLane : uint8
{
	Worker,
	FrozenGameThread
};

/** GT-owned opt-in callback host for one live Chaos scene. It does not alter existing vehicle routing. */
class KINETIFORGE_API FVehiclePhysicsScene
{
public:
	using FStepPtr = TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe>;
	using FContextUpdate = TFunction<void(float, const FVehiclePhysicsExecutionContext&)>;

	~FVehiclePhysicsScene();
	FVehiclePhysicsScene(const FVehiclePhysicsScene&) = delete;
	FVehiclePhysicsScene& operator=(const FVehiclePhysicsScene&) = delete;

	FStepPtr RegisterStep(FContextUpdate Update, bool bAutomaticUpdateEnabled = true,
		EVehiclePhysicsExecutionLane Lane = EVehiclePhysicsExecutionLane::Worker);
	bool UnregisterStep(const FStepPtr& Step);
	// GT-only lifecycle operation. Must not be called from a registered step because it drains those steps.
	void Close();
	// GT-only status query.
	bool IsClosed() const { return bClosed; }

private:
	explicit FVehiclePhysicsScene(FPhysScene_Chaos& InScene);
	void ScenePreTick(FPhysScene_Chaos* InScene, float DeltaTime);
	void EnsureFrozenCallback();

	FPhysScene_Chaos* Scene = nullptr;
	Chaos::FPBDRigidsSolver* Solver = nullptr;
	FVehicleSceneCallback* Callback = nullptr;
	FVehicleFrozenSceneCallback* FrozenCallback = nullptr;
	FDelegateHandle PreTickHandle;
	TSharedPtr<FVehiclePhysicsSchedule, ESPMode::ThreadSafe> WorkerSchedule;
	TSharedPtr<FVehiclePhysicsSchedule, ESPMode::ThreadSafe> FrozenSchedule;
	FVehiclePhysicsSchedule::FStepArray WorkerSteps;
	FVehiclePhysicsSchedule::FStepArray FrozenSteps;
	bool bClosed = false;

	friend class FKinetiForgeModule;
};
