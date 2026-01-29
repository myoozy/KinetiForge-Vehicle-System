// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/SimpleConstructionScript.h"
#include "Engine/SCS_Node.h"
#include "PhysicsProxy/SingleParticlePhysicsProxy.h"

/**
 * 
 */
class KINETIFORGE_API VehicleUtil
{
public:
	VehicleUtil();
	~VehicleUtil();

    template<typename T>
    static FORCEINLINE T SafeDivide(const T& A, const T& B)
    {
        return FMath::IsNearlyZero(B) ? T(0) : A / B;
    }

    static FORCEINLINE FVector3f SafeDivide(const FVector3f& A, const float& B)
    {
        return FMath::IsNearlyZero(B) ? FVector3f(0.f) : A / B;
    }

    template<typename T>
    static FORCEINLINE T* GetComponentByName(const AActor* Actor, const FName Name)
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
    static FORCEINLINE TArray<FName> GetNamesOfComponentsOfActor(UObject* Obj)
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

        /*
        if (UActorComponent* ObjComp = Cast<UActorComponent>(Obj))
        {
            if (AActor* Owner = ObjComp->GetOwner())
            {
                for (UActorComponent* Comp : Owner->GetComponents())
                {
                    if (T* c = Cast<T>(Comp))
                    {
                        Names.Add(FName(Comp->GetName()));
                        bFound = true;
                    }
                }
            }
        }
        */
        
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

    static FORCEINLINE Chaos::FRigidBodyHandle_Internal* GetInternalHandle(UPrimitiveComponent* Component, FName BoneName)
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
