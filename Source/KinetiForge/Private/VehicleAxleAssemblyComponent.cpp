// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)


#include "VehicleAxleAssemblyComponent.h"
#include "ProfilingDebugging/CpuProfilerTrace.h"
#include "VehicleWheelComponent.h"
#include "VehicleDifferentialComponent.h"
#include "VehicleWheelCoordinatorComponent.h"
#include "VehicleUtilities.h"
#include "VehicleAxleSolver.h"
#include "VehicleAxleBridge.h"
#include "Misc/ScopeLock.h"

TSharedPtr<FVehicleAxleBridge, ESPMode::ThreadSafe> UVehicleAxleAssemblyComponent::GetAxleBridge() const
{
	return AxleBridge;
}

FVehicleAxleRuntimeSettings UVehicleAxleAssemblyComponent::CaptureRuntimeSettings() const
{
	check(IsInGameThread());
	FVehicleAxleRuntimeSettings Settings;
	Settings.AxleConfig = AxleConfig;
	Settings.SteeringConfig = AxleSteeringConfig;
	Settings.SteeringAssistConfig = SteeringAssistConfig;
	Settings.TCSConfig = TCSConfig;
	Settings.Layout = AxleLayout;
	Settings.SuspensionType = SuspensionType;
	if (UVehicleDifferentialComponent* Diff = Differential.Get())
	{
		Settings.bHasDifferential = true;
		Settings.DifferentialConfig = Diff->GetConfig();
	}
	return Settings;
}

bool UVehicleAxleAssemblyComponent::BindIsolatedAxleRuntime()
{
	check(IsInGameThread());
	if (!IsRegistered() || IsBeingDestroyed()) return false;
	UVehicleWheelComponent* Wheels[] = { LeftWheel.Get(), RightWheel.Get() };
	for (auto* Wheel : Wheels) if (Wheel && (!Wheel->IsRegistered() || Wheel->IsBeingDestroyed())) return false;
	// A axle-level assembly may not take ownership of wheels already assigned to another axle.
	TArray<UVehicleAxleAssemblyComponent*> OtherAxles;
	if (AActor* Owner = GetOwner()) Owner->GetComponents(OtherAxles);
	for (auto* Other : OtherAxles)
	{
		if (Other == this) continue;
		for (auto* Wheel : Wheels)
			if (Wheel && (Other->LeftWheel.Get() == Wheel || Other->RightWheel.Get() == Wheel)) return false;
	}
	return GetAxleBridge().IsValid();
}

void UVehicleAxleAssemblyComponent::SubmitRuntimeSettings()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_AxleAssembly_SubmitRuntimeSettings);
	if (TSharedPtr<FVehicleAxleBridge, ESPMode::ThreadSafe> Bridge = GetAxleBridge())
	{
		check(IsInGameThread());
		if (UVehicleDifferentialComponent* Diff = Differential.Get()) Diff->AddConfigurationConsumer(this);
		Bridge->SubmitSettings(CaptureRuntimeSettings());
	}
}

const FVehicleAxleSimState& UVehicleAxleAssemblyComponent::GetStateForCaller() const
{
	return IsInGameThread() ? State : GetAxleBridge()->GetRuntimeForSimulation().State;
}

FVehicleAxleSimState& UVehicleAxleAssemblyComponent::GetSimulationState()
{
	return GetAxleBridge()->GetRuntimeForSimulation().State;
}

const FVehicleAxleConfig& UVehicleAxleAssemblyComponent::GetSimulationAxleConfig()
{
	return GetAxleBridge()->GetRuntimeForSimulation().Settings.AxleConfig;
}

void UVehicleAxleAssemblyComponent::RefreshPublishedAxleState()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_AxleAssembly_RefreshPublishedState);
	check(IsInGameThread());
	TSharedPtr<FVehicleAxleBridge, ESPMode::ThreadSafe> Bridge = GetAxleBridge();
	if (!Bridge) return;
	const FVehicleAxleResult Result = Bridge->ReadResult();
	if (LastAxlePublicationSequence == Result.PublicationSequence) return;
	LastAxlePublicationSequence = Result.PublicationSequence;
	State = Result.State;
	if (Result.AppliedP3Sequence < RequestedP3Sequence) State.P3MotorTorque = RequestedP3MotorTorque;
	if (Result.AppliedWheelBaseSequence < RequestedWheelBaseSequence) State.WheelBase = RequestedWheelBase;
}

void UVehicleAxleAssemblyComponent::SetAxleConfig(const FVehicleAxleConfig& NewConfig)
{
	AxleConfig = NewConfig;
	SubmitRuntimeSettings();
}

void UVehicleAxleAssemblyComponent::SetAxleSteeringConfig(const FVehicleAxleSteeringConfig& NewConfig)
{
	AxleSteeringConfig = NewConfig;
	SubmitRuntimeSettings();
}

void UVehicleAxleAssemblyComponent::SetSteeringAssistConfig(const FVehicleSteeringAssistConfig& NewConfig)
{
	SteeringAssistConfig = NewConfig;
	SubmitRuntimeSettings();
}

void UVehicleAxleAssemblyComponent::SetTCSConfig(const FVehicleTCSConfig& NewConfig)
{
	TCSConfig = NewConfig;
	SubmitRuntimeSettings();
}

void UVehicleAxleAssemblyComponent::SetAxleSuspensionType(EVehicleAxleSuspensionType NewType)
{
	SuspensionType = NewType;
	SubmitRuntimeSettings();
}

void UVehicleAxleAssemblyComponent::SetP3MotorTorque(float Torque)
{
	if (TSharedPtr<FVehicleAxleBridge, ESPMode::ThreadSafe> Bridge = GetAxleBridge())
	{
		if (IsInGameThread())
		{
			RequestedP3MotorTorque = State.P3MotorTorque = Torque;
			RequestedP3Sequence = Bridge->RequestP3MotorTorque(Torque);
		}
		else Bridge->GetRuntimeForSimulation().State.P3MotorTorque = Torque;
		return;
	}
}

