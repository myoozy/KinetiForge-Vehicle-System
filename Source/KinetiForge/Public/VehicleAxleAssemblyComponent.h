// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "VehicleAxleStructs.h"
#include "VehicleAxleAssemblyComponent.generated.h"

class UVehicleDifferentialComponent;
class UVehicleWheelCoordinatorComponent;
class UVehicleWheelComponent;

/**
* The AxleAssemblyComponent holds (weak) pointer to (up to) two wheels and one differential.
* Please attach this to a primitive component.
* 
* To make the axle functional, it requires at least one wheel and one differential.
* The wheels and the differential can be automatically generated or selected from existing components.
* 
* If the axle is connected to a DriveAssemblyComponent, axle physics will be updated automatically.
* Otherwise it will not update physics by itself.
* 
* To use the axle independently (e.g. to build aeroplane), please call the UpdatePhysics function manually.
* Please use the AsyncPhysicsTick event (or any other physics callback) to update physics.
* 
* This AxleAssemblyComponent will automatically attach itself to the closest primitive component .
* (usually the vehicle chassis / car body)
* 
* A WheelCoordinatorComponent will be automatically generated if needed.
* The WheelCoordinatorComponent will update wheelbase for every axle automatically.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class KINETIFORGE_API UVehicleAxleAssemblyComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVehicleAxleAssemblyComponent();

	/**
	* Select whether to use existing wheel components. 
	* If not, wheels will be automatically created from the subclass.
	* The dynamically created wheels can also be recorded in the level sequence.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	bool bUseExistingWheelComponent = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (GetOptions = "GetNamesOfWheelsOfOwner", EditCondition = "bUseExistingWheelComponent", EditConditionHides))
	FName LeftWheelComponentName = FName();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (GetOptions = "GetNamesOfWheelsOfOwner", EditCondition = "bUseExistingWheelComponent", EditConditionHides))
	FName RightWheelComponentName = FName();

	/**
	* Wheel components will be automatically generated from this subclass.
	* If the value is empty, it will generate a default wheel.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (EditCondition = "!bUseExistingWheelComponent", EditConditionHides))
	TSubclassOf<UVehicleWheelComponent> WheelConfig;

	/**
	* The rotation of the wheel component.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (EditCondition = "!bUseExistingWheelComponent", EditConditionHides))
	FRotator VehicleWheelComponentSetupRotation;

	/**
	* Select whether to use existing differential component.
	* If not, a differential will be automatically created from the subclass.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	bool bUseExistingDifferentialComponent = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (GetOptions = "GetNamesOfDifferentialsOfOwner", EditCondition = "bUseExistingDifferentialComponent", EditConditionHides))
	FName DifferentialComponentName = FName();

	/**
	* Differential will be generated from this subclass.
	* If the value is empty, it will generate a default differential.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (EditCondition = "!bUseExistingDifferentialComponent", EditConditionHides))
	TSubclassOf<UVehicleDifferentialComponent> DifferentialConfig;

	/**
	* Decides wether the axle has two wheels or only one wheel.
	* This can not be modified in realtime.
	* If you want to destroy a wheel in realtime, just call destroy component to destroy the wheel component.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	EVehicleAxleLayout AxleLayout = EVehicleAxleLayout::TwoWheels;

	/**
	* To simulate solid axle, the axle needs two wheels.
	* If there is only one wheel, it will switch to independent suspension.
	*/
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
	TWeakObjectPtr<UPrimitiveComponent> Carbody;
	UPROPERTY()
	TWeakObjectPtr<UVehicleWheelCoordinatorComponent> WheelCoordinator;
	
	FVehicleAxleSimState State;

	UPROPERTY()
	TWeakObjectPtr<UVehicleWheelComponent> LeftWheel;
	UPROPERTY()
	TWeakObjectPtr<UVehicleWheelComponent> RightWheel;
	UPROPERTY()
	TWeakObjectPtr<UVehicleDifferentialComponent> Differential;

	void UpdateTwoWheelAxle(
		UVehicleWheelComponent* WheelL, 
		UVehicleWheelComponent* WheelR, 
		float InDriveTorque, 
		float InReflectedInertia
	);
	void UpdateSingleWheelAxle(
		UVehicleWheelComponent* WheelL, 
		UVehicleWheelComponent* WheelR, 
		float InDriveTorque, 
		float InReflectedInertia
	);
	void UpdateSteering(float InSteeringInput);
	void UpdateSteeringAssist(float InSteeringInput);
	void UpdateLinearVelocity(
		UVehicleWheelComponent* WheelL, 
		UVehicleWheelComponent* WheelR
	);
	void UpdateSwaybarForce(
		UVehicleWheelComponent* WheelL, 
		UVehicleWheelComponent* WheelR
	);
	void UpdateTCS(
		UVehicleWheelComponent* WheelL, 
		UVehicleWheelComponent* WheelR, 
		float TargetDriveTorque
	);
	void UpdateIndependentSuspensionPhysics(
		UVehicleWheelComponent* WheelL, 
		UVehicleWheelComponent* WheelR
	);
	void UpdateSolidAxlePhysics(
		UVehicleWheelComponent* WheelL, 
		UVehicleWheelComponent* WheelR
	);

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
	{ OutLeftWheel = LeftWheel.Get(); OutRightWheel = RightWheel.Get(); }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleAxleAssembly")
	void GetDifferential(UVehicleDifferentialComponent*& OutDifferential)
	{ OutDifferential = Differential.Get(); }
	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	void GetAxleMovement(FVehicleAxleSimState& Out) { Out = State; }
	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	void SetP3MotorTorque(float NewTorque) { State.P3MotorTorque = NewTorque; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleAxleAssembly")
	float GetP3MotorTorque() { return State.P3MotorTorque; }
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

	void SetWheelBase(float NewWheelBase) { State.WheelBase = NewWheelBase; }
	void GetLinearVelocity(FVector3f& OutLocalVelocity, FVector3f& OutWorldVelocity);
	float GetAngularVelocity() { return State.AxleAngularVelocity; }
	float GetTotalAxleInertia() { return State.AxleTotalInertia; }
	int32 GetNumOfWheelsOnGround() { return State.NumOfWheelOnGround; }
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
