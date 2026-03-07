// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)


#include "VehicleADASComponent.h"
#include "VehicleUtilities.h"
#include "VehicleDrivetrainStructs.h"
#include "VehicleDriveAssemblyComponent.h"
#include "VehicleWheelComponent.h"
#include "VehicleDifferentialComponent.h"
#include "VehicleAxleAssemblyComponent.h"
#include "VehicleWheelCoordinatorComponent.h"
#include "VehicleEngineComponent.h"
#include "VehicleClutchComponent.h"
#include "VehicleGearboxComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

// Sets default values for this component's properties
UVehicleADASComponent::UVehicleADASComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVehicleADASComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	DriveAssemblyComponent = UVehicleUtilities::GetComponentByName<UVehicleDriveAssemblyComponent>(
		GetOwner(),
		DriveAssemblyName
	);

#if 0
	if (UWorld* World = GetWorld())
	{
		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.SetTimer(
			ADASTickTimerHandle,
			this,
			&UVehicleADASComponent::TickADAS,
			RefreshInterval,
			true,
			0.0f
		);
	}
#endif
}

void UVehicleADASComponent::TickADAS()
{
	const float DeltaTime = RefreshInterval;
}


// Called every frame
void UVehicleADASComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UVehicleADASComponent::SetDriveAssemblyComponent(UVehicleDriveAssemblyComponent* NewDriveAssembly)
{
	if (IsValid(NewDriveAssembly))
	{
		DriveAssemblyComponent = NewDriveAssembly;
	}
}

UVehicleDriveAssemblyComponent* UVehicleADASComponent::GetDriveAssemblyComponent()
{
	return DriveAssemblyComponent.Get();
}

float UVehicleADASComponent::GetMaxSpeedToBrake(float Distance, float Acceleration, float DeltaTime, float EndSpeed)
{
	float a_dt = Acceleration * DeltaTime;
	float RootTerm = a_dt * a_dt + (EndSpeed * EndSpeed) + (2.f * Acceleration * Distance);
	float MaxVelocity = -a_dt + FMath::Sqrt(FMath::Max(0.f, RootTerm));
	return FMath::Max(0.f, MaxVelocity);
}

float UVehicleADASComponent::GetMaxSpeedToTurn(const TArray<FVector>& PathPoints, float AccelLimit, float MinTurningRadius)
{
	if (PathPoints.Num() > 2)
	{
		// max speed to turn with min turn radius. unit: m/s
		const float Speed2TurnMin = FMath::Sqrt(FMath::Abs(MinTurningRadius * AccelLimit));

		// 通过转角算出外接圆半径
		const FVector A = (PathPoints[0] - PathPoints[1]) * 0.01f;
		const FVector B = (PathPoints[2] - PathPoints[1]) * 0.01f;
		const float a = A.Length();
		const float b = B.Length();
		if (a > SMALL_NUMBER || b > SMALL_NUMBER)
		{
			const float cos_theta = FMath::Clamp(FVector::DotProduct(A / a, B / b), -1.f, 1.f);
			const float sin_theta_inv = FMath::InvSqrt(1.f - cos_theta * cos_theta);
			const float denominator = FMath::Sqrt(a * a + b * b - 2.f * a * b * cos_theta);
			const float radius = denominator * 0.5f * sin_theta_inv;
			const float Speed2Turn = FMath::Max(FMath::Sqrt(AccelLimit * radius), Speed2TurnMin);
			return Speed2Turn;
		}
		else
		{
			return Speed2TurnMin;
		}
	}
	else
	{
		return BIG_NUMBER;
	}
}

void UVehicleADASComponent::UpdateCruiseControl(
	UVehicleDriveAssemblyComponent* DriveAssembly,
	float TargetSpeed, 
	float MaxAcceleration, 
	float DeltaTime,
	FVector2D ShiftInterval)
{
	if (!IsValid(DriveAssembly))return;
	UPrimitiveComponent* Carbody = DriveAssembly->GetCarbody();
	if (!IsValid(Carbody))return;

	const float CarMass = Carbody->GetMass();
	const float MaxDriveForce = GetVehicleMaxDriveForce(DriveAssembly);
	const float MaxBrakeForce = GetVehicleMaxBrakeForce(DriveAssembly);

	const float MaxDriveAccel = UVehicleUtilities::SafeDivide(MaxDriveForce, CarMass);
	const float MaxBrakeAccel = UVehicleUtilities::SafeDivide(MaxBrakeForce, CarMass);

	const float CurrSpeed = DriveAssembly->GetVehicleForwardSpeed();
	const float SpeedDiff = TargetSpeed - CurrSpeed;
	const float AccelLimit = FMath::Abs(UVehicleUtilities::SafeDivide(SpeedDiff, DeltaTime));

	const float AbsDemandAccel = (MaxAcceleration > SMALL_NUMBER) ? FMath::Min(MaxAcceleration, AccelLimit) : AccelLimit;
	const float DemandThrottleInput = UVehicleUtilities::SafeDivide(AbsDemandAccel, MaxDriveAccel);
	const float DemandBrakeInput = UVehicleUtilities::SafeDivide(AbsDemandAccel, MaxBrakeAccel);

	const bool bGoForward = TargetSpeed >= 0.f;
	const bool bShouldAccel = bGoForward ? SpeedDiff >= 0.f : SpeedDiff <= 0.f;

	if (bShouldAccel)
	{
		SpeedUpVehicle(
			DriveAssembly,
			DemandThrottleInput,
			DemandBrakeInput,
			DeltaTime,
			bGoForward,
			ShiftInterval
		);
	}
	else
	{
		SlowDownVehicle(DriveAssembly, DemandBrakeInput);
	}
}

float UVehicleADASComponent::GetVehicleMaxDriveForce(UVehicleDriveAssemblyComponent* DriveAssembly)
{
	if (!IsValid(DriveAssembly))return 0.f;
	UVehicleEngineComponent* Engine = DriveAssembly->GetEngine();
	UVehicleGearboxComponent* Gearbox = DriveAssembly->GetGearbox();
	UVehicleDifferentialComponent* TransferCase = DriveAssembly->GetTransferCase();
	if (!(IsValid(Engine) && IsValid(Gearbox) && IsValid(TransferCase)))return 0.f;

	const float TurboBoost = FMath::Max(0.f, Engine->TurboConfig.MaxBoostPressure * Engine->TurboConfig.BoostEfficiency);
	const float MaxEngineTorque = Engine->NAConfig.MaxEngineTorque * (1.f + TurboBoost);
	const float MaxDriveTorque = MaxEngineTorque * Gearbox->GetCurrentGearRatio();

	const float cm2m = 0.01f;
	const float EffectiveRadius = TransferCase->CalculateEffectiveWheelRadius(DriveAssembly->GetAxles()) * cm2m;

	const float DriveForce = UVehicleUtilities::SafeDivide(MaxDriveTorque, EffectiveRadius);
	return FMath::Abs(DriveForce);
}

float UVehicleADASComponent::GetVehicleMaxBrakeForce(UVehicleDriveAssemblyComponent* DriveAssembly)
{
	float BrakeForce = 0.f;

	if (IsValid(DriveAssembly))
	{
		for (UVehicleAxleAssemblyComponent* Axle : DriveAssembly->GetAxles())
		{
			if (IsValid(Axle))
			{
				UVehicleWheelComponent* WheelL;
				UVehicleWheelComponent* WheelR;
				Axle->GetWheels(WheelL, WheelR);
				const float cm2m = 0.01f;
				const float AvgRadius = 0.5f * cm2m * (WheelL->WheelConfig.Radius + WheelR->WheelConfig.Radius);

				const float MaxBrakeTorque = Axle->AxleConfig.MaxBrakeTorque * 2.f;

				BrakeForce += UVehicleUtilities::SafeDivide(MaxBrakeTorque, AvgRadius);
			}
		}
	}

	return BrakeForce;
}

