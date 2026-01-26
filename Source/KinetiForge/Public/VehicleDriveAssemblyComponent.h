// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "VehicleInputStructs.h"
#include "VehicleDrivetrainStructs.h"
#include "VehicleDriveAssemblyComponent.generated.h"

class UVehicleWheelComponent;
class UVehicleDifferentialComponent;
class UVehicleAxleAssemblyComponent;
class UVehicleWheelCoordinatorComponent;
class UVehicleEngineComponent;
class UVehicleClutchComponent;
class UVehicleGearboxComponent;
class UVehicleAsyncTickComponent;

USTRUCT(BlueprintType)
struct KINETIFORGE_API FAxleAssemblyConfig
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Only by using existing manually created Components can the level sequence correctly recognize and record its animations."))
    bool bUseExistingComponent = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (GetOptions = "GetNamesOfAxlesOfOwner", EditCondition = "bUseExistingComponent", EditConditionHides))
    FName AxleComponentName = FName();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!bUseExistingComponent", EditConditionHides))
    TSubclassOf<UVehicleAxleAssemblyComponent> AxleConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!bUseExistingComponent", EditConditionHides))
    FVector AxlePosition = FVector(0.f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!bUseExistingComponent", EditConditionHides))
    bool bDiasbleSteering = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!bUseExistingComponent", EditConditionHides))
    bool bDisableHandbrake = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!bUseExistingComponent", EditConditionHides))
    bool bDisableTractionControl = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "if < 0, won't override", EditCondition = "!bUseExistingComponent", EditConditionHides))
    float TorqueWeightOverride = -1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "if < 0, won't override", EditCondition = "!bUseExistingComponent", EditConditionHides))
    float TrackWidthOverride = -1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!bUseExistingComponent", EditConditionHides))
    TSubclassOf<UVehicleWheelComponent> WheelOverride = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "!bUseExistingComponent", EditConditionHides))
    TSubclassOf<UVehicleDifferentialComponent> DifferentialOverride = nullptr;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class KINETIFORGE_API UVehicleDriveAssemblyComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UVehicleDriveAssemblyComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    bool bUseExistingEngineComponent = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (GetOptions = "GetNamesOfEnginesOfOwner", EditCondition = "bUseExistingEngineComponent", EditConditionHides))
    FName EngineComponentName = FName();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (EditCondition = "!bUseExistingEngineComponent", EditConditionHides))
    TSubclassOf<UVehicleEngineComponent> EngineConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    bool bUseExistingClutchComponent = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (GetOptions = "GetNamesOfClutchesOfOwner", EditCondition = "bUseExistingClutchComponent", EditConditionHides))
    FName ClutchComponentName = FName();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (EditCondition = "!bUseExistingClutchComponent", EditConditionHides))
    TSubclassOf<UVehicleClutchComponent> ClutchConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    bool bUseExistingGearboxComponent = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (GetOptions = "GetNamesOfGearboxesOfOwner", EditCondition = "bUseExistingGearboxComponent", EditConditionHides))
    FName GearboxComponentName = FName();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (EditCondition = "!bUseExistingGearboxComponent", EditConditionHides))
    TSubclassOf<UVehicleGearboxComponent> GearboxConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    bool bUseExistingTransferCaseComponent = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (GetOptions = "GetNamesOfTransferCasesOfOwner", EditCondition = "bUseExistingTransferCaseComponent", EditConditionHides))
    FName TransferCaseComponentName = FName();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (EditCondition = "!bUseExistingTransferCaseComponent", EditConditionHides))
    TSubclassOf<UVehicleDifferentialComponent> TransferCaseConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    TArray<FAxleAssemblyConfig> AxleConfigs;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    FVehiclInputConfig InputConfig;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    FVehicleInputAssistConfig InputAssistConfig;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    FAutoGearboxConfig AutoGearboxConfig;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    bool bUpdatePhysicsAutomatically = true;

