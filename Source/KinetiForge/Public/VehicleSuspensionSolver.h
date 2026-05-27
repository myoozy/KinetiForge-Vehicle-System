// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "VehicleSuspensionStructs.h"

class UVehicleWheelComponent;

/**
 * 
 */
class KINETIFORGE_API FVehicleSuspensionSolver
{
public:
	FVehicleSuspensionSolver();
	~FVehicleSuspensionSolver();

	bool Initialize(UVehicleWheelComponent* WheelComponent);
	void SetSprungMass(
		const FVehicleSuspensionSpringConfig& SpringConfig,
		const float NewSprungMass);
	void UpdateSuspension(
		const float WheelRadius,
		const float WheelWidth,
		const FVector3f& TireForce,
		const FVehicleSuspensionKinematicsConfig& KineConfig,
		const FVehicleSuspensionSpringConfig& SpringConfig,
		const FTransform& ComponentRelativeTransform,
		const FTransform& AsyncChassisWorldTransform,
		const UWorld* CurrentWorld,
		Chaos::FRigidBodyHandle_Internal* ChassisHandle,
		float InDeltaTime,
		float InSteeringAngle,
		float InSwaybarForce
	);
	void StartUpdateSolidAxle(
		const float WheelRadius,
		const float WheelWidth,
		const FVehicleSuspensionKinematicsConfig& KineConfig,
		const FTransform& ComponentRelativeTransform,
		const FTransform& AsyncChassisWorldTransform,
		const UWorld* CurrentWorld,
		float InSteeringAngle,
		FVector& OutHitWorldLocation,
		FVehicleSuspensionSimContext& Ctx
	);
	void FinalizeUpdateSolidAxle(
		const float WheelRadius,
		const FVehicleSuspensionKinematicsConfig& KineConfig,
		const FVehicleSuspensionSpringConfig& SpringConfig,
		const FTransform& AsyncChassisWorldTransform,
		Chaos::FRigidBodyHandle_Internal* ChassisHandle,
		float InDeltaTime,
		float InSwaybarForce,
		FVehicleSuspensionSimContext& Ctx,
		const float InTrackWidth,
		const FVector& InThisWheelHitWorldLocation,
		const FVector& InOtherWheelHitWorldLocation,
		const FVector3f& TireForce
	);
	static void RoughlyInitializeState(
		const FTransform& ComponentRelativeTransform,
		const FVehicleSuspensionKinematicsConfig& KineConfig,
		FVehicleSuspensionSimState& InState
	);
	static FVehicleSuspensionSimState SolveKinematicsAtExtension(
		const float WheelRadius,
		const FVehicleSuspensionKinematicsConfig& KineConfig,
		const FTransform& ComponentRelativeTransform,
		float InExtensionRatio = 1.f,
		float InSteeringAngle = 0.f,
		int32 Iteration = 2,
		const FVehicleSuspensionCachedLUTs* LUTs = nullptr,
		const FVehicleSuspensionSimState* PrevState = nullptr
	);
	static void StartSolveSolidAxleAtExtension(
		const FVehicleSuspensionSimState& PrevState,
		const float WheelRadius,
		const FVehicleSuspensionKinematicsConfig& KineConfig,
		const FTransform& ComponentRelativeTransform,
		float InExtensionRatio,
		float InSteeringAngle,
		FVector& OutHitWorldLocation,
		FVehicleSuspensionSimContext& Ctx
	);
	static FVehicleSuspensionSimState FinalizeSolveSolidAxleAtExtension(
		const float WheelRadius,
		const FVehicleSuspensionKinematicsConfig& KineConfig,
		FVehicleSuspensionSimContext& Ctx,
		const float InTrackWidth,
		const FVector& InThisWheelHitWorldLocation,
		const FVector& InOtherWheelHitWorldLocation
	);
	void DrawSuspension(
		UVehicleWheelComponent* WheelComponent,
		float Duration = -1, 
		float Thickness = 0, 
		bool bDrawSuspension = true,
		bool bDrawWheel = true,
		bool bDrawRayCast = true);
	void DrawSuspensionForce(
		UVehicleWheelComponent* WheelComponent,
		float Duration = -1,
		float Thickness = 5, 
		float Length = 1);

