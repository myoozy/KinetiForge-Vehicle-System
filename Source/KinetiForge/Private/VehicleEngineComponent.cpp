// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)


#include "VehicleEngineComponent.h"
#include "ProfilingDebugging/CpuProfilerTrace.h"
#include "VehicleUtilities.h"
#include "VehicleEngineSolver.h"
#include "VehiclePowerUnitBridge.h"

// Sets default values for this component's properties
UVehicleEngineComponent::UVehicleEngineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PostUpdateWork;

	// ...
	if (!NAConfig.EngineTorqueCurve)
	{
		static ConstructorHelpers::FObjectFinder<UCurveFloat> DefaultTorqueCurveObj(
			TEXT("/Script/Engine.CurveFloat'/KinetiForge/DefaultConfigs/Curves/DefaultNaturallyAspiratedEngineTorqueCurve.DefaultNaturallyAspiratedEngineTorqueCurve'")
		);
		if (DefaultTorqueCurveObj.Succeeded())
		{
			NAConfig.EngineTorqueCurve = DefaultTorqueCurveObj.Object;
		}
	}
}


// Called when the game starts
void UVehicleEngineComponent::BeginPlay()
{
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::BeginPlay();

	// ...

	UpdateCachedLookUpTables();

	// reset rev-limiter
	State.RevLimiterTimer = NAConfig.RevLimiterTime;

#if WITH_EDITOR
	TimeSinceLastConfigSync = FMath::FRandRange(0.f, ConfigSyncInterval);
	TimeSinceLastConfigSync += ConfigSyncInterval;
#endif
}


void UVehicleEngineComponent::EngineAcceleration(const float DeltaTime)
{
	if (bPowerUnitBridgeBound.Load()) return;
	FVehicleEngineSolver::EngineAcceleration(FVehicleEngineParameters::FromConfig(NAConfig),
		TurboConfig, CachedTorqueLUT, State, DeltaTime, bShouldTriggerTurboBlowOffCallback);
}

void UVehicleEngineComponent::UpdateExhaust(const float DeltaTime)
{
	if (bPowerUnitBridgeBound.Load()) return;
	FVehicleEngineSolver::UpdateExhaust(FVehicleEngineParameters::FromConfig(NAConfig),
		ExhaustConfig, State, DeltaTime, bShouldTriggerBackfiringCallback);
}

float UVehicleEngineComponent::EstimateThrottleForTorque(float TargetEffectiveTorque)
{
	return FVehicleEngineSolver::EstimateThrottleForTorque(FVehicleEngineParameters::FromConfig(NAConfig),
		State, CachedTorqueLUT, TargetEffectiveTorque);
}

// Called every frame
void UVehicleEngineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	RefreshPublishedRuntimeState();

	// ...

#if WITH_EDITOR
	if (ConfigSyncInterval >= 0.f)
	{
		TimeSinceLastConfigSync += DeltaTime;
		if (TimeSinceLastConfigSync > ConfigSyncInterval)
		{
			TimeSinceLastConfigSync -= ConfigSyncInterval;
			UpdateCachedLookUpTables();
		}
	}
#endif
	
	if (bShouldTriggerTurboBlowOffCallback)
	{
		bShouldTriggerTurboBlowOffCallback = false;
		for (FOnTurboBlowOffDelegate BlowOffCallback : TurboBlowOffCallbacks)
		{
			if (BlowOffCallback.IsBound())
			{
				BlowOffCallback.Execute();
			}
			else
			{
				TurboBlowOffCallbacks.Remove(BlowOffCallback);
			}
		}
	}

	if (bShouldTriggerBackfiringCallback)
	{
		bShouldTriggerBackfiringCallback = false;
		for (FOnBackfiringDelegate BackfiringCallback : BackfiringCallbacks)
		{
			if (BackfiringCallback.IsBound())
			{
				BackfiringCallback.Execute();
			}
			else
			{
				BackfiringCallbacks.Remove(BackfiringCallback);
			}
		}
	}
}

void UVehicleEngineComponent::UpdatePhysics(float InDeltaTime, float InThrottle, float InLoadTorque, bool bDisableSpark)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_Engine_UpdatePhysics);
	if (bPowerUnitBridgeBound.Load()) return;

	FVehicleEngineSolver::UpdatePhysics(FVehicleEngineParameters::FromConfig(NAConfig),
		TurboConfig, ExhaustConfig, CachedTorqueLUT, State, InDeltaTime, InThrottle, InLoadTorque,
		bDisableSpark, bShouldTriggerTurboBlowOffCallback, bShouldTriggerBackfiringCallback);
}

void UVehicleEngineComponent::SetNaturallyAspiratedEngineConfig(const FVehicleNaturallyAspiratedEngineConfig& NewConfig)
{
	NAConfig = NewConfig;
	UpdateCachedLookUpTables();
}

void UVehicleEngineComponent::SetTurboConfig(const FVehicleEngineTurboConfig& NewConfig)
{
	TurboConfig = NewConfig;
	SubmitRuntimeSettings();
}

void UVehicleEngineComponent::SetExhaustConfig(const FVehicleEngineExhaustConfig& NewConfig)
{
	ExhaustConfig = NewConfig;
	SubmitRuntimeSettings();
}

