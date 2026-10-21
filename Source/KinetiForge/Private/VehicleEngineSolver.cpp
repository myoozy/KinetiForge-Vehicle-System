// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehicleEngineSolver.h"
#include "VehicleUtilities.h"
#include "VehicleDrivetrainMath.h"

FVehicleEngineParameters FVehicleEngineParameters::FromConfig(const FVehicleNaturallyAspiratedEngineConfig& Config)
{
	FVehicleEngineParameters Parameters;
	Parameters.MaxEngineTorque = Config.MaxEngineTorque;
	Parameters.EngineIdleRPM = Config.EngineIdleRPM;
	Parameters.EngineMaxRPM = Config.EngineMaxRPM;
	Parameters.EngineInertia = Config.EngineInertia;
	Parameters.StartFriction = Config.StartFriction;
	Parameters.FrictionCoefficient = Config.FrictionCoefficient;
	Parameters.PumpingLossCoefficient = Config.PumpingLossCoefficient;
	Parameters.IdleThrottleInterpSpeed = Config.IdleThrottleInterpSpeed;
	Parameters.RevLimiterTime = Config.RevLimiterTime;
	Parameters.MaxPowerLambda = Config.MaxPowerLambda;
	Parameters.DeceleratingLambda = Config.DeceleratingLambda;
	Parameters.UnburntFuelAccumulationRate = Config.UnburntFuelAccumulationRate;
	return Parameters;
}

float FVehicleEngineSolver::CalculateMaxTorque(const FVehicleEngineParameters& Parameters,
	const FVehicleEngineTurboConfig& TurboConfig)
{
	const float TurboBoost = FMath::Max(0.f, TurboConfig.MaxBoostPressure * TurboConfig.BoostEfficiency);
	return Parameters.MaxEngineTorque * (1.f + TurboBoost);
}

