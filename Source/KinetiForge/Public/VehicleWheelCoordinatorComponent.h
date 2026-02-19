// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "VehicleWheelCoordinatorComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWheelCoordinator, Log, All);

class UVehicleWheelComponent;
class UVehicleAxleAssemblyComponent;
class UVehicleDriveAssemblyComponent;

UCLASS()
class KINETIFORGE_API UVehicleWheelCoordinatorComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVehicleWheelCoordinatorComponent();
	
	// avoid refreshing in every frame
	float RefreshInterval = 1.f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnRegister() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	
	float TimeSinceLastRefresh = 0.f;

	UPROPERTY()
	TWeakObjectPtr<UPrimitiveComponent> Carbody;
	FVector3f CarbodyCOM;		//local position


	UPROPERTY()
	TArray<TWeakObjectPtr<UVehicleWheelComponent>> RegisteredWheels;
	TArray<float> StaticLoadMasses;
	bool bMassMatrixDirty = false;	//if wheel position changed, or if wheel is added or destoryed 

	UPROPERTY()
	TArray<TWeakObjectPtr<UVehicleAxleAssemblyComponent>> RegisteredAxles;
	TArray<float> WheelBases;
	bool bWheelBaseDataDirty = false;

	UPROPERTY()
	TArray<TWeakObjectPtr<UVehicleDriveAssemblyComponent>> RegisteredDriveAssemblies;

	bool UpdateWheelSprungMass();
	void UpdateWheelBase();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	static UVehicleWheelCoordinatorComponent* FindWheelCoordinator(USceneComponent* Carbody);

	void NotifyWheelMoved();
	void RegisterWheel(UVehicleWheelComponent* NewWheel);
	void RegisterAxle(UVehicleAxleAssemblyComponent* NewAxle);
	void RegisterDriveAssembly(UVehicleDriveAssemblyComponent* NewDriveAssembly);
	static bool ComputeSprungMasses(const TArray<FVector3f>& MassSpringPositions, const float TotalMass, TArray<float>& OutSprungMasses);
	static bool ComputeSprungMasses(const TArray<FVector3f>& LocalSpringPositions, const FVector3f& LocalCenterOfMass, const float TotalMass, TArray<float>& OutSprungMasses);

		
};