void UVehicleAxleAssemblyComponent::SetWheelBase(float WheelBase)
{
	if (TSharedPtr<FVehicleAxleBridge, ESPMode::ThreadSafe> Bridge = GetAxleBridge())
	{
		if (IsInGameThread())
		{
			RequestedWheelBase = State.WheelBase = WheelBase;
			RequestedWheelBaseSequence = Bridge->RequestWheelBase(WheelBase);
		}
		else Bridge->GetRuntimeForSimulation().State.WheelBase = WheelBase;
		return;
	}
}

FVehicleAxleWheelInput UVehicleAxleAssemblyComponent::CaptureWheelTransmissionInput(UVehicleWheelComponent* Wheel)
{
	if (!Wheel) return FVehicleAxleWheelInput();
	return Wheel->CaptureSimulationTransmissionInput();
}

float UVehicleAxleAssemblyComponent::CaptureSimulationTrackWidth(float FallbackWidth, UVehicleWheelComponent* Left, UVehicleWheelComponent* Right)
{
	if (!(Left && Right)) return FallbackWidth;
	const float LeftY = FTransform3f(Left->GetSimulationRelativeTransform()).TransformPositionNoScale(
		Left->GetSimulationDesignedHubTransform().GetLocation()).Y;
	const float RightY = FTransform3f(Right->GetSimulationRelativeTransform()).TransformPositionNoScale(
		Right->GetSimulationDesignedHubTransform().GetLocation()).Y;
	return FMath::Abs(RightY - LeftY);
}

// Sets default values for this component's properties
UVehicleAxleAssemblyComponent::UVehicleAxleAssemblyComponent()
{
	AxleBridge = MakeShared<FVehicleAxleBridge, ESPMode::ThreadSafe>(FVehicleAxleRuntime{});
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true; // GT publication/config transfer only; manual physics stays manual.
	// ...
}


// Called when the game starts
void UVehicleAxleAssemblyComponent::BeginPlay()
{
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::BeginPlay();

	// ...
	// OnRegister already established generated wheels. BeginPlay only re-resolves
	// referenced wheels, whose components may not exist until the world starts.
	if (bUseExistingWheelComponent)
	{
		InitializeWheels();
	}
	GenerateDifferential();
	WheelCoordinator = UVehicleWheelCoordinatorComponent::FindWheelCoordinator(Chassis.Get());
	if (WheelCoordinator.IsValid())WheelCoordinator->RegisterAxle(this);
	SubmitRuntimeSettings();
}

void UVehicleAxleAssemblyComponent::OnRegister()
{
	bManualPhysicsReady.Store(false);
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::OnRegister();

	//...
	Chassis = UVehicleUtilities::FindPhysicalParent(this);
	//PreviewWheelMesh();
	InitializeWheels();
	SubmitRuntimeSettings();
	bManualPhysicsReady.Store(true);
}

void UVehicleAxleAssemblyComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	bManualPhysicsReady.Store(false);
	PhysicsOwnership.InvalidateOwnerAndWait();
	UVehicleUtilities::DestroyAutoGeneratedComponents(AutoGeneratedComponents);
	//...
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UVehicleAxleAssemblyComponent::OnUnregister()
{
	bManualPhysicsReady.Store(false);
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::OnUnregister();
}

void UVehicleAxleAssemblyComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	bManualPhysicsReady.Store(false);
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::EndPlay(EndPlayReason);
}

void UVehicleAxleAssemblyComponent::BeginDestroy()
{
	bManualPhysicsReady.Store(false);
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::BeginDestroy();
}

void UVehicleAxleAssemblyComponent::UpdateSteering(
	float InSteeringInput,
	UVehicleWheelComponent* WheelL,
	UVehicleWheelComponent* WheelR)
{
	const auto Bridge = GetAxleBridge();
	FVehicleAxleSimState& StepState = GetSimulationState();
	const FVehicleAxleSteeringConfig& SteeringConfig = Bridge->GetRuntimeForSimulation().Settings.SteeringConfig;
	const FVehicleAxleConfig& StepAxleConfig = Bridge->GetRuntimeForSimulation().Settings.AxleConfig;
	// approximate the track width
	FVehicleAxleSolver::UpdateSteering(StepState, SteeringConfig, InSteeringInput,
		CaptureSimulationTrackWidth(StepAxleConfig.TrackWidth, WheelL, WheelR));
}

void UVehicleAxleAssemblyComponent::UpdateSteeringAssist(
	float DeltaTime, 
	float InSteeringInput)
{
	const auto Bridge = GetAxleBridge();
	FVehicleAxleSimState& StepState = GetSimulationState();
	const FVehicleAxleSteeringConfig& SteeringConfig = Bridge->GetRuntimeForSimulation().Settings.SteeringConfig;
	const FVehicleSteeringAssistConfig& AssistConfig = Bridge->GetRuntimeForSimulation().Settings.SteeringAssistConfig;
	FVehicleAxleSolver::UpdateSteeringAssist(StepState, SteeringConfig, AssistConfig, DeltaTime, InSteeringInput);
}

void UVehicleAxleAssemblyComponent::UpdateLinearVelocity(
	UVehicleWheelComponent* WheelL, 
	UVehicleWheelComponent* WheelR)
{
	FVehicleAxleSimState& StepState = GetSimulationState();
	const FVehicleSuspensionSimState* LeftState = WheelL ? &WheelL->GetSimulationSuspensionState() : nullptr;
	const FVehicleSuspensionSimState* RightState = WheelR ? &WheelR->GetSimulationSuspensionState() : nullptr;
	FQuat4f LeftRotation, RightRotation;
	if (WheelL) LeftRotation = (FQuat4f)WheelL->GetSimulationChassisTransform().GetRotation();
	if (WheelR) RightRotation = (FQuat4f)WheelR->GetSimulationChassisTransform().GetRotation();
	FVehicleAxleSolver::UpdateLinearVelocity(StepState, LeftState, WheelL ? &LeftRotation : nullptr,
		RightState, WheelR ? &RightRotation : nullptr);
}

void UVehicleAxleAssemblyComponent::UpdateTCS(
	UVehicleWheelComponent* WheelL, 
	UVehicleWheelComponent* WheelR, 
	float TargetDriveTorque)
{
	const auto Bridge = GetAxleBridge();
	FVehicleAxleSimState& StepState = GetSimulationState();
	const FVehicleTCSConfig& StepTCSConfig = Bridge->GetRuntimeForSimulation().Settings.TCSConfig;
	const FVehicleAxleWheelInput LeftInput = CaptureWheelTransmissionInput(WheelL);
	const FVehicleAxleWheelInput RightInput = CaptureWheelTransmissionInput(WheelR);
	FVehicleAxleSolver::UpdateTCS(StepState, StepTCSConfig,
		WheelL ? &LeftInput : nullptr, WheelR ? &RightInput : nullptr, TargetDriveTorque);
}

