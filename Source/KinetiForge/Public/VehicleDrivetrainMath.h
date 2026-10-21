// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"

namespace VehicleDrivetrainMath
{
	// Match UVehicleUtilities::SafeDivide without depending on a UObject helper.
	// Shared by the value solvers; named scope also avoids Unity-build collisions.
	// The public UObject compatibility wrappers also delegate here. Denominators may
	// differ from the result type, as with a FVector divided by a float.
	template <typename T, typename TDenominator>
	FORCEINLINE T DivideOrDefault(const T& Numerator, const TDenominator& Denominator, const T& DefaultValue = T(0))
	{
		return FMath::Abs(Denominator) > SMALL_NUMBER ? Numerator / Denominator : DefaultValue;
	}
}