protected:
    // Called when the game starts
    virtual void BeginPlay() override;
    virtual void OnRegister() override;
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
    virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    void UpdateInput(float InDeltaTime);
    void UpdateThrottle(float InDeltaTime);
    void UpdateBrake(float InDeltaTime);
    void UpdateClutch(float InDeltaTime);
    void UpdateSteering(float InDeltaTime);
    void UpdateAutomaticGearbox(float InDeltaTime);

    UFUNCTION(Server, Reliable, Category = "Input")
    void ServerInputThrottle(float InValue, bool bDirectInput);
    UFUNCTION(NetMulticast, Reliable, Category = "Input")
    void MultiCastInputThrottle(float InValue, bool bDirectInput);
    UFUNCTION(Server, Reliable, Category = "Input")
    void ServerInputBrake(float InValue, bool bDirectInput);
    UFUNCTION(NetMulticast, Reliable, Category = "Input")
    void MultiCastInputBrake(float InValue, bool bDirectInput);
    UFUNCTION(Server, Reliable, Category = "Input")
    void ServerInputClutch(float InValue, bool bDirectInput);
    UFUNCTION(NetMulticast, Reliable, Category = "Input")
    void MultiCastInputClutch(float InValue, bool bDirectInput);
    UFUNCTION(Server, Reliable, Category = "Input")
    void ServerInputSteering(float InValue, bool bDirectInput);
    UFUNCTION(NetMulticast, Reliable, Category = "Input")
    void MultiCastInputSteering(float InValue, bool bDirectInput);
    UFUNCTION(Server, Reliable, Category = "Input")
    void ServerInputHandbrake(float InValue, bool bDirectInput);
    UFUNCTION(NetMulticast, Reliable, Category = "Input")
    void MultiCastInputHandbrake(float InValue, bool bDirectInput);

    UFUNCTION(Server, Reliable, Category = "Input")
    void ServerShiftToTargetGear(int32 InTargetGear, bool bImmediate);
    UFUNCTION(Server, Reliable, Category = "Input")
    void ServerShiftFinishedCallback();
    UFUNCTION(NetMulticast, Reliable, Category = "Input")
    void MultiCastShiftToTargetGear(int32 InTargetGear, bool bImmediate);
    UFUNCTION()
    void OnRep_ServerCurrentGear();

    UFUNCTION(NetMulticast, Reliable, Category = "Input")
    void ServerStartVehicleEngine();
    UFUNCTION(NetMulticast, Reliable, Category = "Input")
    void MultiCastStartVehicleEngine();
    UFUNCTION(NetMulticast, Reliable, Category = "Input")
    void ServerShutVehicleEngine();
    UFUNCTION(NetMulticast, Reliable, Category = "Input")
    void MultiCastShutVehicleEngine();
    UFUNCTION()
    void OnRep_ServerVehicleEngineOperationMode();

    UPROPERTY(ReplicatedUsing = OnRep_ServerCurrentGear)
    int32 ServerCurrentGear;
    UPROPERTY(ReplicatedUsing = OnRep_ServerVehicleEngineOperationMode)
    EVehicleEngineOperationMode ServerVehicleEngineOperationMode;

    UPROPERTY()
    TArray <TObjectPtr<UVehicleAxleAssemblyComponent>> Axles;
    UPROPERTY()
    TObjectPtr<UVehicleEngineComponent> Engine;
    UPROPERTY()
    TObjectPtr<UVehicleClutchComponent> Clutch;
    UPROPERTY()
    TObjectPtr<UVehicleGearboxComponent> Gearbox;
    UPROPERTY()
    TObjectPtr<UVehicleDifferentialComponent> TransferCase;
    UPROPERTY()
    TObjectPtr<UVehicleWheelCoordinatorComponent> WheelCoordinator;
    UPROPERTY()
    TObjectPtr<UVehicleAsyncTickComponent> VehicleAsyncTickComponent;
    UPROPERTY()
    TObjectPtr<UPrimitiveComponent> Carbody;

    //physics
    float PhysicsDeltaTime;
    float AutoGearboxCount;
    int32 NumOfWheelsOnGround;
    int32 NumOfDriveAxles;
    bool bIsInAir;
    FVector3f AbsoluteWorldLinearVelocity;
    FVector3f LocalLinearVelocity;//relative to ground
    FVector3f WorldLinearVelocity;//relative to ground
    FVector3f LocalVelocityClamped;
    FVector3f WorldAcceleration;
    FVector3f LocalAcceleration;
    TArray<FVector2D> SpeedRangeOfEachGear;

    FVehicleInputPipeline InputValues;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    void UpdatePhysics(float InDeltaTime);
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    void InputThrottle(float InValue, bool bDirectInput = false);
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    void InputBrake(float InValue, bool bDirectInput = false);
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    void InputClutch(float InValue, bool bDirectInput = false);
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    void InputSteering(float InValue, bool bDirectInput = false);
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    void InputHandbrake(float InValue, bool bDirectInput = false);
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    void ShiftToTargetGear(int32 InTargetGear, float InAutoShiftCoolDown = 5.f, bool bImmediate = false);
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    void ShiftUp(float InAutoShiftCoolDown = 5.f, bool bImmediate = false);
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    void ShiftDown(float InAutoShiftCoolDown = 5.f, bool bImmediate = false);
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    EVehicleEngineOperationMode StartVehicleEngine();
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    EVehicleEngineOperationMode ShutVehicleEngine();
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    static void RotateCamera(
        USceneComponent* InSpringArm,
        FVector2D InMouseInput,
        bool bInvertYAxis = false, 
        float InMaxPitch = 80);
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly", meta = (ToolTip = "Unit of speed: m/s"))
    void StretchSpringArmBySpeed(
        USpringArmComponent* InSpringArm,
        float InInitialSpringArmLength = 500.f,
        UCurveFloat* InScaleCurve = nullptr);
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    void CameraLookAtVelocity(
        USceneComponent* InSpringArm, 
        float InPitch, 
        float InSensitivity = 1.f,
        float InInterpSpeed = 5.f, 
        float InStartSpeed_mps = 5.f);
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    void GetInputValues(FVehicleInputPipeline& Out) { Out = InputValues; }
    UVehicleWheelCoordinatorComponent* GetWheelCoordinator() { return WheelCoordinator; }
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleDriveAssembly")
    TArray<UVehicleAxleAssemblyComponent*> GetAxles() { return Axles; }
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleDriveAssembly")
    UVehicleEngineComponent* GetEngine() { return Engine; }
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleDriveAssembly")
    UVehicleClutchComponent* GetClutch() { return Clutch; }
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleDriveAssembly")
    UVehicleGearboxComponent* GetGearbox() { return Gearbox; }
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    TArray<UVehicleWheelComponent*> GetWheels();
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    void DestroyTargetAxle(UVehicleAxleAssemblyComponent* InTargetAxle);
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    void GetVehicleSpeed(
        FVector3f& OutAbsoluteWorldLinearVelocity, 
        FVector3f& OutWorldLinearVelocity,
        FVector3f& OutLocalLinearVelocity,
        float& OutKph, 
        float& OutMph)
    {
        OutAbsoluteWorldLinearVelocity = AbsoluteWorldLinearVelocity; 
        OutWorldLinearVelocity = WorldLinearVelocity;
        OutLocalLinearVelocity = LocalLinearVelocity;
        OutKph = LocalLinearVelocity.X * 3.6; 
        OutMph = LocalLinearVelocity.X * 2.237;
    }
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    void GetVehicleAcceleration(
        FVector3f& OutWorldAcceleration, 
        FVector3f& OutLocalAcceleration)
    {
        OutWorldAcceleration = WorldAcceleration; 
        OutLocalAcceleration = LocalAcceleration;
    }
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    float GetThrottleValue() { return InputValues.Final.Throttle; }
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    float GetBrakeValue() { return InputValues.Final.Brake; }
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    float GetClutchValue() { return InputValues.Final.Clutch; }
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    float GetSteeringValue() { return InputValues.Final.Steering; }
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    float GetHandbrakeValue() { return InputValues.Final.Handbrake; }
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    int32 GetCurrentGear();

private:
    bool GeneratePowerUnit();
    int GenerateAxles();    //-1: no owner actor; -2: no valid Carbody; -3: there're already axles
    int SearchExistingAxles();

    UFUNCTION()
    TArray<FName> GetNamesOfAxlesOfOwner();
    UFUNCTION()
    TArray<FName> GetNamesOfEnginesOfOwner();
    UFUNCTION()
    TArray<FName> GetNamesOfClutchesOfOwner();
    UFUNCTION()
    TArray<FName> GetNamesOfGearboxesOfOwner();
    UFUNCTION()
    TArray<FName> GetNamesOfTransferCasesOfOwner();
};
