// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VehicleDifferentialComponent.generated.h"

USTRUCT(BlueprintType)
struct FVehicleLimitedSlipDifferentialConfig
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "0.0"))
	float GearRatio = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float LockRatio = 0.f;	//range: 0 - 1
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class KINETIFORGE_API UVehicleDifferentialComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVehicleDifferentialComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	FVehicleLimitedSlipDifferentialConfig Config;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "PhysicsLogic")
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
	UFUNCTION(BlueprintCallable, Category = "PhysicsLogic")
	void UpdateInputShaft(
		float InLeftOutputShaftAngularVelocity,
		float InRightOutputShaftAngularVelocity,
		float InLeftWheelInertia,
		float InRightWheelInertia,
		float& OutInputShaftVelocity,
		float& OutReflectedInertia
	);

private:
	UFUNCTION(BlueprintCallable, Category = "PhysicsLogic")
	void GetOutputTorque(
		float InTorque, 
		float InLeftAngularVelocity,
		float InRightAngularVelocity,
		float InLeftTotalInertia,
		float InRightTotalInertia,
		float InDeltaTime,
		float& OutLeftTorque,
		float& OutRightTorque);
	UFUNCTION(BlueprintCallable, Category = "PhysicsLogic")
	void GetOpenDiffOutputTorque(float InTorque, float& OutTorqueLeft, float& OutTorqueRight);

	UFUNCTION(BlueprintCallable, Category = "PhysicsLogic")
	float GetInputShaftVelocity(float OutputShaftAngularVelocityLeft, float OutputShaftAngularVelocityRight);
		
};
