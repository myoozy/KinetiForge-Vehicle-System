// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "VehicleDrivetrainStructs.h"

template <int32 NumSamples>
struct FVehicleLUT;

/** Scalar engine settings copied from the UObject config at the component boundary. */
struct KINETIFORGE_API FVehicleEngineParameters
{
	float MaxEngineTorque = 400.f;
	float EngineIdleRPM = 900.f;
	float EngineMaxRPM = 6000.f;
	float EngineInertia = 0.2f;
	float StartFriction = 50.f;
	float FrictionCoefficient = 0.005f;
	float PumpingLossCoefficient = 0.005f;
	float IdleThrottleInterpSpeed = 1.f;
	float RevLimiterTime = 0.05f;
	float MaxPowerLambda = 0.85f;
	float DeceleratingLambda = 0.9f;
	float UnburntFuelAccumulationRate = 2.f;

	static FVehicleEngineParameters FromConfig(const FVehicleNaturallyAspiratedEngineConfig& Config);
};

/** Value-based engine step equations. State, LUT, and discrete event flags remain caller-owned. */
class KINETIFORGE_API FVehicleEngineSolver
{
public:
	static float CalculateMaxTorque(const FVehicleEngineParameters& Parameters,
		const FVehicleEngineTurboConfig& TurboConfig);
	static void EngineAcceleration(const FVehicleEngineParameters& Parameters,
		const FVehicleEngineTurboConfig& TurboConfig, const FVehicleLUT<32>& TorqueLUT,
		FVehicleEngineSimState& State, float DeltaTime, bool& bShouldTriggerTurboBlowOffCallback);
	static void UpdateExhaust(const FVehicleEngineParameters& Parameters,
		const FVehicleEngineExhaustConfig& ExhaustConfig, FVehicleEngineSimState& State,
		float DeltaTime, bool& bShouldTriggerBackfiringCallback);
	static float EstimateThrottleForTorque(const FVehicleEngineParameters& Parameters,
		const FVehicleEngineSimState& State, const FVehicleLUT<32>& TorqueLUT,
		float TargetEffectiveTorque);
	static void UpdatePhysics(const FVehicleEngineParameters& Parameters,
		const FVehicleEngineTurboConfig& TurboConfig, const FVehicleEngineExhaustConfig& ExhaustConfig,
		const FVehicleLUT<32>& TorqueLUT, FVehicleEngineSimState& State,
		float DeltaTime, float Throttle, float LoadTorque, bool bDisableSpark,
		bool& bShouldTriggerTurboBlowOffCallback, bool& bShouldTriggerBackfiringCallback);
	static EVehicleEngineOperationMode StartVehicleEngine(FVehicleEngineSimState& State);
	static EVehicleEngineOperationMode ShutdownVehicleEngine(FVehicleEngineSimState& State);
};
