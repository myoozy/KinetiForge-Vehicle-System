// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)


#include "VehicleDifferentialComponent.h"
#include "VehicleAxleAssemblyComponent.h"
#include "VehicleWheelComponent.h"
#include "VehicleUtilities.h"

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

	OutReflectedInertia = UVehicleUtilities::SafeDivide(InLeftWheelInertia + InRightWheelInertia, Config.GearRatio * Config.GearRatio);
}

int32 UVehicleDifferentialComponent::SubstepTransferCase(
	TArrayView<UVehicleAxleAssemblyComponent* const> InAxles,
	float InSubstepDeltaTime,
	float InGearboxOutputTorque,
	float InBrakeValue,
	float InHandbrakeValue,
	bool bLineLockActive,
	float& OutTransmissionOutputShaftAngularVelocity,
	float& OutTransmissionOutputShaftEffectiveInertia,
	float& OutEffectiveDriveShaftStiffness)
{
	// 1. First iterate over all axles to gather Total Inertia, Total Angular Momentum, and Base Weights
	int32 NumOfDriveAxles = 0;
	float SumTorqueWeight = 0.f;

	float TotalDriveAxleInertia = 0.f;
	float TotalAngularMomentum = 0.f;

	for (UVehicleAxleAssemblyComponent* Axle : InAxles)
	{
		if (Axle == nullptr) continue;

		bool IsDriveAxle = Axle->GetAxleConfig().TorqueWeight > SMALL_NUMBER;
		if (IsDriveAxle)
		{
			NumOfDriveAxles++;
			SumTorqueWeight += FMath::Abs(Axle->GetAxleConfig().TorqueWeight);

			float AxleTotalInertia = Axle->GetTotalAxleInertia();
			float AxleAngVel = Axle->GetAngularVelocity();

			TotalDriveAxleInertia += AxleTotalInertia;
			TotalAngularMomentum += AxleTotalInertia * AxleAngVel;
		}
	}
	float FloatNumOfDriveAxles = (float)NumOfDriveAxles;

	// Calculate the physically correct locked angular velocity (momentum-weighted average)
	float TargetLockedAngVel = UVehicleUtilities::SafeDivide(TotalAngularMomentum, TotalDriveAxleInertia);

	// update axles
	float DriveTorque = Config.GearRatio * InGearboxOutputTorque;
	
	float SumAngVel = 0.f;
	float SumDriveAxleInertia = 0.f;
	float SumStiffness = 0.f;

	for (UVehicleAxleAssemblyComponent* Axle : InAxles)
	{
		if (Axle == nullptr) continue;

		float AxleInertia = 0.f;
		float AxleAngVel = 0.f;
		float AxleStiffness = 0.f;

		bool IsDriveAxle = Axle->GetAxleConfig().TorqueWeight > SMALL_NUMBER;
		if (IsDriveAxle)
		{
			// central diff locking logic
			// Use TargetLockedAngVel instead of simple arithmetic average
			float AngVelDifference = TargetLockedAngVel - Axle->GetAngularVelocity();

			bool bIsDrive = (DriveTorque * TargetLockedAngVel) >= 0.f;
			float CurrentLockRatio = bIsDrive ? Config.DriveLockRatio : Config.CoastLockRatio;

			// The calculated TorqueBias is now guaranteed to sum to exactly 0 across all axles
			float TorqueBias = UVehicleUtilities::SafeDivide(AngVelDifference * Axle->GetTotalAxleInertia() * CurrentLockRatio, InSubstepDeltaTime);
			float NormTorqueWeight = UVehicleUtilities::SafeDivide(Axle->GetAxleConfig().TorqueWeight, SumTorqueWeight);

			// Combine mechanical static split + LSD clutch pack transfer
			float AxleDriveTorque = DriveTorque * NormTorqueWeight + TorqueBias;

			// burnout assist
			bool IsMainDriveAxle = NormTorqueWeight > 0.5f;
			bool ShouldReleaseBrake = IsMainDriveAxle && bLineLockActive;

			Axle->SubstepAxle(
				InSubstepDeltaTime,
				AxleDriveTorque,
				InBrakeValue * !ShouldReleaseBrake,
				InHandbrakeValue,
				AxleInertia, AxleAngVel, AxleStiffness
			);

			SumAngVel += AxleAngVel;
			SumDriveAxleInertia += AxleInertia;
			SumStiffness += AxleStiffness;
		}
		else
		{
			Axle->SubstepAxle(
				InSubstepDeltaTime,
				0.f,
				InBrakeValue,
				InHandbrakeValue,
				AxleInertia, AxleAngVel, AxleStiffness
			);
		}
	}

	// For the output shaft, arithmetic mean is generally fine for RPM display/feedback, 
	// though TargetLockedAngVel * Config.GearRatio is also physically valid if fully locked.
	OutTransmissionOutputShaftAngularVelocity = UVehicleUtilities::SafeDivide(SumAngVel * Config.GearRatio, FloatNumOfDriveAxles);
	
	const float GearRatioSquareInv = UVehicleUtilities::SafeDivide(1.f, Config.GearRatio * Config.GearRatio);
	OutTransmissionOutputShaftEffectiveInertia = SumDriveAxleInertia * GearRatioSquareInv;
	OutEffectiveDriveShaftStiffness = SumStiffness * GearRatioSquareInv;

	return NumOfDriveAxles;
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
	float& OutTransmissionOutputShaftAngularVelocity,
	float& OutTransmissionOutputShaftEffectiveInertia,
	float& OutEffectiveDriveShaftStiffness)
{
	for (UVehicleAxleAssemblyComponent* Axle : InAxles)
	{
		if (IsValid(Axle))
		{
			Axle->PreStepAxle(InDeltaTime, InSteeringValue);
		}
	}

	int32 NumOfDriveAxles = SubstepTransferCase(
		InAxles, InDeltaTime, InGearboxOutputTorque,
		InBrakeValue, InHandbrakeValue, bLineLockActive,
		OutTransmissionOutputShaftAngularVelocity,
		OutTransmissionOutputShaftEffectiveInertia,
		OutEffectiveDriveShaftStiffness
	);

	for (UVehicleAxleAssemblyComponent* Axle : InAxles)
	{
		if (IsValid(Axle))
		{
			Axle->PostStepAxle();
		}
	}

	//return the number of drive axles
	return NumOfDriveAxles;
}

