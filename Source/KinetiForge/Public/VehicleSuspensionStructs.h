// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "VehicleSuspensionStructs.generated.h"

UENUM(BlueprintType)
enum class ESuspensionType : uint8
{
	StraightLine		UMETA(ToolTip = "Only moves up and down. This can simulate fork suspension like the front suspension of a motorbike, but with camber and toe setup."),
	Macpherson			UMETA(ToolTip = "Simplified macpherson suspension. It has one control arm and one strut. The rotation of the wheel knuckle or the wheel hub will be automatically calculated."),
	DoubleWishbone		UMETA(ToolTip = "Simplified wishbone suspension. It only has one control arm and one strut. The changes of the camber/toe/caster are defined by user's float curves.")
};

UENUM(BlueprintType)
enum class ESuspensionRayCastMode : uint8
{
	LineTrace					UMETA(ToolTip = "The fastest raycast mode. The Line Trace will start at outer side of the wheel."),
	SphereTrace					UMETA(ToolTip = "Using sphere(radius is wheel radius) instead of line trace. If impact point is wrong(e.g. hit the wall), it will do sphere trace again."),
	BoxTrace					UMETA(ToolTip = "Using a box (with 45deg of pitch displacement) to trace."),
	SphereTraceNoRefinement		UMETA(ToolTip = "Using a small sphere to trace(radius = min(WheelRadius, WheelWidth * 0.5)."),
	MultiSphereTrace			UMETA(ToolTip = "Similar to sphere trace. First it uses sphere trace. If the result is not valid(e.g. hit the wall), it will do box trace(to roughly check if there're obstacles). If box trace did hit, then it will do multi-sphere trace. The performance can be really bad in worst case. But normally the same as SphereTrace. Some times not very stable.")
};

UENUM(BlueprintType)
enum class EPositionToApplyForce : uint8
{
	ImpactPoint			UMETA(ToolTip = "Apply suspension and tire force at the impact point of the ray-casting"),
	WheelCenter			UMETA(ToolTip = "Apply suspension and tire force at the center of wheel, which causes less pitch and roll")
};

USTRUCT(BlueprintType)
struct KINETIFORGE_API FVehicleSuspensionKinematicsConfig
{
	GENERATED_BODY()

	//SuspensionSetup
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESuspensionType SuspensionType = ESuspensionType::Macpherson;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESuspensionRayCastMode RayCastMode = ESuspensionRayCastMode::LineTrace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_WorldDynamic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPositionToApplyForce PositionToApplyForce = EPositionToApplyForce::ImpactPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "The location of the top mount relative to the pivot of the arm. It is under the coordinate of the wheel component, not car body."))
	FVector3f TopMountPosition = FVector3f(0.f, 40.f, 35.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ToolTip = "Unit: cm. The traveling distance of the strut"))
	float Stroke = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ToolTip = "Unit: cm. The length of the strut when the suspension is fully compressed."))
	float MinStrutLength = 30.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ToolTip = "Unit: cm"))
	float ArmLength = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "The distance of the wheel hub(where the wheel is mounted) to the wheel knuckle(where the arm and wheel-hub/strut is connected)."))
	float AxialHubOffset = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "For StraightLine: the camber under all the conditions; For Macpherson: the camber when the control arm is horizontal; For DoubleWishbone: the camber when the suspension is not compressed."))
	float BaseCamber = -1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "For StraightLine: the toe under all the conditions; For Macpherson: the toe when the control arm is horizontal; For DoubleWishbone: the toe when the suspension is not compressed."))
	float BaseToe = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "SuspensionType == ESuspensionType::DoubleWishbone", EditConditionHides, ToolTip = "X: SuspensionCompressionRatio; Y:CamberGain; Only enabled when the suspension type is double-wishbone"))
	UCurveFloat* CamberCurve = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "SuspensionType == ESuspensionType::DoubleWishbone", EditConditionHides, ToolTip = "X: SuspensionCompressionRatio; Y:ToeGain; Only enabled when the suspension type is double-wishbone"))
	UCurveFloat* ToeCurve = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "SuspensionType == ESuspensionType::DoubleWishbone", EditConditionHides, ToolTip = "X: SuspensionCompressionRatio; Y:Caster; Only enabled when the suspension type is double-wishbone"))
	UCurveFloat* CasterCurve = nullptr;
};

USTRUCT(BlueprintType)
struct KINETIFORGE_API FVehicleSuspensionSpringConfig
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
	float BottomOutStiffness = 0.8f;
};

USTRUCT()
struct KINETIFORGE_API FVehicleSuspensionCachedRichCurves
{
	GENERATED_BODY()

