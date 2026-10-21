// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once


#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "VehicleSuspensionSolver.h"
#include "VehicleWheelSolver.h"
#include "VehicleWheelPhysics.h"
#include "VehiclePhysicsStep.h"
#include "VehicleWheelComponent.generated.h"	//generated.h must be the last

class UVehicleWheelCoordinatorComponent;
struct FVehicleWheelRuntimeSettings;
struct FVehicleAxleWheelInput;
class FVehicleWheelBridge;

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
	friend class UVehicleDriveAssemblyComponent;
	friend class UVehicleAxleAssemblyComponent;
	friend struct FVehicleDriveOwnershipProbe;
	friend struct FVehicleWheelContactProbe;
	friend struct FVehicleAxleRuntimeProbe;
	friend struct FVehicleWheelBindingProbe;
	friend struct FVehicleAxleBindingProbe;
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVehicleWheelComponent();

	/** Serialized compatibility only. All wheels now use the same C++ runtime. */
	UPROPERTY(BlueprintReadWrite, Category = "Setup", meta = (DeprecatedProperty, DeprecationMessage = "All wheels use the C++ runtime; this setting is ignored."))
	bool bUseIsolatedWheelRuntime = false;

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
	virtual void OnUnregister() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginDestroy() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	bool InitializeMeshComponents();
	void ApplyWheelForce(Chaos::FRigidBodyHandle_Internal* const ChassisHandle);

	UPROPERTY()
	TWeakObjectPtr<USceneComponent> WheelHubComponent;
	UPROPERTY()
	TWeakObjectPtr<UStaticMeshComponent> WheelMeshComponent;
	UPROPERTY()
	TWeakObjectPtr<UStaticMeshComponent> BrakeMeshComponent;
	
	//
	UPROPERTY()
	TWeakObjectPtr<UPrimitiveComponent> Chassis;
	UPROPERTY()
	TWeakObjectPtr<UVehicleWheelCoordinatorComponent> WheelCoordinator;

	// GT resource/preview preparation. Live numerical state belongs to WheelBridge's runtime.
	FVehicleSuspensionCachedLUTs CachedSuspensionLUTs;
	FVehicleWheelCachedLUTs CachedTireLUTs;
	FCollisionQueryParams CollisionQueryParams;
	FCollisionResponseParams CollisionResponseParams;

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
	float WorldGravityZ = 9.81;
	FVector3f CachedRelativeLocation;
	FQuat4f CachedRelativeRotation;

#if WITH_EDITORONLY_DATA
	float TimeSinceLastConfigSync = 0.f;
#endif

private:
	// Admission only, not a lifetime lock. Worker adapters must externally exclude component teardown.
	TAtomic<bool> bManualPhysicsReady{false};
	bool CanUpdateManualPhysics(float DeltaTime) const;
	// GT presentation copies. The simulation owner writes only the bridge's runtime.
	FVehicleWheelSimState PublishedWheelState;
	FVehicleSuspensionSimState PublishedSuspensionState;
	FVehicleSuspensionHitResult PublishedContactResult;
	float PublishedPhysicsDeltaTimeInv = 0.f;
	// GT reads presentation. Non-GT access requires the sole simulation caller, as with the manual API.
	const FVehicleWheelSimState& GetWheelStateForCaller() const;
	const FVehicleSuspensionSimState& GetSuspensionStateForCaller() const;
	const FVehicleSuspensionHitResult& GetContactResultForCaller() const;
	float GetPhysicsDeltaTimeInvForCaller() const;
	FVehiclePhysicsOwnership PhysicsOwnership;
	UPROPERTY()
	TArray<TWeakObjectPtr<UActorComponent>> AutoGeneratedComponents;
	// Assigned during construction and never replaced; bridge lifetime is shared with in-flight users.
	TSharedPtr<FVehicleWheelBridge, ESPMode::ThreadSafe> WheelBridge;
	uint64 LastWheelPublicationSequence = 0;
	uint64 RequestedSuspensionResetSequence = 0;
	uint64 RequestedP4Sequence = 0, RequestedEspSequence = 0, RequestedMassSequence = 0;
	float RequestedP4MotorTorque = 0.f;
	float RequestedEspBrakeTorque = 0.f;
	float RequestedSprungMass = 0.f;
	void SubmitWheelRuntimeSettings();
	void SubmitWheelEnvironment();
	void SubmitSuspensionReset();

public:
	/** Game-thread snapshot of current numeric config and cached tables; no curve object references. */
	FVehicleWheelRuntimeSettings CaptureRuntimeSettings() const;
	/** GT compatibility validation after registration. Runtime already exists; never changes backend. */
	bool BindIsolatedWheelRuntime();
	TSharedPtr<FVehicleWheelBridge, ESPMode::ThreadSafe> GetWheelBridge() const;
	void RefreshPublishedWheelState();
