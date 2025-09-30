// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#include "VehicleAsyncTickComponent.h"
#include "VehicleDriveAssemblyComponent.h"

void UVehicleAsyncTickComponent::NativeAsyncTick(float DeltaTime)
{
	Super::NativeAsyncTick(DeltaTime);

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

void UVehicleAsyncTickComponent::Register(UVehicleDriveAssemblyComponent* newDriveAssembly)
{
	DriveAssemblies.Add(newDriveAssembly);
}
