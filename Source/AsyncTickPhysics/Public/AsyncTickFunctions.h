// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PhysicsProxy/SingleParticlePhysicsProxy.h"
#include "AsyncTickFunctions.generated.h"

/**
 * 
 */
UCLASS()
class ASYNCTICKPHYSICS_API UAsyncTickFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "ATP - AsyncTickPhysics")
	static void ATP_AddForce(UPrimitiveComponent* Component, FVector Force, bool bAccelChange,
	                         FName BoneName = NAME_None);
	UFUNCTION(BlueprintCallable, Category = "ATP - AsyncTickPhysics")
	static void ATP_AddForceAtPosition(UPrimitiveComponent* Component, FVector Position, FVector Force,
	                                   FName BoneName = NAME_None);
	UFUNCTION(BlueprintCallable, Category = "ATP - AsyncTickPhysics")
	static void ATP_AddTorque(UPrimitiveComponent* Component, FVector Torque, bool bAccelChange,
	                          FName BoneName = NAME_None);
	UFUNCTION(BlueprintCallable, Category = "ATP - AsyncTickPhysics")
	static void ATP_AddImpulse(UPrimitiveComponent* Component, FVector Impulse, bool bVelChange,
	                           FName BoneName = NAME_None);
	UFUNCTION(BlueprintCallable, Category = "ATP - AsyncTickPhysics")
	static void ATP_AddImpulseAtPosition(UPrimitiveComponent* Component, FVector Position, FVector Impulse,
	                                     FName BoneName = NAME_None);
	UFUNCTION(BlueprintCallable, Category = "ATP - AsyncTickPhysics")
	static void ATP_AddAngularImpulseInRadians(UPrimitiveComponent* Component, FVector Torque, bool bVelChange,
	                                           FName BoneName = NAME_None);
	UFUNCTION(BlueprintCallable, Category = "ATP - AsyncTickPhysics")
	static void ATP_AddAngularImpulseInDegrees(UPrimitiveComponent* Component, FVector Torque, bool bVelChange,
	                                           FName BoneName = NAME_None);

	UFUNCTION(BlueprintCallable, Category = "ATP - AsyncTickPhysics")
	static FTransform ATP_GetTransform(UPrimitiveComponent* Component);

	UFUNCTION(BlueprintCallable, Category = "ATP - AsyncTickPhysics")
	static FVector ATP_GetLinearVelocity(UPrimitiveComponent* Component, FName BoneName = NAME_None);

	UFUNCTION(BlueprintCallable, Category = "ATP - AsyncTickPhysics")
	static FVector ATP_GetCoMPosition(UPrimitiveComponent* Component);


	UFUNCTION(BlueprintCallable, Category = "ATP - AsyncTickPhysics")
	static FVector ATP_GetAngularVelocity(UPrimitiveComponent* Component, FName BoneName = NAME_None);

	UFUNCTION(BlueprintCallable, Category = "ATP - AsyncTickPhysics")
	static void ATP_SetLinearVelocity(UPrimitiveComponent* Component, FVector Velocity, bool bAddToCurrent,
	                                  FName BoneName = NAME_None);
	UFUNCTION(BlueprintCallable, Category = "ATP - AsyncTickPhysics")
	static void ATP_SetAngularVelocityInRadians(UPrimitiveComponent* Component, FVector AngVelocity, bool bAddToCurrent,
	                                            FName BoneName = NAME_None);
	UFUNCTION(BlueprintCallable, Category = "ATP - AsyncTickPhysics")
	static void ATP_SetAngularVelocityInDegrees(UPrimitiveComponent* Component, FVector AngVelocity, bool bAddToCurrent,
	                                            FName BoneName = NAME_None);
	UFUNCTION(BlueprintCallable, Category = "ATP - AsyncTickPhysics")
	static void ATP_SetWorldLocation(USceneComponent* Component, FVector Location);
	UFUNCTION(BlueprintCallable, Category = "ATP - AsyncTickPhysics")
	static void ATP_SetWorldRotation(UPrimitiveComponent* Component, FRotator Rotation);
	UFUNCTION(BlueprintCallable, Category = "ATP - AsyncTickPhysics")
	static void ATP_SetWorldLocationAndRotation(UPrimitiveComponent* Component, FVector Location, FRotator Rotation);

	UFUNCTION(BlueprintCallable, Category = "ATP - AsyncTickPhysics")
	static FVector ATP_GetLinearVelocityAtPoint(UPrimitiveComponent* Component, FVector Point,
	                                            FName BoneName = NAME_None);

private:
	static Chaos::FRigidBodyHandle_Internal* GetInternalHandle(UPrimitiveComponent* Component,
	                                                           FName BoneName = NAME_None);
};
