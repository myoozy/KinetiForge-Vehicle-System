// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehicleAxlePhysicsAssembly.h"
#include "ProfilingDebugging/CpuProfilerTrace.h"

#include "VehicleAxleSolver.h"
#include "VehicleDifferentialSolver.h"
#include "VehicleSuspensionSolver.h"
#include "VehicleWheelPhysics.h"
#include "VehicleUtilities.h"
#include "Chaos/ParticleHandle.h"
#include "Chaos/Particle/ParticleUtilities.h"

FVehicleAxlePhysicsAssembly::FVehicleAxlePhysicsAssembly(
	TSharedPtr<FVehicleAxleBridge, ESPMode::ThreadSafe> InAxleBridge,
	FVehicleAxlePhysicsWheelBinding InLeft, FVehicleAxlePhysicsWheelBinding InRight)
	: AxleBridge(MoveTemp(InAxleBridge)), Left(MoveTemp(InLeft)), Right(MoveTemp(InRight))
{
	check(AxleBridge.IsValid());
}

FVehicleAxleRuntime& FVehicleAxlePhysicsAssembly::GetRuntimeForSimulation()
{
	check(AxleBridge.IsValid());
	return AxleBridge->GetRuntimeForSimulation();
}

Chaos::FRigidBodyHandle_Internal* FVehicleAxlePhysicsAssembly::ResolveBody(
	FVehiclePhysicsMacrostepBodyResolver& Resolver, const FVehicleAxlePhysicsWheelBinding& Binding)
{
	return Binding.IsPresent() ? Resolver.ResolveBody(Binding.ChassisBodyIdentity) : nullptr;
}

bool FVehicleAxlePhysicsAssembly::AreBodiesAvailable(const FVehiclePhysicsExecutionContext& Context) const
{
	FVehiclePhysicsMacrostepBodyResolver Resolver(Context);
	return AreBodiesAvailable(Resolver);
}

bool FVehicleAxlePhysicsAssembly::AreBodiesAvailable(FVehiclePhysicsMacrostepBodyResolver& Resolver) const
{
	if (!AxleBridge.IsValid()) return false;
	if (Left.IsPresent() && !Resolver.ResolveBody(Left.ChassisBodyIdentity)) return false;
	if (Right.IsPresent() && !Resolver.ResolveBody(Right.ChassisBodyIdentity)) return false;
	return true;
}

FVehicleAxleWheelInput FVehicleAxlePhysicsAssembly::CaptureWheelInput(
	const FVehicleAxlePhysicsWheelBinding& Binding)
{
	if (!Binding.IsPresent()) return FVehicleAxleWheelInput();
	const FVehicleWheelRuntime& Runtime = Binding.WheelBridge->GetRuntimeForSimulation();
	return { Runtime.WheelState.AngularVelocity, Runtime.WheelState.EffectiveInertia,
		Runtime.Settings.WheelConfig.Inertia, Runtime.WheelState.PredictedSlipRatio };
}

float FVehicleAxlePhysicsAssembly::CaptureTrackWidth(float FallbackWidth,
	const FVehicleAxlePhysicsWheelBinding& LeftBinding, const FVehicleAxlePhysicsWheelBinding& RightBinding)
{
	if (!LeftBinding.IsPresent() || !RightBinding.IsPresent()) return FallbackWidth;
	const FVehicleWheelEnvironment& LeftEnvironment = LeftBinding.WheelBridge->GetEnvironmentForSimulation();
	const FVehicleWheelEnvironment& RightEnvironment = RightBinding.WheelBridge->GetEnvironmentForSimulation();
	const float LeftY = FTransform3f(LeftEnvironment.ComponentRelativeTransform).TransformPositionNoScale(
		LeftEnvironment.DesignedHubLocalTransform.GetLocation()).Y;
	const float RightY = FTransform3f(RightEnvironment.ComponentRelativeTransform).TransformPositionNoScale(
		RightEnvironment.DesignedHubLocalTransform.GetLocation()).Y;
	return FMath::Abs(RightY - LeftY);
}

