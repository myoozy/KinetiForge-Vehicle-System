// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehicleFixedDriveRuntime.h"
#include "ProfilingDebugging/CpuProfilerTrace.h"
#include "VehicleDifferentialSolver.h"
#include "VehiclePhysicsStep.h"
#include "Chaos/ParticleHandle.h"
#include "Chaos/Particle/ParticleUtilities.h"

FVehicleFixedDriveRuntime::FVehicleFixedDriveRuntime(
	TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> InPowerUnit,
	FVehiclePhysicsBodyIdentity InChassis, TArray<FVehicleAxlePhysicsAssembly> InAxles)
	: PowerUnit(MoveTemp(InPowerUnit)), Chassis(MoveTemp(InChassis)), Axles(MoveTemp(InAxles)) {}

void FVehicleFixedDriveRuntime::Update(float DeltaTime, const FVehiclePhysicsExecutionContext& Context)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_DriveAssembly_UpdatePhysics);
	// This vehicle owns its borrowed lookups only until the current macrostep returns.
	FVehiclePhysicsMacrostepBodyResolver Resolver(Context);
	// Validate the whole assembly before consuming commands or integrating any axle.
	Chaos::FRigidBodyHandle_Internal* ChassisHandle = nullptr;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_DriveAssembly_ValidateBodies);
		ChassisHandle = Resolver.ResolveBody(Chassis);
		if (!PowerUnit || !ChassisHandle) return;
		for (const auto& Axle : Axles) if (!Axle.AreBodiesAvailable(Resolver)) return;
	}
	PowerUnit->ConsumeInputs();
	auto& Input = PowerUnit->GetDriveInputForSimulation();
	auto& Unit = PowerUnit->GetRuntimeForSimulation();
	auto& Telemetry = Input.Telemetry;
	Telemetry.PhysicsDeltaTime = DeltaTime;
	const int32 Substeps = FMath::Max(1, FMath::CeilToInt32(
		UVehicleUtilities::SafeDivide(DeltaTime, Input.Settings.SubstepDeltaTime)));
	const float SubstepDeltaTime = DeltaTime / Substeps;

	// Suspension preparation precedes the input and drivetrain substeps, as in the component path.
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_DriveAssembly_SuspensionPreStep);
		for (auto& Axle : Axles) Axle.PreStep(Resolver, DeltaTime, Input.InputValues.Final.Steering);
	}
	for (int32 Substep = 0; Substep < Substeps; ++Substep)
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_DriveAssembly_PhysicsSubstep);
		FVehicleInputStepContext InputContext;
		InputContext.bHasGearbox = InputContext.bHasEngine = true;
		InputContext.GearboxConfig = Unit.GearboxConfig;
		InputContext.GearboxState = Unit.GearboxState;
		InputContext.EngineRPM = Unit.EngineState.EngineRPM;
		InputContext.LocalLongitudinalVelocity = Telemetry.LocalLinearVelocity.X;
		InputContext.bIsInAir = Telemetry.bIsInAir;
		{
			TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_DriveAssembly_UpdateInput);
			FVehicleInputSolver::Update(Input.Settings, Input.InputValues, InputContext, SubstepDeltaTime);
		}
		const auto& Controls = Input.InputValues.Final;
		const float OutputTorque = Unit.UpdateEngineAndCalculateOutputTorque(
			SubstepDeltaTime, Controls.Throttle, Telemetry.NumOfDriveAxles > 0);
		float OutputAngularVelocity = 0.f, OutputInertia = 0.f, OutputStiffness = 0.f;
		Telemetry.NumOfDriveAxles = SubstepTransferCase(SubstepDeltaTime, OutputTorque,
			Controls.Brake, Controls.Handbrake, Input.Settings.Assist.bBurnOutAssist
			&& Controls.Throttle > 0.9f && Controls.Brake > 0.9f && Telemetry.LocalLinearVelocity.X < 1.f,
			Context, OutputAngularVelocity, OutputInertia, OutputStiffness);
		// Feedback from this substep supplies the next clutch step.
		Unit.UpdateClutchFromOutputShaft(SubstepDeltaTime, Controls.Clutch,
			OutputAngularVelocity, OutputInertia, OutputStiffness);
	}
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_DriveAssembly_PostStep);
		for (auto& Axle : Axles) Axle.PostStep(Resolver);
	}
	Telemetry.NumOfWheelsOnGround = 0;
	FVector3f SumLocalVelocity(0.f), SumWorldVelocity(0.f);
	for (auto& Axle : Axles)
	{
		const auto& State = Axle.GetRuntimeForSimulation().State;
		Telemetry.NumOfWheelsOnGround += State.NumOfWheelOnGround;
		SumLocalVelocity += State.LocalLinearVelocity * State.NumOfWheelOnGround;
		SumWorldVelocity += State.WorldLinearVelocity * State.NumOfWheelOnGround;
	}
	Telemetry.bIsInAir = !Telemetry.NumOfWheelsOnGround;
	const float GroundedInverse = UVehicleUtilities::SafeDivide(1.f, float(Telemetry.NumOfWheelsOnGround));
	Telemetry.LocalLinearVelocity = SumLocalVelocity * GroundedInverse;
	Telemetry.WorldLinearVelocity = SumWorldVelocity * GroundedInverse;
	Telemetry.LocalVelocityClamped = Telemetry.LocalLinearVelocity.SquaredLength() > 0.01f
		? Telemetry.LocalLinearVelocity : FVector3f(0.f);
	const FVector3f PreviousVelocity = Telemetry.AbsoluteWorldLinearVelocity;
	Telemetry.AbsoluteWorldLinearVelocity = 0.01f * FVector3f(ChassisHandle->V());
	Telemetry.WorldAcceleration = UVehicleUtilities::SafeDivide(
		Telemetry.AbsoluteWorldLinearVelocity - PreviousVelocity, DeltaTime);
	const FQuat4f ChassisRotation(Chaos::FParticleUtilitiesGT::GetActorWorldTransform(ChassisHandle).GetRotation());
	Telemetry.LocalAcceleration = ChassisRotation.UnrotateVector(Telemetry.WorldAcceleration);
	PowerUnit->PublishResult();
}