void UVehicleADASComponent::SpeedUpVehicle(
	UVehicleDriveAssemblyComponent* DriveAssembly, 
	float MaxThrottle,
	float MaxBrake,
	float DeltaTime,
	bool bGoForward,
	FVector2D ShiftInterval)
{
	if (!IsValid(DriveAssembly))return;

	const bool bDirectInput = true;
	const float AutoShiftCoolDown = DeltaTime * 2.f;

	const float TargetDirection = bGoForward ? 1.f : -1.f;
	const int32 FirstGear = bGoForward ? 1 : -1;

	const float SpeedTolerance = 1.f; // m/s
	const bool bGoingInCorrectDirection = DriveAssembly->GetVehicleForwardSpeed() * TargetDirection > -SpeedTolerance;
	const bool bIsInCorrectGear = bGoForward ? DriveAssembly->GetCurrentGear() > 0 : DriveAssembly->GetCurrentGear() < 0;

	if (bGoingInCorrectDirection)
	{
		/*
		* Pause the automatic gearbox because we want to override the logic
		*/
		DriveAssembly->PauseAutoGearbox(AutoShiftCoolDown);

		/*
		* When arcade gearbox is true, means when going backwards, throttle and brake will switch.
		* Throttle input will be brake input and brake input will be throttle input.
		*/
		const bool bArcadeGearbox = DriveAssembly->AutoGearboxConfig.bArcadeAutoGearbox;
		const bool bAutoGearbox = DriveAssembly->AutoGearboxConfig.bAutomaticGearbox;
		const bool bIsInReverseGear = DriveAssembly->GetCurrentGear() < 0;
		const bool bSwitchThrottleAndBrake = bAutoGearbox && bArcadeGearbox && bIsInReverseGear;

		if (bIsInCorrectGear)
		{
			if (bSwitchThrottleAndBrake)
			{
				DriveAssembly->InputThrottle(0.f, bDirectInput);
				DriveAssembly->InputBrake(MaxThrottle, bDirectInput);
			}
			else
			{
				DriveAssembly->InputBrake(0.f, bDirectInput);
				DriveAssembly->InputThrottle(MaxThrottle, bDirectInput);
			}
		}
		else
		{
			DriveAssembly->ShiftToTargetGear(FirstGear, AutoShiftCoolDown);
		}

		/*
		* Simulate a very simple automatic gearbox.
		* This will override the native auto gearbox
		*/
		if (UVehicleEngineComponent* Engine = DriveAssembly->GetEngine())
		{
			const float NormaliedRPM = FMath::GetMappedRangeValueUnclamped(
				FVector2f(Engine->GetIdleRPM(), Engine->GetMaxRPM()),
				FVector2f(0.f, 1.f),
				Engine->GetRPM()
			);

			if (NormaliedRPM > ShiftInterval.Y)
			{
				// up shift
				if (bGoForward)
				{
					DriveAssembly->ShiftUp(AutoShiftCoolDown);
				}
				else
				{
					DriveAssembly->ShiftDown(AutoShiftCoolDown);
				}
			}
			else if (NormaliedRPM < ShiftInterval.X)
			{
				// down shift
				if (bGoForward)
				{
					if (DriveAssembly->GetCurrentGear() != 1)
					DriveAssembly->ShiftDown(AutoShiftCoolDown);
				}
				else
				{
					if (DriveAssembly->GetCurrentGear() != -1)
					DriveAssembly->ShiftUp(AutoShiftCoolDown);
				}
			}
		}
	}
	else
	{
		SlowDownVehicle(DriveAssembly, MaxBrake);
	}
}

void UVehicleADASComponent::SlowDownVehicle(
	UVehicleDriveAssemblyComponent* DriveAssembly,
	float MaxBrake,
	float Tolerance)
{
	if (!IsValid(DriveAssembly))return;

	if (FMath::Abs(DriveAssembly->GetVehicleForwardSpeed()) < Tolerance)
	{
		DriveAssembly->InputThrottle(0.f);
		DriveAssembly->InputBrake(0.f);
		return;
	}

	const bool bDirectInput = true;

	const bool bAutoGearbox = DriveAssembly->AutoGearboxConfig.bAutomaticGearbox;
	if (bAutoGearbox)
	{
		/*
		* When arcade gearbox is true, means when going backwards, throttle and brake will switch.
		* Throttle input will be brake input and brake input will be throttle input.
		*/
		const bool bArcadeGearbox = DriveAssembly->AutoGearboxConfig.bArcadeAutoGearbox;
		if (bArcadeGearbox)
		{
			FVehicleInputPipeline Input;
			DriveAssembly->GetInputValues(Input);
			const bool bSwitchThrottleAndBrake = Input.bSwitchThrottleAndBrake;
			if (bSwitchThrottleAndBrake)
			{
				DriveAssembly->InputThrottle(MaxBrake, bDirectInput);
				DriveAssembly->InputBrake(0.f, bDirectInput);
				return;
			}
		}
	}

	DriveAssembly->InputThrottle(0.f, bDirectInput);
	DriveAssembly->InputBrake(MaxBrake, bDirectInput);
}