bool FVehicleAxlePhysicsAssembly::PrepareWheelFrame(const FVehicleAxlePhysicsWheelBinding& Binding,
	Chaos::FRigidBodyHandle_Internal* ChassisHandle, float MacroDeltaTime, bool bNormalizeRotation)
{
	if (!Binding.IsPresent()) return true;
	if (!ChassisHandle) return false;
	FVehicleWheelBridge& Bridge = *Binding.WheelBridge;
	FVehicleWheelPhysicsFrame& Frame = Bridge.GetFrameForSimulation();
	const FVehicleWheelEnvironment& Environment = Bridge.GetEnvironmentForSimulation();
	Frame.MacroDeltaTime = MacroDeltaTime;
	Frame.ChassisWorldTransform = Chaos::FParticleUtilitiesGT::GetActorWorldTransform(ChassisHandle);
	if (bNormalizeRotation && !Frame.ChassisWorldTransform.IsRotationNormalized())
		Frame.ChassisWorldTransform.NormalizeRotation();
	Frame.ChassisState.FetchChassisPhysicsState(ChassisHandle, MacroDeltaTime);
	Frame.ComponentRelativeTransform = Environment.ComponentRelativeTransform;
	Frame.WorldGravityZ = Environment.WorldGravityZ;
	return true;
}

void FVehicleAxlePhysicsAssembly::PreStepSolidSuspension(float ActiveSwaybarStiffness)
{
	FVehicleAxleRuntime& AxleRuntime = GetRuntimeForSimulation();
	FVehicleWheelRuntime& LeftRuntime = Left.WheelBridge->GetRuntimeForSimulation();
	FVehicleWheelRuntime& RightRuntime = Right.WheelBridge->GetRuntimeForSimulation();
	FVehicleWheelPhysicsFrame& LeftFrame = Left.WheelBridge->GetFrameForSimulation();
	FVehicleWheelPhysicsFrame& RightFrame = Right.WheelBridge->GetFrameForSimulation();
	const FVehicleWheelEnvironment& LeftEnvironment = Left.WheelBridge->GetEnvironmentForSimulation();
	const FVehicleWheelEnvironment& RightEnvironment = Right.WheelBridge->GetEnvironmentForSimulation();

	const float HubZLeft = LeftRuntime.SuspensionState.HubChassisLocation.Z;
	const float HubZRight = RightRuntime.SuspensionState.HubChassisLocation.Z;
	FVehicleWheelPhysics::StartSolidAxleSuspension(LeftRuntime, LeftSolidContext, LeftFrame,
		LeftEnvironment.World, LeftEnvironment.QueryParams, LeftEnvironment.ResponseParams,
		AxleRuntime.State.LeftWheelSteeringAngle, ActiveSwaybarStiffness, HubZRight);
	FVehicleWheelPhysics::StartSolidAxleSuspension(RightRuntime, RightSolidContext, RightFrame,
		RightEnvironment.World, RightEnvironment.QueryParams, RightEnvironment.ResponseParams,
		AxleRuntime.State.RightWheelSteeringAngle, ActiveSwaybarStiffness, HubZLeft);

	const float TrackWidth = CaptureTrackWidth(AxleRuntime.Settings.AxleConfig.TrackWidth, Left, Right);
	const float AxleHalfWidth = (TrackWidth
		- LeftRuntime.Settings.KinematicsConfig.HubOffsetFromLowerJoint.Y
		- RightRuntime.Settings.KinematicsConfig.HubOffsetFromLowerJoint.Y) * 0.5f;
	const FVector3f LeftTopMount = FVehicleSuspensionSolver::GetTopMountChassisLocation(
		LeftRuntime.Settings.KinematicsConfig, FTransform3f(LeftEnvironment.ComponentRelativeTransform));
	const FVector3f RightTopMount = FVehicleSuspensionSolver::GetTopMountChassisLocation(
		RightRuntime.Settings.KinematicsConfig, FTransform3f(RightEnvironment.ComponentRelativeTransform));
	const float LeftStrutRealLength = LeftSolidContext.StrutCurrentLength
		+ LeftRuntime.Settings.KinematicsConfig.MinStrutLength;
	const float RightStrutRealLength = RightSolidContext.StrutCurrentLength
		+ RightRuntime.Settings.KinematicsConfig.MinStrutLength;
	FVector3f AxleCenter = FVector3f::ZeroVector;
	FQuat4f AxleRotation = FQuat4f::Identity;
	FVehicleSuspensionSolver::SolveSolidAxlePosture(LeftTopMount, RightTopMount,
		LeftStrutRealLength, RightStrutRealLength, AxleHalfWidth, AxleCenter, AxleRotation);

	FVehicleWheelPhysics::FinalizeSolidAxleSuspension(LeftRuntime, LeftSolidContext, LeftFrame,
		ActiveSwaybarStiffness, HubZRight, AxleHalfWidth, AxleCenter, AxleRotation,
		Left.WheelBridge->GetContactForSimulation());
	FVehicleWheelPhysics::FinalizeSolidAxleSuspension(RightRuntime, RightSolidContext, RightFrame,
		ActiveSwaybarStiffness, HubZLeft, AxleHalfWidth, AxleCenter, AxleRotation,
		Right.WheelBridge->GetContactForSimulation());
}

