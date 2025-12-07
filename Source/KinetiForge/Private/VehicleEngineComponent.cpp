// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)


#include "VehicleEngineComponent.h"

// Sets default values for this component's properties
UVehicleEngineComponent::UVehicleEngineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PostUpdateWork;

	// ...
	if (!NAConfig.EngineTorqueCurve)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> DefaultTorqueCurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultNaturallyAspiratedEngineTorqueCurve.DefaultNaturallyAspiratedEngineTorqueCurve'")
		);
		if (DefaultTorqueCurveObj.Succeeded())
		{
			NAConfig.EngineTorqueCurve = DefaultTorqueCurveObj.Object;
		}
	}
}


// Called when the game starts
void UVehicleEngineComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Initialize();
}


void UVehicleEngineComponent::EngineAcceleration()
{
	// absolut engine rpm
	float AbsolutRPM = FMath::Abs(SimData.EngineRPM);

	// internal friction of the engine
	float FrictionTorque = NAConfig.StartFriction + NAConfig.FrictionCoefficient * AbsolutRPM;

	// check if there is combustion
	bool bCombustion = SimData.bFuelInjection && SimData.bSpark;

	// torque generated inside of the engine
	float IndicatedTorque = 0.f;
	if (NAConfig.EngineTorqueCurve)
	{
		IndicatedTorque = SimData.RealThrottle* (FrictionTorque + NAConfig.MaxEngineTorque * NAConfig.EngineTorqueCurve->GetFloatValue(AbsolutRPM));
	}
	else
	{
		IndicatedTorque = SimData.RealThrottle * (FrictionTorque + NAConfig.MaxEngineTorque);
	}

	// no combustion, no torque
	IndicatedTorque *= bCombustion;

	/********************* Turbo Charging Logic *********************/

	//turbo charged and na have different behavior
	//check if engine is really turbo charged
	if (TurboConfig.MaxBoostPressure > SMALL_NUMBER && NAConfig.EngineIdleRPM > 0.f)
	{
		float RPMFactor = FMath::GetMappedRangeValueClamped(
			FVector2D(0.f, TurboConfig.FullBoostRPM),
			FVector2D(0.0f, 1.0f), 
			AbsolutRPM
		);

		float TargetSpool = RPMFactor * SimData.RealThrottle;

		float MinSpoolForPositiveBoost = (TurboConfig.FullBoostRPM > SMALL_NUMBER) ? TurboConfig.SpoolStartRPM / TurboConfig.FullBoostRPM : 0.f;

		// if blow off valve closed or antt-lag triggered, interp speed = 3.f / TurboConfig.SpoolUpDuration;
		// else, interp speed = 3.f / TurboConfig.PressureDecayDuration
		float SpoolInterpSpeed = 0.f;

		// if throttle is released, blow off valve is open, target vacuum will be 0.0
		// other wise it will be related to throttle input(smaller imput -> more vacuum)
		float ManifoldVacuum = 0.f;

		SimData.bIsAntiLagTriggered = TurboConfig.bEnableAntiLag
			&& TargetSpool < SimData.TurboSpool
			&& AbsolutRPM > TurboConfig.AntiLagMinRPM
			&& SimData.TurboSpool > MinSpoolForPositiveBoost;

		// if throttle input is too small, blow off valve will be opened
		// if anti-lag is triggered, the valve should not be open
		bool bIsBlowOffValveOpen = SimData.RawThrottleInput <= SMALL_NUMBER && !SimData.bIsAntiLagTriggered;

		if (bIsBlowOffValveOpen)
		{
			SpoolInterpSpeed = (TurboConfig.PressureDecayDuration > SMALL_NUMBER) ? 3.f / TurboConfig.PressureDecayDuration : 0.f;

			// if blowing off not marked and turbo spinning fast
			if (!SimData.bIsTurboBlowingOff && SimData.TurboSpool > MinSpoolForPositiveBoost)
			{
				// trigger blow off callbacks in next game tick
				SimData.bIsTurboBlowingOff = true;
				bShouldTriggerTurboBlowOffCallback = true;
			}
		}
		else
		{
			SimData.bIsTurboBlowingOff = false;

			// if throttle is hit, blow off valve close
			SpoolInterpSpeed = (TurboConfig.SpoolUpDuration > SMALL_NUMBER) ? 3.f / TurboConfig.SpoolUpDuration : 0.f;

			if (SimData.bIsAntiLagTriggered)
			{
				// inject fuel during exhaust stroke
				SimData.bFuelInjection = true;

				float ALS_TargetSpool = FMath::Sqrt(TurboConfig.AntiLagTargetPressureRatio);
				TargetSpool = FMath::Min(SimData.TurboSpool, ALS_TargetSpool);
			}
			else
			{
				// if the turbo rpm is too low, the intake will have restriction
				// we estimate the max restriction as vacuum pressure
				float CurrentIntakeRestriction = FMath::GetMappedRangeValueClamped(
					FVector2D(0.f, MinSpoolForPositiveBoost),
					FVector2D(TurboConfig.StaticIntakeRestriction, 0.f),
					SimData.TurboSpool
				);

				ManifoldVacuum = FMath::Lerp(TurboConfig.StaticIntakeRestriction, CurrentIntakeRestriction, SimData.RawThrottleInput);
			}
		}

		// get spool
		SimData.TurboSpool = FMath::FInterpTo(
			SimData.TurboSpool, 
			TargetSpool, 
			SimData.PhysicsDeltaTime, 
			SpoolInterpSpeed
		);

		// get turbo boost
		SimData.TurboSpool = FMath::Clamp(SimData.TurboSpool, 0.f, 1.f);
		float SpoolSqr = SimData.TurboSpool * SimData.TurboSpool;
		float CurrentBoost = FMath::GetMappedRangeValueClamped(
			FVector2D(MinSpoolForPositiveBoost * MinSpoolForPositiveBoost, 1.f),
			FVector2D(0.0f, TurboConfig.MaxBoostPressure),
			SpoolSqr
		);

		// get target pressure and get current pressure
		float TargetPressure = CurrentBoost + ManifoldVacuum;
		float PressureBuildSpeed = 3.f / 0.1f; // 3 / PressureBuildTime
		SimData.TurboPressure = FMath::FInterpTo(
			SimData.TurboPressure,
			TargetPressure,
			SimData.PhysicsDeltaTime,
			PressureBuildSpeed
		);

		//get indicated torque
		float TorqueMultiplier = 1.f + SimData.TurboPressure * TurboConfig.BoostEfficiency;
		IndicatedTorque *= FMath::Max(SMALL_NUMBER, TorqueMultiplier);
	}
	else
	{
		// clear all cache if not turbo charged
		SimData.bIsTurboBlowingOff = false;
		SimData.bIsAntiLagTriggered = false;
		SimData.TurboPressure *= 0.9f;
		SimData.TurboSpool *= 0.9f;
	}

	/*************** Calculation of Angular Velocity ***************/

	// consider P1 motor
	IndicatedTorque += SimData.P1MotorTorque;

	// accelerate engine
	SimData.EngineAngularVelocity += SafeDivide(SimData.PhysicsDeltaTime, NAConfig.EngineInertia) * (IndicatedTorque - SimData.LoadTorque + SimData.StarterMotorTorque);
	
	// get the direction of friction torque
	float AngVelSignWithoutFriction = FMath::Sign(SimData.EngineAngularVelocity);
	FrictionTorque = FrictionTorque * AngVelSignWithoutFriction;
	SimData.EngineAngularVelocity -= SafeDivide(SimData.PhysicsDeltaTime, NAConfig.EngineInertia) * FrictionTorque;
	
	// zero cross check
	bool bCrossZero = FMath::Sign(SimData.EngineAngularVelocity) != AngVelSignWithoutFriction;
	if (bCrossZero)SimData.EngineAngularVelocity = 0.f;
	
	// get engine rpm
	SimData.EngineRPM = RadToRPM * SimData.EngineAngularVelocity;
	
	// get the effective engine torque (the power)
	SimData.EffectiveTorque = IndicatedTorque - FrictionTorque;
}

