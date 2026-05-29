// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

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

# if WITH_EDITORONLY_DATA
	/**
	* Determines how often the look up tables will be synced.
	* Set to 0 to sync in every frame.
	* Set to <0 to disable sync (for performance).
	* Only works with editor
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float ConfigSyncInterval = -1.f;
#endif

protected:
	// Physics Setup

	/*
	* For wheel
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	FVehicleWheelConfig WheelConfig;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	FVehicleTireConfig TireConfig;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	FVehicleABSConfig ABSConfig;

	/*
	* For Suspension
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	FVehicleSuspensionKinematicsConfig SuspensionKinematicsConfig;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	FVehicleSuspensionSpringConfig SuspensionSpringConfig;

	// Anim Setup

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	bool bUpdateAnimAutomatically = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup", meta = (ClampMin = "0.0"))
	float AnimInterpSpeed = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	float AnimWheelMaxRotationSpeed = 0.f;

	// Mesh Setup

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	UStaticMesh* WheelMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	FTransform WheelMeshTransform;	//relative to VehicleWheelComponent
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	UStaticMesh* BrakeMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	FTransform BrakeMeshTransform;	//relative to VehicleWheelComponent

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnRegister() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	bool InitializeMeshComponents();
	void ApplyWheelForce(Chaos::FRigidBodyHandle_Internal* InChassisHandle);

	UPROPERTY()
	TObjectPtr<USceneComponent> WheelHubComponent;
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> WheelMeshComponent;
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> BrakeMeshComponent;
	
	//
	UPROPERTY()
	TWeakObjectPtr<UPrimitiveComponent> Chassis;
	UPROPERTY()
	TWeakObjectPtr<UVehicleWheelCoordinatorComponent> WheelCoordinator;

	//suspension movement
	FVehicleSuspensionSolver Suspension;
	//wheel movement
	FVehicleWheelSolver Wheel;

	Chaos::FRigidBodyHandle_Internal* ChassisHandle;
	FTransform ChassisAsyncWorldTransform;
	FTransform3f DesignedHubLocalTransform;

	//anim
	FVector3f PrevLowerBallJointChassisLocation;
	FVector3f PrevUpperBallJointChassisLocation;
	FVector3f PrevHubChassisLocation;
	FQuat4f PrevHubChassisRotation;
	FVector3f AnimHubChassisLocation;
	FQuat4f AnimHubChassisRotation;
	float AnimWheelRotationAngle = 0.f;
	float TimeSinceLastPhysicsTick = 0.0f;

	//cache
	FVector3f CachedRelativeLocation;
	FQuat4f CachedRelativeRotation;

#if WITH_EDITORONLY_DATA
	float TimeSinceLastConfigSync = 0.f;
#endif

public:
	void PreStepIndependentSuspension(
		float InMacroDeltaTime,
		float InSteeringAngle,
		float InSwaybarForce);
	void StartPreStepSolidAxleSuspension(
		float InSteeringAngle,
		FVector& OutHitWorldLocation,
		FVehicleSuspensionSimContext& Ctx);
	void FinalizePreStepSolidAxleSuspension(
		float InMacroDeltaTime,
		float InSwaybarForce,
		FVehicleSuspensionSimContext& Ctx,
		const float InTrackWidth,
		const FVector& InThisWheelHitWorldLocation,
		const FVector& InOtherWheelHitWorldLocation);

	void PreStepWheel(
		float InMacroDeltaTime);
	void SubStepWheel(
		float InSubstepDeltaTime,
		float InDriveTorque,
		float InBrakeTorque,
		float InHandbrakeTorque,
		float InReflectedInertia);
	void PostStepApplyForce();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	static void CopyWheelConfig(const UVehicleWheelComponent* Source, UVehicleWheelComponent* Target, bool bReInitialize = false);

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void InitializeWheel();

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void CacheDesignedHubTransform(const float DesignedExtensionRatio = 0.5f);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	const FVehicleWheelConfig& GetWheelConfig() { return WheelConfig; }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void SetWheelConfig(const FVehicleWheelConfig& NewConfig);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	const FVehicleTireConfig& GetTireConfig() { return TireConfig; }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void SetTireConfig(const FVehicleTireConfig& NewConfig);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	const FVehicleABSConfig& GetABSConfig() { return ABSConfig; }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void SetABSConfig(const FVehicleABSConfig& NewConfig);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	const FVehicleSuspensionKinematicsConfig& GetSuspensionKinematicsConfig() { return SuspensionKinematicsConfig; }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void SetSuspensionKinematicsConfig(const FVehicleSuspensionKinematicsConfig& NewConfig);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	const FVehicleSuspensionSpringConfig& GetSuspensionSpringConfig() { return SuspensionSpringConfig; }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void SetSuspensionSpringConfig(const FVehicleSuspensionSpringConfig& NewConfig);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	const FTransform3f& GetDesignedHubLocalTransform() { return DesignedHubLocalTransform; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	UStaticMeshComponent* GetWheelMeshComponent() { return WheelMeshComponent.Get(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	UStaticMesh* GetWheelMesh() { return WheelMeshComponent->GetStaticMesh(); }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void SetWheelMesh(UStaticMesh* NewMesh, const FTransform NewTransform = FTransform(), const bool bUseNewTransform = false);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	UStaticMeshComponent* GetBrakeMeshComponent() { return BrakeMeshComponent.Get(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	UStaticMesh* GetBrakeMesh() { return BrakeMeshComponent->GetStaticMesh(); }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void SetBrakeMesh(UStaticMesh* NewMesh, const FTransform NewTransform = FTransform(), const bool bUseNewTransform = false);

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void SetSprungMass(float NewSprungMass);

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	FVector3f GetTopMountChassisLocation();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	void GetLowerWishboneState(
		FVector& OutPivotChassisLocation,
		FVector& OutAxisChassisDirection,
		FVector& OutBallJointChassisLocation);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	void GetUpperWishboneState(
		FVector& OutPivotChassisLocation,
		FVector& OutAxisChassisDirection,
		FVector& OutBallJointChassisLocation);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	FTransform3f GetHubChassisTransform();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetSuspensionLength() { return Suspension.State.SuspensionCurrentLength; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetSteeringAngle() { return Suspension.State.SteeringAngle; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetWheelRadius() { return WheelConfig.Radius; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetWheelWidth() { return WheelConfig.Width; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetIntrinsicInertia() { return WheelConfig.Inertia; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetWheelInertia() { return GetIntrinsicInertia(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetSlipRatio(bool bTransientSlip = true);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetSlipAngle(bool bTransientSlip = true);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetPredictedSlipRatio() { return Wheel.State.PredictedSlipRatio; }

	/**
	* Obtain a normalized tire combination slip value. 
	* This function can be used for tire skid particles or tire sound effects.
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	float GetNormalizedSlip(float LongitudinalScale = 1.f, float LateralScale = 1.f);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetAngularVelocity() { return Wheel.State.AngularVelocity; }

	/*
	* Including all the inertia from the drivetrain which is reflected on the wheel
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetEffectiveInertia() { return Wheel.State.EffectiveInertia; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetTotalInertia() { return GetEffectiveInertia(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	FVector3f GetWorldLinearVelocity() { return Suspension.State.ImpactWorldVelocity; }

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
	* This function checks if the wheel has been moved.
	* If the wheel is moved, the sprung mass and the critical damping need to be updated.
	* 
	* If the wheel found its wheel coordinator:
	* This function will be called by the wheel coordinator automatically.
	* And the sprung mass and critical damping will automatically be updated.
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	bool CheckHasBeenMoved();

	/**
	* Start updating the solid axle physics, 
	* this function will only prepare suspension simulation and do raycast.
	* If the wheel is generated by a AxleAssemblyComponent,
	* this function will be called by the axle automatically
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void StartUpdateSolidAxlePhysics(
		float InSteeringAngle,
		FVector& OutHitWorldLocation,
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
		const float InTrackWidth,
		const FVector& InThisWheelHitWorldLocation,
		const FVector& InOtherWheelHitWorldLocation);

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void ApplySuspensionStateDirect(float InExtensionRatio = 1.f, float InSteeringAngle = 0.f);

	void StartApplySolidAxleStateDirect(
		float InExtensionRatio,
		float InSteeringAngle,
		FVector& OutHitWorldLocation,
		FVehicleSuspensionSimContext& Ctx
	);

	void FinalizeApplySolidAxleStateDirect(
		FVehicleSuspensionSimContext& Ctx,
		const float InTrackWidth,
		const FVector& InThisWheelHitWorldLocation,
		const FVector& InOtherWheelHitWorldLocation
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

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel", meta = (DeprecatedFunction, DeprecationMessage = "Please use GetWheelState instead!"))
	void GetWheelMovement(FVehicleWheelSimState& Out) { Out = Wheel.State; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	const FVehicleWheelSimState& GetWheelState() { return Wheel.State; }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel", meta = (DeprecatedFunction, DeprecationMessage = "Please use GetSuspensionState instead!"))
	const FVehicleSuspensionSimState& GetSuspensionMovement() { return Suspension.State; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	const FVehicleSuspensionSimState& GetSuspensionState() { return Suspension.State; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	const FVehicleSuspensionHitResult& GetSuspensionHitResult() { return Suspension.RayCastResult; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	UPrimitiveComponent* GetChassis() { return Chassis.Get(); }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	bool GetRayCastResult(FVehicleSuspensionHitResult& Out) { Out = Suspension.RayCastResult; return Suspension.State.bHitGround; }
	bool GetRayCastResult() { return Suspension.State.bHitGround; }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	UPrimitiveComponent* GetRayCastHitComponent() { return Suspension.RayCastResult.Component.Get(); }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	FVector GetRayCastImpactPoint() { return Suspension.State.ImpactWorldLocation; }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	FTransform GetChassisAsyncWorldTransform() { return ChassisAsyncWorldTransform; }
	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	FTransform GetChassisWorldTransform();

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	FVector GetHubChassisLocation() { return FVector(Suspension.State.HubChassisLocation); }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	FVector GetAnimHubChassisLocation() { return FVector(AnimHubChassisLocation); }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel", meta = (DeprecatedFunction, DeprecationMessage = "Please use GetHubChassisTransform instead!"))
	FTransform GetWheelRelativeTransform() { return FTransform(GetHubChassisTransform()); }

	//debug draw
	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void DrawSuspension(
		float Duration = -1, 
		float Thickness = 0,
		bool bDrawSuspension = true, 
		bool bDrawWheel = true,
		bool bDrawRayCast = true,
		UVehicleAxleAssemblyComponent* ParentAxle = nullptr);

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
	USceneComponent* GetWheelHubComponent() { return WheelHubComponent; }

	/**
	* Compute and return the world location and world rotation of skid mark.
	* In the returned value, the scale is local linear velocity of the wheel
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	FTransform GetSkidMarkWorldTransform(float InSkidMarkBias, float InSkidMarkScale);

	/**
	* Rotate the arm around the pivot of an wishbone.
	* Returns the transform of the mesh relative to the chassis.
	* @param bFollowUpperWishbone: If true, follow upper wishbone. If false, follow lower wishbone.
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	FTransform UpdateWishboneAnim(
		USceneComponent* InArmMesh,
		const bool bFollowUpperWishbone = false,
		const FTransform InOffset = FTransform(),
		const FVector InMeshForwardVector = FVector::ForwardVector,
		const FVector InMeshRightVector = FVector::RightVector, 
		const bool bScaleToMatchLength = false,
		const float MeshDesignLength = 100.f
	);

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel", meta = (DeprecatedFunction, DeprecationMessage = "Please use UpdateWishboneAnim instead!"))
	FTransform UpdateSuspensionArmAnim(
		USceneComponent* InArmMesh,
		const bool bFollowUpperWishbone = false,
		const FTransform InOffset = FTransform(),
		const FVector InMeshForwardVector = FVector::ForwardVector,
		const FVector InMeshRightVector = FVector::RightVector,
		const bool bScaleToMatchLength = false,
		const float MeshDesignLength = 100.f
	);

	/**
	* Rotate and scale the spring mesh around the top mmount.
	* Returns the Transform of the strut relative to chassis.
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void UpdateShockAbsorberAnim(
		USceneComponent* InUpperStrutMesh,
		USceneComponent* InLowerStrutMesh,
		USceneComponent* InSpringMesh,
		const float SpringDesignLength = 100.f,
		const FVector InMeshUpVector = FVector(0.f, 0.f, 1.f)
	);

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel", meta = (DeprecatedFunction, DeprecationMessage = "Please use UpdateShockAbsorberAnim instead!"))
	void UpdateSuspensionSpringAnim(
		USceneComponent* InUpperStrutMesh,
		USceneComponent* InLowerStrutMesh,
		USceneComponent* InSpringMesh,
		const float SpringDesignLength = 100.f,
		const FVector InMeshUpVector = FVector(0.f, 0.f, 1.f)
	);
	/**
	* Orients a component towards a target location and scales it along the tracking axis to bridge the distance.
	*/
	UFUNCTION(BlueprintCallable, Category = "Math|ProceduralTransform")
	static void OrientAndScaleToLocation(
		USceneComponent* InComponent,
		const FVector& TargetWorldLocation,
		const float UnscaledLength = 100.f,
		const FVector TrackingAxisLocal = FVector::UpVector
	);

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void AttachComponentToWheelHub(
		USceneComponent* InComponent, 
		bool bKeepWorldTransform = false
	);

};
