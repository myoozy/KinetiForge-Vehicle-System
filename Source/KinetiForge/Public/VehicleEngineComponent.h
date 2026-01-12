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

	UFUNCTION(BlueprintCallable, Category = "Physics")
	void UpdatePhysics(float InDeltaTime, float InThrottle, float InLoadTorque);

	UFUNCTION(BlueprintCallable, Category = "Physics")
	void GetEngineMovement(FVehicleEngineSimState& Out) { Out = State; }

	UFUNCTION(BlueprintCallable, Category = "Physics")
	void SetP1MotorTorque(float NewTorque) {State.P1MotorTorque = NewTorque; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	float GetP1MotorTorque() { return State.P1MotorTorque; }

	UFUNCTION(BlueprintCallable, Category = "Initialize")
	void Initialize();

	UFUNCTION(BlueprintCallable, Category = "Initialize")
	void UpdateCachedRichCurve();

	UFUNCTION(BlueprintCallable, Category = "Input")
	EVehicleEngineOperationMode StartVehicleEngine();

	UFUNCTION(BlueprintCallable, Category = "Input")
	EVehicleEngineOperationMode ShutVehicleEngine();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	float GetEngineTorque() { return State.EffectiveTorque; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	EVehicleEngineOperationMode GetEngineOperationMode() { return State.OperationMode; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	float GetMaxRPM() { return NAConfig.EngineMaxRPM; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	float GetIdleRPM() { return NAConfig.EngineIdleRPM; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	float GetRPM() { return State.EngineRPM; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	float GetAngularVelocity() { return State.EngineAngularVelocity; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	float GetTurboSpool() { return State.TurboSpool; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	float GetTurboPressure() { return State.TurboPressure; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	float GetBackfireIntensity() { return State.BackfireIntensity; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	float GetRealThrottleValue() { return State.RealThrottle; }

	UFUNCTION(BlueprintCallable, Category = "Physics")
	void BindEventToOnTurboBlowOff(FOnTurboBlowOffDelegate InOnTurboBlowOff);

	UFUNCTION(BlueprintCallable, Category = "Physics")
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
