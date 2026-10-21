// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "VehicleAxleStructs.h"
#include "VehiclePhysicsStep.h"
#include "VehicleAxleAssemblyComponent.generated.h"

class UVehicleDifferentialComponent;
class UVehicleWheelCoordinatorComponent;
class UVehicleWheelComponent;
class FVehicleAxleBridge;
struct FVehicleAxleRuntimeSettings;
struct FVehicleAxleWheelInput;

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
	friend class UVehicleDriveAssemblyComponent;
	friend class UVehicleDifferentialComponent;
	friend struct FVehicleDriveOwnershipProbe;
	friend struct FVehicleAxleRuntimeProbe;
	friend struct FVehicleWheelBindingProbe;
	friend struct FVehicleAxleBindingProbe;
	friend struct FVehicleWheelSlotProbe;
	GENERATED_BODY()

public:	
	/** GT compatibility validation of the registered wheel set. All axles already own their C++ runtime. */
	bool BindIsolatedAxleRuntime();
	TSharedPtr<FVehicleAxleBridge, ESPMode::ThreadSafe> GetAxleBridge() const;
	FVehicleAxleRuntimeSettings CaptureRuntimeSettings() const;
	void RefreshPublishedAxleState();
	/** Sole simulation consumer only. GT/debug/animation use GetAxleState and published getters. */
	FVehicleAxleSimState& GetSimulationState();
	const FVehicleAxleConfig& GetSimulationAxleConfig();
	// Sets default values for this component's properties
	UVehicleAxleAssemblyComponent();

