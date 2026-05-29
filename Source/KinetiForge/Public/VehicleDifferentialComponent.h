// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VehicleDrivetrainStructs.h"
#include "VehicleDifferentialComponent.generated.h"

class UVehicleAxleAssemblyComponent;

/**
* Surprise! I'm also a transfer case.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class KINETIFORGE_API UVehicleDifferentialComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVehicleDifferentialComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	FVehicleLimitedSlipDifferentialConfig Config;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleDifferential")
	const FVehicleLimitedSlipDifferentialConfig& GetConfig() { return Config; }

	UFUNCTION(BlueprintCallable, Category = "VehicleDifferential")
	void SetConfig(const FVehicleLimitedSlipDifferentialConfig& NewConfig) { Config = NewConfig; }

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
	UFUNCTION(BlueprintCallable, Category = "VehicleDifferential")
	int32 SubstepTransferCase(
		const TArray<UVehicleAxleAssemblyComponent*>& InAxles,
		float InSubstepDeltaTime,
		float InGearboxOutputTorque,
		float InReflectedInertia,
		float InBrakeValue,
		float InHandbrakeValue,
		bool bLineLockActive,
		float& OutTransmissionOutputShaftAngularVelocity,
		float& OutTransmissionOutputShaftEffectiveInertia);
	int32 SubstepTransferCase_Internal(
		TArrayView<UVehicleAxleAssemblyComponent* const> InAxles,
		float InSubstepDeltaTime,
		float InGearboxOutputTorque,
		float InReflectedInertia,
		float InBrakeValue,
		float InHandbrakeValue,
		bool bLineLockActive,
		float& OutTransmissionOutputShaftAngularVelocity,
		float& OutTransmissionOutputShaftEffectiveInertia);
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
		float& OutTransmissionOutputShaftEffectiveInertia);
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
