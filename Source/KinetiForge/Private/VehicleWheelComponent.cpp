// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)


#include "VehicleWheelComponent.h"
#include "ProfilingDebugging/CpuProfilerTrace.h"
#include "VehicleWheelCoordinatorComponent.h"
#include "VehicleAxleAssemblyComponent.h"
#include "VehicleUtilities.h"
#include "VehicleSuspensionContactBridge.h"
#include "VehicleWheelRuntime.h"
#include "VehicleWheelPhysics.h"
#include "VehicleWheelBridge.h"
#include "VehicleAxleSolver.h"
#include "Misc/ScopeLock.h"

TSharedPtr<FVehicleWheelBridge, ESPMode::ThreadSafe> UVehicleWheelComponent::GetWheelBridge() const
{
	return WheelBridge;
}

bool UVehicleWheelComponent::BindIsolatedWheelRuntime()
{
	check(IsInGameThread());
	return IsRegistered() && !IsBeingDestroyed() && GetWheelBridge().IsValid();
}

const FVehicleWheelSimState& UVehicleWheelComponent::GetWheelStateForCaller() const
{
	return IsInGameThread() ? PublishedWheelState : GetWheelBridge()->GetRuntimeForSimulation().WheelState;
}

const FVehicleSuspensionSimState& UVehicleWheelComponent::GetSuspensionStateForCaller() const
{
	return IsInGameThread() ? PublishedSuspensionState : GetWheelBridge()->GetRuntimeForSimulation().SuspensionState;
}

const FVehicleSuspensionHitResult& UVehicleWheelComponent::GetContactResultForCaller() const
{
	return IsInGameThread() ? PublishedContactResult : GetWheelBridge()->GetContactForSimulation();
}

float UVehicleWheelComponent::GetPhysicsDeltaTimeInvForCaller() const
{
	return IsInGameThread() ? PublishedPhysicsDeltaTimeInv : GetWheelBridge()->GetRuntimeForSimulation().WheelContext.MacroDeltaTimeInv;
}

void UVehicleWheelComponent::SubmitWheelRuntimeSettings()
{
	if (TSharedPtr<FVehicleWheelBridge, ESPMode::ThreadSafe> Bridge = GetWheelBridge())
	{
		check(IsInGameThread());
		Bridge->SubmitSettings(CaptureRuntimeSettings());
	}
}

void UVehicleWheelComponent::SubmitWheelEnvironment()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForge_Wheel_SubmitEnvironment);
	if (TSharedPtr<FVehicleWheelBridge, ESPMode::ThreadSafe> Bridge = GetWheelBridge())
	{
		check(IsInGameThread());
		FVehicleWheelEnvironment Environment;
		Environment.ComponentRelativeTransform = GetRelativeTransform();
		Environment.DesignedHubLocalTransform = DesignedHubLocalTransform;
		Environment.WorldGravityZ = WorldGravityZ;
		Environment.World = GetWorld();
		Environment.QueryParams = CollisionQueryParams;
		Environment.ResponseParams = CollisionResponseParams;
		Bridge->SubmitEnvironment(MoveTemp(Environment));
	}
}

void UVehicleWheelComponent::SubmitSuspensionReset()
{
	if (TSharedPtr<FVehicleWheelBridge, ESPMode::ThreadSafe> Bridge = GetWheelBridge())
	{
		check(IsInGameThread());
		RequestedSprungMass = PublishedSuspensionState.StaticSprungMass;
		RequestedMassSequence = 0;
		RequestedSuspensionResetSequence = Bridge->RequestSuspensionReset(PublishedSuspensionState);
	}
}

void UVehicleWheelComponent::RefreshPublishedWheelState()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForge_Wheel_RefreshPublishedState);
	check(IsInGameThread());
	TSharedPtr<FVehicleWheelBridge, ESPMode::ThreadSafe> Bridge = GetWheelBridge();
	if (!Bridge) return;
	const FVehicleWheelResult Result = Bridge->ReadResult();
	if (Result.AppliedSuspensionResetSequence < RequestedSuspensionResetSequence) return;
	if (Result.PublicationSequence == LastWheelPublicationSequence) return;
	LastWheelPublicationSequence = Result.PublicationSequence;
	PrevLowerBallJointChassisLocation = Result.PreviousSuspensionState.LowerBallJointChassisLocation;
	PrevUpperBallJointChassisLocation = Result.PreviousSuspensionState.UpperBallJointChassisLocation;
	PrevHubChassisLocation = Result.PreviousSuspensionState.HubChassisLocation;
	PrevHubChassisRotation = Result.PreviousSuspensionState.HubChassisRotation;
	TimeSinceLastPhysicsTick = 0.f;
	PublishedWheelState = Result.WheelState;
	if (Result.AppliedP4Sequence < RequestedP4Sequence) PublishedWheelState.P4MotorTorque = RequestedP4MotorTorque;
	if (Result.AppliedEspSequence < RequestedEspSequence) PublishedWheelState.BrakeTorqueFromESP = RequestedEspBrakeTorque;
	PublishedSuspensionState = Result.SuspensionState;
	if (Result.AppliedMassSequence < RequestedMassSequence) PublishedSuspensionState.StaticSprungMass = RequestedSprungMass;
	PublishedContactResult = Bridge->ReadContactResult();
	ChassisAsyncWorldTransform = Result.ChassisWorldTransform;
	PublishedPhysicsDeltaTimeInv = UVehicleUtilities::SafeDivide(1.f, Result.MacroDeltaTime);
}

void UVehicleWheelComponent::PrepareSimulationStepInputs()
{
	if (TSharedPtr<FVehicleWheelBridge, ESPMode::ThreadSafe> Bridge = GetWheelBridge()) Bridge->BeginPhysicsStep();
}

FVehicleAxleWheelInput UVehicleWheelComponent::CaptureSimulationTransmissionInput()
{
	const FVehicleWheelRuntime& Runtime = GetWheelBridge()->GetRuntimeForSimulation();
	return { Runtime.WheelState.AngularVelocity, Runtime.WheelState.EffectiveInertia,
		Runtime.Settings.WheelConfig.Inertia, Runtime.WheelState.PredictedSlipRatio };
}

const FVehicleSuspensionSimState& UVehicleWheelComponent::GetSimulationSuspensionState()
{
	return GetWheelBridge()->GetRuntimeForSimulation().SuspensionState;
}

FTransform UVehicleWheelComponent::GetSimulationChassisTransform()
{
	return GetWheelBridge()->GetFrameForSimulation().ChassisWorldTransform;
}

FTransform UVehicleWheelComponent::GetSimulationRelativeTransform()
{
	return GetWheelBridge()->GetEnvironmentForSimulation().ComponentRelativeTransform;
}

FTransform3f UVehicleWheelComponent::GetSimulationDesignedHubTransform()
{
	return GetWheelBridge()->GetEnvironmentForSimulation().DesignedHubLocalTransform;
}

const FVehicleSuspensionKinematicsConfig& UVehicleWheelComponent::GetSimulationKinematicsConfig()
{
	return GetWheelBridge()->GetRuntimeForSimulation().Settings.KinematicsConfig;
}

FVector3f UVehicleWheelComponent::GetSimulationTopMountChassisLocation()
{
	return FVehicleSuspensionSolver::GetTopMountChassisLocation(GetSimulationKinematicsConfig(), FTransform3f(GetSimulationRelativeTransform()));
}

FVehicleWheelRuntimeSettings UVehicleWheelComponent::CaptureRuntimeSettings() const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForge_Wheel_CaptureRuntimeSettings);
	check(IsInGameThread());
	return FVehicleWheelRuntimeSettings::FromCachedConfig(WheelConfig, TireConfig, ABSConfig,
		SuspensionKinematicsConfig, SuspensionSpringConfig, CachedTireLUTs, CachedSuspensionLUTs);
}

