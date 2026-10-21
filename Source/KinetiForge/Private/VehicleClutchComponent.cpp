// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)


#include "VehicleClutchComponent.h"
#include "VehicleTransmissionSolver.h"
#include "VehiclePowerUnitBridge.h"

// Sets default values for this component's properties
UVehicleClutchComponent::UVehicleClutchComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PrePhysics;

	// ...
}


// Called when the game starts
void UVehicleClutchComponent::BeginPlay()
{
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::BeginPlay();

	// ...
}


float UVehicleClutchComponent::GetTorqueSpringModel(
	const float DeltaTime,
	const float ClutchSlip,
	const float EngineInertia,
	const float GearboxReflectedInertia,
	const float DriveShaftStiffness)
{
	if (bPowerUnitBridgeBound.Load()) return State.ClutchTorque;
	return FVehicleClutchSolver::SolveSpringTorque(Config, State,
		DeltaTime, ClutchSlip, EngineInertia, GearboxReflectedInertia, DriveShaftStiffness);
}

float UVehicleClutchComponent::GetTorqueDampingModel(
	const float DeltaTime,
	const float ClutchSlip,
	const float EngineInertia,
	const float GearboxReflectedInertia)
{
	if (bPowerUnitBridgeBound.Load()) return State.ClutchTorque;
	return FVehicleClutchSolver::SolveDampingTorque(Config, State,
		DeltaTime, ClutchSlip, EngineInertia, GearboxReflectedInertia);
}

float UVehicleClutchComponent::GetTorqueConstraintModel(
	const float DeltaTime,
	const float ClutchSlip,
	const float EngineInertia,
	const float GearboxReflectedInertia)
{
	if (bPowerUnitBridgeBound.Load()) return State.ClutchTorque;
	return FVehicleClutchSolver::SolveConstraintTorque(State,
		DeltaTime, ClutchSlip, EngineInertia, GearboxReflectedInertia);
}

// Called every frame
void UVehicleClutchComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UVehicleClutchComponent::UpdatePhysics(
	const float InDeltaTime,
	const float InClutchValue,
	const float InGearboxInputShaftVelocity,
	const float InGearboxReflectedInertia,
	const float InCurrentGearRatio,
	const float InDriveShaftStiffness,
	const float InEngineAngularVelocity,
	const float InEngineInertia,
	const float InEngineMaxTorque)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_Clutch_UpdatePhysics);
	if (bPowerUnitBridgeBound.Load()) return;

	FVehicleClutchStepInput Input;
	Input.DeltaTime = InDeltaTime;
	Input.ClutchPedalValue = InClutchValue;
	Input.GearboxInputAngularVelocity = InGearboxInputShaftVelocity;
	Input.GearboxReflectedInertia = InGearboxReflectedInertia;
	Input.GearRatio = InCurrentGearRatio;
	Input.ReflectedDriveShaftStiffness = InDriveShaftStiffness;
	// get some data from engine
	Input.EngineAngularVelocity = InEngineAngularVelocity;
	Input.EngineInertia = InEngineInertia;
	Input.EngineMaxTorque = InEngineMaxTorque;
	FVehicleClutchSolver::Update(Config, State, Input);
}

void UVehicleClutchComponent::SetConfig(const FVehicleClutchConfig& NewConfig)
{
	if (bPowerUnitBridgeBound.Load())
	{
		check(IsInGameThread());
		Config = NewConfig;
		if (TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> Bridge = PowerUnitBridge.Pin())
		{
			Bridge->SubmitClutchSettings(Config);
		}
		return;
	}
	Config = NewConfig;
}

bool UVehicleClutchComponent::CanBindPowerUnitBridge() const
{
	check(IsInGameThread());
	return !bPowerUnitBridgeBound.Load();
}

bool UVehicleClutchComponent::BindPowerUnitBridge(
	TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> InBridge)
{
	check(IsInGameThread());
	if (!CanBindPowerUnitBridge() || !InBridge.IsValid()) return false;
	PowerUnitBridge = InBridge;
	bPowerUnitBridgeBound.Store(true);
	InBridge->SubmitClutchSettings(Config);
	RefreshPublishedRuntimeState();
	return true;
}

void UVehicleClutchComponent::RefreshPublishedRuntimeState()
{
	if (!bPowerUnitBridgeBound.Load()) return;
	check(IsInGameThread());
	if (TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> Bridge = PowerUnitBridge.Pin())
	{
		Bridge->ReadClutchResult(State);
	}
}

float UVehicleClutchComponent::GetCluchTorque()
{
	return State.ClutchTorque;
}

void UVehicleClutchComponent::OnRegister()
{
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::OnRegister();
}

void UVehicleClutchComponent::OnUnregister()
{
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::OnUnregister();
}

void UVehicleClutchComponent::BeginDestroy()
{
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::BeginDestroy();
}

void UVehicleClutchComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::EndPlay(EndPlayReason);
}

void UVehicleClutchComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}