void UVehicleADASComponent::UpdateDirectDrive(
	UVehicleDriveAssemblyComponent* DriveAssembly,
	FVector TargetLocation,
	float SpeedLimit,
	float AccelLimit,
	float EndSpeed,
	float Tolerance,
	float DeltaTime,
	FVector2D ShiftInterval)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_ADAS_DirectDriveTo);

	if (!IsValid(DriveAssembly)) return;

	SpeedLimit = FMath::Abs(SpeedLimit);
	AccelLimit = FMath::Abs(AccelLimit);

	float SteeringInput = 0.f;
	float CruiseControlSpeed = 0.f;
	const float cm2m = 0.01f;

	FVector BoundExtent = FVector(0.f);
	FVector BoundOrigin = FVector(0.f);
	FTransform VehicleTransform;
	DriveAssembly->GetVehicleBound(BoundExtent, BoundOrigin, VehicleTransform);

	// target location in local space
	const FVector3f TargetLocal = (FVector3f)VehicleTransform.InverseTransformPositionNoScale(TargetLocation);
	// target location in local space and 2d
	const FVector2f TargetLoc2D_m = FVector2f(TargetLocal.X * cm2m, TargetLocal.Y * cm2m);
	const float Dist2Target_m = TargetLoc2D_m.Length();

	// stop if is close enough
	if (Dist2Target_m < (Tolerance * cm2m))
	{
		DriveAssembly->InputSteering(0.f);
		UpdateCruiseControl(DriveAssembly, EndSpeed, AccelLimit, DeltaTime, ShiftInterval);
		return;
	}

	const float MinTurningRadius_m = DriveAssembly->GetMinTurningRadius();
	const float MaxSteeringAngle = DriveAssembly->GetMaxSteeringAngle();
	const float CurrForwardSpeed = DriveAssembly->GetVehicleForwardSpeed();

	const bool bIsTargetBehind = TargetLoc2D_m.X < 0.f;
	const FVector2f VirtualTarget_m = bIsTargetBehind ? FVector2f(-TargetLoc2D_m.X, -TargetLoc2D_m.Y) : TargetLoc2D_m;

	/* ========== Steering Control ========== */

	// 1. Calculate the steering input for the missile line-of-sight (LOS) method
	const float TargetAngleRad = FMath::Atan2(VirtualTarget_m.Y, VirtualTarget_m.X);
	const float TargetAngleDeg = FMath::RadiansToDegrees(TargetAngleRad);
	const float SteeringInputLOS = UVehicleUtilities::SafeDivide(TargetAngleDeg, MaxSteeringAngle);

	// 2. Calculate steering input for Pure Pursuit (Radius)
	float SteeringInputRadius = 0.f;
	float SignedRequiredRadius_m = MAX_flt;
	bool bTooCloseToTurn = false;
	if (FMath::Abs(VirtualTarget_m.Y) > 1.f)
	{
		SignedRequiredRadius_m = (VirtualTarget_m.SquaredLength()) / (2.f * VirtualTarget_m.Y);
		// Simply divide the minimum turning radius by the required radius.
		SteeringInputRadius = UVehicleUtilities::SafeDivide(MinTurningRadius_m, SignedRequiredRadius_m);
		bTooCloseToTurn = FMath::Abs(SignedRequiredRadius_m) < MinTurningRadius_m;
	}

	// 3. Hybrid Method: Take the absolute value of the larger number, but must retain the original sign!
	SteeringInput = FMath::Abs(SteeringInputLOS) > FMath::Abs(SteeringInputRadius) ? SteeringInputLOS : SteeringInputRadius;
	
	// 4. take account the lateral acceleration
	float MaxSteeringInput = 1.f;
	if (AccelLimit > SMALL_NUMBER)
	{
		const float MaxTurningRadius = CurrForwardSpeed * CurrForwardSpeed / AccelLimit;
		MaxSteeringInput = FMath::Abs(MinTurningRadius_m / MaxTurningRadius);
		MaxSteeringInput = FMath::Min(1.f, MaxSteeringInput);
	}
	SteeringInput = FMath::Clamp(SteeringInput, -MaxSteeringInput, MaxSteeringInput);

	/* ========== Speed Control ========== */

	if (AccelLimit > SMALL_NUMBER)
	{
		// Lateral deceleration: No manual parameters are introduced; geometric ratios are used directly. 
		// The greater the steering angle, the slower the speed.
		// Subtract (absolute steering angle / 90 degrees) from 1 to ensure deceleration is capped at 10%.
		float MaxLatV = SpeedLimit;
		if (FMath::Abs(TargetAngleDeg) > SMALL_NUMBER)
		{
			float ApproxSteeringRadius_m = FMath::Abs(MinTurningRadius_m * MaxSteeringAngle / TargetAngleDeg);
			MaxLatV = FMath::Sqrt(AccelLimit * ApproxSteeringRadius_m);
		}

		// Vertical Braking: Solving a quadratic equation purely through physics to perfectly counteract DeltaTime latency
		const float LookaheadMultiplier = 2.f; // maybe it is similar as Nyquist Sampling Theorem ?
		float MaxLongV = GetMaxSpeedToBrake(Dist2Target_m, AccelLimit, DeltaTime * LookaheadMultiplier, EndSpeed);

		CruiseControlSpeed = FMath::Min(FMath::Min(MaxLongV, MaxLatV), SpeedLimit);
	}

	if (bIsTargetBehind)
	{
		if (CruiseControlSpeed > 0.f)
			CruiseControlSpeed = -CruiseControlSpeed;
	}
	else
	{
		if (CurrForwardSpeed < -0.1f)
			SteeringInput = -SteeringInput;
	}

	// max speed to turn with min turn radius. unit: m/s
	const float Speed2TurnMin = FMath::Sqrt(FMath::Abs(MinTurningRadius_m * AccelLimit));

	const float CurrentSteering = DriveAssembly->GetSteeringValueFromAxles();
	if (CurrentSteering * SteeringInput < 0.f && CurrForwardSpeed * CruiseControlSpeed < 0.f)
		CruiseControlSpeed = 0.f; // if going back, brake first, then turn

	/*
	* apply steering and speed
	*/
	DriveAssembly->InputSteering(SteeringInput);
	UpdateCruiseControl(DriveAssembly, CruiseControlSpeed, AccelLimit, DeltaTime, ShiftInterval);
}

bool UVehicleADASComponent::GetPathPointsToTarget(
	UWorld* CurrentWorld,
	FVector StartLocation, 
	FVector TargetLocation, 
	TArray<FVector>& OutPathPoints,
	AActor* PathfindingContext)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_ADAS_GetPathPoints);

	OutPathPoints.Empty();

	if (!IsValid(CurrentWorld))return false;
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(CurrentWorld);
	if (!IsValid(NavSys)) return false;

	UNavigationPath* NavPath = NavSys->FindPathToLocationSynchronously(
		CurrentWorld,
		StartLocation,
		TargetLocation,
		PathfindingContext
	);

	if (IsValid(NavPath) && NavPath->IsValid() && NavPath->PathPoints.Num() > 1)
	{
		OutPathPoints = NavPath->PathPoints;
		return true;
	}

	return false;
}

float UVehicleADASComponent::GetRemainingPathLength(const TArray<FVector>& Path, FVector CurrLoc)
{
	if (Path.Num() == 0) return 0.f;
	if (Path.Num() == 1) return FVector::Dist(CurrLoc, Path[0]);

	// 1. Find the index of the point closest to the vehicle along the path
    // A more accurate approach would be to find the nearest “line segment,” 
	// but locating the “point” offers the best performance.
	int32 ClosestIdx = 0;
	float MinDistSq = MAX_flt;

	for (int32 i = 0; i < Path.Num(); ++i)
	{
		float DistSq = FVector::DistSquared(CurrLoc, Path[i]);
		if (DistSq < MinDistSq)
		{
			MinDistSq = DistSq;
			ClosestIdx = i;
		}
	}

	// 2. Cumulative Distance
	// First Segment: Distance from the vehicle's current position to the nearest point
	float TotalDist = FMath::Sqrt(MinDistSq);

	// Subsequent segment: 
	// Starting from the nearest point, accumulate point-to-point distances until reaching the endpoint.
	for (int32 i = ClosestIdx; i < Path.Num() - 1; ++i)
	{
		TotalDist += FVector::Dist(Path[i], Path[i + 1]);
	}

	return TotalDist;
}

