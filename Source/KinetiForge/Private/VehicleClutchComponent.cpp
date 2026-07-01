// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)


#include "VehicleClutchComponent.h"
#include "VehicleWheelCoordinatorComponent.h"
#include "VehicleEngineComponent.h"
#include "VehicleUtilities.h"

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
	const float DeltaTime,
	const float ClutchSlip,
	const float EngineInertia,
	const float GearboxReflectedInertia,
	const float DriveShaftStiffness)
{
	float J_Gearbox = GearboxReflectedInertia;
	float J_Engine = EngineInertia;
	float J_Total = UVehicleUtilities::SafeDivide(J_Gearbox * J_Engine, J_Gearbox + J_Engine);

	// to simulate torson spring on drive shaft
	float K_Shaft = DriveShaftStiffness;

	// to simulate torson spring on clutch
	float K_Clutch = Config.TorsionalStiffness;

	float K_Series = UVehicleUtilities::SafeDivide(K_Shaft * K_Clutch, K_Shaft + K_Clutch);

	float DampingRatio = Config.DampingRatio;
	float CriticalDamping = 2.0f * FMath::Sqrt(K_Series * J_Total);
	float D_Total = CriticalDamping * DampingRatio;

	float ClutchSlipScaled = ClutchSlip * State.ClutchLock;//ClutchSlip * ClutchLock
	float CurrentAngleDiff = State.AngleDiff;

	float DontKnowWhatItIs = K_Series * DeltaTime + D_Total;
	float TorqueNumerator = K_Series * CurrentAngleDiff + DontKnowWhatItIs * ClutchSlipScaled;
	float TorqueDenominator = 1.0f + UVehicleUtilities::SafeDivide(DontKnowWhatItIs * DeltaTime, J_Total);

	float SpringModelTorque = TorqueNumerator / TorqueDenominator;

	float CurrentCapacity = State.MaxClutchTorque * State.ClutchLock;

	if (FMath::Abs(SpringModelTorque) > CurrentCapacity)
	{
		SpringModelTorque = FMath::Sign(SpringModelTorque) * CurrentCapacity;
	}
	else
	{
		State.AngleDiff = State.ClutchLock * (State.AngleDiff + ClutchSlipScaled * DeltaTime);
	}

	return State.ClutchTorque = SpringModelTorque;
}

float UVehicleClutchComponent::GetTorqueDampingModel(
	const float DeltaTime,
	const float ClutchSlip,
	const float EngineInertia,
	const float GearboxReflectedInertia)
{
	State.AngleDiff = 0.f;

	float J_Gearbox = GearboxReflectedInertia;
	float J_Engine = EngineInertia;
	float J_Total = UVehicleUtilities::SafeDivide(J_Gearbox * J_Engine, J_Gearbox + J_Engine);

	float D_Clutch = Config.ViscousDamping;

	float TorqueNumerator = D_Clutch * ClutchSlip;
	float TorqueDenominator = 1.0f + UVehicleUtilities::SafeDivide(D_Clutch * DeltaTime, J_Total);

	float DampingModelTorque = TorqueNumerator / TorqueDenominator;
	float CurrentCapacity = State.MaxClutchTorque * State.ClutchLock;
	return State.ClutchTorque = FMath::Clamp(DampingModelTorque, -CurrentCapacity, CurrentCapacity);
}

float UVehicleClutchComponent::GetTorqueConstraintModel(
	const float DeltaTime,
	const float ClutchSlip,
	const float EngineInertia,
	const float GearboxReflectedInertia)
{
	float J_Gearbox = GearboxReflectedInertia;
	float J_Engine = EngineInertia;
	float J_Effective = UVehicleUtilities::SafeDivide(J_Gearbox * J_Engine, J_Gearbox + J_Engine);

	float ExactLockTorque = (ClutchSlip * J_Effective) / DeltaTime;
	float CurrentCapacity = State.MaxClutchTorque * State.ClutchLock;
	return State.ClutchTorque = FMath::Clamp(ExactLockTorque, -CurrentCapacity, CurrentCapacity);
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

	float ClutchValue = FMath::Clamp(InClutchValue, 0.f, 1.f);

	// get some data from engine
	const float& EngineAngularVelocity = InEngineAngularVelocity;
	const float& EngineInertia = InEngineInertia;
	const float& EngineMaxTorque = InEngineMaxTorque;

	State.MaxClutchTorque = EngineMaxTorque * Config.Capacity;

	State.ClutchLock = FMath::Clamp((float)(InCurrentGearRatio != 0) - ClutchValue, 0.f, 1.f);
	const float& GearboxAngularVelocity = InGearboxInputShaftVelocity;
	const float ClutchSlip = EngineAngularVelocity - GearboxAngularVelocity;

	switch (Config.SimMode)
	{
	default:
	case EClutchSimMode::ConstraintLock:
		GetTorqueConstraintModel(
			InDeltaTime,
			ClutchSlip,
			EngineInertia,
			InGearboxReflectedInertia
		);
		break;
	case EClutchSimMode::FrictionClutch:
		GetTorqueSpringModel(
			InDeltaTime,
			ClutchSlip,
			EngineInertia,
			InGearboxReflectedInertia,
			InDriveShaftStiffness
		);
		break;
	case EClutchSimMode::FluidCoupling:
		GetTorqueDampingModel(
			InDeltaTime,
			ClutchSlip,
			EngineInertia,
			InGearboxReflectedInertia
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