float UVehicleAxleAssemblyComponent::GetTrackWidth(
	UVehicleWheelComponent* WheelL, 
	UVehicleWheelComponent* WheelR)
{
	if (!(WheelL && WheelR))return AxleConfig.TrackWidth;

	const FTransform& TransformL = WheelL->GetRelativeTransform();
	const FTransform& TransformR = WheelR->GetRelativeTransform();

	float WheelL_Y = FTransform3f(TransformL).TransformPositionNoScale(WheelL->GetDesignedHubLocalTransform().GetLocation()).Y;
	float WheelR_Y = FTransform3f(TransformR).TransformPositionNoScale(WheelR->GetDesignedHubLocalTransform().GetLocation()).Y;

	return FMath::Abs(WheelR_Y - WheelL_Y);
}

void UVehicleAxleAssemblyComponent::PreStepSolidAxleSuspension(
	const float InMacroDeltaTime,
	const float SteerAngleLeft,
	const float SteerAngleRight,
	const float ActiveSwaybarStiffness,
	UVehicleWheelComponent* WheelL,
	UVehicleWheelComponent* WheelR)
{
	const FVehicleAxleConfig& StepAxleConfig = GetSimulationAxleConfig();
	FVehicleSuspensionSimContext Ctx_L;
	FVehicleSuspensionSimContext Ctx_R;

	float HubZ_R = WheelR->GetSimulationSuspensionState().HubChassisLocation.Z;
	float HubZ_L = WheelL->GetSimulationSuspensionState().HubChassisLocation.Z;

	WheelL->StartPreStepSolidAxleSuspension(Ctx_L, InMacroDeltaTime, SteerAngleLeft, ActiveSwaybarStiffness, HubZ_R);
	WheelR->StartPreStepSolidAxleSuspension(Ctx_R, InMacroDeltaTime, SteerAngleRight, ActiveSwaybarStiffness, HubZ_L);

	float TrackWidth = CaptureSimulationTrackWidth(StepAxleConfig.TrackWidth, WheelL, WheelR);
	float AxleHalfWidth = (
		TrackWidth
		- WheelL->GetSimulationKinematicsConfig().HubOffsetFromLowerJoint.Y
		- WheelR->GetSimulationKinematicsConfig().HubOffsetFromLowerJoint.Y
		) * 0.5f;

	FVector3f LeftTopMount = WheelL->GetSimulationTopMountChassisLocation();
	FVector3f RightTopMount = WheelR->GetSimulationTopMountChassisLocation();

	FVector3f AxleCenter = FVector3f::ZeroVector;
	FQuat4f AxleRotation = FQuat4f::Identity;

	float StrutRealLength_L = Ctx_L.StrutCurrentLength + WheelL->GetSimulationKinematicsConfig().MinStrutLength;
	float StrutRealLength_R = Ctx_R.StrutCurrentLength + WheelR->GetSimulationKinematicsConfig().MinStrutLength;

	FVehicleSuspensionSolver::SolveSolidAxlePosture(
		LeftTopMount, RightTopMount,
		StrutRealLength_L, StrutRealLength_R,
		AxleHalfWidth,
		AxleCenter, AxleRotation
	);

	WheelL->FinalizePreStepSolidAxleSuspension(
		Ctx_L, InMacroDeltaTime,
		ActiveSwaybarStiffness, HubZ_R,
		AxleHalfWidth, AxleCenter, AxleRotation
	);

	WheelR->FinalizePreStepSolidAxleSuspension(
		Ctx_R, InMacroDeltaTime,
		ActiveSwaybarStiffness, HubZ_L,
		AxleHalfWidth, AxleCenter, AxleRotation
	);
}

// Called every frame
void UVehicleAxleAssemblyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_AxleAssembly_GameThreadTick);
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	RefreshPublishedAxleState();
	SubmitRuntimeSettings();

	// ...
}

void UVehicleAxleAssemblyComponent::CopyAxleConfig(const UVehicleAxleAssemblyComponent* Source, UVehicleAxleAssemblyComponent* Target, bool bReInitializeWheel)
{
	Target->bUseExistingWheelComponent = Source->bUseExistingWheelComponent;
	Target->LeftWheelComponentName = Source->LeftWheelComponentName;
	Target->RightWheelComponentName = Source->RightWheelComponentName;
	Target->WheelClass = Source->WheelClass;
	Target->VehicleWheelComponentSetupRotation = Source->VehicleWheelComponentSetupRotation;
	Target->bUseExistingDifferentialComponent = Source->bUseExistingDifferentialComponent;
	Target->DifferentialComponentName = Source->DifferentialComponentName;
	Target->DifferentialClass = Source->DifferentialClass;
	Target->AxleLayout = Source->AxleLayout;
	Target->AxleConfig = Source->AxleConfig;
	Target->AxleSteeringConfig = Source->AxleSteeringConfig;
	Target->SteeringAssistConfig = Source->SteeringAssistConfig;
	Target->TCSConfig = Source->TCSConfig;
	Target->SubmitRuntimeSettings();

	if (bReInitializeWheel)
	{
		Target->InitializeWheels();
	}
}

void UVehicleAxleAssemblyComponent::ApplyInitialOverrides(const FVehicleAxleSpawnTemplate& AxleTemplate)
{
	//override these
	if (AxleTemplate.WheelClassOverride)WheelClass = AxleTemplate.WheelClassOverride;
	if (AxleTemplate.DifferentialClassOverride)DifferentialClass = AxleTemplate.DifferentialClassOverride;
	if (AxleTemplate.TrackWidthOverride >= 0)AxleConfig.TrackWidth = AxleTemplate.TrackWidthOverride;
	if (AxleTemplate.bDiasbleSteering)AxleSteeringConfig.bAffectedBySteering = false;
	if (AxleTemplate.bDisableHandbrake)AxleConfig.bAffectedByHandbrake = false;
	if (AxleTemplate.bDisableTractionControl)TCSConfig.bTractionControlSystemEnabled = false;
	if (AxleTemplate.TorqueWeightOverride >= 0)AxleConfig.TorqueWeight = AxleTemplate.TorqueWeightOverride;
	SubmitRuntimeSettings();
}