bool FVehicleAxlePhysicsAssembly::PreStep(const FVehiclePhysicsExecutionContext& Context,
	float MacroDeltaTime, float SteeringInput)
{
	FVehiclePhysicsMacrostepBodyResolver Resolver(Context);
	return PreStep(Resolver, MacroDeltaTime, SteeringInput);
}

bool FVehicleAxlePhysicsAssembly::PreStep(FVehiclePhysicsMacrostepBodyResolver& Resolver,
	float MacroDeltaTime, float SteeringInput)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_AxleAssembly_PhysicsPreStep);
	bMacroStepPrepared = false;
	if (!AreBodiesAvailable(Resolver)) return false;
	Chaos::FRigidBodyHandle_Internal* LeftHandle = ResolveBody(Resolver, Left);
	Chaos::FRigidBodyHandle_Internal* RightHandle = ResolveBody(Resolver, Right);

	AxleBridge->ConsumeInputs();
	if (Left.IsPresent()) Left.WheelBridge->BeginPhysicsStep();
	if (Right.IsPresent()) Right.WheelBridge->BeginPhysicsStep();
	FVehicleAxleRuntime& AxleRuntime = GetRuntimeForSimulation();
	FVehicleAxleSimState& State = AxleRuntime.State;
	const FVehicleAxleRuntimeSettings& Settings = AxleRuntime.Settings;
	State.NumOfWheels = (Left.IsPresent() ? 1 : 0) + (Right.IsPresent() ? 1 : 0);
	if (State.NumOfWheels <= 0)
	{
		bMacroStepPrepared = true;
		return true;
	}

	const FVehicleSuspensionSimState* LeftSuspension = Left.IsPresent()
		? &Left.WheelBridge->GetRuntimeForSimulation().SuspensionState : nullptr;
	const FVehicleSuspensionSimState* RightSuspension = Right.IsPresent()
		? &Right.WheelBridge->GetRuntimeForSimulation().SuspensionState : nullptr;
	FQuat4f LeftRotation, RightRotation;
	if (Left.IsPresent()) LeftRotation = (FQuat4f)Left.WheelBridge->GetFrameForSimulation().ChassisWorldTransform.GetRotation();
	if (Right.IsPresent()) RightRotation = (FQuat4f)Right.WheelBridge->GetFrameForSimulation().ChassisWorldTransform.GetRotation();
	FVehicleAxleSolver::UpdateLinearVelocity(State, LeftSuspension, Left.IsPresent() ? &LeftRotation : nullptr,
		RightSuspension, Right.IsPresent() ? &RightRotation : nullptr);

	if (Settings.SteeringConfig.bAffectedBySteering)
	{
		FVehicleAxleSolver::UpdateSteeringAssist(State, Settings.SteeringConfig,
			Settings.SteeringAssistConfig, MacroDeltaTime, SteeringInput);
		FVehicleAxleSolver::UpdateSteering(State, Settings.SteeringConfig, State.RealSteeringValue,
			CaptureTrackWidth(Settings.AxleConfig.TrackWidth, Left, Right));
	}
	// Velocity feedback uses the preceding wheel frame, before suspension refreshes its pose.
	const bool bNormalizeRotation = State.NumOfWheels == 1 || Settings.SuspensionType != EVehicleAxleSuspensionType::Solid;
	if (!PrepareWheelFrame(Left, LeftHandle, MacroDeltaTime, bNormalizeRotation)
		|| !PrepareWheelFrame(Right, RightHandle, MacroDeltaTime, bNormalizeRotation)) return false;

	if (State.NumOfWheels == 2)
	{
		if (Settings.SuspensionType == EVehicleAxleSuspensionType::Solid)
		{
			LeftSolidContext = FVehicleSuspensionSimContext();
			RightSolidContext = FVehicleSuspensionSimContext();
			PreStepSolidSuspension(Settings.AxleConfig.SwaybarStiffness);
		}
		else
		{
			LeftIndependentStep = FVehicleWheelIndependentSuspensionStep();
			RightIndependentStep = FVehicleWheelIndependentSuspensionStep();
			FVehicleWheelRuntime& LeftRuntime = Left.WheelBridge->GetRuntimeForSimulation();
			FVehicleWheelRuntime& RightRuntime = Right.WheelBridge->GetRuntimeForSimulation();
			FVehicleWheelPhysicsFrame& LeftFrame = Left.WheelBridge->GetFrameForSimulation();
			FVehicleWheelPhysicsFrame& RightFrame = Right.WheelBridge->GetFrameForSimulation();
			const FVehicleWheelEnvironment& LeftEnvironment = Left.WheelBridge->GetEnvironmentForSimulation();
			const FVehicleWheelEnvironment& RightEnvironment = Right.WheelBridge->GetEnvironmentForSimulation();
			LeftFrame.MacroDeltaTime = MacroDeltaTime;
			RightFrame.MacroDeltaTime = MacroDeltaTime;
			FVehicleWheelPhysics::PrepareIndependentSuspension(LeftRuntime.SuspensionState,
				LeftIndependentStep.Context, LeftFrame, LeftRuntime.Settings.WheelConfig,
				LeftRuntime.Settings.KinematicsConfig, LeftEnvironment.World,
				LeftEnvironment.QueryParams, LeftEnvironment.ResponseParams, State.LeftWheelSteeringAngle);
			FVehicleWheelPhysics::PrepareIndependentSuspension(RightRuntime.SuspensionState,
				RightIndependentStep.Context, RightFrame, RightRuntime.Settings.WheelConfig,
				RightRuntime.Settings.KinematicsConfig, RightEnvironment.World,
				RightEnvironment.QueryParams, RightEnvironment.ResponseParams, State.RightWheelSteeringAngle);
			FVehicleSuspensionIntegrationInput LeftInput{ LeftIndependentStep.Context, LeftFrame.ChassisState,
				LeftFrame.WorldGravityZ, LeftRuntime.Settings.WheelConfig.Radius,
				LeftRuntime.Settings.WheelConfig.Inertia, LeftRuntime.Settings.KinematicsConfig,
				LeftRuntime.Settings.SpringConfig, LeftRuntime.Settings.SuspensionLUTs };
			FVehicleSuspensionIntegrationInput RightInput{ RightIndependentStep.Context, RightFrame.ChassisState,
				RightFrame.WorldGravityZ, RightRuntime.Settings.WheelConfig.Radius,
				RightRuntime.Settings.WheelConfig.Inertia, RightRuntime.Settings.KinematicsConfig,
				RightRuntime.Settings.SpringConfig, RightRuntime.Settings.SuspensionLUTs };
			FVehicleSuspensionSolver::IntegratePairedIndependentSuspension(
				LeftInput, RightInput, Settings.AxleConfig.SwaybarStiffness);
			FVehicleSuspensionSolver::PrepareIndependentSuspensionGeometry(LeftInput.Context,
				LeftInput.WheelRadius, LeftInput.KinematicsConfig, LeftInput.CachedLUTs);
			FVehicleSuspensionSolver::PrepareIndependentSuspensionGeometry(RightInput.Context,
				RightInput.WheelRadius, RightInput.KinematicsConfig, RightInput.CachedLUTs);
			const float FinalLeftHubZ = LeftInput.Context.HubChassisTransform.GetLocation().Z;
			const float FinalRightHubZ = RightInput.Context.HubChassisTransform.GetLocation().Z;
			FVehicleWheelPhysics::FinalizePreparedIndependentSuspension(LeftInput.Context,
				LeftRuntime.SuspensionState, LeftRuntime.WheelState, LeftRuntime.Settings.WheelConfig,
				LeftRuntime.Settings.KinematicsConfig, LeftRuntime.Settings.SpringConfig,
				LeftRuntime.Settings.SuspensionLUTs, LeftFrame, Settings.AxleConfig.SwaybarStiffness,
				FinalRightHubZ, Left.WheelBridge->GetContactForSimulation());
			FVehicleWheelPhysics::FinalizePreparedIndependentSuspension(RightInput.Context,
				RightRuntime.SuspensionState, RightRuntime.WheelState, RightRuntime.Settings.WheelConfig,
				RightRuntime.Settings.KinematicsConfig, RightRuntime.Settings.SpringConfig,
				RightRuntime.Settings.SuspensionLUTs, RightFrame, Settings.AxleConfig.SwaybarStiffness,
				FinalLeftHubZ, Right.WheelBridge->GetContactForSimulation());
		}
		State.NumOfWheelOnGround = Left.WheelBridge->GetRuntimeForSimulation().SuspensionState.bWheelOnGround
			+ Right.WheelBridge->GetRuntimeForSimulation().SuspensionState.bWheelOnGround;
		Left.WheelBridge->GetRuntimeForSimulation().PrepareWheel(MacroDeltaTime,
			Left.WheelBridge->GetFrameForSimulation().ChassisWorldTransform);
		Right.WheelBridge->GetRuntimeForSimulation().PrepareWheel(MacroDeltaTime,
			Right.WheelBridge->GetFrameForSimulation().ChassisWorldTransform);
	}
	else
	{
		FVehicleAxlePhysicsWheelBinding& Active = Left.IsPresent() ? Left : Right;
		const float SteeringAngle = Left.IsPresent() ? State.LeftWheelSteeringAngle : State.RightWheelSteeringAngle;
		FVehicleWheelRuntime& Runtime = Active.WheelBridge->GetRuntimeForSimulation();
		FVehicleWheelPhysicsFrame& Frame = Active.WheelBridge->GetFrameForSimulation();
		const FVehicleWheelEnvironment& Environment = Active.WheelBridge->GetEnvironmentForSimulation();
		if (Left.IsPresent()) LeftIndependentStep = FVehicleWheelIndependentSuspensionStep();
		else RightIndependentStep = FVehicleWheelIndependentSuspensionStep();
		Frame.MacroDeltaTime = MacroDeltaTime;
		FVehicleWheelPhysics::UpdateIndependentSuspension(Runtime, Frame, Environment.World,
			Environment.QueryParams, Environment.ResponseParams, SteeringAngle, 0.f, 0.f,
			Active.WheelBridge->GetContactForSimulation());
		State.NumOfWheelOnGround = Runtime.SuspensionState.bWheelOnGround;
		Runtime.PrepareWheel(MacroDeltaTime, Frame.ChassisWorldTransform);
	}

	bMacroStepPrepared = true;
	return true;
}

