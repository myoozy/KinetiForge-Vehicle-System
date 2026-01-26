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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ToolTip = "Select whether to use existing Components. If not, dynamically created wheels can still be correctly identified and have their animations recorded by the level sequence."))
	bool bUseExistingWheelComponent = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (GetOptions = "GetNamesOfWheelsOfOwner", EditCondition = "bUseExistingWheelComponent", EditConditionHides))
	FName LeftWheelComponentName = FName();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (GetOptions = "GetNamesOfWheelsOfOwner", EditCondition = "bUseExistingWheelComponent", EditConditionHides))
	FName RightWheelComponentName = FName();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (EditCondition = "!bUseExistingWheelComponent", EditConditionHides))
	TSubclassOf<UVehicleWheelComponent> WheelConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (EditCondition = "!bUseExistingWheelComponent", EditConditionHides))
	FRotator VehicleWheelComponentSetupRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	bool bUseExistingDifferentialComponent = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (GetOptions = "GetNamesOfDifferentialsOfOwner", EditCondition = "bUseExistingDifferentialComponent", EditConditionHides))
	FName DifferentialComponentName = FName();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (EditCondition = "!bUseExistingDifferentialComponent", EditConditionHides))
	TSubclassOf<UVehicleDifferentialComponent> DifferentialConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	EVehicleAxleLayout AxleLayout = EVehicleAxleLayout::TwoWheels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	EVehicleAxleSuspensionType SuspensionType = EVehicleAxleSuspensionType::Independent;
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
	void UpdateIndependentSuspensionPhysics();
	void UpdateSolidAxlePhysics();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	static void CopyAxleConfig(const UVehicleAxleAssemblyComponent* Source, UVehicleAxleAssemblyComponent* Target, bool bReInitializeWheel = false);

	void InitializeWheels();

	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	void UpdatePhysics(
		float InPhysicsDeltaTime,
		float InDriveTorque,
		float InBrakeInput,
		float InHandbrakeInput,
		float InSteeringInput,
		float InReflectedInertia,
		float& OutAxleTotalInertia, 
		float& OutAngularVelocity);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleAxleAssembly")
	void GetWheels(UVehicleWheelComponent*& OutLeftWheel, UVehicleWheelComponent*& OutRightWheel)
	{ OutLeftWheel = LeftWheel; OutRightWheel = RightWheel; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleAxleAssembly")
	void GetDifferential(UVehicleDifferentialComponent*& OutDifferential)
	{ OutDifferential = Differential; }
	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	void GetAxleMovement(FVehicleAxleSimData& Out) { Out = SimData; }
	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	void SetP3MotorTorque(float NewTorque) { SimData.P3MotorTorque = NewTorque; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleAxleAssembly")
	float GetP3MotorTorque() { return SimData.P3MotorTorque; }
	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	void SetWheelPosition(float NewTrackWidth);
	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	void UpdateTrackWidth();
	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	void UpdateSolidAxleAnim(
		USceneComponent* InSolidAxleMesh,
		EVehicleSolidAxleAnimPivot AxleMeshAnchorPoint = EVehicleSolidAxleAnimPivot::Center
	);
	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	void ApplySolidAxleStateDirect(
		float InExtensionRatio = 1.f,
		float SteeringAngle = 0.f
	);
	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	void ApplySuspensionStateDirect(
		float InExtensionRatio = 1.f,
		float SteeringAngle = 0.f
	);

	void SetWheelBase(float NewWheelBase) { SimData.WheelBase = NewWheelBase; }
	void GetLinearVelocity(FVector3f& OutLocalVelocity, FVector3f& OutWorldVelocity);
	float GetAngularVelocity() { return SimData.AxleAngularVelocity; }
	float GetTotalAxleInertia() { return SimData.AxleTotalInertia; }
	int32 GetNumOfWheelsOnGround() { return SimData.NumOfWheelOnGround; }
	FVector3f GetAxleCenter();

private:
	bool GenerateWheels();
	bool SearchExistingWheels();
	bool GenerateDifferential();

	UFUNCTION()
	TArray<FName> GetNamesOfWheelsOfOwner();
	UFUNCTION()
	TArray<FName> GetNamesOfDifferentialsOfOwner();
};
