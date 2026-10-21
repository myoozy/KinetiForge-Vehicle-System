// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehicleInputSolver.h"

#include "VehicleDrivetrainMath.h"

namespace
{
	float InterpAxis(float Current, float Target, float DeltaTime, FVector2f Speed)
	{
		const float SelectedSpeed = Target < SMALL_NUMBER ? Speed.Y : Speed.X;
		return SelectedSpeed <= 0.f ? Target : FMath::FInterpConstantTo(Current, Target, DeltaTime, SelectedSpeed);
	}

	void UpdateThrottle(const FVehicleInputRuntimeSettings& Settings, FVehicleInputPipeline& Pipeline,
		const FVehicleInputStepContext& Context, float DeltaTime)
	{
		const float RealThrottleInput = Pipeline.bSwitchThrottleAndBrake ? Pipeline.Raw.Brake : Pipeline.Raw.Throttle;
		if (Context.bHasGearbox)
		{
			//if not in gear and should rev-match
			if (Context.GearboxState.bShouldRevMatch &&
				RealThrottleInput < Settings.Assist.RevMatchMaxThrottle &&
				FMath::Abs(Context.LocalLongitudinalVelocity) > 0.5f &&
				Settings.Assist.bRevMatching)
			{
				const float Rate = 5.f;
				Pipeline.Smoothened.Throttle += VehicleDrivetrainMath::DivideOrDefault(
					DeltaTime * Rate, Context.GearboxConfig.ShiftDelay);
				Pipeline.Smoothened.Throttle = FMath::Min(Pipeline.Smoothened.Throttle, Settings.Assist.RevMatchMaxThrottle);
			}
			//if not in gear and no rev-matching and not sequential
			else if (!Context.GearboxState.bIsInGear &&
				Settings.Assist.bAutomaticClutch && !Context.GearboxConfig.bSequentialGearbox)
			{
				Pipeline.Smoothened.Throttle = 0.f;
			}
			else
			{
				Pipeline.Smoothened.Throttle = InterpAxis(Pipeline.Smoothened.Throttle, RealThrottleInput,
					DeltaTime, Settings.Throttle.InterpSpeed);
				Pipeline.Smoothened.Throttle *= !(Settings.Assist.bEVClutchLogic &&
					Context.GearboxState.CurrentGearRatio == 0.f);
			}
		}
		else
		{
			Pipeline.Smoothened.Throttle = InterpAxis(Pipeline.Smoothened.Throttle, RealThrottleInput,
				DeltaTime, Settings.Throttle.InterpSpeed);
		}
		Pipeline.Smoothened.Throttle = FMath::Clamp(Pipeline.Smoothened.Throttle, 0.f, 1.f);
		Pipeline.Final.Throttle = FMath::Clamp(Settings.Throttle.ResponseCurve.Evaluate(Pipeline.Smoothened.Throttle), 0.f, 1.f);
	}

	void UpdateBrake(const FVehicleInputRuntimeSettings& Settings, FVehicleInputPipeline& Pipeline, float DeltaTime)
	{
		const float RealBrakeInput = Pipeline.bSwitchThrottleAndBrake ? Pipeline.Raw.Throttle : Pipeline.Raw.Brake;
		//update brake
		Pipeline.Smoothened.Brake = InterpAxis(Pipeline.Smoothened.Brake, RealBrakeInput, DeltaTime, Settings.Brake.InterpSpeed);
		Pipeline.Smoothened.Brake = FMath::Clamp(Pipeline.Smoothened.Brake, 0.f, 1.f);
		Pipeline.Final.Brake = FMath::Clamp(Settings.Brake.ResponseCurve.Evaluate(Pipeline.Smoothened.Brake), 0.f, 1.f);
	}

