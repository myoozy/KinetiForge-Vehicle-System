// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "VehicleAxlePhysicsAssembly.h"
#include "VehiclePowerUnitBridge.h"

/** One simulation consumer owns this drivetrain assembly. Components only prepare its
 * bindings and transfer commands/results; suspension and drivetrain remain separate stages. */
class KINETIFORGE_API FVehicleDriveRuntime
{
public:
	FVehicleDriveRuntime(TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> InPowerUnit,
		FVehiclePhysicsBodyIdentity InChassis, TArray<FVehicleAxlePhysicsAssembly> InAxles);
	void Update(float DeltaTime, const FVehiclePhysicsExecutionContext& Context);

private:
	int32 SubstepTransferCase(float DeltaTime, float OutputTorque, float Brake, float Handbrake,
		bool bLineLock, const FVehiclePhysicsExecutionContext& Context,
		float& OutputAngularVelocity, float& OutputInertia, float& OutputStiffness);
	TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> PowerUnit;
	FVehiclePhysicsBodyIdentity Chassis;
	TArray<FVehicleAxlePhysicsAssembly> Axles;
};