// Sets default values for this component's properties
UVehicleWheelComponent::UVehicleWheelComponent()
{
	WheelBridge = MakeShared<FVehicleWheelBridge, ESPMode::ThreadSafe>(FVehicleWheelRuntime{});
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PrePhysics;

	// ...
	
	//load default wheel mesh
	if (!WheelMesh)
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultWheelMeshObj(
			TEXT("/Script/Engine.StaticMesh'/KinetiForge/DefaultConfigs/Meshes/DefaultWheel_235R33.DefaultWheel_235R33'")
		);
		if (DefaultWheelMeshObj.Succeeded())
		{
			WheelMesh = DefaultWheelMeshObj.Object;
		}
	}

	//load default curves
	if (!TireConfig.Fx)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultFx.DefaultFx'")
		);
		if (CurveObj.Succeeded())
		{
			TireConfig.Fx = CurveObj.Object;
		}
	}
	
	if (!TireConfig.Fy)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultFy.DefaultFy'")
		);
		if (CurveObj.Succeeded())
		{
			TireConfig.Fy = CurveObj.Object;
		}
	}

	if (!TireConfig.CamberToLateralDrift)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultCamberToLateralDrift.DefaultCamberToLateralDrift'")
		);
		if (CurveObj.Succeeded())
		{
			TireConfig.CamberToLateralDrift = CurveObj.Object;
		}
	}

#if 0
	if (!SuspensionKinematicsConfig.CamberCurve)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultCamberCurve.DefaultCamberCurve'")
		);
		if (CurveObj.Succeeded())
		{
			SuspensionKinematicsConfig.CamberCurve = CurveObj.Object;
		}
	}

	if (!SuspensionKinematicsConfig.AntiDiveCurve)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveObj(
				TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultAntiDiveCurve.DefaultAntiDiveCurve'")
		);
		if (CurveObj.Succeeded())
		{
			SuspensionKinematicsConfig.AntiDiveCurve = CurveObj.Object;
		}
	}

	if (!SuspensionKinematicsConfig.AntiSquatCurve)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultAntiSquatCurve.DefaultAntiSquatCurve'")
		);
		if (CurveObj.Succeeded())
		{
			SuspensionKinematicsConfig.AntiSquatCurve = CurveObj.Object;
		}
	}

	if (!SuspensionKinematicsConfig.AntiRollCurve)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultAntiRollCurve.DefaultAntiRollCurve'")
		);
		if (CurveObj.Succeeded())
		{
			SuspensionKinematicsConfig.AntiRollCurve = CurveObj.Object;
		}
	}
#endif
}

// Called when the game starts
void UVehicleWheelComponent::BeginPlay()
{
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::BeginPlay();

	// ...
	WheelCoordinator = UVehicleWheelCoordinatorComponent::FindWheelCoordinator(Chassis.Get());
	if (WheelCoordinator.IsValid())
	{
		WheelCoordinator->RegisterWheel(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WheelPhysics: WheelCoordinatorNotFound! Sprung mass can not be computed."));
	}

#if WITH_EDITOR
	TimeSinceLastConfigSync = FMath::FRandRange(0.f, ConfigSyncInterval);
	TimeSinceLastConfigSync += ConfigSyncInterval;
#endif
}

void UVehicleWheelComponent::OnRegister()
{
	bManualPhysicsReady.Store(false);
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::OnRegister();

	//...
	InitializeWheel();
	bManualPhysicsReady.Store(true);
}

void UVehicleWheelComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	bManualPhysicsReady.Store(false);
	PhysicsOwnership.InvalidateOwnerAndWait();
	UVehicleUtilities::DestroyAutoGeneratedComponents(AutoGeneratedComponents);

	if (WheelCoordinator.IsValid() && !WheelCoordinator->IsBeingDestroyed())
	{ 
		WheelCoordinator->NotifyWheelMoved(); 
	}

	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UVehicleWheelComponent::OnUnregister()
{
	bManualPhysicsReady.Store(false);
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::OnUnregister();
}

void UVehicleWheelComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	bManualPhysicsReady.Store(false);
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::EndPlay(EndPlayReason);
}

void UVehicleWheelComponent::BeginDestroy()
{
	bManualPhysicsReady.Store(false);
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::BeginDestroy();
}