void UVehicleEngineComponent::UpdateExhaust()
{
	// absolut engine rpm
	float AbsolutRPM = FMath::Abs(SimData.EngineRPM);

	// normalized rpm
	float NormalizedRPM = SafeDivide(AbsolutRPM, NAConfig.EngineMaxRPM);

	// check if engine is slowing down (and not idling)
	bool bIsDecelerating =
		SimData.EffectiveTorque < 0.f
		&& AbsolutRPM >(NAConfig.EngineIdleRPM * 1.5f)
		&& AbsolutRPM < NAConfig.EngineMaxRPM;

	// get the AFR or lambda
	float TargetLamda = bIsDecelerating ?
		NAConfig.DeceleratingLambda :
		FMath::GetMappedRangeValueClamped(
			FVector2D(0.8f, 0.9f),
			FVector2D(1.f, NAConfig.MaxPowerLambda),
			SimData.RawThrottleInput
		);

	// if there is fuel injection, do the backfiring logic
	if (SimData.bFuelInjection)
	{
		// approximate fuel flow
		float BaseFuelFlow = SimData.bIsAntiLagTriggered ? 
			SimData.TurboSpool * SimData.TurboSpool : NormalizedRPM * SimData.RealThrottle;

		// if spark is off, or if anti-lag is on
		// all the fuel will be sent to the exhaust
		bool bAllInExhaust = !SimData.bSpark || SimData.bIsAntiLagTriggered;

		float UnburntFuel = bAllInExhaust ?
			BaseFuelFlow : BaseFuelFlow * FMath::Max(0.f, 1.f - TargetLamda);

		// accumulate unburnt fuel
		SimData.UnburntFuelBuffer += NAConfig.UnburntFuelAccumulationRate * UnburntFuel * SimData.PhysicsDeltaTime;
	}

	// higher lambda or higher rpm causes the unburnt fuel decay faster
	float UnBurntFuelDecayAmount = ExhaustConfig.ExhaustScavengingStrength * SimData.UnburntFuelBuffer * TargetLamda * NormalizedRPM;

	// update unburnt fuel
	SimData.UnburntFuelBuffer -= UnBurntFuelDecayAmount * SimData.PhysicsDeltaTime;

	// get target exhaust heat
	// high rpm & lot of throttle = heat
	float TargetHeat = NormalizedRPM * SimData.RealThrottle;

	// get interp speed 
	float HeatInterpSpeed = TargetHeat > SimData.ExhaustHeat ? 
		ExhaustConfig.HeatUpRate : ExhaustConfig.CoolDownRate;

	// update exhaust heat
	SimData.ExhaustHeat = FMath::FInterpTo(
		SimData.ExhaustHeat,
		TargetHeat,
		SimData.PhysicsDeltaTime,
		HeatInterpSpeed
	);

	SimData.BackfireIntensity = 0.f;

	if (SimData.ExhaustHeat > ExhaustConfig.FlashPoint && SimData.UnburntFuelBuffer > ExhaustConfig.PopFuelThreshold)
	{
		float Intensity = FMath::GetMappedRangeValueUnclamped(
			FVector2D(
				ExhaustConfig.PopFuelThreshold, 
				ExhaustConfig.FlameFuelThreshold
			),
			FVector2D(0.f, 1.f),
			SimData.UnburntFuelBuffer
		);

		Intensity = FMath::Max(0.f, Intensity);

		float Chance = ExhaustConfig.IgnitionProbability * Intensity;

		if (FMath::FRand() < Chance)
		{
			SimData.BackfireIntensity = Intensity;
			SimData.UnburntFuelBuffer -= SimData.UnburntFuelBuffer * Intensity;
			float HeatAdded = ExhaustConfig.BackfireHeatSpike * Intensity;
			SimData.ExhaustHeat += HeatAdded;
			bShouldTriggerBackfiringCallback = true;
		}
	}

	// clamp the value
	SimData.UnburntFuelBuffer = FMath::Max(0.f, SimData.UnburntFuelBuffer);
	SimData.ExhaustHeat = FMath::Clamp(SimData.ExhaustHeat, 0.f, 1.f);
}