int32 FVehicleFixedDriveRuntime::SubstepTransferCase(float DeltaTime, float OutputTorque,
	float Brake, float Handbrake, bool bLineLock, const FVehiclePhysicsExecutionContext& Context,
	float& OutputAngularVelocity, float& OutputInertia, float& OutputStiffness)
{
	const auto& Config = PowerUnit->GetTransferCaseConfigForSimulation();
	int32 DrivenAxles = 0;
	float SumWeight = 0.f, TotalInertia = 0.f, TotalMomentum = 0.f;
	for (auto& Axle : Axles)
	{
		const auto& Runtime = Axle.GetRuntimeForSimulation();
		if (Runtime.Settings.AxleConfig.TorqueWeight <= SMALL_NUMBER) continue;
		++DrivenAxles;
		SumWeight += FMath::Abs(Runtime.Settings.AxleConfig.TorqueWeight);
		TotalInertia += Runtime.State.AxleEffectiveInertia;
		TotalMomentum += Runtime.State.AxleEffectiveInertia * Runtime.State.AxleAngularVelocity;
	}
	const float LockedVelocity = FVehicleDifferentialSolver::CalculateLockedAngularVelocity(TotalMomentum, TotalInertia);
	float SumVelocity = 0.f, SumInertia = 0.f, SumStiffness = 0.f;
	for (auto& Axle : Axles)
	{
		const auto& Runtime = Axle.GetRuntimeForSimulation();
		const bool bDriven = Runtime.Settings.AxleConfig.TorqueWeight > SMALL_NUMBER;
		float Torque = 0.f;
		bool bReleaseBrake = false;
		if (bDriven)
		{
			const auto Output = FVehicleDifferentialSolver::CalculateTransferCaseAxleOutput(Config,
				Config.GearRatio * OutputTorque, LockedVelocity, Runtime.State.AxleAngularVelocity,
				Runtime.State.AxleEffectiveInertia, Runtime.Settings.AxleConfig.TorqueWeight, SumWeight, DeltaTime);
			Torque = Output.DriveTorque;
			bReleaseBrake = Output.NormalizedTorqueWeight > 0.5f && bLineLock;
		}
		float AxleInertia = 0.f, AxleVelocity = 0.f, AxleStiffness = 0.f;
		Axle.Substep(Context, DeltaTime, Torque, Brake * !bReleaseBrake, Handbrake,
			AxleInertia, AxleVelocity, AxleStiffness);
		if (bDriven)
		{
			SumVelocity += AxleVelocity;
			SumInertia += AxleInertia;
			SumStiffness += AxleStiffness;
		}
	}
	OutputAngularVelocity = UVehicleUtilities::SafeDivide(SumVelocity * Config.GearRatio, float(DrivenAxles));
	const float RatioSquareInverse = UVehicleUtilities::SafeDivide(1.f, Config.GearRatio * Config.GearRatio);
	OutputInertia = SumInertia * RatioSquareInverse;
	OutputStiffness = SumStiffness * RatioSquareInverse;
	return DrivenAxles;
}
