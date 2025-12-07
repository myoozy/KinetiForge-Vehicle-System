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
	FVehicleEngineSimData SimData;

	void EngineAcceleration();
	void UpdateExhaust();
	float SafeDivide(float a, float b);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Physics")
	void UpdatePhysics(float InDeltaTime, float InThrottle, float InLoadTorque);

	UFUNCTION(BlueprintCallable, Category = "Physics")
	void GetEngineMovement(FVehicleEngineSimData& Out) { Out = SimData; }

	UFUNCTION(BlueprintCallable, Category = "Physics")
	void SetP1MotorTorque(float NewTorque) {SimData.P1MotorTorque = NewTorque; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	float GetP1MotorTorque() { return SimData.P1MotorTorque; }

	UFUNCTION(BlueprintCallable, Category = "Initialize")
	void Initialize();

	UFUNCTION(BlueprintCallable, Category = "Input")
	EVehicleEngineState StartVehicleEngine();

	UFUNCTION(BlueprintCallable, Category = "Input")
	EVehicleEngineState ShutVehicleEngine();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	float GetEngineTorque() { return SimData.EffectiveTorque; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	float GetMaxRPM() { return NAConfig.EngineMaxRPM; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	float GetIdleRPM() { return NAConfig.EngineIdleRPM; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	float GetRPM() { return SimData.EngineRPM; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	float GetAngularVelocity() { return SimData.EngineAngularVelocity; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	float GetTurboSpool() { return SimData.TurboSpool; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	float GetTurboPressure() { return SimData.TurboPressure; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	float GetBackfireIntensity() { return SimData.BackfireIntensity; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	float GetRealThrottleValue() { return SimData.RealThrottle; }

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
