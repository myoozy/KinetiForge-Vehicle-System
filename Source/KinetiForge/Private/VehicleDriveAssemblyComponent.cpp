// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)


#include "VehicleDriveAssemblyComponent.h"
#include "ProfilingDebugging/CpuProfilerTrace.h"
#include "PhysicsEngine/BodyInstance.h"
#include "VehicleWheelComponent.h"
#include "VehicleDifferentialComponent.h"
#include "VehicleAxleAssemblyComponent.h"
#include "VehicleWheelCoordinatorComponent.h"
#include "VehicleEngineComponent.h"
#include "VehicleClutchComponent.h"
#include "VehicleGearboxComponent.h"
#include "VehicleAsyncTickComponent.h"
#include "VehicleUtilities.h"
#include "Net/UnrealNetwork.h" 
#include "GameFramework/SpringArmComponent.h"
#include "VehiclePowerUnitBridge.h"
#include "VehiclePhysicsStep.h"
#include "AsyncTickPhysicsExecution.h"
#include "PBDRigidsSolver.h"
#include "VehicleFixedDriveRuntime.h"
#include "KinetiForge.h"
#include "Modules/ModuleManager.h"
#include "Misc/ScopeLock.h"

// Sets default values for this component's properties
UVehicleDriveAssemblyComponent::UVehicleDriveAssemblyComponent()
{
	PhysicsStep = MakeShared<FVehiclePhysicsStep, ESPMode::ThreadSafe>(
		[](float) {});
	PhysicsStep->InvalidateBindingAndWait(); // No executable entry until a complete scene binding is published.
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PostPhysics;

	// ...
	SetIsReplicatedByDefault(true); // enable replication

	//load default curves
	if (!InputConfig.Steering.ResponseCurve)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> DefaultSteeringCurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultSteeringCurve.DefaultSteeringCurve'")
		);
		if (DefaultSteeringCurveObj.Succeeded())
		{
			InputConfig.Steering.ResponseCurve = DefaultSteeringCurveObj.Object;
		}
	}

	if (!InputConfig.HighSpeedSteeringScale)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> DefaultSteeringSpeedScaleCurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultHighspeedSteeringCurve.DefaultHighspeedSteeringCurve'")
		);
		if (DefaultSteeringSpeedScaleCurveObj.Succeeded())
		{
			InputConfig.HighSpeedSteeringScale = DefaultSteeringSpeedScaleCurveObj.Object;
		}
	}

	if (!AutoGearboxConfig.UpShiftRPMCurve)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> DefaultAutoGearboxInputCurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultAutoGearboxCurve.DefaultAutoGearboxCurve'")
		);
		if (DefaultAutoGearboxInputCurveObj.Succeeded())
		{
			AutoGearboxConfig.UpShiftRPMCurve = DefaultAutoGearboxInputCurveObj.Object;
		}
	}

	if (!AutoGearboxConfig.DownShiftRPMCurve)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> DefaultAutoGearboxInputCurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultAutoGearboxCurve.DefaultAutoGearboxCurve'")
		);
		if (DefaultAutoGearboxInputCurveObj.Succeeded())
		{
			AutoGearboxConfig.DownShiftRPMCurve = DefaultAutoGearboxInputCurveObj.Object;
		}
	}
}


// Called when the game starts
void UVehicleDriveAssemblyComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	WheelCoordinator = UVehicleWheelCoordinatorComponent::FindWheelCoordinator(Chassis.Get());
	if (WheelCoordinator.IsValid())WheelCoordinator->RegisterDriveAssembly(this);

	SearchExistingAxles();
	GeneratePowerUnit();
	bPowerUnitBridgeRequested = true;
	if (bPowerUnitBridgeRequested) InitializePowerUnitBridge();

	// Publish the selected backend before registering for asynchronous updates.
	VehicleAsyncTickComponent = UVehicleAsyncTickComponent::FindVehicleAsyncTickComponent(GetOwner());
	if (VehicleAsyncTickComponent.IsValid())VehicleAsyncTickComponent->RegisterDriveAssembly(this);

	//initialize autogearbox timer
	AutoGearboxCount = FMath::FRandRange(-AutoGearboxConfig.AutomaticGearboxRefreshTime, 0.0f) + AutoGearboxConfig.AutomaticGearboxRefreshTime;

}

void UVehicleDriveAssemblyComponent::OnRegister()
{
	const auto PreviousAsyncTickComponent = VehicleAsyncTickComponent;
	StopPhysicsUpdates();
	Super::OnRegister();
	//...
	Chassis = UVehicleUtilities::FindPhysicalParent(this);
	if (AActor* Owner = GetOwner())
	{
#if WITH_EDITOR
		Owner->bRunConstructionScriptOnDrag = false;
#endif
		Owner->SetReplicates(true);
	}
	GenerateAxles();
	PrepareIsolatedAxles();
	PreparePhysicsStepRegistration();
	if (HasBegunPlay() && PreviousAsyncTickComponent.IsValid())
	{
		PreviousAsyncTickComponent->RegisterDriveAssembly(this);
	}
}

TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe> UVehicleDriveAssemblyComponent::GetPhysicsStep() const
{
	FScopeLock Lock(&PhysicsStepMutex);
	return PhysicsStep;
}

bool UVehicleDriveAssemblyComponent::PrepareIsolatedAxles()
{
	check(IsInGameThread());
	SearchExistingAxles();
	for (const auto& Axle : Axles)
	{
		if (!Axle.IsValid()) continue;
		if (Axle->PhysicsOwnership.HasActiveOwner()) return false;
		UVehicleWheelComponent* Left = nullptr;
		UVehicleWheelComponent* Right = nullptr;
		Axle->GetWheels(Left, Right);
		for (auto* Wheel : {Left, Right}) if (Wheel && Wheel->PhysicsOwnership.HasActiveOwner()) return false;
	}
	for (const auto& Axle : Axles)
	{
		if (Axle.IsValid() && !Axle->BindIsolatedAxleRuntime()) return false;
	}
	return true;
}

TArray<FVehiclePhysicsBodyIdentity> UVehicleDriveAssemblyComponent::CaptureAssemblyBodyIdentities() const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_DriveAssembly_CaptureBodyIdentities);
	check(IsInGameThread());
	TArray<FVehiclePhysicsBodyIdentity> Identities;
	const auto Capture = [&Identities](UPrimitiveComponent* Body)
	{
		Identities.Add(FVehiclePhysicsBodyIdentity::Capture(Body ? Body->GetBodyInstance() : nullptr));
	};
	Capture(Chassis.Get());
	for (const auto& Axle : Axles)
	{
		if (!Axle.IsValid()) continue;
		UVehicleWheelComponent* Left = nullptr;
		UVehicleWheelComponent* Right = nullptr;
		Axle->GetWheels(Left, Right);
		for (auto* Wheel : {Left, Right}) if (Wheel) Capture(Wheel->Chassis.Get());
	}
	return Identities;
}

TSharedPtr<FVehicleFixedDriveRuntime, ESPMode::ThreadSafe> UVehicleDriveAssemblyComponent::CaptureFixedDriveRuntime()
{
	check(IsInGameThread());
	const auto Unit = GetPowerUnitBridge();
	if (!Unit || !VehicleAsyncTickComponent.IsValid()) return nullptr;
	if (!PrepareIsolatedAxles()) return nullptr;
	TArray<FVehiclePhysicsBodyIdentity> Identities = CaptureAssemblyBodyIdentities();
	for (const auto& Identity : Identities) if (!Identity.IsValid()) return nullptr;
	TArray<FVehicleAxlePhysicsAssembly> Assemblies;
	for (const auto& Axle : Axles)
	{
		if (!Axle.IsValid()) continue;
		const auto AxleBridge = Axle->GetAxleBridge();
		if (!AxleBridge) return nullptr; // Bind before BeginPlay; never fall back to a component consumer.
		UVehicleWheelComponent* Left = nullptr;
		UVehicleWheelComponent* Right = nullptr;
		Axle->GetWheels(Left, Right);
		auto CaptureWheel = [](UVehicleWheelComponent* Wheel)
		{
			FVehicleAxlePhysicsWheelBinding Binding;
			if (Wheel)
			{
				Binding.WheelBridge = Wheel->GetWheelBridge();
				UPrimitiveComponent* Body = Wheel->Chassis.Get();
				Binding.ChassisBodyIdentity = FVehiclePhysicsBodyIdentity::Capture(Body ? Body->GetBodyInstance() : nullptr);
			}
			return Binding;
		};
		if ((Left && !Left->GetWheelBridge()) || (Right && !Right->GetWheelBridge())) return nullptr;
		Assemblies.Emplace(AxleBridge, CaptureWheel(Left), CaptureWheel(Right));
	}
	BoundAssemblyBodies = Identities;
	return MakeShared<FVehicleFixedDriveRuntime, ESPMode::ThreadSafe>(Unit, Identities[0], MoveTemp(Assemblies));
}

