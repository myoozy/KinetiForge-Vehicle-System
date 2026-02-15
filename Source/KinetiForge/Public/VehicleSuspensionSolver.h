// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

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
		const FTransform& AsyncCarbodyWorldTransform,
		const UWorld* CurrentWorld,
		Chaos::FRigidBodyHandle_Internal* CarbodyHandle,
		float InDeltaTime,
		float InSteeringAngle,
		float InSwaybarForce
	);
	void StartUpdateSolidAxle(
		const float WheelRadius,
		const float WheelWidth,
		const FVehicleSuspensionKinematicsConfig& KineConfig,
		const FTransform& ComponentRelativeTransform,
		const FTransform& AsyncCarbodyWorldTransform,
		const UWorld* CurrentWorld,
		float InSteeringAngle,
		FVector& OutApporximatedWheelWorldPos,
		FVehicleSuspensionSimContext& Ctx
	);
	void FinalizeUpdateSolidAxle(
		const float WheelRadius,
		const FVehicleSuspensionKinematicsConfig& KineConfig,
		const FVehicleSuspensionSpringConfig& SpringConfig,
		const FTransform& AsyncCarbodyWorldTransform,
		Chaos::FRigidBodyHandle_Internal* CarbodyHandle,
		float InDeltaTime,
		float InSwaybarForce,
		FVehicleSuspensionSimContext& Ctx,
		const FVector& InKnuckleWorldPos,
		const FVector& InAxleWorldDirection,
		const FVector3f& TireForce
	);
	void ApplySuspensionStateDirect(
		const float WheelRadius,
		const FVehicleSuspensionKinematicsConfig& KineConfig,
		const FTransform& ComponentRelativeTransform,
		const FTransform& AsyncCarbodyWorldTransform,
		float InExtensionRatio = 1.f,
		float InSteeringAngle = 0.f
	);
	void StartApplySolidAxleStateDirect(
		const float WheelRadius,
		const FVehicleSuspensionKinematicsConfig& KineConfig,
		const FTransform& ComponentRelativeTransform,
		const FTransform& AsyncCarbodyWorldTransform,
		float InExtensionRatio,
		float InSteeringAngle,
		FVector& OutApporximatedWheelWorldPos,
		FVehicleSuspensionSimContext& Ctx
	);
	void FinalizeApplySolidAxleStateDirect(
		const float WheelRadius,
		const FVehicleSuspensionKinematicsConfig& KineConfig,
		FVehicleSuspensionSimContext& Ctx,
		const FVector& InKnuckleWorldPos,
		const FVector& InAxleWorldDirection
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

	void UpdateCachedRichCurves(
		FVehicleSuspensionKinematicsConfig& KineConfig);

	bool CheckAndFixTriangle(
		FVehicleSuspensionKinematicsConfig& KineConfig);//Abandoned

	static float GetVector2dAngleDegrees(FVector2D V2D);//Abandoned
	static FVector2D ComputeCircleIntersection(FVector2D A, float RA, float R0, bool ReturnX1 = true); //Abandoned //Compute the intersection of the circle on point A and the point on (0, 0)
	static FQuat4f MakeQuatFrom2DVectors(const FVector2f From, const FVector2f To, const FVector3f Axis);

	void CopyContextToState(const FVehicleSuspensionSimContext& Context);

	void CopyStateToContext(FVehicleSuspensionSimContext& Context);

	static FVector3f GetCamberToeCasterFromCurve(
		const FVehicleSuspensionCachedRichCurves& Curves,
		float CompressionRatio,
		float WheelYPosSign,
		float BaseCamber = 0.f,
		float BaseToe = 0.f,
		float BaseCaster = 0.f);

	static FQuat4f GetSpindleMountQuat(
		const FRotator3f& InSpindleMountRotationConfig,
		const float WheelPos);

	static FORCEINLINE FVector3f SuspensionPlaneToZYPlane(FVector2f V2D, float WheelYPosSign)
	{
		return FVector3f(0, V2D.Y * WheelYPosSign, V2D.X);
	}

	static FORCEINLINE FVector2f ZYPlaneToSuspensionPlane(FVector3f V3D, float WheelYPosSign)
	{
		return FVector2f(V3D.Z, V3D.Y * WheelYPosSign);
	}

	FVehicleSuspensionSimState State;
	FVehicleSuspensionCachedRichCurves CachedCurves;
	FVehicleSuspensionHitResult RayCastResult;

protected:
	FCollisionQueryParams QueryParams = FCollisionQueryParams::DefaultQueryParam;
	FCollisionResponseParams ResponseParams = FCollisionResponseParams::DefaultResponseParam;

private:
	//cache
	float CachedSpringStiffness = -1;
};
