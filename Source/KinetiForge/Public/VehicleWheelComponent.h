// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once


#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "VehicleSuspensionSolver.h"
#include "VehicleWheelSolver.h"
#include "VehicleWheelComponent.generated.h"	//generated.h must be the last

class UVehicleWheelCoordinatorComponent;

/**
* The WheelComponent is a raycast wheel with simple suspension kinematics.
* Please attach this to a primitive component.
* 
* If the wheel is connected to a AxleAssemblyComponent, wheel physics will be updated automatically.
* Otherwise it will not update physics by itself.
* 
* To use the wheel independently (e.g. to build aeroplane), please call the UpdatePhysics function manually.
* Please use the AsyncPhysicsTick event (or any other physics callback) to update physics.
* 
* This WheelComponent will automatically attach itself to the closest primitive component.
* (usually the vehicle chassis / car body)
* 
* A WheelCoordinatorComponent will be automatically generated if needed.
* The WheelCoordinatorComponent will update sprungmass for every wheel automatically.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class KINETIFORGE_API UVehicleWheelComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVehicleWheelComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float ConfigSyncInterval = 1.f;

	// Physics Setup

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	FVehicleWheelConfig WheelConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	FVehicleTireConfig TireConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	FVehicleABSConfig ABSConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	FVehicleSuspensionKinematicsConfig SuspensionKinematicsConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	FVehicleSuspensionSpringConfig SuspensionSpringConfig;

	// Anim Setup

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	bool bUpdateAnimAutomatically = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup", meta = (ClampMin = "0.0"))
	float AnimInterpSpeed = 50.f;

	// Mesh Setup

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	UStaticMesh* WheelMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	FTransform WheelMeshTransform;	//relative to VehicleWheelComponent
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	UStaticMesh* BrakeMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	FTransform BrakeMeshTransform;	//relative to VehicleWheelComponent

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnRegister() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	bool GenerateMeshComponents();
	void ApplyWheelForce(Chaos::FRigidBodyHandle_Internal* CarbodyHandle);

	UPROPERTY()
	TObjectPtr<USceneComponent> WheelKnuckleComponent;
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> WheelMeshComponent;
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> BrakeMeshComponent;
	
	//
	UPROPERTY()
	TWeakObjectPtr<UPrimitiveComponent> Carbody;
	UPROPERTY()
	TWeakObjectPtr<UVehicleWheelCoordinatorComponent> WheelCoordinator;

	//suspension movement
	FVehicleSuspensionSolver Suspension;
	//wheel movement
	FVehicleWheelSolver Wheel;

	FTransform CarbodyAsyncWorldTransform;

	//anim
	FVector2f PrevKnucklePos2D;
	FQuat4f PrevWheelRelativeRot;
	FVector2f AnimKnucklePos2D;
	FQuat4f AnimWheelRelativeRot;
	float AnimWheelRotationAngle = 0.f;
	float TimeSinceLastPhysicsTick = 0.0f;

	//cache
	FVector CachedComponentRelativeLocation;
	float TimeSinceLastConfigSync = 0.f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	static void CopyWheelConfig(const UVehicleWheelComponent* Source, UVehicleWheelComponent* Target, bool bReInitialize = false);

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void InitializeWheel();

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void SetSprungMass(float NewSprungMass);

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	FVector3f GetTopMountRelativeLocation() { return Suspension.State.TopMountRelativePos; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	FTransform3f GetWheelRelativeTransform();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetSuspensionLength() { return Suspension.State.SuspensionCurrentLength; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetSteeringAngle() { return Suspension.State.SteeringAngle; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetSlipRatio() { return Wheel.State.SlipRatio; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetSlipAngle() { return Wheel.State.SlipAngle; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetPredictedSlipRatio() { return Wheel.State.PredictedSlipRatio; }

	/**
	* Obtain a normalized tire combination slip value. This function can be used for tire skid particles or tire sound effects.
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	float GetNormalizedSlip(float LongitudinalScale = 1.f, float LateralScale = 1.f);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetAngularVelocity() { return Wheel.State.AngularVelocity; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetTotalInertia() { return Wheel.State.TotalInertia; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	FVector3f GetWorldLinearVelocity() { return Suspension.State.ImpactPointWorldVelocity; }

	/**
	* Update the independent suspension and wheel physics.
	* If the wheel is generated by a AxleAssemblyComponent,
	* this function will be called by the axle automatically
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void UpdatePhysics(
		float InPhysicsDeltaTime,
		float InDriveTorque,
		float InBrakeTorque, 
		float InHandbrakeTorque,
		float InSteeringAngle, 
		float InSwaybarForce, 
		float InReflectedInertia);

	/**
	* Start updating the solid axle physics, 
	* this function will only prepare suspension simulation and do raycast.
	* If the wheel is generated by a AxleAssemblyComponent,
	* this function will be called by the axle automatically
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void StartUpdateSolidAxlePhysics(
		float InSteeringAngle,
		FVector& OutApporximatedWheelWorldPos,
		FVehicleSuspensionSimContext& Ctx
	);

	/**
	* Finalize updating the solid axle physics, 
	* this function will get final suspension geometry and update tire physics.
	* If the wheel is generated by a AxleAssemblyComponent,
	* this function will be called by the axle automatically
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void FinalizeUpdateSolidAxlePhysics(
		float InPhysicsDeltaTime, 
		float InDriveTorque,
		float InBrakeTorque,
		float InHandbrakeTorque,
		float InSwaybarForce,
		float InReflectedInertia,
		FVehicleSuspensionSimContext& Ctx,
		const FVector& InKnuckleWorldPos,
		const FVector& InAxleWorldDirection);

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void ApplySuspensionStateDirect(float InExtensionRatio = 1.f, float InSteeringAngle = 0.f);

	void StartApplySolidAxleStateDirect(
		float InExtensionRatio,
		float InSteeringAngle,
		FVector& OutApporximatedWheelWorldPos,
		FVehicleSuspensionSimContext& Ctx
	);

	void FinalizeApplySolidAxleStateDirect(
		FVehicleSuspensionSimContext& Ctx,
		const FVector& InKnuckleWorldPos,
		const FVector& InAxleWorldDirection
	);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	void GetWheelCoordinator(UVehicleWheelCoordinatorComponent*& OutWheelCoordinator);

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	float ComputeFeedBackTorque();

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void SetEspBrakeTorque(float NewTorque = 0.f) { Wheel.State.BrakeTorqueFromESP = NewTorque; }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	float GetEspBrakeTorque() { return Wheel.State.BrakeTorqueFromESP; }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void SetP4MotorTorque(float NewTorque = 0.f) { Wheel.State.P4MotorTorque = NewTorque; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetP4MotorTorque() { return Wheel.State.P4MotorTorque; }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void GetWheelMovement(FVehicleWheelSimState& Out) { Out = Wheel.State; }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	FVehicleSuspensionSimState GetSuspensionMovement() { return Suspension.State; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	UPrimitiveComponent* GetCarbody() { return Carbody.Get(); }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	bool GetRayCastResult(FVehicleSuspensionHitResult& Out) { Out = Suspension.RayCastResult; return Suspension.State.bHitGround; }
	bool GetRayCastResult() { return Suspension.State.bHitGround; }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	FVector GetRayCastImpactPoint() { return Suspension.State.ImpactPoint; }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	FTransform GetCarbodyAsyncWorldTransform() { return CarbodyAsyncWorldTransform; }
	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	FTransform GetCarbodyWorldTransform();

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	FVector GetKnuckleRelativePosition() { return FVector(Suspension.State.KnuckleRelativePos); }

	//debug draw
	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void DrawSuspension(
		float Duration = -1, 
		float Thickness = 0,
		bool bDrawSuspension = true, 
		bool bDrawWheel = true,
		bool bDrawRayCast = true);

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void DrawSuspensionForce(
		float Duration = -1,
		float Thickness = 5, 
		float Length = 1);

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void DrawWheelForce(
		float Duration = -1, 
		float Thickness = 5,
		float Length = 1, 
		bool bDrawVelocity = true,
		bool bDrawSlip = true, 
		bool bDrawInertia = true);


	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	bool SetMesh(
		float AxialHubOffset,
		UStaticMesh* NewWheelMesh,
		FTransform WheelMeshRelatvieTransform,
		UStaticMesh* NewBrakeMesh, 
		FTransform BrakeMeshRelativeTransform);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	void GetWheelAndBrakeMeshComponents(UStaticMeshComponent*& OutWheel, UStaticMeshComponent*& OutBrake)
	{
		OutWheel = WheelMeshComponent; OutBrake = BrakeMeshComponent;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	void GetWheelAndBrakeMeshes(UStaticMesh*& OutWheelMesh, UStaticMesh*& OutBrakeMesh)
	{
		OutWheelMesh = WheelMesh; OutBrakeMesh = BrakeMesh;
	}

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	bool RefreshWheelMesh();

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void UpdateWheelAnim(float DeltaTime = 0.f, float MaxAnimAngularVelocity = 60.f);

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void GetWheelKnuckleComponent(USceneComponent*& OutHub) { OutHub = WheelKnuckleComponent; }

	/**
	* Compute and return the world location and world rotation of skid mark.
	* In the returned value, the scale is local linear velocity of the wheel
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	FTransform GetSkidMarkWorldTransform(float InSkidMarkBias, float InSkidMarkScale);

	/**
	* Rotate the arm mesh around its own origin
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	FQuat4f UpdateSuspensionArmAnim(USceneComponent* InArmMesh,
		FRotator InRotationOffset = FRotator(0.f, 0.f, 0.f));

	/**
	* Rotate and scale the spring mesh around its own origin
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	FTransform3f UpdateSuspensionSpringAnim(USceneComponent* InSpringMesh,
		FVector InScaleAxis = FVector(0.f, 0.f, 1.f),
		float InOffsetAlongArm = 0.f,
		FVector InKnuckleOffset = FVector(0.f, 0.f, 0.f),
		FRotator InRotationOffset = FRotator(0.f, 0.f, 0.f),
		float InLengthBias = 0.f,
		FVector InInitialScale = FVector(1.f, 1.f, 1.f));

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void AttachComponentToKnuckle(USceneComponent* InComponent, FTransform InTransform);

};
