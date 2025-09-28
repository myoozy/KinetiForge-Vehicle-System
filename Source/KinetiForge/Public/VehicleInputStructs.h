// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "VehicleInputStructs.generated.h"

USTRUCT(BlueprintType)
struct FVehicleInputAxisConfig
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D InterpSpeed = FVector2D(5.f, 5.f);
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UCurveFloat* ResponseCurve = nullptr;

    FVehicleInputAxisConfig(
        FVector2D newInterpSpeed = FVector2D(5.f, 5.f),
        UCurveFloat* newResponseCurve = nullptr)
    {
        InterpSpeed = newInterpSpeed;
        ResponseCurve = newResponseCurve;
    }

    static float InterpInputValueConstant(
        float Current,
        float Target,
        float DeltaTime,
        FVector2D Speed
    )
    {
        float s = (Target < SMALL_NUMBER) ? Speed.Y : Speed.X;
        return (s <= 0) ? Target : FMath::FInterpConstantTo(Current, Target, DeltaTime, s);
    }

    static float InterpInputValue(
        float Current,
        float Target,
        float DeltaTime,
        FVector2D Speed
    )
    {
        float s = (Target < SMALL_NUMBER) ? Speed.Y : Speed.X;
        return (s <= 0) ? Target : FMath::FInterpTo(Current, Target, DeltaTime, s);
    }
};

USTRUCT(BlueprintType)
struct FVehiclInputConfig
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVehicleInputAxisConfig Throttle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVehicleInputAxisConfig Brake;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVehicleInputAxisConfig Clutch;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVehicleInputAxisConfig Handbrake = FVehicleInputAxisConfig(FVector2D(15.f, 15.f));
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVehicleInputAxisConfig Steering = FVehicleInputAxisConfig(FVector2D(2.5f, 2.5f));
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
struct FVehicleInputState
{
    GENERATED_USTRUCT_BODY()

    //input
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Throttle = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Brake = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Clutch = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Handbrake = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Steering = 0.f;
};

USTRUCT(BlueprintType)
struct FVehicleInputPipeline
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVehicleInputState Raw;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVehicleInputState Smoothened;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVehicleInputState Final;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSwitchThrottleAndBrake = false;
};