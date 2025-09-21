// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "VehicleDriveAssemblyComponent.generated.h"

class UVehicleWheelComponent;
class UVehicleDifferentialComponent;
class UVehicleAxleAssemblyComponent;
class UVehicleWheelCoordinatorComponent;
class UVehicleEngineComponent;
class UVehicleClutchComponent;
class UVehicleGearboxComponent;


USTRUCT(BlueprintType)
struct FAxleAssemblyConfig
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UVehicleAxleAssemblyComponent> AxleConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector AxlePosition = FVector(0.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bDiasbleSteering = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bDisableHandbrake = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bDisableTractionControl = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "if < 0, won't override"))
    float TorqueWeightOverride = -1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "if < 0, won't override"))
    float TrackWidthOverride = -1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UVehicleWheelComponent> WheelOverride = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UVehicleDifferentialComponent> DifferentialOverride = nullptr;
};

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
struct FAutoGearboxConfig
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "engage clutch when eg. changing gear or low rpm"))
    bool bAutomaticClutch = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "disable AutomaticClutch, and disable throttle when in N gear"))
    bool bEVClutchLogic = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "at which rpm the clutch should be (gradually) released"))
    FVector2D AutoClutchRange = FVector2D(1200, 2500);
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float RevMatchMaxThrottle = 0.6;
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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class KINETIFORGE_API UVehicleDriveAssemblyComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UVehicleDriveAssemblyComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    TSubclassOf<UVehicleEngineComponent> EngineConfig;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    TSubclassOf<UVehicleClutchComponent> ClutchConfig;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    TSubclassOf<UVehicleGearboxComponent> GearboxConfig;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
    float CentralDiffLockRatio = 0.f;   //range: 0 - 1
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    TArray<FAxleAssemblyConfig> AxleConfigs;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    FVehiclInputInterpSpeed InputConfig;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    FAutoGearboxConfig AutoGearboxConfig;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    bool bUpdatePhysicsAutomatically = true;

protected:
    // Called when the game starts
    virtual void BeginPlay() override;
    virtual void OnRegister() override;
    virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
    void UpdateAxles(
        float InGearboxOutputTorque, 
        float InReflectedInertia,
        float& OutGearboxOutputShaftAngularVelocity,
        float& OutInertia);
    void UpdateInput(float InDeltaTime);
    void UpdateThrottle(float InDeltaTime);
    void UpdateBrake(float InDeltaTime);
    void UpdateClutch(float InDeltaTime);
    void UpdateSteering(float InDeltaTime);
    void UpdateAutomaticGearbox(float InDeltaTime);

    UPROPERTY()
    TArray<UVehicleAxleAssemblyComponent*> Axles;
    UPROPERTY()
    UVehicleEngineComponent* Engine;
    UPROPERTY()
    UVehicleClutchComponent* Clutch;
    UPROPERTY()
    UVehicleGearboxComponent* Gearbox;
    UPROPERTY()
    UVehicleWheelCoordinatorComponent* WheelCoordinator;
    UPROPERTY()
    UPrimitiveComponent* Carbody;
    UPROPERTY()
    AActor* ParentActor;

    //physics
    float PhysicsDeltaTime;
    float Speed_kph;
    float AutoShiftTimer;
    int NumOfWheelsOnGround;
    bool bIsInAir;
    FVector LocalLinearVelocity;
    FVector WorldLinearVelocity;
    FVector LocalVelocityClamped;
    TArray<FVector2D> SpeedRangeOfEachGear;

    FVehicleInputValue InputValues;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    virtual void AsyncPhysicsTickComponent(float DeltaTime, float SimTime) override;

    UFUNCTION(BlueprintCallable, Category = "PhysicsThreadOnly")
    void UpdatePhysics(float InDeltaTime);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void InputThrottle(float InValue);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void InputBrake(float InValue);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void InputClutch(float InValue);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void InputSteering(float InValue);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void InputHandbrake(float InValue);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void ShiftToTargetGear(int32 InTargetGear, float InAutoShiftCoolDown = 5.f, bool bImmediate = false);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void ShiftUp(float InAutoShiftCoolDown = 5.f, bool bImmediate = false);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void ShiftDown(float InAutoShiftCoolDown = 5.f, bool bImmediate = false);
    UFUNCTION(BlueprintCallable, Category = "Input")
    EEngineState StartVehicleEngine();
    UFUNCTION(BlueprintCallable, Category = "Input")
    EEngineState ShutVehicleEngine();
    UFUNCTION(BlueprintCallable, Category = "Input")
    float InterpInputValueConstant(float Current, float InTarget, float InDeltaTime, FVector2D InInterpSpeed);
    UFUNCTION(BlueprintCallable, Category = "Input")
    float InterpInputValue(float Current, float InTarget, float InDeltaTime, FVector2D InInterpSpeed);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void RotateCamera(USceneComponent* InSpringArm, FVector2D InMouseInput, bool bInvertYAxis = false, float InMaxPitch = 80);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void UpdateDriftCamera(USceneComponent* InSpringArm, float InPitch, float InDriftCamRate = 1.f, float InDriftCamInterpSpeed = 5.f, float InDriftCamStartSpeed_mps = 5.f);
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Input")
    void GetInputValues(FVehicleInputValue& Out) { Out = InputValues; }
    UVehicleWheelCoordinatorComponent* GetWheelCoordinator() { return WheelCoordinator; }
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
    TArray<UVehicleAxleAssemblyComponent*> GetAxles() { return Axles; }
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
    UVehicleEngineComponent* GetEngine() { return Engine; }
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
    UVehicleClutchComponent* GetClutch() { return Clutch; }
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
    UVehicleGearboxComponent* GetGearbox() { return Gearbox; }
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
    TArray<UVehicleWheelComponent*> GetWheels();
    UFUNCTION(BlueprintCallable, Category = "Components")
    void DestroyTargetAxle(UVehicleAxleAssemblyComponent* InTargetAxle);
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
    void GetVehicleSpeed(float& OutKph, FVector& OutWorldLinearVelocity, FVector& OutLocalLinearVelocity) 
    { OutKph = Speed_kph;OutWorldLinearVelocity = WorldLinearVelocity;OutLocalLinearVelocity = LocalLinearVelocity; }
    UFUNCTION(BlueprintCallable, Category = "Physics")
    TArray<FVector2D> CalculateSpeedRangeOfEachGear();

private:
    UPrimitiveComponent* FindPhysicalParent();
    bool FindWheelCoordinator();
    bool GeneratePowerUnit();
    int GenerateAxles();    //-1: no owner actor; -2: no valid Carbody; -3: there're already axles
    float SafeDivide(float a, float b);
    FVector SafeDivide(FVector a, float b);
    float ClampToZero(float a, float Tolerance);
    FVector ClampToZero(FVector v, float Tolerance);
};
