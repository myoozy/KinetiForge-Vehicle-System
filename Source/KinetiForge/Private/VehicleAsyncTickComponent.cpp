// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#include "VehicleAsyncTickComponent.h"
#include "VehicleDriveAssemblyComponent.h"
#include "Windows/WindowsHWrapper.h"

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
}

void UVehicleAsyncTickComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	SetAsyncPhysicsTickEnabled(false);

	Super::EndPlay(EndPlayReason);

	// ...
}

void  UVehicleAsyncTickComponent::OnUnregister()
{
	SetAsyncPhysicsTickEnabled(false);

	Super::OnUnregister();

	// ...
}

void  UVehicleAsyncTickComponent::BeginDestroy()
{
	SetAsyncPhysicsTickEnabled(false);

	Super::BeginDestroy();
}

void UVehicleAsyncTickComponent::NativeAsyncTick(float DeltaTime)
{
	Super::NativeAsyncTick(DeltaTime);

	if (!bUpdatePhysicsOnGameThread)
	{
		UpdateVehiclePhysics(DeltaTime);
	}
}

void UVehicleAsyncTickComponent::AsyncPhysicsTickComponent(float DeltaTime, float SimTime)
{
	Super::AsyncPhysicsTickComponent(DeltaTime, SimTime);

	if (bUpdatePhysicsOnGameThread)
	{
		UpdateVehiclePhysics(DeltaTime);
	}
}

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

void UVehicleAsyncTickComponent::UnRegister(UVehicleDriveAssemblyComponent* targetDriveAssembly)
{
	DriveAssemblies.Remove(targetDriveAssembly);
}

void UVehicleAsyncTickComponent::UpdateVehiclePhysics(float DeltaTime)
{
	if (!IsValid(this) || IsBeingDestroyed()) return;

	//update physics
	for (TWeakObjectPtr<UVehicleDriveAssemblyComponent> DriveAssemblyPtr : DriveAssemblies)
	{
		UVehicleDriveAssemblyComponent* DriveAssembly = DriveAssemblyPtr.Get();
		if (IsValid(DriveAssembly) && DriveAssembly->bUpdatePhysicsAutomatically)
		{
			DriveAssembly->UpdatePhysics(DeltaTime);
		}
	}
}

void UVehicleAsyncTickComponent::Register(UVehicleDriveAssemblyComponent* newDriveAssembly)
{
	DriveAssemblies.Add(newDriveAssembly);
}