bool UVehicleWheelComponent::InitializeMeshComponents()
{
	UObject* Outer = nullptr;
	if (AActor* Owner = GetOwner())Outer = Owner;

	if (!IsValid(Outer))return false;

	// add meshes
	FString ThisName = FString();
	GetName(ThisName);
	if (!WheelHubComponent.IsValid())
	{
		FName Name = FName(ThisName + "_WheelHub");
		WheelHubComponent = FindObject<USceneComponent>(Outer, *Name.ToString());
		if (!WheelHubComponent.IsValid())
		{
			WheelHubComponent = UVehicleUtilities::CreateComponentByClass<USceneComponent>(
				Outer,
				nullptr,
				Name
			);
			AutoGeneratedComponents.AddUnique(WheelHubComponent);
		}
	}
	if (!WheelMeshComponent.IsValid())
	{
		FName Name = FName(ThisName + "_WheelMesh");
		WheelMeshComponent = FindObject<UStaticMeshComponent>(Outer, *Name.ToString());
		if (!WheelMeshComponent.IsValid())
		{
			WheelMeshComponent = UVehicleUtilities::CreateComponentByClass<UStaticMeshComponent>(
				Outer,
				nullptr,
				Name
			);
			AutoGeneratedComponents.AddUnique(WheelMeshComponent);
		}
	}
	if (!BrakeMeshComponent.IsValid())
	{
		FName Name = FName(ThisName + "_BrakeMesh");
		BrakeMeshComponent = FindObject<UStaticMeshComponent>(Outer, *Name.ToString());
		if (!BrakeMeshComponent.IsValid())
		{
			BrakeMeshComponent = UVehicleUtilities::CreateComponentByClass<UStaticMeshComponent>(
				Outer,
				nullptr,
				Name
			);
			AutoGeneratedComponents.AddUnique(BrakeMeshComponent);
		}
	}

	if (USceneComponent* Hub = WheelHubComponent.Get())
	{
		if (Hub->GetAttachParent() != this)
			Hub->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

		if (UStaticMeshComponent* Mesh = WheelMeshComponent.Get())
		{
			if (Mesh->GetAttachParent() != Hub)
				Mesh->AttachToComponent(Hub, FAttachmentTransformRules::KeepRelativeTransform);
		}

		if (UStaticMeshComponent* Mesh = BrakeMeshComponent.Get())
		{
			if (Mesh->GetAttachParent() != Hub)
				Mesh->AttachToComponent(Hub, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}

	return RefreshWheelMesh();
}

void UVehicleWheelComponent::ApplyWheelForce(Chaos::FRigidBodyHandle_Internal* const ChassisHandle)
{
	const auto Bridge = GetWheelBridge();
	const FVehicleWheelRuntime& Runtime = Bridge->GetRuntimeForSimulation();
	FVehicleWheelPhysics::ApplyWheelForce(Runtime.WheelState, Runtime.WheelContext, Runtime.SuspensionState,
		Bridge->GetContactForSimulation(), Runtime.Settings.KinematicsConfig.LocationToApplyForce,
		Bridge->GetFrameForSimulation().ChassisWorldTransform, ChassisHandle);
}

void UVehicleWheelComponent::PreStepIndependentSuspension(
	const float InMacroDeltaTime, const float InSteeringAngle,
	const float ActiveSwaybarStiffness, const float OtherHubChassisZ)
{
	FVehicleWheelIndependentSuspensionStep Step;
	if (StartPreStepIndependentSuspension(InMacroDeltaTime, InSteeringAngle, Step))
		FinalizePreStepIndependentSuspension(ActiveSwaybarStiffness, OtherHubChassisZ, Step, false);
}

bool UVehicleWheelComponent::StartPreStepIndependentSuspension(
	const float InMacroDeltaTime, const float InSteeringAngle,
	FVehicleWheelIndependentSuspensionStep& OutStep)
{
	Chaos::FRigidBodyHandle_Internal* const ChassisHandle = UVehicleUtilities::GetInternalHandle(Chassis.Get());
	if (!ChassisHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("WheelPhysics: No Valid Chassis!!!"));
		return false;
	}
	const auto Bridge = GetWheelBridge();
	Bridge->BeginPhysicsStep();
	FVehicleWheelPhysicsFrame& Frame = Bridge->GetFrameForSimulation();
	Frame.MacroDeltaTime = InMacroDeltaTime;
	Frame.ChassisWorldTransform = Chaos::FParticleUtilitiesGT::GetActorWorldTransform(ChassisHandle);
	if (!Frame.ChassisWorldTransform.IsRotationNormalized()) Frame.ChassisWorldTransform.NormalizeRotation();
	Frame.ChassisState.FetchChassisPhysicsState(ChassisHandle, InMacroDeltaTime);
	const FVehicleWheelEnvironment& Environment = Bridge->GetEnvironmentForSimulation();
	OutStep.Frame = Frame;
	const FVehicleWheelRuntime& Runtime = Bridge->GetRuntimeForSimulation();
	FVehicleWheelPhysics::PrepareIndependentSuspension(Runtime.SuspensionState, OutStep.Context, OutStep.Frame,
		Runtime.Settings.WheelConfig, Runtime.Settings.KinematicsConfig, Environment.World,
		Environment.QueryParams, Environment.ResponseParams, InSteeringAngle);
	return true;
}

FVehicleSuspensionIntegrationInput UVehicleWheelComponent::GetPreparedIndependentSuspensionInput(
	FVehicleWheelIndependentSuspensionStep& Step)
{
	const FVehicleWheelRuntimeSettings& Settings = GetWheelBridge()->GetRuntimeForSimulation().Settings;
	return { Step.Context, Step.Frame.ChassisState, Step.Frame.WorldGravityZ, Settings.WheelConfig.Radius,
		Settings.WheelConfig.Inertia, Settings.KinematicsConfig, Settings.SpringConfig, Settings.SuspensionLUTs };
}

void UVehicleWheelComponent::FinalizePreStepIndependentSuspension(
	float ActiveSwaybarStiffness, float OtherHubChassisZ,
	FVehicleWheelIndependentSuspensionStep& Step, bool bAlreadyPairedAndGeometryPrepared)
{
	const auto Bridge = GetWheelBridge();
	FVehicleWheelRuntime& Runtime = Bridge->GetRuntimeForSimulation();
	const auto& Settings = Runtime.Settings;
	if (!bAlreadyPairedAndGeometryPrepared)
	{
		FVehicleSuspensionSolver::IntegrateSingleWheelSuspension(Step.Context,
			Step.Frame.ChassisState, Step.Frame.WorldGravityZ, Settings.WheelConfig.Radius,
			Settings.WheelConfig.Inertia, Settings.KinematicsConfig, Settings.SpringConfig,
			Settings.SuspensionLUTs, ActiveSwaybarStiffness, OtherHubChassisZ);
		FVehicleSuspensionSolver::PrepareIndependentSuspensionGeometry(Step.Context,
			Settings.WheelConfig.Radius, Settings.KinematicsConfig, Settings.SuspensionLUTs);
	}
	FVehicleWheelPhysics::FinalizePreparedIndependentSuspension(Step.Context, Runtime.SuspensionState,
		Runtime.WheelState, Settings.WheelConfig, Settings.KinematicsConfig, Settings.SpringConfig,
		Settings.SuspensionLUTs, Step.Frame, ActiveSwaybarStiffness, OtherHubChassisZ, Bridge->GetContactForSimulation());
}

void UVehicleWheelComponent::StartPreStepSolidAxleSuspension(
	FVehicleSuspensionSimContext& Ctx, const float InMacroDeltaTime, const float InSteeringAngle,
	const float ActiveSwaybarStiffness, const float OtherHubChassisZ)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForge_Wheel_UpdatePhysics);
	Chaos::FRigidBodyHandle_Internal* const ChassisHandle = UVehicleUtilities::GetInternalHandle(Chassis.Get());
	if (!ChassisHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("WheelPhysics: No Valid Chassis!!!"));
		return;
	}
	const auto Bridge = GetWheelBridge();
	Bridge->BeginPhysicsStep();
	FVehicleWheelPhysicsFrame& Frame = Bridge->GetFrameForSimulation();
	Frame.MacroDeltaTime = InMacroDeltaTime;
	Frame.ChassisWorldTransform = Chaos::FParticleUtilitiesGT::GetActorWorldTransform(ChassisHandle);
	Frame.ChassisState.FetchChassisPhysicsState(ChassisHandle, InMacroDeltaTime);
	const FVehicleWheelEnvironment& Environment = Bridge->GetEnvironmentForSimulation();
	FVehicleWheelPhysics::StartSolidAxleSuspension(Bridge->GetRuntimeForSimulation(), Ctx, Frame,
		Environment.World, Environment.QueryParams, Environment.ResponseParams,
		InSteeringAngle, ActiveSwaybarStiffness, OtherHubChassisZ);
}

void UVehicleWheelComponent::FinalizePreStepSolidAxleSuspension(
	FVehicleSuspensionSimContext& Ctx, const float InMacroDeltaTime,
	const float ActiveSwaybarStiffness, const float OtherHubChassisZ, const float AxleHalfWidth,
	const FVector3f& AxleChassisCenter, const FQuat4f& AxleChassisRotation)
{
	const auto Bridge = GetWheelBridge();
	FVehicleWheelPhysicsFrame& Frame = Bridge->GetFrameForSimulation();
	Frame.MacroDeltaTime = InMacroDeltaTime;
	FVehicleWheelPhysics::FinalizeSolidAxleSuspension(Bridge->GetRuntimeForSimulation(), Ctx, Frame,
		ActiveSwaybarStiffness, OtherHubChassisZ, AxleHalfWidth, AxleChassisCenter, AxleChassisRotation,
		Bridge->GetContactForSimulation());
}

void UVehicleWheelComponent::PreStepWheel(float InMacroDeltaTime)
{
	const auto Bridge = GetWheelBridge();
	Bridge->GetRuntimeForSimulation().PrepareWheel(InMacroDeltaTime, Bridge->GetFrameForSimulation().ChassisWorldTransform);
}

void UVehicleWheelComponent::SubStepWheel(float InSubstepDeltaTime, float InDriveTorque,
	float InBrakeTorque, float InHandbrakeTorque, float InReflectedInertia)
{
	GetWheelBridge()->GetRuntimeForSimulation().IntegrateWheel(InSubstepDeltaTime,
		InDriveTorque, InBrakeTorque, InHandbrakeTorque, InReflectedInertia);
}

void UVehicleWheelComponent::PostStepApplyForce()
{
	const auto Bridge = GetWheelBridge();
	Bridge->GetRuntimeForSimulation().FinalizeWheel();
	ApplyWheelForce(UVehicleUtilities::GetInternalHandle(Chassis.Get()));
	Bridge->PublishResult();
	// Manual GT/frozen-GT callers can read this step's result immediately after returning.
	if (IsInGameThread()) RefreshPublishedWheelState();
}

// Called every frame
void UVehicleWheelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForge_Wheel_GameThreadTick);
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	RefreshPublishedWheelState();

	// ...

#if WITH_EDITOR
	if (ConfigSyncInterval >= 0.f)
	{
		TimeSinceLastConfigSync += DeltaTime;
		if (TimeSinceLastConfigSync > ConfigSyncInterval)
		{
			TimeSinceLastConfigSync -= ConfigSyncInterval;

			// update cached curves
			UpdateCachedLookUpTables();
		}
	}
#endif
	
	// update animation
	if (bUpdateAnimAutomatically)
	{
		UpdateWheelAnim(DeltaTime, AnimWheelMaxRotationSpeed);
	}

	// cache gravity
	if (UWorld* World = GetWorld())
	{
		// cm to m and flip sign
		WorldGravityZ = World->GetGravityZ() * -0.01f;
	}
	else
	{
		WorldGravityZ = 9.81f;
	}
	SubmitWheelEnvironment();
	SubmitWheelRuntimeSettings();
}