void UVehicleAxleAssemblyComponent::PreStepAxle(
	float InMacroDeltaTime, 
	float InSteeringInput)
{
	const auto Bridge = GetAxleBridge();
	if (Bridge) Bridge->ConsumeInputs();
	FVehicleAxleSimState& StepState = GetSimulationState();
	const FVehicleAxleSteeringConfig& SteeringConfig = Bridge->GetRuntimeForSimulation().Settings.SteeringConfig;
	const FVehicleAxleConfig& StepAxleConfig = Bridge->GetRuntimeForSimulation().Settings.AxleConfig;
	const EVehicleAxleSuspensionType StepSuspensionType = Bridge->GetRuntimeForSimulation().Settings.SuspensionType;
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_AxleAssembly_PhysicsPreStep);

	UVehicleWheelComponent* WheelL = LeftWheel.Get();
	UVehicleWheelComponent* WheelR = RightWheel.Get();

	StepState.NumOfWheels = (int32)(WheelL != nullptr) + (int32)(WheelR != nullptr);
	if (StepState.NumOfWheels <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AxleAssembly: No Valid Wheels"));
		return;
	}

	if (WheelL) WheelL->PrepareSimulationStepInputs();
	if (WheelR) WheelR->PrepareSimulationStepInputs();
	UpdateLinearVelocity(WheelL, WheelR);

	if (SteeringConfig.bAffectedBySteering)
	{
		UpdateSteeringAssist(InMacroDeltaTime, InSteeringInput);
		UpdateSteering(StepState.RealSteeringValue, WheelL, WheelR);
	}

	if (StepState.NumOfWheels == 2)
	{
		float HubZ_R = 0.f;
		float HubZ_L = 0.f;
		switch (StepSuspensionType)
		{
		default:
		case EVehicleAxleSuspensionType::Independent:
			HubZ_R = WheelR->GetSimulationSuspensionState().HubChassisLocation.Z;
			HubZ_L = WheelL->GetSimulationSuspensionState().HubChassisLocation.Z;
			{
				FVehicleWheelIndependentSuspensionStep LeftStep;
				FVehicleWheelIndependentSuspensionStep RightStep;
				const bool bPreparedLeft = WheelL->StartPreStepIndependentSuspension(
					InMacroDeltaTime, StepState.LeftWheelSteeringAngle, LeftStep);
				const bool bPreparedRight = WheelR->StartPreStepIndependentSuspension(
					InMacroDeltaTime, StepState.RightWheelSteeringAngle, RightStep);
				if (bPreparedLeft && bPreparedRight)
				{
					FVehicleSuspensionIntegrationInput LeftInput = WheelL->GetPreparedIndependentSuspensionInput(LeftStep);
					FVehicleSuspensionIntegrationInput RightInput = WheelR->GetPreparedIndependentSuspensionInput(RightStep);
					FVehicleSuspensionSolver::IntegratePairedIndependentSuspension(
						LeftInput, RightInput, StepAxleConfig.SwaybarStiffness);
					FVehicleSuspensionSolver::PrepareIndependentSuspensionGeometry(
						LeftInput.Context, LeftInput.WheelRadius, LeftInput.KinematicsConfig, LeftInput.CachedLUTs);
					FVehicleSuspensionSolver::PrepareIndependentSuspensionGeometry(
						RightInput.Context, RightInput.WheelRadius, RightInput.KinematicsConfig, RightInput.CachedLUTs);
					const float FinalLeftHubZ = LeftInput.Context.HubChassisTransform.GetLocation().Z;
					const float FinalRightHubZ = RightInput.Context.HubChassisTransform.GetLocation().Z;
					WheelL->FinalizePreStepIndependentSuspension(
						StepAxleConfig.SwaybarStiffness, FinalRightHubZ, LeftStep, true);
					WheelR->FinalizePreStepIndependentSuspension(
						StepAxleConfig.SwaybarStiffness, FinalLeftHubZ, RightStep, true);
				}
				else
				{
					if (bPreparedLeft)
						WheelL->FinalizePreStepIndependentSuspension(
							StepAxleConfig.SwaybarStiffness, HubZ_R, LeftStep, false);
					if (bPreparedRight)
						WheelR->FinalizePreStepIndependentSuspension(
							StepAxleConfig.SwaybarStiffness, HubZ_L, RightStep, false);
				}
			}
			break;
		case EVehicleAxleSuspensionType::Solid:
			PreStepSolidAxleSuspension(
				InMacroDeltaTime,
				StepState.LeftWheelSteeringAngle,
				StepState.RightWheelSteeringAngle,
				StepAxleConfig.SwaybarStiffness,
				WheelL,
				WheelR
			);
			break;
		}
		StepState.NumOfWheelOnGround = WheelL->GetSimulationSuspensionState().bWheelOnGround + WheelR->GetSimulationSuspensionState().bWheelOnGround;
		WheelL->PreStepWheel(InMacroDeltaTime);
		WheelR->PreStepWheel(InMacroDeltaTime);
	}
	else
	{
		if (WheelL)
		{
			WheelL->PreStepIndependentSuspension(
				InMacroDeltaTime,
				StepState.LeftWheelSteeringAngle,
				0.f,
				0.f
			);
			StepState.NumOfWheelOnGround = WheelL->GetSimulationSuspensionState().bWheelOnGround;
			WheelL->PreStepWheel(InMacroDeltaTime);
		}

		if (WheelR)
		{
			WheelR->PreStepIndependentSuspension(
				InMacroDeltaTime,
				StepState.RightWheelSteeringAngle,
				0.f,
				0.f
			);
			StepState.NumOfWheelOnGround = WheelR->GetSimulationSuspensionState().bWheelOnGround;
			WheelR->PreStepWheel(InMacroDeltaTime);
		}
	}
}

