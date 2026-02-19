// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "AsyncTickActorComponent.h"
#include "VehicleAsyncTickComponent.generated.h"

class UVehicleDriveAssemblyComponent;
class UVehicleAsyncSpringArmComponent;
class FAsyncTickManager;

/**
 * This component recieves callback from physics thread and calls other components to update physics.
 * This component will be automatically generated if needed.
 */
UCLASS()
class KINETIFORGE_API UVehicleAsyncTickComponent : public UAsyncTickActorComponent
{
	GENERATED_BODY()

public:
	bool bUpdatePhysicsOnGameThread = false;

	static UVehicleAsyncTickComponent* FindVehicleAsyncTickComponent(AActor* VehicleActor);

	void Register(UVehicleDriveAssemblyComponent* newDriveAssembly);
	void UnRegister(UVehicleDriveAssemblyComponent* targetDriveAssembly);

	void Register(UVehicleAsyncSpringArmComponent* newAsyncSpringArm);
	void UnRegister(UVehicleAsyncSpringArmComponent* targetAsyncSpringArm);

protected:
	TArray<TWeakObjectPtr<UVehicleDriveAssemblyComponent>> DriveAssemblies;
	TArray<TWeakObjectPtr<UVehicleAsyncSpringArmComponent>> AsyncSpringArms;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnUnregister() override;

	/**
	* This function runs on worker thread (not safe but much better performance)
	*/
	virtual void NativeAsyncTick(float DeltaTime) override;

	/**
	* This function runs on frozen game thread, it is thread-safe but the performance is bad
	*/
	virtual void AsyncPhysicsTickComponent(float DeltaTime, float SimTime) override;

private:
	void UpdateVehiclePhysics(float DeltaTime);
	void UpdateAsyncSpringArms(float DeltaTime);
	FAsyncTickManager* CachedAsyncTickManager = nullptr;
};
