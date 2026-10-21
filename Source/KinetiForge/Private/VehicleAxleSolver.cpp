// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehicleAxleSolver.h"
#include "VehicleDifferentialSolver.h"
#include "VehicleUtilities.h"

void FVehicleAxleSolver::UpdateLinearVelocity(FVehicleAxleSimState& State,
	const FVehicleSuspensionSimState* LeftSuspension, const FQuat4f* LeftChassisRotation,
	const FVehicleSuspensionSimState* RightSuspension, const FQuat4f* RightChassisRotation)
{
	FVector3f LeftWorldVel = FVector3f(0.f);
	FVector3f RightWorldVel = FVector3f(0.f);
	FQuat4f ChassisRot = FQuat4f();
	if (LeftSuspension)
	{
		LeftWorldVel = LeftSuspension->ImpactWorldVelocity;
		if (LeftChassisRotation) ChassisRot = *LeftChassisRotation;
	}
	if (RightSuspension)
	{
		RightWorldVel = RightSuspension->ImpactWorldVelocity;
		if (RightChassisRotation) ChassisRot = *RightChassisRotation;
	}
	State.WorldLinearVelocity = State.NumOfWheels ?
		FVector3f(LeftWorldVel + RightWorldVel) / State.NumOfWheels : FVector3f(0.f);

	ChassisRot.Normalize();
	State.LocalLinearVelocity = ChassisRot.UnrotateVector(State.WorldLinearVelocity);
}

void FVehicleAxleSolver::UpdateSteeringAssist(FVehicleAxleSimState& State,
	const FVehicleAxleSteeringConfig& SteeringConfig,
	const FVehicleSteeringAssistConfig& AssistConfig, float DeltaTime, float SteeringInput)
{
	if (AssistConfig.bSteeringAssistEnabled)
	{
		float TargetInput = 0.f;
		FVector2D LocalVelocity2D = FVector2D(State.LocalLinearVelocity.X, State.LocalLinearVelocity.Y);
		FVector2D LocalVelocityDirection = LocalVelocity2D.GetSafeNormal();

		//get the slip angle of the Chassis
		float ChassisSlipAngle = FMath::Asin(LocalVelocityDirection.Y);
		ChassisSlipAngle = FMath::RadiansToDegrees(ChassisSlipAngle);

		if (State.LocalLinearVelocity.X > AssistConfig.ActivationSpeed
			&& FMath::Abs(ChassisSlipAngle) > AssistConfig.ActivationAngle)
		{
			TargetInput = FMath::GetMappedRangeValueClamped(
				FVector2D(-SteeringConfig.MaxSteeringAngle, SteeringConfig.MaxSteeringAngle),
				FVector2D(-1.f, 1.f),
				ChassisSlipAngle * AssistConfig.Level
			);
		}

		if (AssistConfig.MaxSteerRate > SMALL_NUMBER)
		{
			State.SteeringAssistInput = FMath::FInterpConstantTo(
				State.SteeringAssistInput,
				TargetInput,
				DeltaTime,
				AssistConfig.MaxSteerRate
			);
		}
		else
		{
			// teleport if max steer rate is not valid
			State.SteeringAssistInput = TargetInput;
		}

		State.SteeringAssistInput = FMath::Clamp(State.SteeringAssistInput, -1.f, 1.f);
		State.RealSteeringValue = FMath::Clamp(SteeringInput + State.SteeringAssistInput, -1.f, 1.f);
	}
	else
	{
		State.SteeringAssistInput = 0.f;
		State.RealSteeringValue = SteeringInput;
	}
}

