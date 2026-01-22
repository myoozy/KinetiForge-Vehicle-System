// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VehicleDrivetrainStructs.h"
#include "VehicleGearboxComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnShiftFinishedDelegate);

class UVehicleAxleAssemblyComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class KINETIFORGE_API UVehicleGearboxComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVehicleGearboxComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	FVehicleGearboxConfig Config;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FOnShiftFinishedDelegate ShiftFinishedCallback;
	FTimerHandle GearShiftTimerHandle;
	TArray<float> GearRatios;
	TArray<float> ReverseGearRatios;
	float CurrentGearRatio;
	float P2MotorTorque = 0.f;
	int32 CurrentGear;
	int32 TargetGear;
	bool bIsInGear = true;
	bool bShouldRevMatch = false;

	//cache
	float CachedFirstGear = -1;
	float CachedTopGear = -1;
	float CachedGearRatioBias = -1;
	int32 CachedNumGears = -1;
	int32 CachedNumRGears = -1;

	void StartShift(int32 InTargetGear, bool bImmediate = false);
	void FinalizeShift();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "VehicleGearbox")
	void ShiftToTargetGear(int32 InTargetGear, bool bImmediate = false);

	UFUNCTION(BlueprintCallable, Category = "VehicleGearbox")
	void ShiftToTargetGearWithDelegate(FOnShiftFinishedDelegate InOnShiftFinished, int32 InTargetGear, bool bImmediate = false);

	UFUNCTION(BlueprintCallable, Category = "VehicleGearbox")
	void UpdateOutputShaft(
		float InClutchTorque, 
		float& OutTorque, 
		float& OutReflectedInertia
	);

	UFUNCTION(BlueprintCallable, Category = "VehicleGearbox")
	void UpdateInputShaft(
		float InAxleVelocity, 
		float InAxleInertia, 
		float& OutClutchVelocity, 
		float& OutReflectedInertia,
		float& OutInputShaftInertia,
		float& OutCurrentGearRatio, 
		float& GearboxReflectedInertia_HighestGear
	);

	UFUNCTION(BlueprintCallable, Category = "VehicleGearbox")
	float GetGearRatio(int InTarget);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleGearbox")
	int32 GetCurrentGear() { return CurrentGear; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleGearbox")
	float GetCurrentGearRatio() { return CurrentGearRatio; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleGearbox")
	bool GetIsInGear() { return bIsInGear; }

	UFUNCTION(BlueprintCallable, Category = "VehicleGearbox")
	void SetP2MotorTorque(float NewTorque) { P2MotorTorque = NewTorque; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleGearbox")
	float GetP2MotorTorque() { return P2MotorTorque; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleGearbox")
	void CalculateSpeedRangeOfEachGear(
		float InEffectiveWheelRadius,
		float InEngineIdleRPM,
		float InEngineMaxRPM,
		TArray<FVector2D>& OutSpeedRanges
	);

	UFUNCTION(BlueprintCallable, Category = "VehicleGearbox", meta = (ToolTip = "Calculate and update gear ratios"))
	bool CalculateGearRatios();
	bool CalculateGearRatios(TArray<float>& LargerArray, TArray<float>& SmallerArray, bool bInverseSign = false);

	bool IsGearDataDirty();
	bool GetShouldRevMatch() { return bShouldRevMatch; }
};