float UVehicleEngineComponent::SafeDivide(float a, float b)
{
	return (FMath::IsNearlyZero(b)) ? 0.0f : a / b;
}

// Called every frame
void UVehicleEngineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (CachedEngineIdleRPM != NAConfig.EngineIdleRPM || 
		CachedStartFriction != NAConfig.StartFriction ||
		CachedFrictionCoefficient != NAConfig.FrictionCoefficient ||
		CachedMaxEngineTorque != NAConfig.MaxEngineTorque)
	{
		Initialize();
	}

	if (bShouldTriggerTurboBlowOffCallback)
	{
		bShouldTriggerTurboBlowOffCallback = false;
		for (FOnTurboBlowOffDelegate BlowOffCallback : TurboBlowOffCallbacks)
		{
			if (BlowOffCallback.IsBound())
			{
				BlowOffCallback.Execute();
			}
			else
			{
				TurboBlowOffCallbacks.Remove(BlowOffCallback);
			}
		}
	}

	if (bShouldTriggerBackfiringCallback)
	{
		bShouldTriggerBackfiringCallback = false;
		for (FOnBackfiringDelegate BackfiringCallback : BackfiringCallbacks)
		{
			if (BackfiringCallback.IsBound())
			{
				BackfiringCallback.Execute();
			}
			else
			{
				BackfiringCallbacks.Remove(BackfiringCallback);
			}
		}
	}
}