bool UVehicleDriveAssemblyComponent::PreparePhysicsStepRegistration()
{
	check(IsInGameThread());
	const auto PreviousStep = GetPhysicsStep();
	TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe> Candidate;
	TSharedPtr<FVehicleFixedDriveRuntime, ESPMode::ThreadSafe> FixedRuntime;
	bool bFixedRuntimeReady = true;
	// Only reuse an entry actually published by the scene host. In particular,
	// an unregistered component's constructor entry is not a fixed-runtime binding.
	if (PreviousStep && !PreviousStep->IsStopped() && PreviousStep == ScenePhysicsStep) Candidate = PreviousStep;
	else
	{
		FixedRuntime = CaptureFixedDriveRuntime();
		bFixedRuntimeReady = FixedRuntime.IsValid();
		Candidate = MakeShared<FVehiclePhysicsStep, ESPMode::ThreadSafe>(
			[Runtime = FixedRuntime](float DeltaTime, const FVehiclePhysicsExecutionContext& Context)
			{ if (Runtime) Runtime->Update(DeltaTime, Context); });
	}
	TArray<UVehicleAxleAssemblyComponent*> ClaimedAxles;
	TArray<UVehicleWheelComponent*> ClaimedWheels;
	TArray<FVehiclePhysicsOwnership*> ClaimedPowerOwnerships;
	TArray<UVehicleDifferentialComponent*> DependentDifferentials;
	bool bCanClaim = true;
	bool bDependencyNotReady = false;
	const bool bAssemblyHasBegunPlay = HasBegunPlay();
	for (const auto& AxleRef : Axles)
	{
		UVehicleAxleAssemblyComponent* Axle = AxleRef.Get();
		if (!Axle) continue; // Destroyed dependencies are omitted; surviving single-wheel paths remain valid.
		if (!Axle->IsRegistered() || (bAssemblyHasBegunPlay && !Axle->HasBegunPlay()))
		{
			bCanClaim = false;
			bDependencyNotReady = true;
			break;
		}
		if (ClaimedAxles.Contains(Axle) || !Axle->PhysicsOwnership.CanClaim(Candidate))
		{
			bCanClaim = false;
			break;
		}
		ClaimedAxles.Add(Axle);
		UVehicleDifferentialComponent* AxleDifferential = nullptr;
		Axle->GetDifferential(AxleDifferential);
		if (AxleDifferential)
		{
			if (!AxleDifferential->IsRegistered() || (bAssemblyHasBegunPlay && !AxleDifferential->HasBegunPlay()))
			{
				bCanClaim = false;
				bDependencyNotReady = true;
				break;
			}
			DependentDifferentials.AddUnique(AxleDifferential);
		}
		UVehicleWheelComponent* Left = nullptr;
		UVehicleWheelComponent* Right = nullptr;
		Axle->GetWheels(Left, Right);
		for (UVehicleWheelComponent* Wheel : {Left, Right})
		{
			if (!Wheel) continue;
			if (!Wheel->IsRegistered() || (bAssemblyHasBegunPlay && !Wheel->HasBegunPlay()))
			{
				bCanClaim = false;
				bDependencyNotReady = true;
				break;
			}
			if (ClaimedWheels.Contains(Wheel) || !Wheel->PhysicsOwnership.CanClaim(Candidate))
			{
				bCanClaim = false;
				break;
			}
			ClaimedWheels.Add(Wheel);
		}
		if (!bCanClaim) break;
	}
	// Preview registration precedes GeneratePowerUnit/BeginPlay. Once play starts,
	// the complete primary power unit must remain registered before any entry can resume.
	const auto AssemblyBridge = GetPowerUnitBridge();
	auto PreflightPowerComponent = [&](auto* Component)
	{
		if (!Component) { bDependencyNotReady |= bAssemblyHasBegunPlay; return !bAssemblyHasBegunPlay; }
		if (!Component->IsRegistered() || (bAssemblyHasBegunPlay && !Component->HasBegunPlay()))
		{
			bDependencyNotReady = true;
			return false;
		}
		if (!Component->PhysicsOwnership.CanClaim(Candidate)) return false;
		if (!Component->CanBindPowerUnitBridge()
			&& (!AssemblyBridge || Component->PowerUnitBridge.Pin() != AssemblyBridge)) return false;
		ClaimedPowerOwnerships.Add(&Component->PhysicsOwnership);
		return true;
	};
	bCanClaim = bCanClaim && PreflightPowerComponent(Engine.Get())
		&& PreflightPowerComponent(Clutch.Get()) && PreflightPowerComponent(Gearbox.Get());
	if (UVehicleDifferentialComponent* CentralDifferential = TransferCase.Get())
	{
		if (!CentralDifferential->IsRegistered() || (bAssemblyHasBegunPlay && !CentralDifferential->HasBegunPlay()))
		{
			bCanClaim = false;
			bDependencyNotReady = true;
		}
		DependentDifferentials.AddUnique(CentralDifferential);
	}
	else if (bAssemblyHasBegunPlay) { bCanClaim = false; bDependencyNotReady = true; }
	if (!bFixedRuntimeReady) { bCanClaim = false; bDependencyNotReady = true; }
	if (AssemblyBridge)
	{
		// Re-registering the same components preserves owned simulation state.
		// Replacing a bound power component needs a separate quiescent bridge rebuild.
		bCanClaim &= Engine.Get() == BoundPowerUnitEngine.Get() && Clutch.Get() == BoundPowerUnitClutch.Get()
			&& Gearbox.Get() == BoundPowerUnitGearbox.Get();
	}
	if (!bCanClaim)
	{
		// No partial claims. Keep a dependency-invalidated entry available for a later GT retry.
		if (PreviousStep && !PreviousStep->IsStopped())
		{
			if (bDependencyNotReady) PreviousStep->InvalidateBindingAndWait();
			else PreviousStep->StopAndWait();
		}
		else if (bDependencyNotReady && (!PreviousStep || !PreviousStep->IsBindingInvalidated()))
		{
			// This explicit registration requests a new epoch even if its children are not ready.
			// Keep the old permanently stopped entry closed; only the fresh pending entry may retry.
			Candidate->InvalidateBindingAndWait();
			FScopeLock Lock(&PhysicsStepMutex);
			PhysicsStep = Candidate;
		}
		return false;
	}
	if (FixedRuntime)
	{
		UWorld* World = GetWorld();
		auto Host = World ? FModuleManager::GetModuleChecked<FKinetiForgeModule>(TEXT("KinetiForge"))
			.AcquirePhysicsScene(World->GetPhysicsScene()) : nullptr;
		// Register disabled; no solver callback may enter until all ownership claims have been published.
		auto RegisteredStep = Host ? Host->RegisterStep(
			[FixedRuntime](float DeltaTime, const FVehiclePhysicsExecutionContext& Context)
			{ FixedRuntime->Update(DeltaTime, Context); }, false,
			VehicleAsyncTickComponent->bUpdatePhysicsOnGameThread
				? EVehiclePhysicsExecutionLane::FrozenGameThread : EVehiclePhysicsExecutionLane::Worker) : nullptr;
		if (!RegisteredStep)
		{
			Candidate->InvalidateBindingAndWait();
			FScopeLock Lock(&PhysicsStepMutex);
			PhysicsStep = Candidate;
			return false;
		}
		if (PhysicsScene && ScenePhysicsStep) PhysicsScene->UnregisterStep(ScenePhysicsStep);
		PhysicsScene = MoveTemp(Host);
		ScenePhysicsStep = RegisteredStep;
		Candidate = MoveTemp(RegisteredStep);
	}
	for (auto* Axle : ClaimedAxles) Axle->PhysicsOwnership.Claim(Candidate);
	for (auto* Wheel : ClaimedWheels) Wheel->PhysicsOwnership.Claim(Candidate);
	for (auto* Ownership : ClaimedPowerOwnerships) Ownership->Claim(Candidate);
	for (auto* Differential : DependentDifferentials) Differential->AddPhysicsDependent(Candidate);
	if (PreviousStep && PreviousStep != Candidate) PreviousStep->InvalidateBindingAndWait();
	// A destroyed axle differential must not leave the isolated axle using its last parameter snapshot.
	for (auto* Axle : ClaimedAxles) Axle->SubmitRuntimeSettings();
	Candidate->SetAutomaticUpdateEnabled(bUpdatePhysicsAutomatically);
	{
		FScopeLock Lock(&PhysicsStepMutex);
		PhysicsStep = Candidate;
	}
	return true;
}

void UVehicleDriveAssemblyComponent::StopPhysicsUpdates()
{
	check(IsInGameThread());
	if (const auto Step = GetPhysicsStep()) Step->StopAndWait();
	if (PhysicsScene && ScenePhysicsStep) PhysicsScene->UnregisterStep(ScenePhysicsStep);
	ScenePhysicsStep.Reset();
	PhysicsScene.Reset();
	if (VehicleAsyncTickComponent.IsValid()) VehicleAsyncTickComponent->UnRegisterDriveAssembly(this);
}

