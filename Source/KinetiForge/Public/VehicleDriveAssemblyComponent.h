// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "HAL/CriticalSection.h"
#include "Components/SceneComponent.h"
#include "VehicleInputStructs.h"
#include "VehicleDrivetrainStructs.h"
#include "VehicleAxleStructs.h"
#include "VehiclePhysicsBodyBinding.h"
#include "VehicleDriveAssemblyComponent.generated.h"

class UVehicleWheelComponent;
class UVehicleDifferentialComponent;
class UVehicleAxleAssemblyComponent;
class UVehicleWheelCoordinatorComponent;
class UVehicleEngineComponent;
class UVehicleClutchComponent;
class UVehicleGearboxComponent;
class UVehicleAsyncTickComponent;
class FVehiclePowerUnitBridge;
class FVehiclePhysicsStep;
class FVehicleDriveRuntime;
class FVehiclePhysicsExecutionContext;
class FVehiclePhysicsScene;
struct FVehicleGearboxRuntimeState;
struct FVehicleInputRuntimeSettings;
struct FVehicleInputStepContext;
enum class EVehicleInputAxis : uint8;

/**
* The DriveAssemblyComponent holds a a complete drivetrain. Including:
* - One Engine
* - One Clutch
* - One Transmission
* - One TransferCase
* - Any amount of Axles
*
* All the components above can be automatically generated or selected from existing components.
*
* Please attach this to a primitive component.
* 
* The vehicle physics will be automatically updated.
*
* A WheelCoordinatorComponent will be automatically generated if needed.
* A VehicleAsyncTickComponent will be automatically generated if needed.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class KINETIFORGE_API UVehicleDriveAssemblyComponent : public USceneComponent
{
    GENERATED_BODY()
    friend class FVehicleExistingBlueprintRuntimeTest;
    friend struct FVehicleDriveInputRegressionProbe;
    friend struct FVehicleDriveAssemblyRuntimeProbe;
    friend struct FVehicleDriveOwnershipProbe;
    friend struct FVehiclePowerLifetimeProbe;
    friend struct FVehicleDriveAssemblyIntegrationProbe;
    friend struct FVehicleDriveContactIntegrationProbe;
    friend struct FVehicleDriveAsyncLifecycleProbe;
    friend class UVehicleAsyncTickComponent;
    friend class UVehicleDifferentialComponent;

public:
    // Sets default values for this component's properties
    UVehicleDriveAssemblyComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    bool bUseExistingEngineComponent = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (GetOptions = "GetNamesOfEnginesOfOwner", EditCondition = "bUseExistingEngineComponent", EditConditionHides))
    FName EngineComponentName = FName();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (EditCondition = "!bUseExistingEngineComponent", EditConditionHides))
    TSubclassOf<UVehicleEngineComponent> EngineClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    bool bUseExistingClutchComponent = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (GetOptions = "GetNamesOfClutchesOfOwner", EditCondition = "bUseExistingClutchComponent", EditConditionHides))
    FName ClutchComponentName = FName();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (EditCondition = "!bUseExistingClutchComponent", EditConditionHides))
    TSubclassOf<UVehicleClutchComponent> ClutchClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    bool bUseExistingGearboxComponent = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (GetOptions = "GetNamesOfGearboxesOfOwner", EditCondition = "bUseExistingGearboxComponent", EditConditionHides))
    FName GearboxComponentName = FName();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (EditCondition = "!bUseExistingGearboxComponent", EditConditionHides))
    TSubclassOf<UVehicleGearboxComponent> GearboxClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    bool bUseExistingTransferCaseComponent = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (GetOptions = "GetNamesOfTransferCasesOfOwner", EditCondition = "bUseExistingTransferCaseComponent", EditConditionHides))
    FName TransferCaseComponentName = FName();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (EditCondition = "!bUseExistingTransferCaseComponent", EditConditionHides))
    TSubclassOf<UVehicleDifferentialComponent> TransferCaseClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    TArray<FVehicleAxleSpawnTemplate> AxleTemplates;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    FVehiclInputConfig InputConfig;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    FVehicleInputAssistConfig InputAssistConfig;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    FAutoGearboxConfig AutoGearboxConfig;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    bool bUpdatePhysicsAutomatically = true;

    /** Serialized compatibility field; production vehicles always use the whole-vehicle runtime. */
    UPROPERTY(BlueprintReadOnly, Category = "Setup", meta = (DeprecatedProperty, DeprecationMessage = "Vehicles now use one C++ runtime; this setting is ignored."))
    bool bUseIsolatedPowerUnitRuntime = false;
    /** Serialized compatibility field; false no longer selects the old component physics path. */
    UPROPERTY(BlueprintReadOnly, Category = "Setup", meta = (DeprecatedProperty, DeprecationMessage = "Vehicles now use one C++ runtime; this setting is ignored."))
    bool bUseIsolatedVehicleRuntime = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    bool bDrivetrainSubstepping = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
    float SubstepDeltaTime = 1.f / 240.f;