void UVehicleAxleAssemblyComponent::SubstepAxle(
	float InSubstepDeltaTime,
	float InDriveTorque,
	float InBrakeInput,
	float InHandbrakeInput,
	float& OutAxleEffectiveInertia,
	float& OutAngularVelocity,
	float& OutDriveShaftStiffness)
{
	const auto Bridge = GetAxleBridge();
	FVehicleAxleSimState& StepState = GetSimulationState();
	const FVehicleAxleConfig& StepAxleConfig = Bridge->GetRuntimeForSimulation().Settings.AxleConfig;
	const FVehicleTCSConfig& StepTCSConfig = Bridge->GetRuntimeForSimulation().Settings.TCSConfig;
	const EVehicleAxleLayout StepLayout = Bridge->GetRuntimeForSimulation().Settings.Layout;
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_AxleAssembly_PhysicsSubstep);

	UVehicleWheelComponent* WheelL = LeftWheel.Get();
	UVehicleWheelComponent* WheelR = RightWheel.Get();
	const FVehicleLimitedSlipDifferentialConfig* DiffConfig = Bridge->GetRuntimeForSimulation().Settings.bHasDifferential
		? &Bridge->GetRuntimeForSimulation().Settings.DifferentialConfig : nullptr;
	FVehicleAxleWheelInput LeftInput = CaptureWheelTransmissionInput(WheelL);
	FVehicleAxleWheelInput RightInput = CaptureWheelTransmissionInput(WheelR);
	const bool bWheelNotDriven = FVehicleAxleSolver::PrepareTransmissionStep(StepState,
		StepAxleConfig, StepTCSConfig, StepLayout, DiffConfig,
		WheelL ? &LeftInput : nullptr, WheelR ? &RightInput : nullptr,
		InSubstepDeltaTime, InDriveTorque, InBrakeInput, InHandbrakeInput);

	// 3. wheel substepping
	if (WheelL) WheelL->SubStepWheel(InSubstepDeltaTime, StepState.LeftDriveTorque, StepState.BrakeTorque, StepState.HandbrakeTorque, StepState.ReflectedInertiaOnWheel);
	if (WheelR) WheelR->SubStepWheel(InSubstepDeltaTime, StepState.RightDriveTorque, StepState.BrakeTorque, StepState.HandbrakeTorque, StepState.ReflectedInertiaOnWheel);

	// Feedback must observe this substep, never the preceding published display result.
	LeftInput = CaptureWheelTransmissionInput(WheelL);
	RightInput = CaptureWheelTransmissionInput(WheelR);
	const float EffectiveDriveShaftStiffness = FVehicleAxleSolver::FinalizeTransmissionStep(StepState,
		StepAxleConfig, DiffConfig, WheelL ? &LeftInput : nullptr, WheelR ? &RightInput : nullptr, bWheelNotDriven);
	OutAxleEffectiveInertia = StepState.AxleEffectiveInertia;
	OutAngularVelocity = StepState.AxleAngularVelocity;
	OutDriveShaftStiffness = EffectiveDriveShaftStiffness;
}

void UVehicleAxleAssemblyComponent::PostStepAxle()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_AxleAssembly_PhysicsPostStep);

	UVehicleWheelComponent* WheelL = LeftWheel.Get();
	UVehicleWheelComponent* WheelR = RightWheel.Get();

	if (WheelL)
	{
		WheelL->PostStepApplyForce();
	}
	if (WheelR)
	{
		WheelR->PostStepApplyForce();
	}
	GetAxleBridge()->PublishResult();
	if (IsInGameThread()) RefreshPublishedAxleState();
}

void UVehicleAxleAssemblyComponent::InitializeWheels()
{
	check(IsInGameThread());
	// A different wheel can share the old body's identity. Retire the captured assembly
	// before changing slots rather than relying on the Drive's body-rebuild polling.
	PhysicsOwnership.InvalidateOwnerAndWait();

	// Single entry point for every wheel-set change. Reconcile only touches slots whose
	// state actually differs, so calling this repeatedly (registration, BeginPlay,
	// Blueprint refresh) converges instead of accumulating or destroying components.
	ReconcileWheelSlots();

	// The template only configures wheels this axle generated. A referenced wheel
	// keeps the settings its owner authored for it.
	if (!bUseExistingWheelComponent && IsValid(WheelClass))
	{
		if (const UVehicleWheelComponent* TemplateWheel = Cast<UVehicleWheelComponent>(WheelClass->GetDefaultObject()))
		{
			if (LeftWheel.IsValid()) UVehicleWheelComponent::CopyWheelConfig(TemplateWheel, LeftWheel.Get());
			if (RightWheel.IsValid()) UVehicleWheelComponent::CopyWheelConfig(TemplateWheel, RightWheel.Get());
		}
	}

	// Track width is an axle-owned decision; manual wheels stay where their owner put them.
	PlaceManagedWheels();

	if (LeftWheel.IsValid()) LeftWheel->InitializeWheel();
	if (RightWheel.IsValid()) RightWheel->InitializeWheel();
	if (HasBothWheels()) ApplySolidAxleStateDirect();
}

void UVehicleAxleAssemblyComponent::UpdatePhysics(
	float InPhysicsDeltaTime,
	float InDriveTorque, 
	float InBrakeInput,
	float InHandbrakeInput,
	float InSteeringInput,
	float& OutAxleEffectiveInertia,
	float& OutAngularVelocity,
	float& OutDriveShaftStiffness)
{
	if (!bManualPhysicsReady.Load() || !FMath::IsFinite(InPhysicsDeltaTime) || InPhysicsDeltaTime <= 0.f
		|| !IsRegistered() || IsBeingDestroyed() || PhysicsOwnership.HasActiveOwner()) return;
	const AActor* Owner = GetOwner();
	if (Owner && Owner->IsActorBeingDestroyed()) return;
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_AxleAssembly_UpdatePhysics);

	UPrimitiveComponent* Body = Chassis.Get();
	if (!Body || !Body->IsRegistered() || Body->IsBeingDestroyed()
		|| !UVehicleUtilities::GetInternalHandle(Body)) return;
	UVehicleWheelComponent* WheelL = LeftWheel.Get();
	UVehicleWheelComponent* WheelR = RightWheel.Get();
	// Preflight the complete set before consuming inputs or advancing either wheel.
	// An absent side still supports the existing single-wheel path.
	if ((!WheelL && !WheelR) || (WheelL && WheelL == WheelR)
		|| (WheelL && !WheelL->CanUpdateManualPhysics(InPhysicsDeltaTime))
		|| (WheelR && !WheelR->CanUpdateManualPhysics(InPhysicsDeltaTime))) return;

	PreStepAxle(InPhysicsDeltaTime, InSteeringInput);
	SubstepAxle(InPhysicsDeltaTime,
		InDriveTorque, InBrakeInput, InHandbrakeInput,
		OutAxleEffectiveInertia, OutAngularVelocity, OutDriveShaftStiffness);
	PostStepAxle();
}

