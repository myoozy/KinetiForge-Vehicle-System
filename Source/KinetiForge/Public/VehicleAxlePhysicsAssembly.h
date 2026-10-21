// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "VehicleAxleBridge.h"
#include "VehicleWheelBridge.h"
#include "VehiclePhysicsBodyBinding.h"

struct FVehicleAxleWheelInput;
#include "VehiclePhysicsStep.h"

namespace Chaos { class FRigidBodyHandle_Internal; }

/** One fixed axle side. An unset wheel bridge represents a missing wheel. Body identity is
 * captured on GT and must be replaced by rebuilding the assembly when the body changes. */
struct KINETIFORGE_API FVehicleAxlePhysicsWheelBinding
{
	TSharedPtr<FVehicleWheelBridge, ESPMode::ThreadSafe> WheelBridge;
	FVehiclePhysicsBodyIdentity ChassisBodyIdentity;

	bool IsPresent() const { return WheelBridge.IsValid(); }
};

/** Fixed left/right wheel assembly. UObject discovery and component ownership remain in the
 * GT adapter. World queries/contact binding are the deliberate UE boundary in WheelPhysics. */
class KINETIFORGE_API FVehicleAxlePhysicsAssembly
{
public:
	FVehicleAxlePhysicsAssembly(TSharedPtr<FVehicleAxleBridge, ESPMode::ThreadSafe> InAxleBridge,
		FVehicleAxlePhysicsWheelBinding InLeft, FVehicleAxlePhysicsWheelBinding InRight);

	FVehicleAxleRuntime& GetRuntimeForSimulation();

	/** Call across the complete Drive assembly before consuming any inputs or integrating. */
	bool AreBodiesAvailable(const FVehiclePhysicsExecutionContext& Context) const;
	bool AreBodiesAvailable(FVehiclePhysicsMacrostepBodyResolver& Resolver) const;

	/** Consumes each axle/wheel mailbox once, refreshes borrowed body frames, and runs the
	 * existing axle steering/suspension/wheel-prestep order. */
	bool PreStep(const FVehiclePhysicsExecutionContext& Context, float MacroDeltaTime, float SteeringInput);
	bool PreStep(FVehiclePhysicsMacrostepBodyResolver& Resolver, float MacroDeltaTime, float SteeringInput);

	/** Transmission prepare, wheel integration, then same-substep wheel feedback/finalize. */
	void Substep(const FVehiclePhysicsExecutionContext& Context, float SubstepDeltaTime,
		float DriveTorque, float BrakeInput, float HandbrakeInput,
		float& OutAxleEffectiveInertia, float& OutAngularVelocity, float& OutDriveShaftStiffness);

	/** Finalizes wheel states, applies chassis/contact impulses in this solver phase, then publishes. */
	void PostStep(const FVehiclePhysicsExecutionContext& Context);
	void PostStep(FVehiclePhysicsMacrostepBodyResolver& Resolver);

private:
	TSharedPtr<FVehicleAxleBridge, ESPMode::ThreadSafe> AxleBridge;
	FVehicleAxlePhysicsWheelBinding Left;
	FVehicleAxlePhysicsWheelBinding Right;
	FVehicleWheelIndependentSuspensionStep LeftIndependentStep;
	FVehicleWheelIndependentSuspensionStep RightIndependentStep;
	FVehicleSuspensionSimContext LeftSolidContext;
	FVehicleSuspensionSimContext RightSolidContext;
	bool bMacroStepPrepared = false;

	static FVehicleAxleWheelInput CaptureWheelInput(const FVehicleAxlePhysicsWheelBinding& Binding);
	static float CaptureTrackWidth(float FallbackWidth,
		const FVehicleAxlePhysicsWheelBinding& Left, const FVehicleAxlePhysicsWheelBinding& Right);
	static Chaos::FRigidBodyHandle_Internal* ResolveBody(FVehiclePhysicsMacrostepBodyResolver& Resolver,
		const FVehicleAxlePhysicsWheelBinding& Binding);
	bool PrepareWheelFrame(const FVehicleAxlePhysicsWheelBinding& Binding,
		Chaos::FRigidBodyHandle_Internal* ChassisHandle, float MacroDeltaTime, bool bNormalizeRotation);
	void PreStepSolidSuspension(float ActiveSwaybarStiffness);
};
