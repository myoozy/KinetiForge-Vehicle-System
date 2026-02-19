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
struct KINETIFORGE_API FVehicleWheelConfig
{
	GENERATED_BODY()

	//WheelSetup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float Radius = 33.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float Width = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float Inertia = 1.f;

	/**
	* This is just a constant additional handbrake torque
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float RollingRisistance = 5.f;
};

USTRUCT(BlueprintType)
struct KINETIFORGE_API FVehicleTireConfig
{
	GENERATED_BODY()

	/**
	* Overall grip multiplier for this tire.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", DisplayName = "Global Friction Scale"))
	float FrictionMultiplier = 1.5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETireFrictionCombineMode TireFrictionCombineMode = ETireFrictionCombineMode::Average;

	/**
	* Simulates tire lag. Higher values = softer/laggy response.
	* Increase this if the vehicle jitters at low frame rates.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", DisplayName = "Relaxation Length"))
	FVector2f RelaxationLength = FVector2f(0.1f, 0.2f);

	/**
	* if value is 1, the wheel load will be proportional to suspension force. 
	* If value is 0, the wheel load will be constant (gravity of sprungmass).
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float WheelLoadInfluenceFactor = 0.8f;

	/**
	* Balances grip between turning and accelerating.
	* 0.5 = Balanced.
	* >0.5 = Prioritizes Turning (Easier to corner while braking).
	* <0.5 = Prioritizes Acceleration.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0", DisplayName = "Grip Distribution Bias"))
	float CombinedSlipBias = 0.5f;

	/**
	* How much longitudinal slip (spinning) reduces lateral grip.
	* 1.0 = Realistic (Spinning wheels can't turn).
	* 0.0 = Arcade (Full turning grip even when burning out).
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0", DisplayName = "Accel Affects Turning"))
	float LongitudinalToLateralInterference = 1.f;

	/**
	* How much lateral slip (drifting) reduces forward acceleration.
	* Lower values (e.g., 0.8) make it easier to maintain speed while drifting.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0", DisplayName = "Turning Affects Accel"))
	float LateralToLongitudinalInterference = 1.f;

	/**
	* Scales the Fx curve output. Determines max longitudinal force.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", DisplayName = "Longitudinal Grip Limit"))
	float MaxFx = 1.f;

	/**
	* Scales the Fy curve output. Determines max cornering force.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", DisplayName = "Lateral Grip Limit"))
	float MaxFy = 1.f;

	/**
	* Input: Slip Ratio. Output: Friction Coefficient.
	* Note: Slope at origin will be used to determine Optimal Slip.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Curve: Fx (Longitudinal)"))
	UCurveFloat* Fx = nullptr;

	/**
	* Input: Slip Angle (Deg). Output: Friction Coefficient.
	* Note: Slope at origin will be used to determine Optimal Slip.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Curve: Fy (Lateral)"))
	UCurveFloat* Fy = nullptr;
};

USTRUCT(BlueprintType)
struct KINETIFORGE_API FVehicleABSConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAntiBrakeSystemEnabled = true;

	/**
	* The target slip ratio the anti-brake-system try to maintain.
	* The value is dependent on your tire config and the road condition.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float OptimalSlip = 0.1f;

	/**
	* Minimum speed required to activate anti-brake-system.
	* Unit: meter per second, m/s
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float ActivationSpeed = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float Sensitivity = 5.f;
};

USTRUCT()
struct KINETIFORGE_API FVehicleWheelCachedRichCurves
{
	GENERATED_BODY()

	UPROPERTY()
	FRichCurve Fx;
	UPROPERTY()
	FRichCurve Fy;
};

USTRUCT(BlueprintType, meta = (ToolTip = "wheel state in simulation"))
struct KINETIFORGE_API FVehicleWheelSimState
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DeltaTime")
	float PhysicsDeltaTime = 0.008333f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DeltaTime")
	float PhysicsDeltaTimeInv = 120.f;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float R = 0.33f;				//Radius in meter
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float RInv = 3.0303f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float TotalInertia = 1.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float AngularVelocity = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float AngularAcceleration = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Slip")
	float LongSlipVelocity = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Slip")
	float SlipRatio = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Slip")
	float SlipAngle = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Slip")
	float PredictedSlipRatio = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float DynFrictionMultiplier = 1.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector2f LocalLinearVelocity = FVector2f(0.f, 0.f);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force")
	float WheelLoad = 0.f;
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
	FVector2f MFTireForce2D = FVector2f(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	FVector2f GravityCompensationForce = FVector2f(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	FVector2f TireForce2D = FVector2f(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	FVector3f TireForce = FVector3f(0.f, 0.f, 0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsLocked = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AntiBrake")
	bool bABSTriggered = false;
};