void FVehicleEngineSolver::EngineAcceleration(const FVehicleEngineParameters& Parameters,
	const FVehicleEngineTurboConfig& TurboConfig, const FVehicleLUT<32>& TorqueLUT,
	FVehicleEngineSimState& State, const float DeltaTime, bool& bShouldTriggerTurboBlowOffCallback)
{
	// absolut engine rpm
	float AbsolutRPM = FMath::Abs(State.EngineRPM);

	// internal friction of the engine
	float MechFriction = Parameters.StartFriction + Parameters.FrictionCoefficient * AbsolutRPM;
	float PumpingLoss = Parameters.PumpingLossCoefficient * AbsolutRPM * FMath::Clamp(1.0f - State.RealThrottle, 0.f, 1.f);
	float FrictionTorque = MechFriction + PumpingLoss;

	// check if there is combustion
	bool bCombustion = State.bFuelInjection && State.bSpark;

	// torque generated inside of the engine
	const float TorqueLUT_Value = TorqueLUT.FastEval(
		AbsolutRPM,
		FVector2f(Parameters.EngineIdleRPM, Parameters.EngineMaxRPM)
	).Value;

	float IndicatedTorque =
		State.RealThrottle * (FrictionTorque + Parameters.MaxEngineTorque * TorqueLUT_Value);

	// no combustion, no torque
	IndicatedTorque *= bCombustion;

	/********************* Turbo Charging Logic *********************/

	//turbo charged and na have different behavior
	//check if engine is really turbo charged
	if (TurboConfig.MaxBoostPressure > SMALL_NUMBER && Parameters.EngineIdleRPM > 0.f)
	{
		float RPMFactor = FMath::GetMappedRangeValueClamped(
			FVector2f(0.f, TurboConfig.FullBoostRPM),
			FVector2f(0.0f, 1.0f),
			AbsolutRPM
		);

		float TargetSpool = RPMFactor * State.RealThrottle;

		float MinSpoolForPositiveBoost = (TurboConfig.FullBoostRPM > SMALL_NUMBER) ? TurboConfig.SpoolStartRPM / TurboConfig.FullBoostRPM : 0.f;

		// if blow off valve closed or antt-lag triggered, interp speed = 3.f / TurboConfig.SpoolUpDuration;
		// else, interp speed = 3.f / TurboConfig.PressureDecayDuration
		float SpoolInterpSpeed = 0.f;

		// if throttle is released, blow off valve is open, target vacuum will be 0.0
		// other wise it will be related to throttle input(smaller imput -> more vacuum)
		float ManifoldVacuum = 0.f;

		State.bIsAntiLagTriggered = TurboConfig.bEnableAntiLag
			&& TargetSpool < State.TurboSpool
			&& AbsolutRPM > TurboConfig.AntiLagMinRPM
			&& State.TurboSpool > MinSpoolForPositiveBoost;

		// if throttle input is too small, blow off valve will be opened
		// if anti-lag is triggered, the valve should not be open
		bool bIsBlowOffValveOpen = State.RawThrottleInput <= SMALL_NUMBER && !State.bIsAntiLagTriggered;

		if (bIsBlowOffValveOpen)
		{
			SpoolInterpSpeed = (TurboConfig.PressureDecayDuration > SMALL_NUMBER) ? 3.f / TurboConfig.PressureDecayDuration : 0.f;

			// if blowing off not marked and turbo spinning fast
			if (!State.bIsTurboBlowingOff && State.TurboSpool > MinSpoolForPositiveBoost)
			{
				// trigger blow off callbacks in next game tick
				State.bIsTurboBlowingOff = true;
				bShouldTriggerTurboBlowOffCallback = true;
			}
		}
		else
		{
			State.bIsTurboBlowingOff = false;

			// if throttle is hit, blow off valve close
			SpoolInterpSpeed = (TurboConfig.SpoolUpDuration > SMALL_NUMBER) ? 3.f / TurboConfig.SpoolUpDuration : 0.f;

			const float StaticIntakeRestriction = FMath::GetMappedRangeValueClamped(
				FVector2f(0.f, Parameters.EngineIdleRPM),
				FVector2f(0.f, TurboConfig.StaticIntakeRestriction),
				State.EngineRPM
			);

			if (State.bIsAntiLagTriggered)
			{
				// inject fuel during exhaust stroke
				State.bFuelInjection = true;

				float ALS_TargetSpool = FMath::Sqrt(TurboConfig.AntiLagTargetPressureRatio);
				TargetSpool = FMath::Min(State.TurboSpool, ALS_TargetSpool);
			}
			else
			{
				// if the turbo rpm is too low, the intake will have restriction
				// we estimate the max restriction as vacuum pressure
				float CurrentIntakeRestriction = FMath::GetMappedRangeValueClamped(
					FVector2f(0.f, MinSpoolForPositiveBoost),
					FVector2f(StaticIntakeRestriction, 0.f),
					State.TurboSpool
				);

				ManifoldVacuum = FMath::Lerp(StaticIntakeRestriction, CurrentIntakeRestriction, State.RawThrottleInput);
			}
		}

		// get spool
		State.TurboSpool = FMath::FInterpTo(
			State.TurboSpool,
			TargetSpool,
			DeltaTime,
			SpoolInterpSpeed
		);

		// get turbo boost
		State.TurboSpool = FMath::Clamp(State.TurboSpool, 0.f, 1.f);
		float SpoolSqr = State.TurboSpool * State.TurboSpool;
		float CurrentBoost = FMath::GetMappedRangeValueClamped(
			FVector2f(MinSpoolForPositiveBoost * MinSpoolForPositiveBoost, 1.f),
			FVector2f(0.0f, TurboConfig.MaxBoostPressure),
			SpoolSqr
		);

		// get target pressure and get current pressure
		float TargetPressure = CurrentBoost + ManifoldVacuum;
		float PressureBuildSpeed = 3.f / 0.1f; // 3 / PressureBuildTime
		State.TurboPressure = FMath::FInterpTo(
			State.TurboPressure,
			TargetPressure,
			DeltaTime,
			PressureBuildSpeed
		);

		//get indicated torque
		float TorqueMultiplier = 1.f + State.TurboPressure * TurboConfig.BoostEfficiency;
		IndicatedTorque *= FMath::Max(SMALL_NUMBER, TorqueMultiplier);
	}
	else
	{
		// clear all cache if not turbo charged
		State.bIsTurboBlowingOff = false;
		State.bIsAntiLagTriggered = false;
		State.TurboPressure *= 0.9f;
		State.TurboSpool *= 0.9f;
	}

	/*************** Calculation of Angular Velocity ***************/

	// consider P1 motor
	IndicatedTorque += State.P1MotorTorque;

	// accelerate engine
	State.EngineAngularVelocity += VehicleDrivetrainMath::DivideOrDefault(DeltaTime, Parameters.EngineInertia) * (IndicatedTorque - State.LoadTorque + State.StarterMotorTorque);

	// get the direction of friction torque
	float AngVelSignWithoutFriction = FMath::Sign(State.EngineAngularVelocity);
	FrictionTorque = FrictionTorque * AngVelSignWithoutFriction;
	State.EngineAngularVelocity -= VehicleDrivetrainMath::DivideOrDefault(DeltaTime, Parameters.EngineInertia) * FrictionTorque;

	// zero cross check
	bool bCrossZero = FMath::Sign(State.EngineAngularVelocity) != AngVelSignWithoutFriction;
	if (bCrossZero)State.EngineAngularVelocity = 0.f;

	// get engine rpm
	const float RadToRPM = 30 / PI;
	State.EngineRPM = RadToRPM * State.EngineAngularVelocity;

	// get the effective engine torque (the power)
	State.EffectiveTorque = IndicatedTorque - FrictionTorque;
}

