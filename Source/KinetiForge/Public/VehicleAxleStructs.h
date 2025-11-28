// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "VehicleAxleStructs.generated.h"

UENUM(BlueprintType)
enum class EVehicleAxleLayout : uint8
{
	TwoWheels,
	SingleLeft,
	SingleRight
};

UENUM(BlueprintType)
enum class EVehicleAxleSuspensionType : uint8
{
	Independent,
	Solid
};

USTRUCT(BlueprintType, Blueprintable)
struct FVehicleAxleConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AxleSetup", meta = (ClampMin = "0.0"))
	float DriveShaftInertia = 0.1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AxleSetup", meta = (ClampMin = "0.0"))
	float TrackWidth = 160.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AxleSetup", meta = (ClampMin = "0.0", ToolTip = "Raw weight before normalization, can be negative. Used to assign torque proportionally."))
	float TorqueWeight = 1.f;	// Raw weight before normalization, can be negative. Used to assign torque proportionally.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AxleSetup", meta = (ClampMin = "0.0"))
	float SwaybarStiffness = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AxleSetup", meta = (ClampMin = "0.0"))
	float MaxBrakeTorque = 2500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AxleSetup")
	bool bAffectedByHandbrake = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AxleSetup", meta = (ClampMin = "0.0"))
	float MaxHandbrakeTorque = 2500.f;
};

USTRUCT(BlueprintType, Blueprintable)
struct FVehicleAxleSteeringConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteeringSetup")
	bool bAffectedBySteering = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteeringSetup")
	float MaxSteeringAngle = 35.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteeringSetup")
	bool bAckermannSteering = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteeringSetup")
	float AckermannRatio = 1.f;
};

USTRUCT(BlueprintType, Blueprintable)
struct FVehicleSteeringAssistConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteeringSetup", meta = (ToolTip = "reduce steering angle when understeering or oversteering"))
	bool bSteeringAssistEnabled = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteeringSetup", meta = (ClampMin = "0.0"))
	float Level = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteeringSetup", meta = (ClampMin = "0.0", ClampMax = "0.99999", ToolTip = "steering assist smoothing(0 = immediately, 1 = never)"))
	float Smoothing = 0.8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteeringSetup", meta = (ClampMin = "0.0", ToolTip = "minimum speed(m / s) to activate assist"))
	float ActivationSpeed = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteeringSetup", meta = (ClampMin = "0.0", ToolTip = "minimum angle(degrees) to activate assist"))
	float ActivationAngle = 1.f;
};

USTRUCT(BlueprintType, Blueprintable)
struct FVehicleTCSConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCSSetup")
	bool bTractionControlSystemEnabled = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCSSetup", meta = (ClampMin = "0.0"))
	float OptimalSlip = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCSSetup", meta = (ClampMin = "0.0", ToolTip = "minimum speed(m / s) to activate assist"))
	float ActivationSpeed = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCSSetup", meta = (ClampMin = "0.0"))
	float Sensitivity = 1.f;
};


USTRUCT(BlueprintType, Blueprintable)
struct FVehicleAxleSimData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TractionControll")
	bool bTCSTriggered = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	int32 NumOfWheels = 2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	int32 NumOfWheelOnGround = 2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DeltaTime")
	float PhysicsDeltaTime = 0.0083333;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float LeftWheelSteeringAngle = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float RightWheelSteeringAngle = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float AxleDriveTorque = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float LeftDriveTorque = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float RightDriveTorque = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float SwaybarForce = 0.f;	//for left wheel
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float ReflectedInertiaOnWheel = 0.f;	//on single wheel
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float BrakeTorque = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float HandbrakeTorque = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	float SteeringAssistInput = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	float RealSteeringValue = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float AxleTotalInertia = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float AxleAngularVelocity = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float P3MotorTorque = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float WheelBase = 0.1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector WorldLinearVelocity = FVector(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector LocalLinearVelocity = FVector(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector CachedAxleCenter = FVector(0.f);
};