void UVehicleWheelComponent::CopyWheelConfig(const UVehicleWheelComponent* Source, UVehicleWheelComponent* Target, bool bReInitialize)
{
	Target->WheelConfig = Source->WheelConfig;
	Target->TireConfig = Source->TireConfig;
	Target->ABSConfig = Source->ABSConfig;
	Target->SuspensionKinematicsConfig = Source->SuspensionKinematicsConfig;
	Target->SuspensionSpringConfig = Source->SuspensionSpringConfig;
	Target->bUpdateAnimAutomatically = Source->bUpdateAnimAutomatically;
	Target->AnimInterpSpeed = Source->AnimInterpSpeed;
	Target->AnimWheelMaxRotationSpeed = Source->AnimWheelMaxRotationSpeed;
	Target->WheelMesh = Source->WheelMesh;
	Target->WheelMeshTransform = Source->WheelMeshTransform;
	Target->BrakeMesh = Source->BrakeMesh;
	Target->BrakeMeshTransform = Source->BrakeMeshTransform;

	if (bReInitialize)
	{
		Target->InitializeWheel();
	}
	Target->SubmitWheelRuntimeSettings();
	Target->SubmitWheelEnvironment();
}

void UVehicleWheelComponent::InitializeWheel()
{
	Chassis = UVehicleUtilities::FindPhysicalParent(this);
	if (Chassis.IsValid() && Chassis != GetAttachParent())
	{
		AttachToComponent(Chassis.Get(), FAttachmentTransformRules::KeepWorldTransform);
	}
	if (!Chassis.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("WheelPhysics: Chassis Not Found!"));
	}

	//initialize meshes
	InitializeMeshComponents();

	FVehicleSuspensionSolver::CacheLUTs(CachedSuspensionLUTs, SuspensionKinematicsConfig);
	FVehicleWheelSolver::CacheLUTs(CachedTireLUTs, TireConfig);
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	CollisionQueryParams.bReturnPhysicalMaterial = true;
	CollisionQueryParams.bReturnFaceIndex = false;
	CollisionQueryParams.bTraceComplex = false;
	PublishedSuspensionState = FVehicleSuspensionSolver::SolveKinematicsAtExtension(
		WheelConfig.Radius, SuspensionKinematicsConfig, GetRelativeTransform(), 1.f, 0.f, 1);
	SubmitSuspensionReset();
	SubmitWheelRuntimeSettings();

	CacheDesignedHubTransform();

	// initialize animation
	UpdateWheelAnim();
	SubmitWheelEnvironment();
}

void UVehicleWheelComponent::CacheDesignedHubTransform(const float DesignedExtensionRatio)
{
	FVehicleSuspensionCachedLUTs TempLUTs;

	FVehicleSuspensionSolver::CacheLUTs(TempLUTs, SuspensionKinematicsConfig);

	FVehicleSuspensionSimState DesignedState = FVehicleSuspensionSolver::SolveKinematicsAtExtension(
		WheelConfig.Radius,
		SuspensionKinematicsConfig,
		GetRelativeTransform(),
		DesignedExtensionRatio,
		0.f,
		1,
		&TempLUTs
	);

	FTransform3f CompRelativeTrans3f = FTransform3f(GetRelativeTransform());

	DesignedHubLocalTransform = FTransform3f(
		CompRelativeTrans3f.InverseTransformRotation(DesignedState.HubChassisRotation),
		CompRelativeTrans3f.InverseTransformPositionNoScale(DesignedState.HubChassisLocation),
		FVector3f(1.f)
	);

	if (UVehicleWheelCoordinatorComponent* WheelCoord = WheelCoordinator.Get())
	{
		WheelCoordinator->NotifyWheelMoved();
	}
}

void UVehicleWheelComponent::SetWheelConfig(const FVehicleWheelConfig& NewConfig)
{
	WheelConfig = NewConfig;
	CacheDesignedHubTransform();
	SubmitWheelRuntimeSettings();
	SubmitWheelEnvironment();
}

void UVehicleWheelComponent::SetTireConfig(const FVehicleTireConfig& NewConfig)
{
	TireConfig = NewConfig;
	FVehicleWheelSolver::CacheLUTs(CachedTireLUTs, NewConfig);
	SubmitWheelRuntimeSettings();
}

void UVehicleWheelComponent::SetABSConfig(const FVehicleABSConfig& NewConfig)
{
	ABSConfig = NewConfig;
	SubmitWheelRuntimeSettings();
}

void UVehicleWheelComponent::SetSuspensionKinematicsConfig(const FVehicleSuspensionKinematicsConfig& NewConfig)
{
	SuspensionKinematicsConfig = NewConfig;
	FVehicleSuspensionSolver::CacheLUTs(CachedSuspensionLUTs, NewConfig);
	CacheDesignedHubTransform();
	SubmitWheelRuntimeSettings();
	SubmitWheelEnvironment();
}

void UVehicleWheelComponent::SetSuspensionSpringConfig(const FVehicleSuspensionSpringConfig& NewConfig)
{
	SuspensionSpringConfig = NewConfig;
	SubmitWheelRuntimeSettings();
}

void UVehicleWheelComponent::SetWheelMesh(UStaticMesh* NewMesh, const FTransform NewTransform, const bool bUseNewTransform)
{
	if (IsValid(NewMesh))
	{
		WheelMesh = NewMesh;
		WheelMeshComponent->SetStaticMesh(NewMesh);
	}
	if (bUseNewTransform)
	{
		WheelMeshTransform = NewTransform;
		WheelMeshComponent->SetRelativeTransform(NewTransform);
	}
}

void UVehicleWheelComponent::SetBrakeMesh(UStaticMesh* NewMesh, const FTransform NewTransform, const bool bUseNewTransform)
{
	if (IsValid(NewMesh))
	{
		BrakeMesh = NewMesh;
		BrakeMeshComponent->SetStaticMesh(NewMesh);
	}
	if (bUseNewTransform)
	{
		BrakeMeshTransform = NewTransform;
		BrakeMeshComponent->SetRelativeTransform(NewTransform);
	}
}

void UVehicleWheelComponent::SetSprungMass(float NewSprungMass)
{
	if (TSharedPtr<FVehicleWheelBridge, ESPMode::ThreadSafe> Bridge = GetWheelBridge())
	{
		if (IsInGameThread())
		{
			FVehicleSuspensionSolver::SetSprungMass(PublishedSuspensionState, NewSprungMass);
			RequestedSprungMass = PublishedSuspensionState.StaticSprungMass;
			RequestedMassSequence = Bridge->RequestSprungMass(NewSprungMass);
		}
		else Bridge->GetRuntimeForSimulation().SetSprungMass(NewSprungMass);
		return;
	}
}

void UVehicleWheelComponent::SetEspBrakeTorque(float NewTorque)
{
	if (TSharedPtr<FVehicleWheelBridge, ESPMode::ThreadSafe> Bridge = GetWheelBridge())
	{
		if (IsInGameThread())
		{
			RequestedEspBrakeTorque = PublishedWheelState.BrakeTorqueFromESP = NewTorque;
			RequestedEspSequence = Bridge->RequestEspBrakeTorque(NewTorque);
		}
		else Bridge->GetRuntimeForSimulation().WheelState.BrakeTorqueFromESP = NewTorque;
		return;
	}
}

void UVehicleWheelComponent::SetP4MotorTorque(float NewTorque)
{
	if (TSharedPtr<FVehicleWheelBridge, ESPMode::ThreadSafe> Bridge = GetWheelBridge())
	{
		if (IsInGameThread())
		{
			RequestedP4MotorTorque = PublishedWheelState.P4MotorTorque = NewTorque;
			RequestedP4Sequence = Bridge->RequestP4MotorTorque(NewTorque);
		}
		else Bridge->GetRuntimeForSimulation().WheelState.P4MotorTorque = NewTorque;
		return;
	}
}

FVector3f UVehicleWheelComponent::GetTopMountChassisLocation()
{
	return FVehicleSuspensionSolver::GetTopMountChassisLocation(
		SuspensionKinematicsConfig,
		FTransform3f(GetRelativeTransform())
	);
}