void FVehicleAxlePhysicsAssembly::Substep(const FVehiclePhysicsExecutionContext& Context,
	float SubstepDeltaTime, float DriveTorque, float BrakeInput, float HandbrakeInput,
	float& OutAxleEffectiveInertia, float& OutAngularVelocity, float& OutDriveShaftStiffness)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_AxleAssembly_PhysicsSubstep);
	(void)Context; // Body handles are only needed at the macrostep frame and poststep boundaries.
	FVehicleAxleRuntime& AxleRuntime = GetRuntimeForSimulation();
	FVehicleAxleSimState& State = AxleRuntime.State;
	if (!bMacroStepPrepared)
	{
		OutAxleEffectiveInertia = State.AxleEffectiveInertia;
		OutAngularVelocity = State.AxleAngularVelocity;
		OutDriveShaftStiffness = 0.f;
		return;
	}
	const FVehicleAxleRuntimeSettings& Settings = AxleRuntime.Settings;
	const FVehicleAxleWheelInput LeftBefore = CaptureWheelInput(Left);
	const FVehicleAxleWheelInput RightBefore = CaptureWheelInput(Right);
	const FVehicleLimitedSlipDifferentialConfig* DiffConfig = Settings.bHasDifferential
		? &Settings.DifferentialConfig : nullptr;
	const bool bWheelNotDriven = FVehicleAxleSolver::PrepareTransmissionStep(State,
		Settings.AxleConfig, Settings.TCSConfig, Settings.Layout, DiffConfig,
		Left.IsPresent() ? &LeftBefore : nullptr, Right.IsPresent() ? &RightBefore : nullptr,
		SubstepDeltaTime, DriveTorque, BrakeInput, HandbrakeInput);

	if (Left.IsPresent()) Left.WheelBridge->GetRuntimeForSimulation().IntegrateWheel(SubstepDeltaTime,
		State.LeftDriveTorque, State.BrakeTorque, State.HandbrakeTorque, State.ReflectedInertiaOnWheel);
	if (Right.IsPresent()) Right.WheelBridge->GetRuntimeForSimulation().IntegrateWheel(SubstepDeltaTime,
		State.RightDriveTorque, State.BrakeTorque, State.HandbrakeTorque, State.ReflectedInertiaOnWheel);

	// Feedback must observe this substep, never the preceding published display result.
	const FVehicleAxleWheelInput LeftAfter = CaptureWheelInput(Left);
	const FVehicleAxleWheelInput RightAfter = CaptureWheelInput(Right);
	OutDriveShaftStiffness = FVehicleAxleSolver::FinalizeTransmissionStep(State,
		Settings.AxleConfig, DiffConfig, Left.IsPresent() ? &LeftAfter : nullptr,
		Right.IsPresent() ? &RightAfter : nullptr, bWheelNotDriven);
	OutAxleEffectiveInertia = State.AxleEffectiveInertia;
	OutAngularVelocity = State.AxleAngularVelocity;
}