void UVehicleAxleAssemblyComponent::SetWheelPosition(float NewTrackWidth)
{
	NewTrackWidth = FMath::Abs(NewTrackWidth);
	EnsureAxleAttachedToChassis();
	PlaceWheel(EWheelSide::Left, NewTrackWidth);
	PlaceWheel(EWheelSide::Right, NewTrackWidth);
}

void UVehicleAxleAssemblyComponent::UpdateTrackWidth()
{
	SetWheelPosition(AxleConfig.TrackWidth);
}

float UVehicleAxleAssemblyComponent::GetTrackWidth()
{
	UVehicleWheelComponent* WheelL = LeftWheel.Get();
	UVehicleWheelComponent* WheelR = RightWheel.Get();
	if (WheelL && WheelR)
	{
		return GetTrackWidth(WheelL, WheelR);
	}
	else
	{
		return AxleConfig.TrackWidth;
	}
}

void UVehicleAxleAssemblyComponent::UpdateSolidAxleAnim(
	USceneComponent* InSolidAxleMesh, 
	EVehicleSolidAxleAnimPivot AxleMeshAnchorPoint, 
	const FVector InMeshRightVector,
	const FVector InMeshForwardVector)
{
	if (!InSolidAxleMesh || !LeftWheel.IsValid() || !RightWheel.IsValid() || !Chassis.IsValid()) return;

	FVector LeftLocal = (FVector)LeftWheel->GetAnimHubChassisLocation();
	FVector RightLocal = (FVector)RightWheel->GetAnimHubChassisLocation();

	FVector AxleDirLocal = (RightLocal - LeftLocal).GetSafeNormal();
	FVector AxleCenterLocal = (RightLocal + LeftLocal) * 0.5f;

	// 第一步：把网格体的主轴（如 Y 轴）对齐到左右轮的连线上
	FQuat Q1 = FQuat::FindBetweenNormals(InMeshRightVector, AxleDirLocal);

	// 此时，网格体可能沿着车轴发生了任意角度的“自转”。
	// 我们把它的副轴（如 X 轴）经过第一步旋转后的实际朝向拿出来：
	FVector RotatedForward = Q1.RotateVector(InMeshForwardVector);

	// 我们希望它的副轴尽量指向底盘的正前方 (1, 0, 0)。
	// 把 (1, 0, 0) 投影到垂直于车轴的平面上，得到理想的朝向：
	FVector TargetForward = FVector::VectorPlaneProject(FVector(1.f, 0.f, 0.f), AxleDirLocal).GetSafeNormal();

	// 第二步：算出消除自转的纠正 Quat
	FQuat Q2 = FQuat::FindBetweenNormals(RotatedForward, TargetForward);

	// 合并两步旋转：先做 Q1，再做 Q2 (注意四元数乘法从右向左读)
	FQuat FinalLocalRotation = Q2 * Q1;

	FVector FinalLocalLocation = AxleCenterLocal;
	switch (AxleMeshAnchorPoint)
	{
	case EVehicleSolidAxleAnimPivot::Left:
		FinalLocalLocation = LeftLocal;
		break;
	case EVehicleSolidAxleAnimPivot::Right:
		FinalLocalLocation = RightLocal;
		break;
	case EVehicleSolidAxleAnimPivot::Center:
	default:
		break;
	}

	FTransform ChassisTransform = Chassis->GetComponentTransform();
	FVector WorldLocation = ChassisTransform.TransformPositionNoScale(FinalLocalLocation);
	FQuat WorldRotation = ChassisTransform.GetRotation() * FinalLocalRotation;

	InSolidAxleMesh->SetWorldLocationAndRotation(WorldLocation, WorldRotation, false, nullptr, ETeleportType::TeleportPhysics);
}

void UVehicleAxleAssemblyComponent::ApplySolidAxleStateDirect(float InExtensionRatio, float SteeringAngle)
{
	if (!LeftWheel.IsValid() || !RightWheel.IsValid())return;

	FVehicleSuspensionSimState LeftState;
	FVehicleSuspensionSimState RightState;
	LeftWheel->RoughlyInitializeSuspensionState(LeftState);
	RightWheel->RoughlyInitializeSuspensionState(RightState);

	const int32 Iteration = 1;
	for (int32 i = 0; i < Iteration; i++)
	{
		FVehicleSuspensionSimContext Ctx_L;
		FVehicleSuspensionSimContext Ctx_R;

		LeftWheel->StartApplySolidAxleStateDirect(InExtensionRatio, SteeringAngle, &LeftState, Ctx_L);
		RightWheel->StartApplySolidAxleStateDirect(InExtensionRatio, SteeringAngle, &RightState, Ctx_R);

		float TrackWidth = GetTrackWidth();
		float AxleHalfWidth = (
			TrackWidth
			- LeftWheel->GetSuspensionKinematicsConfig().HubOffsetFromLowerJoint.Y
			- RightWheel->GetSuspensionKinematicsConfig().HubOffsetFromLowerJoint.Y
			) * 0.5f;

		FVector3f LeftTopMount = LeftWheel->GetTopMountChassisLocation();
		FVector3f RightTopMount = RightWheel->GetTopMountChassisLocation();

		FVector3f AxleCenter = FVector3f::ZeroVector;
		FQuat4f AxleRotation = FQuat4f::Identity;

		float StrutRealLength_L = Ctx_L.StrutCurrentLength + LeftWheel->GetSuspensionKinematicsConfig().MinStrutLength;
		float StrutRealLength_R = Ctx_R.StrutCurrentLength + RightWheel->GetSuspensionKinematicsConfig().MinStrutLength;

		FVehicleSuspensionSolver::SolveSolidAxlePosture(
			LeftTopMount, RightTopMount,
			StrutRealLength_L, StrutRealLength_R,
			AxleHalfWidth,
			AxleCenter, AxleRotation
		);

		LeftWheel->FinalizeApplySolidAxleStateDirect(
			Ctx_L,
			AxleHalfWidth,
			AxleCenter, AxleRotation
		);
		RightWheel->FinalizeApplySolidAxleStateDirect(
			Ctx_R,
			AxleHalfWidth,
			AxleCenter, AxleRotation
		);

		LeftState = LeftWheel->GetSuspensionState(); 
		RightState = RightWheel->GetSuspensionState();
	}
}