	void UpdateCachedLUTs(
		const FVehicleSuspensionKinematicsConfig& KineConfig);

	static void CacheLUTs(FVehicleSuspensionCachedLUTs& LUTs, const FVehicleSuspensionKinematicsConfig& KineConfig);

	static float GetVector2dAngleDegrees(FVector2D V2D);//Abandoned
	static FVector2D ComputeCircleIntersection(FVector2D A, float RA, float R0, bool ReturnX1 = true); //Abandoned //Compute the intersection of the circle on point A and the point on (0, 0)
	static FQuat4f MakeQuatFrom2DVectors(const FVector2f From, const FVector2f To, const FVector3f Axis);

	static void CopyContextToState(const FVehicleSuspensionSimContext& Context, FVehicleSuspensionSimState& NewState);
	static void CopyContextToHitResult(const FVehicleSuspensionSimContext& Context, FVehicleSuspensionHitResult& NewHitResult);
	static void CopyStateToContext(const FVehicleSuspensionSimState& PrevState, FVehicleSuspensionSimContext& Context);

	static FRotator3f GetCamberToeCasterFromLUTs(
		const FVehicleSuspensionCachedLUTs& LUTs,
		float CompressionRatio,
		float WheelYPosSign,
		float BaseCamber = 0.f,
		float BaseToe = 0.f,
		float BaseCaster = 0.f);

	static FQuat4f GetSpindleMountQuat(
		const FRotator3f& InSpindleMountRotationConfig,
		const float WheelPos);

	static FORCEINLINE FVector3f Coord2DTo3D(FVector2f V2D, float WheelYPosSign)
	{
		return FVector3f(0, V2D.Y * WheelYPosSign, V2D.X);
	}

	static FORCEINLINE FVector2f Coord3DTo2D(FVector3f V3D, float WheelYPosSign)
	{
		return FVector2f(V3D.Z, V3D.Y * WheelYPosSign);
	}

	static FVector3f GetTopMountChassisLocation(
		const FVehicleSuspensionKinematicsConfig& Config,
		const FTransform3f& WheelComponentRelativeTransform
	);

	static void GetLowerWishboneState(
		const FVehicleSuspensionKinematicsConfig& Config,
		const FVehicleSuspensionSimState& SuspensionState,
		const FTransform3f& WheelComponentRelativeTransform,
		FVector3f& OutPivotChassisLocation,
		FVector3f& OutAxisChassisDirection,
		FVector3f& OutBallJointChassisLocation
	);

	static void GetUpperWishboneState(
		const FVehicleSuspensionKinematicsConfig& Config,
		const FVehicleSuspensionSimState& SuspensionState,
		const FTransform3f& WheelComponentRelativeTransform,
		FVector3f& OutPivotChassisLocation,
		FVector3f& OutAxisChassisDirection,
		FVector3f& OutBallJointChassisLocation
	);

