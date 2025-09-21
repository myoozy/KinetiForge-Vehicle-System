// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "VehicleChassisParams.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ESuspensionType : uint8
{
	StraightLine,
	Macpherson,
	DoubleWishbone
};

UENUM(BlueprintType)
enum class ESuspensionRayCastMode : uint8
{
	LineTrace,
	SphereTrace
};

UENUM(BlueprintType)
enum class EPositionToApplyForce : uint8
{
	ImpactPoint			UMETA(ToolTip = "Apply suspension and tire force at the impact point of the ray-casting"),
	ImpactPointWithBias UMETA(ToolTip = "Apply force at ImpactPoint + WheelRightVector * HalfWheelWidth"),
	WheelCenter			UMETA(ToolTip = "Apply suspension and tire force at the center of wheel")
};

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
	float BrakeTorque = 0.f;		//braketorque + rolling resistance
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float BrakeTorqueFromBrake = 0.f;	//not including the rolling resistance
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float BrakeTorqueFromHandbrake = 0.f;
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

USTRUCT(BlueprintType)
struct FVehicleSuspensionKinematicsConfig
{
	GENERATED_BODY()

	//SuspensionSetup
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESuspensionType SuspensionType = ESuspensionType::Macpherson;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESuspensionRayCastMode RayCastMode = ESuspensionRayCastMode::LineTrace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_WorldDynamic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "X: Horizontal; Y:Vertical"))
	FVector2D TopMountPosition = FVector2D(-2.f, 15.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ToolTip = "cm"))
	float Stroke = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ToolTip = "cm"))
	float MinStrutLength = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ToolTip = "cm"))
	float ArmLength = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SteeringAxleOffset = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseCamber = -1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseToe = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "X: SuspensionCompressionRatio; Y:CamberGain"))
	UCurveFloat* CamberCurve = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "X: SuspensionCompressionRatio; Y:CasterGain"))
	UCurveFloat* CasterCurve = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "X: SuspensionCompressionRatio; Y:ToeGain"))
	UCurveFloat* ToeCurve = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPositionToApplyForce PositionToApplyForce = EPositionToApplyForce::ImpactPointWithBias;
};

USTRUCT(BlueprintType)
struct FVehicleSuspensionSpringConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float SpringStiffness = 350.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float CompressionDampingRatio = 0.4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float ReboundDampingRatio = 0.6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float SpringPreload = 0.f;			//N
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0", ToolTip = "Scales the stiffness of the auxiliary spring to prevent ground penetration at full suspension compression."))
	float BottomOutStiffness = 0.f;
};

USTRUCT(BlueprintType)
struct FVehicleSuspensionSimData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Position")
	float WheelPos = 1.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast")
	bool bHitGround = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast")
	bool bRayCastRevised = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DeltaTime")
	float PhysicsDelatTime = 0.00833333;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast")
	float RayCastLength = 1.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast")
	float HitDistance = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	float SuspensionCurrentLength = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	float SuspensionExtensionRatio = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SteeringAngle = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	float CriticalDamping = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	float ValidPreload = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float SwaybarForce = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float SuspensionForceRaw = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float SuspensionForce = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float PositiveSuspensionForce = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float SprungMass = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float WorldGravityZ = 9.8;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FVector2D TopMountPos2D = FVector2D(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FVector2D BallJointPos2D = FVector2D(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast")
	FVector2D RayCastStart2D = FVector2D(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FVector2D StrutDirection2D = FVector2D(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FVector ComponentUpVector = FVector(0.f);			//world
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FVector ComponentRelativeForwardVector = FVector(0.f);		//relative
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FVector WheelRightVector = FVector(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FVector WheelOffsetToBallJoint = FVector(0.f);		//relative
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FVector BallJointRelativePos = FVector(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FVector TopMountRelativePos = FVector(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast")
	FVector RayCastStartPos = FVector(0.f);	//for sphere trace
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast")
	FVector RayCastEndPos = FVector(0.f);		//for sphere trace
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FVector StrutRelativeDirection = FVector(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FVector StrutDirection = FVector(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	FVector SuspensionForceVecRaw = FVector(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	FVector SuspensionForceVector = FVector(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast")
	FVector ImpactPointToRayCastStart = FVector(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FVector WheelWorldPos = FVector(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast")
	FTransform RayCastTransform = FTransform(FQuat(0.f));
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FTransform CarbodyWorldTransform = FTransform(FQuat(0.f));
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FTransform RelativeTransform = FTransform(FQuat(0.f));
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FTransform WheelRelativeTransform = FTransform(FQuat(0.f));	//relative to carbody
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast")
	FHitResult HitStruct = FHitResult();
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
	float MaxSlipRatio = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCSSetup", meta = (ClampMin = "0.0", ToolTip = "minimum speed(m / s) to activate assist"))
	float ActivationSpeed = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TCSSetup", meta = (ClampMin = "0.0"))
	float InterpSpeed = 100.f;
};


USTRUCT(BlueprintType, Blueprintable)
struct FVehicleAxleSimData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TractionControll")
	bool bTCSTriggered = false;
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