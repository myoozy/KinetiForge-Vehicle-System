// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)


#include "VehicleDifferentialComponent.h"
#include "VehicleAxleAssemblyComponent.h"
#include "VehicleWheelComponent.h"
#include "VehicleUtil.h"

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

	OutReflectedInertia = VehicleUtil::SafeDivide(InLeftWheelInertia + InRightWheelInertia, Config.GearRatio * Config.GearRatio);
}

int32 UVehicleDifferentialComponent::UpdateTransferCase(
	const TArray<UVehicleAxleAssemblyComponent*>& InAxles, 
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
	int32 NumOfDriveAxles = 0;
	float SumTorqueWeight = 0.f;
	float SumAngVel = 0.f;
	for (UVehicleAxleAssemblyComponent* Axle : InAxles)
	{
		if (!IsValid(Axle))continue;

		bool IsDriveAxle = Axle->AxleConfig.TorqueWeight > 0;
		NumOfDriveAxles += IsDriveAxle;
		SumTorqueWeight += IsDriveAxle * Axle->AxleConfig.TorqueWeight;
		SumAngVel += IsDriveAxle * Axle->GetAngularVelocity();
	}
	float FloatNumOfDriveAxles = (float)NumOfDriveAxles;
	float AverageAxleAngularVelocity = VehicleUtil::SafeDivide(SumAngVel, FloatNumOfDriveAxles);

	//update axles
	float DriveTorque = Config.GearRatio * InGearboxOutputTorque;
	float ReflInertia = Config.GearRatio * Config.GearRatio * InReflectedInertia;
	//reflected inertia
	float ReflectedInertiaOnAxle = VehicleUtil::SafeDivide(ReflInertia, FloatNumOfDriveAxles);

	SumAngVel = 0.f;
	float SumDriveAxleInertia = 0.f;
	for (UVehicleAxleAssemblyComponent* Axle : InAxles)
	{
		if (!IsValid(Axle))continue;
		float AxleInertia = 0.f;
		float AxleAngVel = 0.f;

		bool IsDriveAxle = Axle->AxleConfig.TorqueWeight > 0;
		if (IsDriveAxle)
		{
			//central diff
			float AngVelDifference = AverageAxleAngularVelocity - Axle->GetAngularVelocity();
			float TorqueBias = VehicleUtil::SafeDivide(AngVelDifference * Axle->GetTotalAxleInertia() * Config.LockRatio, InDeltaTime);
			float NormTorqueWeight = VehicleUtil::SafeDivide(Axle->AxleConfig.TorqueWeight, SumTorqueWeight);
			float AxleDriveTorque = DriveTorque * NormTorqueWeight + TorqueBias;

			//burnout assist
			bool IsMainDriveAxle = NormTorqueWeight > 0.5;
			bool ShouldReleaseBrake = IsMainDriveAxle && bLineLockActive;

			Axle->UpdatePhysics(
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
			Axle->UpdatePhysics(
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
	OutGearboxOutputShaftAngularVelocity = VehicleUtil::SafeDivide(SumAngVel * Config.GearRatio, FloatNumOfDriveAxles);
	OutTotalInertia = VehicleUtil::SafeDivide(SumDriveAxleInertia, Config.GearRatio * Config.GearRatio);

	//return the number of drive axles
	return NumOfDriveAxles;
}

float UVehicleDifferentialComponent::CalculateEffectiveWheelRadius(
	const TArray<UVehicleAxleAssemblyComponent*>& InAxles)
{
	float effectiveR = 0.f;
	int32 driveAxleNum = 0;
	for (UVehicleAxleAssemblyComponent* Axle : InAxles)
	{
		if (!IsValid(Axle))continue;

		if (Axle->AxleConfig.TorqueWeight > 0)
		{
			UVehicleWheelComponent* LeftWheel;
			UVehicleWheelComponent* RightWheel;
			UVehicleDifferentialComponent* Diff;

			Axle->GetWheels(LeftWheel, RightWheel);
			Axle->GetDifferential(Diff);

			float SumR = 0.f;
			int32 n = 0;
			if (IsValid(LeftWheel))
			{
				SumR += LeftWheel->WheelConfig.Radius;
				n++;
			}
			if (IsValid(RightWheel))
			{
				SumR += RightWheel->WheelConfig.Radius;
				n++;
			}
			float avgR = VehicleUtil::SafeDivide(SumR, (float)n);
			effectiveR += VehicleUtil::SafeDivide(avgR, Diff->Config.GearRatio);

			driveAxleNum++;
		}
	}
	effectiveR = VehicleUtil::SafeDivide(effectiveR, (float)driveAxleNum * Config.GearRatio);

	return effectiveR;
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