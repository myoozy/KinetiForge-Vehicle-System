// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VehicleDrivetrainStructs.h"
#include "VehicleClutchComponent.generated.h"

class UVehicleEngineComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class KINETIFORGE_API UVehicleClutchComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVehicleClutchComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	FVehicleClutchConfig Config;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FVehicleClutchSimState State;

	float GetTorqueSpringModel(
		const float DeltaTime,
		const float ClutchSlip,
		const float EngineInertia,
		const float GearboxReflectedInertia,
		const float GearboxInputShaftInertia,
		const float GearboxReflectedInertia_HighestGear);
	float GetTorqueDampingModel(
		const float DeltaTime,
		const float ClutchSlip,
		const float EngineInertia,
		const float GearboxReflectedInertia,
		const float GearboxInputShaftInertia);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleClutch")
	const FVehicleClutchConfig& GetConfig() { return Config; }

	UFUNCTION(BlueprintCallable, Category = "VehicleClutch")
	void SetConfig(const FVehicleClutchConfig& NewConfig) { Config = NewConfig; }

	UFUNCTION(BlueprintCallable, Category = "VehicleClutch")
	void UpdatePhysics(
		const float InDeltaTime, 
		const float InClutchValue, 
		const float InGearboxInputShaftVelocity, 
		const float InGearboxReflectedInertia,
		const float InGearboxInputShaftInertia,
		const float InCurrentGearRatio, 
		const float GearboxReflectedInertia_HighestGear,
		const float InEngineAngularVelocity,
		const float InEngineInertia,
		const float InEngineMaxTorque);
	UFUNCTION(BlueprintCallable, Category = "VehicleClutch")
	float GetCluchTorque();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleClutch")
	const FVehicleClutchSimState& GetClutchState() { return State; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleClutch", meta = (DeprecatedFunction, DeprecationMessage = "Please use GetClutchState instead!"))
	void GetClutchMovement(FVehicleClutchSimState& Out) { Out = State; }

private:
	float CalculateStiffness(float InFrequency, float InInertia);
	float CalculateCriticalDamping(float InFrequency, float InInertia);
};
