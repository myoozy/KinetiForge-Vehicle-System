// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)


#include "VehicleGearboxComponent.h"
#include "VehicleUtilities.h"
#include "VehicleTransmissionSolver.h"
#include "VehiclePowerUnitBridge.h"

// Sets default values for this component's properties
UVehicleGearboxComponent::UVehicleGearboxComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void UVehicleGearboxComponent::BeginPlay()
{
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::BeginPlay();

	// ...
	CalculateGearRatios();	//to avoid nullptr
}

void UVehicleGearboxComponent::StartShift(int32 InTargetGear, bool bImmediate)
{
	//check if gearratios has to be calculated again
	if (IsGearDataDirty())
	{
		CalculateGearRatios();
	}

	//check if target gear is valid
	FVehicleGearboxRuntimeState RuntimeState = CaptureRuntimeState();
	if (!FVehicleGearboxSolver::BeginShift(Config, RuntimeState,
		GearRatios.Num(), ReverseGearRatios.Num(), InTargetGear)) return;

	//if GetWorld()=false, can't set timer, if ShiftDelay to small, immediate
	bImmediate = bImmediate || !IsValid(GetWorld()) || Config.ShiftDelay < SMALL_NUMBER;

	//Sequential gearbox can only shift one gear once
	//prepare shift
	ApplyRuntimeState(RuntimeState);

	//start shift
	if (bImmediate)
	{
		FinalizeShift();
	}
	else
	{
		//set timer
		GetWorld()->GetTimerManager().SetTimer(
			GearShiftTimerHandle,
			this,
			&UVehicleGearboxComponent::FinalizeShift,	//call this function after delay
			Config.ShiftDelay,
			false // non cycle
		);
	}
}

void UVehicleGearboxComponent::FinalizeShift()
{
	FVehicleGearboxRuntimeState RuntimeState = CaptureRuntimeState();
	FVehicleGearboxSolver::CompleteShift(Config, RuntimeState, GearRatios, ReverseGearRatios);
	ApplyRuntimeState(RuntimeState);

	if (ShiftFinishedCallback.IsBound())
	{
		ShiftFinishedCallback.Execute();
		ShiftFinishedCallback.Unbind();	//if it is not manually unbound, the delegate will be called after every shift
	}
}

// Called every frame
void UVehicleGearboxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UVehicleGearboxComponent::ShiftToTargetGear(int32 InTargetGear, bool bImmediate)
{
	StartShift(InTargetGear, bImmediate);
}

void UVehicleGearboxComponent::ShiftToTargetGearWithDelegate(FOnShiftFinishedDelegate InOnShiftFinished, int32 InTargetGear, bool bImmediate)
{
	ShiftFinishedCallback = InOnShiftFinished;
	StartShift(InTargetGear, bImmediate);
}

void UVehicleGearboxComponent::UpdateOutputShaft(
	float InClutchTorque, 
	float& OutTorque
)
{
	OutTorque = FVehicleGearPathSolver::CalculateOutputTorque(
		InClutchTorque + P2MotorTorque, CurrentGearRatio, Config.Efficiency);
	//OutReflectedInertia = Config.InputShaftInertia * CurrentGearRatio * CurrentGearRatio;
}

void UVehicleGearboxComponent::UpdateInputShaft(
	float InAxleVelocity, 
	float InAxleInertia, 
	float InDriveShaftStiffness,
	float& OutClutchVelocity, 
	float& OutReflectedInertia,
	float& OutReflectedDriveShaftStiffness,
	float& OutCurrentGearRatio
)
{
	const FVehicleGearPathReflection Reflection = FVehicleGearPathSolver::ReflectOutputShaft(
		InAxleVelocity, InAxleInertia, InDriveShaftStiffness, CurrentGearRatio);
	OutClutchVelocity = Reflection.InputAngularVelocity;
	OutReflectedInertia = Reflection.ReflectedInertia;
	OutReflectedDriveShaftStiffness = Reflection.ReflectedDriveShaftStiffness;
	OutCurrentGearRatio = CurrentGearRatio;		
}

float UVehicleGearboxComponent::GetGearRatio(int InTarget)
{
	//InTarget = FMath::Clamp(InTarget, -ReverseGearRatios.Num(), GearRatios.Num());
	return FVehicleGearboxSolver::GetGearRatio(Config, GearRatios, ReverseGearRatios, InTarget);
}

void UVehicleGearboxComponent::CalculateSpeedRangeOfEachGear(
	float InEffectiveWheelRadius,
	float InEngineIdleRPM, 
	float InEngineMaxRPM, 
	TArray<FVector2D>& OutSpeedRanges
)
{
	if (IsGearDataDirty())CalculateGearRatios();

	FVehicleGearboxSolver::CalculateSpeedRanges(Config, GearRatios, ReverseGearRatios,
		InEffectiveWheelRadius, InEngineIdleRPM, InEngineMaxRPM, OutSpeedRanges);
}

