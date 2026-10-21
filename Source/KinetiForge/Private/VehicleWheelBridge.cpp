// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehicleWheelBridge.h"
#include "Misc/ScopeLock.h"

FVehicleWheelBridge::FVehicleWheelBridge(FVehicleWheelRuntime InitialRuntime)
	: Runtime(MoveTemp(InitialRuntime))
{
	PreviousSuspensionState = Runtime.SuspensionState;
	PublishResult();
}

void FVehicleWheelBridge::SubmitSettings(FVehicleWheelRuntimeSettings Settings)
{
	FScopeLock Lock(&TransferMutex);
	PendingSettings.Emplace(MoveTemp(Settings));
}

void FVehicleWheelBridge::SubmitEnvironment(FVehicleWheelEnvironment NewEnvironment)
{
	FScopeLock Lock(&TransferMutex);
	PendingEnvironment.Emplace(MoveTemp(NewEnvironment));
}

uint64 FVehicleWheelBridge::RequestSuspensionReset(const FVehicleSuspensionSimState& State)
{
	FScopeLock Lock(&TransferMutex);
	PendingSuspensionReset = State;
	PendingSprungMass.Reset(); // A later mass request may override the reset, just as on the legacy path.
	PendingSuspensionResetSequence = ++SuspensionResetSequence;
	ResetSupersedesMassSequence = MassRequestSequence;
	return PendingSuspensionResetSequence;
}

uint64 FVehicleWheelBridge::RequestP4MotorTorque(float Torque)
{
	FScopeLock Lock(&TransferMutex);
	PendingP4MotorTorque = Torque;
	return ++P4RequestSequence;
}

uint64 FVehicleWheelBridge::RequestEspBrakeTorque(float Torque)
{
	FScopeLock Lock(&TransferMutex);
	PendingEspBrakeTorque = Torque;
	return ++EspRequestSequence;
}

uint64 FVehicleWheelBridge::RequestSprungMass(float Mass)
{
	FScopeLock Lock(&TransferMutex);
	PendingSprungMass = Mass;
	return ++MassRequestSequence;
}

void FVehicleWheelBridge::ConsumeInputs()
{
	TOptional<FVehicleWheelRuntimeSettings> Settings;
	TOptional<FVehicleWheelEnvironment> NewEnvironment;
	TOptional<FVehicleSuspensionSimState> SuspensionReset;
	uint64 ResetSequence = 0;
	uint64 P4Sequence = 0, EspSequence = 0, MassSequence = 0, SupersededMassSequence = 0;
	TOptional<float> P4MotorTorque, EspBrakeTorque, SprungMass;
	{
		FScopeLock Lock(&TransferMutex);
		Settings = MoveTemp(PendingSettings);
		PendingSettings.Reset();
		NewEnvironment = MoveTemp(PendingEnvironment);
		PendingEnvironment.Reset();
		SuspensionReset = MoveTemp(PendingSuspensionReset);
		PendingSuspensionReset.Reset();
		ResetSequence = PendingSuspensionResetSequence;
		SupersededMassSequence = ResetSupersedesMassSequence;
		P4Sequence = P4RequestSequence;
		EspSequence = EspRequestSequence;
		MassSequence = MassRequestSequence;
		P4MotorTorque = PendingP4MotorTorque;
		EspBrakeTorque = PendingEspBrakeTorque;
		SprungMass = PendingSprungMass;
		PendingP4MotorTorque.Reset();
		PendingEspBrakeTorque.Reset();
		PendingSprungMass.Reset();
	}
	if (Settings.IsSet()) Runtime.ApplySettings(MoveTemp(Settings.GetValue()));
	if (NewEnvironment.IsSet()) Environment = MoveTemp(NewEnvironment.GetValue());
	if (SuspensionReset.IsSet())
	{
		Runtime.SuspensionState = SuspensionReset.GetValue();
		AppliedSuspensionResetSequence = ResetSequence;
		AppliedMassSequence = FMath::Max(AppliedMassSequence, SupersededMassSequence);
	}
	if (P4MotorTorque.IsSet())
	{
		Runtime.WheelState.P4MotorTorque = P4MotorTorque.GetValue();
		AppliedP4Sequence = P4Sequence;
	}
	if (EspBrakeTorque.IsSet())
	{
		Runtime.WheelState.BrakeTorqueFromESP = EspBrakeTorque.GetValue();
		AppliedEspSequence = EspSequence;
	}
	if (SprungMass.IsSet())
	{
		Runtime.SetSprungMass(SprungMass.GetValue());
		AppliedMassSequence = MassSequence;
	}
}

void FVehicleWheelBridge::BeginPhysicsStep()
{
	if (bPhysicsStepBegun) return;
	bPhysicsStepBegun = true;
	ConsumeInputs();
	PreviousSuspensionState = Runtime.SuspensionState;
	Frame.ComponentRelativeTransform = Environment.ComponentRelativeTransform;
	Frame.WorldGravityZ = Environment.WorldGravityZ;
}

void FVehicleWheelBridge::PublishResult()
{
	FVehicleWheelResult Result;
	Result.WheelState = Runtime.WheelState;
	Result.SuspensionState = Runtime.SuspensionState;
	Result.PreviousSuspensionState = PreviousSuspensionState;
	Result.ChassisWorldTransform = Frame.ChassisWorldTransform;
	Result.MacroDeltaTime = Runtime.WheelContext.MacroDeltaTime;
	Result.PublicationSequence = ++PublicationSequence;
	Result.AppliedSuspensionResetSequence = AppliedSuspensionResetSequence;
	Result.AppliedP4Sequence = AppliedP4Sequence;
	Result.AppliedEspSequence = AppliedEspSequence;
	Result.AppliedMassSequence = AppliedMassSequence;
	bPhysicsStepBegun = false;
	FScopeLock Lock(&TransferMutex);
	PublishedResult = MoveTemp(Result);
	PublishedContact = Contact;
}

FVehicleSuspensionHitResult FVehicleWheelBridge::ReadContactResult() const
{
	FScopeLock Lock(&TransferMutex);
	return PublishedContact;
}

FVehicleWheelResult FVehicleWheelBridge::ReadResult() const
{
	FScopeLock Lock(&TransferMutex);
	return PublishedResult;
}
