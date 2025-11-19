// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)


#include "VehicleClutchComponent.h"
#include "VehicleWheelCoordinatorComponent.h"
#include "VehicleEngineComponent.h"

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
	float FirstGearTotalInertia = SafeDivide(SimData.EngineInertia * SimData.FirstGearReflectedInertia, SimData.EngineInertia + SimData.FirstGearReflectedInertia);
	SimData.SpringStiffness = CalculateStiffness(Config.NaturalFrequency, FirstGearTotalInertia);
	SimData.CriticalDamping = CalculateCriticalDamping(Config.NaturalFrequency, FirstGearTotalInertia);
	SimData.SpringDamping = SimData.CriticalDamping * Config.Damping;
}

float UVehicleClutchComponent::GetTorqueSpringModel()
{
	UpdateSpringStiffness();
	
	float ClutchSlipScaled = SimData.ClutchSlip * SimData.ClutchLock;//ClutchSlip * ClutchLock
	float CurrentAngleDiff = SimData.AngleDiff;

	float DontKnowWhatItIs = SimData.SpringStiffness * SimData.PhysicsDeltaTime + SimData.SpringDamping;
	float TorqueNumerator = SimData.SpringStiffness * CurrentAngleDiff + DontKnowWhatItIs * ClutchSlipScaled;
	float TorqueDenominator = 1.0f + SafeDivide(DontKnowWhatItIs * SimData.PhysicsDeltaTime, SimData.TotalInertia);

	float SpringModelTorque = TorqueNumerator / TorqueDenominator;

	float DampingModelTorque = SimData.ClutchLock * FMath::Clamp(SimData.CriticalDamping * SimData.ClutchSlip, -SimData.MaxClutchTorque, SimData.MaxClutchTorque);
	
	if (FMath::Abs(SpringModelTorque) > SimData.MaxClutchTorque)
	{
		SpringModelTorque = FMath::Sign(SpringModelTorque) * SimData.MaxClutchTorque;
	}
	else
	{
		SimData.AngleDiff = SimData.ClutchLock * (SimData.AngleDiff + ClutchSlipScaled * SimData.PhysicsDeltaTime);
	}

	return FMath::Lerp(DampingModelTorque, SpringModelTorque, SimData.ClutchLock);
}

float UVehicleClutchComponent::GetTorqueDampingModel()
{
	SimData.AngleDiff = 0;

	SimData.CriticalDamping = 2 * SimData.TotalInertia * SimData.MaxNaturalFrequency;

	float UnSmoothenedTorque = FMath::Clamp(SimData.ClutchSlip * SimData.CriticalDamping, -SimData.MaxClutchTorque, SimData.MaxClutchTorque);
	SimData.SmoothenedTorque += FMath::Clamp(1 - Config.Damping, 0, 1) * (UnSmoothenedTorque - SimData.SmoothenedTorque);
	return SimData.SmoothenedTorque * SimData.ClutchLock;
}

// Called every frame
void UVehicleClutchComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UVehicleClutchComponent::UpdatePhysics(float InDeltaTime, float InClutchValue, float InGearboxInputShaftVelocity, float InReflectedInertia, float InCurrentGearRatio, float InFirstGearReflectedInertia, UVehicleEngineComponent* TargetEngine)
{
	SimData.PhysicsDeltaTime = InDeltaTime;
	InClutchValue = FMath::Clamp(InClutchValue, 0.f, 1.f);

	if (!IsValid(TargetEngine))return;
	SimData.EngineAngularVelocity = TargetEngine->GetAngularVelocity();
	SimData.EngineInertia = TargetEngine->NAConfig.EngineInertia;

	float EngineMaxTorque = TargetEngine->NAConfig.MaxEngineTorque;
	if (TargetEngine->TurboConfig.MaxBoostPressure > SMALL_NUMBER)
	{
		// if turbo charged
		EngineMaxTorque *= (1.f + TargetEngine->TurboConfig.MaxBoostPressure * TargetEngine->TurboConfig.BoostEfficiency);
	}
	SimData.MaxClutchTorque = EngineMaxTorque * Config.Capacity;
	
	SimData.ClutchLock = FMath::Clamp((float)(InCurrentGearRatio != 0) - InClutchValue, 0.f, 1.f);
	SimData.GearboxAngularVelocity = InGearboxInputShaftVelocity;
	SimData.ClutchSlip = SimData.EngineAngularVelocity - SimData.GearboxAngularVelocity;

	SimData.ReflectedInertia = InReflectedInertia;
	SimData.FirstGearReflectedInertia = InFirstGearReflectedInertia;

	SimData.MaxNaturalFrequency = SafeDivide(1, SimData.PhysicsDeltaTime);
	SimData.TotalInertia = SafeDivide(SimData.EngineInertia * SimData.ReflectedInertia, SimData.EngineInertia + SimData.ReflectedInertia);

	switch (Config.SimMode)
	{
	case EClutchSimMode::SpringModel:SimData.ClutchTorque = GetTorqueSpringModel();
		break;
	case EClutchSimMode::DampingModel:SimData.ClutchTorque = GetTorqueDampingModel();
		break;
	default:SimData.ClutchTorque = GetTorqueDampingModel();
		break;
	}
}

float UVehicleClutchComponent::GetCluchTorque()
{
	return SimData.ClutchTorque;
}

float UVehicleClutchComponent::SafeDivide(float a, float b)
{
	return (FMath::IsNearlyZero(b)) ? 0.0f : a / b;
}

float UVehicleClutchComponent::CalculateStiffness(float InFrequency, float InInertia)
{
	return InFrequency * InFrequency * InInertia;
}

float UVehicleClutchComponent::CalculateCriticalDamping(float InFrequency, float InInertia)
{
	return 2 * InFrequency * InInertia;
}
