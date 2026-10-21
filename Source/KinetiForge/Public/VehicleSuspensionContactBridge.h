// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "VehicleSuspensionStructs.h"

/**
 * UE5.3 contact boundary: UWorld queries, hit object/material access and reaction impulses.
 * These calls remain in the physics phase and are knowingly not UObject-thread-safe.
 * Replace this bridge when upgrading the engine; do not defer reaction impulses to game Tick.
 * Context still carries the legacy FHitResult. Numerical consumers may read its geometry,
 * but must not dereference its object references.
 */
class KINETIFORGE_API FVehicleSuspensionContactBridge
{
public:
	static void Query(
		FVehicleSuspensionSimContext& Context,
		const UWorld* World,
		float WheelRadius,
		float HalfWheelWidth,
		const FCollisionQueryParams& QueryParams,
		const FCollisionResponseParams& ResponseParams,
		const FVehicleSuspensionKinematicsConfig& Config);

	static void CalculateImpactPointWorldVelocity(
		FVehicleSuspensionSimContext& Context,
		const FVehicleChassisSimState& ChassisState);

	/** ChassisImpulse already includes dt and the conversion to Unreal units. */
	static void ApplyContactReactionImpulse(
		const FVehicleSuspensionHitResult& Contact,
		const FVector& ImpactWorldLocation,
		const FVector& ChassisImpulse);

private:
	static bool ShouldDoRefinedTrace(FVehicleSuspensionSimContext& Context,
		float HalfWheelWidth, const FVehicleSuspensionKinematicsConfig& Config);
	static void ComputeHitDistance(FVehicleSuspensionSimContext& Context,
		float WheelRadius, float EquivalentSphereTraceRadius);
	static void CacheImpactFriction(
		FVehicleSuspensionSimContext& Ctx
	);
	static float SuspensionLineTrace(
		FVehicleSuspensionSimContext& Ctx,
		const UWorld* World,
		const float WheelRadius,
		const float HalfWheelWidth,
		const FCollisionQueryParams& QueryParams,
		const FCollisionResponseParams& ResponseParams,
		const FVehicleSuspensionKinematicsConfig& Config
	);
	static float SuspensionSphereTrace(
		FVehicleSuspensionSimContext& Ctx,
		const UWorld* World,
		const float WheelRadius,
		const float HalfWheelWidth,
		const FCollisionQueryParams& QueryParams,
		const FCollisionResponseParams& ResponseParams,
		const FVehicleSuspensionKinematicsConfig& Config
	);
	static float SuspensionBoxTrace(
		FVehicleSuspensionSimContext& Ctx,
		const UWorld* World,
		const float WheelRadius,
		const float HalfWheelWidth,
		const FCollisionQueryParams& QueryParams,
		const FCollisionResponseParams& ResponseParams,
		const FVehicleSuspensionKinematicsConfig& Config
	);
	static float SuspensionSphereTraceNoRefinement(
		FVehicleSuspensionSimContext& Ctx,
		const UWorld* World,
		const float WheelRadius,
		const float HalfWheelWidth,
		const FCollisionQueryParams& QueryParams,
		const FCollisionResponseParams& ResponseParams,
		const FVehicleSuspensionKinematicsConfig& Config
	);
	static float SuspensionMultiSphereTrace(
		FVehicleSuspensionSimContext& Ctx,
		const UWorld* World,
		const float WheelRadius,
		const float HalfWheelWidth,
		const FCollisionQueryParams& QueryParams,
		const FCollisionResponseParams& ResponseParams,
		const FVehicleSuspensionKinematicsConfig& Config
	);
};