protected:
	/**
	* Select whether to use existing wheel components. 
	* If not, wheels will be automatically created from the subclass.
	* The dynamically created wheels can also be recorded in the level sequence.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	bool bUseExistingWheelComponent = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup", meta = (GetOptions = "GetNamesOfWheelsOfOwner", EditCondition = "bUseExistingWheelComponent", EditConditionHides))
	FName LeftWheelComponentName = FName();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup", meta = (GetOptions = "GetNamesOfWheelsOfOwner", EditCondition = "bUseExistingWheelComponent", EditConditionHides))
	FName RightWheelComponentName = FName();

	/**
	* Wheel components will be automatically generated from this subclass.
	* If the value is empty, it will generate a default wheel.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup", meta = (EditCondition = "!bUseExistingWheelComponent", EditConditionHides))
	TSubclassOf<UVehicleWheelComponent> WheelClass;

	/**
	* The rotation of the wheel component.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup", meta = (EditCondition = "!bUseExistingWheelComponent", EditConditionHides))
	FRotator VehicleWheelComponentSetupRotation;

	/**
	* Select whether to use existing differential component.
	* If not, a differential will be automatically created from the subclass.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	bool bUseExistingDifferentialComponent = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup", meta = (GetOptions = "GetNamesOfDifferentialsOfOwner", EditCondition = "bUseExistingDifferentialComponent", EditConditionHides))
	FName DifferentialComponentName = FName();

	/**
	* Differential will be generated from this subclass.
	* If the value is empty, it will generate a default differential.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup", meta = (EditCondition = "!bUseExistingDifferentialComponent", EditConditionHides))
	TSubclassOf<UVehicleDifferentialComponent> DifferentialClass;

	/**
	* Decides wether the axle has two wheels or only one wheel.
	* This can not be modified in realtime.
	* If you want to destroy a wheel in realtime, just call destroy component to destroy the wheel component.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	EVehicleAxleLayout AxleLayout = EVehicleAxleLayout::TwoWheels;

	/**
	* To simulate solid axle, the axle needs two wheels.
	* If there is only one wheel, it will switch to independent suspension.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	EVehicleAxleSuspensionType SuspensionType = EVehicleAxleSuspensionType::Independent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	FVehicleAxleConfig AxleConfig;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	FVehicleAxleSteeringConfig AxleSteeringConfig;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	FVehicleSteeringAssistConfig SteeringAssistConfig;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	FVehicleTCSConfig TCSConfig;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginDestroy() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	UPROPERTY()
	TWeakObjectPtr<UPrimitiveComponent> Chassis;
	UPROPERTY()
	TWeakObjectPtr<UVehicleWheelCoordinatorComponent> WheelCoordinator;
	
	FVehicleAxleSimState State;

	UPROPERTY()
	TWeakObjectPtr<UVehicleWheelComponent> LeftWheel;
	UPROPERTY()
	TWeakObjectPtr<UVehicleWheelComponent> RightWheel;
	UPROPERTY()
	TWeakObjectPtr<UVehicleDifferentialComponent> Differential;

	void UpdateSteering(
		float InSteeringInput,
		UVehicleWheelComponent* WheelL,
		UVehicleWheelComponent* WheelR
	);
	void UpdateSteeringAssist(
		float DeltaTime, 
		float InSteeringInput);
	void UpdateLinearVelocity(
		UVehicleWheelComponent* WheelL, 
		UVehicleWheelComponent* WheelR
	);
	void UpdateTCS(
		UVehicleWheelComponent* WheelL, 
		UVehicleWheelComponent* WheelR, 
		float TargetDriveTorque
	);
	float GetTrackWidth(
		UVehicleWheelComponent* WheelL,
		UVehicleWheelComponent* WheelR
	);
	void PreStepSolidAxleSuspension(
		const float InMacroDeltaTime,
		const float SteerAngleLeft,
		const float SteerAngleRight,
		const float ActiveSwaybarStiffness,
		UVehicleWheelComponent* WheelL,
		UVehicleWheelComponent* WheelR
	);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	static void CopyAxleConfig(const UVehicleAxleAssemblyComponent* Source, UVehicleAxleAssemblyComponent* Target, bool bReInitializeWheel = false);

	void ApplyInitialOverrides(const FVehicleAxleSpawnTemplate& AxleTemplate);

	void PreStepAxle(
		float InMacroDeltaTime,
		float InSteeringInput
	);
	void SubstepAxle(
		float InSubstepDeltaTime,
		float InDriveTorque,
		float InBrakeInput,
		float InHandbrakeInput,
		float& OutAxleEffectiveInertia,
		float& OutAngularVelocity,
		float& OutDriveShaftStiffness
	);
	void PostStepAxle();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleAxleAssembly")
	EVehicleAxleSuspensionType GetAxleSuspensionType() { return SuspensionType; }

	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	void SetAxleSuspensionType(EVehicleAxleSuspensionType NewType);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleAxleAssembly")
	const FVehicleAxleConfig& GetAxleConfig() { return AxleConfig; }

	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	void SetAxleConfig(const FVehicleAxleConfig& NewConfig);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleAxleAssembly")
	const FVehicleAxleSteeringConfig& GetAxleSteeringConfig() { return AxleSteeringConfig; }

	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	void SetAxleSteeringConfig(const FVehicleAxleSteeringConfig& NewConfig);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleAxleAssembly")
	const FVehicleSteeringAssistConfig& GetSteeringAssistConfig() { return SteeringAssistConfig; }

	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	void SetSteeringAssistConfig(const FVehicleSteeringAssistConfig& NewConfig);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleAxleAssembly")
	const FVehicleTCSConfig& GetTCSConfig() { return TCSConfig; }

	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	void SetTCSConfig(const FVehicleTCSConfig& NewConfig);

	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	void InitializeWheels();

	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	void UpdatePhysics(
		float InPhysicsDeltaTime,
		float InDriveTorque,
		float InBrakeInput,
		float InHandbrakeInput,
		float InSteeringInput,
		float& OutAxleEffectiveInertia,
		float& OutAngularVelocity,
		float& OutDriveShaftStiffness);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleAxleAssembly")
	void GetWheels(UVehicleWheelComponent*& OutLeftWheel, UVehicleWheelComponent*& OutRightWheel)
	{ OutLeftWheel = LeftWheel.Get(); OutRightWheel = RightWheel.Get(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleAxleAssembly")
	void GetDifferential(UVehicleDifferentialComponent*& OutDifferential)
	{ OutDifferential = Differential.Get(); }

	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly", meta = (DeprecatedFunction, DeprecationMessage = "Please use GetAxleState instead!"))
	void GetAxleMovement(FVehicleAxleSimState& Out) { Out = GetStateForCaller(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleAxleAssembly")
	const FVehicleAxleSimState& GetAxleState() { return GetStateForCaller(); }

	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	void SetP3MotorTorque(float NewTorque);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleAxleAssembly")
	float GetP3MotorTorque() { return GetStateForCaller().P3MotorTorque; }

	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	void SetWheelPosition(float NewTrackWidth);

	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	void UpdateTrackWidth();

	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	float GetTrackWidth();

	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	void UpdateSolidAxleAnim(
		USceneComponent* InSolidAxleMesh,
		EVehicleSolidAxleAnimPivot AxleMeshAnchorPoint,
		const FVector InMeshRightVector = FVector(0.f, 1.f, 0.f),
		const FVector InMeshForwardVector = FVector(1.f, 0.f, 0.f) 
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

	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	float GetWheelBase() { return GetStateForCaller().WheelBase; }

	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	void SetWheelBase(float NewWheelBase);

	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	void GetLinearVelocity(FVector3f& OutLocalVelocity, FVector3f& OutWorldVelocity);

	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	float GetAngularVelocity() { return GetStateForCaller().AxleAngularVelocity; }

	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	float GetTotalAxleInertia() { return GetStateForCaller().AxleEffectiveInertia; }

	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	int32 GetNumOfWheelsOnGround() { return GetStateForCaller().NumOfWheelOnGround; }

	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	FVector3f GetAxleCenter();

	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	float GetSteeringValue() { return GetStateForCaller().RealSteeringValue; }

	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	void GetSteeringAngle(float& OutLeft, float& OutRight) 
	{
		OutLeft = GetStateForCaller().LeftWheelSteeringAngle;
		OutRight = GetStateForCaller().RightWheelSteeringAngle;
	}

	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	UVehicleWheelCoordinatorComponent* GetWheelCoordinator() { return WheelCoordinator.Get(); }

	UFUNCTION(BlueprintCallable, Category = "VehicleAxleAssembly")
	UPrimitiveComponent* GetChassis() { return Chassis.Get(); }

private:
	/**
	* Wheel slot model. There are exactly two slots (left/right). AxleLayout decides which
	* slots are required; the slot's wheel is either generated by this axle (destroyable,
	* positioned and configured by the axle) or referenced from the level (never destroyed,
	* never repositioned or reconfigured by InitializeWheels). All slot mutation goes through
	* ReconcileWheelSlots, so registration order and repeated registration are irrelevant.
	*/
	enum class EWheelSide : uint8 { Left, Right };

	bool IsWheelRequired(EWheelSide Side) const;
	FName GetGeneratedWheelName(EWheelSide Side) const;
	TWeakObjectPtr<UVehicleWheelComponent>& WheelSlotRef(EWheelSide Side);
	bool IsWheelGenerated(EWheelSide Side) const;
	void SetWheelGenerated(EWheelSide Side, bool bGenerated);
	bool HasBothWheels() const;
	void EnsureAxleAttachedToChassis();
	void PlaceWheel(EWheelSide Side, float TrackWidth);
	void PlaceManagedWheels();
	void DestroyGeneratedWheel(EWheelSide Side);
	void ResolveWheelSlot(EWheelSide Side);
	void ReconcileWheelSlots();

	/** True only for wheels this axle created; referenced wheels are never destroyed. */
	bool bLeftWheelGenerated = false;
	bool bRightWheelGenerated = false;

	TAtomic<bool> bManualPhysicsReady{false};
	const FVehicleAxleSimState& GetStateForCaller() const;
	static FVehicleAxleWheelInput CaptureWheelTransmissionInput(UVehicleWheelComponent* Wheel);
	static float CaptureSimulationTrackWidth(float FallbackWidth, UVehicleWheelComponent* Left, UVehicleWheelComponent* Right);
	FVehiclePhysicsOwnership PhysicsOwnership;
	// Assigned during construction and never replaced; bridge lifetime is shared with in-flight users.
	TSharedPtr<FVehicleAxleBridge, ESPMode::ThreadSafe> AxleBridge;
	uint64 LastAxlePublicationSequence = 0;
	uint64 RequestedP3Sequence = 0, RequestedWheelBaseSequence = 0;
	float RequestedP3MotorTorque = 0.f, RequestedWheelBase = 0.f;
	void SubmitRuntimeSettings();
	bool GenerateDifferential();

	UPROPERTY()
	TArray<TWeakObjectPtr<UActorComponent>> AutoGeneratedComponents;

	UFUNCTION()
	TArray<FName> GetNamesOfWheelsOfOwner();
	UFUNCTION()
	TArray<FName> GetNamesOfDifferentialsOfOwner();
};
