// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehiclePhysicsScene.h"

#include "Chaos/SimCallbackObject.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "PBDRigidsSolver.h"

struct FVehicleSceneCallbackInput : Chaos::FSimCallbackInput
{
	TSharedPtr<FVehiclePhysicsSchedule, ESPMode::ThreadSafe> Schedule;
	void Reset() { Schedule.Reset(); }
};

template <Chaos::ESimCallbackOptions Options>
class TVehicleSceneCallback : public Chaos::TSimCallbackObject<FVehicleSceneCallbackInput, Chaos::FSimCallbackNoOutput, Options>
{
private:
	void OnPreSimulate_Internal() override
	{
		const FVehicleSceneCallbackInput* Input = this->GetConsumerInput_Internal();
		if (!Input || !Input->Schedule.IsValid()) return;
		auto* SolverPtr = static_cast<Chaos::FPBDRigidsSolver*>(this->GetSolver());
		if (!SolverPtr) return;
		const FVehiclePhysicsExecutionContext Context(*SolverPtr);
		const FVehiclePhysicsExecutionScope Scope(Context);
		FVehiclePhysicsSchedule::ExecuteCapturedSteps(Input->Schedule->CaptureSteps(), this->GetDeltaTime_Internal(), Context, true);
	}
};

class FVehicleSceneCallback final : public TVehicleSceneCallback<Chaos::ESimCallbackOptions::Presimulate> {};
class FVehicleFrozenSceneCallback final : public TVehicleSceneCallback<
	Chaos::ESimCallbackOptions::Presimulate | Chaos::ESimCallbackOptions::RunOnFrozenGameThread> {};

FVehiclePhysicsScene::FVehiclePhysicsScene(FPhysScene_Chaos& InScene)
	: Scene(&InScene)
	, WorkerSchedule(MakeShared<FVehiclePhysicsSchedule, ESPMode::ThreadSafe>())
	, FrozenSchedule(MakeShared<FVehiclePhysicsSchedule, ESPMode::ThreadSafe>())
{
	check(IsInGameThread());
	Solver = static_cast<Chaos::FPBDRigidsSolver*>(InScene.GetSolver());
	check(Solver);
	Callback = Solver->CreateAndRegisterSimCallbackObject_External<FVehicleSceneCallback>();
	check(Callback);
	PreTickHandle = InScene.OnPhysScenePreTick.AddRaw(this, &FVehiclePhysicsScene::ScenePreTick);
}

FVehiclePhysicsScene::~FVehiclePhysicsScene()
{
	if (!bClosed)
	{
		check(IsInGameThread());
		Close();
	}
}

FVehiclePhysicsScene::FStepPtr FVehiclePhysicsScene::RegisterStep(FContextUpdate Update,
	bool bAutomaticUpdateEnabled, EVehiclePhysicsExecutionLane Lane)
{
	check(IsInGameThread());
	if (bClosed || !Scene || !Solver || !Callback || !Update) return nullptr;
	if (Lane == EVehiclePhysicsExecutionLane::FrozenGameThread)
	{
		EnsureFrozenCallback();
		if (!FrozenCallback) return nullptr;
	}
	FStepPtr Step = MakeShared<FVehiclePhysicsStep, ESPMode::ThreadSafe>(MoveTemp(Update));
	Step->SetAutomaticUpdateEnabled(bAutomaticUpdateEnabled);
	FVehiclePhysicsSchedule::FStepArray Retired;
	if (Lane == EVehiclePhysicsExecutionLane::FrozenGameThread)
	{
		FrozenSteps.Add(Step);
		Retired = FrozenSchedule->PublishSteps(FrozenSteps);
	}
	else
	{
		WorkerSteps.Add(Step);
		Retired = WorkerSchedule->PublishSteps(WorkerSteps);
	}
	return Step;
}

bool FVehiclePhysicsScene::UnregisterStep(const FStepPtr& Step)
{
	check(IsInGameThread());
	if (bClosed || !Step) return false;
	FVehiclePhysicsSchedule::FStepArray Retired;
	if (WorkerSteps.RemoveSingle(Step) > 0)
	{
		Retired = WorkerSchedule->PublishSteps(WorkerSteps);
	}
	else if (FrozenSteps.RemoveSingle(Step) > 0)
	{
		Retired = FrozenSchedule->PublishSteps(FrozenSteps);
	}
	else
	{
		return false;
	}
	Step->StopAndWait();
	return true;
}

void FVehiclePhysicsScene::EnsureFrozenCallback()
{
	check(IsInGameThread());
	if (bClosed || FrozenCallback || !Solver) return;
	FrozenCallback = Solver->CreateAndRegisterSimCallbackObject_External<FVehicleFrozenSceneCallback>();
}

void FVehiclePhysicsScene::Close()
{
	check(IsInGameThread());
	if (bClosed) return;
	bClosed = true;
	FPhysScene_Chaos* SceneToClose = Scene;
	Chaos::FPBDRigidsSolver* SolverToClose = Solver;
	FVehicleSceneCallback* CallbackToClose = Callback;
	FVehicleFrozenSceneCallback* FrozenCallbackToClose = FrozenCallback;
	if (SceneToClose && PreTickHandle.IsValid()) SceneToClose->OnPhysScenePreTick.Remove(PreTickHandle);
	PreTickHandle.Reset();
	Scene = nullptr;
	Solver = nullptr;
	Callback = nullptr;
	FrozenCallback = nullptr;
	WorkerSchedule->ClearAndStop();
	FrozenSchedule->ClearAndStop();
	WorkerSteps.Reset();
	FrozenSteps.Reset();
	if (SolverToClose && CallbackToClose)
	{
		SolverToClose->UnregisterAndFreeSimCallbackObject_External(CallbackToClose);
	}
	if (SolverToClose && FrozenCallbackToClose)
	{
		SolverToClose->UnregisterAndFreeSimCallbackObject_External(FrozenCallbackToClose);
	}
}

void FVehiclePhysicsScene::ScenePreTick(FPhysScene_Chaos* InScene, float DeltaTime)
{
	check(IsInGameThread());
	if (bClosed || InScene != Scene || !Callback) return;
	if (FVehicleSceneCallbackInput* Input = Callback->GetProducerInputData_External())
	{
		Input->Schedule = WorkerSchedule;
	}
	if (FrozenCallback)
	{
		if (FVehicleSceneCallbackInput* Input = FrozenCallback->GetProducerInputData_External())
		{
			Input->Schedule = FrozenSchedule;
		}
	}
}