void UVehicleAxleAssemblyComponent::ApplySuspensionStateDirect(float InExtensionRatio, float InSteeringAngle)
{
	switch (SuspensionType)
	{
	case EVehicleAxleSuspensionType::Independent:
		if (LeftWheel.IsValid())LeftWheel->ApplySuspensionStateDirect(InExtensionRatio, InSteeringAngle);
		if (RightWheel.IsValid())RightWheel->ApplySuspensionStateDirect(InExtensionRatio, InSteeringAngle);
		break;
	case EVehicleAxleSuspensionType::Solid:
		ApplySolidAxleStateDirect(InExtensionRatio, InSteeringAngle);
		break;
	default:
		if (LeftWheel.IsValid())LeftWheel->ApplySuspensionStateDirect(InExtensionRatio, InSteeringAngle);
		if (RightWheel.IsValid())RightWheel->ApplySuspensionStateDirect(InExtensionRatio, InSteeringAngle);
		break;
	}
}

void UVehicleAxleAssemblyComponent::GetLinearVelocity(FVector3f& OutLocalVelocity, FVector3f& OutWorldVelocity)
{
	OutLocalVelocity = GetStateForCaller().LocalLinearVelocity;
	OutWorldVelocity = GetStateForCaller().WorldLinearVelocity;
}

FVector3f UVehicleAxleAssemblyComponent::GetAxleCenter()
{
	UVehicleWheelComponent* WheelL = LeftWheel.Get();
	UVehicleWheelComponent* WheelR = RightWheel.Get();

	int32 n = (WheelL != nullptr) + (WheelR != nullptr);

	if (n == 2)
	{
		return 0.5f * (FVector3f)(WheelL->GetRelativeLocation() + WheelR->GetRelativeLocation());
	}
	else
	{
		if (WheelL)return (FVector3f)WheelL->GetRelativeLocation();
		if (WheelR)return (FVector3f)WheelR->GetRelativeLocation();
		return FVector3f(0.f);
	}
}

bool UVehicleAxleAssemblyComponent::IsWheelRequired(EWheelSide Side) const
{
	switch (AxleLayout)
	{
	case EVehicleAxleLayout::SingleLeft:
		return Side == EWheelSide::Left;
	case EVehicleAxleLayout::SingleRight:
		return Side == EWheelSide::Right;
	case EVehicleAxleLayout::TwoWheels:
	default:
		return true;
	}
}

FName UVehicleAxleAssemblyComponent::GetGeneratedWheelName(EWheelSide Side) const
{
	FString ThisName;
	GetName(ThisName);
	const TCHAR* Suffix = (Side == EWheelSide::Left) ? TEXT("_Wheel_L") : TEXT("_Wheel_R");
	return FName(ThisName + Suffix);
}

TWeakObjectPtr<UVehicleWheelComponent>& UVehicleAxleAssemblyComponent::WheelSlotRef(EWheelSide Side)
{
	if (Side == EWheelSide::Left) return LeftWheel;
	return RightWheel;
}

bool UVehicleAxleAssemblyComponent::IsWheelGenerated(EWheelSide Side) const
{
	return (Side == EWheelSide::Left) ? bLeftWheelGenerated : bRightWheelGenerated;
}

void UVehicleAxleAssemblyComponent::SetWheelGenerated(EWheelSide Side, bool bGenerated)
{
	if (Side == EWheelSide::Left) bLeftWheelGenerated = bGenerated;
	else bRightWheelGenerated = bGenerated;
}

bool UVehicleAxleAssemblyComponent::HasBothWheels() const
{
	return LeftWheel.IsValid() && RightWheel.IsValid();
}

