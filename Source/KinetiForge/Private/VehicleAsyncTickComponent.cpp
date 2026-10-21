// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehicleAsyncTickComponent.h"
#include "VehicleDriveAssemblyComponent.h"
#include "VehicleWheelCoordinatorComponent.h"
#include "VehicleAsyncSpringArmComponent.h"
#include "AsyncTickManager.h"
#include "VehiclePhysicsStep.h"
#include "Misc/ScopeLock.h"

UVehicleAsyncTickComponent* UVehicleAsyncTickComponent::FindVehicleAsyncTickComponent(AActor* VehicleActor)
{
	if (!IsValid(VehicleActor))return nullptr;

	//if found
	if(UVehicleAsyncTickComponent* AsyncTickComp = 
		Cast<UVehicleAsyncTickComponent>(VehicleActor->GetComponentByClass(UVehicleAsyncTickComponent::StaticClass())))
	{
		return AsyncTickComp;
	}
	else
	{
		//if not found
		AsyncTickComp =
			Cast<UVehicleAsyncTickComponent>(VehicleActor->AddComponentByClass(UVehicleAsyncTickComponent::StaticClass(), false, FTransform(), false));
		return AsyncTickComp;
	}
}

void UVehicleAsyncTickComponent::RegisterDriveAssembly(UVehicleDriveAssemblyComponent* newDriveAssembly)
{
	check(IsInGameThread());
	if (!IsValid(newDriveAssembly)) return;
	if (newDriveAssembly->VehicleAsyncTickComponent.IsValid()
		&& newDriveAssembly->VehicleAsyncTickComponent.Get() != this) return;
	if (!newDriveAssembly->PreparePhysicsStepRegistration()) return;
	newDriveAssembly->VehicleAsyncTickComponent = this;
	const auto Step = newDriveAssembly->GetPhysicsStep();
	TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe> ReplacedStep;
	{
		FScopeLock Lock(&RegistrationMutex);
		if (const auto* ExistingStep = DrivePhysicsSteps.Find(newDriveAssembly))
		{
			ReplacedStep = *ExistingStep;
		}
		DriveAssemblies.AddUnique(newDriveAssembly);
		DrivePhysicsSteps.Add(newDriveAssembly, Step);
	}
	// Stop a replaced entry without the registration lock.
	if (ReplacedStep && ReplacedStep != Step) ReplacedStep->InvalidateBindingAndWait();
}

void UVehicleAsyncTickComponent::UnRegisterDriveAssembly(UVehicleDriveAssemblyComponent* targetDriveAssembly)
{
	check(IsInGameThread());
	TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe> RemovedStep;
	{
		FScopeLock Lock(&RegistrationMutex);
		DriveAssemblies.Remove(targetDriveAssembly);
		DrivePhysicsSteps.RemoveAndCopyValue(targetDriveAssembly, RemovedStep);
	}
	// Never wait while holding the list mutex: an in-flight tick may be copying that list.
	if (RemovedStep) RemovedStep->StopAndWait();
}

void UVehicleAsyncTickComponent::RegisterWheelCoordinator(UVehicleWheelCoordinatorComponent* newWheelCoordinator)
{
	FScopeLock Lock(&RegistrationMutex);
	if (IsValid(newWheelCoordinator))WheelCoordinators.AddUnique(newWheelCoordinator);
}

void UVehicleAsyncTickComponent::UnRegisterWheelCoordinator(UVehicleWheelCoordinatorComponent* targetWheelCoordinator)
{
	FScopeLock Lock(&RegistrationMutex);
	WheelCoordinators.Remove(targetWheelCoordinator);
}

void UVehicleAsyncTickComponent::RegisterAsyncSpringArm(UVehicleAsyncSpringArmComponent* newAsyncSpringArm)
{
	FScopeLock Lock(&RegistrationMutex);
	if (IsValid(newAsyncSpringArm))AsyncSpringArms.AddUnique(newAsyncSpringArm);
}

void UVehicleAsyncTickComponent::UnRegisterAsyncSpringArm(UVehicleAsyncSpringArmComponent* targetAsyncSpringArm)
{
	FScopeLock Lock(&RegistrationMutex);
	AsyncSpringArms.Remove(targetAsyncSpringArm);
}

