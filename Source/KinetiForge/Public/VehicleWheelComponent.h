// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once


#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "VehicleSuspensionSolver.h"
#include "VehicleWheelSolver.h"
#include "VehicleWheelComponent.generated.h"	//generated.h must be the last

class UVehicleWheelCoordinatorComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class KINETIFORGE_API UVehicleWheelComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVehicleWheelComponent();

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
	void ApplyWheelForce();

	UPROPERTY()
	TObjectPtr<USceneComponent> WheelKnuckleComponent;
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> WheelMeshComponent;
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> BrakeMeshComponent;
	
	//
	UPROPERTY()
	TObjectPtr<UWorld> CurrentWorld;
	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> Carbody;
	UPROPERTY()
	TObjectPtr<UVehicleWheelCoordinatorComponent> WheelCoordinator;

	//suspension movement
	FVehicleSuspensionSolver Suspension;
	//wheel movement
	FVehicleWheelSolver Wheel;

	//anim
	FVector2D PrevKnucklePos2D;
	FQuat PrevWheelRelativeRot;
	FVector2D AnimKnucklePos2D;
	FQuat AnimWheelRelativeRot;
	float TimeSinceLastPhysicsTick = 0.0f;

	//cache
	FVector CachedComponentRelativeLocation;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	static Chaos::FRigidBodyHandle_Internal* GetInternalHandle(
		UPrimitiveComponent* Component,
		FName BoneName = NAME_None);

	UFUNCTION(BlueprintCallable, Category = "Initialization")
	static void CopyWheelConfig(const UVehicleWheelComponent* Source, UVehicleWheelComponent* Target, bool bReInitialize = false);

	UFUNCTION(BlueprintCallable, Category = "Initialization")
	void InitializeWheel();

	UFUNCTION(BlueprintCallable, Category = "Suspension")
	void SetSprungMass(float NewSprungMass);

	UFUNCTION(BlueprintCallable, Category = "Suspension")
	FVector GetTopMountRelativeLocation() { return Suspension.SimData.TopMountRelativePos; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Suspension")
	FTransform GetWheelRelativeTransform() { return Suspension.SimData.WheelRelativeTransform; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Suspension")
	float GetSuspensionLength() { return Suspension.SimData.SuspensionCurrentLength; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Suspension")
	float GetSteeringAngle() { return Suspension.SimData.SteeringAngle; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Tire")
	float GetSlipRatio() { return Wheel.SimData.SlipRatio; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Tire")
	float GetSlipAngle() { return Wheel.SimData.SlipAngle; }

	UFUNCTION(BlueprintCallable, Category = "Tire", meta = (ToolTip = "Obtain a normalized tire combination slip value. This function can be used for tire skid particles or tire sound effects."))
	float GetNormalizedSlip(float LongitudinalScale = 1.f, float LateralScale = 1.f);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Wheel")
	float GetAngularVelocity() { return Wheel.SimData.AngularVelocity; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Wheel")
	float GetTotalInertia() { return Wheel.SimData.TotalInertia; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Wheel")
	FVector GetWorldLinaerVelocity() { return Wheel.SimData.WorldLinearVelocity; }

	UFUNCTION(BlueprintCallable, Category = "Physics", meta = (ToolTip = "Update the independent suspension and wheel physics"))
	void UpdatePhysics(
		float InPhysicsDeltaTime,
		float InDriveTorque,
		float InBrakeTorque, 
		float InHandbrakeTorque,
		float InSteeringAngle, 
		float InSwaybarForce, 
		float InReflectedInertia);

	UFUNCTION(BlueprintCallable, Category = "Physics", meta = (ToolTip = "Start updating the solid axle physics, this function will only prepare and do raycast"))
	void StartUpdateSolidAxlePhysics(float InSteeringAngle);

	UFUNCTION(BlueprintCallable, Category = "Physics", meta = (ToolTip = "Finalize updating the solid axle physics, this function will only apply the wheel transform"))
	void FinalizeUpdateSolidAxlePhysics(
		float InPhysicsDeltaTime, 
		float InDriveTorque,
		float InBrakeTorque,
		float InHandbrakeTorque,
		float InSwaybarForce,
		float InReflectedInertia,
		const FVector& InKnuckleWorldPos,
		const FVector& InAxleWorldDirection);

	UFUNCTION(BlueprintCallable, Category = "Suspension")
	void ApplySuspensionStateDirect(float InExtensionRatio = 1.f, float InSteeringAngle = 0.f)
	{ Suspension.ApplySuspensionStateDirect(InExtensionRatio, InSteeringAngle); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	void GetWheelCoordinator(UVehicleWheelCoordinatorComponent*& OutWheelCoordinator);

	UFUNCTION(BlueprintCallable, Category = "Physics")
	float ComputeFeedBackTorque();

	UFUNCTION(BlueprintCallable, Category = "Physics")
	void SetEspBrakeTorque(float NewTorque = 0.f) { Wheel.SimData.BrakeTorqueFromESP = NewTorque; }

	UFUNCTION(BlueprintCallable, Category = "Physics")
	float GetEspBrakeTorque() { return Wheel.SimData.BrakeTorqueFromESP; }

	UFUNCTION(BlueprintCallable, Category = "Physics")
	void SetP4MotorTorque(float NewTorque = 0.f) { Wheel.SimData.P4MotorTorque = NewTorque; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	float GetP4MotorTorque() { return Wheel.SimData.P4MotorTorque; }

	UFUNCTION(BlueprintCallable, Category = "Physics", meta = (ToolTip = "kg/m^2"))
	static void SetInertiaTensor(UPrimitiveComponent* InComponent, FVector InInertia);

	UFUNCTION(BlueprintCallable, Category = "Wheel")
	void GetWheelMovement(FVehicleWheelSimData& Out) { Out = Wheel.SimData; }

	UFUNCTION(BlueprintCallable, Category = "Suspension")
	void GetSuspensionMovement(FVehicleSuspensionSimData& Out) { Out = Suspension.SimData; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ReadOnly")
	UPrimitiveComponent* GetCarbody() { return Carbody; }

	UFUNCTION(BlueprintCallable, Category = "Suspension")
	bool GetRayCastResult(FHitResult& OutHitResult, bool& OutRefinement);
	bool GetRayCastResult() { return Suspension.SimData.bHitGround; }

	UFUNCTION(BlueprintCallable, Category = "Suspension")
	FVector GetRayCastHitLocation();

	UFUNCTION(BlueprintCallable, Category = "Suspension")
	float GetRayCastHitDistance() { return Suspension.SimData.HitDistance; }

	UFUNCTION(BlueprintCallable, Category = "Suspension")
	FVector GetRayCastImpactPoint() { return Suspension.SimData.HitStruct.ImpactPoint; }

	UFUNCTION(BlueprintCallable, Category = "Suspension")
	FTransform GetCarbodyWorldTransform() { return Suspension.SimData.CarbodyWorldTransform; }

	UFUNCTION(BlueprintCallable, Category = "Suspension")
	FVector GetKnuckleRelativePosition() { return Suspension.SimData.KnuckleRelativePos; }

	//debug draw
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DrawSuspension(
		float Duration = -1, 
		float Thickness = 0,
		bool bDrawSuspension = true, 
		bool bDrawWheel = true,
		bool bDrawRayCast = true);

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DrawSuspensionForce(
		float Duration = -1,
		float Thickness = 5, 
		float Length = 1);

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DrawWheelForce(
		float Duration = -1, 
		float Thickness = 5,
		float Length = 1, 
		bool bDrawVelocity = true,
		bool bDrawSlip = true, 
		bool bDrawInertia = true);


	UFUNCTION(BlueprintCallable, Category = "Mesh")
	bool SetMesh(
		float AxialHubOffset,
		UStaticMesh* NewWheelMesh,
		FTransform WheelMeshRelatvieTransform,
		UStaticMesh* NewBrakeMesh, 
		FTransform BrakeMeshRelativeTransform);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Mesh")
	void GetWheelAndBrakeMeshComponents(UStaticMeshComponent*& OutWheel, UStaticMeshComponent*& OutBrake)
	{
		OutWheel = WheelMeshComponent; OutBrake = BrakeMeshComponent;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Mesh")
	void GetWheelAndBrakeMeshes(UStaticMesh*& OutWheelMesh, UStaticMesh*& OutBrakeMesh)
	{
		OutWheelMesh = WheelMesh; OutBrakeMesh = BrakeMesh;
	}

	UFUNCTION(BlueprintCallable, Category = "Mesh")
	bool RefreshWheelMesh();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void UpdateWheelAnim(float DeltaTime = 0.f, float MaxAnimAngularVelocity = 60.f);

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void GetWheelKnuckleComponent(USceneComponent*& OutHub) { OutHub = WheelKnuckleComponent; }

	UFUNCTION(BlueprintCallable, Category = "Animation", meta = (ToolTip = "Compute and return the world location and world rotation of skid mark, the scale is local linear velocity of the wheel"))
	FTransform GetSkidMarkWorldTransform(float InSkidMarkBias, float InSkidMarkScale);

	UFUNCTION(BlueprintCallable, Category = "Animation", meta = (ToolTip = "Rotate the arm mesh around its own origin"))
	FQuat UpdateSuspensionArmAnim(USceneComponent* InArmMesh,
		FRotator InRotationOffset = FRotator(0.f, 0.f, 0.f));

	UFUNCTION(BlueprintCallable, Category = "Animation", meta = (ToolTip = "Rotate and scale the spring mesh around its own origin"))
	FTransform UpdateSuspensionSpringAnim(USceneComponent* InSpringMesh,
		FVector InScaleAxis = FVector(0.f, 0.f, 1.f),
		float InOffsetAlongArm = 0.f,
		FVector2D InKnuckleOffset = FVector2D(0.f, 0.f),
		FRotator InRotationOffset = FRotator(0.f, 0.f, 0.f),
		float InLengthBias = 0.f,
		FVector InInitialScale = FVector(1.f, 1.f, 1.f));

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void AttachComponentToKnuckle(USceneComponent* InComponent, FTransform InTransform);

private:
	float SafeDivide(auto a, auto b);
};