private:
	/** Manual axle implementation only; these read the live simulation, not the BP display snapshot. */
	FVehicleAxleWheelInput CaptureSimulationTransmissionInput();
	void PrepareSimulationStepInputs();
	const FVehicleSuspensionSimState& GetSimulationSuspensionState();
	FTransform GetSimulationChassisTransform();
	FTransform GetSimulationRelativeTransform();
	FTransform3f GetSimulationDesignedHubTransform();
	const FVehicleSuspensionKinematicsConfig& GetSimulationKinematicsConfig();
	FVector3f GetSimulationTopMountChassisLocation();
public:
	void PreStepIndependentSuspension(
		const float InMacroDeltaTime,
		const float InSteeringAngle,
		const float ActiveSwaybarStiffness,
		const float OtherHubChassisZ);
	/** Query and velocity preparation for the axle's paired independent-suspension phase. */
	bool StartPreStepIndependentSuspension(const float InMacroDeltaTime, const float InSteeringAngle,
		FVehicleWheelIndependentSuspensionStep& OutStep);
	FVehicleSuspensionIntegrationInput GetPreparedIndependentSuspensionInput(
		FVehicleWheelIndependentSuspensionStep& Step);
	/** Paired callers have already integrated both contexts and prepared both final geometries. */
	void FinalizePreStepIndependentSuspension(float ActiveSwaybarStiffness,
		float OtherHubChassisZ, FVehicleWheelIndependentSuspensionStep& Step,
		bool bAlreadyPairedAndGeometryPrepared);
	void StartPreStepSolidAxleSuspension(
		FVehicleSuspensionSimContext& Ctx,
		const float InMacroDeltaTime,
		const float InSteeringAngle,
		const float ActiveSwaybarStiffness,
		const float OtherHubChassisZ);
	void FinalizePreStepSolidAxleSuspension(
		FVehicleSuspensionSimContext& Ctx,
		const float InMacroDeltaTime,
		const float ActiveSwaybarStiffness,
		const float OtherHubChassisZ,
		const float AxleHalfWidth,
		const FVector3f& AxleChassisCenter,
		const FQuat4f& AxleChassisRotation);

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
	float GetSprungMass() { return GetSuspensionStateForCaller().StaticSprungMass; }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	float GetUnsprungMass() { return GetSuspensionStateForCaller().VirtualUnsprungMass; }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	FVector3f GetTopMountChassisLocation();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	FVector3f GetLowerBallJointChassisLocation() { return GetSuspensionStateForCaller().LowerBallJointChassisLocation; }

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
	float GetSuspensionLength() { return GetSuspensionStateForCaller().StrutCurrentLength; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetSteeringAngle() { return GetSuspensionStateForCaller().SteeringAngle; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetWheelRadius() { return WheelConfig.Radius; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetWheelWidth() { return WheelConfig.Width; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetIntrinsicInertia() { return WheelConfig.Inertia; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel", meta = (DeprecatedFunction, DeprecationMessage = "Use GetIntrinsicInertia instead."))
	float GetWheelInertia() { return GetIntrinsicInertia(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetSlipRatio(bool bTransientSlip = true);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetSlipAngle(bool bTransientSlip = true);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetPredictedSlipRatio() { return GetWheelStateForCaller().PredictedSlipRatio; }

	/**
	* Get the actual velocity difference between ground and wheel
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	FVector2f GetSlipVelocity() { return FVector2f(GetWheelStateForCaller().LongSlipVelocity, GetWheelStateForCaller().LocalLinearVelocity.Y); }

	/**
	* Get the intensity of tire skid.
	* The Power (power = slipVelocity * tireForce) is calculated, and then scaled by MaxSkidPowerThreshold.
	* The output will be Clamp(Power/MaxSkidPowerThreshold, 0.0, 1.0).
	* This function is more robust than GetNormalizedSlip.
	* 
	* Try GetNormalizedSlip if you don't like this function.
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	float GetSkidIntensity(float LongitudinalScale = 1.f, float LateralScale = 1.f, float MaxSkidPowerThreshold = 50000.f);

	/**
	* Obtain a normalized tire combination slip value. Only works when there's proper tire setup.
	* This function can be used for tire skid particles or tire sound effects.
	* 
	* Try GetSkidIntensity if you don't like this function.
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	float GetNormalizedSlip(float LongitudinalScale = 1.f, float LateralScale = 1.f);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetAngularVelocity() { return GetWheelStateForCaller().AngularVelocity; }

	/*
	* Including all the inertia from the drivetrain which is reflected on the wheel
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetEffectiveInertia() { return GetWheelStateForCaller().EffectiveInertia; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel", meta = (DeprecatedFunction, DeprecationMessage = "Use GetEffectiveInertia instead."))
	float GetTotalInertia() { return GetEffectiveInertia(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	FVector2f GetLocalLinearVelocity2D() { return GetWheelStateForCaller().LocalLinearVelocity; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	FVector3f GetWorldLinearVelocity() { return GetSuspensionStateForCaller().ImpactWorldVelocity; }

	/**
	* Update the independent suspension and wheel physics.
	* If the wheel is generated by a AxleAssemblyComponent,
	* this function will be called by the axle automatically
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void UpdatePhysics(
		const float InPhysicsDeltaTime,
		const float InDriveTorque,
		const float InBrakeTorque, 
		const float InHandbrakeTorque,
		const float InSteeringAngle,
		const float ActiveSwaybarStiffness,
		const float OtherHubChassisZ,
		const float InReflectedInertia);

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
		const float InPhysicsDeltaTime,
		const float InSteeringAngle,
		const float ActiveSwaybarStiffness,
		const float OtherHubChassisZ,
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
		const float InPhysicsDeltaTime,
		const float InDriveTorque,
		const float InBrakeTorque,
		const float InHandbrakeTorque,
		const float ActiveSwaybarStiffness,
		const float OtherHubChassisZ,
		const float InReflectedInertia,
		const float AxleHalfWidth,
		const FVector3f& AxleChassisCenter,
		const FQuat4f& AxleChassisRotation,
		FVehicleSuspensionSimContext& Ctx);

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void ApplySuspensionStateDirect(float InExtensionRatio = 1.f, float InSteeringAngle = 0.f);

	void RoughlyInitializeSuspensionState(
		FVehicleSuspensionSimState& InState
	);

	void StartApplySolidAxleStateDirect(
		float InExtensionRatio,
		float InSteeringAngle,
		const FVehicleSuspensionSimState* PrevState,
		FVehicleSuspensionSimContext& Ctx
	);

	void FinalizeApplySolidAxleStateDirect(
		FVehicleSuspensionSimContext& Ctx,
		const float AxleHalfWidth,
		const FVector3f& AxleChassisCenter,
		const FQuat4f& AxleChassisRotation
	);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	void GetWheelCoordinator(UVehicleWheelCoordinatorComponent*& OutWheelCoordinator);

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	float ComputeFeedBackTorque();

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void SetEspBrakeTorque(float NewTorque = 0.f);

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	float GetEspBrakeTorque() { return GetWheelStateForCaller().BrakeTorqueFromESP; }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void SetP4MotorTorque(float NewTorque = 0.f);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	float GetP4MotorTorque() { return GetWheelStateForCaller().P4MotorTorque; }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel", meta = (DeprecatedFunction, DeprecationMessage = "Please use GetWheelState instead!"))
	void GetWheelMovement(FVehicleWheelSimState& Out) { Out = GetWheelStateForCaller(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	const FVehicleWheelSimState& GetWheelState() { return GetWheelStateForCaller(); }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel", meta = (DeprecatedFunction, DeprecationMessage = "Please use GetSuspensionState instead!"))
	const FVehicleSuspensionSimState& GetSuspensionMovement() { return GetSuspensionStateForCaller(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	const FVehicleSuspensionSimState& GetSuspensionState() { return GetSuspensionStateForCaller(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	const FVehicleSuspensionHitResult& GetSuspensionHitResult() { return GetContactResultForCaller(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "VehicleWheel")
	UPrimitiveComponent* GetChassis() { return Chassis.Get(); }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	bool GetRayCastResult(FVehicleSuspensionHitResult& Out) { Out = GetContactResultForCaller(); return GetSuspensionStateForCaller().bHitGround; }
	bool GetRayCastResult() { return GetSuspensionStateForCaller().bHitGround; }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	bool GetIsWheelOnGround() { return GetSuspensionStateForCaller().bWheelOnGround; }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	UPrimitiveComponent* GetRayCastHitComponent() { return GetContactResultForCaller().Component.Get(); }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	FVector GetRayCastImpactPoint() { return GetSuspensionStateForCaller().ImpactWorldLocation; }

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	FTransform GetChassisAsyncWorldTransform() { return ChassisAsyncWorldTransform; }
	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	FTransform GetChassisWorldTransform();

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	FVector GetHubChassisLocation() { return FVector(GetSuspensionStateForCaller().HubChassisLocation); }

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
		OutWheel = WheelMeshComponent.Get(); OutBrake = BrakeMeshComponent.Get();
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
	USceneComponent* GetWheelHubComponent() { return WheelHubComponent.Get(); }

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

	UFUNCTION(BlueprintCallable, Category = "VehicleWheel")
	void UpdateCachedLookUpTables();
};
