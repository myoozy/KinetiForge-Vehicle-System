// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehicleWheelPhysics.h"
#include "VehicleSuspensionContactBridge.h"
#include "Chaos/ParticleHandle.h"
#include "Chaos/Particle/ParticleUtilities.h"

void FVehicleWheelPhysics::UpdateIndependentSuspension(FVehicleWheelRuntime& Runtime,
	const FVehicleWheelPhysicsFrame& Frame, const UWorld* World,
	const FCollisionQueryParams& QueryParams, const FCollisionResponseParams& ResponseParams,
	float SteeringAngle, float ActiveSwaybarStiffness, float OtherHubChassisZ,
	FVehicleSuspensionHitResult& OutputContact)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_Wheel_SuspensionSolver_UpdateSuspension);
	const FVehicleWheelRuntimeSettings& Settings = Runtime.Settings;
	FVehicleSuspensionSimContext Context;
	PrepareIndependentSuspension(Runtime.SuspensionState, Context, Frame, Settings.WheelConfig,
		Settings.KinematicsConfig,
		World, QueryParams, ResponseParams, SteeringAngle);
	FVehicleSuspensionSolver::IntegrateSingleWheelSuspension(Context, Frame.ChassisState,
		Frame.WorldGravityZ, Settings.WheelConfig.Radius, Settings.WheelConfig.Inertia,
		Settings.KinematicsConfig, Settings.SpringConfig, Settings.SuspensionLUTs,
		ActiveSwaybarStiffness, OtherHubChassisZ);
	FVehicleSuspensionSolver::PrepareIndependentSuspensionGeometry(Context,
		Settings.WheelConfig.Radius, Settings.KinematicsConfig, Settings.SuspensionLUTs);
	FinalizePreparedIndependentSuspension(Context, Runtime.SuspensionState, Runtime.WheelState,
		Settings.WheelConfig, Settings.KinematicsConfig, Settings.SpringConfig,
		Settings.SuspensionLUTs, Frame, ActiveSwaybarStiffness, OtherHubChassisZ, OutputContact);
}

void FVehicleWheelPhysics::PrepareIndependentSuspension(const FVehicleSuspensionSimState& PreviousState,
	FVehicleSuspensionSimContext& Context,
	const FVehicleWheelPhysicsFrame& Frame, const FVehicleWheelConfig& WheelConfig,
	const FVehicleSuspensionKinematicsConfig& KinematicsConfig,
	const UWorld* World, const FCollisionQueryParams& QueryParams,
	const FCollisionResponseParams& ResponseParams, float SteeringAngle)
{
	Context.PhysicsDeltaTime = Frame.MacroDeltaTime;
	FVehicleSuspensionSolver::PrepareSuspensionContext(PreviousState, Context,
		Frame.ComponentRelativeTransform, Frame.ChassisWorldTransform, KinematicsConfig, SteeringAngle);
	FVehicleSuspensionContactBridge::Query(Context, World, WheelConfig.Radius,
		WheelConfig.Width * 0.5f, QueryParams, ResponseParams, KinematicsConfig);
	FVehicleSuspensionContactBridge::CalculateImpactPointWorldVelocity(Context, Frame.ChassisState);
}

void FVehicleWheelPhysics::FinalizePreparedIndependentSuspension(FVehicleSuspensionSimContext& Context,
	FVehicleSuspensionSimState& SuspensionState, const FVehicleWheelSimState& WheelState,
	const FVehicleWheelConfig& WheelConfig,
	const FVehicleSuspensionKinematicsConfig& KinematicsConfig,
	const FVehicleSuspensionSpringConfig& SpringConfig,
	const FVehicleSuspensionCachedLUTs& SuspensionLUTs,
	const FVehicleWheelPhysicsFrame& Frame,
	float ActiveSwaybarStiffness, float OtherHubChassisZ,
	FVehicleSuspensionHitResult& OutputContact)
{
	FVehicleSuspensionSolver::FinalizePreparedIndependentSuspension(Context, SuspensionState,
		WheelConfig.Radius, WheelState.TireForce, KinematicsConfig,
		SpringConfig, Frame.ChassisWorldTransform, Frame.ChassisState, Frame.WorldGravityZ,
		SuspensionLUTs, ActiveSwaybarStiffness, OtherHubChassisZ);
	FVehicleSuspensionSolver::CopyContextToHitResult(Context, OutputContact);
}

