// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

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
};