float UVehicleEngineComponent::GetMaxEngineTorque()
{
	return FVehicleEngineSolver::CalculateMaxTorque(FVehicleEngineParameters::FromConfig(NAConfig), TurboConfig);
}

void UVehicleEngineComponent::UpdateCachedLookUpTables()
{
	if (IsValid(NAConfig.EngineTorqueCurve))
	{
		CachedTorqueLUT.CopyFromRichCurve(
			NAConfig.EngineTorqueCurve->FloatCurve,
			FVector2f(NAConfig.EngineIdleRPM, NAConfig.EngineMaxRPM)
		);
	}
	else
	{
		CachedTorqueLUT.SetAllTo(1.f);
	}
	SubmitRuntimeSettings();
}

FVehicleEngineRuntimeSettings UVehicleEngineComponent::CaptureRuntimeSettings() const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(KinetiForgeVehicle_Engine_CaptureRuntimeSettings);
	check(IsInGameThread());
	FVehicleEngineRuntimeSettings Settings;
	Settings.Parameters = FVehicleEngineParameters::FromConfig(NAConfig);
	Settings.TurboConfig = TurboConfig;
	Settings.ExhaustConfig = ExhaustConfig;
	Settings.TorqueLUT = CachedTorqueLUT;
	return Settings;
}

bool UVehicleEngineComponent::CanBindPowerUnitBridge() const
{
	check(IsInGameThread());
	return !bPowerUnitBridgeBound.Load();
}

bool UVehicleEngineComponent::BindPowerUnitBridge(
	TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> InBridge)
{
	check(IsInGameThread());
	if (!CanBindPowerUnitBridge() || !InBridge.IsValid()) return false;
	PowerUnitBridge = InBridge;
	RequestedP1MotorTorque = State.P1MotorTorque;
	bPowerUnitBridgeBound.Store(true);
	SubmitRuntimeSettings();
	RefreshPublishedRuntimeState();
	return true;
}

void UVehicleEngineComponent::SubmitRuntimeSettings()
{
	if (!bPowerUnitBridgeBound.Load()) return;
	check(IsInGameThread());
	if (TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> Bridge = PowerUnitBridge.Pin())
	{
		Bridge->SubmitEngineSettings(CaptureRuntimeSettings());
	}
}

void UVehicleEngineComponent::RefreshPublishedRuntimeState()
{
	if (!bPowerUnitBridgeBound.Load()) return;
	check(IsInGameThread());
	if (TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> Bridge = PowerUnitBridge.Pin())
	{
		Bridge->ReadEngineResult(State, bShouldTriggerTurboBlowOffCallback,
			bShouldTriggerBackfiringCallback);
		// P1 is a commanded input, unlike RPM: keep the current GT value even if the result is older.
		State.P1MotorTorque = RequestedP1MotorTorque;
	}
}

void UVehicleEngineComponent::SetP1MotorTorque(float NewTorque)
{
	RequestedP1MotorTorque = NewTorque;
	if (bPowerUnitBridgeBound.Load())
	{
		check(IsInGameThread());
		State.P1MotorTorque = NewTorque;
		if (TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> Bridge = PowerUnitBridge.Pin())
		{
			Bridge->RequestP1MotorTorque(NewTorque);
		}
		return;
	}
	State.P1MotorTorque = NewTorque;
}

EVehicleEngineOperationMode UVehicleEngineComponent::StartVehicleEngine()
{
	if (bPowerUnitBridgeBound.Load())
	{
		check(IsInGameThread());
		if (TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> Bridge = PowerUnitBridge.Pin())
		{
			Bridge->RequestEngineStart();
		}
	}
	return FVehicleEngineSolver::StartVehicleEngine(State);
}

EVehicleEngineOperationMode UVehicleEngineComponent::ShutVehicleEngine()
{
	if (bPowerUnitBridgeBound.Load())
	{
		check(IsInGameThread());
		if (TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> Bridge = PowerUnitBridge.Pin())
		{
			Bridge->RequestEngineShutdown();
		}
	}
	return FVehicleEngineSolver::ShutdownVehicleEngine(State);
}

void UVehicleEngineComponent::BindEventToOnTurboBlowOff(FOnTurboBlowOffDelegate InOnTurboBlowOff)
{
	if (TurboBlowOffCallbacks.Find(InOnTurboBlowOff) == INDEX_NONE)
	{
		TurboBlowOffCallbacks.Add(InOnTurboBlowOff);
	}
}

void UVehicleEngineComponent::BindEventToOnBackfiring(FOnBackfiringDelegate InOnBackFiring)
{
	if (BackfiringCallbacks.Find(InOnBackFiring) == INDEX_NONE)
	{
		BackfiringCallbacks.Add(InOnBackFiring);
	}
}


void UVehicleEngineComponent::OnRegister()
{
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::OnRegister();
}

void UVehicleEngineComponent::OnUnregister()
{
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::OnUnregister();
}

void UVehicleEngineComponent::BeginDestroy()
{
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::BeginDestroy();
}

void UVehicleEngineComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::EndPlay(EndPlayReason);
}

void UVehicleEngineComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}
