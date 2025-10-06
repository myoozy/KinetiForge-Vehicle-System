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
	float FrictionTorque = NAConfig.StartFriction + NAConfig.FrictionCoefficient * FMath::Abs(SimData.EngineRPM);
	float NAInitialTorque;
	float InitialTorque;
	if (NAConfig.EngineTorqueCurve)
	{
		NAInitialTorque = SimData.bSpark * SimData.RealThrottle* (FrictionTorque + NAConfig.MaxEngineTorque * NAConfig.EngineTorqueCurve->GetFloatValue(FMath::Abs(SimData.EngineRPM)));
	}
	else
	{
		NAInitialTorque = SimData.bSpark * SimData.RealThrottle * (FrictionTorque + NAConfig.MaxEngineTorque);
	}

	//turbo charged and na have different behavior
	//check if engine is really turbo charged
	if (TurboConfig.TurboBoostTorque > SMALL_NUMBER && TurboConfig.TurboMaxPressure > SMALL_NUMBER)
	{
		float PressureRange = TurboConfig.TurboMaxPressure - TurboConfig.TurboNegPressure;
		//if throttle is hit
		if (SimData.RawThrottleInput > SMALL_NUMBER)
		{
			//get target turbo boost
			float TargetBoost = FMath::GetMappedRangeValueUnclamped(
				FVector2D(TurboConfig.TurboStartRPM, TurboConfig.TurboFinishRPM), 
				FVector2D(0.f, TurboConfig.TurboMaxPressure), 
				SimData.EngineRPM * SimData.RealThrottle);
			TargetBoost = FMath::Clamp(TargetBoost, TurboConfig.TurboNegPressure, TurboConfig.TurboMaxPressure);

			float InterpSpeed = SafeDivide(PressureRange * SimData.RealThrottle, TurboConfig.TurboLag);
			SimData.TurboPressure = FMath::FInterpConstantTo(SimData.TurboPressure, TargetBoost, SimData.PhysicsDeltaTime, InterpSpeed);
		}
		else
		{
			float WasteInterpSpeed = SafeDivide(PressureRange, TurboConfig.TurboWasteGateLag);
			float AntiLagInterpSpeed = FMath::GetMappedRangeValueClamped(
				FVector2D(TurboConfig.TurboStartRPM, TurboConfig.TurboFinishRPM),
				FVector2D(WasteInterpSpeed * 0.05, 0.001),
				SimData.EngineRPM);
			float InterpSpeed = WasteInterpSpeed;
			if (TurboConfig.bAntiLag)InterpSpeed = AntiLagInterpSpeed;
			SimData.TurboPressure = FMath::FInterpConstantTo(SimData.TurboPressure, 0.f, SimData.PhysicsDeltaTime, InterpSpeed);
		}
		//get initial torque
		float TurboBoost = SafeDivide(SimData.TurboPressure, TurboConfig.TurboMaxPressure);
		TurboBoost = FMath::Max(0.f, TurboBoost);
		InitialTorque = NAInitialTorque + SimData.bSpark * SimData.RealThrottle * TurboBoost * TurboConfig.TurboBoostTorque;
	}
	else
	{
		InitialTorque = NAInitialTorque;
		SimData.TurboPressure = 0.f;
	}

	InitialTorque += SimData.P1MotorTorque;

	//accelerate engine
	SimData.EngineAngularVelocity += SafeDivide(SimData.PhysicsDeltaTime, NAConfig.EngineInertia) * (InitialTorque - SimData.LoadTorque + SimData.StarterMotorTorque);
	//get the direction of friction torque
	float AngVelSignWithoutFriction = FMath::Sign(SimData.EngineAngularVelocity);
	FrictionTorque = FrictionTorque * AngVelSignWithoutFriction;
	SimData.EngineAngularVelocity -= SafeDivide(SimData.PhysicsDeltaTime, NAConfig.EngineInertia) * FrictionTorque;
	//zero cross check
	if (FMath::Sign(SimData.EngineAngularVelocity) != AngVelSignWithoutFriction)SimData.EngineAngularVelocity = 0;
	//get engine rpm
	SimData.EngineRPM = RadToRPM * SimData.EngineAngularVelocity;
	SimData.EffectiveTorque = InitialTorque - FrictionTorque;
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
	case EEngineState::On:
		//cut throttle at max rpm
		if (SimData.EngineRPM > NAConfig.EngineMaxRPM)
		{
			SimData.bSpark = false;
			SimData.RevLimiterTimer = 0.f;
		}
		else
		{
			SimData.bSpark = SimData.RawThrottleInput > SMALL_NUMBER && SimData.RevLimiterTimer >= NAConfig.RevLimiterTime;
		}

		//check if throttle is released
		if (SimData.bSpark || SimData.bIdleRPMInValid)
		{
			SimData.RealThrottle = FMath::Lerp(SimData.IdleThrottle, 1.f, SimData.RawThrottleInput);
		}
		else
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
			SimData.State = EEngineState::Off;
			SimData.bSpark = false;
		}
		break;
	case EEngineState::Off:
		SimData.bSpark = false;
		if (SimData.EngineRPM > SimData.EngineOffRPM)
		{
			SimData.State = EEngineState::On;
			SimData.bSpark = true;
		}
		break;
	case EEngineState::Starting:
		SimData.RealThrottle = FMath::Lerp(SimData.IdleThrottle, 1.f, SimData.RawThrottleInput);
		SimData.bSpark = true;
		//check if engine is started
		if (SimData.EngineRPM > FMath::Min(1.5 * NAConfig.EngineIdleRPM, 0.9 * NAConfig.EngineMaxRPM))
		{
			SimData.State = EEngineState::On;
			SimData.StarterMotorTorque = 0.f;
		}
		else
		{
			SimData.StarterMotorTorque += SimData.PhysicsDeltaTime * SimData.TorqueRequiredToStartEngine * 10;
		}
		break;
	case EEngineState::Shutting:
		SimData.StarterMotorTorque = 0.f;
		SimData.bSpark = false;
		if (SimData.EngineRPM <= SimData.EngineOffRPM)
		{
			SimData.State = EEngineState::Off;
		}
		break;
	default:
		SimData.State = EEngineState::On;
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
		SimData.bIdleRPMInValid = false;
		SimData.EngineOffRPM = 0.7 * NAConfig.EngineIdleRPM;
	}
	else
	{
		SimData.bIdleRPMInValid = true;
		SimData.IdleThrottle = 0;
		SimData.EngineOffRPM = -BIG_NUMBER;
	}
}

EEngineState UVehicleEngineComponent::StartVehicleEngine()
{
	switch (SimData.State)
	{
	case EEngineState::On:
		break;
	case EEngineState::Off:SimData.State = EEngineState::Starting;
		break;
	case EEngineState::Starting:
		break;
	case EEngineState::Shutting://State = EEngineState::Starting;
		break;
	default:
		break;
	}

	return SimData.State;
}

EEngineState UVehicleEngineComponent::ShutVehicleEngine()
{
	switch (SimData.State)
	{
	case EEngineState::On:SimData.State = EEngineState::Shutting;
		break;
	case EEngineState::Off:
		break;
	case EEngineState::Starting://State = EEngineState::Shutting;
		break;
	case EEngineState::Shutting:
		break;
	default:
		break;
	}

	return SimData.State;
}
