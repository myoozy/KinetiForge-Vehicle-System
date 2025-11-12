// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "VehicleDrivetrainStructs.generated.h"

/*****************************ENGINE******************************/

UENUM(BlueprintType)
enum class EVehicleEngineState : uint8
{
	On  UMETA(DisplayName = "EngineON"),
	Off UMETA(DisplayName = "EngineOFF"),
	Starting    UMETA(DisplayName = "EngineSTARTING"),
	Shutting    UMETA(DisplayName = "EngineSHUTTING")
};

USTRUCT(BlueprintType, Blueprintable)
struct FVehicleNaturallyAspiratedEngineConfig
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0"))
	float MaxEngineTorque = 400;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0"))
	UCurveFloat* EngineTorqueCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0"))
	float EngineIdleRPM = 900;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0"))
	float EngineMaxRPM = 6000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0"))
	float EngineInertia = 0.2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0"))
	float StartFriction = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0"))
	float FrictionCoefficient = 0.005;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0"))
	float IdleThrottleInterpSpeed = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0"))
	float RevLimiterTime = 0.05;

};

USTRUCT(BlueprintType, Blueprintable)
struct FVehicleEngineTurboConfig
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup", meta = (ClampMin = "0.0", ToolTip = "if boost torque > 0, will be considered as turbo charged"))
	float TurboBoostTorque = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup", meta = (ClampMin = "0.0"))
	float TurboStartRPM = 1200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup", meta = (ClampMin = "0.0"))
	float TurboFinishRPM = 2000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup", meta = (ClampMin = "0.0"))
	float TurboMaxPressure = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup", meta = (ClampMin = "-1.0", ClampMax = "0.0"))
	float TurboNegPressure = -0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup", meta = (ClampMin = "0.0"))
	float TurboLag = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup", meta = (ClampMin = "0.0"))
	float TurboWasteGateLag = 0.1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup")
	bool bAntiLag = false;
};

USTRUCT(BlueprintType, Blueprintable)
struct FVehicleEngineSimData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EVehicleEngineState State = EVehicleEngineState::Starting;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DeltaTime")
	float PhysicsDeltaTime = 0.00833333;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float EngineAngularVelocity = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	float RawThrottleInput = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	float IdleThrottle = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	float RealThrottle = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float EffectiveTorque = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float LoadTorque = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float TorqueRequiredToStartEngine = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force")
	float StarterMotorTorque = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float EngineRPM = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float EngineOffRPM = 0.f;	//under this rpm, the engine will be considered as off
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turbo")
	float TurboPressure = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float RevLimiterTimer = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force")
	float P1MotorTorque = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bSpark = 0.f;
};

/*******************************CLUTCH********************************/


UENUM(BlueprintType)
enum class EClutchSimMode : uint8
{
	SpringModel     UMETA(ToolTip = "more realistic, but not recommended for Arcade/EVs/Large physics time step"),
	DampingModel    UMETA(ToolTip = "always safe to use")
};

USTRUCT(BlueprintType)
struct FVehicleClutchConfig
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClutchSetup", meta = (ClampMin = "0.0", ToolTip = "The natural frequency that the clutch tries to reach, it should be lower than game physics frequency"))
	float NaturalFrequency = 120;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClutchSetup", meta = (ClampMin = "0.0", ClampMax = "1.0", ToolTip = "Attention!!! If SimMode is SpringModel, then Damping refers to damping ratio of the spring. If SimMode is DampingModel, then Damping refers to smoothing factor!"))
	float Damping = 0.1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClutchSetup", meta = (ClampMin = "0.0"))
	float Capacity = 1.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClutchSetup", meta = (ClampMin = "0.0", ToolTip = "SpringModel: Using a spring to transmit torque. Using this model will cause engine speed fluctuations (especially when shifting gears with a sequential transmission and low damping ratio). It is recommended to use this model when the physical step size is short (because the natural angular frequency of the spring cannot exceed the game's physical simulation frequency). If the physical step size is large, stiffness can be increased by increasing damping ratio; DampingModel: Use critical damping to transfer torque. Critical damping is calculated based on the current rotational inertia and game physics frequency. The torque is then smoothed to avoid numerical jitter. The torque values are very smooth and do not cause speed jitter during gear changes. This model is recommended for electric vehicles (as electric vehicles typically do not have clutches)."))
	EClutchSimMode SimMode = EClutchSimMode::SpringModel;
};

USTRUCT(BlueprintType)
struct FVehicleClutchSimData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DeltaTime")
	float PhysicsDeltaTime = 0.00833333;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SpringStiffness = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SpringDamping = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float CriticalDamping = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float MaxNaturalFrequency = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float EngineInertia = 0.2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float EngineAngularVelocity = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float GearboxAngularVelocity = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float ClutchSlip = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float AngleDiff = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float ClutchLock = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float MaxClutchTorque = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float SmoothenedTorque = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float ClutchTorque = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float TotalInertia = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float ReflectedInertia = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float FirstGearReflectedInertia = 0.f;
};

/*******************************GEARBOX********************************/


USTRUCT(BlueprintType)
struct FVehicleGearboxConfig
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "0.0", ToolTip = "Please also consider the inertia of the clutch disk"))
	float InputShaftInertia = 0.05;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "0.0"))
	float ShiftDelay = 0.2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "0.0"))
	float FirstGear = 3.636;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "0.0"))
	float TopGear = 0.842;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float GearRatioBias = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "1.0"))
	int32 NumberOfGears = 6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "1.0", ToolTip = "the number of reverse gears should be smaller than non-reverse gears"))
	int32 NumOfReverseGears = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Efficiency = 0.9;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	bool bSequentialGearbox = false;
};

USTRUCT(BlueprintType)
struct FAutoGearboxConfig
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "this simulates the logic of a real automatic gearbox. depending on the vehicle speed and the throttle/brake input. this is actually a AMT gearbox."))
	bool bAutomaticGearbox = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "this helps shifting from D to R / R to D / N to D / D to N / N to R / R to N (automatic gearboxes in reallife will never do this)"))
	bool bArcadeAutoGearbox = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "if true, the arcade gearbox will be updated without any delay. eg. shift from D to N immediately, then in the next frame, shift to D again immediately."))
	bool bArcadeShiftInstant = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "if using sport mode, the auto gearbox will shift up as late as possible and shift down as quick as possible. Also, automatic rev-matching."))
	bool bSportMode = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "this roughly decides the refershrate of the auto gearbox. eg. if AutoGearboxRefreshTime = 0.5 seconds, the auto gearbox will be refreshed 2 times per second."))
	float AutomaticGearboxRefreshTime = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "this roughly decides how long the auto gearbox needs to cool down after one shift."))
	float AutoShiftCoolDown = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxUpShiftSteps = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxDownShiftSteps = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "0.99", ToolTip = "this decides how aggressive the auto gearbox could be. larger number makes the auto gearbox more aggressive."))
	float AutoGearboxShiftFactor = 0.95;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "this also decides how aggressive the auto gearbox is. Normally, larger throttle input causes more aggressive auto gearbox. But if the curve is set, the throttle input will be mapped to this curve."))
	UCurveFloat* AutoGearboxShiftFactorCurve = nullptr;
};
