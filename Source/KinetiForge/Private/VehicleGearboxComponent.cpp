// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)


#include "VehicleGearboxComponent.h"

// Sets default values for this component's properties
UVehicleGearboxComponent::UVehicleGearboxComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void UVehicleGearboxComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CalculateGearRatios();	//to avoid nullptr
}


void UVehicleGearboxComponent::PrepareShift()
{
	CurrentGearRatio = 0.f;
	bIsInGear = false;

	if (FMath::Abs(TargetGear) >= FMath::Abs(CurrentGear))
	{
		bShouldCutThrottle = true;
	}
	else
	{
		if (TargetGear != 0)bShouldRevMatch = true;
	}
}

void UVehicleGearboxComponent::FinalizeShift()
{
	CurrentGear = TargetGear;
	bIsInGear = true;
	bShouldRevMatch = false;
	bShouldCutThrottle = false;
	CurrentGearRatio = GetGearRatio(CurrentGear);
	TargetGear = 0;
}

// Called every frame
void UVehicleGearboxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UVehicleGearboxComponent::ShiftToTargetGear(int32 InTargetGear, bool bImmediate)
{
	//check if gearratios has to be calculated again
	if (IsGearDataDirty())
	{
		CalculateGearRatios();
	}

	//check if target gear is valid
	if (InTargetGear == CurrentGear 
		|| InTargetGear > GearRatios.Num() 
		|| InTargetGear < -ReverseGearRatios.Num() 
		|| !bIsInGear)
		return;

	//if GetWorld()=false, can't set timer, if ShiftDelay to small, immediate
	bImmediate = !(!bImmediate && GetWorld()) || Config.ShiftDelay < SMALL_NUMBER;

	//Sequential gearbox can only shift one gear once
	if (Config.bSequentialGearbox)InTargetGear = FMath::Clamp(InTargetGear, CurrentGear - 1, CurrentGear + 1);

	TargetGear = InTargetGear;
	PrepareShift();

	if (bImmediate)
	{
		FinalizeShift();
		return;
	}

	//set timer
	GetWorld()->GetTimerManager().SetTimer(
		GearShiftTimerHandle,
		this,
		&UVehicleGearboxComponent::FinalizeShift,	//call this function after delay
		Config.ShiftDelay,
		false // non cycle
	);
}

void UVehicleGearboxComponent::UpdateOutputShaft(
	float InClutchTorque, 
	float& OutTorque, 
	float& OutReflectedInertia
)
{
	OutTorque = (InClutchTorque + P2MotorTorque) * CurrentGearRatio * Config.Efficiency;
	OutReflectedInertia = Config.InputShaftInertia * CurrentGearRatio * CurrentGearRatio;
}

void UVehicleGearboxComponent::UpdateInputShaft(
	float InAxleVelocity, 
	float InAxleInertia, 
	float& OutClutchVelocity, 
	float& OutReflectedInertia,
	float& OutCurrentGearRatio,
	float& OutFirstGearInertia
)
{
	OutClutchVelocity = InAxleVelocity * CurrentGearRatio;
	OutReflectedInertia = Config.InputShaftInertia + SafeDivide(InAxleInertia, CurrentGearRatio * CurrentGearRatio);
	OutCurrentGearRatio = CurrentGearRatio;
	OutFirstGearInertia = Config.InputShaftInertia + SafeDivide(InAxleInertia, FMath::Square(GearRatios[0]));	//tip: do not use the variant FirstGear, because the user can change it any time
}

float UVehicleGearboxComponent::GetGearRatio(int InTarget)
{
	//InTarget = FMath::Clamp(InTarget, -ReverseGearRatios.Num(), GearRatios.Num());
	if (InTarget > Config.NumberOfGears || InTarget < -Config.NumOfReverseGears)return 0;
	if (InTarget > 0)return GearRatios[InTarget - 1];
	if (InTarget < 0)return ReverseGearRatios[-InTarget - 1];
	return 0.0f;
}

