// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "VehicleDrivetrainStructs.h"

/** Caller-owned gear selection. The component retains its World timer for now. */
struct KINETIFORGE_API FVehicleGearboxRuntimeState
{
	float CurrentGearRatio = 0.f;
	float P2MotorTorque = 0.f;
	int32 CurrentGear = 0;
	int32 TargetGear = 0;
	bool bIsInGear = true;
	bool bShouldRevMatch = false;
	bool bShouldCutSpark = false;
	/** Used by standalone runtimes; the legacy component still uses its timer. */
	float RemainingShiftTime = 0.f;
};

/** Gear generation and shift transitions with no World, timer or delegate access. */
class KINETIFORGE_API FVehicleGearboxSolver
{
public:
	static bool CalculateGearRatios(FVehicleGearboxConfig& Config,
		TArray<float>& ForwardGearRatios, TArray<float>& ReverseGearRatios);
	static bool CalculateGearRatios(const FVehicleGearboxConfig& Config,
		TArray<float>& LargerArray, TArray<float>& SmallerArray, bool bInverseSign);
	static float GetGearRatio(const FVehicleGearboxConfig& Config,
		const TArray<float>& ForwardGearRatios, const TArray<float>& ReverseGearRatios, int32 TargetGear);
	static void CalculateSpeedRanges(const FVehicleGearboxConfig& Config,
		const TArray<float>& ForwardGearRatios, const TArray<float>& ReverseGearRatios,
		float EffectiveWheelRadius, float EngineIdleRPM, float EngineMaxRPM, TArray<FVector2D>& SpeedRanges);
	static bool BeginShift(const FVehicleGearboxConfig& Config, FVehicleGearboxRuntimeState& State,
		int32 ForwardGearCount, int32 ReverseGearCount, int32 TargetGear);
	static void CompleteShift(const FVehicleGearboxConfig& Config, FVehicleGearboxRuntimeState& State,
		const TArray<float>& ForwardGearRatios, const TArray<float>& ReverseGearRatios);
	/** Returns true once on completion. Does not dispatch a callback. */
	static bool AdvanceShift(const FVehicleGearboxConfig& Config, FVehicleGearboxRuntimeState& State,
		const TArray<float>& ForwardGearRatios, const TArray<float>& ReverseGearRatios, float DeltaTime);
};
