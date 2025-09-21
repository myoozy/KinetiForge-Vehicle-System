// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VehicleClutchComponent.generated.h"

class UVehicleEngineComponent;

UENUM(BlueprintType)
enum class EClutchSimMode : uint8
{
	SpringModel     UMETA(ToolTip = "more realistic, but not recommended for Arcade/EVs/Large physics time step"),
	DampingModel    UMETA(ToolTip = "always safe to use")
};

USTRUCT(BlueprintType)
struct FVehicleClutchConfig
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClutchSetup", meta = (ClampMin = "0.0", ToolTip = "The natural frequency that the clutch tries to reach, it should be lower than game physics frequency"))
	float NaturalFrequency = 120;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClutchSetup", meta = (ClampMin = "0.0", ClampMax = "1.0", ToolTip = "Attention!!! If SimMode is SpringModel, then Damping refers to damping ratio of the spring. If SimMode is DampingModel, then Damping refers to smoothing factor!"))
	float Damping = 0.1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClutchSetup", meta = (ClampMin = "0.0"))
	float Capacity = 1.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ClutchSetup", meta = (ClampMin = "0.0", ToolTip = "SpringModel: Using a spring to transmit torque. Using this model will cause engine speed fluctuations (especially when shifting gears with a sequential transmission and low damping ratio). It is recommended to use this model when the physical step size is short (because the natural angular frequency of the spring cannot exceed the game's physical simulation frequency). If the physical step size is large, stiffness can be increased by increasing damping ratio; DampingModel: Use critical damping to transfer torque. Critical damping is calculated based on the current rotational inertia and game physics frequency. The torque is then smoothed to avoid numerical jitter. The torque values are very smooth and do not cause speed jitter during gear changes. This model is recommended for electric vehicles (as electric vehicles typically do not have clutches)."))
	EClutchSimMode SimMode = EClutchSimMode::SpringModel;
};

USTRUCT(BlueprintType)
struct FVehicleClutchSimData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DeltaTime")
	float PhysicsDeltaTime = 0.00833333;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SpringStiffness = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SpringDamping = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float CriticalDamping = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float MaxNaturalFrequency = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float EngineInertia = 0.2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float EngineAngularVelocity = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float ClutchAngularVelocity = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float ClutchSlip = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float AngleDiff = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float ClutchLock = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float MaxClutchTorque = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float SmoothenedTorque = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float ClutchTorque = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float TotalInertia = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float ReflectedInertia = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float FirstGearReflectedInertia = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool ClutchSlipping = 0.f;
};

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

	FVehicleClutchSimData SimData;

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
	void GetClutchMovement(FVehicleClutchSimData& Out) { Out = SimData; }

private:
	float SafeDivide(float a, float b);
	float CalculateStiffness(float InFrequency, float InInertia);
	float CalculateCriticalDamping(float InFrequency, float InInertia);
};
