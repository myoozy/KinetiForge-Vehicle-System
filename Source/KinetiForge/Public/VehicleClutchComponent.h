// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	FVehicleClutchConfig Config;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FVehicleClutchSimState State;

	float GetTorqueSpringModel(
		float DeltaTime,
		float ClutchSlip,
		float EngineInertia,
		float GearboxReflectedInertia,
		float GearboxInputShaftInertia,
		float GearboxReflectedInertia_HighestGear);
	float GetTorqueDampingModel(
		float DeltaTime,
		float ClutchSlip,
		float EngineInertia,
		float GearboxReflectedInertia,
		float GearboxInputShaftInertia);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "VehicleClutch")
	void UpdatePhysics(float InDeltaTime, 
		float InClutchValue, 
		float InGearboxInputShaftVelocity, 
		float InGearboxReflectedInertia,
		float InGearboxInputShaftInertia,
		float InCurrentGearRatio, 
		float GearboxReflectedInertia_HighestGear,
		UVehicleEngineComponent* TargetEngine);
	UFUNCTION(BlueprintCallable, Category = "VehicleClutch")
	float GetCluchTorque();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleClutch")
	void GetClutchMovement(FVehicleClutchSimState& Out) { Out = State; }

private:
	float CalculateStiffness(float InFrequency, float InInertia);
	float CalculateCriticalDamping(float InFrequency, float InInertia);
};
