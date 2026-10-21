// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)


#include "VehicleDifferentialComponent.h"
#include "VehicleAxleAssemblyComponent.h"
#include "VehicleDriveAssemblyComponent.h"
#include "VehicleWheelComponent.h"
#include "VehicleUtilities.h"
#include "VehicleDifferentialSolver.h"

void UVehicleDifferentialComponent::AddConfigurationConsumer(UActorComponent* Consumer)
{
	check(IsInGameThread());
	ConfigurationConsumers.RemoveAll([](const auto& Entry) { return !Entry.IsValid(); });
	ConfigurationConsumers.AddUnique(Consumer);
}

void UVehicleDifferentialComponent::SetConfig(const FVehicleLimitedSlipDifferentialConfig& NewConfig)
{
	check(IsInGameThread());
	Config = NewConfig;
	// Submit through each consumer's current binding, so an old subscription cannot restore a replaced diff.
	// Copy first: submission also refreshes subscriptions and may prune the original list.
	const auto Consumers = ConfigurationConsumers;
	for (const auto& Consumer : Consumers)
	{
		if (auto* Axle = Cast<UVehicleAxleAssemblyComponent>(Consumer.Get())) Axle->SubmitRuntimeSettings();
		else if (auto* Drive = Cast<UVehicleDriveAssemblyComponent>(Consumer.Get())) Drive->SubmitTransferCaseRuntimeSettings();
	}
}

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
	InvalidatePhysicsDependents();
	Super::BeginPlay();

	// ...
	
}

void UVehicleDifferentialComponent::AddPhysicsDependent(const TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe>& Step)
{
	check(IsInGameThread());
	PhysicsDependents.RemoveAll([](const auto& Entry)
	{
		const auto Existing = Entry.Pin();
		return !Existing || Existing->IsStopped();
	});
	for (const auto& Entry : PhysicsDependents) if (Entry.Pin() == Step) return;
	PhysicsDependents.Add(Step);
}

void UVehicleDifferentialComponent::InvalidatePhysicsDependents()
{
	check(IsInGameThread());
	for (const auto& Entry : PhysicsDependents)
	{
		if (const auto Step = Entry.Pin()) Step->InvalidateBindingAndWait();
	}
}

void UVehicleDifferentialComponent::OnRegister()
{
	InvalidatePhysicsDependents();
	Super::OnRegister();
}

void UVehicleDifferentialComponent::OnUnregister()
{
	InvalidatePhysicsDependents();
	Super::OnUnregister();
}

void UVehicleDifferentialComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	InvalidatePhysicsDependents();
	Super::EndPlay(EndPlayReason);
}

void UVehicleDifferentialComponent::BeginDestroy()
{
	InvalidatePhysicsDependents();
	Super::BeginDestroy();
}

void UVehicleDifferentialComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	InvalidatePhysicsDependents();
	Super::OnComponentDestroyed(bDestroyingHierarchy);
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
	OutReflectedInertiaEachWheel = FVehicleDifferentialSolver::CalculateReflectedOutputInertiaPerWheel(
		InReflectedInertia, Config.GearRatio);
}

void UVehicleDifferentialComponent::UpdateInputShaft(float InLeftOutputShaftAngularVelocity, float InRightOutputShaftAngularVelocity, float InLeftWheelInertia, float InRightWheelInertia, float& OutInputShaftVelocity, float& OutReflectedInertia)
{
	OutInputShaftVelocity = GetInputShaftVelocity(InLeftOutputShaftAngularVelocity, InRightOutputShaftAngularVelocity);

	OutReflectedInertia = FVehicleDifferentialSolver::CalculateReflectedInputInertia(
		InLeftWheelInertia, InRightWheelInertia, Config.GearRatio);
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
	return SubstepTransferCaseWithConfig(Config, InAxles, InSubstepDeltaTime, InGearboxOutputTorque,
		InBrakeValue, InHandbrakeValue, bLineLockActive, OutTransmissionOutputShaftAngularVelocity,
		OutTransmissionOutputShaftEffectiveInertia, OutEffectiveDriveShaftStiffness);
}

