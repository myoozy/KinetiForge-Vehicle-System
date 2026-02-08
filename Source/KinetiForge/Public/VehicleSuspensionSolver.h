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
		const FVector3f& TireForce,
		const FVehicleSuspensionKinematicsConfig& KineConfig,
		const FTransform& ComponentRelativeTransform,
		const FTransform& AsyncCarbodyWorldTransform,
		const UWorld* CurrentWorld,
		Chaos::FRigidBodyHandle_Internal* CarbodyHandle,
		float InSteeringAngle,
		FVector& OutApporximatedWheelWorldPos,
		FVehicleSuspensionSimContext& Ctx
	);
	void FinalizeUpdateSolidAxle(
		const float WheelRadius,
		const FVehicleSuspensionKinematicsConfig& KineConfig,
		const FVehicleSuspensionSpringConfig& SpringConfig,
		Chaos::FRigidBodyHandle_Internal* CarbodyHandle,
		float InDeltaTime,
		float InSwaybarForce,
		FVehicleSuspensionSimContext& Ctx,
		const FVector& InKnuckleWorldPos,
		const FVector& InAxleWorldDirection
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

	FORCEINLINE void CopyContextToState(const FVehicleSuspensionSimContext& Context)
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
		State.ImpactNormal = FVector3f(Context.HitStruct.Normal);
		State.ImpactPoint = Context.HitStruct.ImpactPoint;
		State.WheelWorldPos = Context.WheelWorldPos;
		State.WheelRelativeRotation = FQuat4f(Context.WheelRelativeTransform.GetRotation());
		State.AntiPitchScale = Context.AntiPitchScale;
		State.AntiRollScale = Context.AntiRollScale;
		State.Compression = 1.f - Context.SuspensionExtensionRatio;

		State.ImpactFriction = Context.ImpactFriction;
		
		// copy hitresult
		RayCastResult = FVehicleSuspensionHitResult(
			Context.HitStruct.PhysMaterial,
			Context.HitStruct.Component,
			Context.HitStruct.BoneName,
			Context.HitStruct.bBlockingHit,
			Context.HitStruct.TraceStart,
			Context.HitStruct.TraceEnd,
			Context.HitStruct.Location,
			(FQuat4f)Context.RayCastTransform.GetRotation()
		);
	}

	FORCEINLINE void CopyStateToContext(FVehicleSuspensionSimContext& Context)
	{
		Context.SuspensionCurrentLength = State.SuspensionCurrentLength;
		Context.SuspensionExtensionRatio = 1.f - State.Compression;
		Context.SprungMass = State.SprungMass;
		Context.KnucklePos2D = State.KnucklePos2D;
		Context.WheelCenterToKnuckle = State.WheelCenterToKnuckle;
		Context.WheelRelativeTransform.SetRotation(State.WheelRelativeRotation);
		Context.WheelRelativeTransform.SetLocation(State.WheelCenterToKnuckle + State.KnuckleRelativePos);
		Context.HitStruct.Normal = FVector(State.ImpactNormal);
		Context.HitStruct.ImpactPoint = State.ImpactPoint;
	}

	static FORCEINLINE FVector3f GetCamberToeCasterFromCurve(
		const FVehicleSuspensionCachedRichCurves& Curves,
		float CompressionRatio,
		float WheelYPosSign,
		float BaseCamber = 0.f,
		float BaseToe = 0.f,
		float BaseCaster = 0.f)
	{
		FVector3f v;
		v.X = BaseCaster;
		v.Y = BaseToe;
		v.Z = BaseCamber;

		v.Y += Curves.ToeCurve.Eval(CompressionRatio);
		v.Z += Curves.CamberCurve.Eval(CompressionRatio);

		// flip the caster and toe if necessary
		v *= WheelYPosSign;

		v.X += Curves.CasterCurve.Eval(CompressionRatio);

		return v;
	}

	static inline FQuat4f GetSpindleMountQuat(
		const FRotator3f& InSpindleMountRotationConfig,
		const float WheelPos)
	{
		return FQuat4f(FRotator3f(
			InSpindleMountRotationConfig.Pitch,
			InSpindleMountRotationConfig.Yaw * WheelPos,
			InSpindleMountRotationConfig.Roll * WheelPos
		));
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
