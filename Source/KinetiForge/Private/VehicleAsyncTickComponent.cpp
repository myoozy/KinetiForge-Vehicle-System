// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#include "VehicleAsyncTickComponent.h"
#include "VehicleDriveAssemblyComponent.h"

void UVehicleAsyncTickComponent::NativeAsyncTick(float DeltaTime)
{
	Super::NativeAsyncTick(DeltaTime);

	//update physics
	for (TWeakObjectPtr<UVehicleDriveAssemblyComponent> TempDriveAssemblyPtr : DriveAssemblies)
	{
		UVehicleDriveAssemblyComponent* TempDriveAssembly = TempDriveAssemblyPtr.Get();
		if (IsValid(TempDriveAssembly) && TempDriveAssembly->bUpdatePhysicsAutomatically)
		{
			TempDriveAssembly->UpdatePhysics(DeltaTime);
		}
	}
}

UVehicleAsyncTickComponent* UVehicleAsyncTickComponent::FindVehicleAsyncTickComponent(AActor* VehicleActor)
{
	if (!IsValid(VehicleActor))return nullptr;

	//if found
	if(UVehicleAsyncTickComponent* TempAsyncTickComp = 
		Cast<UVehicleAsyncTickComponent>(VehicleActor->GetComponentByClass(UVehicleAsyncTickComponent::StaticClass())))
	{
		return TempAsyncTickComp;
	}
	else
	{
		//if not found
		TempAsyncTickComp =
			Cast<UVehicleAsyncTickComponent>(VehicleActor->AddComponentByClass(UVehicleAsyncTickComponent::StaticClass(), false, FTransform(), false));
		return TempAsyncTickComp;
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
