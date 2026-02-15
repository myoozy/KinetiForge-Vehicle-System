// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)


#include "VehicleEngineComponent.h"
#include "VehicleUtil.h"

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

	TimeSinceLastConfigSync = FMath::FRandRange(0.f, ConfigSyncInterval);
}


void UVehicleEngineComponent::EngineAcceleration()
{
	// absolut engine rpm
	float AbsolutRPM = FMath::Abs(State.EngineRPM);

	// internal friction of the engine
	float FrictionTorque = NAConfig.StartFriction + NAConfig.FrictionCoefficient * AbsolutRPM;

	// check if there is combustion
	bool bCombustion = State.bFuelInjection && State.bSpark;

	// torque generated inside of the engine
	float IndicatedTorque = 
		State.RealThrottle * (FrictionTorque + NAConfig.MaxEngineTorque * CachedTorqueCurve.Eval(AbsolutRPM));
	
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
					FVector2D(0.f, MinSpoolForPositiveBoost),
					FVector2D(TurboConfig.StaticIntakeRestriction, 0.f),
					State.TurboSpool
				);

				ManifoldVacuum = FMath::Lerp(TurboConfig.StaticIntakeRestriction, CurrentIntakeRestriction, State.RawThrottleInput);
			}
		}

		// get spool
		State.TurboSpool = FMath::FInterpTo(
			State.TurboSpool, 
			TargetSpool, 
			State.PhysicsDeltaTime, 
			SpoolInterpSpeed
		);

		// get turbo boost
		State.TurboSpool = FMath::Clamp(State.TurboSpool, 0.f, 1.f);
		float SpoolSqr = State.TurboSpool * State.TurboSpool;
		float CurrentBoost = FMath::GetMappedRangeValueClamped(
			FVector2D(MinSpoolForPositiveBoost * MinSpoolForPositiveBoost, 1.f),
			FVector2D(0.0f, TurboConfig.MaxBoostPressure),
			SpoolSqr
		);

		// get target pressure and get current pressure
		float TargetPressure = CurrentBoost + ManifoldVacuum;
		float PressureBuildSpeed = 3.f / 0.1f; // 3 / PressureBuildTime
		State.TurboPressure = FMath::FInterpTo(
			State.TurboPressure,
			TargetPressure,
			State.PhysicsDeltaTime,
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
	State.EngineAngularVelocity += UVehicleUtil::SafeDivide(State.PhysicsDeltaTime, NAConfig.EngineInertia) * (IndicatedTorque - State.LoadTorque + State.StarterMotorTorque);
	
	// get the direction of friction torque
	float AngVelSignWithoutFriction = FMath::Sign(State.EngineAngularVelocity);
	FrictionTorque = FrictionTorque * AngVelSignWithoutFriction;
	State.EngineAngularVelocity -= UVehicleUtil::SafeDivide(State.PhysicsDeltaTime, NAConfig.EngineInertia) * FrictionTorque;
	
	// zero cross check
	bool bCrossZero = FMath::Sign(State.EngineAngularVelocity) != AngVelSignWithoutFriction;
	if (bCrossZero)State.EngineAngularVelocity = 0.f;
	
	// get engine rpm
	State.EngineRPM = RadToRPM * State.EngineAngularVelocity;
	
	// get the effective engine torque (the power)
	State.EffectiveTorque = IndicatedTorque - FrictionTorque;
}

