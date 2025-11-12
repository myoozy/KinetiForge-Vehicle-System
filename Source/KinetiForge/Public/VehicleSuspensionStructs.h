// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "VehicleSuspensionStructs.generated.h"

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
	LineTrace					UMETA(ToolTip = "The fastest raycast mode."),
	SphereTrace					UMETA(ToolTip = "Using sphere(radius is wheel radius) instead of line trace. If impact point is wrong(e.g. hit the wall), it will do sphere trace again."),
	BoxTrace					UMETA(ToolTip = "Using a box (with 45deg of pitch displacement) to trace."),
	SphereTraceNoRefinement		UMETA(ToolTip = "Using a small sphere to trace(radius = min(WheelRadius, WheelWidth * 0.5)."),
	MultiSphereTrace			UMETA(ToolTip = "Similar to sphere trace. First it uses sphere trace. If the result is not valid(e.g. hit the wall), it will do box trace(to roughly check if there're obstacles). If box trace did hit, then it will do multi-sphere trace. The performance can be really bad in worst case. But normally the same as SphereTrace.")
};

UENUM(BlueprintType)
enum class EPositionToApplyForce : uint8
{
	ImpactPoint			UMETA(ToolTip = "Apply suspension and tire force at the impact point of the ray-casting"),
	ImpactPointWithBias UMETA(ToolTip = "Apply force at ImpactPoint + WheelRightVector * HalfWheelWidth"),
	WheelCenter			UMETA(ToolTip = "Apply suspension and tire force at the center of wheel")
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
	bool bRayCastRefined = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DeltaTime")
	float PhysicsDelatTime = 0.00833333;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast")
	float RayCastLength = 1.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast", meta = (ToolTip = "The smoothened hit distance from hit result (smoothened using spring and damper)"))
	float HitDistance = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast")
	float HitDistanceRate = 0.f;
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
	float SuspensionForce = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float WheelLoad = 0.f;
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
	FVector SuspensionForceVector = FVector(0.f);
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