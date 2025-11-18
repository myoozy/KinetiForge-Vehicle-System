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
	
	//I tried to use convex instead of sphere trace, but I failed :(
	int32 NumSegments = 8;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnRegister() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	
	UPROPERTY()
	UPrimitiveComponent* Carbody;
	FVector CarbodyCOM;		//local position


	UPROPERTY()
	TArray<TWeakObjectPtr<UVehicleWheelComponent>> RegisteredWheels;
	int NumOfWheels = 0.f;
	TArray<float> StaticLoadMasses;
	bool bMassMatrixDirty = false;	//if wheel position changed, or if wheel is added or destoryed 

	UPROPERTY()
	TArray<TWeakObjectPtr<UVehicleAxleAssemblyComponent>> RegisteredAxles;
	int NumOfAxles = 0.f;
	TArray<float> WheelBases;
	bool bWheelBaseDataDirty = false;

	UPROPERTY()
	TArray<TWeakObjectPtr<UVehicleDriveAssemblyComponent>> RegisteredDriveAssemblies;

	bool FindCarbody();
	int FindAllWheels(TArray<UVehicleWheelComponent*>& OutWheels);
	int FindAllAxles(TArray <UVehicleAxleAssemblyComponent*>& OutAxles);
	int FindAllDriveAssemblies(TArray<UVehicleDriveAssemblyComponent*>& OutDriveAssemblies);
	bool UpdateWheelSprungMass();
	void UpdateWheelBase();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	static UPrimitiveComponent* FindPhysicalParent(USceneComponent* ChildSceneComponent);
	static UVehicleWheelCoordinatorComponent* FindWheelCoordinator(USceneComponent* Carbody);

	void NotifyWheelMoved();
	void RegisterWheel(UVehicleWheelComponent* NewWheel);
	void RegisterAxle(UVehicleAxleAssemblyComponent* NewAxle);
	void RegisterDriveAssembly(UVehicleDriveAssemblyComponent* NewDriveAssembly);
	static bool ComputeSprungMasses(const TArray<FVector>& MassSpringPositions, const float TotalMass, TArray<float>& OutSprungMasses);
	static bool ComputeSprungMasses(const TArray<FVector>& LocalSpringPositions, const FVector& LocalCenterOfMass, const float TotalMass, TArray<float>& OutSprungMasses);

		
};