TArray<FVector> UVehicleADASComponent::FixPathCollisions(
	UWorld* World,
	const TArray<FVector>& PathPoints,
	const TArray<AActor*>& IgnoredActors,
	float TraceRadius,
	int32 MaxBounces,
	int32 NumPointsToFix,
	ECollisionChannel TraceChannel)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_ADAS_FixPathCollisions);

	if (PathPoints.Num() < 2 || !IsValid(World) || NumPointsToFix == 0) return PathPoints;

	TArray<FVector> NewPath;
	NewPath.Add(PathPoints[0]);

	FCollisionQueryParams Params;
	Params.AddIgnoredActors(IgnoredActors);

	const FCollisionShape Sphere = FCollisionShape::MakeSphere(TraceRadius);
	const float OffsetDist = TraceRadius * 0.5f;
	FVector Offset = FVector(0.f, 0.f, OffsetDist);

	for (int32 i = 0; i < PathPoints.Num() - 1; ++i)
	{
		if (NumPointsToFix > 0 && i >= NumPointsToFix)
		{
			for (int32 j = i + 1; j < PathPoints.Num(); ++j)
			{
				NewPath.Add(PathPoints[j]);
			}
		}

		FVector Start = PathPoints[i];
		FVector End = PathPoints[i + 1];
		
		Start += Offset;
		End += Offset;

		int32 Bounces = 0;
		while (Bounces < MaxBounces)
		{
			FHitResult Hit;
			bool bHit = World->SweepSingleByChannel(
				Hit, Start, End, FQuat::Identity, TraceChannel, Sphere, Params
			);

			if (bHit)
			{
				Bounces++;
				const float Bias = 1.f;
				const FVector& N = Hit.Normal; // 必须使用球的Normal，而不是ImpactNormal
				/*
				* 如果路径点被阻挡，原因一定是nav mesh的代理半径太窄。
				* 把EscapePoint略微沿法向偏移一些些，防止浮点精度问题
				*/
				FVector EscapePoint = Hit.ImpactPoint + N * (TraceRadius + Bias);

				/*
				* 沿着切线步进一小段距离。
				* NavMesh导航点的位置会在墙角（外），最坏的情况是导航代理半径为0，
				* 所以把Hit.Location沿着切线移动一个检测半径的距离，让车与墙角保持距离。
				* 再往墙里移动一小段距离，检查是不是在墙角，如果在墙角就往墙角转弯。
				*/
				FVector StepAlongTangent;
				const FVector StepIntoWall = -N * (TraceRadius + Bias);
				FVector NextDirection;
				if (i + 2 < PathPoints.Num())
				{
					NextDirection = PathPoints[i + 2] - Hit.Location;
				}
				else
				{
					NextDirection = End - Start;
				}
				const FVector ApproxDirection = FVector::VectorPlaneProject(NextDirection, N).GetSafeNormal();
				const bool bDrivingIntoWall = ApproxDirection.SquaredLength() < 0.99f;
				if (bDrivingIntoWall)
				{
					// turn left or right randomly?
					const float Sign = FMath::RandBool() ? 1.f : -1.f;
					FVector Vertical2Normal = FVector::CrossProduct(Offset, N) * Sign;
					StepAlongTangent = Vertical2Normal.GetSafeNormal() * TraceRadius;
				}
				else
				{
					StepAlongTangent = ApproxDirection * TraceRadius;
				}

				// 把下一个目标点设为墙外的点
				FVector CurrentTarget = EscapePoint + StepAlongTangent;

				// 简单平滑一下z高度（直接取平均值，或者用什么办法插值）
				const float AvgZ = (PathPoints[i].Z + PathPoints[i + 1].Z) * 0.5f;

				// 然后再往墙里步进一小段，如果这里没有障碍物，就瞄准这里
				Start = EscapePoint + StepAlongTangent;
				Start.Z = AvgZ + OffsetDist;
				End = Start + StepIntoWall * 0.5f;

				// 再次射线检测，看看有没有障碍物，顺便确认是否有两面墙
				bHit = World->SweepSingleByChannel(
					Hit, Start, End, FQuat::Identity, TraceChannel, Sphere, Params
				);

				if (bHit)
				{
					// 如果发生碰撞，说明肯定不是墙角（外）
					CurrentTarget = Hit.Location + Hit.Normal * Bias;

					if (Hit.bStartPenetrating)
					{
						/*
						* 如果StartPenetrating，说明前方有另一面墙
						* 可能是墙角（内），也可能是一条很窄的缝隙
						* 我们需要检查是否是墙角（内）
						*/
						Start = EscapePoint;
						Start.Z = AvgZ + OffsetDist;
						End = Start - StepIntoWall * 0.5f;
						bHit = World->SweepSingleByChannel(
							Hit, Start, End, FQuat::Identity, TraceChannel, Sphere, Params
						);
						if (bHit)
						{
							if (Hit.bStartPenetrating)
							{
								// 如果一点空间也没有，那就当成死路
								return NewPath;
							}
							else
							{
								CurrentTarget = Hit.Location + Hit.Normal * Bias;
							}
						}
						else
						{
							CurrentTarget = End;
						}
					}
				}
				else
				{
					/*
					* 这里我不知道是选择跟下个目标点夹角更小的（更好转弯）？（暂定）
					* 还是距离更近的。
					* 如果选夹角更小的，会少很多莫名其妙的转向
					*/
					const FVector DirCurr2Next = (PathPoints[i + 1] - PathPoints[i]).GetSafeNormal();
					const FVector DirCurr2Start = (Start - Offset - PathPoints[i]).GetSafeNormal();
					const FVector DirCurr2End = (End - Offset - PathPoints[i]).GetSafeNormal();
					const float DotProduct_Start = FVector::DotProduct(DirCurr2Next, DirCurr2Start);
					const float DotProduct_End = FVector::DotProduct(DirCurr2Next, DirCurr2End);

					if (DotProduct_Start > DotProduct_End)
					{
						CurrentTarget = Start;
					}
					else
					{
						CurrentTarget = End;
					}
				}

				// 最后把目标添加到新路径，距离太近的不要
				CurrentTarget.Z = AvgZ;
				if (FVector::DistSquared(CurrentTarget, PathPoints[i]) > TraceRadius * TraceRadius)
				{
					NewPath.Add(CurrentTarget);
				}

				// 下次射线的位置
				Start = CurrentTarget + Offset;
				End = PathPoints[i + 1] + Offset;
			}
			else
			{
				// 中间没有障碍物，把点位添加到new path然后退出循环
				bool bEnoughDist = FVector::DistSquared(PathPoints[i + 1], PathPoints[i]) > TraceRadius * TraceRadius;
				if (bEnoughDist || i + 2 == PathPoints.Num())
				{
					NewPath.Add(PathPoints[i + 1]);
				}
				break;
			}
		}
	}

	return NewPath;
}

