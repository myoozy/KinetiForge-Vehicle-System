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
	SimData.RevLimiterTimer = NAConfig.RevLimiterTime;
}


void UVehicleEngineComponent::EngineAcceleration()
{
	// absolut engine rpm
	float AbsolutRPM = FMath::Abs(SimData.EngineRPM);

	// internal friction of the engine
	float FrictionTorque = NAConfig.StartFriction + NAConfig.FrictionCoefficient * AbsolutRPM;

	// torque generated inside of the engine
	float IndicatedTorque = 0.f;
	if (NAConfig.EngineTorqueCurve)
	{
		IndicatedTorque = SimData.bSpark * SimData.RealThrottle* (FrictionTorque + NAConfig.MaxEngineTorque * NAConfig.EngineTorqueCurve->GetFloatValue(AbsolutRPM));
	}
	else
	{
		IndicatedTorque = SimData.bSpark * SimData.RealThrottle * (FrictionTorque + NAConfig.MaxEngineTorque);
	}

	//turbo charged and na have different behavior
	//check if engine is really turbo charged
	if (TurboConfig.MaxBoostPressure > SMALL_NUMBER)
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
		float TargetVacuum = 0.f;

		bool bAntiLagActive = TurboConfig.bEnableAntiLag
			&& TargetSpool < SimData.TurboSpool
			&& AbsolutRPM > TurboConfig.AntiLagMinRPM
			&& SimData.TurboSpool > MinSpoolForPositiveBoost;

		// if throttle input is too small, blow off valve will be opened
		// if anti-lag is triggered, the valve should not be open
		bool bIsBlowOffValveOpen = SimData.RawThrottleInput <= SMALL_NUMBER && !bAntiLagActive;

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

			if (bAntiLagActive)
			{
				float ALS_TargetSpool = FMath::Sqrt(TurboConfig.AntiLagTargetPressureRatio);
				TargetSpool = FMath::Min(SimData.TurboSpool, ALS_TargetSpool);

				if (!SimData.bIsAntiLagTriggered)
				{
					SimData.bIsAntiLagTriggered = true;
					bShouldTriggerAntiLagCallback = true;
				}
			}
			else
			{
				SimData.bIsAntiLagTriggered = false;

				// if the turbo rpm is too low, the intake will have restriction
				// we estimate the max restriction as vacuum pressure
				float CurrentIntakeRestriction = FMath::GetMappedRangeValueClamped(
					FVector2D(0.f, MinSpoolForPositiveBoost),
					FVector2D(TurboConfig.StaticIntakeRestriction, 0.f),
					SimData.TurboSpool
				);

				TargetVacuum = FMath::Lerp(TurboConfig.StaticIntakeRestriction, CurrentIntakeRestriction, SimData.RawThrottleInput);
			}
		}

		// get spool
		SimData.TurboSpool = FMath::FInterpTo(
			SimData.TurboSpool, 
			TargetSpool, 
			SimData.PhysicsDeltaTime, 
			SpoolInterpSpeed
		);

		// get manifold vacuum
		SimData.ManifoldVacuum = FMath::FInterpTo(
			SimData.ManifoldVacuum,
			TargetVacuum,
			SimData.PhysicsDeltaTime,
			50.f
		);

		// get turbo pressure
		SimData.TurboSpool = FMath::Clamp(SimData.TurboSpool, 0.f, 1.f);
		float SpoolSqr = SimData.TurboSpool * SimData.TurboSpool;
		float CurrentBoost = FMath::GetMappedRangeValueClamped(
			FVector2D(MinSpoolForPositiveBoost * MinSpoolForPositiveBoost, 1.f),
			FVector2D(0.0f, TurboConfig.MaxBoostPressure),
			SpoolSqr
		);
		SimData.TurboPressure = CurrentBoost + SimData.ManifoldVacuum;

		//get indicated torque
		float TorqueMultiplier = 1.f + SimData.TurboPressure * TurboConfig.BoostEfficiency;
		IndicatedTorque *= FMath::Max(SMALL_NUMBER, TorqueMultiplier);
	}
	else
	{
		SimData.bIsTurboBlowingOff = false;
		SimData.bIsAntiLagTriggered = false;
		SimData.TurboPressure *= 0.5f;
		SimData.TurboSpool *= 0.5f;
		SimData.ManifoldVacuum *= 0.5f;
	}

	// consider P1 motor
	IndicatedTorque += SimData.P1MotorTorque;

	//accelerate engine
	SimData.EngineAngularVelocity += SafeDivide(SimData.PhysicsDeltaTime, NAConfig.EngineInertia) * (IndicatedTorque - SimData.LoadTorque + SimData.StarterMotorTorque);
	//get the direction of friction torque
	float AngVelSignWithoutFriction = FMath::Sign(SimData.EngineAngularVelocity);
	FrictionTorque = FrictionTorque * AngVelSignWithoutFriction;
	SimData.EngineAngularVelocity -= SafeDivide(SimData.PhysicsDeltaTime, NAConfig.EngineInertia) * FrictionTorque;
	//zero cross check
	if (FMath::Sign(SimData.EngineAngularVelocity) != AngVelSignWithoutFriction)SimData.EngineAngularVelocity = 0;
	//get engine rpm
	SimData.EngineRPM = RadToRPM * SimData.EngineAngularVelocity;
	SimData.EffectiveTorque = IndicatedTorque - FrictionTorque;
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

	if (bShouldTriggerAntiLagCallback)
	{
		bShouldTriggerAntiLagCallback = false;
		for (FOnAntiLagTriggeredDelegate AntiLagCallback : AntiLagCallbacks)
		{
			if (AntiLagCallback.IsBound())
			{
				AntiLagCallback.Execute();
			}
			else
			{
				AntiLagCallbacks.Remove(AntiLagCallback);
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

		if (SimData.EngineRPM > NAConfig.EngineMaxRPM)
		{
			//cut power(disable spark) at max rpm
			SimData.bSpark = false;
			SimData.RevLimiterTimer = 0.f;
		}
		else
		{
			// only enable spark when there's throttle input
			SimData.bSpark = SimData.RawThrottleInput > SMALL_NUMBER && SimData.RevLimiterTimer >= NAConfig.RevLimiterTime;
		}

		//check if throttle is released and if idle is valid
		if (SimData.RawThrottleInput < SMALL_NUMBER && NAConfig.EngineIdleRPM > 0)
		{
			//check if engine is idling
			if (SimData.EngineRPM < NAConfig.EngineIdleRPM)
			{
				SimData.RealThrottle = FMath::FInterpTo(SimData.RealThrottle, 1.f, SimData.PhysicsDeltaTime, NAConfig.IdleThrottleInterpSpeed);
				SimData.bSpark = true;
			}
			else
			{
				SimData.RealThrottle = FMath::FInterpTo(SimData.RealThrottle, 0.f, SimData.PhysicsDeltaTime, NAConfig.IdleThrottleInterpSpeed);
				SimData.bSpark = false;
			}
		}

		if (SimData.EngineRPM <= SimData.EngineOffRPM)
		{
			SimData.State = EVehicleEngineState::Off;
			SimData.bSpark = false;
		}
		break;
	case EVehicleEngineState::Off:
		SimData.bSpark = false;
		if (SimData.EngineRPM > SimData.EngineOffRPM)
		{
			SimData.State = EVehicleEngineState::On;
			SimData.bSpark = true;
		}
		break;
	case EVehicleEngineState::Starting:
		SimData.RealThrottle = FMath::Lerp(SimData.IdleThrottle, 1.f, SimData.RawThrottleInput);
		SimData.bSpark = true;
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
		SimData.bSpark = false;
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

void UVehicleEngineComponent::BindEventToOnAntiLagTriggered(FOnAntiLagTriggeredDelegate InOnAntiLagTriggered)
{
	if (AntiLagCallbacks.Find(InOnAntiLagTriggered) == INDEX_NONE)
	{
		AntiLagCallbacks.Add(InOnAntiLagTriggered);
	}
}
