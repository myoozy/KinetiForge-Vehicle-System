// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "HAL/PlatformTLS.h"
#include "VehicleAeroBaseComponent.h"
#include "VehicleAsyncSpringArmComponent.h"
#include "VehicleAuxiliaryLaneTestComponents.generated.h"

UCLASS()
class UVehicleAuxiliaryLaneTestAero : public UVehicleAeroBaseComponent
{
	GENERATED_BODY()
public:
	TAtomic<int32> Calls{0};
	TAtomic<int32> ThreadId{-1};
	FTransform LastParentWorldTransform = FTransform::Identity; // Written and observed only on GT.
	virtual void CalculateAerodynamics(float PhysicsDeltaTime, const FVector& ParentCOMWorldLocation,
		const FVector& ParentWorldLinearVelocity, const FVector& ParentWorldAngularVelocity,
		const FTransform& ParentWorldTransform, const FVector& WindWorldVelocity,
		FVector& OutLinearForce, FVector& OutAngularTorque) override
	{
		ThreadId.Store(static_cast<int32>(FPlatformTLS::GetCurrentThreadId()));
		++Calls;
		LastParentWorldTransform = ParentWorldTransform;
		OutLinearForce = FVector(0.f, 0.f, 0.01f);
		OutAngularTorque = FVector::ZeroVector;
	}
};

UCLASS()
class UVehicleAuxiliaryLaneTestSpringArm : public UVehicleAsyncSpringArmComponent
{
	GENERATED_BODY()
public:
	float ReadLastPhysicsDeltaTime() const { return LastPhysicsDeltaTime; }
	float ReadTimeSinceLastPhysTick() const { return TimeSinceLastPhysTick; }
	FTransform ReadDesiredTransform() const { return DesiredTransformToPhysParent; }
	FTransform ReadPhysicsOriginWorldTransform() const { return AsyncDesiredArmOriginWorldTransform; }
	void SetPhysicsStateSentinels() { LastPhysicsDeltaTime = 7.f; TimeSinceLastPhysTick = 9.f; }
};