void FVehicleWheelPhysics::StartSolidAxleSuspension(const FVehicleWheelRuntime& Runtime,
	FVehicleSuspensionSimContext& Context, const FVehicleWheelPhysicsFrame& Frame, const UWorld* World,
	const FCollisionQueryParams& QueryParams, const FCollisionResponseParams& ResponseParams,
	float SteeringAngle, float ActiveSwaybarStiffness, float OtherHubChassisZ)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_Wheel_SuspensionSolver_UpdateSuspension);
	const FVehicleWheelRuntimeSettings& Settings = Runtime.Settings;
	// The unsprung integration divides the per-frame limit change by this timestep, so it must be
	// the real macrostep. A stale value scales the damper velocity (and therefore the damping
	// ratio) by realDt/staleDt, which shows up as frame-rate-dependent body damping.
	Context.PhysicsDeltaTime = Frame.MacroDeltaTime;
	FVehicleSuspensionSolver::PrepareSuspensionContext(Runtime.SuspensionState, Context,
		Frame.ComponentRelativeTransform, Frame.ChassisWorldTransform, Settings.KinematicsConfig, SteeringAngle);
	FVehicleSuspensionContactBridge::Query(Context, World, Settings.WheelConfig.Radius,
		Settings.WheelConfig.Width * 0.5f, QueryParams, ResponseParams, Settings.KinematicsConfig);
	FVehicleSuspensionContactBridge::CalculateImpactPointWorldVelocity(Context, Frame.ChassisState);
	FVehicleSuspensionSolver::IntegrateSingleWheelSuspension(Context, Frame.ChassisState,
		Frame.WorldGravityZ, Settings.WheelConfig.Radius, Settings.WheelConfig.Inertia,
		Settings.KinematicsConfig, Settings.SpringConfig, Settings.SuspensionLUTs,
		ActiveSwaybarStiffness, OtherHubChassisZ);
}

void FVehicleWheelPhysics::FinalizeSolidAxleSuspension(FVehicleWheelRuntime& Runtime,
	FVehicleSuspensionSimContext& Context, const FVehicleWheelPhysicsFrame& Frame,
	float ActiveSwaybarStiffness, float OtherHubChassisZ, float AxleHalfWidth,
	const FVector3f& AxleChassisCenter, const FQuat4f& AxleChassisRotation,
	FVehicleSuspensionHitResult& OutputContact)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_Wheel_SuspensionSolver_UpdateSuspension);
	const FVehicleWheelRuntimeSettings& Settings = Runtime.Settings;
	Context.PhysicsDeltaTime = Frame.MacroDeltaTime;
	FVehicleSuspensionSolver::FinalizeSolidAxleSuspension(Context, Runtime.SuspensionState,
		Settings.WheelConfig.Radius, Settings.KinematicsConfig, Settings.SpringConfig,
		Frame.ChassisWorldTransform, Frame.ChassisState, Frame.WorldGravityZ, Settings.SuspensionLUTs,
		ActiveSwaybarStiffness, OtherHubChassisZ, AxleHalfWidth, AxleChassisCenter, AxleChassisRotation,
		Runtime.WheelState.TireForce);
	FVehicleSuspensionSolver::CopyContextToHitResult(Context, OutputContact);
}

void FVehicleWheelPhysics::ApplyWheelForce(const FVehicleWheelSimState& WheelState,
	const FVehicleWheelSimContext& WheelContext, const FVehicleSuspensionSimState& SuspensionState,
	const FVehicleSuspensionHitResult& Contact, EVehicleForceApplicationPoint ForceApplicationPoint,
	const FTransform& ChassisWorldTransform, Chaos::FRigidBodyHandle_Internal* ChassisHandle)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForge_Wheel_ApplyForce);
	if (!SuspensionState.bHitGround || ChassisHandle == nullptr) return;

	FVector PosToApplyImpulse = FVector(0.f);
	switch (ForceApplicationPoint)
	{
	case EVehicleForceApplicationPoint::ImpactPoint:
		PosToApplyImpulse = SuspensionState.ImpactWorldLocation;
		break;
	case EVehicleForceApplicationPoint::WheelCenter:
		PosToApplyImpulse = ChassisWorldTransform.TransformPositionNoScale((FVector)SuspensionState.HubChassisLocation);
		break;
	default:
		PosToApplyImpulse = SuspensionState.ImpactWorldLocation;
		break;
	}

	// get world com
	Chaos::FVec3 ChassisWorldCOM = Chaos::FParticleUtilitiesGT::GetCoMWorldPosition(ChassisHandle);
	// get arm
	Chaos::FVec3 LeverArmVec = PosToApplyImpulse - ChassisWorldCOM;
	// get normal of ground
	FVector ImpactNormal = (FVector)SuspensionState.ImpactWorldNormal;

	/*-------------------------------------APPLY FORCE---------------------------------------*/
	// project the suspension force onto the impact normal
	// so that the car will not move by itself when parked
	FVector SuspensionForceProj = ImpactNormal * SuspensionState.ForceAlongImpactNormal;
	FVector Impulse = ((FVector)WheelState.TireForce + SuspensionForceProj) * WheelContext.MacroDeltaTime;
	Impulse *= 100.; // because of the unit of unreal engine

	// apply force to Chassis
	FVector AngularImpulse = FVector::CrossProduct(LeverArmVec, Impulse);
	ChassisHandle->SetLinearImpulse(ChassisHandle->LinearImpulse() + Impulse, false);
	ChassisHandle->SetAngularImpulse(ChassisHandle->AngularImpulse() + AngularImpulse, false);

	// also add force to the contacted component, in the same physics phase
	FVehicleSuspensionContactBridge::ApplyContactReactionImpulse(
		Contact, SuspensionState.ImpactWorldLocation, Impulse);
}
