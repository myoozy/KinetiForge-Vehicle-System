// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VehicleDrivetrainStructs.h"
#include "VehicleGearboxSolver.h"
#include "VehiclePhysicsStep.h"
#include "VehicleGearboxComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnShiftFinishedDelegate);

class UVehicleAxleAssemblyComponent;
class FVehiclePowerUnitBridge;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class KINETIFORGE_API UVehicleGearboxComponent : public UActorComponent
{
	GENERATED_BODY()
	friend class UVehicleDriveAssemblyComponent;
	friend struct FVehiclePowerLifetimeProbe;

public:	
	// Sets default values for this component's properties
	UVehicleGearboxComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	FVehicleGearboxConfig Config;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginDestroy() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	FVehiclePhysicsOwnership PhysicsOwnership;

	FOnShiftFinishedDelegate ShiftFinishedCallback;
	FTimerHandle GearShiftTimerHandle;
	TArray<float> GearRatios;
	TArray<float> ReverseGearRatios;
	float CurrentGearRatio;
	float P2MotorTorque = 0.f;
	int32 CurrentGear;
	int32 TargetGear;
	bool bIsInGear = true;
	bool bShouldRevMatch = false;
	bool bShouldCutSpark = false;

	//cache
	float CachedFirstGear = -1;
	float CachedTopGear = -1;
	float CachedGearRatioBias = -1;
	int32 CachedNumGears = -1;
	int32 CachedNumRGears = -1;

	void StartShift(int32 InTargetGear, bool bImmediate = false);
	void FinalizeShift();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleGearbox")
	const FVehicleGearboxConfig& GetConfig() { return Config; }

	UFUNCTION(BlueprintCallable, Category = "VehicleGearbox")
	void SetConfig(const FVehicleGearboxConfig& NewConfig);

	/** Game-thread authority snapshot; the existing World timer owns gear selection. */
	FVehicleGearboxRuntimeState CaptureRuntimeSettingsState() const { return CaptureRuntimeState(); }
	bool CanBindPowerUnitBridge() const;
	bool BindPowerUnitBridge(TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> InBridge);
	void SubmitRuntimeSettings();

	UFUNCTION(BlueprintCallable, Category = "VehicleGearbox")
	void ShiftToTargetGear(int32 InTargetGear, bool bImmediate = false);

	UFUNCTION(BlueprintCallable, Category = "VehicleGearbox")
	void ShiftToTargetGearWithDelegate(FOnShiftFinishedDelegate InOnShiftFinished, int32 InTargetGear, bool bImmediate = false);

	UFUNCTION(BlueprintCallable, Category = "VehicleGearbox")
	void UpdateOutputShaft(
		float InClutchTorque, 
		float& OutTorque
	);

	UFUNCTION(BlueprintCallable, Category = "VehicleGearbox")
	void UpdateInputShaft(
		float InAxleVelocity,
		float InAxleInertia,
		float InDriveShaftStiffness,
		float& OutClutchVelocity,
		float& OutReflectedInertia,
		float& OutReflectedDriveShaftStiffness,
		float& OutCurrentGearRatio
	);

	UFUNCTION(BlueprintCallable, Category = "VehicleGearbox")
	float GetGearRatio(int InTarget);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleGearbox")
	int32 GetCurrentGear() { return CurrentGear; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleGearbox")
	float GetCurrentGearRatio() { return CurrentGearRatio; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleGearbox")
	bool GetIsInGear() { return bIsInGear; }

	UFUNCTION(BlueprintCallable, Category = "VehicleGearbox")
	void SetP2MotorTorque(float NewTorque);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleGearbox")
	float GetP2MotorTorque() { return P2MotorTorque; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleGearbox")
	void CalculateSpeedRangeOfEachGear(
		float InEffectiveWheelRadius,
		float InEngineIdleRPM,
		float InEngineMaxRPM,
		TArray<FVector2D>& OutSpeedRanges
	);

	UFUNCTION(BlueprintCallable, Category = "VehicleGearbox", meta = (ToolTip = "Calculate and update gear ratios"))
	bool CalculateGearRatios();
	bool CalculateGearRatios(TArray<float>& LargerArray, TArray<float>& SmallerArray, bool bInverseSign = false);

	bool IsGearDataDirty();
	bool GetShouldRevMatch() { return bShouldRevMatch; }
	bool GetShouldCutSpark() { return bShouldCutSpark; }

private:
	// Adapt legacy protected fields without changing C++ subclass access.
	FVehicleGearboxRuntimeState CaptureRuntimeState() const;
	void ApplyRuntimeState(const FVehicleGearboxRuntimeState& RuntimeState);
	TWeakPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> PowerUnitBridge;
	bool bPowerUnitBridgeBound = false; // Accessed only by game-thread control APIs.
};
