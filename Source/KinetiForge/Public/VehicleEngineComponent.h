// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Curves/CurveFloat.h"
#include "VehicleDrivetrainStructs.h"
#include "VehicleEngineComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnTurboBlowOffDelegate);
DECLARE_DYNAMIC_DELEGATE(FOnBackfiringDelegate);

class UVehicleClutchComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class KINETIFORGE_API UVehicleEngineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVehicleEngineComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float ConfigSyncInterval = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	FVehicleNaturallyAspiratedEngineConfig NAConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	FVehicleEngineTurboConfig TurboConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	FVehicleEngineExhaustConfig ExhaustConfig;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool bShouldTriggerTurboBlowOffCallback = false;
	bool bShouldTriggerBackfiringCallback = false;
	TArray<FOnTurboBlowOffDelegate> TurboBlowOffCallbacks;
	TArray<FOnBackfiringDelegate> BackfiringCallbacks;
	FVehicleEngineSimState State;
	FRichCurve CachedTorqueCurve;
	float TimeSinceLastConfigSync = 0.f;

	void EngineAcceleration();
	void UpdateExhaust();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "VehicleEngine")
	void UpdatePhysics(float InDeltaTime, float InThrottle, float InLoadTorque);

	UFUNCTION(BlueprintCallable, Category = "VehicleEngine")
	void GetEngineMovement(FVehicleEngineSimState& Out) { Out = State; }

	UFUNCTION(BlueprintCallable, Category = "VehicleEngine")
	void SetP1MotorTorque(float NewTorque) {State.P1MotorTorque = NewTorque; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleEngine")
	float GetP1MotorTorque() { return State.P1MotorTorque; }

	UFUNCTION(BlueprintCallable, Category = "VehicleEngine")
	void Initialize();

	UFUNCTION(BlueprintCallable, Category = "VehicleEngine")
	void UpdateCachedRichCurve();

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

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleEngine", meta = (ToolTip = "Intensity < 0: no backfire; 0 <= Intensity < 1: backfiring, but only sound, no flame; Intensity >= 1: backfiring and flame is visible."))
	float GetBackfireIntensity() { return State.BackfireIntensity; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleEngine", meta = (ToolTip = "Actual throttle valve opening"))
	float GetRealThrottleValue() { return State.RealThrottle; }

	UFUNCTION(BlueprintCallable, Category = "VehicleEngine", meta = (ToolTip = "Use the function 'GetTurboPressure' and 'GetTurboSpool' to determine the sound effect."))
	void BindEventToOnTurboBlowOff(FOnTurboBlowOffDelegate InOnTurboBlowOff);

	UFUNCTION(BlueprintCallable, Category = "VehicleEngine", meta = (ToolTip = "Use the function 'GetBackfireIntensity' to determine the sound & visual effect."))
	void BindEventToOnBackfiring(FOnBackfiringDelegate InOnBackfiring);

private:
	float RPMToRad = PI / 30;
	float RadToRPM = 30 / PI;


	//cache
	float CachedStartFriction;
	float CachedEngineIdleRPM;
	float CachedFrictionCoefficient;
	float CachedMaxEngineTorque;
	float CachedTurboLag;
	float CachedTurboWasteGateLag;
	float CachedTurboNegPressure;
	float CachedTurboMaxPressure;
};
