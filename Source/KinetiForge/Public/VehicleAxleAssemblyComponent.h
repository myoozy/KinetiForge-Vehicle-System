// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "VehicleAxleStructs.h"
#include "VehicleAxleAssemblyComponent.generated.h"

class UVehicleDifferentialComponent;
class UVehicleWheelCoordinatorComponent;
class UVehicleWheelComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class KINETIFORGE_API UVehicleAxleAssemblyComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVehicleAxleAssemblyComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	TSubclassOf<UVehicleWheelComponent> WheelConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	FRotator VehicleWheelComponentSetupRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	TSubclassOf<UVehicleDifferentialComponent> DifferentialConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	EVehicleAxleLayout AxleLayout = EVehicleAxleLayout::TwoWheels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	FVehicleAxleConfig AxleConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	FVehicleAxleSteeringConfig AxleSteeringConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	FVehicleSteeringAssistConfig SteeringAssistConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	FVehicleTCSConfig TCSConfig;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnRegister() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> Carbody;
	UPROPERTY()
	TObjectPtr<UVehicleWheelCoordinatorComponent> WheelCoordinator;
	
	FVehicleAxleSimData SimData;

	UPROPERTY()
	TObjectPtr<UVehicleWheelComponent> LeftWheel;
	UPROPERTY()
	TObjectPtr<UVehicleWheelComponent> RightWheel;
	UPROPERTY()
	TObjectPtr<UVehicleDifferentialComponent> Differential;

	void UpdateTwoWheelAxle(float InDriveTorque, float InReflectedInertia);
	void UpdateSingleWheelAxle(float InDriveTorque, float InReflectedInertia);
	void UpdateSteering(float InSteeringInput);
	void UpdateSteeringAssist(float InSteeringInput);
	void CalculateLinearVelocity();
	void UpdateSwaybarForce();
	void UpdateTCS(float TargetDriveTorque);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Physics")
	void UpdatePhysics(
		float InPhysicsDeltaTime,
		float InDriveTorque,
		float InBrakeInput,
		float InHandbrakeInput,
		float InSteeringInput,
		float InReflectedInertia,
		float& OutAxleTotalInertia, 
		float& OutAngularVelocity);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Component")
	void GetWheels(UVehicleWheelComponent*& OutLeftWheel, UVehicleWheelComponent*& OutRightWheel)
	{ OutLeftWheel = LeftWheel; OutRightWheel = RightWheel; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Component")
	void GetDifferential(UVehicleDifferentialComponent*& OutDifferential)
	{ OutDifferential = Differential; }
	UFUNCTION(BlueprintCallable, Category = "Physics")
	void GetAxleMovement(FVehicleAxleSimData& Out) { Out = SimData; }
	UFUNCTION(BlueprintCallable, Category = "Physics")
	void SetP3MotorTorque(float NewTorque) { SimData.P3MotorTorque = NewTorque; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	float GetP3MotorTorque() { return SimData.P3MotorTorque; }
	UFUNCTION(BlueprintCallable, Category = "Physics")
	void SetWheelPosition(float NewTrackWidth);
	UFUNCTION(BlueprintCallable, Category = "Physics")
	void UpdateTrackWidth();

	void SetWheelBase(float NewWheelBase) { SimData.WheelBase = NewWheelBase; }
	void GetLinearVelocity(FVector& OutLocalVelocity, FVector& OutWorldVelocity);
	float GetAngularVelocity() { return SimData.AxleAngularVelocity; }
	float GetTotalAxleInertia() { return SimData.AxleTotalInertia; }
	int32 GetNumOfWheelsOnGround() { return SimData.NumOfWheelOnGround; }
	FVector GetAxleCenter();

private:
	bool GenerateWheels();
	bool GenerateDifferential();
	bool GenerateComponents();
};