protected:
    // Called when the game starts
    virtual void BeginPlay() override;
    virtual void OnRegister() override;
    virtual void OnUnregister() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void BeginDestroy() override;
    virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    // GT-only compatibility helpers for unbound components; owned input state is advanced by the runtime.
    void UpdateInput(float InDeltaTime);
    void UpdateThrottle(float InDeltaTime);
    void UpdateBrake(float InDeltaTime);
    void UpdateClutch(float InDeltaTime);
    void UpdateSteering(float InDeltaTime);
    void UpdateHandbrake(float InDeltaTime);
    void UpdateAutomaticGearbox(float InDeltaTime);

    UFUNCTION(Server, Reliable)
    void ServerInputThrottle(float InValue, bool bDirectInput);
    UFUNCTION(NetMulticast, Reliable)
    void MultiCastInputThrottle(float InValue, bool bDirectInput);
    UFUNCTION(Server, Reliable)
    void ServerInputBrake(float InValue, bool bDirectInput);
    UFUNCTION(NetMulticast, Reliable)
    void MultiCastInputBrake(float InValue, bool bDirectInput);
    UFUNCTION(Server, Reliable)
    void ServerInputClutch(float InValue, bool bDirectInput);
    UFUNCTION(NetMulticast, Reliable)
    void MultiCastInputClutch(float InValue, bool bDirectInput);
    UFUNCTION(Server, Reliable)
    void ServerInputSteering(float InValue, bool bDirectInput);
    UFUNCTION(NetMulticast, Reliable)
    void MultiCastInputSteering(float InValue, bool bDirectInput);
    UFUNCTION(Server, Reliable)
    void ServerInputHandbrake(float InValue, bool bDirectInput);
    UFUNCTION(NetMulticast, Reliable)
    void MultiCastInputHandbrake(float InValue, bool bDirectInput);

    UFUNCTION(Server, Reliable)
    void ServerShiftToTargetGear(int32 InTargetGear, bool bImmediate);
    UFUNCTION(Server, Reliable)
    void ServerShiftFinishedCallback();
    UFUNCTION(NetMulticast, Reliable)
    void MultiCastShiftToTargetGear(int32 InTargetGear, bool bImmediate);
    UFUNCTION()
    void OnRep_ServerCurrentGear();

    UFUNCTION(NetMulticast, Reliable)
    void ServerStartVehicleEngine();
    UFUNCTION(NetMulticast, Reliable)
    void MultiCastStartVehicleEngine();
    UFUNCTION(NetMulticast, Reliable)
    void ServerShutVehicleEngine();
    UFUNCTION(NetMulticast, Reliable)
    void MultiCastShutVehicleEngine();
    UFUNCTION()
    void OnRep_ServerVehicleEngineOperationMode();

    UPROPERTY(ReplicatedUsing = OnRep_ServerCurrentGear)
    int32 ServerCurrentGear;
    UPROPERTY(ReplicatedUsing = OnRep_ServerVehicleEngineOperationMode)
    EVehicleEngineOperationMode ServerVehicleEngineOperationMode;

    UPROPERTY()
    TArray <TWeakObjectPtr<UVehicleAxleAssemblyComponent>> Axles;
    UPROPERTY()
    TWeakObjectPtr<UVehicleEngineComponent> Engine;
    UPROPERTY()
    TWeakObjectPtr<UVehicleClutchComponent> Clutch;
    UPROPERTY()
    TWeakObjectPtr<UVehicleGearboxComponent> Gearbox;
    UPROPERTY()
    TWeakObjectPtr<UVehicleDifferentialComponent> TransferCase;
    UPROPERTY()
    TWeakObjectPtr<UVehicleWheelCoordinatorComponent> WheelCoordinator;
    UPROPERTY()
    TWeakObjectPtr<UVehicleAsyncTickComponent> VehicleAsyncTickComponent;
    UPROPERTY()
    TWeakObjectPtr<UPrimitiveComponent> Chassis;

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

    /** Manual callers disable automatic updates. Overlapping calls and stopped entries are ignored.
     * Component lifecycle changes run on GT, outside an executing physics callback. */
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    void UpdatePhysics(float InDeltaTime);
    /** Context-aware manual entry for the isolated assembly. Disable automatic updates first.
     * Caller must already be executing in this scene's physics callback. */
    void UpdatePhysics(float InDeltaTime, const FVehiclePhysicsExecutionContext& Context);
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
    void PauseAutoGearbox(float InCoolDownTime = 5.f);
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    EVehicleEngineOperationMode StartVehicleEngine();
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    EVehicleEngineOperationMode ShutVehicleEngine();
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
        float InInterpSpeed = 0.f, 
        FVector2D InStartSpeed_meter_per_second = FVector2D(1.f, 5.f));
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    void GetInputValues(FVehicleInputPipeline& Out) { Out = InputValues; }
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    UVehicleWheelCoordinatorComponent* GetWheelCoordinator() { return WheelCoordinator.Get(); }
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleDriveAssembly")
    void GetAxles(TArray<UVehicleAxleAssemblyComponent*>& OutAxles);
    TArray<UVehicleAxleAssemblyComponent*> GetAxles();
    TArray<TWeakObjectPtr<UVehicleAxleAssemblyComponent>>& GetAxlesRef();
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleDriveAssembly")
    UVehicleEngineComponent* GetEngine() { return Engine.Get(); }
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleDriveAssembly")
    UVehicleClutchComponent* GetClutch() { return Clutch.Get(); }
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleDriveAssembly")
    UVehicleGearboxComponent* GetGearbox() { return Gearbox.Get(); }
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleDriveAssembly")
    UVehicleGearboxComponent* GetTransmission() { return Gearbox.Get(); }
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleDriveAssembly")
    UVehicleDifferentialComponent* GetTransferCase() { return TransferCase.Get(); }
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    void GetWheels(TArray<UVehicleWheelComponent*>& OutWheels);
    TArray<UVehicleWheelComponent*> GetWheels();
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    void DestroyTargetAxle(UVehicleAxleAssemblyComponent* InTargetAxle);
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    void GetVehicleSpeed(
        FVector& OutAbsoluteWorldLinearVelocity, 
        FVector& OutWorldLinearVelocity,
        FVector& OutLocalLinearVelocity,
        float& OutKph, 
        float& OutMph)
    {
        OutAbsoluteWorldLinearVelocity = FVector(AbsoluteWorldLinearVelocity);
        OutWorldLinearVelocity = FVector(WorldLinearVelocity);
        OutLocalLinearVelocity = FVector(LocalLinearVelocity);
        OutKph = LocalLinearVelocity.X * 3.6; 
        OutMph = LocalLinearVelocity.X * 2.237;
    }

    /*
    * Unit: m/s
    */
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    FVector GetWorldLinearVelocity() { return FVector(WorldLinearVelocity); }

    /*
    * Unit: cm
    */
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    float GetMaxTrackWidth();

    /*
    * Unit: cm
    */
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    float GetMaxWheelBase();

    /*
    * Unit: cm
    * Returns half size and origin of the collision of vehicle body.
    * And the transform of the collision box. (world or relative)
    */
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    void GetVehicleBound(
        FVector& OutExtent, 
        FVector& OutOrigin, 
        FTransform& OutTransform,
        bool bReturnWorldTransform = true
    );

    /*
    * Unit: m/s
    */
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    float GetVehicleForwardSpeed();
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
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    float GetSteeringValueFromAxles();
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    UPrimitiveComponent* GetChassis() { return Chassis.Get(); }
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    float GetMinTurningRadius();
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    float GetMaxSteeringAngle();
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    TArray<AActor*> GetRayCastHitActors();
    UFUNCTION(BlueprintCallable, Category = "VehicleDriveAssembly")
    TArray<UPrimitiveComponent*> GetRayCastHitComponents();