void UVehicleEngineComponent::UpdatePhysics(float InDeltaTime, float InThrottle, float InLoadTorque)
{
	SimData.PhysicsDeltaTime = InDeltaTime;
	SimData.RawThrottleInput = FMath::Clamp(InThrottle, 0.f, 1.f);
	SimData.LoadTorque = InLoadTorque;

	//update revlimit timer
	SimData.RevLimiterTimer += SimData.PhysicsDeltaTime;
	//avoid overflow
	// avoid rapid toggling due to RevLimiterTime being too short (not unit conversion)
	SimData.RevLimiterTimer = FMath::Min(SimData.RevLimiterTimer, NAConfig.RevLimiterTime * 99999);

	switch (SimData.State)
	{
	case EVehicleEngineState::On:
		// get real throttle value
		SimData.RealThrottle = FMath::Lerp(SimData.IdleThrottle, 1.f, SimData.RawThrottleInput);

		// enable fuel injection and spark if there's throttle input
		// the fuel injection and spark might be disabled in certain conditions
		SimData.bFuelInjection = SimData.RawThrottleInput > SMALL_NUMBER;
		SimData.bSpark = true;

		if (SimData.EngineRPM > NAConfig.EngineMaxRPM)
		{
			//cut power(disable spark) at max rpm
			// but keep fuel injection, because we want back fireing
			SimData.bSpark = false;
			SimData.RevLimiterTimer = 0.f;
		}
		else
		{
			// if rpm is smaller than max rpm and the timer fullfills the timer limit
			SimData.bSpark = SimData.RevLimiterTimer >= NAConfig.RevLimiterTime;
		}

		//check if throttle is released and if idle is valid
		if (SimData.RawThrottleInput < SMALL_NUMBER && NAConfig.EngineIdleRPM > 0.f)
		{
			//check if engine is idling
			if (SimData.EngineRPM < NAConfig.EngineIdleRPM)
			{
				// if rpm is too low
				// interp the throttle to 1
				// and enable spark and fuel injection
				// to maintain the rpm
				SimData.RealThrottle = FMath::FInterpTo(
					SimData.RealThrottle, 
					1.f, 
					SimData.PhysicsDeltaTime, 
					NAConfig.IdleThrottleInterpSpeed);

				SimData.bFuelInjection = SimData.bSpark = true;
			}
			else
			{
				// if rpm is higher than idle rpm
				// interp the trottle to 0
				// because sometimes the idle throttle is slighly too large
				// (Due to floating-point precision)
				SimData.RealThrottle = FMath::FInterpTo(
					SimData.RealThrottle, 
					0.f, 
					SimData.PhysicsDeltaTime, 
					NAConfig.IdleThrottleInterpSpeed);

				SimData.bFuelInjection = false;
			}
		}

		// check if engine is off
		if (SimData.EngineRPM <= SimData.EngineOffRPM)
		{
			SimData.State = EVehicleEngineState::Off;
			SimData.bFuelInjection = SimData.bSpark = false;
		}
		break;
	case EVehicleEngineState::Off:
		SimData.bFuelInjection = SimData.bSpark = false;
		if (SimData.EngineRPM > SimData.EngineOffRPM)
		{
			SimData.State = EVehicleEngineState::On;
			SimData.bFuelInjection = SimData.bSpark = true;
		}
		break;
	case EVehicleEngineState::Starting:
		SimData.RealThrottle = FMath::Lerp(SimData.IdleThrottle, 1.f, SimData.RawThrottleInput);
		SimData.bFuelInjection = SimData.bSpark = true;
		//check if engine is started
		if (SimData.EngineRPM > FMath::Min(1.5 * NAConfig.EngineIdleRPM, 0.9 * NAConfig.EngineMaxRPM))
		{
			SimData.State = EVehicleEngineState::On;
			SimData.StarterMotorTorque = 0.f;
		}
		else
		{
			SimData.StarterMotorTorque += SimData.PhysicsDeltaTime * SimData.TorqueRequiredToStartEngine * 10;
		}
		break;
	case EVehicleEngineState::Shutting:
		SimData.StarterMotorTorque = 0.f;
		SimData.bFuelInjection = false;
		if (SimData.EngineRPM <= SimData.EngineOffRPM)
		{
			SimData.State = EVehicleEngineState::Off;
		}
		break;
	default:
		SimData.State = EVehicleEngineState::On;
		break;
	}

	EngineAcceleration();
	UpdateExhaust();
}

