// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"

namespace Chaos { class FRigidBodyHandle_Internal; }
struct FProxyTimestampBase;

/** Physics-consumer state for ONE chassis. Transport angular momentum when the engine
 * does not provide rigid-body gyroscopic integration. No body handle survives a step. */
struct KINETIFORGE_API FVehicleChassisMomentum
{
	void Apply(Chaos::FRigidBodyHandle_Internal* Body);
	void Reset();

private:
	TSharedPtr<FProxyTimestampBase, ESPMode::ThreadSafe> PreviousBody;
	FQuat PreviousMassRotation = FQuat::Identity;
	FVector PreviousInertia = FVector::ZeroVector;
	bool bHasHistory = false;
};
