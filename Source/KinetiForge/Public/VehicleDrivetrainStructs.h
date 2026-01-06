// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "VehicleDrivetrainStructs.generated.h"

/*****************************ENGINE******************************/

UENUM(BlueprintType)
enum class EVehicleEngineOperationMode : uint8
{
	On  UMETA(DisplayName = "EngineON"),
	Off UMETA(DisplayName = "EngineOFF"),
	Starting    UMETA(DisplayName = "EngineSTARTING"),
	Shutting    UMETA(DisplayName = "EngineSHUTTING")
};

USTRUCT(BlueprintType, Blueprintable)
struct KINETIFORGE_API FVehicleNaturallyAspiratedEngineConfig
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0"))
	float MaxEngineTorque = 400.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0", ToolTip = "normalized engine torque curve"))
	UCurveFloat* EngineTorqueCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0"))
	float EngineIdleRPM = 900.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0"))
	float EngineMaxRPM = 6000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0"))
	float EngineInertia = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0", ToolTip = "The internal friction of the engine at 0 rpm"))
	float StartFriction = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0", ToolTip = "The Slope of the internal friction of engine"))
	float FrictionCoefficient = 0.005f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0", AdvancedDisplay))
	float IdleThrottleInterpSpeed = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0", AdvancedDisplay))
	float RevLimiterTime = 0.05f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0", AdvancedDisplay, ToolTip = "If Lambda < 1.f, there will be unburnt fuel, which will cause back fireing"))
	float MaxPowerLambda = 0.85f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0", AdvancedDisplay, ToolTip = "If Lambda < 1.f, there will be unburnt fuel, which will cause back fireing"))
	float DeceleratingLambda = 0.9f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0", AdvancedDisplay))
	float UnburntFuelAccumulationRate = 2.f;
};

USTRUCT(BlueprintType, Blueprintable)
struct KINETIFORGE_API FVehicleEngineTurboConfig
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup", meta = (ClampMin = "0.0", ToolTip = "if max boost pressure > 0, will be considered as turbo charged"))
	float MaxBoostPressure = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup", meta = (ClampMin = "-1.0", ClampMax = "0.0", ToolTip = "Minimum pressure (vacuum, restriction from the intake) when throttle input is really small(but above 0)."))
	float StaticIntakeRestriction = -0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup", meta = (ClampMin = "0.0", ToolTip = "The engine RPM at which the turbo starts to generate positive pressure (spool up)."))
	float SpoolStartRPM = 1200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup", meta = (ClampMin = "0.0", ToolTip = "The engine RPM at which the turbo reaches maximum pressure."))
	float FullBoostRPM = 3500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup", meta = (ClampMin = "0.0", ToolTip = "Time in seconds to reach ~95% max boost."))
	float SpoolUpDuration = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup", meta = (ClampMin = "0.0", ToolTip = "Time in seconds to loose pressure when throttle is closed(BOV open)."))
	float PressureDecayDuration = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup", meta = (ClampMin = "0.0", ClampMax = "1.0", ToolTip = "Determines how effectively boost pressure converts to torque."))
	float BoostEfficiency = 0.7f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup", meta = (ToolTip = "Enables Anti-Lag System (bang-bang) to keep turbo spooled off-throttle."))
	bool bEnableAntiLag = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup", meta = (ClampMin = "0.0", ToolTip = "minimum rpm to trigger anti-lag system"))
	float AntiLagMinRPM = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup", meta = (ClampMin = "0.0", ToolTip = "When antilag is activated, attempt to maintain a proportion of the maximum boost value. e.g. MaxBoostPressure = 1.0bar, AntiLagTargetPressureRatio = 0.8, then anti-lag system will keep the target pressure at 0.8bar."))
	float AntiLagTargetPressureRatio = 0.9f;
};

USTRUCT(BlueprintType, Blueprintable)
struct KINETIFORGE_API FVehicleEngineExhaustConfig
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhaustSetup", meta = (ClampMin = "0.0", AdvancedDisplay))
	float ExhaustScavengingStrength = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhaustSetup", meta = (ClampMin = "0.0", AdvancedDisplay))
	float HeatUpRate = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhaustSetup", meta = (ClampMin = "0.0", AdvancedDisplay))
	float CoolDownRate = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhaustSetup", meta = (ClampMin = "0.0", ClampMax = "1.0", AdvancedDisplay))
	float FlashPoint = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhaustSetup", meta = (ClampMin = "0.0", ClampMax = "1.0", AdvancedDisplay))
	float IgnitionProbability = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhaustSetup", meta = (ClampMin = "0.0", AdvancedDisplay))
	float PopFuelThreshold = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhaustSetup", meta = (ClampMin = "0.0", AdvancedDisplay))
	float FlameFuelThreshold = 0.25f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExhaustSetup", meta = (ClampMin = "0.0", AdvancedDisplay))
	float BackfireHeatSpike = 0.15f;
};

USTRUCT(BlueprintType, Blueprintable)
struct KINETIFORGE_API FVehicleEngineSimState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EVehicleEngineOperationMode OperationMode = EVehicleEngineOperationMode::Starting;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fuel")
	bool bSpark = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fuel")
	bool bFuelInjection = false;
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fuel")
	float UnburntFuelBuffer = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turbo")
	float TurboSpool = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turbo")
	float TurboPressure = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Exhaust")
	float ExhaustHeat = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Exhaust")
	float BackfireIntensity = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float RevLimiterTimer = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force")
	float P1MotorTorque = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turbo")
	bool bIsTurboBlowingOff = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turbo")
	bool bIsAntiLagTriggered = false;
};

/*******************************CLUTCH********************************/


UENUM(BlueprintType)
enum class EClutchSimMode : uint8
{
	SpringModel     UMETA(ToolTip = "more realistic, but not recommended for Arcade/EVs/Large physics time step"),
	DampingModel    UMETA(ToolTip = "always safe to use")
};

USTRUCT(BlueprintType)
struct KINETIFORGE_API FVehicleClutchConfig
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
struct KINETIFORGE_API FVehicleClutchSimState
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
struct KINETIFORGE_API FVehicleGearboxConfig
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
struct KINETIFORGE_API FAutoGearboxConfig
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
