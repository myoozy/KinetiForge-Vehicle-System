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

	bool Initialize(UVehicleWheelComponent* InTargetWheelComponent);
	void SetSprungMass(float NewSprungMass);
	float ComputeCriticalDamping();
	void UpdateSuspension(
		float InDeltaTime,
		float InSteeringAngle,
		float InSwaybarForce
	);
	void StartUpdateSolidAxle(
		float InSteeringAngle,
		FVector& OutApporximatedWheelWorldPos,
		FVehicleSuspensionSimContext& Ctx
	);
	void FinalizeUpdateSolidAxle(
		float InDeltaTime,
		float InSwaybarForce,
		FVehicleSuspensionSimContext& Ctx,
		const FVector& InKnuckleWorldPos,
		const FVector& InAxleWorldDirection
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
	static FVector2D ComputeCircleIntersection(FVector2D A, float RA, float R0, bool ReturnX1 = true); //Abandoned //Compute the intersection of the circle on point A and the point on (0, 0)
	static FORCEINLINE FQuat4f MakeQuatFrom2DVectors(const FVector2f From, const FVector2f To, const FVector3f Axis)
	{
		FVector2f A = From.GetSafeNormal();
		FVector2f B = To.GetSafeNormal();

		float cosTheta = FVector2f::DotProduct(A, B);              // cosθ
		float sinTheta = A.X * B.Y - A.Y * B.X;                    // sinθ（2D 叉积）

		// Clamp for numerical stability
		cosTheta = FMath::Clamp(cosTheta, -1.0f, 1.0f);

		float cosHalf = FMath::Sqrt((1.0f + cosTheta) * 0.5f);    // cos(θ/2)
		float sinHalf = FMath::Sqrt((1.0f - cosTheta) * 0.5f);    // sin(θ/2)
		sinHalf *= FMath::Sign(sinTheta);                         // 保持方向

		FVector3f axisNorm = Axis.GetSafeNormal();
		return FQuat4f(
			axisNorm.X * sinHalf,
			axisNorm.Y * sinHalf,
			axisNorm.Z * sinHalf,
			cosHalf
		);
	}

	static FORCEINLINE FVector3f SuspensionPlaneToZYPlane(FVector2f V2D, float WheelYPosSign)
	{
		return FVector3f(0, V2D.Y * WheelYPosSign, V2D.X);
	}

	static FORCEINLINE FVector2f ZYPlaneToSuspensionPlane(FVector3f V3D, float WheelYPosSign)
	{
		return FVector2f(V3D.Z, V3D.Y * WheelYPosSign);
	}

	static FORCEINLINE FVector3f GetCamberToeCasterFromCurve(
		float CompressionRatio, 
		float WheelYPosSign, 
		FVehicleSuspensionKinematicsConfig& Config)
	{
		FVector3f v;
		v.X = 0.f;
		v.Y = Config.BaseToe;
		v.Z = Config.BaseCamber;

		if (Config.ToeCurve)v.Y += Config.ToeCurve->GetFloatValue(CompressionRatio);
		if (Config.CamberCurve)v.Z += Config.CamberCurve->GetFloatValue(CompressionRatio);

		// flip the caster and toe if necessary
		v *= WheelYPosSign;

		if (Config.CasterCurve) v.X = Config.CasterCurve->GetFloatValue(CompressionRatio);

		return v;
	}

	static FORCEINLINE void CopyContextToState(FVehicleSuspensionSimState& State, const FVehicleSuspensionSimContext& Context)
	{
		State.bIsRightWheel = Context.WheelPos >= 0.f;
		State.bHitGround = Context.bHitGround;
		State.SteeringAngle = Context.SteeringAngle;
		State.SuspensionCurrentLength = Context.SuspensionCurrentLength;
		State.ForceAlongImpactNormal = Context.ForceAlongImpactNormal;
		State.KnucklePos2D = FVector2f(Context.KnucklePos2D);
		State.WheelRightVector = FVector3f(Context.WheelRightVector);
		State.WheelCenterToKnuckle = FVector3f(Context.WheelCenterToKnuckle);
		State.KnuckleRelativePos = FVector3f(Context.KnuckleRelativePos);
		State.TopMountRelativePos = FVector3f(Context.TopMountRelativePos);
		State.StrutDirection = FVector3f(Context.StrutDirection);
		State.ImpactPointWorldVelocity = FVector3f(Context.ImpactPointWorldVelocity);
		State.ImpactNormal = FVector3f(Context.HitStruct.ImpactNormal);
		State.ImpactPoint = Context.HitStruct.ImpactPoint;
		State.WheelWorldPos = Context.WheelWorldPos;
		State.WheelRelativeRotation = FQuat4f(Context.WheelRelativeTransform.GetRotation());

		State.ImpactFriction = 1.f;
		State.ImpactComponent = nullptr;

		const FHitResult& Hit = Context.HitStruct;
		if (Hit.bBlockingHit)
		{
			UPhysicalMaterial* HitPhysMat = Hit.PhysMaterial.Get();
			if (IsValid(HitPhysMat))
			{
				State.ImpactFriction = HitPhysMat->Friction;
			}

			State.ImpactComponent = Hit.GetComponent();
		}
	}

	static FORCEINLINE void CopyStateToContext(FVehicleSuspensionSimContext& Context, const FVehicleSuspensionSimState& State)
	{
		Context.SuspensionCurrentLength = State.SuspensionCurrentLength;
		Context.CriticalDamping = State.CriticalDamping;
		Context.SprungMass = State.SprungMass;
		Context.KnucklePos2D = State.KnucklePos2D;
		Context.WheelCenterToKnuckle = State.WheelCenterToKnuckle;
		Context.WheelRelativeTransform.SetRotation(State.WheelRelativeRotation);
		Context.WheelRelativeTransform.SetLocation(State.WheelCenterToKnuckle + State.KnuckleRelativePos);
	}

	FVehicleSuspensionSimState State;

protected:
	UVehicleWheelComponent* TargetWheelComponent;

	FCollisionQueryParams QueryParams = FCollisionQueryParams::DefaultQueryParam;

private:

	//cache
	float CachedSpringStiffness = -1;
};
