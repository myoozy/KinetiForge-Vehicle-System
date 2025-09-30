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

	bool Initialize(TWeakObjectPtr<UVehicleWheelComponent> InTargetWheelComponent);
	void SetSprungMass(float NewSprungMass);
	float ComputeCriticalDamping();
	void UpdateSuspension(
		float InDeltaTime,
		float InSteeringAngle,
		float InSwaybarForce
	);
	void ApplySuspensionStateDirect(
		float InExtensionRatio = 1.f,
		float InSteeringAngle = 0.f
	);
	void DrawSuspension(
		float Duration = -1, 
		float Thickness = 0, 
		bool bDrawSuspension = true,
		bool bDrawWheel = true,
		bool bDrawRayCast = true);
	void DrawSuspensionForce(
		float Duration = -1,
		float Thickness = 5, 
		float Length = 1);

	bool CheckIsDampingDirty();

	bool CheckAndFixTriangle();//Abandoned

	static float GetVector2dAngleDegrees(FVector2D V2D);//Abandoned
	static float SafeDivide(float a, float b); //returns a / b
	static FVector2D ComputeCircleIntersection(FVector2D A, float RA, float R0, bool ReturnX1 = true); //Abandoned //Compute the intersection of the circle on point A and the point on (0, 0)
	static FQuat MakeQuatFrom2DVectors(const FVector2D From, const FVector2D To, const FVector Axis);
	bool SingleSphereTrace(FVector Start, FVector End, float Radius, FHitResult& OutHit);
	FVector SuspensionPlaneToZYPlane(FVector2D V2D);
	FVector2D ZYPlaneToSuspensionPlane(FVector V3D);
	FVector GetCamberCasterToeFromCurve();

	FVehicleSuspensionSimData SimData;

protected:
	TWeakObjectPtr<UVehicleWheelComponent> TargetWheelComponent;

	FCollisionQueryParams QueryParams = FCollisionQueryParams::DefaultQueryParam;

private:
	void PrepareSimulation();
	float ComputeValidPreload();
	void ComputeRayCastLocation();
	void SuspensionRayCast();
	void SuspensionLineTrace();
	void SuspensionSphereTrace();
	void ComputeStraightSuspension();
	void ComputeMacpherson();
	void ComputeDoubleWishbone();
	void ComputeSuspensionForce();

	//cache
	float CachedSpringStiffness = -1;
};