void UVehicleDriveAssemblyComponent::RefreshInvalidatedPhysicsBinding()
{
	check(IsInGameThread());
	const auto Step = GetPhysicsStep();
	if (!Step || !Step->IsBindingInvalidated() || !IsRegistered() || !HasBegunPlay()) return;
	if (VehicleAsyncTickComponent.IsValid())
	{
		if (VehicleAsyncTickComponent->IsRegistered()) VehicleAsyncTickComponent->RegisterDriveAssembly(this);
	}
	else PreparePhysicsStepRegistration();
}

void UVehicleDriveAssemblyComponent::OnUnregister()
{
	StopPhysicsUpdates();
	Super::OnUnregister();
}

void UVehicleDriveAssemblyComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopPhysicsUpdates();
	Super::EndPlay(EndPlayReason);
}

void UVehicleDriveAssemblyComponent::BeginDestroy()
{
	// Also cover an unregistered component that never received BeginPlay/OnComponentDestroyed.
	StopPhysicsUpdates();
	Super::BeginDestroy();
}

void UVehicleDriveAssemblyComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	StopPhysicsUpdates();
	UVehicleUtilities::DestroyAutoGeneratedComponents(AutoGeneratedComponents);

	if (VehicleAsyncTickComponent.IsValid())
	{
		VehicleAsyncTickComponent->UnRegisterDriveAssembly(this);
	}
	//...
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UVehicleDriveAssemblyComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UVehicleDriveAssemblyComponent, ServerCurrentGear);
}

void UVehicleDriveAssemblyComponent::UpdateInput(float InDeltaTime)
{
	check(IsInGameThread());
	if (!IsInGameThread() || GetPowerUnitBridge()) return; // The simulation owns its pipeline; GT only submits commands and reads results.
	FVehicleInputSolver::Update(CaptureInputRuntimeSettings(), InputValues, CaptureInputStepContext(), InDeltaTime);
}

void UVehicleDriveAssemblyComponent::UpdateInputAxis(EVehicleInputAxis Axis, float InDeltaTime)
{
	check(IsInGameThread());
	if (!IsInGameThread() || GetPowerUnitBridge()) return;
	FVehicleInputSolver::UpdateAxis(CaptureInputRuntimeSettings(), InputValues, CaptureInputStepContext(), Axis, InDeltaTime);
}

void UVehicleDriveAssemblyComponent::UpdateThrottle(float InDeltaTime)
{
	UpdateInputAxis(EVehicleInputAxis::Throttle, InDeltaTime);
}

void UVehicleDriveAssemblyComponent::UpdateBrake(float InDeltaTime)
{
	UpdateInputAxis(EVehicleInputAxis::Brake, InDeltaTime);
}

void UVehicleDriveAssemblyComponent::UpdateClutch(float InDeltaTime)
{
	UpdateInputAxis(EVehicleInputAxis::Clutch, InDeltaTime);
}

void UVehicleDriveAssemblyComponent::UpdateSteering(float InDeltaTime)
{
	UpdateInputAxis(EVehicleInputAxis::Steering, InDeltaTime);
}

void UVehicleDriveAssemblyComponent::UpdateHandbrake(float InDeltaTime)
{
	UpdateInputAxis(EVehicleInputAxis::Handbrake, InDeltaTime);
}

FVehicleInputStepContext UVehicleDriveAssemblyComponent::CaptureInputStepContext() const
{
	check(IsInGameThread());
	FVehicleInputStepContext Context;
	Context.bHasGearbox = Gearbox.IsValid();
	Context.bHasEngine = Engine.IsValid();
	Context.GearboxConfig = GetGearboxConfigForInput();
	Context.GearboxState = GetGearboxStateForInput();
	Context.EngineRPM = GetEngineRPMForInput();
	Context.LocalLongitudinalVelocity = LocalLinearVelocity.X;
	Context.bIsInAir = bIsInAir;
	return Context;
}

void UVehicleDriveAssemblyComponent::UpdateAutomaticGearbox(float InDeltaTime)
{	
	APawn* p = Cast<APawn>(GetOwner());
	if (!p->IsLocallyControlled())return;

	AutoGearboxCount += InDeltaTime;
	//reset timer
	//保留误差
	bool AutoGearboxTimerOverFlowed = AutoGearboxCount > 0.f;
	AutoGearboxCount -= AutoGearboxConfig.AutomaticGearboxRefreshTime * AutoGearboxTimerOverFlowed;

	UVehicleGearboxComponent* GearboxRaw = Gearbox.Get();
	UVehicleEngineComponent* EngineRaw = Engine.Get();

	if (bIsInAir || 
		!GearboxRaw ||
		!EngineRaw ||
		!GearboxRaw->GetIsInGear() ||
		!AutoGearboxTimerOverFlowed ||
		(!GearboxRaw->GetConfig().NumberOfGears && !GearboxRaw->GetConfig().NumOfReverseGears)
		)return;

	float LinearVelocityX = FMath::Abs(LocalLinearVelocity.X);

	//arcade gearbox logic; automatically shift to N / D1 / R1 gear
	if (AutoGearboxConfig.bArcadeAutoGearbox)
	{
		//if in N gear
		if (GearboxRaw->GetCurrentGear() == 0)
		{
			if (InputValues.Raw.Throttle > SMALL_NUMBER)
			{
				ShiftToTargetGear(1, 0.f, AutoGearboxConfig.bArcadeShiftInstant);
				AutoGearboxCount += AutoGearboxConfig.AutomaticGearboxRefreshTime * AutoGearboxConfig.bArcadeShiftInstant;
				return;
			}
			else if (InputValues.Raw.Brake > SMALL_NUMBER && InputValues.Raw.Throttle <= SMALL_NUMBER)
			{
				ShiftToTargetGear(-1, 0.f, AutoGearboxConfig.bArcadeShiftInstant);
				AutoGearboxCount += AutoGearboxConfig.AutomaticGearboxRefreshTime * AutoGearboxConfig.bArcadeShiftInstant;
				return;
			}
		}
		//if the vehicle is almost stopped, or going in wrong direction
		else if (LinearVelocityX < 1 || (LocalLinearVelocity.X * GearboxRaw->GetCurrentGear()) < 0)
		{
			if (InputValues.Raw.Throttle > SMALL_NUMBER)
			{
				ShiftToTargetGear(1, 0.f, AutoGearboxConfig.bArcadeShiftInstant);
				AutoGearboxCount += AutoGearboxConfig.AutomaticGearboxRefreshTime * AutoGearboxConfig.bArcadeShiftInstant;
				return;
			}
			else if (InputValues.Raw.Brake > SMALL_NUMBER)
			{
				ShiftToTargetGear(-1, 0.f, AutoGearboxConfig.bArcadeShiftInstant);
				AutoGearboxCount += AutoGearboxConfig.AutomaticGearboxRefreshTime * AutoGearboxConfig.bArcadeShiftInstant;
				return;
			}
			else
			{
				ShiftToTargetGear(0, 0.f, AutoGearboxConfig.bArcadeShiftInstant);
				AutoGearboxCount += AutoGearboxConfig.AutomaticGearboxRefreshTime * AutoGearboxConfig.bArcadeShiftInstant;
				return;
			}
		}
	}

	//if in N gear, don't update
	if (!GearboxRaw->GetCurrentGear())return;

	TArray<UVehicleAxleAssemblyComponent*, TInlineAllocator<8>> AxlesRaw;

	for (TWeakObjectPtr<UVehicleAxleAssemblyComponent> AxlePtr : Axles)
	{
		if (UVehicleAxleAssemblyComponent* AxleRaw = AxlePtr.Get())
		{
			AxlesRaw.Add(AxleRaw);
		}
	}
	
	GearboxRaw->CalculateSpeedRangeOfEachGear(
		TransferCase->CalculateEffectiveWheelRadius_Internal(AxlesRaw),
		EngineRaw->GetIdleRPM(),
		EngineRaw->GetMaxRPM(),
		SpeedRangeOfEachGear);

	//如果急刹车时，也希望尽早降档，发动机协助制动
	float Input = FMath::Max(InputValues.Final.Throttle, InputValues.Final.Brake);

	float UpShiftTime = AutoGearboxConfig.UpShiftRPM;
	float DownShiftTime = AutoGearboxConfig.DownShiftRPM;

	//in sport mode, shift as late as it can
	if (!AutoGearboxConfig.bSportMode)
	{
		if (AutoGearboxConfig.UpShiftRPMCurve)
		{
			UpShiftTime *= AutoGearboxConfig.UpShiftRPMCurve->GetFloatValue(Input);
		}
		else
		{
			UpShiftTime *= Input;
		}
		if (AutoGearboxConfig.DownShiftRPMCurve)
		{
			DownShiftTime *= AutoGearboxConfig.DownShiftRPMCurve->GetFloatValue(Input);
		}
		else
		{
			DownShiftTime *= Input;
		}
	}

	// Upshift time should be greater than downshift
	DownShiftTime = FMath::Min(DownShiftTime, UpShiftTime);

	// should shift down if rpm is too low
	float MinDownShiftTime = UVehicleUtilities::SafeDivide(EngineRaw->GetIdleRPM(), EngineRaw->GetMaxRPM());
	DownShiftTime = FMath::Max(MinDownShiftTime, DownShiftTime);

	// Upshift time should be greater than downshift
	UpShiftTime = FMath::Max(UpShiftTime, DownShiftTime);

	int32 UnsignedCurrentGear = FMath::Abs(GearboxRaw->GetCurrentGear());
	int32 StartGear = FMath::Max(UnsignedCurrentGear - AutoGearboxConfig.MaxDownShiftSteps, 1);
	int32 EndGear = FMath::Min(UnsignedCurrentGear + AutoGearboxConfig.MaxUpShiftSteps,
		FMath::Max(GearboxRaw->GetConfig().NumberOfGears, GearboxRaw->GetConfig().NumOfReverseGears));
	
	int32 TargetGear = StartGear;
	float UnsignedSpeed = FMath::Abs(LocalLinearVelocity.X);

	// just loop through all possible gears (O(N))
	// there're just a few gears, we don't need any fancy algorithm
	for (int i = EndGear; i > StartGear; i--)
	{
		const bool GearingDown = UnsignedCurrentGear > i;
		const float ShiftTime = GearingDown ? DownShiftTime : UpShiftTime;

		if (UnsignedSpeed > ShiftTime * SpeedRangeOfEachGear[i - 1].Y)
		{
			TargetGear = i;
			break;
		}
	}

	bool GearingUp = UnsignedCurrentGear < TargetGear;

	//check sign
	if (LocalLinearVelocity.X < 0)TargetGear = -TargetGear;

	// 1. 目标档位不等于当前档位才能换挡
	// 2. 目标档位和当前档位方向相同才能换挡
	// 3. 松开刹车时才能升档
	if (GearboxRaw->GetCurrentGear() != TargetGear
		&& TargetGear * GearboxRaw->GetCurrentGear() > 0
		&& !(GearingUp && InputValues.Final.Brake > SMALL_NUMBER))
	{
		ShiftToTargetGear(TargetGear, 0.f);
		AutoGearboxCount -= AutoGearboxConfig.AutoShiftCoolDown;
	}
}