void FVehicleEngineSolver::UpdateExhaust(const FVehicleEngineParameters& Parameters,
	const FVehicleEngineExhaustConfig& ExhaustConfig, FVehicleEngineSimState& State,
	const float DeltaTime, bool& bShouldTriggerBackfiringCallback)
{
	// absolut engine rpm
	float AbsolutRPM = FMath::Abs(State.EngineRPM);

	// normalized rpm
	float NormalizedRPM = VehicleDrivetrainMath::DivideOrDefault(AbsolutRPM, Parameters.EngineMaxRPM);

	// check if engine is slowing down (and not idling)
	bool bIsDecelerating =
		State.EffectiveTorque < 0.f
		&& AbsolutRPM >(Parameters.EngineIdleRPM * 1.5f)
		&& AbsolutRPM < Parameters.EngineMaxRPM;

	// get the AFR or lambda
	float TargetLamda = bIsDecelerating ?
		Parameters.DeceleratingLambda :
		FMath::GetMappedRangeValueClamped(
			FVector2f(0.8f, 0.9f),
			FVector2f(1.f, Parameters.MaxPowerLambda),
			State.RawThrottleInput
		);

	// if there is fuel injection, do the backfiring logic
	if (State.bFuelInjection)
	{
		// approximate fuel flow
		float BaseFuelFlow = State.bIsAntiLagTriggered ?
			State.TurboSpool * State.TurboSpool : NormalizedRPM * State.RealThrottle;

		// if spark is off, or if anti-lag is on
		// all the fuel will be sent to the exhaust
		bool bAllInExhaust = !State.bSpark || State.bIsAntiLagTriggered;

		float UnburntFuel = bAllInExhaust ?
			BaseFuelFlow : BaseFuelFlow * FMath::Max(0.f, 1.f - TargetLamda);

		// accumulate unburnt fuel
		State.UnburntFuelBuffer += Parameters.UnburntFuelAccumulationRate * UnburntFuel * DeltaTime;
	}

	// higher lambda or higher rpm causes the unburnt fuel decay faster
	float UnBurntFuelDecayAmount = ExhaustConfig.ExhaustScavengingStrength * State.UnburntFuelBuffer * TargetLamda * NormalizedRPM;

	// update unburnt fuel
	State.UnburntFuelBuffer -= UnBurntFuelDecayAmount * DeltaTime;

	// get target exhaust heat
	// high rpm & lot of throttle = heat
	float TargetHeat = NormalizedRPM * State.RealThrottle;

	// get interp speed
	float HeatInterpSpeed = TargetHeat > State.ExhaustHeat ?
		ExhaustConfig.HeatUpRate : ExhaustConfig.CoolDownRate;

	// update exhaust heat
	State.ExhaustHeat = FMath::FInterpTo(
		State.ExhaustHeat,
		TargetHeat,
		DeltaTime,
		HeatInterpSpeed
	);

	State.BackfireIntensity = 0.f;

	if (State.ExhaustHeat > ExhaustConfig.FlashPoint && State.UnburntFuelBuffer > ExhaustConfig.PopFuelThreshold)
	{
		float Intensity = FMath::GetMappedRangeValueUnclamped(
			FVector2D(
				ExhaustConfig.PopFuelThreshold,
				ExhaustConfig.FlameFuelThreshold
			),
			FVector2D(0.f, 1.f),
			State.UnburntFuelBuffer
		);

		Intensity = FMath::Max(0.f, Intensity);

		float Chance = ExhaustConfig.IgnitionProbability * Intensity;

		if (FMath::FRand() < Chance)
		{
			State.BackfireIntensity = Intensity;
			State.UnburntFuelBuffer -= State.UnburntFuelBuffer * Intensity;
			float HeatAdded = ExhaustConfig.BackfireHeatSpike * Intensity;
			State.ExhaustHeat += HeatAdded;
			bShouldTriggerBackfiringCallback = true;
		}
	}

	// clamp the value
	State.UnburntFuelBuffer = FMath::Max(0.f, State.UnburntFuelBuffer);
	State.ExhaustHeat = FMath::Clamp(State.ExhaustHeat, 0.f, 1.f);
}

