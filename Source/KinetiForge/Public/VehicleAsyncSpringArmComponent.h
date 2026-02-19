// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "VehicleAsyncSpringArmComponent.generated.h"

class UVehicleAsyncTickComponent;


/**
 * Please attach this component to a primive component for best effect.
 * This is a spring arm component supporting camera lag with async physics enabled.
 * The native spring arm component of unreal engine does'nt support async physics very well.
 * When async physics and camera lag are enabled at same time, the native spring arm will jitter.
 * This component fixed that issue (almost).
 * 
 * VehicleAsyncTickComponent will be automatically generated if needed.
 * 
 * Here is description of the native spring arm:
 * 
 * This component tries to maintain its children at a fixed distance from the parent,
 * but will retract the children if there is a collision, and spring back when there is no collision.
 *
 * Example: Use as a 'camera boom' or 'selfie stick' to keep the follow camera for a player from colliding into the world.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class KINETIFORGE_API UVehicleAsyncSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

public:
	/**
	 * If LookAtVelocity is true, the spring arm will trace the direction of the velocity of closest primitive parent
	 * It will stop looking at velocity if the velocity is too small.
	 * @see MinVelocityToTrace
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraSettings, AdvancedDisplay)
	bool bLookAtVelocity = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraSettings, AdvancedDisplay, meta = (editcondition = "bEnableCameraRotationLag", ClampMin = "0.0"))
	FVector2f MinVelocityToTrace = FVector2f(100.f, 500.f);

	void UpdatePhysics(float DeltaTime);
protected:
	/** rotational bias in realtime **/
	FQuat DynArmRotOffset = FQuat::Identity;

	/** only use in physics thread **/
	FTransform AsyncDesiredArmOriginWorldTransform;

	/**  **/
	float TimeSinceLastPhysTick = 0.f;

	float LastPhysicsDeltaTime = 1.f/60.f;

	/** relative transform to physical parent **/
	FTransform ArmOriginToPhysParent/*[3]*/;

	/** smoothened relative transform to physical parent */
	FTransform DesiredTransformToPhysParent/*[3]*/;
	FTransform PrevDesiredTransformToPhysParent;

	/*TAtomic<int32> PhysicsIndex{0};*/
	/*TAtomic<int32> GameIndex{1};*/
	/*TAtomic<int32> ExchangeIndex{2};*/

	/** Primitive parent **/
	UPROPERTY()
	TWeakObjectPtr<UPrimitiveComponent> PhysicalParent;

	/** the component that send async tick **/
	UPROPERTY()
	TWeakObjectPtr<UVehicleAsyncTickComponent> AsyncTickComponent;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	// when destroyed
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	/** Updates the desired arm location, calling BlendLocations to do the actual blending if a trace is done */
	virtual void UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime) override;

};
