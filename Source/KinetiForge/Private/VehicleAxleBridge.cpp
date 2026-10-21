// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehicleAxleBridge.h"
#include "Misc/ScopeLock.h"

FVehicleAxleBridge::FVehicleAxleBridge(FVehicleAxleRuntime InitialRuntime) : Runtime(MoveTemp(InitialRuntime))
{
	PublishResult();
}

void FVehicleAxleBridge::SubmitSettings(FVehicleAxleRuntimeSettings Settings)
{
	FScopeLock Lock(&TransferMutex);
	PendingSettings.Emplace(MoveTemp(Settings));
}

uint64 FVehicleAxleBridge::RequestP3MotorTorque(float Torque)
{
	FScopeLock Lock(&TransferMutex);
	PendingP3MotorTorque = Torque;
	return ++P3RequestSequence;
}

uint64 FVehicleAxleBridge::RequestWheelBase(float WheelBase)
{
	FScopeLock Lock(&TransferMutex);
	PendingWheelBase = WheelBase;
	return ++WheelBaseRequestSequence;
}

void FVehicleAxleBridge::ConsumeInputs()
{
	TOptional<FVehicleAxleRuntimeSettings> Settings;
	TOptional<float> P3MotorTorque, WheelBase;
	uint64 P3Sequence, WheelBaseSequence;
	{
		FScopeLock Lock(&TransferMutex);
		Settings = MoveTemp(PendingSettings);
		PendingSettings.Reset();
		P3MotorTorque = PendingP3MotorTorque;
		WheelBase = PendingWheelBase;
		PendingP3MotorTorque.Reset();
		PendingWheelBase.Reset();
		P3Sequence = P3RequestSequence;
		WheelBaseSequence = WheelBaseRequestSequence;
	}
	if (Settings.IsSet()) Runtime.Settings = MoveTemp(Settings.GetValue());
	if (P3MotorTorque.IsSet())
	{
		Runtime.State.P3MotorTorque = P3MotorTorque.GetValue();
		AppliedP3Sequence = P3Sequence;
	}
	if (WheelBase.IsSet())
	{
		Runtime.State.WheelBase = WheelBase.GetValue();
		AppliedWheelBaseSequence = WheelBaseSequence;
	}
}

void FVehicleAxleBridge::PublishResult()
{
	FVehicleAxleResult Result;
	Result.State = Runtime.State;
	Result.PublicationSequence = ++PublicationSequence;
	Result.AppliedP3Sequence = AppliedP3Sequence;
	Result.AppliedWheelBaseSequence = AppliedWheelBaseSequence;
	FScopeLock Lock(&TransferMutex);
	PublishedResult = MoveTemp(Result);
}

FVehicleAxleResult FVehicleAxleBridge::ReadResult() const
{
	FScopeLock Lock(&TransferMutex);
	return PublishedResult;
}
