// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "VehicleWheelRuntime.h"

/** Borrowed values for one physics step. Each wheel may use a different chassis frame. */
struct KINETIFORGE_API FVehicleWheelPhysicsFrame
{
	FTransform ComponentRelativeTransform = FTransform::Identity;
	FTransform ChassisWorldTransform = FTransform::Identity;
	FVehicleChassisSimState ChassisState;
	float WorldGravityZ = 9.81f;
	float MacroDeltaTime = 1.f / 120.f;
};

/** Short-lived caller-owned data for one prepared independent suspension step. */
struct KINETIFORGE_API FVehicleWheelIndependentSuspensionStep
{
	FVehicleWheelPhysicsFrame Frame;
	FVehicleSuspensionSimContext Context;
};

/** Suspension/query and impulse orchestration for an owned wheel, separate from tire
 * integration. No component discovery or binding lifetime is managed here. World queries
 * and contact-object access retain the acknowledged UE5.3 risks inside the contact bridge. */
class KINETIFORGE_API FVehicleWheelPhysics
{
public:
	static void UpdateIndependentSuspension(FVehicleWheelRuntime& Runtime,
		const FVehicleWheelPhysicsFrame& Frame, const UWorld* World,
		const FCollisionQueryParams& QueryParams, const FCollisionResponseParams& ResponseParams,
		float SteeringAngle, float ActiveSwaybarStiffness, float OtherHubChassisZ,
		FVehicleSuspensionHitResult& OutputContact);
	/** Query/contact preparation shared by paired independent suspension. */
	static void PrepareIndependentSuspension(const FVehicleSuspensionSimState& PreviousState,
		FVehicleSuspensionSimContext& Context,
		const FVehicleWheelPhysicsFrame& Frame, const FVehicleWheelConfig& WheelConfig,
		const FVehicleSuspensionKinematicsConfig& KinematicsConfig,
		const UWorld* World, const FCollisionQueryParams& QueryParams,
		const FCollisionResponseParams& ResponseParams, float SteeringAngle);
	/** Geometry and force finalization after the caller has integrated Context. */
	static void FinalizePreparedIndependentSuspension(FVehicleSuspensionSimContext& Context,
		FVehicleSuspensionSimState& SuspensionState, const FVehicleWheelSimState& WheelState,
		const FVehicleWheelConfig& WheelConfig,
		const FVehicleSuspensionKinematicsConfig& KinematicsConfig,
		const FVehicleSuspensionSpringConfig& SpringConfig,
		const FVehicleSuspensionCachedLUTs& SuspensionLUTs,
		const FVehicleWheelPhysicsFrame& Frame,
		float ActiveSwaybarStiffness, float OtherHubChassisZ,
		FVehicleSuspensionHitResult& OutputContact);
	/** Context timestep is intentionally preserved, as in the legacy solid axle Start. */
	static void StartSolidAxleSuspension(const FVehicleWheelRuntime& Runtime,
		FVehicleSuspensionSimContext& Context, const FVehicleWheelPhysicsFrame& Frame, const UWorld* World,
		const FCollisionQueryParams& QueryParams, const FCollisionResponseParams& ResponseParams,
		float SteeringAngle, float ActiveSwaybarStiffness, float OtherHubChassisZ);
	static void FinalizeSolidAxleSuspension(FVehicleWheelRuntime& Runtime,
		FVehicleSuspensionSimContext& Context, const FVehicleWheelPhysicsFrame& Frame,
		float ActiveSwaybarStiffness, float OtherHubChassisZ, float AxleHalfWidth,
		const FVector3f& AxleChassisCenter, const FQuat4f& AxleChassisRotation,
		FVehicleSuspensionHitResult& OutputContact);

	/** Caller guarantees ChassisHandle stays valid for this physics phase. Reaction is
	 * applied immediately to the contact body, never queued for game Tick. */
	static void ApplyWheelForce(const FVehicleWheelSimState& WheelState,
		const FVehicleWheelSimContext& WheelContext, const FVehicleSuspensionSimState& SuspensionState,
		const FVehicleSuspensionHitResult& Contact, EVehicleForceApplicationPoint ForceApplicationPoint,
		const FTransform& ChassisWorldTransform, Chaos::FRigidBodyHandle_Internal* ChassisHandle);
};
