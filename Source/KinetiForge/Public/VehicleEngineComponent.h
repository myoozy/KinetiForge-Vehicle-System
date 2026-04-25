// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Curves/CurveFloat.h"
#include "VehicleUtilities.h"
#include "VehicleDrivetrainStructs.h"
#include "VehicleEngineComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnTurboBlowOffDelegate);
DECLARE_DYNAMIC_DELEGATE(FOnBackfiringDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class KINETIFORGE_API UVehicleEngineComponent : public UActorComponent
{
	GENERATED_BODY()

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

	bool bShouldTriggerTurboBlowOffCallback = false;
	bool bShouldTriggerBackfiringCallback = false;
	TArray<FOnTurboBlowOffDelegate> TurboBlowOffCallbacks;
	TArray<FOnBackfiringDelegate> BackfiringCallbacks;
	FVehicleEngineSimState State;
	FVehicleLUT<32> CachedTorqueLUT = FVehicleLUT<32>(1.f);

#if WITH_EDITORONLY_DATA
	float TimeSinceLastConfigSync = 0.f;
#endif

	void EngineAcceleration();
	void UpdateExhaust();

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
	const FVehicleEngineSimState& GetEngineState() { return State; }

	UFUNCTION(BlueprintCallable, Category = "VehicleEngine", meta = (DeprecatedFunction, DeprecationMessage = "Please use GetEngineState instead!"))
	void GetEngineMovement(FVehicleEngineSimState& Out) { Out = State; }

	UFUNCTION(BlueprintCallable, Category = "VehicleEngine")
	void SetP1MotorTorque(float NewTorque) {State.P1MotorTorque = NewTorque; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleEngine")
	float GetP1MotorTorque() { return State.P1MotorTorque; }

	UFUNCTION(BlueprintCallable, Category = "VehicleEngine")
	void UpdateCachedLUT();

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
	float RPMToRad = PI / 30;
	float RadToRPM = 30 / PI;
};
