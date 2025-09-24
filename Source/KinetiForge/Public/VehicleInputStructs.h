// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "VehicleInputStructs.generated.h"


USTRUCT(BlueprintType)
struct FVehiclInputInterpSpeed
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D ThrottleInterpSpeed = FVector2D(5, 5);
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UCurveFloat* ThrottleCurve = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D BrakeInterpSpeed = FVector2D(5, 5);
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UCurveFloat* BrakeCurve = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D ClutchInterpSpeed = FVector2D(5, 5);
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UCurveFloat* ClutchCurve = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D HandbrakeInterpSpeed = FVector2D(15, 15);
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UCurveFloat* HandbrakeCurve = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D SteeringInterpSpeed = FVector2D(2.5, 2.5);
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UCurveFloat* SteeringCurve = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UCurveFloat* HighSpeedSteeringScale = nullptr;
};

USTRUCT(BlueprintType)
struct FVehicleInputAssistConfig
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "engage clutch when eg. changing gear or low rpm"))
    bool bAutomaticClutch = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "disable AutomaticClutch, and disable throttle when in N gear"))
    bool bEVClutchLogic = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "at which rpm the clutch should be (gradually) released"))
    FVector2D AutoClutchRange = FVector2D(1200, 2500);
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bRevMatching = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float RevMatchMaxThrottle = 0.6;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAutoHold = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Releases the brake of the axle if the torque weight(normalized) is > 0.5"))
    bool bBurnOutAssist = true;
};

USTRUCT(BlueprintType)
struct FVehicleInputValue
{
    GENERATED_USTRUCT_BODY()

    //input
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ThrottleValue = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BrakeValue = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ClutchValue = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SteeringValue = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HandbrakeValue = 0.f;

    //player input
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ThrottleInput = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BrakeInput = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ClutchInput = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SteeringInput = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HandbrakeInput = 0.f;

    //real input
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RealThrottleValue = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RealBrakeValue = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RealClutchValue = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RealSteeringValue = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RealHandbrakeValue = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSwitchThrottleAndBrake = false;
};