//RPC
void UVehicleDriveAssemblyComponent::ServerInputThrottle_Implementation(float InValue, bool bDirectInput)
{
	InValue = FMath::Clamp(InValue, 0.f, 1.f);
	ApplyInputCommand(EVehicleInputAxis::Throttle, InValue, bDirectInput);
	MultiCastInputThrottle(InValue, bDirectInput);
}

void UVehicleDriveAssemblyComponent::MultiCastInputThrottle_Implementation(float InValue, bool bDirectInput)
{
	InValue = FMath::Clamp(InValue, 0.f, 1.f);
	APawn* p = Cast<APawn>(GetOwner());
	if (p && !p->IsLocallyControlled() && !p->HasAuthority())
	{
		ApplyInputCommand(EVehicleInputAxis::Throttle, InValue, bDirectInput);
	}
}

void UVehicleDriveAssemblyComponent::ServerInputBrake_Implementation(float InValue, bool bDirectInput)
{
	InValue = FMath::Clamp(InValue, 0.f, 1.f);
	ApplyInputCommand(EVehicleInputAxis::Brake, InValue, bDirectInput);
	MultiCastInputBrake(InValue, bDirectInput);
}

void UVehicleDriveAssemblyComponent::MultiCastInputBrake_Implementation(float InValue, bool bDirectInput)
{
	InValue = FMath::Clamp(InValue, 0.f, 1.f);
	APawn* p = Cast<APawn>(GetOwner());
	if (p && !p->IsLocallyControlled() && !p->HasAuthority())
	{
		ApplyInputCommand(EVehicleInputAxis::Brake, InValue, bDirectInput);
	}
}

void UVehicleDriveAssemblyComponent::ServerInputClutch_Implementation(float InValue, bool bDirectInput)
{
	InValue = FMath::Clamp(InValue, 0.f, 1.f);
	ApplyInputCommand(EVehicleInputAxis::Clutch, InValue, bDirectInput);
	MultiCastInputClutch(InValue, bDirectInput);
}

void UVehicleDriveAssemblyComponent::MultiCastInputClutch_Implementation(float InValue, bool bDirectInput)
{
	InValue = FMath::Clamp(InValue, 0.f, 1.f);
	APawn* p = Cast<APawn>(GetOwner());
	if (p && !p->IsLocallyControlled() && !p->HasAuthority())
	{
		ApplyInputCommand(EVehicleInputAxis::Clutch, InValue, bDirectInput);
	}
}

void UVehicleDriveAssemblyComponent::ServerInputSteering_Implementation(float InValue, bool bDirectInput)
{
	InValue = FMath::Clamp(InValue, -1.f, 1.f);
	ApplyInputCommand(EVehicleInputAxis::Steering, InValue, bDirectInput);
	MultiCastInputSteering(InValue, bDirectInput);
}

void UVehicleDriveAssemblyComponent::MultiCastInputSteering_Implementation(float InValue, bool bDirectInput)
{
	InValue = FMath::Clamp(InValue, -1.f, 1.f);
	APawn* p = Cast<APawn>(GetOwner());
	if (p && !p->IsLocallyControlled() && !p->HasAuthority())
	{
		ApplyInputCommand(EVehicleInputAxis::Steering, InValue, bDirectInput);
	}
}

void UVehicleDriveAssemblyComponent::ServerInputHandbrake_Implementation(float InValue, bool bDirectInput)
{
	ApplyInputCommand(EVehicleInputAxis::Handbrake, InValue, bDirectInput);
	MultiCastInputHandbrake(InValue, bDirectInput);
}

void UVehicleDriveAssemblyComponent::MultiCastInputHandbrake_Implementation(float InValue, bool bDirectInput)
{
	APawn* p = Cast<APawn>(GetOwner());
	if (p && !p->IsLocallyControlled() && !p->HasAuthority())
	{
		ApplyInputCommand(EVehicleInputAxis::Handbrake, InValue, bDirectInput);
	}
}

void UVehicleDriveAssemblyComponent::ServerShiftToTargetGear_Implementation(int32 InTargetGear, bool bImmediate)
{
	//bind function to delegate
	FOnShiftFinishedDelegate Delegate;
	Delegate.BindUFunction(this, FName("ServerShiftFinishedCallback"));

	//shift to target gear on server
	Gearbox->ShiftToTargetGearWithDelegate(Delegate, InTargetGear, bImmediate);

	//multicast to every client
	MultiCastShiftToTargetGear(InTargetGear, bImmediate);
}

void UVehicleDriveAssemblyComponent::ServerShiftFinishedCallback_Implementation()
{
	//called after shift
	ServerCurrentGear = Gearbox->GetCurrentGear();
}

void UVehicleDriveAssemblyComponent::MultiCastShiftToTargetGear_Implementation(int32 InTargetGear, bool bImmediate)
{
	APawn* p = Cast<APawn>(GetOwner());
	if (!p->IsLocallyControlled() && !p->HasAuthority())
	{
		Gearbox->ShiftToTargetGear(InTargetGear, bImmediate);
	}
}

void UVehicleDriveAssemblyComponent::OnRep_ServerCurrentGear()
{
	if (Gearbox.IsValid())
	{
		//make sure the current gear equals to server current gear
		if (Gearbox->GetCurrentGear() != ServerCurrentGear)
		{
			Gearbox->ShiftToTargetGear(ServerCurrentGear, true);
		}
	}
}

void UVehicleDriveAssemblyComponent::ServerStartVehicleEngine_Implementation()
{
	ServerVehicleEngineOperationMode = Engine->StartVehicleEngine();
	MultiCastStartVehicleEngine();
}

void UVehicleDriveAssemblyComponent::MultiCastStartVehicleEngine_Implementation()
{
	APawn* p = Cast<APawn>(GetOwner());
	if (!p->IsLocallyControlled() && !p->HasAuthority())
	{
		Engine->StartVehicleEngine();
	}
}

void UVehicleDriveAssemblyComponent::ServerShutVehicleEngine_Implementation()
{
	ServerVehicleEngineOperationMode = Engine->ShutVehicleEngine();
	MultiCastShutVehicleEngine();
}

void UVehicleDriveAssemblyComponent::MultiCastShutVehicleEngine_Implementation()
{
	APawn* p = Cast<APawn>(GetOwner());
	if (!p->IsLocallyControlled() && !p->HasAuthority())
	{
		Engine->ShutVehicleEngine();
	}
}