void UVehicleWheelComponent::GetLowerWishboneState(FVector& OutPivotChassisLocation, FVector& OutAxisChassisDirection, FVector& OutBallJointChassisLocation)
{
	FVector3f Pivot3f, Axis3f, BallJoint3f;

	FVehicleSuspensionSolver::GetLowerWishboneState(
		SuspensionKinematicsConfig,
		GetSuspensionStateForCaller(),
		FTransform3f(GetRelativeTransform()),
		Pivot3f,
		Axis3f,
		BallJoint3f
	);

	OutPivotChassisLocation = FVector(Pivot3f);
	OutAxisChassisDirection = FVector(Axis3f);
	OutBallJointChassisLocation = FVector(BallJoint3f);
}

void UVehicleWheelComponent::GetUpperWishboneState(FVector& OutPivotChassisLocation, FVector& OutAxisChassisDirection, FVector& OutBallJointChassisLocation)
{
	FVector3f Pivot3f, Axis3f, BallJoint3f;

	FVehicleSuspensionSolver::GetUpperWishboneState(
		SuspensionKinematicsConfig,
		GetSuspensionStateForCaller(),
		FTransform3f(GetRelativeTransform()),
		Pivot3f,
		Axis3f,
		BallJoint3f
	);

	OutPivotChassisLocation = FVector(Pivot3f);
	OutAxisChassisDirection = FVector(Axis3f);
	OutBallJointChassisLocation = FVector(BallJoint3f);
}

FTransform3f UVehicleWheelComponent::GetHubChassisTransform()
{
	return FTransform3f(
		GetSuspensionStateForCaller().HubChassisRotation,
		GetSuspensionStateForCaller().HubChassisLocation
	);
}

float UVehicleWheelComponent::GetSlipRatio(bool bTransientSlip)
{
	if (bTransientSlip)
	{
		return GetWheelStateForCaller().TransientSlip.X;
	}
	else
	{
		return GetWheelStateForCaller().SlipRatio;
	}
}

float UVehicleWheelComponent::GetSlipAngle(bool bTransientSlip)
{
	if (bTransientSlip)
	{
		return FMath::RadiansToDegrees(FMath::Atan(GetWheelStateForCaller().TransientSlip.Y));
	}
	else
	{
		return GetWheelStateForCaller().SlipAngle;
	}
}

float UVehicleWheelComponent::GetSkidIntensity(float LongitudinalScale, float LateralScale, float MaxSkidPowerThreshold)
{
	// Get actuall tire force
	float ForceX = GetWheelStateForCaller().TireForce.X;
	float ForceY = GetWheelStateForCaller().TireForce.Y;

	// Get kinematics slip velocity
	float SlipVelX = GetWheelStateForCaller().LongSlipVelocity;
	float SlipVelY = GetWheelStateForCaller().LatSlipVelocity;

	// Get friction power
	float PowerX = FMath::Abs(ForceX * SlipVelX * LongitudinalScale);
	float PowerY = FMath::Abs(ForceY * SlipVelY * LateralScale);

	// Get total power
	const float Bias = TireConfig.CombinedSlipBias;
	float TotalPower = (1.f - Bias) * PowerX + Bias * PowerY;

	float NormalizedSkid = TotalPower / FMath::Max(MaxSkidPowerThreshold, SMALL_NUMBER);
	return FMath::Clamp(NormalizedSkid, 0.f, 1.f);
}

float UVehicleWheelComponent::GetNormalizedSlip(float LongitudinalScale, float LateralScale)
{
	// get tire stiffness
	float Cx = CachedTireLUTs.Fx.LinearStiffness;
	float Cy = CachedTireLUTs.Fy.LinearStiffness;

	Cx = Cx > SMALL_NUMBER ? Cx : 1.f;
	Cy = Cy > SMALL_NUMBER ? Cy : 1.f;

	// get weight
	const float Bias = TireConfig.CombinedSlipBias;
	float Wx = (1.f - Bias) * Cx;
	float Wy = (Bias) * Cy;

	float NormFactor = 1.f / FMath::Max(FMath::Max(Wx, Wy), SMALL_NUMBER);

	// if the slip velocity is too low, should scale the affection of slip ratio, 0.5 is just magic number
	float SpeedScaleX = FMath::Clamp(0.5f * GetWheelStateForCaller().LongSlipVelocity, -1.f, 1.f);
	float NormalizedSx = GetWheelStateForCaller().TransientSlip.X * LongitudinalScale * SpeedScaleX * Wx * NormFactor;

	// also if the lateral velocity is too low, should scale the affection of slip angle
	float SpeedScaleY = FMath::Clamp(0.5f * GetWheelStateForCaller().LatSlipVelocity, -1.f, 1.f);
	float NormalizedSy = FMath::Atan(GetWheelStateForCaller().TransientSlip.Y) / (0.5f * PI) * LateralScale * SpeedScaleY * Wy * NormFactor;

	// combined slip
	float NormalizedSc = FMath::Sqrt(NormalizedSx * NormalizedSx + NormalizedSy * NormalizedSy);
	return FMath::Min(1.f, NormalizedSc);
}

bool UVehicleWheelComponent::CanUpdateManualPhysics(float DeltaTime) const
{
	if (!bManualPhysicsReady.Load() || !FMath::IsFinite(DeltaTime) || DeltaTime <= 0.f
		|| !IsRegistered() || IsBeingDestroyed() || PhysicsOwnership.HasActiveOwner()) return false;
	const AActor* Owner = GetOwner();
	if (Owner && Owner->IsActorBeingDestroyed()) return false;
	UPrimitiveComponent* Body = Chassis.Get();
	// Resolve a fresh handle for this invocation; never cache it across physics callbacks.
	return Body && Body->IsRegistered() && !Body->IsBeingDestroyed()
		&& UVehicleUtilities::GetInternalHandle(Body) != nullptr;
}

void UVehicleWheelComponent::UpdatePhysics(
	const float InPhysicsDeltaTime,
	const float InDriveTorque,
	const float InBrakeTorque,
	const float InHandbrakeTorque,
	const float InSteeringAngle,
	const float ActiveSwaybarStiffness,
	const float OtherHubChassisZ,
	const float InReflectedInertia)
{
	if (!CanUpdateManualPhysics(InPhysicsDeltaTime)) return; // Attached wheels use the owning assembly's stages.
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForge_Wheel_UpdatePhysics);

	PreStepIndependentSuspension(
		InPhysicsDeltaTime,
		InSteeringAngle,
		ActiveSwaybarStiffness,
		OtherHubChassisZ
	);

	PreStepWheel(
		InPhysicsDeltaTime
	);
	SubStepWheel(
		InPhysicsDeltaTime,
		InDriveTorque,
		InBrakeTorque,
		InHandbrakeTorque,
		InReflectedInertia
	);
	PostStepApplyForce();
}

bool UVehicleWheelComponent::CheckHasBeenMoved()
{
	FVector3f CurrLocation = (FVector3f)GetRelativeLocation();
	FQuat4f CurrRotation = (FQuat4f)GetRelativeTransform().GetRotation();
	bool bIsMoved = CachedRelativeLocation != CurrLocation || CachedRelativeRotation != CurrRotation;
	if (bIsMoved)
	{
		CachedRelativeLocation = CurrLocation;
		CachedRelativeRotation = CurrRotation;

		if (WheelCoordinator.IsValid())
		{
			WheelCoordinator->NotifyWheelMoved();
		}
		else
		{
			WheelCoordinator = UVehicleWheelCoordinatorComponent::FindWheelCoordinator(Chassis.Get());
			if (WheelCoordinator.IsValid())
			{
				WheelCoordinator->NotifyWheelMoved();
			}
		}
	}
	if (bIsMoved) SubmitWheelEnvironment();
	return bIsMoved;
}