void UVehicleGearboxComponent::CalculateSpeedRangeOfEachGear(
	float InEffectiveWheelRadius,
	float InEngineIdleRPM, 
	float InEngineMaxRPM, 
	TArray<FVector2D>& OutSpeedRanges
)
{
	if (IsGearDataDirty())CalculateGearRatios();

	int32 NumGears = FMath::Max(Config.NumberOfGears, Config.NumOfReverseGears);
	OutSpeedRanges.SetNum(NumGears + 1);
	OutSpeedRanges[0] = FVector2D(0);

	float RPMToRad = PI * 0.0333333333333f;
	float avgRPM = InEffectiveWheelRadius * RPMToRad * 0.036;

	for (int32 i = 1; i <= NumGears; i++)
	{
		FVector2D SpeedRangeOfCurrentGear;
		float GearRatio = GetGearRatio(i);
		SpeedRangeOfCurrentGear.X = SafeDivide(InEngineIdleRPM * avgRPM, GearRatio);
		SpeedRangeOfCurrentGear.Y = SafeDivide(InEngineMaxRPM * avgRPM, GearRatio);

		OutSpeedRanges[i] = SpeedRangeOfCurrentGear;
	}
}

bool UVehicleGearboxComponent::CalculateGearRatios()
{
	if (Config.NumberOfGears < 1 || Config.NumOfReverseGears < 1)return false;

	//make sure positive
	Config.FirstGear = FMath::Abs(Config.FirstGear);
	Config.TopGear = FMath::Abs(Config.TopGear);
	Config.GearRatioBias = FMath::Clamp(FMath::Abs(Config.GearRatioBias), 0.f, 1.f);
	//make sure topgear is smaller than first gear
	Config.TopGear = FMath::Min(Config.TopGear, Config.FirstGear);

	//clear all gears
	GearRatios.SetNum(Config.NumberOfGears);
	ReverseGearRatios.SetNum(Config.NumOfReverseGears);

	if (Config.NumberOfGears >= Config.NumOfReverseGears)
	{
		return CalculateGearRatios(GearRatios, ReverseGearRatios);
	}
	else
	{
		return CalculateGearRatios(ReverseGearRatios, GearRatios, true);
	}
}

bool UVehicleGearboxComponent::CalculateGearRatios(TArray<float>& LargerArray, TArray<float>& SmallerArray, bool bInverseSign)
{
	int32 LargerLength = LargerArray.Num();
	int32 SmallerLength = SmallerArray.Num();

	if (LargerLength < SmallerLength)return false;

	float sign = 1;
	if (bInverseSign)sign = -1;

	if (LargerLength > 1)
	{
		for (int i = 0; i < LargerLength; ++i)
		{
			float t = (float)i / (float)(LargerLength - 1);	//normalize the factor, min = 0. max = 1
			//calculate bias
			float AdjustedT = FMath::Pow(t, 1.f - Config.GearRatioBias);
			LargerArray[i] = (Config.FirstGear + (Config.TopGear - Config.FirstGear) * AdjustedT) * sign;
		}
	}
	else if(LargerLength == 1)
	{
		LargerArray[0] = Config.FirstGear * sign;
	}
	else
	{
		return false;
	}

	for (int i = 0; i < SmallerLength; ++i)
	{
		SmallerArray[i] = -LargerArray[i];
	}

	return true;
}

bool UVehicleGearboxComponent::IsGearDataDirty()
{
	//check if gearratios has to be calculated again
	if (CachedFirstGear != Config.FirstGear || CachedTopGear != Config.TopGear || CachedGearRatioBias != Config.GearRatioBias
		|| CachedNumGears != Config.NumberOfGears || CachedNumRGears != Config.NumOfReverseGears)
	{
		CachedFirstGear = Config.FirstGear;
		CachedTopGear = Config.TopGear;
		CachedGearRatioBias = Config.GearRatioBias;
		CachedNumGears = Config.NumberOfGears;
		CachedNumRGears = Config.NumOfReverseGears;
		return true;
	}
	else
	{
		return false;
	}
}

float UVehicleGearboxComponent::SafeDivide(float a, float b)
{
	return (FMath::IsNearlyZero(b)) ? 0.0f : a / b;
}