void UVehicleDriveAssemblyComponent::OnRep_ServerVehicleEngineOperationMode()
{
	if (Engine->GetEngineState().OperationMode != ServerVehicleEngineOperationMode)
	{
		switch (ServerVehicleEngineOperationMode)
		{
		case EVehicleEngineOperationMode::On:
			Engine->StartVehicleEngine();
			break;
		case EVehicleEngineOperationMode::Off:
			Engine->ShutVehicleEngine();
			break;
		case EVehicleEngineOperationMode::Starting:
			Engine->StartVehicleEngine();
			break;
		case EVehicleEngineOperationMode::Shutting:
			Engine->ShutVehicleEngine();
			break;
		default:
			break;
		}
	}
}

// Called every frame
void UVehicleDriveAssemblyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_DriveAssembly_GameThreadTick);
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// A deferred bridge may become ready after child BeginPlay. Initialize it before
	// rebinding so the first eligible GT refresh can publish a complete scene entry.
	if (bPowerUnitBridgeRequested)
	{
		if (!GetPowerUnitBridge().IsValid() && !bPowerUnitBridgeInitializationFailed) InitializePowerUnitBridge();
		RefreshPowerUnitBridge();
	}
	if (BoundAssemblyBodies != CaptureAssemblyBodyIdentities())
	{
		if (const auto Step = GetPhysicsStep()) Step->InvalidateBindingAndWait();
	}
	RefreshInvalidatedPhysicsBinding();
	if (const auto Step = GetPhysicsStep()) Step->SetAutomaticUpdateEnabled(bUpdatePhysicsAutomatically);

	// ...
	if (Engine.IsValid() && Gearbox.IsValid() && Chassis.IsValid())
	{
		//update automatic gearbox
		if (AutoGearboxConfig.bAutomaticGearbox)
		{
			UpdateAutomaticGearbox(DeltaTime);
		}
	}
}

void UVehicleDriveAssemblyComponent::UpdatePhysics(float InDeltaTime)
{
	// Preserve the float-only Blueprint API, borrowing only the solver of the actual
	// callback dispatching this thread. Do not resolve a solver from an arbitrary GT call.
	if (const auto* Context = FVehiclePhysicsExecutionContext::GetCurrent())
	{
		UpdatePhysics(InDeltaTime, *Context);
		return;
	}
	if (auto* Solver = AsyncTickPhysics::GetCurrentSolver())
	{
		const FVehiclePhysicsExecutionContext Context(*static_cast<Chaos::FPBDRigidsSolver*>(Solver));
		UpdatePhysics(InDeltaTime, Context);
		return;
	}
	if (IsInGameThread()) RefreshInvalidatedPhysicsBinding(); // GT may prepare a new binding; executing it still requires a real callback context.
	if (const auto Step = GetPhysicsStep()) Step->TryExecute(InDeltaTime);
}

void UVehicleDriveAssemblyComponent::UpdatePhysics(float InDeltaTime, const FVehiclePhysicsExecutionContext& Context)
{
	if (const auto Step = GetPhysicsStep()) Step->TryExecute(InDeltaTime, Context);
}

void UVehicleDriveAssemblyComponent::InputThrottle(float InValue, bool bDirectInput)
{
	InValue = FMath::Clamp(InValue, 0.f, 1.f);
	if (!GetOwner()->HasAuthority())
	{
		ApplyInputCommand(EVehicleInputAxis::Throttle, InValue, bDirectInput);
	}
	ServerInputThrottle(InValue, bDirectInput);
}

void UVehicleDriveAssemblyComponent::InputBrake(float InValue, bool bDirectInput)
{
	InValue = FMath::Clamp(InValue, 0.f, 1.f);
	if (!GetOwner()->HasAuthority())
	{
		ApplyInputCommand(EVehicleInputAxis::Brake, InValue, bDirectInput);
	}
	ServerInputBrake(InValue, bDirectInput);
}

void UVehicleDriveAssemblyComponent::InputClutch(float InValue, bool bDirectInput)
{
	InValue = FMath::Clamp(InValue, 0.f, 1.f);
	if (!GetOwner()->HasAuthority())
	{
		ApplyInputCommand(EVehicleInputAxis::Clutch, InValue, bDirectInput);
	}
	ServerInputClutch(InValue, bDirectInput);
}

void UVehicleDriveAssemblyComponent::InputSteering(float InValue, bool bDirectInput)
{
	InValue = FMath::Clamp(InValue, -1.f, 1.f);
	if (!GetOwner()->HasAuthority())
	{
		ApplyInputCommand(EVehicleInputAxis::Steering, InValue, bDirectInput);
	}
	ServerInputSteering(InValue, bDirectInput);
}

void UVehicleDriveAssemblyComponent::InputHandbrake(float InValue, bool bDirectInput)
{
	if (!GetOwner()->HasAuthority())
	{
		ApplyInputCommand(EVehicleInputAxis::Handbrake, InValue, bDirectInput);
	}
	ServerInputHandbrake(InValue, bDirectInput);
}

void UVehicleDriveAssemblyComponent::ShiftToTargetGear(int32 InTargetGear, float InAutoShiftCoolDown, bool bImmediate)
{
	if (Gearbox.IsValid()) 
	{
		if (AutoGearboxConfig.bAutomaticGearbox)
		{
			AutoGearboxCount = -InAutoShiftCoolDown;
		}

		//Client predict
		if (!GetOwner()->HasAuthority())
		{
			Gearbox->ShiftToTargetGear(InTargetGear, bImmediate);
		}

		ServerShiftToTargetGear(InTargetGear, bImmediate);
	}
}

void UVehicleDriveAssemblyComponent::ShiftUp(float InAutoShiftCoolDown, bool bImmediate)
{
	if (Gearbox.IsValid())
	{
		ShiftToTargetGear(Gearbox->GetCurrentGear() + 1, InAutoShiftCoolDown, bImmediate);
	}
}

void UVehicleDriveAssemblyComponent::ShiftDown(float InAutoShiftCoolDown, bool bImmediate)
{
	if (Gearbox.IsValid())
	{
		ShiftToTargetGear(Gearbox->GetCurrentGear() - 1, InAutoShiftCoolDown, bImmediate);
	}
}

void UVehicleDriveAssemblyComponent::PauseAutoGearbox(float InCoolDownTime)
{
	if (AutoGearboxConfig.bAutomaticGearbox)
	{
		AutoGearboxCount = -InCoolDownTime;
	}
}

EVehicleEngineOperationMode UVehicleDriveAssemblyComponent::StartVehicleEngine()
{
	if (Engine.IsValid())
	{
		if (!GetOwner()->HasAuthority())
		{
			Engine->StartVehicleEngine();
		}

		ServerStartVehicleEngine();
		return ServerVehicleEngineOperationMode;
	}
	else
	{
		return EVehicleEngineOperationMode::Off;
	}
}

EVehicleEngineOperationMode UVehicleDriveAssemblyComponent::ShutVehicleEngine()
{
	if (Engine.IsValid())
	{
		if (!GetOwner()->HasAuthority())
		{
			Engine->ShutVehicleEngine();
		}

		ServerShutVehicleEngine();
		return ServerVehicleEngineOperationMode;
	}
	else
	{
		return EVehicleEngineOperationMode::On;
	}
}

void UVehicleDriveAssemblyComponent::StretchSpringArmBySpeed(
	USpringArmComponent* InSpringArm, 
	float InInitialSpringArmLength, 
	UCurveFloat* InScaleCurve)
{
	if (IsValid(InSpringArm) && IsValid(InScaleCurve))
	{
		float Scale = InScaleCurve->GetFloatValue(FMath::Abs(LocalVelocityClamped.X));
		float L = InInitialSpringArmLength * Scale;
		InSpringArm->TargetArmLength = L;
	}
}

void UVehicleDriveAssemblyComponent::CameraLookAtVelocity(
	USceneComponent* InSpringArm, 
	float InPitch, 
	float InSensitivity, 
	float InInterpSpeed, 
	FVector2D InStartSpeed_meter_per_second)
{
	if (IsValid(InSpringArm))
	{
		FVector2f LerpSpeedRange = (FVector2f)InStartSpeed_meter_per_second;
		FVector2f LocalVelocity2D = FVector2f(LocalLinearVelocity.X, LocalLinearVelocity.Y);
		float CurrSpeed = LocalVelocity2D.Length();
		float Alpha = FMath::GetMappedRangeValueClamped(
			LerpSpeedRange,
			FVector2f(0.f, 1.f),
			CurrSpeed
		);

		FRotator CurrentCamRot = InSpringArm->GetRelativeRotation();
		FQuat CurrCamQuat = CurrentCamRot.Quaternion();

		FVector ScaledLinearVelocity = (FVector)LocalVelocityClamped;
		ScaledLinearVelocity.Y *= InSensitivity;

		FRotator TargetRotator = FRotationMatrix::MakeFromX(ScaledLinearVelocity).Rotator();
		TargetRotator.Pitch = InPitch;

		// remain rotation if speed too low
		FQuat TargetQuat = FMath::Lerp(CurrCamQuat, TargetRotator.Quaternion(), Alpha);

		FRotator NewRot = FMath::QInterpTo(CurrCamQuat, TargetQuat, GetWorld()->DeltaTimeSeconds, InInterpSpeed).Rotator();
		NewRot.Roll = 0;

		InSpringArm->SetRelativeRotation(NewRot);
	}
}