void UVehicleEngineComponent::Initialize()
{
	//get required torque to start engine
	NAConfig.StartFriction = FMath::Max(NAConfig.StartFriction, SMALL_NUMBER);
	NAConfig.FrictionCoefficient = FMath::Max(NAConfig.FrictionCoefficient, SMALL_NUMBER);
	SimData.TorqueRequiredToStartEngine = NAConfig.StartFriction + NAConfig.EngineIdleRPM * NAConfig.FrictionCoefficient;
	//get idle throttle
	if (NAConfig.EngineTorqueCurve)
	{
		SimData.IdleThrottle = FMath::Clamp(
			SafeDivide(SimData.TorqueRequiredToStartEngine,
				SimData.TorqueRequiredToStartEngine + NAConfig.MaxEngineTorque * NAConfig.EngineTorqueCurve->GetFloatValue(NAConfig.EngineIdleRPM)),
			0.f, 1.f);
	}
	else
	{
		SimData.IdleThrottle = FMath::Clamp(
			SafeDivide(SimData.TorqueRequiredToStartEngine,
				SimData.TorqueRequiredToStartEngine + NAConfig.MaxEngineTorque), 0.f, 1.f);
	}

	//check if idle rpm is valid
	if (NAConfig.EngineIdleRPM > 0)
	{
		SimData.EngineOffRPM = 0.7 * NAConfig.EngineIdleRPM;
	}
	else
	{
		SimData.IdleThrottle = 0;
		SimData.EngineOffRPM = -BIG_NUMBER;
	}

	// reset rev-limiter
	SimData.RevLimiterTimer = NAConfig.RevLimiterTime;
}

EVehicleEngineState UVehicleEngineComponent::StartVehicleEngine()
{
	switch (SimData.State)
	{
	case EVehicleEngineState::On:
		break;
	case EVehicleEngineState::Off:SimData.State = EVehicleEngineState::Starting;
		break;
	case EVehicleEngineState::Starting:
		break;
	case EVehicleEngineState::Shutting://State = EVehicleEngineState::Starting;
		break;
	default:
		break;
	}

	return SimData.State;
}

EVehicleEngineState UVehicleEngineComponent::ShutVehicleEngine()
{
	switch (SimData.State)
	{
	case EVehicleEngineState::On:SimData.State = EVehicleEngineState::Shutting;
		break;
	case EVehicleEngineState::Off:
		break;
	case EVehicleEngineState::Starting://State = EVehicleEngineState::Shutting;
		break;
	case EVehicleEngineState::Shutting:
		break;
	default:
		break;
	}

	return SimData.State;
}

void UVehicleEngineComponent::BindEventToOnTurboBlowOff(FOnTurboBlowOffDelegate InOnTurboBlowOff)
{
	if (TurboBlowOffCallbacks.Find(InOnTurboBlowOff) == INDEX_NONE)
	{
		TurboBlowOffCallbacks.Add(InOnTurboBlowOff);
	}
}

void UVehicleEngineComponent::BindEventToOnBackfiring(FOnBackfiringDelegate InOnBackFiring)
{
	if (BackfiringCallbacks.Find(InOnBackFiring) == INDEX_NONE)
	{
		BackfiringCallbacks.Add(InOnBackFiring);
	}
}