bool UVehicleADASComponent::GetBestTargetInFOV(
	FVector& OutBestTarget,
	float& OutBestScore,
	float& OutOptimalSpeedLimit,
	UVehicleDriveAssemblyComponent* DriveAssembly, 
	FVector TargetLocation,
	float DeltaTime,
	float AccelLimit,
	float MaxFOV,
	float MinFOV,
	float GoalWeight,
	int32 MaxNumOfRays,
	ECollisionChannel TraceChannel,
	bool bDebugDraw)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_ADAS_GetBestTargetInFOV);

	if (!IsValid(DriveAssembly))return false;
	UWorld* World = DriveAssembly->GetWorld();
	if (!IsValid(World))return false;

	bool bWayBlocked = false;

	// 处理一下输入
	AccelLimit = FMath::Abs(AccelLimit);
	OutOptimalSpeedLimit = BIG_NUMBER;

	// 速度
	const float Speed = DriveAssembly->GetVehicleForwardSpeed(); // Unit: m/s
	const float AbsSpeed = FMath::Abs(Speed);

	// 包围盒
	FVector BoundExtent = FVector(0.f);
	FVector BoundOrigin = FVector(0.f);
	FTransform VehicleTransform;
	DriveAssembly->GetVehicleBound(BoundExtent, BoundOrigin, VehicleTransform);
	
	const FVector CurrentLocation = VehicleTransform.GetLocation();
	const FVector CompUp = VehicleTransform.GetRotation().GetUpVector();
	const FVector CompRight = VehicleTransform.GetRotation().GetRightVector();
	const FVector CompForward = VehicleTransform.GetRotation().GetForwardVector();

	const FVector Velocity = DriveAssembly->GetWorldLinearVelocity(); // 车辆相对于地面的速度
	const FVector Right = CompRight;

	const FVector Cross_V_R = FVector::CrossProduct(Velocity, Right);
	const float Dot_CrossVR_Up = FVector::DotProduct(Cross_V_R, CompUp);
	const FVector Up = FMath::Lerp(
		CompUp,
		Dot_CrossVR_Up >= 0.f ? Cross_V_R : -Cross_V_R, // 必须保证与组件的上向量方向相同
		FMath::Clamp(AbsSpeed, 0.f, 1.f)
	).GetSafeNormal(); // 用右向量和速度叉乘得到上向量，避免加减速时车辆俯仰影响射线方向

	const FVector Forward = FVector::CrossProduct(Right, Up); //重新叉乘出前向量

	// 机械结构
	const float MaxSteeringAngle = DriveAssembly->GetMaxSteeringAngle();
	const float VehicleHalfLength = BoundExtent.X; // unit: cm
	const float VehicleHalfWidth = BoundExtent.Y; // unit: cm
	const float MinTurnRadius = DriveAssembly->GetMinTurningRadius(); // Unit: m
	const float SweptRadius = FMath::Max(1.f, VehicleHalfWidth);// UE球形检测最小的半径和最小的距离好像是1cm

	TArray<UPrimitiveComponent*> CompsToIgnore = DriveAssembly->GetRayCastHitComponents();
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(DriveAssembly->GetOwner());
	Params.AddIgnoredComponents(CompsToIgnore);

	float DistToTarget = FVector::Dist(CurrentLocation, TargetLocation);

	if (DistToTarget < MinTurnRadius * 200.f)
	{
		// ==========================================
		// 直达视线检测 (Line of Sight Check)
		// ==========================================
		FHitResult LOSHit;
		const float TraceHeightOffset = 0.5f;
		float LOS_SweptRadius = MinTurnRadius * 100.f;
		FVector TraceStart = CurrentLocation + Up * LOS_SweptRadius * TraceHeightOffset;
		FVector TraceEnd = TargetLocation + Up * LOS_SweptRadius * TraceHeightOffset;

		bool bLOSBlocked = World->SweepSingleByChannel(
			LOSHit, TraceStart, TraceEnd, FQuat::Identity, TraceChannel,
			FCollisionShape::MakeSphere(LOS_SweptRadius), Params
		);

		if (bDebugDraw)
		{
			if (bLOSBlocked)
				DrawDebugSphere(World, LOSHit.Location, LOS_SweptRadius, 10,
					FColor::Red, false, DeltaTime, 0, 5.f);
			UVehicleUtilities::DrawDebugCapsuleEasy(
				World, TraceStart, TraceEnd, LOS_SweptRadius, 5.f, DeltaTime, FColor::Green
			);
		}

		// 如果根本没撞到东西
		if (!bLOSBlocked)
		{
			OutBestTarget = TargetLocation;
			OutBestScore = 1.f;
			return true;
		}
	}

	// ==========================================
	// 动态探测距离
	// ==========================================
	const float LookaheadTime = DeltaTime * 2.f; // 奈奎斯特采样定理？
	const float TurnRadius = UVehicleUtilities::SafeDivide(Speed * Speed, AccelLimit, BIG_NUMBER); // min turn radius at current speed
	const float BrakeDist = UVehicleUtilities::SafeDivide(Speed * Speed, 2.f * AccelLimit); // unit: m
	const float LookaheadDist = AbsSpeed * LookaheadTime * 100.f; // unit: cm

	// 安全距离 unit: cm
	const float MinSafeDist = VehicleHalfLength + 1.f;
	const float SafeDist = MinSafeDist + LookaheadDist + 1.f;

	float LookDist = SafeDist * 2.f + FMath::Max(BrakeDist, FMath::Max(MinTurnRadius, TurnRadius)) * 100.f + 1.f; // unit: cm

	// ==========================================
	// 探测距离截断 (Distance Clamping)
	// ==========================================
	float MaxNeededLookDist = DistToTarget + 100.f;
	LookDist = FMath::Min(LookDist, MaxNeededLookDist);

	// ==========================================
	// 动态视野 (FOV) 与 动态射线计算
	// ==========================================
	float FOV_deg_signed = MaxFOV;

	const float Speed2TurnMin = FMath::Sqrt(FMath::Abs(MinTurnRadius * AccelLimit)); // max speed to turn with min turn radius. unit: m/s
	
	// 如果车速超过以最小转弯半径转弯的最大速度，则收紧FOV
	if (AbsSpeed > Speed2TurnMin)
	{
		float Ratio = FMath::Clamp(MinTurnRadius / TurnRadius, 0.1f, 1.0f);
		FOV_deg_signed = FMath::Lerp(MinFOV, MaxFOV, Ratio);
	}

	float FOV_deg = FMath::Abs(FOV_deg_signed);
	bool bLookBehind = FOV_deg_signed < 0.f; // 判断是否往后看
	bool bIs360 = FMath::IsNearlyEqual(FOV_deg, 360.f, 1.f); // 判断当前是否是全向 360 度雷达 (容差 1 度)

	// 计算弧长
	const float FOV_rad = FMath::DegreesToRadians(FOV_deg);
	const float ArcLength = FOV_rad * LookDist; // unit: cm

	// 射线尽量密集
	int32 NumRays = FMath::CeilToInt32(ArcLength * 2.f / SweptRadius);
	NumRays = FMath::Max(1, FMath::Min(NumRays, MaxNumOfRays));
	float AngleStep_rad = 0.f;

	if (bIs360)
	{
		// 如果是360度，设为偶数，这样左右公平
		NumRays = (NumRays % 2 != 0) ? NumRays - 1 : NumRays;
		NumRays = FMath::Max(2, NumRays);

		AngleStep_rad = FOV_rad / NumRays;
	}
	else
	{
		// 如果不是360度，把NumRays设为奇数，左右公平
		NumRays = (NumRays % 2 != 0) ? NumRays : NumRays - 1;
		NumRays = FMath::Max(1, NumRays);

		AngleStep_rad = FOV_rad / FMath::Max(1, NumRays - 1);
	}

	// 提前算好中心射线的“空间基准索引”
	int32 CenterSpatialIndex = NumRays / 2;
	FVector CenterRayDir = bLookBehind ? -Forward : Forward;

	TArray<float> RawScores;
	RawScores.Init(0.f, NumRays);
	TArray<float> HitDists;
	HitDists.Init(0.f, NumRays);
	TArray<FVector> RayDirs;
	RayDirs.Init(FVector::ZeroVector, NumRays);

	for (int32 i = 0; i < NumRays; ++i)
	{
		// 计算扩散偏移量: 0, 1, 1, 2, 2, 3, 3...
		int32 Offset = (i + 1) / 2;
		// 计算左右摇摆符号: 1, -1, 1, -1...
		int32 Sign = (i % 2 == 0) ? 1 : -1;

		// 算出这根射线在物理空间里“从左到右”的绝对索引！
		int32 SpatialIndex = CenterSpatialIndex + (Sign * Offset);

		// 如果传了什么奇怪的参数导致算错，这句能保底不宕机
		if (!RawScores.IsValidIndex(SpatialIndex)) continue;

		// 3. 根据绝对的 SpatialIndex，算出完美的线性物理角度
		// 这样不管 i 是什么执行顺序，算出来的角度永远是规规矩矩从左扫到右的！
		float LinearAngleRad = -FOV_rad * 0.5f + (SpatialIndex * AngleStep_rad);

		FVector RayDir = CenterRayDir.RotateAngleAxisRad(LinearAngleRad, Up);

		FVector ToTargetDir = (TargetLocation - CurrentLocation).GetSafeNormal();

		const float Scale = 0.5f * GoalWeight;
		const float Bias = 1.f - Scale;
		// BaseScore范围：最小值(1.0 - GoalWeight)，最大值1.0
		float BaseScore = FVector::DotProduct(RayDir, ToTargetDir) * Scale + Bias;

		float DangerMultiplier = 1.0f;

		// 射线检测
		FHitResult Hit;
		FVector TraceStart = CurrentLocation;
		FVector TraceEnd = TraceStart + RayDir * LookDist;

		bool bHit = World->SweepSingleByChannel(
			Hit, TraceStart, TraceEnd, FQuat::Identity, TraceChannel,
			FCollisionShape::MakeSphere(SweptRadius), Params
		);

		if (bHit)
		{
			float SafeBuffer = Hit.Distance - SafeDist;

			if (SafeBuffer > 0.f)
			{
				float SafeBufferRatio = SafeBuffer / (LookDist - SafeDist);
				DangerMultiplier = FMath::Clamp(SafeBufferRatio, 0.f, 1.f);
			}
			else
			{
				DangerMultiplier = 0.f;

				if (i == 0)
				{
					float Dist2Wall = Hit.Distance * 0.01f; // unit: m
					float Speed2Brake = GetMaxSpeedToBrake(Dist2Wall, AccelLimit, LookaheadTime, 0.f);
					OutOptimalSpeedLimit = FMath::Min(OutOptimalSpeedLimit, Speed2Brake);

					if (Hit.Distance < MinSafeDist)bWayBlocked = true;
				}
			}
		}

		RayDirs[SpatialIndex] = RayDir;
		HitDists[SpatialIndex] = bHit ? Hit.Distance : LookDist;
		RawScores[SpatialIndex] = BaseScore * DangerMultiplier;
	}

	// ==========================================
	// 第二步：执行高斯加权卷积 (支持 360 度首尾相接)
	// ==========================================
	TArray<float> SmoothScores;
	SmoothScores.Init(0.f, NumRays);

	// 设定平滑核的范围：比如我们要参考左右各 2 根射线
	int32 SmoothingRadius = (NumRays - 1) / 2;

	for (int32 i = 0; i < NumRays; ++i)
	{
		float SmoothedScore = 0.f;
		float TotalWeight = 0.f;


		// 遍历它附近的邻居 (从 i - SmoothingRadius 到 i + SmoothingRadius)
		for (int32 j = -SmoothingRadius; j <= SmoothingRadius; ++j)
		{
			int32 NeighborIdx = i + j;

			// 1. 处理边界：是否需要环形相接
			if (bIs360)
			{
				// 魔法取模运算：保证索引在 0 到 NumRays-1 之间循环
				NeighborIdx = (NeighborIdx % NumRays + NumRays) % NumRays;
			}
			else
			{
				// 不是 360 度，越界的射线直接丢弃，不作为参考
				if (NeighborIdx < 0 || NeighborIdx >= NumRays) continue;
			}

			// 2. 根据距离计算权重 (核心：越远权重越小)
			float Distance = (float)j * AngleStep_rad / FMath::Max(SMALL_NUMBER, FOV_rad * 0.5f);
			Distance = FMath::Clamp(FMath::Abs(Distance), 0.f, 1.f);
			float Weight = FMath::Lerp(1.f, 0.f, Distance * Distance);

			// 3. 累加分数和权重
			SmoothedScore += RawScores[NeighborIdx] * Weight;
			TotalWeight += Weight;
		}

		if (TotalWeight > SMALL_NUMBER)
		{
			SmoothedScore /= TotalWeight;
		}
		SmoothScores[i] = SmoothedScore;

		if (bDebugDraw)
		{
			FVector TraceStart = CurrentLocation;
			FVector TraceEnd = TraceStart + RayDirs[i] * LookDist;
			FVector HitLocation = TraceStart + RayDirs[i] * HitDists[i];

			FColor ScoreColor = FMath::Lerp(FLinearColor::Red, FLinearColor::Green, SmoothScores[i]).ToFColor(true);

			DrawDebugSphere(World, HitLocation, SweptRadius, 8,
				ScoreColor, false, DeltaTime, 0, 3.f);

			UVehicleUtilities::DrawDebugCapsuleEasy(
				World, TraceStart, TraceEnd, SweptRadius, 1.f, DeltaTime, FColor::White
			);
		}
	}

	float GlobalMaxScore = -MAX_flt;
	int32 BestIdx1 = 0;   // 记录最高点在哪个区间
	float BestT_Global = 0.f; // 记录最高点在区间的百分比

	for (int32 i = 0; i < NumRays - (bIs360 ? 0 : 1); ++i)
	{
		// 获取 4 个关键帧索引，处理 360 度的环形越界
		int32 idx0 = (i - 1 + NumRays) % NumRays;
		int32 idx1 = i;
		int32 idx2 = (i + 1) % NumRays;
		int32 idx3 = (i + 2) % NumRays;

		// 如果不是 360 度，边缘的切线可以用重复的端点来代替
		if (!bIs360)
		{
			if (i == 0) idx0 = 0;
			if (i == NumRays - 2) idx3 = NumRays - 1;
		}

		float S0 = SmoothScores[idx0];
		float S1 = SmoothScores[idx1];
		float S2 = SmoothScores[idx2];
		float S3 = SmoothScores[idx3];

		float LocalMaxScore;
		float BestT;
		UVehicleUtilities::FindCatmullRomLocalMax(S0, S1, S2, S3, LocalMaxScore, BestT);

		if (LocalMaxScore > GlobalMaxScore)
		{
			GlobalMaxScore = LocalMaxScore;
			BestIdx1 = idx1;
			BestT_Global = BestT;
		}
	}

	int32 TargetIdx1 = BestIdx1;
	int32 TargetIdx2 = (BestIdx1 + 1) % NumRays;

	// 1. 方向做混合
	FVector Dir1 = RayDirs[TargetIdx1];
	FVector Dir2 = RayDirs[TargetIdx2];
	FVector BestDirection = FMath::Lerp(Dir1, Dir2, BestT_Global).GetSafeNormal();

	// 2. 距离做混合
	float Dist1 = HitDists[TargetIdx1];
	float Dist2 = HitDists[TargetIdx2];
	float FinalDist = FMath::Lerp(Dist1, Dist2, BestT_Global);

	// 3. 重建坐标 (起点 + 最佳方向 * 最佳距离)
	FVector FinalTargetLocation = CurrentLocation + BestDirection * FinalDist;

	OutBestTarget = FinalTargetLocation;
	OutBestScore = GlobalMaxScore;
	return !bWayBlocked;
}

