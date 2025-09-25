// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)


#include "VehicleDifferentialComponent.h"
#include "VehicleAxleAssemblyComponent.h"

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
	
	//update inertia
	OutReflectedInertiaEachWheel = 0.5 * InReflectedInertia * Config.GearRatio * Config.GearRatio;
}

void UVehicleDifferentialComponent::UpdateInputShaft(float InLeftOutputShaftAngularVelocity, float InRightOutputShaftAngularVelocity, float InLeftWheelInertia, float InRightWheelInertia, float& OutInputShaftVelocity, float& OutReflectedInertia)
{
	OutInputShaftVelocity = GetInputShaftVelocity(InLeftOutputShaftAngularVelocity, InRightOutputShaftAngularVelocity);

	OutReflectedInertia = SafeDivide(InLeftWheelInertia + InRightWheelInertia, Config.GearRatio * Config.GearRatio);
}

void UVehicleDifferentialComponent::UpdateTransferCase(
	const TArray<UVehicleAxleAssemblyComponent*> Axles, 
	float InDeltaTime,
	float InGearboxOutputTorque, 
	float InReflectedInertia,
	float InBrakeValue, 
	float InHandbrakeValue, 
	float InSteeringValue, 
	bool bLineLockActive,
	float& OutGearboxOutputShaftAngularVelocity,
	float& OutTotalInertia)
{
	//首先遍历所有车轴，获取驱动轴的数量、扭矩权重、和平均角速度
	//First iterate over all axles to obtain the number of drive axles, torque weights, and average angular velocity
	float NumOfDriveAxles = 0;
	float SumTorqueWeight = 0.f;
	float SumAngVel = 0.f;
	for (UVehicleAxleAssemblyComponent* TempAxle : Axles)
	{
		if (!IsValid(TempAxle))continue;

		bool IsDriveAxle = TempAxle->AxleConfig.TorqueWeight > 0;
		NumOfDriveAxles += IsDriveAxle;
		SumTorqueWeight += IsDriveAxle * TempAxle->AxleConfig.TorqueWeight;
		SumAngVel += IsDriveAxle * TempAxle->GetAngularVelocity();
	}
	float AverageAxleAngularVelocity = SafeDivide(SumAngVel, NumOfDriveAxles);

	//update axles
	float DriveTorque = Config.GearRatio * InGearboxOutputTorque;
	float ReflInertia = Config.GearRatio * Config.GearRatio * InReflectedInertia;
	//reflected inertia
	float ReflectedInertiaOnAxle = SafeDivide(ReflInertia, NumOfDriveAxles);

	SumAngVel = 0.f;
	float SumDriveAxleInertia = 0.f;
	for (UVehicleAxleAssemblyComponent* TempAxle : Axles)
	{
		if (!IsValid(TempAxle))continue;
		float AxleInertia = 0.f;
		float AxleAngVel = 0.f;

		bool IsDriveAxle = TempAxle->AxleConfig.TorqueWeight > 0;
		if (IsDriveAxle)
		{
			//central diff
			float AngVelDifference = AverageAxleAngularVelocity - TempAxle->GetAngularVelocity();
			float TorqueBias = SafeDivide(AngVelDifference * TempAxle->GetTotalAxleInertia() * Config.LockRatio, InDeltaTime);
			float NormTorqueWeight = SafeDivide(TempAxle->AxleConfig.TorqueWeight, SumTorqueWeight);
			float AxleDriveTorque = DriveTorque * NormTorqueWeight + TorqueBias;

			//burnout assist
			bool IsMainDriveAxle = NormTorqueWeight > 0.5;
			bool ShouldReleaseBrake = IsMainDriveAxle && bLineLockActive;

			TempAxle->UpdatePhysics(
				InDeltaTime,
				AxleDriveTorque,
				InBrakeValue * !ShouldReleaseBrake,
				InHandbrakeValue,
				InSteeringValue,
				ReflectedInertiaOnAxle,
				AxleInertia, AxleAngVel);

			SumAngVel += AxleAngVel;
			SumDriveAxleInertia += AxleInertia;
		}
		else
		{
			TempAxle->UpdatePhysics(
				InDeltaTime,
				0.f,
				InBrakeValue,
				InHandbrakeValue,
				InSteeringValue,
				0.f,
				AxleInertia,
				AxleAngVel);
		}
	}

	//get average angular velocity of all drive axles
	OutGearboxOutputShaftAngularVelocity = SafeDivide(SumAngVel * Config.GearRatio, NumOfDriveAxles);
	OutTotalInertia = SafeDivide(SumDriveAxleInertia, Config.GearRatio * Config.GearRatio);
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

float UVehicleDifferentialComponent::SafeDivide(float a, float b)
{
	return (FMath::IsNearlyZero(b)) ? 0.0f : a / b;
}