// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "AsyncTickActorComponent.h"
#include "VehicleAsyncTickComponent.generated.h"

class UVehicleDriveAssemblyComponent;

/**
 * 
 */
UCLASS()
class KINETIFORGE_API UVehicleAsyncTickComponent : public UAsyncTickActorComponent
{
	GENERATED_BODY()
	
public:
	virtual void NativeAsyncTick(float DeltaTime) override;

	static UVehicleAsyncTickComponent* FindVehicleAsyncTickComponent(AActor* VehicleActor);

	void Register(UVehicleDriveAssemblyComponent* newDriveAssembly);
	void UnRegister(UVehicleDriveAssemblyComponent* targetDriveAssembly);

protected:
	TArray<TWeakObjectPtr<UVehicleDriveAssemblyComponent>> DriveAssemblies;
};