void UVehicleWheelComponent::StartUpdateSolidAxlePhysics(
	const float InPhysicsDeltaTime,
	const float InSteeringAngle,
	const float ActiveSwaybarStiffness,
	const float OtherHubChassisZ,
	FVehicleSuspensionSimContext& Ctx
)
{
	if (!CanUpdateManualPhysics(InPhysicsDeltaTime)) return;
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForge_Wheel_UpdatePhysics);

	StartPreStepSolidAxleSuspension(
		Ctx,
		InPhysicsDeltaTime,
		InSteeringAngle,
		ActiveSwaybarStiffness,
		OtherHubChassisZ
	);
}

void UVehicleWheelComponent::FinalizeUpdateSolidAxlePhysics(
	const float InPhysicsDeltaTime,
	const float InDriveTorque,
	const float InBrakeTorque,
	const float InHandbrakeTorque,
	const float ActiveSwaybarStiffness,
	const float OtherHubChassisZ,
	const float InReflectedInertia,
	const float AxleHalfWidth,
	const FVector3f& AxleChassisCenter,
	const FQuat4f& AxleChassisRotation,
	FVehicleSuspensionSimContext& Ctx
)
{
	if (!CanUpdateManualPhysics(InPhysicsDeltaTime)) return;
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForge_Wheel_UpdatePhysics);

	FinalizePreStepSolidAxleSuspension(
		Ctx,
		InPhysicsDeltaTime,
		ActiveSwaybarStiffness,
		OtherHubChassisZ,
		AxleHalfWidth,
		AxleChassisCenter,
		AxleChassisRotation
	);

	PreStepWheel(
		InPhysicsDeltaTime
	);
	SubStepWheel(
		InPhysicsDeltaTime,
		InDriveTorque,
		InBrakeTorque,
		InHandbrakeTorque,
		InReflectedInertia
	);
	PostStepApplyForce();
}

void UVehicleWheelComponent::ApplySuspensionStateDirect(float InExtensionRatio, float InSteeringAngle)
{
	PublishedSuspensionState = FVehicleSuspensionSolver::SolveKinematicsAtExtension(
		WheelConfig.Radius,
		SuspensionKinematicsConfig,
		GetRelativeTransform(),
		InExtensionRatio, 
		InSteeringAngle,
		1);
	SubmitSuspensionReset();
	UpdateWheelAnim();
}

void UVehicleWheelComponent::RoughlyInitializeSuspensionState(FVehicleSuspensionSimState& InState)
{
	FVehicleSuspensionSolver::RoughlyInitializeState(
		GetRelativeTransform(),
		SuspensionKinematicsConfig,
		InState);
}

void UVehicleWheelComponent::StartApplySolidAxleStateDirect(
	float InExtensionRatio, 
	float InSteeringAngle,
	const FVehicleSuspensionSimState* PrevState,
	FVehicleSuspensionSimContext& Ctx)
{
	FVehicleSuspensionSolver::StartSolveSolidAxleAtExtension(
		PrevState,
		WheelConfig.Radius,
		SuspensionKinematicsConfig,
		GetRelativeTransform(),
		InExtensionRatio, 
		InSteeringAngle, 
		Ctx
	);
}

void UVehicleWheelComponent::FinalizeApplySolidAxleStateDirect(
	FVehicleSuspensionSimContext& Ctx,
	const float AxleHalfWidth,
	const FVector3f& AxleChassisCenter,
	const FQuat4f& AxleChassisRotation)
{
	PublishedSuspensionState = FVehicleSuspensionSolver::FinalizeSolveSolidAxleAtExtension(
		WheelConfig.Radius,
		SuspensionKinematicsConfig,
		Ctx,
		AxleHalfWidth,
		AxleChassisCenter,
		AxleChassisRotation
	);
	SubmitSuspensionReset();

	UpdateWheelAnim();
}

void UVehicleWheelComponent::GetWheelCoordinator(UVehicleWheelCoordinatorComponent*& OutWheelCoordinator)
{
	OutWheelCoordinator = WheelCoordinator.Get();
}

float UVehicleWheelComponent::ComputeFeedBackTorque()
{
	// get machanism trail
	FTransform3f ChassisTransform = FTransform3f(ChassisAsyncWorldTransform);
	FVector3f SteerAxis = ChassisTransform.TransformVector(GetSuspensionStateForCaller().SteerAxisChassisDirection);
	FVector3f Arm = WheelConfig.Radius * SteerAxis.ProjectOnToNormal(GetSuspensionStateForCaller().ImpactWorldNormal);
	FVector3f Torque = FVector3f::CrossProduct(Arm, GetWheelStateForCaller().TireForce);
	return FVector3f::DotProduct(SteerAxis, Torque);
}

FTransform UVehicleWheelComponent::GetChassisWorldTransform()
{
	return Chassis.Get() ? Chassis->GetComponentTransform() : FTransform();
}

void UVehicleWheelComponent::DrawSuspension(
	float Duration, float Thickness, 
	bool bDrawSuspension, bool bDrawWheel, bool bDrawRayCast,
	UVehicleAxleAssemblyComponent* ParentAxle)
{
	bool bSolidAxle = false;
	UVehicleWheelComponent* OtherWheel = nullptr;

	if (IsValid(ParentAxle))
	{
		UVehicleWheelComponent* WheelL = nullptr;
		UVehicleWheelComponent* WheelR = nullptr;
		ParentAxle->GetWheels(WheelL, WheelR);
		if (this == WheelL && this != WheelR)OtherWheel = WheelR;
		if (this == WheelR && this != WheelL)OtherWheel = WheelL;

		bSolidAxle = ParentAxle->GetAxleSuspensionType() == EVehicleAxleSuspensionType::Solid && IsValid(OtherWheel);
	}

	FVehicleSuspensionSolver::DrawSuspension(this, Duration, Thickness, bDrawSuspension, bDrawWheel, bDrawRayCast, bSolidAxle, OtherWheel);
}

void UVehicleWheelComponent::DrawSuspensionForce(float Duration, float Thickness, float Length)
{
	FVehicleSuspensionSolver::DrawSuspensionForce(this, Duration, Thickness, Length);
}

void UVehicleWheelComponent::DrawWheelForce(float Duration, float Thickness, float Length, bool bDrawVelocity, bool bDrawSlip, bool bDrawInertia)
{
	FVehicleWheelSolver::DrawWheelForce(this, GetSuspensionStateForCaller(), Duration, Thickness, Length, bDrawVelocity, bDrawSlip, bDrawInertia);
}

bool UVehicleWheelComponent::SetMesh(
	UStaticMesh* NewWheelMesh, 
	FTransform WheelMeshRelatvieTransform,
	UStaticMesh* NewBrakeMesh, 
	FTransform BrakeMeshRelativeTransform)
{
	if (!WheelHubComponent.IsValid() || !WheelMeshComponent.IsValid() || !BrakeMeshComponent.IsValid())return false;

	WheelHubComponent->PrimaryComponentTick.bCanEverTick = false;
	WheelHubComponent->PrimaryComponentTick.bStartWithTickEnabled = false;

	//if is left wheel
	if (GetRelativeLocation().Y < 0)
	{
		FTransform TempTrans = FTransform(FQuat(FRotator(0.f)), FVector(0.f), FVector(1.f, -1.f, 1.f));
		WheelMeshRelatvieTransform *= TempTrans;
		BrakeMeshRelativeTransform *= TempTrans;
	}

	if (NewWheelMesh)
	{
		WheelMeshComponent->SetStaticMesh(NewWheelMesh);
	}
	WheelMeshComponent->SetRelativeTransform(WheelMeshRelatvieTransform);
	WheelMeshComponent->SetGenerateOverlapEvents(false);
	WheelMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WheelMeshComponent->SetCanEverAffectNavigation(false);
	WheelMeshComponent->PrimaryComponentTick.bCanEverTick = false;
	WheelMeshComponent->PrimaryComponentTick.bStartWithTickEnabled = false;
	if (NewBrakeMesh)
	{
		BrakeMeshComponent->SetStaticMesh(NewBrakeMesh);
	}
	BrakeMeshComponent->SetRelativeTransform(BrakeMeshRelativeTransform);
	BrakeMeshComponent->SetGenerateOverlapEvents(false);
	BrakeMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BrakeMeshComponent->SetCanEverAffectNavigation(false);
	BrakeMeshComponent->PrimaryComponentTick.bCanEverTick = false;
	BrakeMeshComponent->PrimaryComponentTick.bStartWithTickEnabled = false;

	return WheelMeshComponent->GetStaticMesh() && BrakeMeshComponent->GetStaticMesh();
}

