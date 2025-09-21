// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)


#include "VehicleDifferentialComponent.h"

// Sets default values for this component's properties
UVehicleDifferentialComponent::UVehicleDifferentialComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void UVehicleDifferentialComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UVehicleDifferentialComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UVehicleDifferentialComponent::UpdateOutputShaft(float InDriveTorque, float InLeftAngularVelocity, float InRightAngularVelocity, float InLeftTotalInertia, float InRightTotalInertia, float InDeltaTime, float InReflectedInertia, float& OutLeftTorque, float& OutRightTorque, float& OutReflectedInertiaEachWheel)
{
	//update torque
	if (Config.LockRatio > 0.f)
	{
		GetOutputTorque(
			InDriveTorque,
			InLeftAngularVelocity,
			InRightAngularVelocity,
			InLeftTotalInertia,
			InRightTotalInertia,
			InDeltaTime,
			OutLeftTorque,
			OutRightTorque
		);
	}
	else
	{
		GetOpenDiffOutputTorque(
			InDriveTorque,
			OutLeftTorque,
			OutRightTorque
		);
	}

	//update inertia
	OutReflectedInertiaEachWheel = 0.5 * InReflectedInertia * Config.GearRatio * Config.GearRatio;
}

void UVehicleDifferentialComponent::UpdateInputShaft(float InLeftOutputShaftAngularVelocity, float InRightOutputShaftAngularVelocity, float InLeftWheelInertia, float InRightWheelInertia, float& OutInputShaftVelocity, float& OutReflectedInertia)
{
	OutInputShaftVelocity = GetInputShaftVelocity(InLeftOutputShaftAngularVelocity, InRightOutputShaftAngularVelocity);

	OutReflectedInertia = (Config.GearRatio > SMALL_NUMBER) ? (InLeftWheelInertia + InRightWheelInertia) / (Config.GearRatio * Config.GearRatio) : 0.f;
}

void UVehicleDifferentialComponent::GetOutputTorque(
	float InTorque,
	float InLeftAngularVelocity,
	float InRightAngularVelocity, 
	float InLeftTotalInertia,
	float InRightTotalInertia,
	float InDeltaTime,
	float& OutLeftTorque,
	float& OutRightTorque)
{
	float OpenDiffTorque = 0.5 * Config.GearRatio * InTorque;

	//prevent divided by 0
	if (!InDeltaTime)
	{
		OutLeftTorque = OutRightTorque = OpenDiffTorque;
		return;
	}

	float AverageAngularVelocity = 0.5 * (InLeftAngularVelocity + InRightAngularVelocity);

	//calculate the torque required for both sides to match the average angular velocity
	float DeltaTimeInv = 1.f / InDeltaTime;
	float LeftTorqueBias = (AverageAngularVelocity - InLeftAngularVelocity) * InLeftTotalInertia * DeltaTimeInv * Config.LockRatio;
	float RightTorqueBias = (AverageAngularVelocity - InRightAngularVelocity) * InRightTotalInertia * DeltaTimeInv * Config.LockRatio;

	OutLeftTorque = OpenDiffTorque + LeftTorqueBias;
	OutRightTorque = OpenDiffTorque + RightTorqueBias;

}

void UVehicleDifferentialComponent::GetOpenDiffOutputTorque(float InTorque, float& OutTorqueLeft, float& OutTorqueRight)
{
	float OpenDiffTorque = 0.5 * Config.GearRatio * InTorque;
	OutTorqueLeft = OpenDiffTorque;
	OutTorqueRight = OpenDiffTorque;
}

float UVehicleDifferentialComponent::GetInputShaftVelocity(float OutputShaftAngularVelocityLeft, float OutputShaftAngularVelocityRight)
{
	return (OutputShaftAngularVelocityLeft + OutputShaftAngularVelocityRight) * 0.5 * Config.GearRatio;
}

