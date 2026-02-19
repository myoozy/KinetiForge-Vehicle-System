// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/SimpleConstructionScript.h"
#include "Engine/SCS_Node.h"
#include "PhysicsProxy/SingleParticlePhysicsProxy.h"
#include "VehicleUtil.generated.h"

namespace Chaos
{
    class FRigidBodyHandle_Internal;
}

/**
 * Did you asked why my name is not VehicleUtility ?
 * Because there is already a VehicleUtility file for Chaos Vehicle.
 */
UCLASS()
class KINETIFORGE_API UVehicleUtil : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
	UVehicleUtil();
	~UVehicleUtil();

    UFUNCTION(BlueprintCallable, Category = "VehicleUtility", meta = (ToolTip = "kg/m^2"))
    static void SetInertiaTensor(UPrimitiveComponent* InComponent, FVector InInertia);

    UFUNCTION(BlueprintCallable, Category = "VehicleUtility")
    static void RotateCamera(
        USceneComponent* InSpringArm,
        FVector2D InMouseInput,
        bool bInvertYAxis = false,
        float InMaxPitch = 80.f);

    UFUNCTION(BlueprintCallable, Category = "VehicleUtility")
    static void CameraLookAtVelocity(
        UPrimitiveComponent* InPrimitiveComponent,
        USceneComponent* InSpringArm,
        float InPitch,
        float InSensitivity = 1.f,
        float InInterpSpeed = 0.f,
        float InStartSpeed_mps = 5.f);

    UFUNCTION(BlueprintCallable, Category = "VehicleUtility")
    static UPrimitiveComponent* FindPhysicalParent(USceneComponent* ChildSceneComponent);

    template<typename T>
    static FORCEINLINE T SafeDivide(const T& A, const T& B)
    {
        return FMath::IsNearlyZero(B) ? T(0) : A / B;
    }

    template<typename T>
    static FORCEINLINE UE::Math::TVector<T> SafeDivide(const UE::Math::TVector<T>& A, const float& B)
    {
        return FMath::IsNearlyZero(B) ? UE::Math::TVector<T>(0.f) : A / B;
    }

    template<typename T>
    static T* GetComponentByName(const AActor* Actor, const FName Name)
    {
        if (IsValid(Actor))
        {
            for (UActorComponent* Comp : Actor->GetComponents())
            {
                T* Result = Cast<T>(Comp);
                if (IsValid(Result))
                {
                    if (Result->GetName() == Name.ToString())
                    {
                        return Result;
                    }
                }
            }
        }
        return nullptr;
    }

    template<typename T>
    static TArray<FName> GetNamesOfComponentsOfActor(UObject* Obj)
    {
        TArray<FName> Names = { NAME_None };
        bool bFound = false;

        if (const AActor* Actor = Obj->GetTypedOuter<AActor>())
        {
            for (UActorComponent* Comp : Actor->GetComponents())
            {
                if (Comp && Comp->IsA<T>())
                {
                    Names.Add(Comp->GetFName());
                    bFound = true;
                }
            }
        }

        if (bFound)return Names;

        if (UObject* Outer = Obj->GetOuter())
        {
            if (UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(Outer))
            {
                if (USimpleConstructionScript* SCS = BPClass->SimpleConstructionScript)
                {
                    for (USCS_Node* Node : SCS->GetAllNodes())
                    {
                        if (Node && Node->ComponentClass && Node->ComponentClass->IsChildOf(T::StaticClass()))
                        {
                            Names.Add(Node->GetVariableName());
                            bFound = true;
                        }
                    }
                }
            }
        }
        return Names;
    }

    static inline Chaos::FRigidBodyHandle_Internal* GetInternalHandle(UPrimitiveComponent* Component, FName BoneName = NAME_None)
    {
        if (IsValid(Component))
        {
            if (const FBodyInstance* BodyInstance = Component->GetBodyInstance(BoneName))
            {
                if (const auto Handle = BodyInstance->ActorHandle)
                {
                    if (Chaos::FRigidBodyHandle_Internal* RigidHandle = Handle->GetPhysicsThreadAPI())
                    {
                        return RigidHandle;
                    }
                }
            }
        }
        return nullptr;
    }

};
