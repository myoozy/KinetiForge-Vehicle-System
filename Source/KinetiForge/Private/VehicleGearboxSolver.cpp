// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehicleGearboxSolver.h"
#include "VehicleDrivetrainMath.h"

bool FVehicleGearboxSolver::BeginShift(const FVehicleGearboxConfig& Config,
	FVehicleGearboxRuntimeState& State, int32 ForwardGearCount, int32 ReverseGearCount, int32 TargetGear)
{
	//check if target gear is valid
	if (TargetGear == State.CurrentGear || TargetGear > ForwardGearCount
		|| TargetGear < -ReverseGearCount || !State.bIsInGear)
	{
		return false;
	}

	//Sequential gearbox can only shift one gear once
	if (Config.bSequentialGearbox)
	{
		TargetGear = FMath::Clamp(TargetGear, State.CurrentGear - 1, State.CurrentGear + 1);
	}

	//prepare shift
	State.TargetGear = TargetGear;
	State.CurrentGearRatio = 0.f;
	State.bIsInGear = false;
	const int32 AbsTargetGear = FMath::Abs(State.TargetGear);
	const int32 AbsCurrentGear = FMath::Abs(State.CurrentGear);
	const bool bShiftingUp = AbsTargetGear > AbsCurrentGear;
	const bool bShiftingDown = AbsTargetGear < AbsCurrentGear;
	State.bShouldRevMatch = bShiftingDown && State.TargetGear != 0;
	State.bShouldCutSpark = Config.bSequentialGearbox && bShiftingUp && AbsTargetGear != 1;
	State.RemainingShiftTime = Config.ShiftDelay;
	return true;
}

void FVehicleGearboxSolver::CompleteShift(const FVehicleGearboxConfig& Config,
	FVehicleGearboxRuntimeState& State, const TArray<float>& ForwardGearRatios,
	const TArray<float>& ReverseGearRatios)
{
	State.CurrentGear = State.TargetGear;
	State.bIsInGear = true;
	State.bShouldRevMatch = false;
	State.bShouldCutSpark = false;
	State.CurrentGearRatio = GetGearRatio(Config, ForwardGearRatios, ReverseGearRatios, State.CurrentGear);
	State.TargetGear = 0;
	State.RemainingShiftTime = 0.f;
}

bool FVehicleGearboxSolver::AdvanceShift(const FVehicleGearboxConfig& Config,
	FVehicleGearboxRuntimeState& State, const TArray<float>& ForwardGearRatios,
	const TArray<float>& ReverseGearRatios, float DeltaTime)
{
	if (State.bIsInGear || DeltaTime < 0.f) return false;
	State.RemainingShiftTime = FMath::Max(0.f, State.RemainingShiftTime - DeltaTime);
	if (State.RemainingShiftTime > 0.f) return false;
	CompleteShift(Config, State, ForwardGearRatios, ReverseGearRatios);
	return true;
}

float FVehicleGearboxSolver::GetGearRatio(const FVehicleGearboxConfig& Config,
	const TArray<float>& ForwardGearRatios, const TArray<float>& ReverseGearRatios, int32 TargetGear)
{
	//InTarget = FMath::Clamp(InTarget, -ReverseGearRatios.Num(), GearRatios.Num());
	if (TargetGear > Config.NumberOfGears || TargetGear < -Config.NumOfReverseGears) return 0;
	if (TargetGear > 0) return ForwardGearRatios.IsValidIndex(TargetGear - 1) ? ForwardGearRatios[TargetGear - 1] : 0.f;
	if (TargetGear < 0) return ReverseGearRatios.IsValidIndex(-TargetGear - 1) ? ReverseGearRatios[-TargetGear - 1] : 0.f;
	return 0.0f;
}

bool FVehicleGearboxSolver::CalculateGearRatios(FVehicleGearboxConfig& Config,
	TArray<float>& ForwardGearRatios, TArray<float>& ReverseGearRatios)
{
	if (Config.NumberOfGears < 1 || Config.NumOfReverseGears < 1) return false;

	//make sure positive
	Config.FirstGear = FMath::Abs(Config.FirstGear);
	Config.TopGear = FMath::Abs(Config.TopGear);
	Config.GearRatioBias = FMath::Clamp(FMath::Abs(Config.GearRatioBias), 0.f, 1.f);
	//make sure topgear is smaller than first gear
	Config.TopGear = FMath::Min(Config.TopGear, Config.FirstGear);

	//clear all gears
	ForwardGearRatios.SetNum(Config.NumberOfGears);
	ReverseGearRatios.SetNum(Config.NumOfReverseGears);
	if (Config.NumberOfGears >= Config.NumOfReverseGears)
	{
		return CalculateGearRatios(Config, ForwardGearRatios, ReverseGearRatios, false);
	}
	return CalculateGearRatios(Config, ReverseGearRatios, ForwardGearRatios, true);
}

void FVehicleGearboxSolver::CalculateSpeedRanges(const FVehicleGearboxConfig& Config,
	const TArray<float>& ForwardGearRatios, const TArray<float>& ReverseGearRatios,
	float EffectiveWheelRadius, float EngineIdleRPM, float EngineMaxRPM, TArray<FVector2D>& SpeedRanges)
{
	const int32 GearCount = FMath::Max(Config.NumberOfGears, Config.NumOfReverseGears);
	SpeedRanges.SetNum(GearCount + 1);
	SpeedRanges[0] = FVector2D(0);
	const float RPMToRad = PI * 0.0333333333333f;
	const float WheelSpeedPerRPM = EffectiveWheelRadius * RPMToRad * 0.01;
	for (int32 GearIndex = 1; GearIndex <= GearCount; ++GearIndex)
	{
		FVector2D SpeedRange;
		const float GearRatio = GetGearRatio(Config, ForwardGearRatios, ReverseGearRatios, GearIndex);
		SpeedRange.X = VehicleDrivetrainMath::DivideOrDefault(EngineIdleRPM * WheelSpeedPerRPM, GearRatio);
		SpeedRange.Y = VehicleDrivetrainMath::DivideOrDefault(EngineMaxRPM * WheelSpeedPerRPM, GearRatio);
		SpeedRanges[GearIndex] = SpeedRange;
	}
}

bool FVehicleGearboxSolver::CalculateGearRatios(const FVehicleGearboxConfig& Config,
	TArray<float>& LargerArray, TArray<float>& SmallerArray, bool bInverseSign)
{
	const int32 LargerLength = LargerArray.Num();
	const int32 SmallerLength = SmallerArray.Num();
	if (LargerLength < SmallerLength) return false;
	float GearDirection = 1;
	if (bInverseSign) GearDirection = -1;

	if (LargerLength > 1)
	{
		for (int32 GearIndex = 0; GearIndex < LargerLength; ++GearIndex)
		{
			float NormalizedGearIndex = float(GearIndex) / float(LargerLength - 1); //normalize the factor, min = 0. max = 1
			//calculate bias
			float AdjustedGearIndex = FMath::Pow(NormalizedGearIndex, 1.f - Config.GearRatioBias);
			LargerArray[GearIndex] = (Config.FirstGear + (Config.TopGear - Config.FirstGear) * AdjustedGearIndex) * GearDirection;
		}
	}
	else if (LargerLength == 1)
	{
		LargerArray[0] = Config.FirstGear * GearDirection;
	}
	else return false;

	for (int32 GearIndex = 0; GearIndex < SmallerLength; ++GearIndex)
	{
		SmallerArray[GearIndex] = -LargerArray[GearIndex];
	}
	return true;
}
