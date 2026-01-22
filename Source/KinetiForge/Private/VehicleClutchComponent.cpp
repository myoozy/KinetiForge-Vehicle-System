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


float UVehicleClutchComponent::GetTorqueSpringModel(
	float DeltaTime,
	float ClutchSlip,
	float EngineInertia,
	float GearboxReflectedInertia,
	float GearboxInputShaftInertia,
	float GearboxReflectedInertia_HighestGear)
{
	float J_Gearbox = GearboxReflectedInertia + GearboxInputShaftInertia;
	float J_Engine = EngineInertia;
	float J_Total = VehicleUtil::SafeDivide(J_Gearbox * J_Engine, J_Gearbox + J_Engine);
	float J_ToGetStiffness = 0.f;

	// to simulate torson spring on drive shaft
	J_Gearbox = GearboxReflectedInertia;
	J_Engine = EngineInertia + GearboxInputShaftInertia;
	J_ToGetStiffness = VehicleUtil::SafeDivide(J_Gearbox * J_Engine, J_Gearbox + J_Engine);
	float K_Shaft = CalculateStiffness(Config.NaturalFrequency, J_ToGetStiffness);

	// to simulate torson spring on clutch
	J_Gearbox = GearboxInputShaftInertia + GearboxReflectedInertia_HighestGear;
	J_Engine = EngineInertia;
	J_ToGetStiffness = VehicleUtil::SafeDivide(J_Gearbox * J_Engine, J_Gearbox + J_Engine);
	float K_Clutch = CalculateStiffness(Config.NaturalFrequency, J_ToGetStiffness);

	float SpringStiffness = VehicleUtil::SafeDivide(K_Shaft * K_Clutch, K_Shaft + K_Clutch);
	float CriticalDamping = 2.0f * FMath::Sqrt(SpringStiffness * J_Total);
	float SpringDamping = CriticalDamping * Config.Damping;
	
	float ClutchSlipScaled = ClutchSlip * State.ClutchLock;//ClutchSlip * ClutchLock
	float CurrentAngleDiff = State.AngleDiff;

	float DontKnowWhatItIs = SpringStiffness * DeltaTime + SpringDamping;
	float TorqueNumerator = SpringStiffness * CurrentAngleDiff + DontKnowWhatItIs * ClutchSlipScaled;
	float TorqueDenominator = 1.0f + VehicleUtil::SafeDivide(DontKnowWhatItIs * DeltaTime, J_Total);

	float SpringModelTorque = TorqueNumerator / TorqueDenominator;

	float DampingModelTorque = State.ClutchLock * FMath::Clamp(CriticalDamping * ClutchSlip, -State.MaxClutchTorque, State.MaxClutchTorque);
	
	if (FMath::Abs(SpringModelTorque) > State.MaxClutchTorque)
	{
		SpringModelTorque = FMath::Sign(SpringModelTorque) * State.MaxClutchTorque;
	}
	else
	{
		State.AngleDiff = State.ClutchLock * (State.AngleDiff + ClutchSlipScaled * DeltaTime);
	}

	return State.ClutchTorque = FMath::Lerp(DampingModelTorque, SpringModelTorque, State.ClutchLock);
}

float UVehicleClutchComponent::GetTorqueDampingModel(
	float DeltaTime,
	float ClutchSlip,
	float EngineInertia,
	float GearboxReflectedInertia,
	float GearboxInputShaftInertia)
{
	State.AngleDiff = 0.f;

	float J_Gearbox = GearboxReflectedInertia + GearboxInputShaftInertia;
	float J_Engine = EngineInertia;

	float J = VehicleUtil::SafeDivide(J_Gearbox * J_Engine, J_Gearbox + J_Engine);
	float GameFrequency = VehicleUtil::SafeDivide(1.f, DeltaTime);
	float CriticalDamping = 2.f * J * GameFrequency;

	float UnSmoothenedTorque = FMath::Clamp(ClutchSlip * CriticalDamping, -State.MaxClutchTorque, State.MaxClutchTorque);
	UnSmoothenedTorque *= State.ClutchLock;
	return State.ClutchTorque += FMath::Clamp(1 - Config.Damping, 0, 1) * (UnSmoothenedTorque - State.ClutchTorque);
}

// Called every frame
void UVehicleClutchComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UVehicleClutchComponent::UpdatePhysics(
	float InDeltaTime, 
	float InClutchValue, 
	float InGearboxInputShaftVelocity, 
	float InGearboxReflectedInertia, 
	float InGearboxInputShaftInertia, 
	float InCurrentGearRatio, 
	float GearboxReflectedInertia_HighestGear,
	UVehicleEngineComponent* TargetEngine)
{
	InClutchValue = FMath::Clamp(InClutchValue, 0.f, 1.f);

	// get some data from engine
	if (!IsValid(TargetEngine))return;
	const float EngineAngularVelocity = TargetEngine->GetAngularVelocity();
	const float EngineInertia = TargetEngine->NAConfig.EngineInertia;

	float EngineMaxTorque = TargetEngine->NAConfig.MaxEngineTorque;
	if (TargetEngine->TurboConfig.MaxBoostPressure > SMALL_NUMBER)
	{
		// if turbo charged
		EngineMaxTorque *= (1.f + TargetEngine->TurboConfig.MaxBoostPressure * TargetEngine->TurboConfig.BoostEfficiency);
	}
	State.MaxClutchTorque = EngineMaxTorque * Config.Capacity;

	State.ClutchLock = FMath::Clamp((float)(InCurrentGearRatio != 0) - InClutchValue, 0.f, 1.f);
	const float& GearboxAngularVelocity = InGearboxInputShaftVelocity;
	const float& ClutchSlip = EngineAngularVelocity - GearboxAngularVelocity;

	switch (Config.SimMode)
	{
	default:
	case EClutchSimMode::SpringModel:
		GetTorqueSpringModel(
			InDeltaTime,
			ClutchSlip,
			EngineInertia,
			InGearboxReflectedInertia,
			InGearboxInputShaftInertia,
			GearboxReflectedInertia_HighestGear);
		break;
	case EClutchSimMode::DampingModel:
		GetTorqueDampingModel(
			InDeltaTime,
			ClutchSlip,
			EngineInertia,
			InGearboxReflectedInertia,
			InGearboxInputShaftInertia
		);
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
