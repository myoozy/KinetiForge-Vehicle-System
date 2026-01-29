// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "AsyncTickActorComponent.h"
#include "VehicleAsyncTickComponent.generated.h"

class UVehicleDriveAssemblyComponent;
class FAsyncTickManager;

/**
 * 
 */
UCLASS()
class KINETIFORGE_API UVehicleAsyncTickComponent : public UAsyncTickActorComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnUnregister() override;
	virtual void NativeAsyncTick(float DeltaTime) override;
	virtual void AsyncPhysicsTickComponent(float DeltaTime, float SimTime) override;


	static UVehicleAsyncTickComponent* FindVehicleAsyncTickComponent(AActor* VehicleActor);

	void Register(UVehicleDriveAssemblyComponent* newDriveAssembly);
	void UnRegister(UVehicleDriveAssemblyComponent* targetDriveAssembly);

	bool bUpdatePhysicsOnGameThread = false;

protected:
	TArray<TWeakObjectPtr<UVehicleDriveAssemblyComponent>> DriveAssemblies;

private:
	void UpdateVehiclePhysics(float DeltaTime);
	FAsyncTickManager* CachedAsyncTickManager = nullptr;
};