bool UVehicleWheelComponent::RefreshWheelMesh()
{
	return SetMesh(
		WheelMesh,
		WheelMeshTransform, 
		BrakeMesh, 
		BrakeMeshTransform);
}

void UVehicleWheelComponent::UpdateWheelAnim(float DeltaTime, float MaxAnimAngularVelocity)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForge_Wheel_UpdateAnimation);

	// update wheel position & alignment
	if (DeltaTime > 0)
	{
		TimeSinceLastPhysicsTick += DeltaTime;

		// blend between physics frames
		float Alpha = FMath::Clamp(TimeSinceLastPhysicsTick * GetPhysicsDeltaTimeInvForCaller(), 0.0f, 1.0f);
		FVector3f TargetAnimHubePos = FMath::Lerp(PrevHubChassisLocation, GetSuspensionStateForCaller().HubChassisLocation, Alpha);
		FQuat4f TargetAnimHubRot = FMath::Lerp(PrevHubChassisRotation, GetSuspensionStateForCaller().HubChassisRotation, Alpha);

		// interp
		AnimHubChassisLocation = (FVector3f)FMath::VInterpTo((FVector)AnimHubChassisLocation, (FVector)TargetAnimHubePos, DeltaTime, AnimInterpSpeed);
		AnimHubChassisRotation = (FQuat4f)FMath::QInterpTo((FQuat)AnimHubChassisRotation, (FQuat)TargetAnimHubRot, DeltaTime, AnimInterpSpeed);
	}
	else
	{
		TimeSinceLastPhysicsTick = 0.f;
		AnimHubChassisLocation = GetSuspensionStateForCaller().HubChassisLocation;
		AnimHubChassisRotation = GetSuspensionStateForCaller().HubChassisRotation;
	}

	FTransform T = FTransform(
		GetRelativeTransform().InverseTransformRotation((FQuat)AnimHubChassisRotation),
		GetRelativeTransform().InverseTransformPositionNoScale((FVector)AnimHubChassisLocation),
			FVector(1.f)
	);

	if (USceneComponent* Hub = WheelHubComponent.Get())
	{
		WheelHubComponent->SetRelativeTransform(
			T,
			false,
			nullptr,
			ETeleportType::TeleportPhysics
		);
	}
	
	// update wheel spin
	float Omega = (MaxAnimAngularVelocity > 0.f) ?
		FMath::Clamp(GetWheelStateForCaller().AngularVelocity, -MaxAnimAngularVelocity, MaxAnimAngularVelocity) :
		GetWheelStateForCaller().AngularVelocity;

	AnimWheelRotationAngle += FMath::RadiansToDegrees(Omega) * DeltaTime;
	AnimWheelRotationAngle = FMath::Fmod(AnimWheelRotationAngle, 360.0f);

	FQuat NewRot = FQuat(FVector(0.f, 1.f, 0.f), FMath::DegreesToRadians(AnimWheelRotationAngle));

	if (UStaticMeshComponent* Mesh = WheelMeshComponent.Get())
	{
		Mesh->SetRelativeRotation(
			NewRot,
			false,
			nullptr,
			ETeleportType::TeleportPhysics
		);
	}
}

FTransform UVehicleWheelComponent::GetSkidMarkWorldTransform(float InSkidMarkBias, float InSkidMarkScale)
{
	if (SuspensionKinematicsConfig.RayCastMode == EVehicleSuspensionRayCastMode::LineTrace)
	{
		// the impact point of line trace is always on the outer side of the wheel
		InSkidMarkBias -= WheelConfig.Width * 0.5f;
	}
	FVector N = (FVector)GetSuspensionStateForCaller().ImpactWorldNormal;
	FQuat q = FRotationMatrix::MakeFromZX(N, FVector(GetSuspensionStateForCaller().ImpactWorldVelocity)).ToQuat();
	FVector v = GetSuspensionStateForCaller().ImpactWorldLocation + N;	//to avoid spawned in the ground
	FVector rightV = FVector::VectorPlaneProject(FVector(GetSuspensionStateForCaller().WheelWorldRightVector), N).GetSafeNormal();
	v += rightV * InSkidMarkBias * (GetSuspensionStateForCaller().bIsRightWheel ? 1.f : -1.f);
	v += q.GetForwardVector() * -0.25 * InSkidMarkScale;	//-0.25 is for no reason
	FVector LocalLinVel = FVector((FVector2D)GetWheelStateForCaller().LocalLinearVelocity, 0.f);
	return FTransform(q, v, LocalLinVel);
}

FTransform UVehicleWheelComponent::UpdateWishboneAnim(
	USceneComponent* InArmMesh,
	const bool bFollowUpperWishbone,
	const FTransform InOffset,
	const FVector InMeshForwardVector,
	const FVector InMeshRightVector,
	const bool bScaleToMatchLength,
	const float MeshDesignLength
)
{
	const FTransform3f& TransformRelative = FTransform3f(GetRelativeTransform());

	FVector3f PivotChassis;
	FVector3f WishboneAxisChassis;
	FVector3f BallJointChassis;
	FVector3f WishboneDirectionChassis;
	float RealWishboneLength = 0.f;

	if (bFollowUpperWishbone)
	{
		FVehicleSuspensionSolver::GetUpperWishboneState(
			SuspensionKinematicsConfig,
			GetSuspensionStateForCaller(),
			TransformRelative,
			PivotChassis,
			WishboneAxisChassis,
			BallJointChassis
		);

		FVector3f PivotToBJ = BallJointChassis - PivotChassis;
		RealWishboneLength = PivotToBJ.Size();
		WishboneDirectionChassis = PivotToBJ.GetSafeNormal();
	}
	else
	{
		FVehicleSuspensionSolver::GetLowerWishboneState(
			SuspensionKinematicsConfig,
			GetSuspensionStateForCaller(),
			TransformRelative,
			PivotChassis,
			WishboneAxisChassis,
			BallJointChassis
		);

		FVector3f PivotToBJ = BallJointChassis - PivotChassis;
		RealWishboneLength = PivotToBJ.Size();
		WishboneDirectionChassis = PivotToBJ.GetSafeNormal();
	}

	// 1. Determine the target coordinate system (Target Basis)
	FVector3f TargetAxis = WishboneAxisChassis;
	FVector3f TargetDir = WishboneDirectionChassis;

	// Force orthogonality: Ensure that TargetDir and TargetAxis are strictly perpendicular 
	// (to prevent unnatural, non-uniform distortion in the swing arm model)
	TargetDir = TargetDir - FVector3f::DotProduct(TargetDir, TargetAxis) * TargetAxis;
	TargetDir.Normalize();

	// 2. Two-step quaternion rotation method
	// Step 1: First, align the mesh's Forward axis with the swing arm's pivot
	FQuat4f Q1 = FQuat4f::FindBetweenNormals(FVector3f(InMeshForwardVector), TargetAxis);

	// Step 2: After rotating the mesh's Right vector by Q1, 
	// it may not yet be aligned with the sphere's center.
	// We extract its current actual direction 
	// and then “twist” it around the rotation axis to align it with TargetDir.
	FVector3f RotatedMeshRight = Q1.RotateVector(FVector3f(InMeshRightVector));
	FQuat4f Q2 = FQuat4f::FindBetweenNormals(RotatedMeshRight, TargetDir);

	// Quaternion multiplication is read from right to left: 
	// Q1 is evaluated first, followed by Q2
	FQuat4f FinalRotation = Q2 * Q1;

	FVector3f FinalScale = FVector3f::OneVector;
	if (bScaleToMatchLength && MeshDesignLength > KINDA_SMALL_NUMBER)
	{
		float ScaleRatio = RealWishboneLength / MeshDesignLength;

		// Use GetAbs() to extract the specified direction.
		// For example, if InMeshRightVector is (0, 1, 0), GetAbs() also returns (0, 1, 0)
		// After adding (ScaleRatio - 1), Scale becomes (1, ScaleRatio, 1)
		FinalScale += FVector3f(InMeshRightVector).GetAbs() * (ScaleRatio - 1.0f);
	}

	FTransform3f BaseTransform(FinalRotation, PivotChassis, FinalScale);
	FTransform FinalTransform = InOffset * FTransform(BaseTransform);

	const FTransform& ChassisWorldTransform = Chassis.IsValid() ? Chassis->GetComponentTransform() : ChassisAsyncWorldTransform;
	if (InArmMesh)
	{
		InArmMesh->SetWorldTransform(FinalTransform * ChassisWorldTransform, false, nullptr, ETeleportType::TeleportPhysics);
	}

	return FinalTransform;
}

