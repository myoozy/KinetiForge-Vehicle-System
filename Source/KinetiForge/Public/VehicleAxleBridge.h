// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "HAL/CriticalSection.h"
#include "VehicleAxleStructs.h"
#include "VehicleDrivetrainStructs.h"

struct KINETIFORGE_API FVehicleAxleRuntimeSettings
{
	FVehicleAxleConfig AxleConfig;
	FVehicleAxleSteeringConfig SteeringConfig;
	FVehicleSteeringAssistConfig SteeringAssistConfig;
	FVehicleTCSConfig TCSConfig;
	EVehicleAxleLayout Layout = EVehicleAxleLayout::TwoWheels;
	EVehicleAxleSuspensionType SuspensionType = EVehicleAxleSuspensionType::Independent;
	FVehicleLimitedSlipDifferentialConfig DifferentialConfig;
	bool bHasDifferential = false;
};

struct KINETIFORGE_API FVehicleAxleRuntime
{
	FVehicleAxleRuntimeSettings Settings;
	FVehicleAxleSimState State;
};

struct KINETIFORGE_API FVehicleAxleResult
{
	FVehicleAxleSimState State;
	uint64 PublicationSequence = 0;
	uint64 AppliedP3Sequence = 0;
	uint64 AppliedWheelBaseSequence = 0;
};

/** One simulation consumer; GT submits settings/absolute commands and reads copies.
 * No component pointers, wheel discovery, physics scheduling or binding lifetimes. */
class KINETIFORGE_API FVehicleAxleBridge
{
public:
	explicit FVehicleAxleBridge(FVehicleAxleRuntime InitialRuntime);
	void SubmitSettings(FVehicleAxleRuntimeSettings Settings);
	uint64 RequestP3MotorTorque(float Torque);
	uint64 RequestWheelBase(float WheelBase);
	void ConsumeInputs();
	FVehicleAxleRuntime& GetRuntimeForSimulation() { return Runtime; }
	void PublishResult();
	FVehicleAxleResult ReadResult() const;
private:
	FVehicleAxleRuntime Runtime;
	mutable FCriticalSection TransferMutex;
	TOptional<FVehicleAxleRuntimeSettings> PendingSettings;
	TOptional<float> PendingP3MotorTorque, PendingWheelBase;
	uint64 P3RequestSequence = 0, WheelBaseRequestSequence = 0;
	uint64 AppliedP3Sequence = 0, AppliedWheelBaseSequence = 0, PublicationSequence = 0;
	FVehicleAxleResult PublishedResult;
};