void UVehicleAsyncTickComponent::OnRegister()
{
	Super::OnRegister();
	if (!HasBegunPlay()) return; // Initial registration is connected by the existing BeginPlay path.
	TArray<TWeakObjectPtr<UVehicleDriveAssemblyComponent>> DrivesToRestore;
	{
		FScopeLock Lock(&RegistrationMutex);
		DrivesToRestore = DriveAssemblies;
	}
	for (const auto& Drive : DrivesToRestore)
	{
		if (Drive.IsValid() && Drive->IsRegistered() && Drive->HasBegunPlay()) RegisterDriveAssembly(Drive.Get());
	}
	SetActive(true);
	SetAsyncPhysicsTickEnabled(true); // Auxiliary UObject updates always use the engine's frozen-GT callback.
	if (UWorld* World = GetWorld())
	{
		if (FAsyncTickManager* Manager = FAsyncTickManager::GetPhysicsManagerFromScene(World->GetPhysicsScene()))
		{
			Manager->AddActorComponent(this);
		}
	}
}

void UVehicleAsyncTickComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//make sure the component is registered and active
	SetActive(true);
	if (IsRegistered() && IsActive())
	{
		SetAsyncPhysicsTickEnabled(true); // Drive keeps its separate worker/frozen choice.
	}

}

void UVehicleAsyncTickComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopDrivePhysicsSteps(true);
	SetAsyncPhysicsTickEnabled(false);
	SetActive(false);

	Super::EndPlay(EndPlayReason);

	// ...
}

void UVehicleAsyncTickComponent::BeginDestroy()
{
	// UObject destruction can bypass normal tick registration teardown.
	StopDrivePhysicsSteps(true);
	Super::BeginDestroy();
}

void UVehicleAsyncTickComponent::OnUnregister()
{
	StopDrivePhysicsSteps(false);
	SetAsyncPhysicsTickEnabled(false);
	SetActive(false);

	// The scene may already be gone during deferred GC; do not retain its raw manager pointer.
	if (UWorld* World = GetWorld())
	{
		if (FAsyncTickManager* Manager = FAsyncTickManager::GetPhysicsManagerFromScene(World->GetPhysicsScene()))
		{
			Manager->RemoveActorComponent(this);
		}
	}

	Super::OnUnregister();
}

void UVehicleAsyncTickComponent::NativeAsyncTick(float DeltaTime)
{
	Super::NativeAsyncTick(DeltaTime);
}

void UVehicleAsyncTickComponent::AsyncPhysicsTickComponent(float DeltaTime, float SimTime)
{
	Super::AsyncPhysicsTickComponent(DeltaTime, SimTime);

	UpdateAuxiliaryPhysics(DeltaTime);
}

void UVehicleAsyncTickComponent::UpdateAuxiliaryPhysics(float DeltaTime)
{
	check(IsInGameThread()); // Invoked by the engine's frozen-GT physics callback, not ordinary GT Tick.
	TArray<TWeakObjectPtr<UVehicleWheelCoordinatorComponent>> Coordinators;
	TArray<TWeakObjectPtr<UVehicleAsyncSpringArmComponent>> SpringArms;
	{
		FScopeLock Lock(&RegistrationMutex);
		Coordinators = WheelCoordinators;
		SpringArms = AsyncSpringArms;
	}
	// aeros
	for (TWeakObjectPtr<UVehicleWheelCoordinatorComponent> WheelCoordinatorPtr : Coordinators)
	{
		if (UVehicleWheelCoordinatorComponent* WheelCoordinator = WheelCoordinatorPtr.Get())
		{
			WheelCoordinator->UpdateAeros(DeltaTime);
		}
	}

	// async spring arms
	for (TWeakObjectPtr<UVehicleAsyncSpringArmComponent> AsyncSpringArmPtr : SpringArms)
	{
		if (UVehicleAsyncSpringArmComponent* SpringArm = AsyncSpringArmPtr.Get())
		{
			SpringArm->UpdatePhysics(DeltaTime);
		}
	}
}

void UVehicleAsyncTickComponent::StopDrivePhysicsSteps(bool bForgetDrives)
{
	check(IsInGameThread());
	TArray<TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe>> Steps;
	{
		FScopeLock Lock(&RegistrationMutex);
		DrivePhysicsSteps.GenerateValueArray(Steps);
		DrivePhysicsSteps.Reset();
		if (bForgetDrives) DriveAssemblies.Reset();
	}
	for (const auto& Step : Steps) if (Step) Step->StopAndWait();
}
