// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VehicleGearboxComponent.generated.h"

class UVehicleAxleAssemblyComponent;

USTRUCT(BlueprintType)
struct FVehicleGearboxConfig
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "0.0", ToolTip = "Please also consider the inertia of the clutch disk"))
	float InputShaftInertia = 0.05;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "0.0"))
	float ShiftDelay = 0.2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "0.0"))
	float FirstGear = 3.636;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "0.0"))
	float TopGear = 0.842;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float GearRatioBias = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "1.0"))
	int32 NumberOfGears = 6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "1.0", ToolTip = "the number of reverse gears should be smaller than non-reverse gears"))
	int32 NumOfReverseGears = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Efficiency = 0.9;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	bool bSequentialGearbox = false;
};

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

	FTimerHandle GearShiftTimerHandle;
	TArray<float> GearRatios;
	TArray<float> ReverseGearRatios;
	float CurrentGearRatio;
	float P2MotorTorque = 0.f;
	int32 CurrentGear;
	int32 TargetGear;
	bool bIsInGear = true;
	bool bShouldCutThrottle = false;
	bool bShouldRevMatch = false;

	//cache
	float CachedFirstGear = -1;
	float CachedTopGear = -1;
	float CachedGearRatioBias = -1;
	int32 CachedNumGears = -1;
	int32 CachedNumRGears = -1;

	UFUNCTION()
	void PrepareShift();
	UFUNCTION()
	void FinalizeShift();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Physics")
	void ShiftToTargetGear(int32 InTargetGear, bool bImmediate = false);
	UFUNCTION(BlueprintCallable, Category = "Physics")
	void UpdateOutputShaft(float InClutchTorque, float& OutTorque, float& OutReflectedInertia);
	UFUNCTION(BlueprintCallable, Category = "Physics")
	void UpdateInputShaft(float InAxleVelocity, float InAxleInertia, float& OutClutchVelocity, float& OutReflectedInertia, float& OutCurrentGearRatio, float& OutFirstGearInertia);
	UFUNCTION(BlueprintCallable, Category = "Physics")
	float GetGearRatio(int InTarget);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	int32 GetCurrentGear() { return CurrentGear; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	float GetCurrentGearRatio() { return CurrentGearRatio; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	bool GetIsInGear() { return bIsInGear; }
	UFUNCTION(BlueprintCallable, Category = "Physics")
	void SetP2MotorTorque(float NewTorque) { P2MotorTorque = NewTorque; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	float GetP2MotorTorque() { return P2MotorTorque; }
	UFUNCTION(BlueprintCallable, Category = "Initialize")
	bool CalculateGearRatios();
	bool CalculateGearRatios(TArray<float>& LargerArray, TArray<float>& SmallerArray, bool bInverseSign = false);

	bool IsGearDateDirty();
	bool GetShouldCutThrottle() { return bShouldCutThrottle; }
	bool GetShouldRavMatch() { return bShouldRevMatch; }

private:
	float SafeDivide(float a, float b);
};