void UVehicleDriveAssemblyComponent::GetAxles(TArray<UVehicleAxleAssemblyComponent*>& OutAxles)
{
	OutAxles.Reset();
	for (TWeakObjectPtr<UVehicleAxleAssemblyComponent> AxlePtr : Axles)
	{
		if (UVehicleAxleAssemblyComponent* AxleRaw = AxlePtr.Get())
		{
			OutAxles.Add(AxleRaw);
		}
	}
}

TArray<UVehicleAxleAssemblyComponent*> UVehicleDriveAssemblyComponent::GetAxles()
{
	TArray<UVehicleAxleAssemblyComponent*> Result;
	GetAxles(Result);
	return Result;
}

TArray<TWeakObjectPtr<UVehicleAxleAssemblyComponent>>& UVehicleDriveAssemblyComponent::GetAxlesRef()
{
	return Axles;
}

void UVehicleDriveAssemblyComponent::GetWheels(TArray<UVehicleWheelComponent*>& OutWheels)
{
	OutWheels.Reset();
	for (TWeakObjectPtr<UVehicleAxleAssemblyComponent> Axle : Axles)
	{
		if (!Axle.IsValid())continue;

		UVehicleWheelComponent* LeftWheel;
		UVehicleWheelComponent* RightWheel;

		Axle->GetWheels(LeftWheel, RightWheel);

		OutWheels.Add(LeftWheel);
		OutWheels.Add(RightWheel);
	}
}

TArray<UVehicleWheelComponent*> UVehicleDriveAssemblyComponent::GetWheels()
{
	TArray<UVehicleWheelComponent*> Result;
	GetWheels(Result);
	return Result;
}

void UVehicleDriveAssemblyComponent::DestroyTargetAxle(UVehicleAxleAssemblyComponent* InTargetAxle)
{
	Axles.Remove(InTargetAxle);
	InTargetAxle->DestroyComponent();
}

float UVehicleDriveAssemblyComponent::GetMaxTrackWidth()
{
	if (UVehicleWheelCoordinatorComponent* WheelCoord = WheelCoordinator.Get())
	{
		return WheelCoord->GetMaxTrackWidth();
	}
	return 0.f;
}

float UVehicleDriveAssemblyComponent::GetMaxWheelBase()
{
	if (UVehicleWheelCoordinatorComponent* WheelCoord = WheelCoordinator.Get())
	{
		return WheelCoord->GetMaxWheelBase();
	}
	return 0.f;
}

void UVehicleDriveAssemblyComponent:: GetVehicleBound(
	FVector& OutExtent, 
	FVector& OutOrigin, 
	FTransform& OutTransform,
	bool bReturnWorldTransform
)
{
	if (UPrimitiveComponent* Prim = Chassis.Get())
	{
		FBoxSphereBounds CollisionBounds = Prim->CalcBounds(FTransform::Identity);
		OutExtent = CollisionBounds.BoxExtent; // HalfSize
		OutOrigin = CollisionBounds.Origin;

		FTransform CompTransform = bReturnWorldTransform ?
			Prim->GetComponentTransform() : Prim->GetRelativeTransform();

		OutTransform = FTransform(
			CompTransform.GetRotation(),
			CompTransform.TransformPosition(CollisionBounds.Origin),
			CompTransform.GetScale3D()
		);
	}
}

float UVehicleDriveAssemblyComponent::GetVehicleForwardSpeed()
{
	return LocalLinearVelocity.X;
}

int32 UVehicleDriveAssemblyComponent::GetCurrentGear()
{
	return Gearbox.IsValid() ? Gearbox->GetCurrentGear() : 0;
}

float UVehicleDriveAssemblyComponent::GetSteeringValueFromAxles()
{
	float Sum = 0.f;
	float Num = 0.f;

	for (TWeakObjectPtr<UVehicleAxleAssemblyComponent> AxlePtr : Axles)
	{
		if (UVehicleAxleAssemblyComponent* Axle = AxlePtr.Get())
		{
			if (Axle->GetAxleSteeringConfig().bAffectedBySteering)
			{
				Num += 1.f;
				Sum += Axle->GetSteeringValue();
			}
		}
	}

	return UVehicleUtilities::SafeDivide(Sum, Num);
}

float UVehicleDriveAssemblyComponent::GetMinTurningRadius()
{
	const float cm2m = 0.01f;

	// Calculate the vehicle's maximum curvature
	float MaxCurvature = 0.f;
	for (TWeakObjectPtr<UVehicleAxleAssemblyComponent> AxlePtr : Axles)
	{
		if (UVehicleAxleAssemblyComponent* Axle = AxlePtr.Get())
		{
			if (Axle->GetAxleSteeringConfig().bAffectedBySteering)
			{
				float SteeringRad = FMath::DegreesToRadians(Axle->GetAxleSteeringConfig().MaxSteeringAngle);
				float AxleWheelBase_m = Axle->GetWheelBase() * cm2m;
				MaxCurvature += FMath::Abs(UVehicleUtilities::SafeDivide(FMath::Tan(SteeringRad), AxleWheelBase_m));
			}
		}
	}
	return UVehicleUtilities::SafeDivide(1.f, MaxCurvature);
}

float UVehicleDriveAssemblyComponent::GetMaxSteeringAngle()
{
	// Obtain the vehicle's maximum steering angle (for mapping to SteeringInput)
	float MaxSteeringAngle = 0.f;
	for (TWeakObjectPtr<UVehicleAxleAssemblyComponent> AxlePtr : Axles)
	{
		if (UVehicleAxleAssemblyComponent* Axle = AxlePtr.Get())
		{
			if (Axle->GetAxleSteeringConfig().bAffectedBySteering)
			{
				MaxSteeringAngle = FMath::Max(FMath::Abs(Axle->GetAxleSteeringConfig().MaxSteeringAngle), MaxSteeringAngle);
			}
		}
	}
	return MaxSteeringAngle;
}

TArray<AActor*> UVehicleDriveAssemblyComponent::GetRayCastHitActors()
{
	TArray<AActor*> HitActors;
	for (TWeakObjectPtr<UVehicleAxleAssemblyComponent>AxlePtr : Axles)
	{
		if (UVehicleAxleAssemblyComponent* Axle = AxlePtr.Get())
		{
			UVehicleWheelComponent* WheelL = nullptr;
			UVehicleWheelComponent* WheelR = nullptr;
			Axle->GetWheels(WheelL, WheelR);
			if (WheelL)
			{
				if (UPrimitiveComponent* HitComponent = WheelL->GetRayCastHitComponent())
				{
					if (AActor* HitActor = HitComponent->GetOwner())
					{
						HitActors.AddUnique(HitActor);
					}
				}
			}
			if (WheelR)
			{
				if (UPrimitiveComponent* HitComponent = WheelR->GetRayCastHitComponent())
				{
					if (AActor* HitActor = HitComponent->GetOwner())
					{
						HitActors.AddUnique(HitActor);
					}
				}
			}
		}
	}
	return HitActors;
}

TArray<UPrimitiveComponent*> UVehicleDriveAssemblyComponent::GetRayCastHitComponents()
{

	TArray<UPrimitiveComponent*> HitComps;
	for (TWeakObjectPtr<UVehicleAxleAssemblyComponent>AxlePtr : Axles)
	{
		if (UVehicleAxleAssemblyComponent* Axle = AxlePtr.Get())
		{
			UVehicleWheelComponent* WheelL = nullptr;
			UVehicleWheelComponent* WheelR = nullptr;
			Axle->GetWheels(WheelL, WheelR);
			if (WheelL)
			{
				if (UPrimitiveComponent* HitComponent = WheelL->GetRayCastHitComponent())
				{
					HitComps.AddUnique(HitComponent);
				}
			}
			if (WheelR)
			{
				if (UPrimitiveComponent* HitComponent = WheelR->GetRayCastHitComponent())
				{
					HitComps.AddUnique(HitComponent);
				}
			}
		}
	}
	return HitComps;
}

TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> UVehicleDriveAssemblyComponent::GetPowerUnitBridge() const
{
	FScopeLock Lock(&PowerUnitBridgeMutex);
	return PowerUnitBridge;
}