	void UpdateClutch(const FVehicleInputRuntimeSettings& Settings, FVehicleInputPipeline& Pipeline,
		const FVehicleInputStepContext& Context, float DeltaTime)
	{
		if (Settings.Assist.bEVClutchLogic)
		{
			Pipeline.Smoothened.Clutch = 0.f;
		}
		else if (Settings.Assist.bAutomaticClutch && Context.bHasGearbox && Context.bHasEngine)
		{
			//check if clutch has to be engaged
			const bool bNotInGearAndNotSequential = !(Context.GearboxState.bIsInGear || Context.GearboxConfig.bSequentialGearbox);
			float TargetClutchValue = (Pipeline.Raw.Handbrake > 0.9f || bNotInGearAndNotSequential) ? 1.f : Pipeline.Raw.Clutch;

			//take engine rpm into account
			const float Bias = FMath::GetMappedRangeValueClamped(Settings.Assist.AutoClutchRange,
				FVector2f(1.f, 0.f), Context.EngineRPM);
			TargetClutchValue = FMath::Clamp(TargetClutchValue + Bias, 0.f, 1.f);
			Pipeline.Smoothened.Clutch = FMath::Min(Pipeline.Smoothened.Clutch + Bias, TargetClutchValue);

			//get interp speed, when changing gear, clutch should engage faster than gear changes
			FVector2f FinalInterpSpeed;
			const float Rate = 10.f;
			FinalInterpSpeed.Y = Settings.Clutch.InterpSpeed.Y;
			FinalInterpSpeed.X = bNotInGearAndNotSequential
				? VehicleDrivetrainMath::DivideOrDefault(Rate, Context.GearboxConfig.ShiftDelay)
				: Settings.Clutch.InterpSpeed.X;

			Pipeline.Smoothened.Clutch = InterpAxis(Pipeline.Smoothened.Clutch, TargetClutchValue, DeltaTime, FinalInterpSpeed);
		}
		else
		{
			Pipeline.Smoothened.Clutch = InterpAxis(Pipeline.Smoothened.Clutch, Pipeline.Raw.Clutch,
				DeltaTime, Settings.Clutch.InterpSpeed);
		}
		Pipeline.Smoothened.Clutch = FMath::Clamp(Pipeline.Smoothened.Clutch, 0.f, 1.f);
		Pipeline.Final.Clutch = FMath::Clamp(Settings.Clutch.ResponseCurve.Evaluate(Pipeline.Smoothened.Clutch), 0.f, 1.f);
	}

	void UpdateSteering(const FVehicleInputRuntimeSettings& Settings, FVehicleInputPipeline& Pipeline,
		const FVehicleInputStepContext& Context, float DeltaTime)
	{
		const float RealSteeringInput = Pipeline.Raw.Steering;
		float Scale = 1.f;

		//reduce the steering value when driving at high speed
		if (Settings.HighSpeedSteeringScale.bEnabled)
		{
			Scale = Settings.HighSpeedSteeringScale.Curve.Eval(FMath::Abs(Context.LocalLongitudinalVelocity));
		}

		Pipeline.Smoothened.Steering = InterpAxis(Pipeline.Smoothened.Steering, RealSteeringInput,
			DeltaTime, Settings.Steering.InterpSpeed);
		Pipeline.Smoothened.Steering = FMath::Clamp(Pipeline.Smoothened.Steering, -1.f, 1.f);

		if (Settings.Steering.ResponseCurve.bEnabled)
		{
			const float CurveValue = Settings.Steering.ResponseCurve.Curve.Eval(FMath::Abs(Pipeline.Smoothened.Steering));
			Pipeline.Final.Steering = CurveValue * FMath::Sign(Pipeline.Smoothened.Steering) * Scale;
		}
		else
		{
			Pipeline.Final.Steering = Pipeline.Smoothened.Steering * Scale;
		}
		Pipeline.Final.Steering = FMath::Clamp(Pipeline.Final.Steering, -1.f, 1.f);
	}

