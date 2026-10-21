// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VehicleDrivetrainStructs.h"
#include "VehiclePhysicsStep.h"
#include "VehicleClutchComponent.generated.h"

class UVehicleEngineComponent;
class FVehiclePowerUnitBridge;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class KINETIFORGE_API UVehicleClutchComponent : public UActorComponent
{
	GENERATED_BODY()
	friend class UVehicleDriveAssemblyComponent;
	friend struct FVehiclePowerLifetimeProbe;

public:	
	// Sets default values for this component's properties
	UVehicleClutchComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	FVehicleClutchConfig Config;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginDestroy() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	FVehiclePhysicsOwnership PhysicsOwnership;

	FVehicleClutchSimState State;
	TWeakPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> PowerUnitBridge;
	TAtomic<bool> bPowerUnitBridgeBound{ false };

	float GetTorqueSpringModel(
		const float DeltaTime,
		const float ClutchSlip,
		const float EngineInertia,
		const float GearboxReflectedInertia,
		const float DriveShaftStiffness);
	float GetTorqueDampingModel(
		const float DeltaTime,
		const float ClutchSlip,
		const float EngineInertia,
		const float GearboxReflectedInertia);
	float GetTorqueConstraintModel(
		const float DeltaTime,
		const float ClutchSlip,
		const float EngineInertia,
		const float GearboxReflectedInertia
	);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleClutch")
	const FVehicleClutchConfig& GetConfig() { return Config; }

	UFUNCTION(BlueprintCallable, Category = "VehicleClutch")
	void SetConfig(const FVehicleClutchConfig& NewConfig);

	/** True until this component has been assigned to a power-unit runtime. Game thread only. */
	bool CanBindPowerUnitBridge() const;
	/** Binds once and publishes the initial clutch configuration. */
	bool BindPowerUnitBridge(TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> InBridge);
	/** Refreshes the UObject-visible clutch result copy. Game thread only. */
	void RefreshPublishedRuntimeState();

	UFUNCTION(BlueprintCallable, Category = "VehicleClutch")
	void UpdatePhysics(
		const float InDeltaTime, 
		const float InClutchValue, 
		const float InGearboxInputShaftVelocity, 
		const float InGearboxReflectedInertia,
		const float InCurrentGearRatio,
		const float InDriveShaftStiffness,
		const float InEngineAngularVelocity,
		const float InEngineInertia,
		const float InEngineMaxTorque);
	UFUNCTION(BlueprintCallable, Category = "VehicleClutch")
	float GetCluchTorque();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleClutch")
	/** With a bound bridge this is a game-thread published view, not the live simulation state. */
	const FVehicleClutchSimState& GetClutchState() { return State; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleClutch", meta = (DeprecatedFunction, DeprecationMessage = "Please use GetClutchState instead!"))
	void GetClutchMovement(FVehicleClutchSimState& Out) { Out = State; }

};