	UPROPERTY()
	FRichCurve CamberCurve;
	UPROPERTY()
	FRichCurve ToeCurve;
	UPROPERTY()
	FRichCurve CasterCurve;
};

USTRUCT(BlueprintType, meta = (ToolTip = "A simplified HitResult"))
struct KINETIFORGE_API FVehicleSuspensionHitResult
{
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<UPhysicalMaterial> PhysMaterial;
	UPROPERTY()
	TWeakObjectPtr<UPrimitiveComponent> Component;
	UPROPERTY()
	FName BoneName;
	UPROPERTY()
	bool bBlockingHit;
	UPROPERTY()
	FVector_NetQuantize TraceStart;
	UPROPERTY()
	FVector_NetQuantize TraceEnd;
	UPROPERTY()
	FVector_NetQuantize Location;
	UPROPERTY()
	FQuat4f TraceRot;
};

USTRUCT(BlueprintType, meta = (ToolTip = "suspension state in simulation"))
struct KINETIFORGE_API FVehicleSuspensionSimState
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SteeringAngle = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	float SuspensionCurrentLength = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	float CriticalDamping = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float SprungMass = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float ForceAlongImpactNormal = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FVector2f KnucklePos2D = FVector2f(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FVector3f WheelRightVector = FVector3f(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry", meta = (ToolTip = "From Knuckle to Center of wheel"))
	FVector3f WheelCenterToKnuckle = FVector3f(0.f);		//relative, from knuckle to center of the wheel
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FVector3f KnuckleRelativePos = FVector3f(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FVector3f TopMountRelativePos = FVector3f(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry", meta = (ToolTip = "the strut also represents the kingpin, if it is macpherson or straight line"))
	FVector3f StrutDirection = FVector3f(0.f);	// strut direction in world space
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector3f ImpactPointWorldVelocity = FVector3f(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast")
	FVector3f ImpactNormal = FVector3f(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast")
	FVector ImpactPoint = FVector(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FVector WheelWorldPos = FVector(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FQuat4f WheelRelativeRotation = FQuat4f(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast")
	float ImpactFriction = 1.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Position")
	bool bIsRightWheel = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast")
	bool bHitGround = true;
};

USTRUCT(BlueprintType, meta = (ToolTip = "suspension context in simulation"))
struct KINETIFORGE_API FVehicleSuspensionSimContext
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Position")
	float WheelPos = 1.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DeltaTime")
	float PhysicsDelatTime = 0.00833333;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast")
	float RayCastLength = 1.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast", meta = (ToolTip = "The smoothened hit distance from hit result (smoothened using spring and damper)"))
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
	float SuspensionForce = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float ForceAlongImpactNormal = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float SprungMass = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float WorldGravityZ = 9.8f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FVector2f TopMountPos2D = FVector2f(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FVector2f KnucklePos2D = FVector2f(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast")
	FVector2f RayCastStart2D = FVector2f(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry", meta = (ToolTip = "the strut also represents the kingpin, if it is macpherson or straight line"))
	FVector2f StrutDirection2D = FVector2f(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FVector3f ComponentRelativeForwardVector = FVector3f(0.f);		//relative, also the axis of arm
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry", meta = (ToolTip = "From Knuckle to Center of wheel"))
	FVector3f WheelCenterToKnuckle = FVector3f(0.f);		//relative, from knuckle to center of the wheel
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FVector3f KnuckleRelativePos = FVector3f(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FVector3f TopMountRelativePos = FVector3f(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry", meta = (ToolTip = "the strut also represents the kingpin, if it is macpherson or straight line"))
	FVector3f StrutRelativeDirection = FVector3f(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector3f ImpactPointWorldVelocity = FVector3f(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FVector ComponentUpVector = FVector(0.f);			//world
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FVector WheelRightVector = FVector(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast")
	FVector RayCastStartPos = FVector(0.f);	//for sphere trace
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast")
	FVector RayCastEndPos = FVector(0.f);		//for sphere trace
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry", meta = (ToolTip = "the strut also represents the kingpin, if it is macpherson or straight line"))
	FVector StrutDirection = FVector(0.f);	// strut direction in world space
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FVector WheelWorldPos = FVector(0.f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast")
	bool bHitGround = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast")
	bool bRayCastRefined = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast")
	float ImpactFriction = 1.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast")
	FTransform RayCastTransform = FTransform(FQuat(0.f));
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FTransform CarbodyWorldTransform = FTransform(FQuat(0.f));
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FTransform3f RelativeTransform = FTransform3f(FQuat4f(0.f));
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Geometry")
	FTransform3f WheelRelativeTransform = FTransform3f(FQuat4f(0.f));	//relative to carbody
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RayCast")
	FHitResult HitStruct = FHitResult();
};