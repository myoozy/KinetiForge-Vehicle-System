// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "AsyncTickActorComponent.h"
#include "HAL/CriticalSection.h"
#include "VehicleAsyncTickComponent.generated.h"

class UVehicleDriveAssemblyComponent;
class UVehicleWheelCoordinatorComponent;
class UVehicleAsyncSpringArmComponent;
class FVehiclePhysicsStep;

/**
 * This component recieves callback from physics thread and calls other components to update physics.
 * This component will be automatically generated if needed.
 */
UCLASS()
class KINETIFORGE_API UVehicleAsyncTickComponent : public UAsyncTickActorComponent
{
	GENERATED_BODY()
	friend struct FVehicleAsyncTickRegistrationProbe;
	friend struct FVehicleAsyncTickBeginDestroyProbe;

public:
	bool bUpdatePhysicsOnGameThread = false;

	static UVehicleAsyncTickComponent* FindVehicleAsyncTickComponent(AActor* VehicleActor);

	void RegisterDriveAssembly(UVehicleDriveAssemblyComponent* newDriveAssembly);
	void UnRegisterDriveAssembly(UVehicleDriveAssemblyComponent* targetDriveAssembly);

	void RegisterWheelCoordinator(UVehicleWheelCoordinatorComponent* newWheelCoordinator);
	void UnRegisterWheelCoordinator(UVehicleWheelCoordinatorComponent* targetWheelCoordinator);

	void RegisterAsyncSpringArm(UVehicleAsyncSpringArmComponent* newAsyncSpringArm);
	void UnRegisterAsyncSpringArm(UVehicleAsyncSpringArmComponent* targetAsyncSpringArm);

protected:
	TArray<TWeakObjectPtr<UVehicleDriveAssemblyComponent>> DriveAssemblies;
	TArray<TWeakObjectPtr<UVehicleWheelCoordinatorComponent>> WheelCoordinators;
	TArray<TWeakObjectPtr<UVehicleAsyncSpringArmComponent>> AsyncSpringArms;

protected:
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnUnregister() override;
	virtual void BeginDestroy() override;

	/** This function runs on worker thread (not safe but much better performance). */
	virtual void NativeAsyncTick(float DeltaTime) override;

	/**
	* This function runs on frozen game thread, it is thread-safe but the performance is bad.
	* Aero/spring-arm UObject state is updated here; Drive steps use the scene callback selected during binding.
	*/
	virtual void AsyncPhysicsTickComponent(float DeltaTime, float SimTime) override;

private:
	void StopDrivePhysicsSteps(bool bForgetDrives);
	mutable FCriticalSection RegistrationMutex;
	TMap<TWeakObjectPtr<UVehicleDriveAssemblyComponent>, TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe>> DrivePhysicsSteps;
	void UpdateAuxiliaryPhysics(float DeltaTime);
};
