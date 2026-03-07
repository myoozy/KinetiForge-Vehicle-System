// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VehicleADASComponent.generated.h"

class UVehicleDriveAssemblyComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KINETIFORGE_API UVehicleADASComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVehicleADASComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void TickADAS();

	FTimerHandle ADASTickTimerHandle;
	TWeakObjectPtr<UVehicleDriveAssemblyComponent> DriveAssemblyComponent = nullptr;
	TArray<FVector> CachedPathPoints;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (GetOptions = "GetNamesOfDriveAssembliesOfOwner"))
	FName DriveAssemblyName = FName();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config", meta = (GetOptions = "GetNamesOfDriveAssembliesOfOwner"))
	float RefreshInterval = 0.5f;

	UFUNCTION(BlueprintCallable, Category = "VehicleADAS")
	void SetDriveAssemblyComponent(UVehicleDriveAssemblyComponent* NewDriveAssembly);

	UFUNCTION(BlueprintCallable, Category = "VehicleADAS")
	UVehicleDriveAssemblyComponent* GetDriveAssemblyComponent();

	UFUNCTION(BlueprintCallable, Category = "VehicleADAS")
	static float GetMaxSpeedToBrake(float Distance, float Acceleration, float DeltaTime, float EndSpeed);

	/*
	* Returns the maximum speed to turn at the next path point (PathPoints[1])
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleADAS")
	static float GetMaxSpeedToTurn(const TArray<FVector>& PathPoints, float AccelLimit, float MinTurningRadius);

	/*
	* Using normalized engine rpm when shifting:
	* NormalizedRPM: 
	* when RPM = IdleRPM, NormalizedRPM = 0; 
	* when RPM = MaxRPM, NormalizedRPM = 1;
	* 
	* ShiftInterval:
	* ShiftInterval.X = NormalizedDownShiftRPM;
	* ShiftInterval.Y = NormalizedUpShiftRPM;
	* 
	* TargetSpeed: m/s
	* MaxAcceleration: m/s^2
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleADAS")
	static void UpdateCruiseControl(
		UVehicleDriveAssemblyComponent* DriveAssembly,
		float TargetSpeed = 0.f, 
		float MaxAcceleration = 0.f, 
		float DeltaTime = 0.5f,
		FVector2D ShiftInterval = FVector2D(0.2f, 0.4f)
	);
		
	/*
	* Approximate max drive force:
	* MaxDriveForce = MaxEngineTorque(including turbo boost) * GearRatio / AverageWheelRadius
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleADAS")
	static float GetVehicleMaxDriveForce(UVehicleDriveAssemblyComponent* DriveAssembly);

	/*
	* Approximate max brake force:
	* MaxBrakeForce = sum(BrakeTorqueOnWheel / WheelRadius)
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleADAS")
	static float GetVehicleMaxBrakeForce(UVehicleDriveAssemblyComponent* DriveAssembly);

	/*
	* Simply simulate a driver's behavior.
	* 
	* Using normalized engine rpm when shifting:
	* NormalizedRPM: 
	* when RPM = IdleRPM, NormalizedRPM = 0; 
	* when RPM = MaxRPM, NormalizedRPM = 1;
	* 
	* ShiftInterval:
	* ShiftInterval.X = NormalizedDownShiftRPM;
	* ShiftInterval.Y = NormalizedUpShiftRPM;
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleADAS")
	static void SpeedUpVehicle(
		UVehicleDriveAssemblyComponent* DriveAssembly,
		float MaxThrottle = 1.f,
		float MaxBrake = 1.f,
		float DeltaTime = 0.5f,
		bool bGoForward = true,
		FVector2D ShiftInterval = FVector2D(0.2f, 0.4f)
	);

	/*
	* Tolerance:
	* Unit: m/s; If |Velocity| < Tolerance, will be considered as stopped.
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleADAS")
	static void SlowDownVehicle(
		UVehicleDriveAssemblyComponent* DriveAssembly,
		float MaxBrake = 1.f,
		float Tolerance = 0.1f
	);

	/*
	* Directly drive to target location. Assuming there's no obstacles.
	* SpeedLimit: unit: m/s
	* AccelLimit: unit: m/s^2
	* EndSpeed: unit: m/s
	* Tolerance: unit: cm
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleADAS")
	static void UpdateDirectDrive(
		UVehicleDriveAssemblyComponent* DriveAssembly,
		FVector TargetLocation,
		float SpeedLimit = 15.f,
		float AccelLimit = 3.f,
		float EndSpeed = 0.f,
		float Tolerance = 50.f,
		float DeltaTime = 0.5f,
		FVector2D ShiftInterval = FVector2D(0.2f, 0.4f)
	);

	UFUNCTION(BlueprintCallable, Category = "VehicleADAS")
	static bool GetPathPointsToTarget(
		UWorld* CurrentWorld,
		FVector StartLocation,
		FVector TargetLocation, 
		TArray<FVector>& OutPathPoints,
		AActor* PathfindingContext = NULL
	);

	/*
	* Unit: cm
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleADAS")
	static float GetRemainingPathLength(const TArray<FVector>& Path, FVector CurrLoc);

	UFUNCTION(BlueprintCallable, Category = "VehicleADAS")
	static TArray<FVector> FixPathCollisions(
		UWorld* World,
		const TArray<FVector>& PathPoints,
		const TArray<AActor*>& IgnoredActors,
		float TraceRadius = 300.f,
		int32 MaxBounces = 1,
		int32 NumPointsToFix = -1,
		ECollisionChannel TraceChannel = ECollisionChannel::ECC_WorldDynamic
	);

	/*
	* This function emits numerous rays around the vehicle to determine where to drive.
	* FOV > 0: detect forward; FOV < 0: detect backward;
	* Returns false if the forward direction is blocked.
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleADAS")
	static bool GetBestTargetInFOV(
		FVector& OutBestTarget,
		float& OutBestScore,
		float& OutOptimalSpeedLimit,
		UVehicleDriveAssemblyComponent* DriveAssembly,
		FVector TargetLocation,
		float DeltaTime = 0.5f,
		float AccelLimit = 3.f,
		float MaxFOV = 90.f,
		float MinFOV = 0.f,
		float GoalWeight = 0.5f,
		int32 MaxNumOfRays = 6,
		ECollisionChannel TraceChannel = ECollisionChannel::ECC_WorldDynamic,
		bool bDebugDraw = false
	);

	/*
	* Simply by using ray casting to reach the target location
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleADAS")
	static void UpdateAutoPilotSimple(
		UVehicleDriveAssemblyComponent* DriveAssembly,
		FVector TargetLocation,
		float SpeedLimit = 15.f,
		float AccelLimit = 3.f,
		float EndSpeed = 0.f,
		float Tolerance = 50.f,
		float DeltaTime = 0.5f,
		float MaxFieldOfView = 90.f,
		float MinFieldOfView = 0.f,
		float GoalWeight = 0.5f,
		float StandstillThreshold = 0.5f,
		int32 MaxNumOfRays = 6,
		FVector2D ShiftInterval = FVector2D(0.2f, 0.4f),
		ECollisionChannel TraceChannel = ECollisionChannel::ECC_WorldDynamic,
		bool bDebugDraw = false
	);

	UFUNCTION(BlueprintCallable, Category = "VehicleADAS")
	static void UpdateAutoPilotNavMesh(
		UVehicleDriveAssemblyComponent* DriveAssembly,
		FVector TargetLocation,
		float SpeedLimit = 15.f,
		float AccelLimit = 3.f,
		float EndSpeed = 0.f,
		float Tolerance = 50.f,
		float DeltaTime = 0.5f,
		int32 LookaheadSteps = 3,
		FVector2D ShiftInterval = FVector2D(0.2f, 0.4f),
		bool bDebugDraw = false
	);

	/*
	* Do not call this function. It is still under developing
	*/
	UFUNCTION(BlueprintCallable, Category = "VehicleADAS")
	static void UpdateAutoPilotHumanLike(
		UVehicleDriveAssemblyComponent* DriveAssembly,
		FVector TargetLocation,
		float SpeedLimit = 15.f,
		float AccelLimit = 3.f,
		float EndSpeed = 0.f,
		float Tolerance = 50.f,
		float DeltaTime = 0.5f,
		float MaxFieldOfView = 90.f,
		float MinFieldOfView = 0.f,
		float GoalWeight = 0.5f,
		float StandstillThreshold = 0.5f,
		int32 MaxNumOfRays = 6,
		FVector2D ShiftInterval = FVector2D(0.2f, 0.4f),
		ECollisionChannel TraceChannel = ECollisionChannel::ECC_WorldDynamic,
		bool bDebugDraw = false
	);

private:
	UFUNCTION()
	TArray<FName> GetNamesOfDriveAssembliesOfOwner();
};
