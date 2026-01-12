// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)


#include "VehicleClutchComponent.h"
#include "VehicleWheelCoordinatorComponent.h"
#include "VehicleEngineComponent.h"
#include "VehicleUtil.h"

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
	Super::BeginPlay();

	// ...
}


void UVehicleClutchComponent::UpdateSpringStiffness()
{
	float FirstGearTotalInertia = VehicleUtil::SafeDivide(State.EngineInertia * State.FirstGearReflectedInertia, State.EngineInertia + State.FirstGearReflectedInertia);
	State.SpringStiffness = CalculateStiffness(Config.NaturalFrequency, FirstGearTotalInertia);
	State.CriticalDamping = CalculateCriticalDamping(Config.NaturalFrequency, FirstGearTotalInertia);
	State.SpringDamping = State.CriticalDamping * Config.Damping;
}

float UVehicleClutchComponent::GetTorqueSpringModel()
{
	UpdateSpringStiffness();
	
	float ClutchSlipScaled = State.ClutchSlip * State.ClutchLock;//ClutchSlip * ClutchLock
	float CurrentAngleDiff = State.AngleDiff;

	float DontKnowWhatItIs = State.SpringStiffness * State.PhysicsDeltaTime + State.SpringDamping;
	float TorqueNumerator = State.SpringStiffness * CurrentAngleDiff + DontKnowWhatItIs * ClutchSlipScaled;
	float TorqueDenominator = 1.0f + VehicleUtil::SafeDivide(DontKnowWhatItIs * State.PhysicsDeltaTime, State.TotalInertia);

	float SpringModelTorque = TorqueNumerator / TorqueDenominator;

	float DampingModelTorque = State.ClutchLock * FMath::Clamp(State.CriticalDamping * State.ClutchSlip, -State.MaxClutchTorque, State.MaxClutchTorque);
	
	if (FMath::Abs(SpringModelTorque) > State.MaxClutchTorque)
	{
		SpringModelTorque = FMath::Sign(SpringModelTorque) * State.MaxClutchTorque;
	}
	else
	{
		State.AngleDiff = State.ClutchLock * (State.AngleDiff + ClutchSlipScaled * State.PhysicsDeltaTime);
	}

	return FMath::Lerp(DampingModelTorque, SpringModelTorque, State.ClutchLock);
}

float UVehicleClutchComponent::GetTorqueDampingModel()
{
	State.AngleDiff = 0.f;

	float GameFrequency = VehicleUtil::SafeDivide(1.f, State.PhysicsDeltaTime);
	State.CriticalDamping = 2.f * State.TotalInertia * GameFrequency;

	float UnSmoothenedTorque = FMath::Clamp(State.ClutchSlip * State.CriticalDamping, -State.MaxClutchTorque, State.MaxClutchTorque);
	State.SmoothenedTorque += FMath::Clamp(1 - Config.Damping, 0, 1) * (UnSmoothenedTorque - State.SmoothenedTorque);
	return State.SmoothenedTorque * State.ClutchLock;
}

// Called every frame
void UVehicleClutchComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UVehicleClutchComponent::UpdatePhysics(float InDeltaTime, float InClutchValue, float InGearboxInputShaftVelocity, float InReflectedInertia, float InCurrentGearRatio, float InFirstGearReflectedInertia, UVehicleEngineComponent* TargetEngine)
{
	State.PhysicsDeltaTime = InDeltaTime;
	InClutchValue = FMath::Clamp(InClutchValue, 0.f, 1.f);

	if (!IsValid(TargetEngine))return;
	State.EngineAngularVelocity = TargetEngine->GetAngularVelocity();
	State.EngineInertia = TargetEngine->NAConfig.EngineInertia;

	float EngineMaxTorque = TargetEngine->NAConfig.MaxEngineTorque;
	if (TargetEngine->TurboConfig.MaxBoostPressure > SMALL_NUMBER)
	{
		// if turbo charged
		EngineMaxTorque *= (1.f + TargetEngine->TurboConfig.MaxBoostPressure * TargetEngine->TurboConfig.BoostEfficiency);
	}
	State.MaxClutchTorque = EngineMaxTorque * Config.Capacity;
	
	State.ClutchLock = FMath::Clamp((float)(InCurrentGearRatio != 0) - InClutchValue, 0.f, 1.f);
	State.GearboxAngularVelocity = InGearboxInputShaftVelocity;
	State.ClutchSlip = State.EngineAngularVelocity - State.GearboxAngularVelocity;

	State.ReflectedInertia = InReflectedInertia;
	State.FirstGearReflectedInertia = InFirstGearReflectedInertia;

	State.TotalInertia = VehicleUtil::SafeDivide(State.EngineInertia * State.ReflectedInertia, State.EngineInertia + State.ReflectedInertia);

	switch (Config.SimMode)
	{
	case EClutchSimMode::SpringModel:State.ClutchTorque = GetTorqueSpringModel();
		break;
	case EClutchSimMode::DampingModel:State.ClutchTorque = GetTorqueDampingModel();
		break;
	default:State.ClutchTorque = GetTorqueDampingModel();
		break;
	}
}

float UVehicleClutchComponent::GetCluchTorque()
{
	return State.ClutchTorque;
}

float UVehicleClutchComponent::CalculateStiffness(float InFrequency, float InInertia)
{
	return InFrequency * InFrequency * InInertia;
}

float UVehicleClutchComponent::CalculateCriticalDamping(float InFrequency, float InInertia)
{
	return 2 * InFrequency * InInertia;
}