void UVehicleADASComponent::UpdateAutoPilotSimple(
	UVehicleDriveAssemblyComponent* DriveAssembly, 
	FVector TargetLocation, 
	float SpeedLimit, 
	float AccelLimit, 
	float EndSpeed, 
	float Tolerance, 
	float DeltaTime,
	float MaxFieldOfView,
	float MinFieldOfView,
	float GoalWeight,
	float StandstillThreshold,
	int32 MaxNumOfRays,
	FVector2D ShiftInterval,
	ECollisionChannel TraceChannel,
	bool bDebugDraw)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_ADAS_UpdateAutoPilotSimple);

	if (!IsValid(DriveAssembly))return;

	SpeedLimit = FMath::Abs(SpeedLimit);
	AccelLimit = FMath::Abs(AccelLimit);

	// 包围盒
	FVector BoundExtent = FVector(0.f);
	FVector BoundOrigin = FVector(0.f);
	FTransform VehicleTransform;
	DriveAssembly->GetVehicleBound(BoundExtent, BoundOrigin, VehicleTransform);

	const FVector CurrentLocation = VehicleTransform.GetLocation();
	const float Dist2Target = FVector::Dist(CurrentLocation, TargetLocation);

	if (Dist2Target < Tolerance)
	{
		UpdateCruiseControl(DriveAssembly, 0.f, AccelLimit, DeltaTime, ShiftInterval);
		DriveAssembly->InputSteering(0.f);
		return;
	}

	float Speed2BrakeAtTarget = GetMaxSpeedToBrake(Dist2Target, AccelLimit, DeltaTime, EndSpeed);
	SpeedLimit = FMath::Min(SpeedLimit, Speed2BrakeAtTarget);

	const float CurrentSpeed = DriveAssembly->GetVehicleForwardSpeed(); // unit: m/s
	const float MinTurnRadius = DriveAssembly->GetMinTurningRadius(); // Unit: m
	const float Speed2TurnMin = FMath::Sqrt(FMath::Abs(MinTurnRadius * AccelLimit)); // max speed to turn with min turn radius. unit: m/s
	
	const bool bReversing = CurrentSpeed < 0.1f;
	if (bReversing)SpeedLimit = FMath::Min(SpeedLimit, Speed2TurnMin);

	const bool bDetectFront = true; // always look at front
	const bool bDetectRear = CurrentSpeed < StandstillThreshold;
	// int32 NumRaysEachDir = FMath::Abs(MaxNumOfRays) / (1 + (int32)bDetectRear);

	float MaxFOV_abs = FMath::Abs(MaxFieldOfView);
	float MinFOV_abs = FMath::Abs(MinFieldOfView);

	// 默认保持不动（安全）
	FVector CurrentTarget = CurrentLocation;

	FVector TargetFront = CurrentTarget;
	FVector TargetBack = CurrentTarget;
	float ScoreFront = -MAX_flt;
	float ScoreBack = -MAX_flt;
	float SpeedLimitFront = SpeedLimit;
	float SpeedLimitBack = SpeedLimit;
	bool bFree2GoFront = true;
	bool bFree2GoBack = true;

	if (bDetectFront)
	{
		bFree2GoFront = GetBestTargetInFOV(
			TargetFront, ScoreFront, SpeedLimitFront, DriveAssembly,
			TargetLocation, DeltaTime, AccelLimit, MaxFOV_abs,
			MinFOV_abs, GoalWeight, MaxNumOfRays, TraceChannel, bDebugDraw
		);
	}

	if (bDetectRear)
	{
		bFree2GoBack = GetBestTargetInFOV(
			TargetBack, ScoreBack, SpeedLimitBack, DriveAssembly,
			TargetLocation, DeltaTime, AccelLimit, -MaxFOV_abs,
			-MinFOV_abs, GoalWeight, MaxNumOfRays, TraceChannel, bDebugDraw
		);
	}

	if ((int32)bFree2GoFront + (int32)bFree2GoBack == 1)
	{
		// if only one way is blocked

		if (bFree2GoBack)
		{
			// 车头被挡住
			CurrentTarget = TargetBack;
			SpeedLimit = FMath::Min(SpeedLimit, SpeedLimitBack);
		}
		if (bFree2GoFront)
		{
			// 车尾被挡住
			CurrentTarget = TargetFront;
			SpeedLimit = FMath::Min(SpeedLimit, SpeedLimitFront);
		}
	}
	else
	{
		if (ScoreFront >= ScoreBack)
		{
			CurrentTarget = TargetFront;
			SpeedLimit = FMath::Min(SpeedLimit, SpeedLimitFront);
		}
		else
		{
			CurrentTarget = TargetBack;
			SpeedLimit = FMath::Min(SpeedLimit, SpeedLimitBack);
		}
	}

	bool bBrakeHard = !bFree2GoFront || !bFree2GoBack;
	if (bBrakeHard)
	{
		EndSpeed = 0.f;
		float Accel2Stop = FMath::Abs(CurrentSpeed) / FMath::Max(SMALL_NUMBER, DeltaTime);
		AccelLimit = FMath::Max(AccelLimit, Accel2Stop);
	}

	UpdateDirectDrive(
		DriveAssembly, CurrentTarget, SpeedLimit, AccelLimit,	
		EndSpeed, Tolerance, DeltaTime,	ShiftInterval
	);

	if (bDebugDraw)
	{
		if (UWorld* World = DriveAssembly->GetWorld())
			DrawDebugSphere(World, CurrentTarget, 100.f, 10,
				FColor::Cyan, false, DeltaTime, 0, 5.f);
	}
}