void FVehicleAxlePhysicsAssembly::PostStep(const FVehiclePhysicsExecutionContext& Context)
{
	FVehiclePhysicsMacrostepBodyResolver Resolver(Context);
	PostStep(Resolver);
}

void FVehicleAxlePhysicsAssembly::PostStep(FVehiclePhysicsMacrostepBodyResolver& Resolver)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_AxleAssembly_PhysicsPostStep);
	if (!bMacroStepPrepared) return;
	const auto FinalizeWheel = [&Resolver](FVehicleAxlePhysicsWheelBinding& Binding)
	{
		if (!Binding.IsPresent()) return;
		FVehicleWheelRuntime& Runtime = Binding.WheelBridge->GetRuntimeForSimulation();
		Runtime.FinalizeWheel();
		Chaos::FRigidBodyHandle_Internal* ChassisHandle = Resolver.ResolveBody(Binding.ChassisBodyIdentity);
		FVehicleWheelPhysics::ApplyWheelForce(Runtime.WheelState, Runtime.WheelContext,
			Runtime.SuspensionState, Binding.WheelBridge->GetContactForSimulation(),
			Runtime.Settings.KinematicsConfig.LocationToApplyForce,
			Binding.WheelBridge->GetFrameForSimulation().ChassisWorldTransform, ChassisHandle);
		Binding.WheelBridge->PublishResult();
	};
	FinalizeWheel(Left);
	FinalizeWheel(Right);
	AxleBridge->PublishResult();
	bMacroStepPrepared = false;
}
