// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "VehicleWheelStructs.generated.h"

UENUM(BlueprintType)
enum class ETireFrictionCombineMode : uint8
{
	Constant,
	Average,
	Multiply,
	Min,
	Max
};

USTRUCT(BlueprintType)
struct FVehicleWheelConfig
{
	GENERATED_BODY()

	//WheelSetup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float Radius = 33;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float Width = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float Inertia = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ToolTip = "This is just a constant additional handbrake torque"))
	float RollingRisistance = 5;
};

USTRUCT(BlueprintType)
struct FVehicleTireConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float FrictionMultiplier = 1.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETireFrictionCombineMode TireFrictionCombineMode = ETireFrictionCombineMode::Average;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ToolTip = "Higher value makes the wheel softer. If the vahicle is not stable at low physics sim frequency/low vehicle weight, try to set this higher."))
	FVector2D RelaxationLength = FVector2D(0.005, 0.01);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ToolTip = "This value determines the length of the semi-axis of the friction ellipse. The larger this value, the greater the longitudinal force that the tire can provide."))
	float MaxFx = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ToolTip = "This value determines the length of the semi-axis of the friction ellipse. The larger this value, the greater the lateral force that the tire can provide."))
	float MaxFy = 1.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float WheelLoadInfluenceFactor = 0.8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Input: absolute value of slip ratio; Output: normalized unscaled longitudinal tire force; If this curve is not set, the tire will be treated as a rigid body in the longitudinal direction."))
	UCurveFloat* Fx = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Input: absolute value of slip angle(in degrees); Output: normalized unscaled lateral tire force; If this curve is not set, the tire will be treated as a rigid body in the lateral direction."))
	UCurveFloat* Fy = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Input: absolute value of slip angle(in degrees); Output: scaling factor for Fx; Through this curve, the distribution of longitudinal forces can be manually adjusted. If no settings are made, the distribution of longitudinal forces will follow the classic friction ellipse."))
	UCurveFloat* Gx = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "Input: absolute value of slip ratio; Output: scaling factor for Fy; Through this curve, the distribution of lateral forces can be manually adjusted. If no settings are made, the distribution of lateral forces will follow the classic friction ellipse."))
	UCurveFloat* Gy = nullptr;
};

USTRUCT(BlueprintType)
struct FVehicleABSConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAntiBrakeSystemEnabled = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float MaxSlipRatio = 0.1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ToolTip = "Meter per second, m/s"))
	float ActivationSpeed = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float InterpSpeed = 500.f;
};

USTRUCT(BlueprintType)
struct FVehicleWheelSimData
{
	GENERATED_BODY()

	//WheelMovement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float R = 0.33;				//Radius in meter
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float RInv = 3.030303;			//1/R
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float ReflectedInertia = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float TotalInertia = 1.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float TotalInertiaInv = 1.f;	//1/TotalInertia
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float AngularVelocity = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Slip")
	float LongSlipVelocity = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Slip")
	float SlipRatio = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Slip")
	float SlipAngle = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float DynFrictionMultiplier = 1.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsLocked = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AntiBrake")
	bool bABSTriggered = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector WorldLinearVelocity = FVector(0.f, 0.f, 0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector2D LocalLinearVelocity = FVector2D(0.f, 0.f);

	//Force
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force")
	float P4MotorTorque = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float DriveTorque = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float BrakeTorque = 0.f;		//brake torque from brake + rolling resistance
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float BrakeTorqueFromBrake = 0.f;	//brake torque input + brake torque from esp; not including the rolling resistance
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float BrakeTorqueFromHandbrake = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float BrakeTorqueFromESP = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float TorqueFromGroundInteraction = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float LongForce = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float LatForce = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	FVector TyreForce = FVector(0.f, 0.f, 0.f);

	//Physics
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DeltaTime")
	float PhysicsDeltaTime = 0.008333;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DeltaTime")
	float PhysicsDeltaTimeInv = 120;		//1/Deltatime
};