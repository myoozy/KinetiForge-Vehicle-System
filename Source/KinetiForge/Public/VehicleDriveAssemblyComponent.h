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
struct FAxleAssemblyConfig
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Only by using existing manually created Components can the level sequence correctly recognize and record its animations."))
    bool bUseExistingComponent = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "bUseExistingComponent", EditConditionHides))
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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (EditCondition = "bUseExistingEngineComponent", EditConditionHides))
    FName EngineComponentName = FName();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (EditCondition = "!bUseExistingEngineComponent", EditConditionHides))
    TSubclassOf<UVehicleEngineComponent> EngineConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    bool bUseExistingClutchComponent = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (EditCondition = "bUseExistingClutchComponent", EditConditionHides))
    FName ClutchComponentName = FName();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (EditCondition = "!bUseExistingClutchComponent", EditConditionHides))
    TSubclassOf<UVehicleClutchComponent> ClutchConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    bool bUseExistingGearboxComponent = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (EditCondition = "bUseExistingGearboxComponent", EditConditionHides))
    FName GearboxComponentName = FName();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (EditCondition = "!bUseExistingGearboxComponent", EditConditionHides))
    TSubclassOf<UVehicleGearboxComponent> GearboxConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    bool bUseExistingTransferCaseComponent = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (EditCondition = "bUseExistingTransferCaseComponent", EditConditionHides))
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
    void ServerInputThrottle(float InValue);
    UFUNCTION(NetMulticast, Reliable, Category = "Input")
    void MultiCastInputThrottle(float InValue);
    UFUNCTION(Server, Reliable, Category = "Input")
    void ServerInputBrake(float InValue);
    UFUNCTION(NetMulticast, Reliable, Category = "Input")
    void MultiCastInputBrake(float InValue);
    UFUNCTION(Server, Reliable, Category = "Input")
    void ServerInputClutch(float InValue);
    UFUNCTION(NetMulticast, Reliable, Category = "Input")
    void MultiCastInputClutch(float InValue);
    UFUNCTION(Server, Reliable, Category = "Input")
    void ServerInputSteering(float InValue);
    UFUNCTION(NetMulticast, Reliable, Category = "Input")
    void MultiCastInputSteering(float InValue);
    UFUNCTION(Server, Reliable, Category = "Input")
    void ServerInputHandbrake(float InValue);
    UFUNCTION(NetMulticast, Reliable, Category = "Input")
    void MultiCastInputHandbrake(float InValue);

    UFUNCTION(Server, Reliable, Category = "Input")
    void ServerShiftToTargetGear(int32 InTargetGear, bool bImmediate = false);
    UFUNCTION(Server, Reliable, Category = "Input")
    void ServerShiftFinishedCallback();
    UFUNCTION(NetMulticast, Reliable, Category = "Input")
    void MultiCastShiftToTargetGear(int32 InTargetGear, bool bImmediate = false);
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
    void OnRep_ServerVehicleEngineState();

    UPROPERTY(ReplicatedUsing = OnRep_ServerCurrentGear)
    int32 ServerCurrentGear;
    UPROPERTY(ReplicatedUsing = OnRep_ServerVehicleEngineState)
    EVehicleEngineState ServerVehicleEngineState;

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
    FVector AbsoluteWorldLinearVelocity;
    FVector LocalLinearVelocity;//relative to ground
    FVector WorldLinearVelocity;//relative to ground
    FVector LocalVelocityClamped;
    FVector WorldAcceleration;
    FVector LocalAcceleration;
    TArray<FVector2D> SpeedRangeOfEachGear;

    FVehicleInputPipeline InputValues;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Physics")
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
    EVehicleEngineState StartVehicleEngine();
    UFUNCTION(BlueprintCallable, Category = "Input")
    EVehicleEngineState ShutVehicleEngine();
    UFUNCTION(BlueprintCallable, Category = "Input")
    void RotateCamera(
        USceneComponent* InSpringArm,
        FVector2D InMouseInput,
        bool bInvertYAxis = false, 
        float InMaxPitch = 80);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void CameraLookAtVelocity(
        USceneComponent* InSpringArm, 
        float InPitch, 
        float InDriftCamRate = 1.f,
        float InDriftCamInterpSpeed = 5.f, 
        float InDriftCamStartSpeed_mps = 5.f);
    UFUNCTION(BlueprintCallable, Category = "Input")
    void GetInputValues(FVehicleInputPipeline& Out) { Out = InputValues; }
    UVehicleWheelCoordinatorComponent* GetWheelCoordinator() { return WheelCoordinator; }
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
    TArray<UVehicleAxleAssemblyComponent*> GetAxles() { return Axles; }
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
    UVehicleEngineComponent* GetEngine() { return Engine; }
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
    UVehicleClutchComponent* GetClutch() { return Clutch; }
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Components")
    UVehicleGearboxComponent* GetGearbox() { return Gearbox; }
    UFUNCTION(BlueprintCallable, Category = "Components")
    TArray<UVehicleWheelComponent*> GetWheels();
    UFUNCTION(BlueprintCallable, Category = "Components")
    void DestroyTargetAxle(UVehicleAxleAssemblyComponent* InTargetAxle);
    UFUNCTION(BlueprintCallable, Category = "Physics")
    void GetVehicleSpeed(
        FVector& OutAbsoluteWorldLinearVelocity, 
        FVector& OutWorldLinearVelocity,
        FVector& OutLocalLinearVelocity,
        float& OutKph, 
        float& OutMph)
    {
        OutAbsoluteWorldLinearVelocity = AbsoluteWorldLinearVelocity; 
        OutWorldLinearVelocity = WorldLinearVelocity;
        OutLocalLinearVelocity = LocalLinearVelocity;
        OutKph = LocalLinearVelocity.X * 3.6; 
        OutMph = LocalLinearVelocity.X * 2.237;
    }
    UFUNCTION(BlueprintCallable, Category = "Physics")
    void GetVehicleAcceleration(
        FVector& OutWorldAcceleration, 
        FVector& OutLocalAcceleration)
    {
        OutWorldAcceleration = WorldAcceleration; 
        OutLocalAcceleration = LocalAcceleration;
    }

private:
    bool GeneratePowerUnit();
    int GenerateAxles();    //-1: no owner actor; -2: no valid Carbody; -3: there're already axles
    int SearchExistingAxles();
    float SafeDivide(float a, float b);
    FVector SafeDivide(FVector a, float b);
    float ClampToZero(float a, float Tolerance);
    FVector ClampToZero(FVector v, float Tolerance);
};