void UVehicleEngineComponent::UpdateExhaust()
{
	// absolut engine rpm
	float AbsolutRPM = FMath::Abs(State.EngineRPM);

	// normalized rpm
	float NormalizedRPM = UVehicleUtil::SafeDivide(AbsolutRPM, NAConfig.EngineMaxRPM);

	// check if engine is slowing down (and not idling)
	bool bIsDecelerating =
		State.EffectiveTorque < 0.f
		&& AbsolutRPM >(NAConfig.EngineIdleRPM * 1.5f)
		&& AbsolutRPM < NAConfig.EngineMaxRPM;

	// get the AFR or lambda
	float TargetLamda = bIsDecelerating ?
		NAConfig.DeceleratingLambda :
		FMath::GetMappedRangeValueClamped(
			FVector2D(0.8f, 0.9f),
			FVector2D(1.f, NAConfig.MaxPowerLambda),
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
		State.UnburntFuelBuffer += NAConfig.UnburntFuelAccumulationRate * UnburntFuel * State.PhysicsDeltaTime;
	}

	// higher lambda or higher rpm causes the unburnt fuel decay faster
	float UnBurntFuelDecayAmount = ExhaustConfig.ExhaustScavengingStrength * State.UnburntFuelBuffer * TargetLamda * NormalizedRPM;

	// update unburnt fuel
	State.UnburntFuelBuffer -= UnBurntFuelDecayAmount * State.PhysicsDeltaTime;

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
		State.PhysicsDeltaTime,
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

// Called every frame
void UVehicleEngineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	TimeSinceLastConfigSync += DeltaTime;
	if (TimeSinceLastConfigSync > ConfigSyncInterval)
	{
		TimeSinceLastConfigSync -= ConfigSyncInterval;

		UpdateCachedRichCurve();

		if (CachedEngineIdleRPM != NAConfig.EngineIdleRPM ||
			CachedStartFriction != NAConfig.StartFriction ||
			CachedFrictionCoefficient != NAConfig.FrictionCoefficient ||
			CachedMaxEngineTorque != NAConfig.MaxEngineTorque)
		{
			Initialize();
		}
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

void UVehicleEngineComponent::UpdatePhysics(float InDeltaTime, float InThrottle, float InLoadTorque, bool bDisableSpark)
{
	State.PhysicsDeltaTime = InDeltaTime;
	State.RawThrottleInput = FMath::Clamp(InThrottle, 0.f, 1.f);
	State.LoadTorque = InLoadTorque;

	// true if rpm > max rpm
	bool bRevLimit = State.EngineRPM > NAConfig.EngineMaxRPM;

	switch (State.OperationMode)
	{
	case EVehicleEngineOperationMode::On:
		// enable fuel injection and spark if there's throttle input
		// the fuel injection and spark might be disabled in certain conditions
		State.bFuelInjection = State.RawThrottleInput > SMALL_NUMBER;

		// update revlimit timer
		State.RevLimiterTimer = bRevLimit ? 0.f : State.RevLimiterTimer + State.PhysicsDeltaTime;
		// avoid overflow
		// avoid rapid toggling due to RevLimiterTime being too short (not unit conversion)
		State.RevLimiterTimer = FMath::Min(State.RevLimiterTimer, NAConfig.RevLimiterTime * 10.f);

		// if rpm is smaller than max rpm and the timer fullfills the timer limit
		State.bSpark = !bDisableSpark && !bRevLimit && State.RevLimiterTimer >= NAConfig.RevLimiterTime;

		//check if engine is idling
		if (State.EngineRPM < NAConfig.EngineIdleRPM && NAConfig.EngineIdleRPM > 0.f)
		{
			// if rpm is too low
			// interp the throttle to 1
			// and enable spark and fuel injection
			// to maintain the rpm
			State.RealThrottle = FMath::FInterpTo(
				State.RealThrottle, 
				1.f, 
				State.PhysicsDeltaTime, 
				NAConfig.IdleThrottleInterpSpeed);

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
		if (State.EngineRPM > FMath::Min(1.5 * NAConfig.EngineIdleRPM, 0.9 * NAConfig.EngineMaxRPM))
		{
			State.OperationMode = EVehicleEngineOperationMode::On;
			State.StarterMotorTorque = 0.f;
		}
		else
		{
			State.StarterMotorTorque += State.PhysicsDeltaTime * State.TorqueRequiredToStartEngine * 10;
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

	EngineAcceleration();
	UpdateExhaust();
}

void UVehicleEngineComponent::Initialize()
{
	//get required torque to start engine
	NAConfig.StartFriction = FMath::Max(NAConfig.StartFriction, SMALL_NUMBER);
	NAConfig.FrictionCoefficient = FMath::Max(NAConfig.FrictionCoefficient, SMALL_NUMBER);
	State.TorqueRequiredToStartEngine = NAConfig.StartFriction + NAConfig.EngineIdleRPM * NAConfig.FrictionCoefficient;
	
	UpdateCachedRichCurve();

	//get idle throttle
	State.IdleThrottle = FMath::Clamp(
		UVehicleUtil::SafeDivide(State.TorqueRequiredToStartEngine,
			State.TorqueRequiredToStartEngine + NAConfig.MaxEngineTorque * CachedTorqueCurve.Eval(NAConfig.EngineIdleRPM)),
		0.f, 1.f);

	//check if idle rpm is valid
	if (NAConfig.EngineIdleRPM > 0)
	{
		State.EngineOffRPM = 0.7 * NAConfig.EngineIdleRPM;
	}
	else
	{
		State.IdleThrottle = 0;
		State.EngineOffRPM = -BIG_NUMBER;
	}

	// reset rev-limiter
	State.RevLimiterTimer = NAConfig.RevLimiterTime;
}

void UVehicleEngineComponent::UpdateCachedRichCurve()
{
	if (IsValid(NAConfig.EngineTorqueCurve))
	{
		CachedTorqueCurve = NAConfig.EngineTorqueCurve->FloatCurve;
	}
	else
	{
		CachedTorqueCurve.Reset();
	}
}

EVehicleEngineOperationMode UVehicleEngineComponent::StartVehicleEngine()
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

EVehicleEngineOperationMode UVehicleEngineComponent::ShutVehicleEngine()
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