bool UVehicleGearboxComponent::CalculateGearRatios()
{
	//make sure positive
	//make sure topgear is smaller than first gear
	//clear all gears
	return FVehicleGearboxSolver::CalculateGearRatios(Config, GearRatios, ReverseGearRatios);
}

bool UVehicleGearboxComponent::CalculateGearRatios(TArray<float>& LargerArray, TArray<float>& SmallerArray, bool bInverseSign)
{
	//normalize the factor, min = 0. max = 1
	//calculate bias
	return FVehicleGearboxSolver::CalculateGearRatios(Config, LargerArray, SmallerArray, bInverseSign);
}

bool UVehicleGearboxComponent::IsGearDataDirty()
{
	//check if gearratios has to be calculated again
	if (CachedFirstGear != Config.FirstGear || CachedTopGear != Config.TopGear || CachedGearRatioBias != Config.GearRatioBias
		|| CachedNumGears != Config.NumberOfGears || CachedNumRGears != Config.NumOfReverseGears)
	{
		CachedFirstGear = Config.FirstGear;
		CachedTopGear = Config.TopGear;
		CachedGearRatioBias = Config.GearRatioBias;
		CachedNumGears = Config.NumberOfGears;
		CachedNumRGears = Config.NumOfReverseGears;
		return true;
	}
	else
	{
		return false;
	}
}

FVehicleGearboxRuntimeState UVehicleGearboxComponent::CaptureRuntimeState() const
{
	FVehicleGearboxRuntimeState RuntimeState;
	RuntimeState.CurrentGearRatio = CurrentGearRatio;
	RuntimeState.P2MotorTorque = P2MotorTorque;
	RuntimeState.CurrentGear = CurrentGear;
	RuntimeState.TargetGear = TargetGear;
	RuntimeState.bIsInGear = bIsInGear;
	RuntimeState.bShouldRevMatch = bShouldRevMatch;
	RuntimeState.bShouldCutSpark = bShouldCutSpark;
	return RuntimeState;
}

void UVehicleGearboxComponent::ApplyRuntimeState(const FVehicleGearboxRuntimeState& RuntimeState)
{
	CurrentGearRatio = RuntimeState.CurrentGearRatio;
	P2MotorTorque = RuntimeState.P2MotorTorque;
	CurrentGear = RuntimeState.CurrentGear;
	TargetGear = RuntimeState.TargetGear;
	bIsInGear = RuntimeState.bIsInGear;
	bShouldRevMatch = RuntimeState.bShouldRevMatch;
	bShouldCutSpark = RuntimeState.bShouldCutSpark;
	SubmitRuntimeSettings();
}

void UVehicleGearboxComponent::SetConfig(const FVehicleGearboxConfig& NewConfig)
{
	check(IsInGameThread());
	Config = NewConfig;
	CalculateGearRatios();
	// Configuration changes update the engaged path without completing/restarting a pending shift.
	CurrentGearRatio = bIsInGear ? GetGearRatio(CurrentGear) : 0.f;
	SubmitRuntimeSettings();
}

void UVehicleGearboxComponent::SetP2MotorTorque(float NewTorque)
{
	P2MotorTorque = NewTorque;
	SubmitRuntimeSettings();
}

bool UVehicleGearboxComponent::CanBindPowerUnitBridge() const
{
	check(IsInGameThread());
	return !bPowerUnitBridgeBound;
}

bool UVehicleGearboxComponent::BindPowerUnitBridge(TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> InBridge)
{
	check(IsInGameThread());
	if (!CanBindPowerUnitBridge() || !InBridge.IsValid()) return false;
	PowerUnitBridge = InBridge;
	bPowerUnitBridgeBound = true;
	SubmitRuntimeSettings();
	return true;
}

void UVehicleGearboxComponent::SubmitRuntimeSettings()
{
	if (!bPowerUnitBridgeBound) return;
	check(IsInGameThread());
	if (TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> Bridge = PowerUnitBridge.Pin())
	{
		Bridge->SubmitGearboxSettings(Config, CaptureRuntimeState());
	}
}


void UVehicleGearboxComponent::OnRegister()
{
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::OnRegister();
}

void UVehicleGearboxComponent::OnUnregister()
{
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::OnUnregister();
}

void UVehicleGearboxComponent::BeginDestroy()
{
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::BeginDestroy();
}

void UVehicleGearboxComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::EndPlay(EndPlayReason);
}

void UVehicleGearboxComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	PhysicsOwnership.InvalidateOwnerAndWait();
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}