void FVehicleAxleSolver::UpdateSteering(FVehicleAxleSimState& State,
	const FVehicleAxleSteeringConfig& SteeringConfig, float SteeringInput, float TrackWidth)
{
	SteeringInput = FMath::Clamp(SteeringInput, -1.f, 1.f);
	float InsideWheelSteeringAngle = SteeringInput * SteeringConfig.MaxSteeringAngle;

	if (SteeringConfig.bAckermannSteering
		&& State.WheelBase > SMALL_NUMBER
		&& State.NumOfWheels == 2)
	{
		//clamp angle to avoid divided by 0
		InsideWheelSteeringAngle = FMath::Clamp(InsideWheelSteeringAngle, -89, 89);
		if (FMath::IsNearlyZero(InsideWheelSteeringAngle))	//if no steering
		{
			State.LeftWheelSteeringAngle = State.RightWheelSteeringAngle = 0.f;
		}
		else
		{
			float SteeringSign = FMath::Sign(InsideWheelSteeringAngle);
			float UnsignedOutsideWheelAngle = 0.f;
			float UnsignedInsideWheelAngle = FMath::Abs(InsideWheelSteeringAngle);
			float InsideSteerRad = FMath::DegreesToRadians(UnsignedInsideWheelAngle);
			float TanInside = FMath::Tan(InsideSteerRad);
			float TanOutSide = State.WheelBase / (State.WheelBase / TanInside + TrackWidth);
			float OutsideSteerRad = FMath::Atan(TanOutSide);
			UnsignedOutsideWheelAngle = FMath::RadiansToDegrees(OutsideSteerRad);
			UnsignedOutsideWheelAngle = FMath::Lerp(UnsignedInsideWheelAngle, UnsignedOutsideWheelAngle, SteeringConfig.AckermannRatio);

			if (SteeringInput > 0)
			{
				State.LeftWheelSteeringAngle = UnsignedOutsideWheelAngle * SteeringSign;
				State.RightWheelSteeringAngle = UnsignedInsideWheelAngle * SteeringSign;
			}
			else
			{
				State.LeftWheelSteeringAngle = UnsignedInsideWheelAngle * SteeringSign;
				State.RightWheelSteeringAngle = UnsignedOutsideWheelAngle * SteeringSign;
			}
		}
	}
	else
	{
		State.LeftWheelSteeringAngle = State.RightWheelSteeringAngle = InsideWheelSteeringAngle;
	}
}

void FVehicleAxleSolver::UpdateTCS(FVehicleAxleSimState& State, const FVehicleTCSConfig& Config,
	const FVehicleAxleWheelInput* LeftWheel, const FVehicleAxleWheelInput* RightWheel, float TargetDriveTorque)
{
	float SumSlipRatio = 0.f;
	if (LeftWheel) SumSlipRatio += FMath::Abs(LeftWheel->PredictedSlipRatio);
	if (RightWheel) SumSlipRatio += FMath::Abs(RightWheel->PredictedSlipRatio);
	float AvrgSlipRatio = SumSlipRatio / State.NumOfWheels;

	State.bTCSTriggered =
		Config.bTractionControlSystemEnabled
		&& State.NumOfWheelOnGround
		&& State.LocalLinearVelocity.X * TargetDriveTorque > SMALL_NUMBER
		&& FMath::Abs(State.LocalLinearVelocity.X) > Config.ActivationSpeed
		&& AvrgSlipRatio > Config.OptimalSlip;

	if (State.bTCSTriggered)
	{
		float Error = AvrgSlipRatio - Config.OptimalSlip;
		float TcsFactor = 1.0f - (Error * Config.Sensitivity);
		TcsFactor = FMath::Clamp(TcsFactor, 0.0f, 1.0f);
		State.AxleDriveTorque = TargetDriveTorque * TcsFactor;
		return;
	}
	State.AxleDriveTorque = TargetDriveTorque;
}