FVehicleInputRuntimeSettings UVehicleDriveAssemblyComponent::CaptureInputRuntimeSettings() const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_Input_CaptureSettings);
	check(IsInGameThread());
	FVehicleInputRuntimeSettings Settings;
	const auto CopyCurve = [](UCurveFloat* Source, FVehicleInputCurve& Destination)
	{
		Destination.bEnabled = IsValid(Source);
		if (Destination.bEnabled)
		{
			TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_Input_CopyCurve);
			Destination.Curve = Source->FloatCurve;
		}
	};
	const auto CopyAxis = [&CopyCurve](const FVehicleInputAxisConfig& Source, FVehicleInputAxisRuntimeConfig& Destination)
	{
		Destination.InterpSpeed = Source.InterpSpeed;
		CopyCurve(Source.ResponseCurve, Destination.ResponseCurve);
	};
	CopyAxis(InputConfig.Throttle, Settings.Throttle);
	CopyAxis(InputConfig.Brake, Settings.Brake);
	CopyAxis(InputConfig.Clutch, Settings.Clutch);
	CopyAxis(InputConfig.Steering, Settings.Steering);
	CopyAxis(InputConfig.Handbrake, Settings.Handbrake);
	CopyCurve(InputConfig.HighSpeedSteeringScale, Settings.HighSpeedSteeringScale);
	Settings.Assist = InputAssistConfig;
	Settings.bAutomaticGearbox = AutoGearboxConfig.bAutomaticGearbox;
	Settings.bArcadeAutoGearbox = AutoGearboxConfig.bArcadeAutoGearbox;
	Settings.SubstepDeltaTime = SubstepDeltaTime;
	return Settings;
}

void UVehicleDriveAssemblyComponent::ApplyInputCommand(EVehicleInputAxis Axis, float Input, bool bDirectInput)
{
	const FVehicleInputCommand Command{Axis, Input, bDirectInput, InputValues.bSwitchThrottleAndBrake};
	// The legacy caller still owns its pipeline. In the isolated lane this is only a GT view.
	FVehicleInputSolver::ApplyCommand(InputValues, Command);
	RequestedInputRaw = InputValues.Raw;
	if (bPowerUnitBridgeRequested)
	{
		check(IsInGameThread());
		if (TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> Bridge = GetPowerUnitBridge())
		{
			Bridge->RequestInput(Command);
		}
		// Before deferred initialization, InputValues itself supplies the one-time seed.
	}
}

bool UVehicleDriveAssemblyComponent::InitializePowerUnitBridge()
{
	check(IsInGameThread());
	if (GetPowerUnitBridge().IsValid()) return true;
	UVehicleEngineComponent* EngineRaw = Engine.Get();
	UVehicleClutchComponent* ClutchRaw = Clutch.Get();
	UVehicleGearboxComponent* GearboxRaw = Gearbox.Get();
	UVehicleDifferentialComponent* TransferCaseRaw = TransferCase.Get();
	if (!EngineRaw || !ClutchRaw || !GearboxRaw || !TransferCaseRaw) return false;
	// Existing components may BeginPlay after this assembly; defer until all initial caches are ready.
	if (!EngineRaw->HasBegunPlay() || !ClutchRaw->HasBegunPlay() || !GearboxRaw->HasBegunPlay()) return false;
	bool bSharedPowerComponent = false;
	TArray<UVehicleDriveAssemblyComponent*> OtherAssemblies;
	if (AActor* Owner = GetOwner()) Owner->GetComponents(OtherAssemblies);
	for (UVehicleDriveAssemblyComponent* Other : OtherAssemblies)
	{
		if (Other == this) continue;
		bSharedPowerComponent |= Other->GetEngine() == EngineRaw || Other->GetClutch() == ClutchRaw || Other->GetGearbox() == GearboxRaw;
		bSharedPowerComponent |= (Other->bUseExistingEngineComponent && Other->EngineComponentName == EngineRaw->GetFName())
			|| (Other->bUseExistingClutchComponent && Other->ClutchComponentName == ClutchRaw->GetFName())
			|| (Other->bUseExistingGearboxComponent && Other->GearboxComponentName == GearboxRaw->GetFName());
	}
	if (bSharedPowerComponent || !EngineRaw->CanBindPowerUnitBridge()
		|| !ClutchRaw->CanBindPowerUnitBridge() || !GearboxRaw->CanBindPowerUnitBridge())
	{
		bPowerUnitBridgeInitializationFailed = true;
		UE_LOG(LogTemp, Warning, TEXT("%s: isolated power-unit runtime cannot own components shared by another DriveAssembly. Use distinct power components for each assembly."), *GetName());
		return false;
	}

	FVehiclePowerUnitRuntime Runtime;
	FVehicleEngineRuntimeSettings EngineSettings = EngineRaw->CaptureRuntimeSettings();
	Runtime.EngineParameters = EngineSettings.Parameters;
	Runtime.TurboConfig = EngineSettings.TurboConfig;
	Runtime.ExhaustConfig = EngineSettings.ExhaustConfig;
	Runtime.TorqueLUT = MoveTemp(EngineSettings.TorqueLUT);
	Runtime.ClutchConfig = ClutchRaw->GetConfig();
	Runtime.GearboxConfig = GearboxRaw->GetConfig();
	Runtime.GearboxState = GearboxRaw->CaptureRuntimeSettingsState();
	if (!Runtime.Initialize())
	{
		bPowerUnitBridgeInitializationFailed = true;
		UE_LOG(LogTemp, Warning, TEXT("%s: isolated power-unit runtime requires valid forward/reverse gear counts."), *GetName());
		return false;
	}
	// Seed once. Subsequent settings must never overwrite integrating state with a displayed result.
	Runtime.EngineState = EngineRaw->GetEngineState();
	Runtime.ClutchState = ClutchRaw->GetClutchState();
	FVehicleDriveInputRuntime DriveInput;
	DriveInput.Settings = CaptureInputRuntimeSettings();
	DriveInput.InputValues = InputValues;
	RequestedInputRaw = InputValues.Raw;
	DriveInput.Telemetry.PhysicsDeltaTime = PhysicsDeltaTime;
	DriveInput.Telemetry.NumOfWheelsOnGround = NumOfWheelsOnGround;
	DriveInput.Telemetry.NumOfDriveAxles = NumOfDriveAxles;
	DriveInput.Telemetry.bIsInAir = bIsInAir;
	DriveInput.Telemetry.AbsoluteWorldLinearVelocity = AbsoluteWorldLinearVelocity;
	DriveInput.Telemetry.LocalLinearVelocity = LocalLinearVelocity;
	DriveInput.Telemetry.WorldLinearVelocity = WorldLinearVelocity;
	DriveInput.Telemetry.LocalVelocityClamped = LocalVelocityClamped;
	DriveInput.Telemetry.WorldAcceleration = WorldAcceleration;
	DriveInput.Telemetry.LocalAcceleration = LocalAcceleration;
	TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> NewBridge =
		MakeShared<FVehiclePowerUnitBridge, ESPMode::ThreadSafe>(MoveTemp(Runtime), MoveTemp(DriveInput), TransferCaseRaw->GetConfig());
	const bool bEngineBound = EngineRaw->BindPowerUnitBridge(NewBridge);
	const bool bClutchBound = ClutchRaw->BindPowerUnitBridge(NewBridge);
	const bool bGearboxBound = GearboxRaw->BindPowerUnitBridge(NewBridge);
	check(bEngineBound && bClutchBound && bGearboxBound);
	if (!bEngineBound || !bClutchBound || !bGearboxBound)
	{
		bPowerUnitBridgeInitializationFailed = true;
		return false;
	}
	{
		FScopeLock Lock(&PowerUnitBridgeMutex);
		BoundPowerUnitEngine = EngineRaw;
		BoundPowerUnitClutch = ClutchRaw;
		BoundPowerUnitGearbox = GearboxRaw;
		PowerUnitBridge = MoveTemp(NewBridge);
	}
	SubmitTransferCaseRuntimeSettings();
	return true;
}

void UVehicleDriveAssemblyComponent::SubmitTransferCaseRuntimeSettings()
{
	check(IsInGameThread());
	if (const auto Bridge = GetPowerUnitBridge())
	{
		if (UVehicleDifferentialComponent* TransferCaseRaw = TransferCase.Get())
		{
			TransferCaseRaw->AddConfigurationConsumer(this);
			Bridge->SubmitTransferCaseConfig(TransferCaseRaw->GetConfig());
		}
	}
}

void UVehicleDriveAssemblyComponent::RefreshPowerUnitBridge()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_DriveAssembly_RefreshPowerUnitBridge);
	check(IsInGameThread());
	TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> Bridge = GetPowerUnitBridge();
	if (!Bridge.IsValid()) return;
	const FVehiclePowerUnitResult Result = Bridge->ReadResult();
	InputValues = Result.InputValues;
	InputValues.Raw = RequestedInputRaw; // Older output must not undo pending player/network commands.
	PhysicsDeltaTime = Result.Telemetry.PhysicsDeltaTime;
	NumOfWheelsOnGround = Result.Telemetry.NumOfWheelsOnGround;
	NumOfDriveAxles = Result.Telemetry.NumOfDriveAxles;
	bIsInAir = Result.Telemetry.bIsInAir;
	AbsoluteWorldLinearVelocity = Result.Telemetry.AbsoluteWorldLinearVelocity;
	LocalLinearVelocity = Result.Telemetry.LocalLinearVelocity;
	WorldLinearVelocity = Result.Telemetry.WorldLinearVelocity;
	LocalVelocityClamped = Result.Telemetry.LocalVelocityClamped;
	WorldAcceleration = Result.Telemetry.WorldAcceleration;
	LocalAcceleration = Result.Telemetry.LocalAcceleration;
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_DriveAssembly_SubmitConfigSnapshots);
	Bridge->SubmitInputSettings(CaptureInputRuntimeSettings());
	SubmitTransferCaseRuntimeSettings();
	if (UVehicleEngineComponent* EngineRaw = Engine.Get())
	{
		EngineRaw->RefreshPublishedRuntimeState();
		Bridge->SubmitEngineSettings(EngineRaw->CaptureRuntimeSettings());
	}
	if (UVehicleClutchComponent* ClutchRaw = Clutch.Get())
	{
		ClutchRaw->RefreshPublishedRuntimeState();
		Bridge->SubmitClutchSettings(ClutchRaw->GetConfig());
	}
	if (UVehicleGearboxComponent* GearboxRaw = Gearbox.Get()) GearboxRaw->SubmitRuntimeSettings();
}