	FVehicleSuspensionSimState State;
	FVehicleSuspensionCachedLUTs CachedLUTs;
	FVehicleSuspensionHitResult RayCastResult;

protected:
	FCollisionQueryParams QueryParams = FCollisionQueryParams::DefaultQueryParam;
	FCollisionResponseParams ResponseParams = FCollisionResponseParams::DefaultResponseParam;

private:
	static void PrepareSimulation(
		FVehicleSuspensionSimContext& Ctx,
		const FTransform& ComponentRelativeTransform,
		const FTransform& AsyncChassisWorldTransform,
		const FVehicleSuspensionKinematicsConfig& Config
	);
	static void ComputeStraightRayCastLocation(
		FVehicleSuspensionSimContext& Ctx,
		const FVehicleSuspensionKinematicsConfig& Config
	);
	static void ComputeWishboneRayCastLocation(
		FVehicleSuspensionSimContext& Ctx,
		const FVehicleSuspensionKinematicsConfig& Config
	);
	static void ComputeRayCastLocation(
		FVehicleSuspensionSimContext& Ctx,
		const FVehicleSuspensionKinematicsConfig& Config
	);
	static bool ShouldDoRefinedTrace(
		FVehicleSuspensionSimContext& Ctx,
		const float HalfWheelWidth,
		const FVehicleSuspensionKinematicsConfig& Config
	);
	static void ComputeHitDistance(
		FVehicleSuspensionSimContext& Ctx,
		const float WheelRadius,
		const float EquivalentSphereTraceRadius
	);
	static void CacheImpactFriction(
		FVehicleSuspensionSimContext& Ctx
	);
	static float SuspensionLineTrace(
		FVehicleSuspensionSimContext& Ctx,
		const UWorld* World,
		const float WheelRadius,
		const float HalfWheelWidth,
		const FCollisionQueryParams& QueryParams,
		const FCollisionResponseParams& ResponseParams,
		const FVehicleSuspensionKinematicsConfig& Config
	);
	static float SuspensionSphereTrace(
		FVehicleSuspensionSimContext& Ctx,
		const UWorld* World,
		const float WheelRadius,
		const float HalfWheelWidth,
		const FCollisionQueryParams& QueryParams,
		const FCollisionResponseParams& ResponseParams,
		const FVehicleSuspensionKinematicsConfig& Config
	);
	static float SuspensionBoxTrace(
		FVehicleSuspensionSimContext& Ctx,
		const UWorld* World,
		const float WheelRadius,
		const float HalfWheelWidth,
		const FCollisionQueryParams& QueryParams,
		const FCollisionResponseParams& ResponseParams,
		const FVehicleSuspensionKinematicsConfig& Config
	);
	static float SuspensionSphereTraceNoRefinement(
		FVehicleSuspensionSimContext& Ctx,
		const UWorld* World,
		const float WheelRadius,
		const float HalfWheelWidth,
		const FCollisionQueryParams& QueryParams,
		const FCollisionResponseParams& ResponseParams,
		const FVehicleSuspensionKinematicsConfig& Config
	);
	static float SuspensionMultiSphereTrace(
		FVehicleSuspensionSimContext& Ctx,
		const UWorld* World,
		const float WheelRadius,
		const float HalfWheelWidth,
		const FCollisionQueryParams& QueryParams,
		const FCollisionResponseParams& ResponseParams,
		const FVehicleSuspensionKinematicsConfig& Config
	);
	static void SuspensionRayCast(
		FVehicleSuspensionSimContext& Ctx,
		const UWorld* World,
		const float WheelRadius,
		const float HalfWheelWidth,
		const FCollisionQueryParams& QueryParams,
		const FCollisionResponseParams& ResponseParams,
		const FVehicleSuspensionKinematicsConfig& Config
	);
	static void SolveLowerWishbone(
		FVehicleSuspensionSimContext& Ctx,
		const float WheelRadius,
		const FVehicleSuspensionKinematicsConfig& Config
	);
	static void ComputeStraightSuspension(
		FVehicleSuspensionSimContext& Ctx,
		const float WheelRadius,
		const FVehicleSuspensionKinematicsConfig& Config,
		const FVehicleSuspensionCachedLUTs& LUTs
	);
	static void ComputeMacpherson(
		FVehicleSuspensionSimContext& Ctx,
		const float WheelRadius,
		const FVehicleSuspensionKinematicsConfig& Config,
		const FVehicleSuspensionCachedLUTs& LUTs
	);
	static bool SolveUpperWishbone(
		const FVector3f& LowerBallPos,
		float KnuckleLength,
		const FVector3f& UpperPivotPos,
		const FVector3f& UpperPivotAxis,
		const FVector3f& StrutDir,
		float UpperArmLength,
		FVector3f& OutUpperBallPos
	);
	static void ComputeDoubleWishbone(
		FVehicleSuspensionSimContext& Ctx,
		const float WheelRadius,
		const FVehicleSuspensionKinematicsConfig& Config,
		const FVehicleSuspensionCachedLUTs& LUTs
	);
	static void ComputeSolidAxle(
		FVehicleSuspensionSimContext& Ctx,
		const float WheelRadius,
		const FVehicleSuspensionKinematicsConfig& Config,
		const float TrackWidth,
		const FVector ThisWheelHitWorldLocation,
		const FVector OtherWheelHitWorldLocation
	);
	static bool Solve2DLineIntersection(
		const FVector2f& P1, const FVector2f& P2,
		const FVector2f& P3, const FVector2f& P4,
		FVector2f& OutIntersection
	);
	static float SolveSwingArmSlope2D(
		const FVector2f& LowerInner, const FVector2f& LowerOuter,
		const FVector2f& UpperInner, const FVector2f& UpperOuter,
		const FVector2f& ContactPatch
	);
	static float CalculateMacPhersonAntiPitchScale(
		const FVector3f& TopMount, const FVector3f& StrutDir,
		const FVector3f& LowerPivot, const FVector3f& LowerBallJoint, const FVector3f& LowerAxis,
		const FVector3f& HubLocation, float WheelRadius
	);
	static float CalculateMacPhersonRollCenterHeight(
		const FVector3f& TopMount, const FVector3f& StrutDir,
		const FVector3f& LowerPivot, const FVector3f& LowerBallJoint, const FVector3f& LowerAxis,
		const FVector3f& HubLocation, float WheelRadius
	);
	static float CalculateDoubleWishboneAntiPitchScale(
		const FVector3f& UpperPivot, const FVector3f& UpperBallJoint, const FVector3f& UpperAxis,
		const FVector3f& LowerPivot, const FVector3f& LowerBallJoint, const FVector3f& LowerAxis,
		const FVector3f& HubLocation, float WheelRadius
	);
	static float CalculateDoubleWishboneRollCenterHeight(
		const FVector3f& UpperPivot, const FVector3f& UpperBallJoint, const FVector3f& UpperAxis,
		const FVector3f& LowerPivot, const FVector3f& LowerBallJoint, const FVector3f& LowerAxis,
		const FVector3f& HubLocation, float WheelRadius
	);
	static void ComputeAntiPitchRollGeometry(
		FVehicleSuspensionSimContext& Ctx,
		Chaos::FRigidBodyHandle_Internal* ChassisHandle,
		const bool bOnlyFromLUTs, // for solid axles
		const EVehicleIndependentSuspensionType SuspensionType,
		const float WheelRadius,
		const FVehicleSuspensionCachedLUTs& LUTs,
		const FTransform& AsyncChassisWorldTransform,
		const FVector3f& TireForce
	);
	static void FetchChassisPhysicsState(
		Chaos::FRigidBodyHandle_Internal* ChassisHandle,
		FVehicleChassisSimState& OutState
	);
	static void CalculateImpactPointWorldVelocity(
		FVehicleSuspensionSimContext& Ctx,
		const FVehicleChassisSimState& ChassisState
	);
	static Chaos::FVec3 CalculatePointEffectiveMass3D(
		const Chaos::FReal TotalMass,
		const Chaos::FMatrix33& WorldInvInertiaTensor,
		const Chaos::FVec3& CoM_WorldLocation,
		const Chaos::FVec3& Contact_WorldLocation,
		Chaos::FVec3 ContactNormal,
		Chaos::FVec3 WheelRight
	);
	static float GetCriticalDamping(
		const float SpringStiffness,
		const float StaticSprungMass
	);
	static void ComputeSuspensionForce(
		FVehicleSuspensionSimContext& Ctx,
		const float WheelRadius,
		const FVehicleChassisSimState& ChassisState,
		const FVehicleSuspensionSpringConfig& SpringConfig,
		const FVehicleSuspensionKinematicsConfig& KineConfig,
		const FVehicleSuspensionCachedLUTs& LUTs
	);
};
