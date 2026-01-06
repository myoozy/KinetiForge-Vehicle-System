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

	void UpdateSpringStiffness();
	float GetTorqueSpringModel();
	float GetTorqueDampingModel();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Physics")
	void UpdatePhysics(float InDeltaTime, 
		float InClutchValue, 
		float InGearboxInputShaftVelocity, 
		float InReflectedInertia, 
		float InCurrentGearRatio, 
		float InFirstGearReflectedInertia,
		UVehicleEngineComponent* TargetEngine);
	UFUNCTION(BlueprintCallable, Category = "Physics")
	float GetCluchTorque();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	void GetClutchMovement(FVehicleClutchSimState& Out) { Out = State; }

private:
	float SafeDivide(float a, float b);
	float CalculateStiffness(float InFrequency, float InInertia);
	float CalculateCriticalDamping(float InFrequency, float InInertia);
};