FVehicleGearboxRuntimeState UVehicleDriveAssemblyComponent::GetGearboxStateForInput() const
{
	if (UVehicleGearboxComponent* GearboxRaw = Gearbox.Get()) return GearboxRaw->CaptureRuntimeSettingsState();
	return FVehicleGearboxRuntimeState();
}

FVehicleGearboxConfig UVehicleDriveAssemblyComponent::GetGearboxConfigForInput() const
{
	if (UVehicleGearboxComponent* GearboxRaw = Gearbox.Get()) return GearboxRaw->GetConfig();
	return FVehicleGearboxConfig();
}

float UVehicleDriveAssemblyComponent::GetEngineRPMForInput() const
{
	if (UVehicleEngineComponent* EngineRaw = Engine.Get()) return EngineRaw->GetRPM();
	return 0.f;
}

bool UVehicleDriveAssemblyComponent::GeneratePowerUnit()
{
	AActor* Owner = GetOwner();
	if (!IsValid(Owner))return false;

	FString ThisName = FString();
	GetName(ThisName);

	if (!Engine.IsValid())
	{
		if (bUseExistingEngineComponent)
		{
			Engine = UVehicleUtilities::GetComponentByName<UVehicleEngineComponent>(Owner, EngineComponentName);
		}

		if (!Engine.IsValid())
		{
			FName Name = FName(ThisName + "_Engine");

			Engine = FindObject<UVehicleEngineComponent>(Owner, *Name.ToString());

			if (!Engine.IsValid())
			{
				Engine = UVehicleUtilities::CreateComponentByClass<UVehicleEngineComponent>(
					Owner,
					EngineClass,
					Name
				);
				AutoGeneratedComponents.AddUnique(Engine);
			}
		}
	}

	if (!Clutch.IsValid())
	{
		if (bUseExistingClutchComponent)
		{
			Clutch = UVehicleUtilities::GetComponentByName<UVehicleClutchComponent>(Owner, ClutchComponentName);
		}

		if (!Clutch.IsValid())
		{
			FName Name = FName(ThisName + "_Clutch");

			Clutch = FindObject<UVehicleClutchComponent>(Owner, *Name.ToString());

			if (!Clutch.IsValid())
			{
				Clutch = UVehicleUtilities::CreateComponentByClass<UVehicleClutchComponent>(
					Owner,
					ClutchClass,
					Name
				);
				AutoGeneratedComponents.AddUnique(Clutch);
			}
		}
	}

	if (!Gearbox.IsValid())
	{
		if (bUseExistingGearboxComponent)
		{
			Gearbox = UVehicleUtilities::GetComponentByName<UVehicleGearboxComponent>(Owner, GearboxComponentName);
		}

		if (!Gearbox.IsValid())
		{
			FName Name = FName(ThisName + "_Gearbox");

			Gearbox = FindObject<UVehicleGearboxComponent>(Owner, *Name.ToString());

			if (!Gearbox.IsValid())
			{
				Gearbox = UVehicleUtilities::CreateComponentByClass<UVehicleGearboxComponent>(
					Owner,
					GearboxClass,
					Name
				);
				AutoGeneratedComponents.AddUnique(Gearbox);
			}
		}
	}

	if (!TransferCase.IsValid())
	{
		if (bUseExistingTransferCaseComponent)
		{
			TransferCase = UVehicleUtilities::GetComponentByName<UVehicleDifferentialComponent>(Owner, TransferCaseComponentName);
		}

		if (!TransferCase.IsValid())
		{
			FName Name = FName(ThisName + "_TransferCase");

			TransferCase = FindObject<UVehicleDifferentialComponent>(Owner, *Name.ToString());

			if (!TransferCase.IsValid())
			{
				TransferCase = UVehicleUtilities::CreateComponentByClass<UVehicleDifferentialComponent>(
					Owner,
					TransferCaseClass,
					Name
				);
				if (TransferCase.IsValid())
				{
					FVehicleLimitedSlipDifferentialConfig DefaultTransferCaseConfig
						= FVehicleLimitedSlipDifferentialConfig{1.f, 0.f};
					TransferCase->SetConfig(DefaultTransferCaseConfig);
				}

				AutoGeneratedComponents.AddUnique(TransferCase);
			}
		}
	}

	return Engine.IsValid() && Clutch.IsValid() && Gearbox.IsValid() && TransferCase.IsValid();
}

int UVehicleDriveAssemblyComponent::GenerateAxles()
{
	if (!Chassis.IsValid())
	{
		Chassis = UVehicleUtilities::FindPhysicalParent(this);
		if (!Chassis.IsValid())
		{
			return -1;
		}
	}

	AActor* Owner = GetOwner();
	if (!IsValid(Owner))return 0;

	FString ThisName = FString();
	GetName(ThisName);

	int32 n = 0;
	for (FVehicleAxleSpawnTemplate AxleTemplate : AxleTemplates)
	{
		if (!AxleTemplate.bUseExistingComponent)
		{
			// generate new axle
			FName Name = FName(ThisName + "_Axle_"+ FString::FromInt(n));

			UVehicleAxleAssemblyComponent* Axle = FindObject<UVehicleAxleAssemblyComponent>(Owner, *Name.ToString());

			if (!IsValid(Axle))
			{
				Axle = UVehicleUtilities::CreateComponentByClass<UVehicleAxleAssemblyComponent>(
					Owner,
					nullptr,
					Name
				);
				AutoGeneratedComponents.AddUnique(Axle);
			}
			
			if (IsValid(Axle))
			{
				Axle->AttachToComponent(Chassis.Get(), FAttachmentTransformRules::KeepRelativeTransform);
				Axle->SetRelativeLocation(AxleTemplate.AxlePosition);
			}

			if (IsValid(AxleTemplate.AxleClass))
			{
				const UVehicleAxleAssemblyComponent* TemplateAxle = Cast<UVehicleAxleAssemblyComponent>(AxleTemplate.AxleClass->GetDefaultObject());
				if (IsValid(Axle))
				{
					UVehicleAxleAssemblyComponent::CopyAxleConfig(TemplateAxle, Axle);
				}
			}
			
			if (IsValid(Axle))
			{
				Axle->ApplyInitialOverrides(AxleTemplate);

				// initialize
				Axle->InitializeWheels();

				Axles.AddUnique(Axle);
				n++;
			}
		}		
	}
	return n;
}

int UVehicleDriveAssemblyComponent::SearchExistingAxles()
{
	int32 n = 0;
	for (FVehicleAxleSpawnTemplate Template : AxleTemplates)
	{
		AActor* Owner = GetOwner();
		if (Template.bUseExistingComponent && IsValid(Owner))
		{
			UVehicleAxleAssemblyComponent* Axle =
				UVehicleUtilities::GetComponentByName<UVehicleAxleAssemblyComponent>(Owner, Template.AxleComponentName);
			if (IsValid(Axle))
			{
				Axles.AddUnique(Axle);
				n++;
			}
		}
	}
	return n;
}

TArray<FName> UVehicleDriveAssemblyComponent::GetNamesOfAxlesOfOwner()
{
	return UVehicleUtilities::GetNamesOfComponentsOfActor<UVehicleAxleAssemblyComponent>(this);
}

TArray<FName> UVehicleDriveAssemblyComponent::GetNamesOfEnginesOfOwner()
{
	return UVehicleUtilities::GetNamesOfComponentsOfActor<UVehicleEngineComponent>(this);
}

TArray<FName> UVehicleDriveAssemblyComponent::GetNamesOfClutchesOfOwner()
{
	return UVehicleUtilities::GetNamesOfComponentsOfActor<UVehicleClutchComponent>(this);
}

TArray<FName> UVehicleDriveAssemblyComponent::GetNamesOfGearboxesOfOwner()
{
	return UVehicleUtilities::GetNamesOfComponentsOfActor<UVehicleGearboxComponent>(this);
}

TArray<FName> UVehicleDriveAssemblyComponent::GetNamesOfTransferCasesOfOwner()
{
	return UVehicleUtilities::GetNamesOfComponentsOfActor<UVehicleDifferentialComponent>(this);
}

