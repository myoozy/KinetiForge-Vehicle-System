// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Curves/CurveFloat.h"
#include "VehicleUtilities.h"
#include "VehicleDrivetrainStructs.h"
#include "VehiclePhysicsStep.h"
#include "VehicleEngineComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnTurboBlowOffDelegate);
DECLARE_DYNAMIC_DELEGATE(FOnBackfiringDelegate);

class FVehiclePowerUnitBridge;
struct FVehicleEngineRuntimeSettings;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class KINETIFORGE_API UVehicleEngineComponent : public UActorComponent
{
	GENERATED_BODY()
	friend class UVehicleDriveAssemblyComponent;
	friend struct FVehiclePowerLifetimeProbe;

	// Non-reflected access for the legacy characterization bench; no runtime API.
	friend struct FVehicleEngineLegacyProbe;

public:	
	// Sets default values for this component's properties
	UVehicleEngineComponent();

#if WITH_EDITORONLY_DATA
	/**
	* Determines how often the look up tables will be synced.
	* Set to 0 to sync in every frame.
	* Set to <0 to disable sync (for performance).
	* Only works in editor
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float ConfigSyncInterval = -1.f;
#endif

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	FVehicleNaturallyAspiratedEngineConfig NAConfig;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	FVehicleEngineTurboConfig TurboConfig;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	FVehicleEngineExhaustConfig ExhaustConfig;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginDestroy() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	FVehiclePhysicsOwnership PhysicsOwnership;

	bool bShouldTriggerTurboBlowOffCallback = false;
	bool bShouldTriggerBackfiringCallback = false;
	TArray<FOnTurboBlowOffDelegate> TurboBlowOffCallbacks;
	TArray<FOnBackfiringDelegate> BackfiringCallbacks;
	FVehicleEngineSimState State;
	FVehicleLUT<32> CachedTorqueLUT = FVehicleLUT<32>(1.f);
	TWeakPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> PowerUnitBridge;
	TAtomic<bool> bPowerUnitBridgeBound{ false };

#if WITH_EDITORONLY_DATA
	float TimeSinceLastConfigSync = 0.f;
#endif

	void EngineAcceleration(const float DeltaTime);
	void UpdateExhaust(const float DeltaTime);
	float EstimateThrottleForTorque(float TargetEffectiveTorque);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "VehicleEngine")
	void UpdatePhysics(float InDeltaTime, float InThrottle, float InLoadTorque, bool bDisableSpark = false);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleEngine")
	const FVehicleNaturallyAspiratedEngineConfig& GetNaturallyAspiratedEngineConfig() { return NAConfig; }

	UFUNCTION(BlueprintCallable, Category = "VehicleEngine")
	void SetNaturallyAspiratedEngineConfig(const FVehicleNaturallyAspiratedEngineConfig& NewConfig);

	UFUNCTION(BlueprintCallable, Category = "VehicleEngine")
	void SetNAConfig(const FVehicleNaturallyAspiratedEngineConfig& NewConfig) { SetNaturallyAspiratedEngineConfig(NewConfig); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleEngine")
	const FVehicleEngineTurboConfig& GetTurboConfig() { return TurboConfig; }

	UFUNCTION(BlueprintCallable, Category = "VehicleEngine")
	void SetTurboConfig(const FVehicleEngineTurboConfig& NewConfig);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleEngine")
	const FVehicleEngineExhaustConfig& GetExhaustConfig() { return ExhaustConfig; }

	UFUNCTION(BlueprintCallable, Category = "VehicleEngine")
	void SetExhaustConfig(const FVehicleEngineExhaustConfig& NewConfig);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleEngine")
	float GetMaxNaturallyAspiratedTorque() { return NAConfig.MaxEngineTorque; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleEngine")
	float GetMaxEngineTorque();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleEngine")
	float GetEngineInertia() { return NAConfig.EngineInertia; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleEngine")
	/** With a bound bridge this is a game-thread published view, not the live simulation state. */
	const FVehicleEngineSimState& GetEngineState() { return State; }

	UFUNCTION(BlueprintCallable, Category = "VehicleEngine", meta = (DeprecatedFunction, DeprecationMessage = "Please use GetEngineState instead!"))
	void GetEngineMovement(FVehicleEngineSimState& Out) { Out = State; }

	UFUNCTION(BlueprintCallable, Category = "VehicleEngine")
	void SetP1MotorTorque(float NewTorque);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleEngine")
	float GetP1MotorTorque() { return State.P1MotorTorque; }

	UFUNCTION(BlueprintCallable, Category = "VehicleEngine")
	void UpdateCachedLookUpTables();

	/** Copies scalar settings and the cached LUT; contains no UObject curve reference. Game thread only. */
	FVehicleEngineRuntimeSettings CaptureRuntimeSettings() const;
	/** True until this component has been assigned to a power-unit runtime. Game thread only. */
	bool CanBindPowerUnitBridge() const;
	/** Binds once and publishes initial settings. */
	bool BindPowerUnitBridge(TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> InBridge);
	/** Refreshes the UObject-visible result copy and pending callback flags. Game thread only. */
	void RefreshPublishedRuntimeState();

	UFUNCTION(BlueprintCallable, Category = "VehicleEngine")
	EVehicleEngineOperationMode StartVehicleEngine();

	UFUNCTION(BlueprintCallable, Category = "VehicleEngine")
	EVehicleEngineOperationMode ShutVehicleEngine();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleEngine")
	float GetEngineTorque() { return State.EffectiveTorque; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleEngine")
	EVehicleEngineOperationMode GetEngineOperationMode() { return State.OperationMode; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleEngine")
	float GetMaxRPM() { return NAConfig.EngineMaxRPM; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleEngine")
	float GetIdleRPM() { return NAConfig.EngineIdleRPM; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleEngine")
	float GetRPM() { return State.EngineRPM; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleEngine")
	float GetAngularVelocity() { return State.EngineAngularVelocity; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleEngine")
	float GetTurboSpool() { return State.TurboSpool; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleEngine")
	float GetTurboPressure() { return State.TurboPressure; }

	/**
	* Intensity < 0: no backfire; 
	* 0 <= Intensity < 1: backfiring, but only sound, no flame; 
	* Intensity >= 1: backfiring and flame is visible.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleEngine")
	float GetBackfireIntensity() { return State.BackfireIntensity; }

	/*
	* Actual throttle valve opening
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleEngine")
	float GetRealThrottleValue() { return State.RealThrottle; }

	/*
	* Use the function 'GetTurboPressure' and 'GetTurboSpool' to determine the sound effect.
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleEngine")
	void BindEventToOnTurboBlowOff(FOnTurboBlowOffDelegate InOnTurboBlowOff);

	/*
	* Use the function 'GetBackfireIntensity' to determine the sound & visual effect.
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleEngine")
	void BindEventToOnBackfiring(FOnBackfiringDelegate InOnBackfiring);

private:
	void SubmitRuntimeSettings();
	float RequestedP1MotorTorque = 0.f; // Game-thread command value; a delayed result must not undo a setter.
};
