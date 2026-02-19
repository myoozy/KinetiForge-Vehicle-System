// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#include "VehicleAsyncTickComponent.h"
#include "VehicleDriveAssemblyComponent.h"
#include "VehicleAsyncSpringArmComponent.h"
#include "AsyncTickManager.h"

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

void UVehicleAsyncTickComponent::Register(UVehicleDriveAssemblyComponent* newDriveAssembly)
{
	DriveAssemblies.AddUnique(newDriveAssembly);
}

void UVehicleAsyncTickComponent::UnRegister(UVehicleDriveAssemblyComponent* targetDriveAssembly)
{
	DriveAssemblies.Remove(targetDriveAssembly);
}

void UVehicleAsyncTickComponent::Register(UVehicleAsyncSpringArmComponent* newAsyncSpringArm)
{
	AsyncSpringArms.AddUnique(newAsyncSpringArm);
}

void UVehicleAsyncTickComponent::UnRegister(UVehicleAsyncSpringArmComponent* targetAsyncSpringArm)
{
	AsyncSpringArms.Remove(targetAsyncSpringArm);
}

void UVehicleAsyncTickComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//make sure the component is registered and active
	SetActive(true);
	if (IsRegistered() && IsActive())
	{
		SetAsyncPhysicsTickEnabled(bUpdatePhysicsOnGameThread);
	}

	if (UWorld* World = GetWorld())
	{
		if (FAsyncTickManager* FoundManager = FAsyncTickManager::GetPhysicsManagerFromScene(World->GetPhysicsScene()))
		{
			CachedAsyncTickManager = FoundManager;
		}
	}
}

void UVehicleAsyncTickComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	SetAsyncPhysicsTickEnabled(false);
	SetActive(false);

	Super::EndPlay(EndPlayReason);

	// ...
}

void UVehicleAsyncTickComponent::OnUnregister()
{
	SetAsyncPhysicsTickEnabled(false);
	SetActive(false);

	if (CachedAsyncTickManager)
	{
		CachedAsyncTickManager->RemoveActorComponent(this);
		CachedAsyncTickManager = nullptr;
	}

	Super::OnUnregister();
}

void UVehicleAsyncTickComponent::NativeAsyncTick(float DeltaTime)
{
	Super::NativeAsyncTick(DeltaTime);

	if (!bUpdatePhysicsOnGameThread)
	{
		UpdateVehiclePhysics(DeltaTime);
	}

	// just update this in worker thread because it doesn't hold any uobject pointer
	UpdateAsyncSpringArms(DeltaTime);
}

void UVehicleAsyncTickComponent::AsyncPhysicsTickComponent(float DeltaTime, float SimTime)
{
	Super::AsyncPhysicsTickComponent(DeltaTime, SimTime);

	if (bUpdatePhysicsOnGameThread)
	{
		UpdateVehiclePhysics(DeltaTime);
	}
}

void UVehicleAsyncTickComponent::UpdateVehiclePhysics(float DeltaTime)
{
	for (TWeakObjectPtr<UVehicleDriveAssemblyComponent> DriveAssemblyPtr : DriveAssemblies)
	{
		if (UVehicleDriveAssemblyComponent* DriveAssembly = DriveAssemblyPtr.Get())
		{
			if (DriveAssembly->bUpdatePhysicsAutomatically)
			{
				DriveAssembly->UpdatePhysics(DeltaTime);
			}
		}
	}
}

void UVehicleAsyncTickComponent::UpdateAsyncSpringArms(float DeltaTime)
{
	for (TWeakObjectPtr<UVehicleAsyncSpringArmComponent> AsyncSpringArmPtr : AsyncSpringArms)
	{
		if (UVehicleAsyncSpringArmComponent* SpringArm = AsyncSpringArmPtr.Get())
		{
			SpringArm->UpdatePhysics(DeltaTime);
		}
	}
}