void UVehicleAxleAssemblyComponent::EnsureAxleAttachedToChassis()
{
	UPrimitiveComponent* ChassisPtr = Chassis.Get();
	if (!ChassisPtr)
	{
		Chassis = UVehicleUtilities::FindPhysicalParent(this);
		ChassisPtr = Chassis.Get();
	}
	if (ChassisPtr && GetAttachParent() != ChassisPtr)
	{
		AttachToComponent(ChassisPtr, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void UVehicleAxleAssemblyComponent::PlaceWheel(EWheelSide Side, float TrackWidth)
{
	UVehicleWheelComponent* Wheel = WheelSlotRef(Side).Get();
	if (!Wheel) return;

	UPrimitiveComponent* ChassisPtr = Chassis.Get();
	if (ChassisPtr && Wheel->GetAttachParent() != ChassisPtr)
	{
		Wheel->AttachToComponent(ChassisPtr, FAttachmentTransformRules::KeepRelativeTransform);
	}

	const float SideSign = (Side == EWheelSide::Left) ? -1.f : 1.f;
	const FRotator SetupRotation = (Side == EWheelSide::Left)
		? FRotator(VehicleWheelComponentSetupRotation.Pitch,
			-VehicleWheelComponentSetupRotation.Yaw,
			-VehicleWheelComponentSetupRotation.Roll)
		: VehicleWheelComponentSetupRotation;
	const FQuat WheelCompRot = FQuat(SetupRotation).GetNormalized();

	FVector WheelCompPos = GetRelativeLocation();
	WheelCompPos.Y += SideSign * TrackWidth * 0.5f;
	Wheel->SetRelativeTransform(FTransform(WheelCompRot, WheelCompPos));
	Wheel->CacheDesignedHubTransform();

	const FVector DesignedHubLocalLocation = (FVector)Wheel->GetDesignedHubLocalTransform().GetLocation();
	const FVector HubOffset = WheelCompRot.RotateVector(DesignedHubLocalLocation);
	WheelCompPos -= HubOffset;
	Wheel->SetRelativeTransform(FTransform(WheelCompRot, WheelCompPos));
}

void UVehicleAxleAssemblyComponent::PlaceManagedWheels()
{
	EnsureAxleAttachedToChassis();
	// Manual wheels are positioned by their owner; only generated slots follow the axle.
	if (bUseExistingWheelComponent) return;
	PlaceWheel(EWheelSide::Left, AxleConfig.TrackWidth);
	PlaceWheel(EWheelSide::Right, AxleConfig.TrackWidth);
}

void UVehicleAxleAssemblyComponent::DestroyGeneratedWheel(EWheelSide Side)
{
	UVehicleWheelComponent* Wheel = WheelSlotRef(Side).Get();
	if (!Wheel) return;
	// Drop the bookkeeping entry before destruction so a dead weak pointer never lingers.
	AutoGeneratedComponents.RemoveAll([Wheel](const TWeakObjectPtr<UActorComponent>& Entry)
	{
		return Entry.Get() == Wheel;
	});
	// DestroyComponent keeps the object findable by name until GC. Free the deterministic
	// slot name first, otherwise a same-frame re-create would adopt the pending-kill object.
	const FName FreedName = MakeUniqueObjectName(Wheel->GetOuter(), Wheel->GetClass(),
		FName(Wheel->GetName() + TEXT("_Destroyed")));
	Wheel->Rename(*FreedName.ToString(), nullptr, REN_DontCreateRedirectors | REN_NonTransactional);
	Wheel->DestroyComponent();
	WheelSlotRef(Side).Reset();
	SetWheelGenerated(Side, false);
}

void UVehicleAxleAssemblyComponent::ResolveWheelSlot(EWheelSide Side)
{
	AActor* Owner = GetOwner();
	if (!IsValid(Owner)) return;

	TWeakObjectPtr<UVehicleWheelComponent>& Slot = WheelSlotRef(Side);

	if (bUseExistingWheelComponent)
	{
		// "Use existing" is a lookup only: this axle never creates or destroys these wheels.
		const FName ConfiguredName = (Side == EWheelSide::Left) ? LeftWheelComponentName : RightWheelComponentName;
		if (!ConfiguredName.IsNone())
		{
			Slot = UVehicleUtilities::GetComponentByName<UVehicleWheelComponent>(Owner, ConfiguredName);
		}
		SetWheelGenerated(Side, false);
		return;
	}

	// Generated slots use a deterministic name so a saved level converges on one wheel
	// per slot instead of accumulating a new component on every registration.
	const FName GeneratedName = GetGeneratedWheelName(Side);
	if (UVehicleWheelComponent* Existing = FindObject<UVehicleWheelComponent>(Owner, *GeneratedName.ToString()))
	{
		if (IsValid(Existing))
		{
			Slot = Existing;
			SetWheelGenerated(Side, false); // Pre-existing objects are adopted, not owned.
			return;
		}
	}

	if (UVehicleWheelComponent* Created = UVehicleUtilities::CreateComponentByClass<UVehicleWheelComponent>(
		Owner, nullptr, GeneratedName))
	{
		Slot = Created;
		SetWheelGenerated(Side, true);
		AutoGeneratedComponents.AddUnique(Created);
	}
}

void UVehicleAxleAssemblyComponent::ReconcileWheelSlots()
{
	for (EWheelSide Side : { EWheelSide::Left, EWheelSide::Right })
	{
		TWeakObjectPtr<UVehicleWheelComponent>& Slot = WheelSlotRef(Side);

		if (!Slot.IsValid())
		{
			Slot.Reset();
			SetWheelGenerated(Side, false);
		}

		if (!IsWheelRequired(Side))
		{
			// The layout no longer needs this side. Only a generated wheel is destroyed;
			// a referenced one stays in the world, it just stops being part of this axle.
			if (Slot.IsValid() && IsWheelGenerated(Side)) DestroyGeneratedWheel(Side);
			Slot.Reset();
			SetWheelGenerated(Side, false);
			continue;
		}

		const FName DesiredName = bUseExistingWheelComponent
			? ((Side == EWheelSide::Left) ? LeftWheelComponentName : RightWheelComponentName)
			: GetGeneratedWheelName(Side);
		// A live component is not necessarily the wheel requested by the current setup.
		// Retire owned wheels on a mode/name change; release external references without touching them.
		if (Slot.IsValid() && ((bUseExistingWheelComponent && IsWheelGenerated(Side))
			|| Slot->GetFName() != DesiredName || Slot->GetOwner() != GetOwner()))
		{
			if (IsWheelGenerated(Side)) DestroyGeneratedWheel(Side);
			else
			{
				Slot.Reset();
				SetWheelGenerated(Side, false);
			}
		}

		if (!Slot.IsValid()) ResolveWheelSlot(Side);
	}
}

bool UVehicleAxleAssemblyComponent::GenerateDifferential()
{
	AActor* Owner = GetOwner();
	//set differential
	if (!Differential.IsValid() && IsValid(Owner))
	{
		bool bExistingDiffFound = false;
		if (bUseExistingDifferentialComponent)
		{
			Differential = UVehicleUtilities::GetComponentByName<UVehicleDifferentialComponent>(Owner, DifferentialComponentName);
			bExistingDiffFound = Differential.IsValid();
		}

		// if not found, generate one
		if (!bExistingDiffFound)
		{
			FString ThisName = FString();
			GetName(ThisName);
			FName Name = FName(ThisName + "_Differential");
			Differential = FindObject<UVehicleDifferentialComponent>(Owner, *Name.ToString());
			if (!Differential.IsValid())
			{
				Differential = UVehicleUtilities::CreateComponentByClass<UVehicleDifferentialComponent>(
					Owner,
					DifferentialClass,
					Name
				);
				AutoGeneratedComponents.AddUnique(Differential);
			}
		}
	}

	return Differential.IsValid();
}

TArray<FName> UVehicleAxleAssemblyComponent::GetNamesOfWheelsOfOwner()
{
	return UVehicleUtilities::GetNamesOfComponentsOfActor<UVehicleWheelComponent>(this);
}

TArray<FName> UVehicleAxleAssemblyComponent::GetNamesOfDifferentialsOfOwner()
{
	return UVehicleUtilities::GetNamesOfComponentsOfActor<UVehicleDifferentialComponent>(this);
}