void UVehicleADASComponent::UpdateAutoPilotNavMesh(
	UVehicleDriveAssemblyComponent* DriveAssembly,
	FVector TargetLocation,
	float SpeedLimit,
	float AccelLimit,
	float EndSpeed,
	float Tolerance,
	float DeltaTime,
	int32 LookaheadSteps,
	FVector2D ShiftInterval,
	bool bDebugDraw)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_ADAS_UpdateAutoPilotNavMesh);

	if (!IsValid(DriveAssembly))return;

	UWorld* World = DriveAssembly->GetWorld();
	if (!IsValid(World)) return;

	// 包围盒
	FVector BoundExtent = FVector(0.f);
	FVector BoundOrigin = FVector(0.f);
	FTransform VehicleTransform;
	DriveAssembly->GetVehicleBound(BoundExtent, BoundOrigin, VehicleTransform);

	// 一些前置准备
	SpeedLimit = FMath::Abs(SpeedLimit);
	AccelLimit = FMath::Abs(AccelLimit);
	float DynSpeedLimit = SpeedLimit;
	float DynAccelLimit = AccelLimit;
	float DynEndSpeed = SpeedLimit;
	FVector CurrLocation = VehicleTransform.GetLocation();
	const float CurrSpeed = DriveAssembly->GetVehicleForwardSpeed();
	const FVector ForwardVector = VehicleTransform.GetRotation().GetForwardVector();
	const FVector RightVector = VehicleTransform.GetRotation().GetRightVector();
	const float MinTurningRadius = DriveAssembly->GetMinTurningRadius();
	const float MaxSpeedToTurn = FMath::Sqrt(FMath::Abs(MinTurningRadius * AccelLimit));
	const float TraceRadius = MinTurningRadius * 50.f;
	TArray<AActor*> ActorsToIgnore = DriveAssembly->GetRayCastHitActors();
	ActorsToIgnore.Add(DriveAssembly->GetOwner());
	ECollisionChannel TraceChannel = ECollisionChannel::ECC_WorldDynamic;

	TArray<FVector> RawPath;
	GetPathPointsToTarget(
		World,
		CurrLocation,
		TargetLocation,
		RawPath
	);

	if (RawPath.Num() < 3)
	{
		// 假如路径点太少或者没有路径点，就把当前位置和目标位置添加进去
		RawPath.Reset();
		RawPath.Add(CurrLocation);
		RawPath.Add(TargetLocation);
	}

	TArray<FVector> FixedPath = FixPathCollisions(
		World,
		RawPath,
		ActorsToIgnore,
		TraceRadius,
		1,
		LookaheadSteps,
		TraceChannel
	);

	/*
	* 看向目标或者最近的导航点
	*/
	FVector CurrentTarget = CurrLocation + ForwardVector * (Tolerance + 1.f);
	if (FixedPath.Num() > 1)
	{
		CurrentTarget = FixedPath[1];
	}

	// 如果有至少三个点，算出安全的转弯速度
	if (FixedPath.Num() > 2)
	{
		// 通过转角算出外接圆半径
		const FVector A = (FixedPath[0] - FixedPath[1]) * 0.01f;
		const FVector B = (FixedPath[2] - FixedPath[1]) * 0.01f;
		const float a = A.Length();
		const float b = B.Length();
		if (a > SMALL_NUMBER || b > SMALL_NUMBER)
		{
			const float cos_theta = FMath::Clamp(FVector::DotProduct(A / a, B / b), -1.f, 1.f);
			const float sin_theta_inv = FMath::InvSqrt(1.f - cos_theta * cos_theta);
			const float denominator = FMath::Sqrt(a * a + b * b - 2.f * a * b * cos_theta);
			const float radius = denominator * 0.5f * sin_theta_inv;
			// 算出安全的速度，如果速度太小的话，反正机械结构限制导致转不过来，直接取沿最小转弯半径的速度
			const float Speed2Turn = FMath::Max(FMath::Sqrt(AccelLimit * radius), MaxSpeedToTurn);
			DynSpeedLimit = FMath::Min(Speed2Turn, DynSpeedLimit);
		}
	}

	// 算出沿直线行驶的安全速度
	const float PathDist_m = GetRemainingPathLength(FixedPath, CurrLocation) * 0.01f;
	const float LookaheadMultiplier = 2.f * 2.f; // maybe it is similar as Nyquist Sampling Theorem ?
	const float LookaheadTime = DeltaTime * LookaheadMultiplier;
	float Speed2Brake = GetMaxSpeedToBrake(PathDist_m, AccelLimit, LookaheadTime, EndSpeed);
	DynEndSpeed = FMath::Min(DynEndSpeed, Speed2Brake);
	DynSpeedLimit = FMath::Min(DynSpeedLimit, Speed2Brake);

	if (CurrentTarget == TargetLocation || CurrentTarget == FixedPath.Last())
	{
		DynEndSpeed = FMath::Min(DynEndSpeed, EndSpeed);
	}

	bool bShouldSlowDown = false;
	bool bIsPathBlocked = FixedPath.Last() != RawPath.Last() || FixedPath.Num() < 2;
	if (bIsPathBlocked)
	{

	}
	else
	{
		// 如果第二个点（下一个点位）被修复了，说明会撞墙，得减速
		bShouldSlowDown = FixedPath[1] != RawPath[1];
	}

	/*
	* 是否要强制刹车？
	* 前后同时射线检测
	*/
	FHitResult BrakeDetectFront;
	FHitResult BrakeDetectRear;
	const float BrakeDist = UVehicleUtilities::SafeDivide(CurrSpeed * CurrSpeed, 2.f * AccelLimit);
	const float DistOffset = MinTurningRadius * 50.f;
	const float BrakeDetectLength = BrakeDist * 100.f + DistOffset;
	const float BrakeDetectRadius = (MinTurningRadius * 0.5f / FMath::Max(1.f, BrakeDist)) * 100.f;
	const FVector Offset = FVector(0.f, 0.f, TraceRadius * 0.5f);
	FCollisionQueryParams Params;
	Params.AddIgnoredActors(ActorsToIgnore);

	const bool bShouldDetectFront = CurrSpeed > -AccelLimit * DeltaTime;
	const bool bShouldDetectRear = CurrSpeed < AccelLimit * DeltaTime;

	if (bShouldDetectFront)
	{
		FVector Start = CurrLocation + ForwardVector * DistOffset + Offset;
		FVector End = Start + ForwardVector * BrakeDetectLength;
		bool bHit = World->SweepSingleByChannel(
			BrakeDetectFront, Start, End, FQuat::Identity,
			TraceChannel, FCollisionShape::MakeSphere(BrakeDetectRadius), Params
		);
		if (bHit)
		{
			bShouldSlowDown = true;
			float MaxV = GetMaxSpeedToBrake(BrakeDetectFront.Distance, AccelLimit, LookaheadTime, 0.f);
			DynSpeedLimit = FMath::Min(DynSpeedLimit, FMath::Max(MaxSpeedToTurn, MaxV));
		}
	}

	if (bShouldDetectRear)
	{
		FVector Start = CurrLocation - ForwardVector * DistOffset + Offset;
		FVector End = Start - ForwardVector * BrakeDetectLength;
		bool bHit = World->SweepSingleByChannel(
			BrakeDetectRear, Start, End, FQuat::Identity,
			TraceChannel, FCollisionShape::MakeSphere(BrakeDetectRadius), Params
		);

		if (bHit)
		{
			bShouldSlowDown = true;
			// 如果后面有障碍物，同时如果前面没有障碍物，必须往前移动
			if (!BrakeDetectFront.bBlockingHit)
			{
				const FVector Curr2Target = CurrentTarget - CurrLocation;
				const bool bIsTargetBehind = FVector::DotProduct(Curr2Target, ForwardVector) < 0.f;
				if (bIsTargetBehind)
				{
					CurrentTarget = CurrentTarget + ForwardVector * MinTurningRadius * 100.f;
				}
			}
		}
	}

	if (bShouldSlowDown)
	{
		// 减速确保转向能力
		DynEndSpeed = FMath::Min(DynEndSpeed, MaxSpeedToTurn);
	}

	UpdateDirectDrive(
		DriveAssembly,
		CurrentTarget,
		DynSpeedLimit,
		DynAccelLimit,
		DynEndSpeed,
		Tolerance,
		DeltaTime,
		ShiftInterval
	);

	if (bDebugDraw)
	{
		const float radius = TraceRadius;
		const float thickness = 5.f;

		if (bIsPathBlocked)
		DrawDebugSphere(World, TargetLocation, radius, 12,
			FColor::Red, false, DeltaTime, 0, thickness);

		DrawDebugSphere(World, CurrentTarget, radius, 10,
			FColor::Cyan, false, DeltaTime, 0, thickness);

		if (BrakeDetectFront.bBlockingHit)
			DrawDebugSphere(World, BrakeDetectFront.Location, BrakeDetectRadius, 8,
				FColor::Orange, false, DeltaTime, 0, thickness);

		if (BrakeDetectRear.bBlockingHit)
			DrawDebugSphere(World, BrakeDetectRear.Location, BrakeDetectRadius, 8,
				FColor::Purple, false, DeltaTime, 0, thickness);

		for (FVector Point : FixedPath)
		{
			FColor color = FColor::Blue;
			DrawDebugSphere(World, Point, radius, 8,
				color, false, DeltaTime, 0, thickness);
		}

		for (FVector Point : RawPath)
		{
			if (FixedPath.Find(Point) == INDEX_NONE)
				DrawDebugSphere(World, Point, radius, 6,
					FColor::White, false, DeltaTime, 0, thickness);
		}
	}
}