bool FVehicleAxleSolver::PrepareTransmissionStep(FVehicleAxleSimState& State,
	const FVehicleAxleConfig& AxleConfig, const FVehicleTCSConfig& TCSConfig,
	EVehicleAxleLayout AxleLayout, const FVehicleLimitedSlipDifferentialConfig* DifferentialConfig,
	const FVehicleAxleWheelInput* LeftWheel, const FVehicleAxleWheelInput* RightWheel,
	float SubstepDeltaTime, float DriveTorque, float BrakeInput, float HandbrakeInput)
{
	// number of living wheels
	State.NumOfWheels = (LeftWheel ? 1 : 0) + (RightWheel ? 1 : 0);

	// get brake torque
	State.BrakeTorque = AxleConfig.MaxBrakeTorque * FMath::Clamp(BrakeInput, 0.f, 1.f);
	State.HandbrakeTorque = AxleConfig.bAffectedByHandbrake ?
		AxleConfig.MaxHandbrakeTorque * FMath::Clamp(HandbrakeInput, 0.f, 1.f) : 0.f;

	const bool bNotPowerAxle = AxleConfig.TorqueWeight <= SMALL_NUMBER && FMath::IsNearlyZero(State.P3MotorTorque);
	const bool bWheelNotDriven = bNotPowerAxle || (DifferentialConfig == nullptr) || (State.NumOfWheels == 0);

	// 1. data preparation
	State.LeftDriveTorque = 0.f;
	State.RightDriveTorque = 0.f;
	State.ReflectedInertiaOnWheel = 0.f;

	// 2. torque distribution and inertia calculation
	if (bWheelNotDriven)
	{
		// losing power or differential broken
		State.AxleDriveTorque = bNotPowerAxle ? 0.f : DriveTorque;
		State.AxleEffectiveInertia = AxleConfig.DriveShaftInertia;
		float AngAcc = UVehicleUtilities::SafeDivide(State.AxleDriveTorque, State.AxleEffectiveInertia);
		State.AxleAngularVelocity += AngAcc * SubstepDeltaTime;
	}
	else
	{
		UpdateTCS(State, TCSConfig, LeftWheel, RightWheel, DriveTorque + State.P3MotorTorque);
		if (State.NumOfWheels == 2)
		{
			// two wheels: limited-slip-differential
			FVehicleDifferentialStepInput Input;
			Input.InputTorque = State.AxleDriveTorque;
			Input.LeftAngularVelocity = LeftWheel->AngularVelocity;
			Input.RightAngularVelocity = RightWheel->AngularVelocity;
			Input.LeftTotalInertia = LeftWheel->EffectiveInertia;
			Input.RightTotalInertia = RightWheel->EffectiveInertia;
			Input.DeltaTime = SubstepDeltaTime;
			const FVehicleDifferentialTorqueOutput Output =
				FVehicleDifferentialSolver::CalculateOutputTorques(*DifferentialConfig, Input);
			State.LeftDriveTorque = Output.LeftTorque;
			State.RightDriveTorque = Output.RightTorque;
			State.ReflectedInertiaOnWheel = FVehicleDifferentialSolver::CalculateReflectedOutputInertiaPerWheel(
				AxleConfig.DriveShaftInertia, DifferentialConfig->GearRatio);
		}
		else // single wheel
		{
			const float DiffGearRatio = DifferentialConfig->GearRatio;
			State.ReflectedInertiaOnWheel = AxleConfig.DriveShaftInertia * DiffGearRatio * DiffGearRatio;
			if (AxleLayout == EVehicleAxleLayout::TwoWheels)
			{
				// open diff
				State.LeftDriveTorque = State.RightDriveTorque =
					FVehicleDifferentialSolver::CalculateOpenOutputTorque(State.AxleDriveTorque, DiffGearRatio);
			}
			else
			{
				// if there was initially only one wheel
				State.LeftDriveTorque = State.RightDriveTorque = DiffGearRatio * State.AxleDriveTorque;
			}
		}
	}
	return bWheelNotDriven;
}

float FVehicleAxleSolver::FinalizeTransmissionStep(FVehicleAxleSimState& State,
	const FVehicleAxleConfig& AxleConfig, const FVehicleLimitedSlipDifferentialConfig* DifferentialConfig,
	const FVehicleAxleWheelInput* LeftWheel, const FVehicleAxleWheelInput* RightWheel, bool bWheelNotDriven)
{
	// 4. get axle effective inertia and angular velocity
	if (!bWheelNotDriven)
	{
		float ReflectedInertiaOfWheels = 0.f;
		if (State.NumOfWheels == 2)
		{
			State.AxleAngularVelocity = FVehicleDifferentialSolver::CalculateInputAngularVelocity(
				LeftWheel->AngularVelocity, RightWheel->AngularVelocity, DifferentialConfig->GearRatio);
			ReflectedInertiaOfWheels = FVehicleDifferentialSolver::CalculateReflectedInputInertia(
				LeftWheel->IntrinsicInertia, RightWheel->IntrinsicInertia, // only the inertia of the wheel itself
				DifferentialConfig->GearRatio);
		}
		else
		{
			// single wheel
			const float DiffGearRatio = DifferentialConfig->GearRatio;
			const FVehicleAxleWheelInput* ActiveWheel = LeftWheel ? LeftWheel : RightWheel;
			State.AxleAngularVelocity = ActiveWheel->AngularVelocity * DiffGearRatio;
			ReflectedInertiaOfWheels = UVehicleUtilities::SafeDivide(ActiveWheel->IntrinsicInertia, DiffGearRatio * DiffGearRatio);
		}
		State.AxleEffectiveInertia = ReflectedInertiaOfWheels + AxleConfig.DriveShaftInertia;
	}
	// 5. output effective inertia and angular velocity
	return bWheelNotDriven ? 0.f : AxleConfig.DriveShaftTorsionalStiffness;
}
