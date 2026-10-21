// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "HAL/CriticalSection.h"
#include "VehicleWheelRuntime.h"
#include "VehicleWheelPhysics.h"

/** Prepared on GT. World is borrowed for the acknowledged UE5.3 query boundary;
 * this snapshot does not guarantee world/body lifetime or stop callbacks. */
struct KINETIFORGE_API FVehicleWheelEnvironment
{
	FTransform ComponentRelativeTransform = FTransform::Identity;
	FTransform3f DesignedHubLocalTransform = FTransform3f::Identity;
	float WorldGravityZ = 9.81f;
	const UWorld* World = nullptr;
	FCollisionQueryParams QueryParams = FCollisionQueryParams::DefaultQueryParam;
	FCollisionResponseParams ResponseParams = FCollisionResponseParams::DefaultResponseParam;
};

/** Numerical display snapshot. No query object references or live runtime references. */
struct KINETIFORGE_API FVehicleWheelResult
{
	FVehicleWheelSimState WheelState;
	FVehicleSuspensionSimState SuspensionState;
	FVehicleSuspensionSimState PreviousSuspensionState;
	FTransform ChassisWorldTransform = FTransform::Identity;
	float MacroDeltaTime = 0.f;
	uint64 PublicationSequence = 0;
	uint64 AppliedSuspensionResetSequence = 0;
	uint64 AppliedP4Sequence = 0;
	uint64 AppliedEspSequence = 0;
	uint64 AppliedMassSequence = 0;
};

/** One simulation consumer owns Runtime. Producers replace settings and absolute command
 * values; readers receive the last published copy. Locks only cover transfer, never equations.
 * This boundary does not bind components, bodies or callbacks, or make those lifetimes safe. */
class KINETIFORGE_API FVehicleWheelBridge
{
public:
	explicit FVehicleWheelBridge(FVehicleWheelRuntime InitialRuntime);
	/** Prepare through GT CaptureRuntimeSettings/FromCachedConfig before submitting. */
	void SubmitSettings(FVehicleWheelRuntimeSettings Settings);
	void SubmitEnvironment(FVehicleWheelEnvironment Environment);
	/** Explicit initialization/preview reset; supersedes older pending mass setters. */
	uint64 RequestSuspensionReset(const FVehicleSuspensionSimState& State);
	uint64 RequestP4MotorTorque(float Torque);
	uint64 RequestEspBrakeTorque(float Torque);
	uint64 RequestSprungMass(float Mass);
	FVehicleWheelResult ReadResult() const;
	FVehicleSuspensionHitResult ReadContactResult() const;

	/** Only the simulation owner may consume, access Runtime or publish. */
	void ConsumeInputs();
	void BeginPhysicsStep();
	FVehicleWheelRuntime& GetRuntimeForSimulation() { return Runtime; }
	const FVehicleWheelEnvironment& GetEnvironmentForSimulation() const { return Environment; }
	FVehicleWheelPhysicsFrame& GetFrameForSimulation() { return Frame; }
	FVehicleSuspensionHitResult& GetContactForSimulation() { return Contact; }
	void PublishResult();

private:
	FVehicleWheelRuntime Runtime;
	FVehicleWheelEnvironment Environment;
	FVehicleWheelPhysicsFrame Frame;
	FVehicleSuspensionHitResult Contact;
	FVehicleSuspensionSimState PreviousSuspensionState;
	mutable FCriticalSection TransferMutex;
	TOptional<FVehicleWheelRuntimeSettings> PendingSettings;
	TOptional<FVehicleWheelEnvironment> PendingEnvironment;
	TOptional<FVehicleSuspensionSimState> PendingSuspensionReset;
	// Absolute setters have no transition side effects: latest value per field wins.
	TOptional<float> PendingP4MotorTorque;
	TOptional<float> PendingEspBrakeTorque;
	TOptional<float> PendingSprungMass;
	FVehicleWheelResult PublishedResult;
	FVehicleSuspensionHitResult PublishedContact;
	uint64 PublicationSequence = 0;
	uint64 SuspensionResetSequence = 0;
	uint64 PendingSuspensionResetSequence = 0;
	uint64 AppliedSuspensionResetSequence = 0;
	bool bPhysicsStepBegun = false;
	uint64 P4RequestSequence = 0, EspRequestSequence = 0, MassRequestSequence = 0;
	uint64 AppliedP4Sequence = 0, AppliedEspSequence = 0, AppliedMassSequence = 0;
	uint64 ResetSupersedesMassSequence = 0;
};