private:
    bool PrepareIsolatedAxles();
    TSharedPtr<FVehicleDriveRuntime, ESPMode::ThreadSafe> CaptureDriveRuntime();
    TArray<FVehiclePhysicsBodyIdentity> CaptureAssemblyBodyIdentities() const;
    TArray<FVehiclePhysicsBodyIdentity> BoundAssemblyBodies; // GT-only; detect body recreation without borrowing PT handles.
    TSharedPtr<FVehiclePhysicsScene, ESPMode::ThreadSafe> PhysicsScene;
    TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe> ScenePhysicsStep; // GT-only registration ownership.
    TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe> GetPhysicsStep() const;
    bool PreparePhysicsStepRegistration(); // GT; preflight all axle/wheel claims before publication.
    void RefreshInvalidatedPhysicsBinding();
    void StopPhysicsUpdates(); // GT lifecycle: close admission and drain before touching children.
    mutable FCriticalSection PhysicsStepMutex;
    TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe> PhysicsStep;
    FVehicleInputRuntimeSettings CaptureInputRuntimeSettings() const;
    FVehicleInputStepContext CaptureInputStepContext() const;
    void UpdateInputAxis(EVehicleInputAxis Axis, float InDeltaTime);
    void ApplyInputCommand(EVehicleInputAxis Axis, float Input, bool bDirectInput);
    FVehicleInputState RequestedInputRaw; // GT commands survive reads of older published results.
    bool InitializePowerUnitBridge();
    void RefreshPowerUnitBridge();
    void SubmitTransferCaseRuntimeSettings();
    TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> GetPowerUnitBridge() const;
    FVehicleGearboxRuntimeState GetGearboxStateForInput() const;
    FVehicleGearboxConfig GetGearboxConfigForInput() const;
    float GetEngineRPMForInput() const;
    TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> PowerUnitBridge;
    TWeakObjectPtr<UVehicleEngineComponent> BoundPowerUnitEngine;
    TWeakObjectPtr<UVehicleClutchComponent> BoundPowerUnitClutch;
    TWeakObjectPtr<UVehicleGearboxComponent> BoundPowerUnitGearbox;
    mutable FCriticalSection PowerUnitBridgeMutex; // Protects publication of the shared pointer, not simulation.
    bool bPowerUnitBridgeRequested = false; // Assigned at BeginPlay.
    bool bPowerUnitBridgeInitializationFailed = false;
    bool GeneratePowerUnit();
    int GenerateAxles();    //-1: no owner actor; -2: no valid Chassis; -3: there're already axles
    int SearchExistingAxles();

    UPROPERTY()
    TArray<TWeakObjectPtr<UActorComponent>> AutoGeneratedComponents;

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