int32 UVehicleDifferentialComponent::SubstepTransferCaseWithConfig(
	const FVehicleLimitedSlipDifferentialConfig& Config,
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

		bool IsDriveAxle = Axle->GetSimulationAxleConfig().TorqueWeight > SMALL_NUMBER;
		if (IsDriveAxle)
		{
			NumOfDriveAxles++;
			SumTorqueWeight += FMath::Abs(Axle->GetSimulationAxleConfig().TorqueWeight);

			float AxleTotalInertia = Axle->GetSimulationState().AxleEffectiveInertia;
			float AxleAngVel = Axle->GetSimulationState().AxleAngularVelocity;

			TotalDriveAxleInertia += AxleTotalInertia;
			TotalAngularMomentum += AxleTotalInertia * AxleAngVel;
		}
	}
	float FloatNumOfDriveAxles = (float)NumOfDriveAxles;

	// Calculate the physically correct locked angular velocity (momentum-weighted average)
	float TargetLockedAngVel = FVehicleDifferentialSolver::CalculateLockedAngularVelocity(
		TotalAngularMomentum, TotalDriveAxleInertia);

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

		bool IsDriveAxle = Axle->GetSimulationAxleConfig().TorqueWeight > SMALL_NUMBER;
		if (IsDriveAxle)
		{
			// central diff locking logic
			// Use TargetLockedAngVel instead of simple arithmetic average
			// The calculated TorqueBias is now guaranteed to sum to exactly 0 across all axles
			// Analytic balance is retained; floating-point sums may have roundoff.
			// Combine mechanical static split + LSD clutch pack transfer
			const FVehicleTransferCaseAxleOutput AxleOutput = FVehicleDifferentialSolver::CalculateTransferCaseAxleOutput(
				Config, DriveTorque, TargetLockedAngVel, Axle->GetSimulationState().AxleAngularVelocity,
				Axle->GetSimulationState().AxleEffectiveInertia, Axle->GetSimulationAxleConfig().TorqueWeight, SumTorqueWeight, InSubstepDeltaTime);

			// burnout assist
			bool IsMainDriveAxle = AxleOutput.NormalizedTorqueWeight > 0.5f;
			bool ShouldReleaseBrake = IsMainDriveAxle && bLineLockActive;

			Axle->SubstepAxle(
				InSubstepDeltaTime,
				AxleOutput.DriveTorque,
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
	//calculate the torque required for both sides to match the average angular velocity
	FVehicleDifferentialStepInput Input;
	Input.InputTorque = InTorque;
	Input.LeftAngularVelocity = InLeftAngularVelocity;
	Input.RightAngularVelocity = InRightAngularVelocity;
	Input.LeftTotalInertia = InLeftTotalInertia;
	Input.RightTotalInertia = InRightTotalInertia;
	Input.DeltaTime = InDeltaTime;
	const FVehicleDifferentialTorqueOutput Output = FVehicleDifferentialSolver::CalculateOutputTorques(Config, Input);
	OutLeftTorque = Output.LeftTorque;
	OutRightTorque = Output.RightTorque;

}

void UVehicleDifferentialComponent::GetOpenDiffOutputTorque(float InTorque, float& OutTorqueLeft, float& OutTorqueRight)
{
	float OpenDiffTorque = FVehicleDifferentialSolver::CalculateOpenOutputTorque(InTorque, Config.GearRatio);
	OutTorqueLeft = OpenDiffTorque;
	OutTorqueRight = OpenDiffTorque;
}

float UVehicleDifferentialComponent::GetInputShaftVelocity(float OutputShaftAngularVelocityLeft, float OutputShaftAngularVelocityRight)
{
	return FVehicleDifferentialSolver::CalculateInputAngularVelocity(
		OutputShaftAngularVelocityLeft, OutputShaftAngularVelocityRight, Config.GearRatio);
}