float UVehicleDifferentialComponent::CalculateEffectiveWheelRadius(const TArray<UVehicleAxleAssemblyComponent*>& InAxles)
{
	return CalculateEffectiveWheelRadius_Internal(InAxles);
}

float UVehicleDifferentialComponent::CalculateEffectiveWheelRadius_Internal(
	TArrayView<UVehicleAxleAssemblyComponent* const> InAxles)
{
	float effectiveR = 0.f;
	int32 driveAxleNum = 0;
	for (UVehicleAxleAssemblyComponent* Axle : InAxles)
	{
		if (!IsValid(Axle))continue;

		if (Axle->GetAxleConfig().TorqueWeight > 0)
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
				SumR += LeftWheel->GetWheelConfig().Radius;
				n++;
			}
			if (IsValid(RightWheel))
			{
				SumR += RightWheel->GetWheelConfig().Radius;
				n++;
			}
			float avgR = UVehicleUtilities::SafeDivide(SumR, (float)n);
			effectiveR += UVehicleUtilities::SafeDivide(avgR, Diff->Config.GearRatio);

			driveAxleNum++;
		}
	}
	effectiveR = UVehicleUtilities::SafeDivide(effectiveR, (float)driveAxleNum * Config.GearRatio);

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

	float AverageAngularVelocity = 0.5 * (InLeftAngularVelocity + InRightAngularVelocity);
	bool bIsDrive = (OpenDiffTorque * AverageAngularVelocity) >= 0.f;
	float CurrentLockRatio = bIsDrive ? Config.DriveLockRatio : Config.CoastLockRatio;

	//calculate the torque required for both sides to match the average angular velocity

	float ReducedInertia = UVehicleUtilities::SafeDivide(
		InLeftTotalInertia * InRightTotalInertia, InLeftTotalInertia + InRightTotalInertia);

	float OmegaDiff = InLeftAngularVelocity - InRightAngularVelocity;

	float MaxTransferTorque = UVehicleUtilities::SafeDivide(ReducedInertia * OmegaDiff, InDeltaTime);
	float TransferTorque = MaxTransferTorque * CurrentLockRatio;

	OutLeftTorque = OpenDiffTorque - TransferTorque;
	OutRightTorque = OpenDiffTorque + TransferTorque;

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