void UVehicleADASComponent::UpdateAutoPilotHumanLike(
	UVehicleDriveAssemblyComponent* DriveAssembly, 
	FVector TargetLocation, 
	float SpeedLimit, 
	float AccelLimit, 
	float EndSpeed, 
	float Tolerance, 
	float DeltaTime,
	float MaxFieldOfView,
	float MinFieldOfView,
	float GoalWeight,
	float StandstillThreshold,
	int32 MaxNumOfRays,
	FVector2D ShiftInterval,
	ECollisionChannel TraceChannel,
	bool bDebugDraw)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_ADAS_UpdateAutoPilotHumanLike);

	if (!IsValid(DriveAssembly))return;

	UPrimitiveComponent* Carbody = DriveAssembly->GetCarbody();
	if (!IsValid(Carbody))return;

	// 包围盒
	FVector BoundExtent = FVector(0.f);
	FVector BoundOrigin = FVector(0.f);
	FTransform VehicleTransform;
	DriveAssembly->GetVehicleBound(BoundExtent, BoundOrigin, VehicleTransform);

	float DynSpeedLimit = FMath::Abs(SpeedLimit);
	float DynAccelLimit = FMath::Abs(AccelLimit);
	const FVector CurrLocation = VehicleTransform.GetLocation();
	const float MinTurnRadius = DriveAssembly->GetMinTurningRadius();

	UWorld* World = DriveAssembly->GetWorld();
	if (!IsValid(World)) return;

	TArray<FVector> PathPoints;
	GetPathPointsToTarget(
		World,
		CurrLocation,
		TargetLocation,
		PathPoints,
		DriveAssembly->GetOwner()
	);

	/*
	* 看向目标或者最近的导航点
	*/
	FVector CurrentTarget = TargetLocation;
	if (PathPoints.Num() > 2)
	{
		// 有至少三个路径点才需要跟随导航路线
		CurrentTarget = PathPoints[1];

		const float Speed2Turn = GetMaxSpeedToTurn(PathPoints, AccelLimit, MinTurnRadius);
		DynSpeedLimit = FMath::Min(Speed2Turn, DynSpeedLimit);
	}
	else
	{
		// 假如路径点太少或者没有路径点，就把当前位置和目标位置添加进去
		PathPoints.Reset();
		PathPoints.Add(CurrLocation);
		PathPoints.Add(TargetLocation);
	}

	float PathDist_m = GetRemainingPathLength(PathPoints, CurrLocation) * 0.01f;
	float LookaheadMultiplier = 2.f * 2.f; // maybe it is similar as Nyquist Sampling Theorem ?
	float MaxLongV = GetMaxSpeedToBrake(PathDist_m, AccelLimit, DeltaTime * LookaheadMultiplier, EndSpeed);
	float DynEndSpeed = FMath::Min(FMath::Max(0.f, MaxLongV), SpeedLimit);
	DynSpeedLimit = FMath::Min(DynSpeedLimit, DynEndSpeed);

	if (CurrentTarget == TargetLocation)
	{
		// 如果朝着终点开，强行把终点速度设为endspeed
		DynEndSpeed = FMath::Min(EndSpeed, DynEndSpeed);
	}

	UpdateAutoPilotSimple(
		DriveAssembly, CurrentTarget,
		DynSpeedLimit, DynAccelLimit, DynEndSpeed,
		Tolerance, DeltaTime, MaxFieldOfView, MinFieldOfView,
		GoalWeight, StandstillThreshold, MaxNumOfRays,
		ShiftInterval, TraceChannel, bDebugDraw
	);

	if (bDebugDraw)
	{
		for (FVector Point : PathPoints)
		{
			DrawDebugSphere(World, Point, 100.f, 6,
				FColor::Blue, false, DeltaTime, 0, 2.f);
		}
	}
}

TArray<FName> UVehicleADASComponent::GetNamesOfDriveAssembliesOfOwner()
{
	return UVehicleUtilities::GetNamesOfComponentsOfActor<UVehicleDriveAssemblyComponent>(this);
}

