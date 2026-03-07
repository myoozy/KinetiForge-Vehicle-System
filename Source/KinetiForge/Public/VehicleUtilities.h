// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/SimpleConstructionScript.h"
#include "Engine/SCS_Node.h"
#include "PhysicsProxy/SingleParticlePhysicsProxy.h"
#include "VehicleUtilities.generated.h"

namespace Chaos
{
    class FRigidBodyHandle_Internal;
}

/**
 * Did you asked why my name is not VehicleUtility ?
 * Because there is already a VehicleUtility file for Chaos Vehicle.
 */
UCLASS()
class KINETIFORGE_API UVehicleUtilities : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
	UVehicleUtilities();
	~UVehicleUtilities();

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

    UFUNCTION(BlueprintCallable, Category = "VehicleUtility")
    static void DrawDebugCapsuleEasy(
        UWorld* World, FVector Start, FVector End, float Radius = 10.f,
        float Thickness = 1.f, float LifeTime = -1.f, FColor Color = FColor::White
    );

    template<typename T>
    static T CatmullRom(T P0, T P1, T P2, T P3, float t)
    {
        return 0.5f * (
            (2.0f * P1) +
            (-P0 + P2) * t +
            (2.0f * P0 - 5.0f * P1 + 4.0f * P2 - P3) * (t * t) +
            (-P0 + 3.0f * P1 - 3.0f * P2 + P3) * (t * t * t)
            );
    }

    static void FindCatmullRomLocalMax(float P0, float P1, float P2, float P3, float& OutMax, float& OutTime)
    {
        float a = 0.5f * (-P0 + 3.0f * P1 - 3.0f * P2 + P3);
        float b = 0.5f * (2.0f * P0 - 5.0f * P1 + 4.0f * P2 - P3);
        float c = 0.5f * (-P0 + P2);
        float d = P1;

        // FirstDerivative
        float A = 3.0f * a;
        float B = 2.0f * b;
        float C = c;

        OutTime = 0.f;
        OutMax = d; // f(0) = d = P1

        float ValueAt1 = a + b + c + d; // f(1) = P2
        if (ValueAt1 > OutMax)
        {
            OutMax = ValueAt1;
            OutTime = 1.0f;
        }

        TArray<float> ValidRoots;

        if (FMath::IsNearlyZero(A))
        {
            if (!FMath::IsNearlyZero(B))
            {
                float t = -C / B;
                if (t > 0.f && t < 1.f) ValidRoots.Add(t);
            }
        }
        else
        {
            float Delta = B * B - 4.0f * A * C;

            if (Delta >= 0.f)
            {
                float SqrtDelta = FMath::Sqrt(Delta);
                float t1 = (-B + SqrtDelta) / (2.0f * A);
                float t2 = (-B - SqrtDelta) / (2.0f * A);

                if (t1 > 0.f && t1 < 1.f) ValidRoots.Add(t1);
                if (t2 > 0.f && t2 < 1.f) ValidRoots.Add(t2);
            }
        }

        for (float t : ValidRoots)
        {
            float SecondDerivative = 2.0f * A * t + B;

            if (SecondDerivative < 0.f)
            {
                float LocalMax = a * (t * t * t) + b * (t * t) + c * t + d;

                if (LocalMax > OutMax)
                {
                    OutMax = LocalMax;
                    OutTime = t;
                }
            }
        }
    }

    template<typename T>
    static FORCEINLINE T SafeDivide(const T& A, const T& B, const T DefaultValue = T(0))
    {
        return FMath::IsNearlyZero(B) ? DefaultValue : A / B;
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
        if (!Obj) return Names;

        AActor* ActorOuter = Obj->GetTypedOuter<AActor>();
        UClass* ActorClass = nullptr;
        if (ActorOuter)
        {
            ActorClass = ActorOuter->GetClass();

            for (UActorComponent* Comp : ActorOuter->GetComponents())
            {
                if (Comp && Comp->IsA<T>())
                {
                    Names.AddUnique(Comp->GetFName());
                }
            }
        }
        else
        {
            ActorClass = Obj->GetTypedOuter<UClass>();
        }

        UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(ActorClass);
        while (BPClass)
        {
            if (USimpleConstructionScript* SCS = BPClass->SimpleConstructionScript)
            {
                for (USCS_Node* Node : SCS->GetAllNodes())
                {
                    if (Node && Node->ComponentClass && Node->ComponentClass->IsChildOf(T::StaticClass()))
                    {
                        Names.AddUnique(Node->GetVariableName());
                    }
                }
            }
            BPClass = Cast<UBlueprintGeneratedClass>(BPClass->GetSuperClass());
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

    template<typename T>
    static T* CreateComponentByClass(
        UObject* Outer,
        const TSubclassOf<UActorComponent> Subclass = nullptr,
        const FName Name = NAME_None,
        const EComponentCreationMethod CreationMethod = EComponentCreationMethod::UserConstructionScript,
        const EObjectFlags Flags = RF_NoFlags,
        const bool bAddInstanceComponent = true,
        const bool bActorAsOuter = true)
    {
        if (!IsValid(Outer)) return nullptr;

        UClass* ClassToSpawn = Subclass ? Subclass.Get() : T::StaticClass();

        if (UWorld* World = Outer->GetWorld())
        {
            AActor* Actor = nullptr;

            if (AActor* a = Cast<AActor>(Outer))
            {
                Actor = a;
            }
            else if (UActorComponent* c = Cast<UActorComponent>(Outer))
            {
                Actor = c->GetOwner();
            }

            UObject* FinalOuter = Outer;
            if (Actor && bActorAsOuter)
            {
                FinalOuter = Actor;
            }

            if (T* Obj = NewObject<T>(FinalOuter, ClassToSpawn, Name, Flags))
            {
                Obj->CreationMethod = CreationMethod;

                if (Actor && bAddInstanceComponent)
                {
                    Actor->AddInstanceComponent(Obj);
                }

                Obj->RegisterComponent();

                return Obj;
            }
        }

        return nullptr;
    }
};


/*
* The result of evaluation
*/
USTRUCT(BlueprintType)
struct FVehicleLUT_EvalResult
{
    GENERATED_BODY()

    UPROPERTY()
    float Value;

    /*
    * This may not be the actual tangent, 
    * since the time axis of look up table is normalized
    */
    UPROPERTY()
    float RightTangent;
};

/*
* A simple look up table.
* The time axis is normalized. ( 0 <= Time <=1 )
*/
template <int32 NumSamples>
struct KINETIFORGE_API FVehicleLUT
{
private:
    /**
    * Data array
    */
    TStaticArray<float, NumSamples> Samples;

public:
    /*
    * Get number of elements
    */
    int32 Num() const { return Samples.Num(); }

    /*
    * Set value of all samples to a given constant
    */
    void SetAllTo(const float NewValue)
    {
        const int32 NumOfSamples = Samples.Num();
        for (int32 i = 0; i < NumOfSamples; i++)
        {
            Samples[i] = NewValue;
        }
    }

    /*
    * Copy data from a FRichCurve.
    * Time interval is optional. Only data in the interval will be copied.
    * If the interval is too small, the interval will be the time interval of first and the lasy key of the FRichCurve
    */
    void CopyFromRichCurve(const FRichCurve& RichCurve, const FVector2f SelectedTimeInterval = FVector2f(0.f, 1.f))
    {
        float TimeMin = 0.f;
        float TimeMax = 1.f;

        if (FMath::Abs(SelectedTimeInterval.Y - SelectedTimeInterval.X) > SMALL_NUMBER)
        {
            // make sure TimeMin <= TimeMax
            TimeMin = SelectedTimeInterval.GetMin();
            TimeMax = SelectedTimeInterval.GetMax();
        }
        else
        {
            int32 NumOfKeys = RichCurve.Keys.Num();
            /*
            * If there is only one key or if there is no key,
            * the time interval doesn't really matter.
            * We only care when there're more than 1 key.
            */
            if (NumOfKeys > 1)
            {
                TimeMin = RichCurve.Keys[0].Time;
                TimeMax = RichCurve.Keys[NumOfKeys - 1].Time;
            }
        }

        /*
        * Save the data
        */
        const int32 NumOfSamples = Samples.Num();
        const int32 LastKeyIdx = NumOfSamples - 1;
        const float dt = UVehicleUtilities::SafeDivide((TimeMax - TimeMin), (float)LastKeyIdx);
        for (int32 i = 0; i < NumOfSamples; i++)
        {
            float CurrTime = TimeMin + dt * (float)i;
            Samples[i] = RichCurve.Eval(CurrTime);
        }
    }

    /*
    * Use interpolation only once to evaluate.
    * Returns value and tagent (right side)
    */
    FORCEINLINE FVehicleLUT_EvalResult FastEval(float Progress) const
    {
        Progress = FMath::Clamp(Progress, 0.f, 1.f);

        const int32 NumOfSamples = Samples.Num();
        const int32 LastIdx = NumOfSamples - 1;

        const float FloatLastIdx = (float)(LastIdx);
        const float FloatIndex = Progress * FloatLastIdx;

        // get the range of index
        int32 idx_low = (int32)FloatIndex;
        int32 idx_high = idx_low + 1 - (idx_low == LastIdx);

        // read value
        const float Val_Low = Samples[idx_low];
        const float Val_High = Samples[idx_high];

        // get bias
        float Bias = Progress * LastIdx - (float)idx_low;

        // get value
        float Value = FMath::Lerp(
            Val_Low,
            Val_High,
            Bias
        );

        // get tangent
        float RightTangent = (Val_High - Val_Low) * FloatLastIdx;

        return FVehicleLUT_EvalResult(Value, RightTangent);
    }

    FORCEINLINE FVehicleLUT_EvalResult FastEval(const float Time, const FVector2f& TimeInterval) const
    {
        const float Progress = FMath::GetMappedRangeValueUnclamped(
            TimeInterval,
            FVector2f(0.f, 1.f),
            Time
        );

        FVehicleLUT_EvalResult Result = FastEval(Progress);

        const float TimeIntervalLength = TimeInterval.Y - TimeInterval.X;

        Result.RightTangent = UVehicleUtilities::SafeDivide(Result.RightTangent, TimeIntervalLength);

        return Result;
    }

    /*
    * Using Catmull-Rom cubic interpolation for evaluation
    * Advantages: Delivers perfectly smooth curves and continuous derivatives even with very few sampling points (e.g., 16 or 32)
    * Disadvantages: More computationally intensive than linear interpolation
    */
    FORCEINLINE FVehicleLUT_EvalResult CubicEval(float Progress) const
    {
        Progress = FMath::Clamp(Progress, 0.f, 1.f);
        const int32 NumOfSamples = Samples.Num();
        const int32 LastIdx = NumOfSamples - 1;
        const float FloatLastIdx = (float)(LastIdx);
        const float FloatIndex = Progress * FloatLastIdx;

        // Two key points
        const int32 idx1 = (int32)FloatIndex;
        const int32 idx2 = idx1 + 1 - (idx1 == LastIdx);

        // Acquire two additional control points before and after (note boundary constraints)
        const int32 idx0 = idx1 - 1 + (idx1 == 0);
        const int32 idx3 = idx2 + 1 - (idx2 == LastIdx);

        // read value of 4 points
        const float p0 = Samples[idx0];
        const float p1 = Samples[idx1];
        const float p2 = Samples[idx2];
        const float p3 = Samples[idx3];

        // Calculate the fractional part t and its powers
        const float t = FloatIndex - (float)idx1;
        const float t2 = t * t;
        const float t3 = t2 * t;

        // Catmull-Rom polynom (Value)
        const float Value = 0.5f * (
            (2.f * p1) +
            (-p0 + p2) * t +
            (2.f * p0 - 5.f * p1 + 4.f * p2 - p3) * t2 +
            (-p0 + 3.f * p1 - 3.f * p2 + p3) * t3
            );

        // Derive polynomials to obtain accurate tangents.
        const float Tangent_t = 0.5f * (
            (-p0 + p2) +
            2.f * (2.f * p0 - 5.f * p1 + 4.f * p2 - p3) * t +
            3.f * (-p0 + 3.f * p1 - 3.f * p2 + p3) * t2
            );

        // Chain Rule: Multiply by the scaling factor of the index to restore the tangent to its correct spatial proportion.
        const float RightTangent = Tangent_t * FloatLastIdx;

        return FVehicleLUT_EvalResult(Value, RightTangent);
    }

    FORCEINLINE FVehicleLUT_EvalResult CubicEval(const float Time, const FVector2f& TimeInterval) const
    {
        const float Progress = FMath::GetMappedRangeValueUnclamped(
            TimeInterval,
            FVector2f(0.f, 1.f),
            Time
        );

        FVehicleLUT_EvalResult Result = CubicEval(Progress);

        const float TimeIntervalLength = TimeInterval.Y - TimeInterval.X;

        Result.RightTangent = UVehicleUtilities::SafeDivide(Result.RightTangent, TimeIntervalLength);

        return Result;
    }

    /*----- CONSTRUCTION -----*/

    FVehicleLUT(const float InitValue = 0.f)
    {
        SetAllTo(InitValue);
    }

    FVehicleLUT(const FRichCurve& RichCurve, const FVector2f SelectedTimeInterval = FVector2f(0.f))
    {
        CopyFromRichCurve(RichCurve, SelectedTimeInterval);
    }
};