FTransform UVehicleWheelComponent::UpdateSuspensionArmAnim(USceneComponent* InArmMesh, const bool bFollowUpperWishbone, const FTransform InOffset, const FVector InMeshForwardVector, const FVector InMeshRightVector, const bool bScaleToMatchLength, const float MeshDesignLength)
{
	return UpdateWishboneAnim(
		InArmMesh,
		bFollowUpperWishbone,
		InOffset,
		InMeshForwardVector,
		InMeshRightVector,
		bScaleToMatchLength,
		MeshDesignLength
	);
}

void UVehicleWheelComponent::UpdateShockAbsorberAnim(USceneComponent* InUpperStrutMesh, USceneComponent* InLowerStrutMesh, USceneComponent* InSpringMesh, const float SpringDesignLength, const FVector InMeshUpVector)
{
	// 1. 获取物理塔顶和底座在 Chassis 空间下的坐标
	// (注意：对于双叉臂，底座可能在下摆臂上；对于麦弗逊，可能是转向节。这里统一用你 state 里存好的)
	FVector TopMountChassis = (FVector)GetSuspensionStateForCaller().TopMountChassisLocation;
	FVector LowerMountChassis = (FVector)GetSuspensionStateForCaller().LowerBallJointChassisLocation;

	// 2. 算出这条共享的“减震器绝对直线”
	FVector StrutVector = TopMountChassis - LowerMountChassis;
	float CurrentStrutLength = StrutVector.Size();
	FVector StrutDirectionUp = StrutVector.GetSafeNormal();   // 指向塔顶
	FVector StrutDirectionDown = -StrutDirectionUp;           // 指向地面

	// 3. 算出共同的旋转基准 (将模型默认的 UpVector 对齐到减震器轴线上)
	FQuat RotationLookingUp = FQuat::FindBetweenNormals(InMeshUpVector, StrutDirectionUp);
	FQuat RotationLookingDown = FQuat::FindBetweenNormals(InMeshUpVector, StrutDirectionDown);

	const FTransform& ChassisWorldTransform = Chassis.IsValid() ? Chassis->GetComponentTransform() : ChassisAsyncWorldTransform;

	// ==========================================
	// 组装 1：上半筒 (Upper Strut)
	// 原点在塔顶，不可压缩，方向朝下 (插入下半筒)
	// ==========================================
	if (InUpperStrutMesh)
	{
		FTransform UpperTransform(RotationLookingDown, TopMountChassis, FVector::OneVector);
		InUpperStrutMesh->SetWorldTransform(UpperTransform * ChassisWorldTransform, false, nullptr, ETeleportType::TeleportPhysics);
	}

	// ==========================================
	// 组装 2：下半筒 (Lower Strut)
	// 原点在底座，不可压缩，方向朝上 (套住上半筒)
	// ==========================================
	if (InLowerStrutMesh)
	{
		FTransform LowerTransform(RotationLookingUp, LowerMountChassis, FVector::OneVector);
		InLowerStrutMesh->SetWorldTransform(LowerTransform * ChassisWorldTransform, false, nullptr, ETeleportType::TeleportPhysics);
	}

	// ==========================================
	// 组装 3：弹簧 (Spring)
	// 原点在底座，方向朝上，并且 Z 轴需要根据行程动态缩放！
	// ==========================================
	if (InSpringMesh)
	{
		// 弹簧的当前长度，可能需要减去一些两端底座的厚度常数，这里简化为总长度
		// 如果你的弹簧只是减震器的一部分，你可以传入一个 SpringOffset 扣除掉不可压缩长度
		float ScaleZ = GetSuspensionStateForCaller().StrutCurrentLength / FMath::Max(0.1f, SpringDesignLength);

		FVector SpringScale = FVector::OneVector;
		// 同样的魔法：根据传入的轴向，只缩放那个轴
		SpringScale += InMeshUpVector.GetAbs() * (ScaleZ - 1.0f);

		FTransform SpringTransform(RotationLookingUp, LowerMountChassis, SpringScale);
		InSpringMesh->SetWorldTransform(SpringTransform * ChassisWorldTransform, false, nullptr, ETeleportType::TeleportPhysics);
	}
}

void UVehicleWheelComponent::UpdateSuspensionSpringAnim(USceneComponent* InUpperStrutMesh, USceneComponent* InLowerStrutMesh, USceneComponent* InSpringMesh, const float SpringDesignLength, const FVector InMeshUpVector)
{
	UpdateShockAbsorberAnim(
		InUpperStrutMesh,
		InLowerStrutMesh,
		InSpringMesh,
		SpringDesignLength,
		InMeshUpVector
	);
}

void UVehicleWheelComponent::OrientAndScaleToLocation(USceneComponent* InComponent, const FVector& TargetWorldLocation, const float UnscaledLength, const FVector TrackingAxisLocal)
{
	if (!InComponent) return;

	// 1. 获取当前世界坐标，计算目标方向和距离
	FVector StartLoc = InComponent->GetComponentLocation();
	FVector Dir = TargetWorldLocation - StartLoc;
	float Dist = Dir.Size();

	// 防呆：如果距离极短（比如重合了），直接跳过计算，防止除零崩溃
	if (Dist < KINDA_SMALL_NUMBER) return;

	FVector DirNormal = Dir / Dist;

	// 2. 【解决旋转】：计算需要补充的旋转量
	// 先把模型局部的 TrackingAxis 转换到当前的世界方向
	FVector CurrentTrackingWorld = InComponent->GetComponentQuat().RotateVector(TrackingAxisLocal).GetSafeNormal();

	// 计算从当前轴向旋转到目标方向的差值 Quat
	FQuat DeltaRot = FQuat::FindBetweenNormals(CurrentTrackingWorld, DirNormal);

	// 叠加到现有的旋转上
	InComponent->SetWorldRotation(DeltaRot * InComponent->GetComponentQuat(), false, nullptr, ETeleportType::TeleportPhysics);

	// 3. 【解决缩放】：沿指定轴向单向拉伸
	float ScaleFactor = Dist / FMath::Max(0.1f, UnscaledLength);

	// 假设模型基础缩放为 1，利用 GetAbs() 自动锁定到策划填写的那个轴上
	FVector NewScale = FVector::OneVector + TrackingAxisLocal.GetAbs() * (ScaleFactor - 1.0f);

	InComponent->SetWorldScale3D(NewScale);
}

void UVehicleWheelComponent::AttachComponentToWheelHub(
	USceneComponent* InComponent,
	bool bKeepWorldTransform)
{
	if (InComponent && WheelHubComponent.IsValid())
	{
		InComponent->AttachToComponent(
			WheelHubComponent.Get(),
			bKeepWorldTransform ? FAttachmentTransformRules::KeepWorldTransform : FAttachmentTransformRules::KeepRelativeTransform
		);
	}
}

void UVehicleWheelComponent::UpdateCachedLookUpTables()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForge_Wheel_UpdateCachedLookUpTables);
	FVehicleSuspensionSolver::CacheLUTs(CachedSuspensionLUTs, SuspensionKinematicsConfig);
	FVehicleWheelSolver::CacheLUTs(CachedTireLUTs, TireConfig);
	SubmitWheelRuntimeSettings();
}
