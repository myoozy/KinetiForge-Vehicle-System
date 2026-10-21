// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VehicleDrivetrainStructs.h"
#include "VehiclePhysicsStep.h"
#include "VehicleDifferentialComponent.generated.h"

class UVehicleAxleAssemblyComponent;

/**
* Surprise! I'm also a transfer case.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class KINETIFORGE_API UVehicleDifferentialComponent : public UActorComponent
{
	GENERATED_BODY()
	friend class UVehicleDriveAssemblyComponent;
	friend class UVehicleAxleAssemblyComponent;
	friend struct FVehiclePowerLifetimeProbe;

public:	
	// Sets default values for this component's properties
	UVehicleDifferentialComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	FVehicleLimitedSlipDifferentialConfig Config;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginDestroy() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	// Differential configuration is stateless; several assemblies may legitimately share it.
	void AddPhysicsDependent(const TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe>& Step);
	void InvalidatePhysicsDependents();
	TArray<TWeakPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe>> PhysicsDependents; // GT only.
	// GT only. Consumers capture their current configuration, including shared differentials.
	void AddConfigurationConsumer(UActorComponent* Consumer);
	TArray<TWeakObjectPtr<UActorComponent>> ConfigurationConsumers;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleDifferential")
	const FVehicleLimitedSlipDifferentialConfig& GetConfig() { return Config; }

	UFUNCTION(BlueprintCallable, Category = "VehicleDifferential")
	void SetConfig(const FVehicleLimitedSlipDifferentialConfig& NewConfig);

	UFUNCTION(BlueprintCallable, Category = "VehicleDifferential")
	void UpdateOutputShaft(
		float InDriveTorque,
		float InLeftAngularVelocity,
		float InRightAngularVelocity,
		float InLeftTotalInertia,
		float InRightTotalInertia,
		float InDeltaTime,
		float InReflectedInertia,
		float& OutLeftTorque,
		float& OutRightTorque,
		float& OutReflectedInertiaEachWheel
	);
	UFUNCTION(BlueprintCallable, Category = "VehicleDifferential")
	void UpdateInputShaft(
		float InLeftOutputShaftAngularVelocity,
		float InRightOutputShaftAngularVelocity,
		float InLeftWheelInertia,
		float InRightWheelInertia,
		float& OutInputShaftVelocity,
		float& OutReflectedInertia
	);
	int32 SubstepTransferCase(
		TArrayView<UVehicleAxleAssemblyComponent* const> InAxles,
		float InSubstepDeltaTime,
		float InGearboxOutputTorque,
		float InBrakeValue,
		float InHandbrakeValue,
		bool bLineLockActive,
		float& OutTransmissionOutputShaftAngularVelocity,
		float& OutTransmissionOutputShaftEffectiveInertia,
		float& OutEffectiveDriveShaftStiffness);
	/** Fixed axle orchestration using an owned config snapshot; contains no component state access. */
	static int32 SubstepTransferCaseWithConfig(
		const FVehicleLimitedSlipDifferentialConfig& Config,
		TArrayView<UVehicleAxleAssemblyComponent* const> InAxles,
		float InSubstepDeltaTime,
		float InGearboxOutputTorque,
		float InBrakeValue,
		float InHandbrakeValue,
		bool bLineLockActive,
		float& OutTransmissionOutputShaftAngularVelocity,
		float& OutTransmissionOutputShaftEffectiveInertia,
		float& OutEffectiveDriveShaftStiffness);
	UFUNCTION(BlueprintCallable, Category = "VehicleDifferential")
	int32 UpdateTransferCase(
		const TArray<UVehicleAxleAssemblyComponent*>& InAxles,
		float InDeltaTime,
		float InGearboxOutputTorque,
		float InReflectedInertia,
		float InBrakeValue,
		float InHandbrakeValue,
		float InSteeringValue,
		bool bLineLockActive,
		float& OutTransmissionOutputShaftAngularVelocity,
		float& OutTransmissionOutputShaftEffectiveInertia,
		float& OutEffectiveDriveShaftStiffness);
	UFUNCTION(BlueprintCallable, Category = "VehicleDifferential")
	float CalculateEffectiveWheelRadius(
		const TArray<UVehicleAxleAssemblyComponent*>& InAxles
	);
	float CalculateEffectiveWheelRadius_Internal(
		TArrayView<UVehicleAxleAssemblyComponent* const> InAxles
	);

	void GetOutputTorque(
		float InTorque, 
		float InLeftAngularVelocity,
		float InRightAngularVelocity,
		float InLeftTotalInertia,
		float InRightTotalInertia,
		float InDeltaTime,
		float& OutLeftTorque,
		float& OutRightTorque);

	void GetOpenDiffOutputTorque(float InTorque, float& OutTorqueLeft, float& OutTorqueRight);

	float GetInputShaftVelocity(float OutputShaftAngularVelocityLeft, float OutputShaftAngularVelocityRight);
};