float FVehicleEngineSolver::EstimateThrottleForTorque(const FVehicleEngineParameters& Parameters,
	const FVehicleEngineSimState& State, const FVehicleLUT<32>& TorqueLUT,
	const float TargetEffectiveTorque)
{
	const float RPM = FMath::Abs(State.EngineRPM);

	const float MechFriction = Parameters.StartFriction + Parameters.FrictionCoefficient * RPM;
	const float PumpingLossAtClosedThrottle = Parameters.PumpingLossCoefficient * RPM;
	const float TorqueCurveValue = TorqueLUT.FastEval(RPM,
		FVector2f(Parameters.EngineIdleRPM, Parameters.EngineMaxRPM)).Value;
	const float CurveTorque = Parameters.MaxEngineTorque * TorqueCurveValue;

	// We want:
	// EffectiveTorque = IndicatedTorque - FrictionTorque = TargetEffectiveTorque
	//
	// FrictionTorque(t) = MechFriction + PumpingLossAtClosedThrottle * (1 - t)
	// IndicatedTorque(t) = t * (FrictionTorque(t) + CurveTorque)
	//
	// Solve:
	// t * (MechFriction + PumpingLossAtClosedThrottle * (1 - t) + CurveTorque)
	// - (MechFriction + PumpingLossAtClosedThrottle * (1 - t))
	// = TargetEffectiveTorque

	const float P = PumpingLossAtClosedThrottle;
	const float B = MechFriction + CurveTorque + 2.f * P;
	const float C = TargetEffectiveTorque + MechFriction + P;
	float RequiredThrottle = 1.f;

	if (P > SMALL_NUMBER)
	{
		const float Discriminant = B * B - 4.f * P * C;
		if (Discriminant > 0.f)
		{
			// smaller root is the physically useful one in [0, 1]
			RequiredThrottle = (B - FMath::Sqrt(Discriminant)) / (2.f * P);
		}
	}
	else
	{
		// no pumping-loss dependency on throttle
		RequiredThrottle = VehicleDrivetrainMath::DivideOrDefault(C, MechFriction + CurveTorque, 1.f);
	}

	return FMath::Clamp(RequiredThrottle, 0.f, 1.f);
}