	void UpdateHandbrake(const FVehicleInputRuntimeSettings& Settings, FVehicleInputPipeline& Pipeline,
		const FVehicleInputStepContext& Context, float DeltaTime)
	{
		const float RealThrottleInput = Pipeline.bSwitchThrottleAndBrake ? Pipeline.Raw.Brake : Pipeline.Raw.Throttle;
		//update handbrake
		float HandbrakeTarget = Pipeline.Raw.Handbrake;
		if (Settings.Assist.bAutoHold && FMath::Abs(Context.LocalLongitudinalVelocity) < 0.1f &&
			RealThrottleInput < SMALL_NUMBER && !Context.bIsInAir)
		{
			HandbrakeTarget = 1.f;
		}
		Pipeline.Smoothened.Handbrake = InterpAxis(Pipeline.Smoothened.Handbrake, HandbrakeTarget,
			DeltaTime, Settings.Handbrake.InterpSpeed);
		Pipeline.Smoothened.Handbrake = FMath::Clamp(Pipeline.Smoothened.Handbrake, 0.f, 1.f);
		Pipeline.Final.Handbrake = FMath::Clamp(Settings.Handbrake.ResponseCurve.Evaluate(Pipeline.Smoothened.Handbrake), 0.f, 1.f);
	}
}

void FVehicleInputSolver::Update(const FVehicleInputRuntimeSettings& Settings, FVehicleInputPipeline& Pipeline,
	const FVehicleInputStepContext& Context, float DeltaTime)
{
	if (Context.bHasGearbox)
	{
		if (Settings.bAutomaticGearbox && Settings.bArcadeAutoGearbox)
		{
			Pipeline.bSwitchThrottleAndBrake = Context.GearboxState.CurrentGear < 0;
		}
	}
	else
	{
		Pipeline.bSwitchThrottleAndBrake = false;
	}

	UpdateThrottle(Settings, Pipeline, Context, DeltaTime);
	UpdateBrake(Settings, Pipeline, DeltaTime);
	UpdateClutch(Settings, Pipeline, Context, DeltaTime);
	UpdateSteering(Settings, Pipeline, Context, DeltaTime);
	UpdateHandbrake(Settings, Pipeline, Context, DeltaTime);
}

void FVehicleInputSolver::UpdateAxis(const FVehicleInputRuntimeSettings& Settings, FVehicleInputPipeline& Pipeline,
	const FVehicleInputStepContext& Context, EVehicleInputAxis Axis, float DeltaTime)
{
	switch (Axis)
	{
	case EVehicleInputAxis::Throttle: UpdateThrottle(Settings, Pipeline, Context, DeltaTime); break;
	case EVehicleInputAxis::Brake: UpdateBrake(Settings, Pipeline, DeltaTime); break;
	case EVehicleInputAxis::Clutch: UpdateClutch(Settings, Pipeline, Context, DeltaTime); break;
	case EVehicleInputAxis::Steering: UpdateSteering(Settings, Pipeline, Context, DeltaTime); break;
	case EVehicleInputAxis::Handbrake: UpdateHandbrake(Settings, Pipeline, Context, DeltaTime); break;
	}
}

void FVehicleInputSolver::ApplyCommand(FVehicleInputPipeline& Pipeline, const FVehicleInputCommand& Command)
{
	float* RawValue = nullptr;
	float* DirectValue = nullptr;
	switch (Command.Axis)
	{
	case EVehicleInputAxis::Throttle:
		RawValue = &Pipeline.Raw.Throttle;
		DirectValue = Command.bSwitchThrottleAndBrake ? &Pipeline.Smoothened.Brake : &Pipeline.Smoothened.Throttle;
		break;
	case EVehicleInputAxis::Brake:
		RawValue = &Pipeline.Raw.Brake;
		DirectValue = Command.bSwitchThrottleAndBrake ? &Pipeline.Smoothened.Throttle : &Pipeline.Smoothened.Brake;
		break;
	case EVehicleInputAxis::Clutch:
		RawValue = &Pipeline.Raw.Clutch;
		DirectValue = &Pipeline.Smoothened.Clutch;
		break;
	case EVehicleInputAxis::Steering:
		RawValue = &Pipeline.Raw.Steering;
		DirectValue = &Pipeline.Smoothened.Steering;
		break;
	case EVehicleInputAxis::Handbrake:
		RawValue = &Pipeline.Raw.Handbrake;
		DirectValue = &Pipeline.Smoothened.Handbrake;
		break;
	default:
		return;
	}
	*RawValue = Command.Input;
	if (Command.bDirectInput)
	{
		*DirectValue = Command.Input;
	}
}