void FVehicleEngineSolver::UpdatePhysics(const FVehicleEngineParameters& Parameters,
	const FVehicleEngineTurboConfig& TurboConfig, const FVehicleEngineExhaustConfig& ExhaustConfig,
	const FVehicleLUT<32>& TorqueLUT, FVehicleEngineSimState& State,
	const float DeltaTime, const float Throttle, const float LoadTorque, const bool bDisableSpark,
	bool& bShouldTriggerTurboBlowOffCallback, bool& bShouldTriggerBackfiringCallback)
{
	State.RawThrottleInput = FMath::Clamp(Throttle, 0.f, 1.f);
	State.LoadTorque = LoadTorque;

	// get torque required to start engine
	State.TorqueRequiredToStartEngine = Parameters.StartFriction + Parameters.EngineIdleRPM * Parameters.FrictionCoefficient;

	//check if idle rpm is valid
	if (Parameters.EngineIdleRPM > 0)
	{
		// 0.7 is a magic number
		State.EngineOffRPM = 0.7f * Parameters.EngineIdleRPM;

		//get idle throttle
		State.IdleThrottle = FMath::Clamp(
			VehicleDrivetrainMath::DivideOrDefault(State.TorqueRequiredToStartEngine,
				State.TorqueRequiredToStartEngine + Parameters.MaxEngineTorque * TorqueLUT.FastEval(0.f).Value),
			0.f, 1.f);
	}
	else
	{
		State.IdleThrottle = 0;
		State.EngineOffRPM = -BIG_NUMBER;
	}

	// true if rpm > max rpm
	bool bRevLimit = State.EngineRPM > Parameters.EngineMaxRPM;

	switch (State.OperationMode)
	{
	case EVehicleEngineOperationMode::On:
		// enable fuel injection and spark if there's throttle input
		// the fuel injection and spark might be disabled in certain conditions
		State.bFuelInjection = State.RawThrottleInput > SMALL_NUMBER;

		// update revlimit timer
		State.RevLimiterTimer = bRevLimit ? 0.f : State.RevLimiterTimer + DeltaTime;
		// avoid overflow
		// avoid rapid toggling due to RevLimiterTime being too short (not unit conversion)
		State.RevLimiterTimer = FMath::Min(State.RevLimiterTimer, Parameters.RevLimiterTime * 10.f);

		// if rpm is smaller than max rpm and the timer fullfills the timer limit
		State.bSpark = !bDisableSpark && !bRevLimit && State.RevLimiterTimer >= Parameters.RevLimiterTime;

		//check if engine is idling
		if (State.EngineRPM < Parameters.EngineIdleRPM && Parameters.EngineIdleRPM > 0.f)
		{
			// if rpm is too low
			// interp the throttle to 1
			// and enable spark and fuel injection
			// to maintain the rpm
			State.RealThrottle = FMath::FInterpTo(State.RealThrottle, 1.f, DeltaTime,
				Parameters.IdleThrottleInterpSpeed);

			const float PositiveLoadTorque = FMath::Max(0.f, State.LoadTorque);
			const float RequiredThrottleByLoad = EstimateThrottleForTorque(
				Parameters, State, TorqueLUT, PositiveLoadTorque);
			State.RealThrottle = FMath::Max(RequiredThrottleByLoad, State.RealThrottle);
			State.bFuelInjection = State.bSpark = true;
		}
		else
		{
			const float FloatingErrorTolerance = 1e-3f;
			// get real throttle value
			State.RealThrottle = FMath::Lerp(State.IdleThrottle - FloatingErrorTolerance, 1.f, State.RawThrottleInput);
		}

		// check if engine is off
		if (State.EngineRPM <= State.EngineOffRPM)
		{
			State.OperationMode = EVehicleEngineOperationMode::Off;
			State.bFuelInjection = State.bSpark = false;
		}
		break;
	case EVehicleEngineOperationMode::Off:
		State.bFuelInjection = State.bSpark = false;
		if (State.EngineRPM > State.EngineOffRPM)
		{
			State.OperationMode = EVehicleEngineOperationMode::On;
			State.bFuelInjection = State.bSpark = true;
		}
		break;
	case EVehicleEngineOperationMode::Starting:
		State.RealThrottle = FMath::Lerp(State.IdleThrottle, 1.f, State.RawThrottleInput);
		State.bFuelInjection = State.bSpark = true;
		//check if engine is started
		if (State.EngineRPM > FMath::Min(1.5 * Parameters.EngineIdleRPM, 0.9 * Parameters.EngineMaxRPM))
		{
			State.OperationMode = EVehicleEngineOperationMode::On;
			State.StarterMotorTorque = 0.f;
		}
		else
		{
			State.StarterMotorTorque += DeltaTime * State.TorqueRequiredToStartEngine * 10;
		}
		break;
	case EVehicleEngineOperationMode::Shutting:
		State.StarterMotorTorque = 0.f;
		State.bFuelInjection = false;
		if (State.EngineRPM <= State.EngineOffRPM)
		{
			State.OperationMode = EVehicleEngineOperationMode::Off;
		}
		break;
	default:
		State.OperationMode = EVehicleEngineOperationMode::On;
		break;
	}

	EngineAcceleration(Parameters, TurboConfig, TorqueLUT, State, DeltaTime, bShouldTriggerTurboBlowOffCallback);
	UpdateExhaust(Parameters, ExhaustConfig, State, DeltaTime, bShouldTriggerBackfiringCallback);
}

EVehicleEngineOperationMode FVehicleEngineSolver::StartVehicleEngine(FVehicleEngineSimState& State)
{
	switch (State.OperationMode)
	{
	case EVehicleEngineOperationMode::On:
		break;
	case EVehicleEngineOperationMode::Off:State.OperationMode = EVehicleEngineOperationMode::Starting;
		break;
	case EVehicleEngineOperationMode::Starting:
		break;
	case EVehicleEngineOperationMode::Shutting://State = EVehicleEngineState::Starting;
		break;
	default:
		break;
	}
	return State.OperationMode;
}

EVehicleEngineOperationMode FVehicleEngineSolver::ShutdownVehicleEngine(FVehicleEngineSimState& State)
{
	switch (State.OperationMode)
	{
	case EVehicleEngineOperationMode::On:State.OperationMode = EVehicleEngineOperationMode::Shutting;
		break;
	case EVehicleEngineOperationMode::Off:
		break;
	case EVehicleEngineOperationMode::Starting://State = EVehicleEngineState::Shutting;
		break;
	case EVehicleEngineOperationMode::Shutting:
		break;
	default:
		break;
	}
	return State.OperationMode;
}
