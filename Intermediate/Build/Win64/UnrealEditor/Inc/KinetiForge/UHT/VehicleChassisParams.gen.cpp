// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "KinetiForge/Public/VehicleChassisParams.h"
#include "../../Source/Runtime/Engine/Classes/Engine/HitResult.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeVehicleChassisParams() {}
// Cross Module References
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FTransform();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector2D();
	ENGINE_API UClass* Z_Construct_UClass_UCurveFloat_NoRegister();
	ENGINE_API UEnum* Z_Construct_UEnum_Engine_ECollisionChannel();
	ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FHitResult();
	KINETIFORGE_API UEnum* Z_Construct_UEnum_KinetiForge_EPositionToApplyForce();
	KINETIFORGE_API UEnum* Z_Construct_UEnum_KinetiForge_ESuspensionRayCastMode();
	KINETIFORGE_API UEnum* Z_Construct_UEnum_KinetiForge_ESuspensionType();
	KINETIFORGE_API UEnum* Z_Construct_UEnum_KinetiForge_ETireFrictionCombineMode();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleABSConfig();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleAxleConfig();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleAxleSimData();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleSuspensionSimData();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleTCSConfig();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleTireConfig();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleWheelConfig();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleWheelSimData();
	UPackage* Z_Construct_UPackage__Script_KinetiForge();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_ESuspensionType;
	static UEnum* ESuspensionType_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_ESuspensionType.OuterSingleton)
		{
			Z_Registration_Info_UEnum_ESuspensionType.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_KinetiForge_ESuspensionType, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("ESuspensionType"));
		}
		return Z_Registration_Info_UEnum_ESuspensionType.OuterSingleton;
	}
	template<> KINETIFORGE_API UEnum* StaticEnum<ESuspensionType>()
	{
		return ESuspensionType_StaticEnum();
	}
	struct Z_Construct_UEnum_KinetiForge_ESuspensionType_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_KinetiForge_ESuspensionType_Statics::Enumerators[] = {
		{ "ESuspensionType::StraightLine", (int64)ESuspensionType::StraightLine },
		{ "ESuspensionType::Macpherson", (int64)ESuspensionType::Macpherson },
		{ "ESuspensionType::DoubleWishbone", (int64)ESuspensionType::DoubleWishbone },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_KinetiForge_ESuspensionType_Statics::Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "DoubleWishbone.Comment", "/**\n * \n */" },
		{ "DoubleWishbone.Name", "ESuspensionType::DoubleWishbone" },
		{ "Macpherson.Comment", "/**\n * \n */" },
		{ "Macpherson.Name", "ESuspensionType::Macpherson" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
		{ "StraightLine.Comment", "/**\n * \n */" },
		{ "StraightLine.Name", "ESuspensionType::StraightLine" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_KinetiForge_ESuspensionType_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		"ESuspensionType",
		"ESuspensionType",
		Z_Construct_UEnum_KinetiForge_ESuspensionType_Statics::Enumerators,
		RF_Public|RF_Transient|RF_MarkAsNative,
		UE_ARRAY_COUNT(Z_Construct_UEnum_KinetiForge_ESuspensionType_Statics::Enumerators),
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_KinetiForge_ESuspensionType_Statics::Enum_MetaDataParams), Z_Construct_UEnum_KinetiForge_ESuspensionType_Statics::Enum_MetaDataParams)
	};
	UEnum* Z_Construct_UEnum_KinetiForge_ESuspensionType()
	{
		if (!Z_Registration_Info_UEnum_ESuspensionType.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_ESuspensionType.InnerSingleton, Z_Construct_UEnum_KinetiForge_ESuspensionType_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_ESuspensionType.InnerSingleton;
	}
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_ESuspensionRayCastMode;
	static UEnum* ESuspensionRayCastMode_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_ESuspensionRayCastMode.OuterSingleton)
		{
			Z_Registration_Info_UEnum_ESuspensionRayCastMode.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_KinetiForge_ESuspensionRayCastMode, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("ESuspensionRayCastMode"));
		}
		return Z_Registration_Info_UEnum_ESuspensionRayCastMode.OuterSingleton;
	}
	template<> KINETIFORGE_API UEnum* StaticEnum<ESuspensionRayCastMode>()
	{
		return ESuspensionRayCastMode_StaticEnum();
	}
	struct Z_Construct_UEnum_KinetiForge_ESuspensionRayCastMode_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_KinetiForge_ESuspensionRayCastMode_Statics::Enumerators[] = {
		{ "ESuspensionRayCastMode::LineTrace", (int64)ESuspensionRayCastMode::LineTrace },
		{ "ESuspensionRayCastMode::SphereTrace", (int64)ESuspensionRayCastMode::SphereTrace },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_KinetiForge_ESuspensionRayCastMode_Statics::Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "LineTrace.Name", "ESuspensionRayCastMode::LineTrace" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
		{ "SphereTrace.Name", "ESuspensionRayCastMode::SphereTrace" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_KinetiForge_ESuspensionRayCastMode_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		"ESuspensionRayCastMode",
		"ESuspensionRayCastMode",
		Z_Construct_UEnum_KinetiForge_ESuspensionRayCastMode_Statics::Enumerators,
		RF_Public|RF_Transient|RF_MarkAsNative,
		UE_ARRAY_COUNT(Z_Construct_UEnum_KinetiForge_ESuspensionRayCastMode_Statics::Enumerators),
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_KinetiForge_ESuspensionRayCastMode_Statics::Enum_MetaDataParams), Z_Construct_UEnum_KinetiForge_ESuspensionRayCastMode_Statics::Enum_MetaDataParams)
	};
	UEnum* Z_Construct_UEnum_KinetiForge_ESuspensionRayCastMode()
	{
		if (!Z_Registration_Info_UEnum_ESuspensionRayCastMode.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_ESuspensionRayCastMode.InnerSingleton, Z_Construct_UEnum_KinetiForge_ESuspensionRayCastMode_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_ESuspensionRayCastMode.InnerSingleton;
	}
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_EPositionToApplyForce;
	static UEnum* EPositionToApplyForce_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_EPositionToApplyForce.OuterSingleton)
		{
			Z_Registration_Info_UEnum_EPositionToApplyForce.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_KinetiForge_EPositionToApplyForce, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("EPositionToApplyForce"));
		}
		return Z_Registration_Info_UEnum_EPositionToApplyForce.OuterSingleton;
	}
	template<> KINETIFORGE_API UEnum* StaticEnum<EPositionToApplyForce>()
	{
		return EPositionToApplyForce_StaticEnum();
	}
	struct Z_Construct_UEnum_KinetiForge_EPositionToApplyForce_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_KinetiForge_EPositionToApplyForce_Statics::Enumerators[] = {
		{ "EPositionToApplyForce::ImpactPoint", (int64)EPositionToApplyForce::ImpactPoint },
		{ "EPositionToApplyForce::ImpactPointWithBias", (int64)EPositionToApplyForce::ImpactPointWithBias },
		{ "EPositionToApplyForce::WheelCenter", (int64)EPositionToApplyForce::WheelCenter },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_KinetiForge_EPositionToApplyForce_Statics::Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ImpactPoint.Name", "EPositionToApplyForce::ImpactPoint" },
		{ "ImpactPoint.ToolTip", "Apply suspension and tire force at the impact point of the ray-casting" },
		{ "ImpactPointWithBias.Name", "EPositionToApplyForce::ImpactPointWithBias" },
		{ "ImpactPointWithBias.ToolTip", "Apply force at ImpactPoint + WheelRightVector * HalfWheelWidth" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
		{ "WheelCenter.Name", "EPositionToApplyForce::WheelCenter" },
		{ "WheelCenter.ToolTip", "Apply suspension and tire force at the center of wheel" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_KinetiForge_EPositionToApplyForce_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		"EPositionToApplyForce",
		"EPositionToApplyForce",
		Z_Construct_UEnum_KinetiForge_EPositionToApplyForce_Statics::Enumerators,
		RF_Public|RF_Transient|RF_MarkAsNative,
		UE_ARRAY_COUNT(Z_Construct_UEnum_KinetiForge_EPositionToApplyForce_Statics::Enumerators),
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_KinetiForge_EPositionToApplyForce_Statics::Enum_MetaDataParams), Z_Construct_UEnum_KinetiForge_EPositionToApplyForce_Statics::Enum_MetaDataParams)
	};
	UEnum* Z_Construct_UEnum_KinetiForge_EPositionToApplyForce()
	{
		if (!Z_Registration_Info_UEnum_EPositionToApplyForce.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EPositionToApplyForce.InnerSingleton, Z_Construct_UEnum_KinetiForge_EPositionToApplyForce_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_EPositionToApplyForce.InnerSingleton;
	}
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_ETireFrictionCombineMode;
	static UEnum* ETireFrictionCombineMode_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_ETireFrictionCombineMode.OuterSingleton)
		{
			Z_Registration_Info_UEnum_ETireFrictionCombineMode.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_KinetiForge_ETireFrictionCombineMode, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("ETireFrictionCombineMode"));
		}
		return Z_Registration_Info_UEnum_ETireFrictionCombineMode.OuterSingleton;
	}
	template<> KINETIFORGE_API UEnum* StaticEnum<ETireFrictionCombineMode>()
	{
		return ETireFrictionCombineMode_StaticEnum();
	}
	struct Z_Construct_UEnum_KinetiForge_ETireFrictionCombineMode_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_KinetiForge_ETireFrictionCombineMode_Statics::Enumerators[] = {
		{ "ETireFrictionCombineMode::Constant", (int64)ETireFrictionCombineMode::Constant },
		{ "ETireFrictionCombineMode::Average", (int64)ETireFrictionCombineMode::Average },
		{ "ETireFrictionCombineMode::Multiply", (int64)ETireFrictionCombineMode::Multiply },
		{ "ETireFrictionCombineMode::Min", (int64)ETireFrictionCombineMode::Min },
		{ "ETireFrictionCombineMode::Max", (int64)ETireFrictionCombineMode::Max },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_KinetiForge_ETireFrictionCombineMode_Statics::Enum_MetaDataParams[] = {
		{ "Average.Name", "ETireFrictionCombineMode::Average" },
		{ "BlueprintType", "true" },
		{ "Constant.Name", "ETireFrictionCombineMode::Constant" },
		{ "Max.Name", "ETireFrictionCombineMode::Max" },
		{ "Min.Name", "ETireFrictionCombineMode::Min" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
		{ "Multiply.Name", "ETireFrictionCombineMode::Multiply" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_KinetiForge_ETireFrictionCombineMode_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		"ETireFrictionCombineMode",
		"ETireFrictionCombineMode",
		Z_Construct_UEnum_KinetiForge_ETireFrictionCombineMode_Statics::Enumerators,
		RF_Public|RF_Transient|RF_MarkAsNative,
		UE_ARRAY_COUNT(Z_Construct_UEnum_KinetiForge_ETireFrictionCombineMode_Statics::Enumerators),
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_KinetiForge_ETireFrictionCombineMode_Statics::Enum_MetaDataParams), Z_Construct_UEnum_KinetiForge_ETireFrictionCombineMode_Statics::Enum_MetaDataParams)
	};
	UEnum* Z_Construct_UEnum_KinetiForge_ETireFrictionCombineMode()
	{
		if (!Z_Registration_Info_UEnum_ETireFrictionCombineMode.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_ETireFrictionCombineMode.InnerSingleton, Z_Construct_UEnum_KinetiForge_ETireFrictionCombineMode_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_ETireFrictionCombineMode.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_VehicleWheelConfig;
class UScriptStruct* FVehicleWheelConfig::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_VehicleWheelConfig.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_VehicleWheelConfig.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FVehicleWheelConfig, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("VehicleWheelConfig"));
	}
	return Z_Registration_Info_UScriptStruct_VehicleWheelConfig.OuterSingleton;
}
template<> KINETIFORGE_API UScriptStruct* StaticStruct<FVehicleWheelConfig>()
{
	return FVehicleWheelConfig::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Radius_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_Radius;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Width_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_Width;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Inertia_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_Inertia;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RollingRisistance_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_RollingRisistance;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FVehicleWheelConfig>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::NewProp_Radius_MetaData[] = {
		{ "Category", "VehicleWheelConfig" },
		{ "ClampMin", "0.0" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//WheelSetup\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "WheelSetup" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::NewProp_Radius = { "Radius", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelConfig, Radius), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::NewProp_Radius_MetaData), Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::NewProp_Radius_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::NewProp_Width_MetaData[] = {
		{ "Category", "VehicleWheelConfig" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::NewProp_Width = { "Width", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelConfig, Width), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::NewProp_Width_MetaData), Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::NewProp_Width_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::NewProp_Inertia_MetaData[] = {
		{ "Category", "VehicleWheelConfig" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::NewProp_Inertia = { "Inertia", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelConfig, Inertia), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::NewProp_Inertia_MetaData), Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::NewProp_Inertia_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::NewProp_RollingRisistance_MetaData[] = {
		{ "Category", "VehicleWheelConfig" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "This is just a constant additional handbrake torque" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::NewProp_RollingRisistance = { "RollingRisistance", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelConfig, RollingRisistance), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::NewProp_RollingRisistance_MetaData), Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::NewProp_RollingRisistance_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::NewProp_Radius,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::NewProp_Width,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::NewProp_Inertia,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::NewProp_RollingRisistance,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		&NewStructOps,
		"VehicleWheelConfig",
		Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::PropPointers),
		sizeof(FVehicleWheelConfig),
		alignof(FVehicleWheelConfig),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FVehicleWheelConfig()
	{
		if (!Z_Registration_Info_UScriptStruct_VehicleWheelConfig.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_VehicleWheelConfig.InnerSingleton, Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_VehicleWheelConfig.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_VehicleTireConfig;
class UScriptStruct* FVehicleTireConfig::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_VehicleTireConfig.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_VehicleTireConfig.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FVehicleTireConfig, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("VehicleTireConfig"));
	}
	return Z_Registration_Info_UScriptStruct_VehicleTireConfig.OuterSingleton;
}
template<> KINETIFORGE_API UScriptStruct* StaticStruct<FVehicleTireConfig>()
{
	return FVehicleTireConfig::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FVehicleTireConfig_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_FrictionMultiplier_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_FrictionMultiplier;
		static const UECodeGen_Private::FBytePropertyParams NewProp_TireFrictionCombineMode_Underlying;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TireFrictionCombineMode_MetaData[];
#endif
		static const UECodeGen_Private::FEnumPropertyParams NewProp_TireFrictionCombineMode;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RelaxationLength_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_RelaxationLength;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MaxFx_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_MaxFx;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MaxFy_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_MaxFy;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Fx_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Fx;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Fy_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Fy;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Gx_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Gx;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Gy_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Gy;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FVehicleTireConfig>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_FrictionMultiplier_MetaData[] = {
		{ "Category", "VehicleTireConfig" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_FrictionMultiplier = { "FrictionMultiplier", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleTireConfig, FrictionMultiplier), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_FrictionMultiplier_MetaData), Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_FrictionMultiplier_MetaData) };
	const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_TireFrictionCombineMode_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_TireFrictionCombineMode_MetaData[] = {
		{ "Category", "VehicleTireConfig" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_TireFrictionCombineMode = { "TireFrictionCombineMode", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleTireConfig, TireFrictionCombineMode), Z_Construct_UEnum_KinetiForge_ETireFrictionCombineMode, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_TireFrictionCombineMode_MetaData), Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_TireFrictionCombineMode_MetaData) }; // 869290566
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_RelaxationLength_MetaData[] = {
		{ "Category", "VehicleTireConfig" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Higher value makes the wheel softer. If the vahicle is not stable at low physics sim frequency/low vehicle weight, try to set this higher." },
#endif
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_RelaxationLength = { "RelaxationLength", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleTireConfig, RelaxationLength), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_RelaxationLength_MetaData), Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_RelaxationLength_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_MaxFx_MetaData[] = {
		{ "Category", "VehicleTireConfig" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "This value determines the length of the semi-axis of the friction ellipse. The larger this value, the greater the longitudinal force that the tire can provide." },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_MaxFx = { "MaxFx", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleTireConfig, MaxFx), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_MaxFx_MetaData), Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_MaxFx_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_MaxFy_MetaData[] = {
		{ "Category", "VehicleTireConfig" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "This value determines the length of the semi-axis of the friction ellipse. The larger this value, the greater the lateral force that the tire can provide." },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_MaxFy = { "MaxFy", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleTireConfig, MaxFy), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_MaxFy_MetaData), Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_MaxFy_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_Fx_MetaData[] = {
		{ "Category", "VehicleTireConfig" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Input: absolute value of slip ratio; Output: normalized unscaled longitudinal tire force; If this curve is not set, the tire will be treated as a rigid body in the longitudinal direction." },
#endif
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_Fx = { "Fx", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleTireConfig, Fx), Z_Construct_UClass_UCurveFloat_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_Fx_MetaData), Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_Fx_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_Fy_MetaData[] = {
		{ "Category", "VehicleTireConfig" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Input: absolute value of slip angle(in degrees); Output: normalized unscaled lateral tire force; If this curve is not set, the tire will be treated as a rigid body in the lateral direction." },
#endif
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_Fy = { "Fy", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleTireConfig, Fy), Z_Construct_UClass_UCurveFloat_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_Fy_MetaData), Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_Fy_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_Gx_MetaData[] = {
		{ "Category", "VehicleTireConfig" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Input: absolute value of slip angle(in degrees); Output: scaling factor for Fx; Through this curve, the distribution of longitudinal forces can be manually adjusted. If no settings are made, the distribution of longitudinal forces will follow the classic friction ellipse." },
#endif
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_Gx = { "Gx", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleTireConfig, Gx), Z_Construct_UClass_UCurveFloat_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_Gx_MetaData), Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_Gx_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_Gy_MetaData[] = {
		{ "Category", "VehicleTireConfig" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Input: absolute value of slip ratio; Output: scaling factor for Fy; Through this curve, the distribution of lateral forces can be manually adjusted. If no settings are made, the distribution of lateral forces will follow the classic friction ellipse." },
#endif
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_Gy = { "Gy", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleTireConfig, Gy), Z_Construct_UClass_UCurveFloat_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_Gy_MetaData), Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_Gy_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_FrictionMultiplier,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_TireFrictionCombineMode_Underlying,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_TireFrictionCombineMode,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_RelaxationLength,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_MaxFx,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_MaxFy,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_Fx,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_Fy,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_Gx,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewProp_Gy,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		&NewStructOps,
		"VehicleTireConfig",
		Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::PropPointers),
		sizeof(FVehicleTireConfig),
		alignof(FVehicleTireConfig),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FVehicleTireConfig()
	{
		if (!Z_Registration_Info_UScriptStruct_VehicleTireConfig.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_VehicleTireConfig.InnerSingleton, Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_VehicleTireConfig.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_VehicleABSConfig;
class UScriptStruct* FVehicleABSConfig::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_VehicleABSConfig.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_VehicleABSConfig.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FVehicleABSConfig, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("VehicleABSConfig"));
	}
	return Z_Registration_Info_UScriptStruct_VehicleABSConfig.OuterSingleton;
}
template<> KINETIFORGE_API UScriptStruct* StaticStruct<FVehicleABSConfig>()
{
	return FVehicleABSConfig::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FVehicleABSConfig_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bAntiBrakeSystemEnabled_MetaData[];
#endif
		static void NewProp_bAntiBrakeSystemEnabled_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bAntiBrakeSystemEnabled;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MaxSlipRatio_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_MaxSlipRatio;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ActivationSpeed_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ActivationSpeed;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_InterpSpeed_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InterpSpeed;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FVehicleABSConfig>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::NewProp_bAntiBrakeSystemEnabled_MetaData[] = {
		{ "Category", "VehicleABSConfig" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::NewProp_bAntiBrakeSystemEnabled_SetBit(void* Obj)
	{
		((FVehicleABSConfig*)Obj)->bAntiBrakeSystemEnabled = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::NewProp_bAntiBrakeSystemEnabled = { "bAntiBrakeSystemEnabled", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FVehicleABSConfig), &Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::NewProp_bAntiBrakeSystemEnabled_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::NewProp_bAntiBrakeSystemEnabled_MetaData), Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::NewProp_bAntiBrakeSystemEnabled_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::NewProp_MaxSlipRatio_MetaData[] = {
		{ "Category", "VehicleABSConfig" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::NewProp_MaxSlipRatio = { "MaxSlipRatio", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleABSConfig, MaxSlipRatio), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::NewProp_MaxSlipRatio_MetaData), Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::NewProp_MaxSlipRatio_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::NewProp_ActivationSpeed_MetaData[] = {
		{ "Category", "VehicleABSConfig" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Meter per second, m/s" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::NewProp_ActivationSpeed = { "ActivationSpeed", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleABSConfig, ActivationSpeed), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::NewProp_ActivationSpeed_MetaData), Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::NewProp_ActivationSpeed_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::NewProp_InterpSpeed_MetaData[] = {
		{ "Category", "VehicleABSConfig" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::NewProp_InterpSpeed = { "InterpSpeed", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleABSConfig, InterpSpeed), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::NewProp_InterpSpeed_MetaData), Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::NewProp_InterpSpeed_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::NewProp_bAntiBrakeSystemEnabled,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::NewProp_MaxSlipRatio,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::NewProp_ActivationSpeed,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::NewProp_InterpSpeed,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		&NewStructOps,
		"VehicleABSConfig",
		Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::PropPointers),
		sizeof(FVehicleABSConfig),
		alignof(FVehicleABSConfig),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FVehicleABSConfig()
	{
		if (!Z_Registration_Info_UScriptStruct_VehicleABSConfig.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_VehicleABSConfig.InnerSingleton, Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_VehicleABSConfig.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_VehicleWheelSimData;
class UScriptStruct* FVehicleWheelSimData::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_VehicleWheelSimData.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_VehicleWheelSimData.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FVehicleWheelSimData, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("VehicleWheelSimData"));
	}
	return Z_Registration_Info_UScriptStruct_VehicleWheelSimData.OuterSingleton;
}
template<> KINETIFORGE_API UScriptStruct* StaticStruct<FVehicleWheelSimData>()
{
	return FVehicleWheelSimData::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_R_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_R;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RInv_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_RInv;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ReflectedInertia_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ReflectedInertia;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TotalInertia_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_TotalInertia;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TotalInertiaInv_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_TotalInertiaInv;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AngularVelocity_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_AngularVelocity;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_LongSlipVelocity_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_LongSlipVelocity;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SlipRatio_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_SlipRatio;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SlipAngle_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_SlipAngle;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_DynFrictionMultiplier_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_DynFrictionMultiplier;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bIsLocked_MetaData[];
#endif
		static void NewProp_bIsLocked_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bIsLocked;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bABSTriggered_MetaData[];
#endif
		static void NewProp_bABSTriggered_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bABSTriggered;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_WorldLinearVelocity_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_WorldLinearVelocity;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_LocalLinearVelocity_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_LocalLinearVelocity;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_P4MotorTorque_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_P4MotorTorque;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_DriveTorque_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_DriveTorque;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BrakeTorque_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_BrakeTorque;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BrakeTorqueFromBrake_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_BrakeTorqueFromBrake;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BrakeTorqueFromHandbrake_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_BrakeTorqueFromHandbrake;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TorqueFromGroundInteraction_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_TorqueFromGroundInteraction;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_LongForce_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_LongForce;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_LatForce_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_LatForce;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TyreForce_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_TyreForce;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_PhysicsDeltaTime_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_PhysicsDeltaTime;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_PhysicsDeltaTimeInv_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_PhysicsDeltaTimeInv;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FVehicleWheelSimData>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_R_MetaData[] = {
		{ "Category", "Movement" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//WheelMovement\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "WheelMovement" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_R = { "R", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelSimData, R), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_R_MetaData), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_R_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_RInv_MetaData[] = {
		{ "Category", "Movement" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Radius in meter\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Radius in meter" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_RInv = { "RInv", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelSimData, RInv), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_RInv_MetaData), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_RInv_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_ReflectedInertia_MetaData[] = {
		{ "Category", "Movement" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//1/R\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "1/R" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_ReflectedInertia = { "ReflectedInertia", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelSimData, ReflectedInertia), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_ReflectedInertia_MetaData), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_ReflectedInertia_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_TotalInertia_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_TotalInertia = { "TotalInertia", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelSimData, TotalInertia), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_TotalInertia_MetaData), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_TotalInertia_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_TotalInertiaInv_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_TotalInertiaInv = { "TotalInertiaInv", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelSimData, TotalInertiaInv), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_TotalInertiaInv_MetaData), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_TotalInertiaInv_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_AngularVelocity_MetaData[] = {
		{ "Category", "Movement" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//1/TotalInertia\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "1/TotalInertia" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_AngularVelocity = { "AngularVelocity", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelSimData, AngularVelocity), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_AngularVelocity_MetaData), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_AngularVelocity_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_LongSlipVelocity_MetaData[] = {
		{ "Category", "Slip" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_LongSlipVelocity = { "LongSlipVelocity", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelSimData, LongSlipVelocity), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_LongSlipVelocity_MetaData), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_LongSlipVelocity_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_SlipRatio_MetaData[] = {
		{ "Category", "Slip" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_SlipRatio = { "SlipRatio", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelSimData, SlipRatio), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_SlipRatio_MetaData), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_SlipRatio_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_SlipAngle_MetaData[] = {
		{ "Category", "Slip" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_SlipAngle = { "SlipAngle", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelSimData, SlipAngle), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_SlipAngle_MetaData), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_SlipAngle_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_DynFrictionMultiplier_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_DynFrictionMultiplier = { "DynFrictionMultiplier", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelSimData, DynFrictionMultiplier), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_DynFrictionMultiplier_MetaData), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_DynFrictionMultiplier_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_bIsLocked_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_bIsLocked_SetBit(void* Obj)
	{
		((FVehicleWheelSimData*)Obj)->bIsLocked = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_bIsLocked = { "bIsLocked", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FVehicleWheelSimData), &Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_bIsLocked_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_bIsLocked_MetaData), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_bIsLocked_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_bABSTriggered_MetaData[] = {
		{ "Category", "AntiBrake" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_bABSTriggered_SetBit(void* Obj)
	{
		((FVehicleWheelSimData*)Obj)->bABSTriggered = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_bABSTriggered = { "bABSTriggered", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FVehicleWheelSimData), &Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_bABSTriggered_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_bABSTriggered_MetaData), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_bABSTriggered_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_WorldLinearVelocity_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_WorldLinearVelocity = { "WorldLinearVelocity", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelSimData, WorldLinearVelocity), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_WorldLinearVelocity_MetaData), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_WorldLinearVelocity_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_LocalLinearVelocity_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_LocalLinearVelocity = { "LocalLinearVelocity", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelSimData, LocalLinearVelocity), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_LocalLinearVelocity_MetaData), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_LocalLinearVelocity_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_P4MotorTorque_MetaData[] = {
		{ "Category", "Force" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Force\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Force" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_P4MotorTorque = { "P4MotorTorque", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelSimData, P4MotorTorque), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_P4MotorTorque_MetaData), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_P4MotorTorque_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_DriveTorque_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_DriveTorque = { "DriveTorque", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelSimData, DriveTorque), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_DriveTorque_MetaData), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_DriveTorque_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_BrakeTorque_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_BrakeTorque = { "BrakeTorque", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelSimData, BrakeTorque), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_BrakeTorque_MetaData), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_BrakeTorque_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_BrakeTorqueFromBrake_MetaData[] = {
		{ "Category", "Force" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//braketorque + rolling resistance\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "braketorque + rolling resistance" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_BrakeTorqueFromBrake = { "BrakeTorqueFromBrake", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelSimData, BrakeTorqueFromBrake), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_BrakeTorqueFromBrake_MetaData), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_BrakeTorqueFromBrake_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_BrakeTorqueFromHandbrake_MetaData[] = {
		{ "Category", "Force" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//not including the rolling resistance\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "not including the rolling resistance" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_BrakeTorqueFromHandbrake = { "BrakeTorqueFromHandbrake", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelSimData, BrakeTorqueFromHandbrake), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_BrakeTorqueFromHandbrake_MetaData), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_BrakeTorqueFromHandbrake_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_TorqueFromGroundInteraction_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_TorqueFromGroundInteraction = { "TorqueFromGroundInteraction", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelSimData, TorqueFromGroundInteraction), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_TorqueFromGroundInteraction_MetaData), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_TorqueFromGroundInteraction_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_LongForce_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_LongForce = { "LongForce", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelSimData, LongForce), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_LongForce_MetaData), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_LongForce_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_LatForce_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_LatForce = { "LatForce", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelSimData, LatForce), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_LatForce_MetaData), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_LatForce_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_TyreForce_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_TyreForce = { "TyreForce", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelSimData, TyreForce), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_TyreForce_MetaData), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_TyreForce_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_PhysicsDeltaTime_MetaData[] = {
		{ "Category", "DeltaTime" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Physics\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Physics" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_PhysicsDeltaTime = { "PhysicsDeltaTime", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelSimData, PhysicsDeltaTime), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_PhysicsDeltaTime_MetaData), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_PhysicsDeltaTime_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_PhysicsDeltaTimeInv_MetaData[] = {
		{ "Category", "DeltaTime" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_PhysicsDeltaTimeInv = { "PhysicsDeltaTimeInv", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleWheelSimData, PhysicsDeltaTimeInv), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_PhysicsDeltaTimeInv_MetaData), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_PhysicsDeltaTimeInv_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_R,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_RInv,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_ReflectedInertia,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_TotalInertia,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_TotalInertiaInv,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_AngularVelocity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_LongSlipVelocity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_SlipRatio,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_SlipAngle,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_DynFrictionMultiplier,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_bIsLocked,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_bABSTriggered,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_WorldLinearVelocity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_LocalLinearVelocity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_P4MotorTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_DriveTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_BrakeTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_BrakeTorqueFromBrake,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_BrakeTorqueFromHandbrake,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_TorqueFromGroundInteraction,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_LongForce,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_LatForce,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_TyreForce,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_PhysicsDeltaTime,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewProp_PhysicsDeltaTimeInv,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		&NewStructOps,
		"VehicleWheelSimData",
		Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::PropPointers),
		sizeof(FVehicleWheelSimData),
		alignof(FVehicleWheelSimData),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FVehicleWheelSimData()
	{
		if (!Z_Registration_Info_UScriptStruct_VehicleWheelSimData.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_VehicleWheelSimData.InnerSingleton, Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_VehicleWheelSimData.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_VehicleSuspensionKinematicsConfig;
class UScriptStruct* FVehicleSuspensionKinematicsConfig::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_VehicleSuspensionKinematicsConfig.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_VehicleSuspensionKinematicsConfig.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("VehicleSuspensionKinematicsConfig"));
	}
	return Z_Registration_Info_UScriptStruct_VehicleSuspensionKinematicsConfig.OuterSingleton;
}
template<> KINETIFORGE_API UScriptStruct* StaticStruct<FVehicleSuspensionKinematicsConfig>()
{
	return FVehicleSuspensionKinematicsConfig::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
		static const UECodeGen_Private::FBytePropertyParams NewProp_SuspensionType_Underlying;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SuspensionType_MetaData[];
#endif
		static const UECodeGen_Private::FEnumPropertyParams NewProp_SuspensionType;
		static const UECodeGen_Private::FBytePropertyParams NewProp_RayCastMode_Underlying;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RayCastMode_MetaData[];
#endif
		static const UECodeGen_Private::FEnumPropertyParams NewProp_RayCastMode;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TraceChannel_MetaData[];
#endif
		static const UECodeGen_Private::FBytePropertyParams NewProp_TraceChannel;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TopMountPosition_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_TopMountPosition;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Stroke_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_Stroke;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MinStrutLength_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_MinStrutLength;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ArmLength_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ArmLength;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SteeringAxleOffset_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_SteeringAxleOffset;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BaseCamber_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_BaseCamber;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BaseToe_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_BaseToe;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_CamberCurve_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_CamberCurve;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_CasterCurve_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_CasterCurve;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ToeCurve_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_ToeCurve;
		static const UECodeGen_Private::FBytePropertyParams NewProp_PositionToApplyForce_Underlying;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_PositionToApplyForce_MetaData[];
#endif
		static const UECodeGen_Private::FEnumPropertyParams NewProp_PositionToApplyForce;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FVehicleSuspensionKinematicsConfig>();
	}
	const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_SuspensionType_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_SuspensionType_MetaData[] = {
		{ "Category", "VehicleSuspensionKinematicsConfig" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//SuspensionSetup\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "SuspensionSetup" },
#endif
	};
#endif
	const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_SuspensionType = { "SuspensionType", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionKinematicsConfig, SuspensionType), Z_Construct_UEnum_KinetiForge_ESuspensionType, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_SuspensionType_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_SuspensionType_MetaData) }; // 3408812646
	const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_RayCastMode_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_RayCastMode_MetaData[] = {
		{ "Category", "VehicleSuspensionKinematicsConfig" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_RayCastMode = { "RayCastMode", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionKinematicsConfig, RayCastMode), Z_Construct_UEnum_KinetiForge_ESuspensionRayCastMode, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_RayCastMode_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_RayCastMode_MetaData) }; // 1888833822
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_TraceChannel_MetaData[] = {
		{ "Category", "VehicleSuspensionKinematicsConfig" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_TraceChannel = { "TraceChannel", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionKinematicsConfig, TraceChannel), Z_Construct_UEnum_Engine_ECollisionChannel, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_TraceChannel_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_TraceChannel_MetaData) }; // 1822723181
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_TopMountPosition_MetaData[] = {
		{ "Category", "VehicleSuspensionKinematicsConfig" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "X: Horizontal; Y:Vertical" },
#endif
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_TopMountPosition = { "TopMountPosition", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionKinematicsConfig, TopMountPosition), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_TopMountPosition_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_TopMountPosition_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_Stroke_MetaData[] = {
		{ "Category", "VehicleSuspensionKinematicsConfig" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "cm" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_Stroke = { "Stroke", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionKinematicsConfig, Stroke), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_Stroke_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_Stroke_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_MinStrutLength_MetaData[] = {
		{ "Category", "VehicleSuspensionKinematicsConfig" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "cm" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_MinStrutLength = { "MinStrutLength", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionKinematicsConfig, MinStrutLength), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_MinStrutLength_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_MinStrutLength_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_ArmLength_MetaData[] = {
		{ "Category", "VehicleSuspensionKinematicsConfig" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "cm" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_ArmLength = { "ArmLength", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionKinematicsConfig, ArmLength), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_ArmLength_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_ArmLength_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_SteeringAxleOffset_MetaData[] = {
		{ "Category", "VehicleSuspensionKinematicsConfig" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_SteeringAxleOffset = { "SteeringAxleOffset", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionKinematicsConfig, SteeringAxleOffset), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_SteeringAxleOffset_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_SteeringAxleOffset_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_BaseCamber_MetaData[] = {
		{ "Category", "VehicleSuspensionKinematicsConfig" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_BaseCamber = { "BaseCamber", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionKinematicsConfig, BaseCamber), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_BaseCamber_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_BaseCamber_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_BaseToe_MetaData[] = {
		{ "Category", "VehicleSuspensionKinematicsConfig" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_BaseToe = { "BaseToe", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionKinematicsConfig, BaseToe), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_BaseToe_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_BaseToe_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_CamberCurve_MetaData[] = {
		{ "Category", "VehicleSuspensionKinematicsConfig" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "X: SuspensionCompressionRatio; Y:CamberGain" },
#endif
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_CamberCurve = { "CamberCurve", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionKinematicsConfig, CamberCurve), Z_Construct_UClass_UCurveFloat_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_CamberCurve_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_CamberCurve_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_CasterCurve_MetaData[] = {
		{ "Category", "VehicleSuspensionKinematicsConfig" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "X: SuspensionCompressionRatio; Y:CasterGain" },
#endif
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_CasterCurve = { "CasterCurve", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionKinematicsConfig, CasterCurve), Z_Construct_UClass_UCurveFloat_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_CasterCurve_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_CasterCurve_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_ToeCurve_MetaData[] = {
		{ "Category", "VehicleSuspensionKinematicsConfig" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "X: SuspensionCompressionRatio; Y:ToeGain" },
#endif
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_ToeCurve = { "ToeCurve", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionKinematicsConfig, ToeCurve), Z_Construct_UClass_UCurveFloat_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_ToeCurve_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_ToeCurve_MetaData) };
	const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_PositionToApplyForce_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_PositionToApplyForce_MetaData[] = {
		{ "Category", "VehicleSuspensionKinematicsConfig" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_PositionToApplyForce = { "PositionToApplyForce", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionKinematicsConfig, PositionToApplyForce), Z_Construct_UEnum_KinetiForge_EPositionToApplyForce, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_PositionToApplyForce_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_PositionToApplyForce_MetaData) }; // 1459486099
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_SuspensionType_Underlying,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_SuspensionType,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_RayCastMode_Underlying,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_RayCastMode,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_TraceChannel,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_TopMountPosition,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_Stroke,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_MinStrutLength,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_ArmLength,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_SteeringAxleOffset,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_BaseCamber,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_BaseToe,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_CamberCurve,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_CasterCurve,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_ToeCurve,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_PositionToApplyForce_Underlying,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewProp_PositionToApplyForce,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		&NewStructOps,
		"VehicleSuspensionKinematicsConfig",
		Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::PropPointers),
		sizeof(FVehicleSuspensionKinematicsConfig),
		alignof(FVehicleSuspensionKinematicsConfig),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig()
	{
		if (!Z_Registration_Info_UScriptStruct_VehicleSuspensionKinematicsConfig.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_VehicleSuspensionKinematicsConfig.InnerSingleton, Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_VehicleSuspensionKinematicsConfig.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_VehicleSuspensionSpringConfig;
class UScriptStruct* FVehicleSuspensionSpringConfig::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_VehicleSuspensionSpringConfig.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_VehicleSuspensionSpringConfig.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("VehicleSuspensionSpringConfig"));
	}
	return Z_Registration_Info_UScriptStruct_VehicleSuspensionSpringConfig.OuterSingleton;
}
template<> KINETIFORGE_API UScriptStruct* StaticStruct<FVehicleSuspensionSpringConfig>()
{
	return FVehicleSuspensionSpringConfig::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SpringStiffness_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_SpringStiffness;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_CompressionDampingRatio_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_CompressionDampingRatio;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ReboundDampingRatio_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ReboundDampingRatio;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SpringPreload_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_SpringPreload;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BottomOutStiffness_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_BottomOutStiffness;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FVehicleSuspensionSpringConfig>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewProp_SpringStiffness_MetaData[] = {
		{ "Category", "VehicleSuspensionSpringConfig" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewProp_SpringStiffness = { "SpringStiffness", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSpringConfig, SpringStiffness), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewProp_SpringStiffness_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewProp_SpringStiffness_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewProp_CompressionDampingRatio_MetaData[] = {
		{ "Category", "VehicleSuspensionSpringConfig" },
		{ "ClampMax", "1.0" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
		{ "UIMax", "1.0" },
		{ "UIMin", "0.0" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewProp_CompressionDampingRatio = { "CompressionDampingRatio", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSpringConfig, CompressionDampingRatio), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewProp_CompressionDampingRatio_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewProp_CompressionDampingRatio_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewProp_ReboundDampingRatio_MetaData[] = {
		{ "Category", "VehicleSuspensionSpringConfig" },
		{ "ClampMax", "1.0" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
		{ "UIMax", "1.0" },
		{ "UIMin", "0.0" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewProp_ReboundDampingRatio = { "ReboundDampingRatio", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSpringConfig, ReboundDampingRatio), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewProp_ReboundDampingRatio_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewProp_ReboundDampingRatio_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewProp_SpringPreload_MetaData[] = {
		{ "Category", "VehicleSuspensionSpringConfig" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewProp_SpringPreload = { "SpringPreload", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSpringConfig, SpringPreload), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewProp_SpringPreload_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewProp_SpringPreload_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewProp_BottomOutStiffness_MetaData[] = {
		{ "Category", "VehicleSuspensionSpringConfig" },
		{ "ClampMax", "1.0" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Scales the stiffness of the auxiliary spring to prevent ground penetration at full suspension compression." },
#endif
		{ "UIMax", "1.0" },
		{ "UIMin", "0.0" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewProp_BottomOutStiffness = { "BottomOutStiffness", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSpringConfig, BottomOutStiffness), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewProp_BottomOutStiffness_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewProp_BottomOutStiffness_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewProp_SpringStiffness,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewProp_CompressionDampingRatio,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewProp_ReboundDampingRatio,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewProp_SpringPreload,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewProp_BottomOutStiffness,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		&NewStructOps,
		"VehicleSuspensionSpringConfig",
		Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::PropPointers),
		sizeof(FVehicleSuspensionSpringConfig),
		alignof(FVehicleSuspensionSpringConfig),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig()
	{
		if (!Z_Registration_Info_UScriptStruct_VehicleSuspensionSpringConfig.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_VehicleSuspensionSpringConfig.InnerSingleton, Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_VehicleSuspensionSpringConfig.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_VehicleSuspensionSimData;
class UScriptStruct* FVehicleSuspensionSimData::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_VehicleSuspensionSimData.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_VehicleSuspensionSimData.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FVehicleSuspensionSimData, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("VehicleSuspensionSimData"));
	}
	return Z_Registration_Info_UScriptStruct_VehicleSuspensionSimData.OuterSingleton;
}
template<> KINETIFORGE_API UScriptStruct* StaticStruct<FVehicleSuspensionSimData>()
{
	return FVehicleSuspensionSimData::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_WheelPos_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_WheelPos;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bHitGround_MetaData[];
#endif
		static void NewProp_bHitGround_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bHitGround;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bRayCastRevised_MetaData[];
#endif
		static void NewProp_bRayCastRevised_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bRayCastRevised;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_PhysicsDelatTime_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_PhysicsDelatTime;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RayCastLength_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_RayCastLength;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_HitDistance_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_HitDistance;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SuspensionCurrentLength_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_SuspensionCurrentLength;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SuspensionExtensionRatio_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_SuspensionExtensionRatio;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SteeringAngle_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_SteeringAngle;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_CriticalDamping_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_CriticalDamping;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ValidPreload_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ValidPreload;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SwaybarForce_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_SwaybarForce;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SuspensionForceRaw_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_SuspensionForceRaw;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SuspensionForce_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_SuspensionForce;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_PositiveSuspensionForce_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_PositiveSuspensionForce;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SprungMass_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_SprungMass;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_WorldGravityZ_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_WorldGravityZ;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TopMountPos2D_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_TopMountPos2D;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BallJointPos2D_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_BallJointPos2D;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RayCastStart2D_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_RayCastStart2D;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_StrutDirection2D_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_StrutDirection2D;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ComponentUpVector_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_ComponentUpVector;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ComponentRelativeForwardVector_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_ComponentRelativeForwardVector;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_WheelRightVector_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_WheelRightVector;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_WheelOffsetToBallJoint_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_WheelOffsetToBallJoint;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BallJointRelativePos_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_BallJointRelativePos;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TopMountRelativePos_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_TopMountRelativePos;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RayCastStartPos_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_RayCastStartPos;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RayCastEndPos_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_RayCastEndPos;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_StrutRelativeDirection_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_StrutRelativeDirection;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_StrutDirection_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_StrutDirection;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SuspensionForceVecRaw_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_SuspensionForceVecRaw;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SuspensionForceVector_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_SuspensionForceVector;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ImpactPointToRayCastStart_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_ImpactPointToRayCastStart;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_WheelWorldPos_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_WheelWorldPos;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RayCastTransform_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_RayCastTransform;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_CarbodyWorldTransform_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_CarbodyWorldTransform;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RelativeTransform_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_RelativeTransform;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_WheelRelativeTransform_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_WheelRelativeTransform;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_HitStruct_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_HitStruct;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FVehicleSuspensionSimData>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WheelPos_MetaData[] = {
		{ "Category", "Position" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WheelPos = { "WheelPos", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, WheelPos), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WheelPos_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WheelPos_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_bHitGround_MetaData[] = {
		{ "Category", "RayCast" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_bHitGround_SetBit(void* Obj)
	{
		((FVehicleSuspensionSimData*)Obj)->bHitGround = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_bHitGround = { "bHitGround", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FVehicleSuspensionSimData), &Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_bHitGround_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_bHitGround_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_bHitGround_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_bRayCastRevised_MetaData[] = {
		{ "Category", "RayCast" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_bRayCastRevised_SetBit(void* Obj)
	{
		((FVehicleSuspensionSimData*)Obj)->bRayCastRevised = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_bRayCastRevised = { "bRayCastRevised", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FVehicleSuspensionSimData), &Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_bRayCastRevised_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_bRayCastRevised_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_bRayCastRevised_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_PhysicsDelatTime_MetaData[] = {
		{ "Category", "DeltaTime" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_PhysicsDelatTime = { "PhysicsDelatTime", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, PhysicsDelatTime), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_PhysicsDelatTime_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_PhysicsDelatTime_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RayCastLength_MetaData[] = {
		{ "Category", "RayCast" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RayCastLength = { "RayCastLength", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, RayCastLength), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RayCastLength_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RayCastLength_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_HitDistance_MetaData[] = {
		{ "Category", "RayCast" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_HitDistance = { "HitDistance", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, HitDistance), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_HitDistance_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_HitDistance_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionCurrentLength_MetaData[] = {
		{ "Category", "Geometry" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionCurrentLength = { "SuspensionCurrentLength", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, SuspensionCurrentLength), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionCurrentLength_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionCurrentLength_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionExtensionRatio_MetaData[] = {
		{ "Category", "Geometry" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionExtensionRatio = { "SuspensionExtensionRatio", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, SuspensionExtensionRatio), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionExtensionRatio_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionExtensionRatio_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SteeringAngle_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SteeringAngle = { "SteeringAngle", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, SteeringAngle), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SteeringAngle_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SteeringAngle_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_CriticalDamping_MetaData[] = {
		{ "Category", "Config" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_CriticalDamping = { "CriticalDamping", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, CriticalDamping), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_CriticalDamping_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_CriticalDamping_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_ValidPreload_MetaData[] = {
		{ "Category", "Config" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_ValidPreload = { "ValidPreload", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, ValidPreload), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_ValidPreload_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_ValidPreload_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SwaybarForce_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SwaybarForce = { "SwaybarForce", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, SwaybarForce), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SwaybarForce_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SwaybarForce_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionForceRaw_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionForceRaw = { "SuspensionForceRaw", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, SuspensionForceRaw), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionForceRaw_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionForceRaw_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionForce_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionForce = { "SuspensionForce", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, SuspensionForce), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionForce_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionForce_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_PositiveSuspensionForce_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_PositiveSuspensionForce = { "PositiveSuspensionForce", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, PositiveSuspensionForce), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_PositiveSuspensionForce_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_PositiveSuspensionForce_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SprungMass_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SprungMass = { "SprungMass", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, SprungMass), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SprungMass_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SprungMass_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WorldGravityZ_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WorldGravityZ = { "WorldGravityZ", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, WorldGravityZ), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WorldGravityZ_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WorldGravityZ_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_TopMountPos2D_MetaData[] = {
		{ "Category", "Geometry" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_TopMountPos2D = { "TopMountPos2D", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, TopMountPos2D), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_TopMountPos2D_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_TopMountPos2D_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_BallJointPos2D_MetaData[] = {
		{ "Category", "Geometry" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_BallJointPos2D = { "BallJointPos2D", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, BallJointPos2D), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_BallJointPos2D_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_BallJointPos2D_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RayCastStart2D_MetaData[] = {
		{ "Category", "RayCast" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RayCastStart2D = { "RayCastStart2D", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, RayCastStart2D), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RayCastStart2D_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RayCastStart2D_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_StrutDirection2D_MetaData[] = {
		{ "Category", "Geometry" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_StrutDirection2D = { "StrutDirection2D", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, StrutDirection2D), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_StrutDirection2D_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_StrutDirection2D_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_ComponentUpVector_MetaData[] = {
		{ "Category", "Geometry" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_ComponentUpVector = { "ComponentUpVector", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, ComponentUpVector), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_ComponentUpVector_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_ComponentUpVector_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_ComponentRelativeForwardVector_MetaData[] = {
		{ "Category", "Geometry" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//world\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "world" },
#endif
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_ComponentRelativeForwardVector = { "ComponentRelativeForwardVector", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, ComponentRelativeForwardVector), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_ComponentRelativeForwardVector_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_ComponentRelativeForwardVector_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WheelRightVector_MetaData[] = {
		{ "Category", "Geometry" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//relative\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "relative" },
#endif
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WheelRightVector = { "WheelRightVector", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, WheelRightVector), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WheelRightVector_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WheelRightVector_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WheelOffsetToBallJoint_MetaData[] = {
		{ "Category", "Geometry" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WheelOffsetToBallJoint = { "WheelOffsetToBallJoint", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, WheelOffsetToBallJoint), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WheelOffsetToBallJoint_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WheelOffsetToBallJoint_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_BallJointRelativePos_MetaData[] = {
		{ "Category", "Geometry" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//relative\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "relative" },
#endif
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_BallJointRelativePos = { "BallJointRelativePos", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, BallJointRelativePos), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_BallJointRelativePos_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_BallJointRelativePos_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_TopMountRelativePos_MetaData[] = {
		{ "Category", "Geometry" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_TopMountRelativePos = { "TopMountRelativePos", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, TopMountRelativePos), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_TopMountRelativePos_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_TopMountRelativePos_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RayCastStartPos_MetaData[] = {
		{ "Category", "RayCast" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RayCastStartPos = { "RayCastStartPos", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, RayCastStartPos), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RayCastStartPos_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RayCastStartPos_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RayCastEndPos_MetaData[] = {
		{ "Category", "RayCast" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//for sphere trace\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "for sphere trace" },
#endif
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RayCastEndPos = { "RayCastEndPos", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, RayCastEndPos), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RayCastEndPos_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RayCastEndPos_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_StrutRelativeDirection_MetaData[] = {
		{ "Category", "Geometry" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//for sphere trace\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "for sphere trace" },
#endif
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_StrutRelativeDirection = { "StrutRelativeDirection", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, StrutRelativeDirection), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_StrutRelativeDirection_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_StrutRelativeDirection_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_StrutDirection_MetaData[] = {
		{ "Category", "Geometry" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_StrutDirection = { "StrutDirection", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, StrutDirection), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_StrutDirection_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_StrutDirection_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionForceVecRaw_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionForceVecRaw = { "SuspensionForceVecRaw", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, SuspensionForceVecRaw), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionForceVecRaw_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionForceVecRaw_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionForceVector_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionForceVector = { "SuspensionForceVector", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, SuspensionForceVector), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionForceVector_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionForceVector_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_ImpactPointToRayCastStart_MetaData[] = {
		{ "Category", "RayCast" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_ImpactPointToRayCastStart = { "ImpactPointToRayCastStart", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, ImpactPointToRayCastStart), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_ImpactPointToRayCastStart_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_ImpactPointToRayCastStart_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WheelWorldPos_MetaData[] = {
		{ "Category", "Geometry" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WheelWorldPos = { "WheelWorldPos", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, WheelWorldPos), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WheelWorldPos_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WheelWorldPos_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RayCastTransform_MetaData[] = {
		{ "Category", "RayCast" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RayCastTransform = { "RayCastTransform", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, RayCastTransform), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RayCastTransform_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RayCastTransform_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_CarbodyWorldTransform_MetaData[] = {
		{ "Category", "Geometry" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_CarbodyWorldTransform = { "CarbodyWorldTransform", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, CarbodyWorldTransform), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_CarbodyWorldTransform_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_CarbodyWorldTransform_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RelativeTransform_MetaData[] = {
		{ "Category", "Geometry" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RelativeTransform = { "RelativeTransform", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, RelativeTransform), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RelativeTransform_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RelativeTransform_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WheelRelativeTransform_MetaData[] = {
		{ "Category", "Geometry" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WheelRelativeTransform = { "WheelRelativeTransform", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, WheelRelativeTransform), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WheelRelativeTransform_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WheelRelativeTransform_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_HitStruct_MetaData[] = {
		{ "Category", "RayCast" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//relative to carbody\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "relative to carbody" },
#endif
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_HitStruct = { "HitStruct", nullptr, (EPropertyFlags)0x0010008000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSuspensionSimData, HitStruct), Z_Construct_UScriptStruct_FHitResult, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_HitStruct_MetaData), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_HitStruct_MetaData) }; // 1891709922
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WheelPos,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_bHitGround,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_bRayCastRevised,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_PhysicsDelatTime,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RayCastLength,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_HitDistance,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionCurrentLength,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionExtensionRatio,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SteeringAngle,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_CriticalDamping,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_ValidPreload,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SwaybarForce,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionForceRaw,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionForce,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_PositiveSuspensionForce,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SprungMass,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WorldGravityZ,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_TopMountPos2D,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_BallJointPos2D,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RayCastStart2D,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_StrutDirection2D,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_ComponentUpVector,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_ComponentRelativeForwardVector,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WheelRightVector,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WheelOffsetToBallJoint,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_BallJointRelativePos,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_TopMountRelativePos,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RayCastStartPos,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RayCastEndPos,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_StrutRelativeDirection,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_StrutDirection,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionForceVecRaw,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_SuspensionForceVector,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_ImpactPointToRayCastStart,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WheelWorldPos,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RayCastTransform,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_CarbodyWorldTransform,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_RelativeTransform,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_WheelRelativeTransform,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewProp_HitStruct,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		&NewStructOps,
		"VehicleSuspensionSimData",
		Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::PropPointers),
		sizeof(FVehicleSuspensionSimData),
		alignof(FVehicleSuspensionSimData),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000005),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FVehicleSuspensionSimData()
	{
		if (!Z_Registration_Info_UScriptStruct_VehicleSuspensionSimData.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_VehicleSuspensionSimData.InnerSingleton, Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_VehicleSuspensionSimData.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_VehicleAxleConfig;
class UScriptStruct* FVehicleAxleConfig::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_VehicleAxleConfig.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_VehicleAxleConfig.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FVehicleAxleConfig, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("VehicleAxleConfig"));
	}
	return Z_Registration_Info_UScriptStruct_VehicleAxleConfig.OuterSingleton;
}
template<> KINETIFORGE_API UScriptStruct* StaticStruct<FVehicleAxleConfig>()
{
	return FVehicleAxleConfig::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_DriveShaftInertia_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_DriveShaftInertia;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TrackWidth_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_TrackWidth;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TorqueWeight_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_TorqueWeight;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SwaybarStiffness_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_SwaybarStiffness;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MaxBrakeTorque_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_MaxBrakeTorque;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bAffectedByHandbrake_MetaData[];
#endif
		static void NewProp_bAffectedByHandbrake_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bAffectedByHandbrake;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MaxHandbrakeTorque_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_MaxHandbrakeTorque;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FVehicleAxleConfig>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_DriveShaftInertia_MetaData[] = {
		{ "Category", "AxleSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_DriveShaftInertia = { "DriveShaftInertia", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleConfig, DriveShaftInertia), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_DriveShaftInertia_MetaData), Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_DriveShaftInertia_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_TrackWidth_MetaData[] = {
		{ "Category", "AxleSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_TrackWidth = { "TrackWidth", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleConfig, TrackWidth), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_TrackWidth_MetaData), Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_TrackWidth_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_TorqueWeight_MetaData[] = {
		{ "Category", "AxleSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Raw weight before normalization, can be negative. Used to assign torque proportionally." },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_TorqueWeight = { "TorqueWeight", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleConfig, TorqueWeight), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_TorqueWeight_MetaData), Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_TorqueWeight_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_SwaybarStiffness_MetaData[] = {
		{ "Category", "AxleSetup" },
		{ "ClampMin", "0.0" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// Raw weight before normalization, can be negative. Used to assign torque proportionally.\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Raw weight before normalization, can be negative. Used to assign torque proportionally." },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_SwaybarStiffness = { "SwaybarStiffness", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleConfig, SwaybarStiffness), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_SwaybarStiffness_MetaData), Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_SwaybarStiffness_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_MaxBrakeTorque_MetaData[] = {
		{ "Category", "AxleSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_MaxBrakeTorque = { "MaxBrakeTorque", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleConfig, MaxBrakeTorque), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_MaxBrakeTorque_MetaData), Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_MaxBrakeTorque_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_bAffectedByHandbrake_MetaData[] = {
		{ "Category", "AxleSetup" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_bAffectedByHandbrake_SetBit(void* Obj)
	{
		((FVehicleAxleConfig*)Obj)->bAffectedByHandbrake = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_bAffectedByHandbrake = { "bAffectedByHandbrake", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FVehicleAxleConfig), &Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_bAffectedByHandbrake_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_bAffectedByHandbrake_MetaData), Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_bAffectedByHandbrake_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_MaxHandbrakeTorque_MetaData[] = {
		{ "Category", "AxleSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_MaxHandbrakeTorque = { "MaxHandbrakeTorque", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleConfig, MaxHandbrakeTorque), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_MaxHandbrakeTorque_MetaData), Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_MaxHandbrakeTorque_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_DriveShaftInertia,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_TrackWidth,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_TorqueWeight,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_SwaybarStiffness,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_MaxBrakeTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_bAffectedByHandbrake,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewProp_MaxHandbrakeTorque,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		&NewStructOps,
		"VehicleAxleConfig",
		Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::PropPointers),
		sizeof(FVehicleAxleConfig),
		alignof(FVehicleAxleConfig),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FVehicleAxleConfig()
	{
		if (!Z_Registration_Info_UScriptStruct_VehicleAxleConfig.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_VehicleAxleConfig.InnerSingleton, Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_VehicleAxleConfig.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_VehicleAxleSteeringConfig;
class UScriptStruct* FVehicleAxleSteeringConfig::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_VehicleAxleSteeringConfig.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_VehicleAxleSteeringConfig.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("VehicleAxleSteeringConfig"));
	}
	return Z_Registration_Info_UScriptStruct_VehicleAxleSteeringConfig.OuterSingleton;
}
template<> KINETIFORGE_API UScriptStruct* StaticStruct<FVehicleAxleSteeringConfig>()
{
	return FVehicleAxleSteeringConfig::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bAffectedBySteering_MetaData[];
#endif
		static void NewProp_bAffectedBySteering_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bAffectedBySteering;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MaxSteeringAngle_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_MaxSteeringAngle;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bAckermannSteering_MetaData[];
#endif
		static void NewProp_bAckermannSteering_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bAckermannSteering;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AckermannRatio_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_AckermannRatio;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FVehicleAxleSteeringConfig>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewProp_bAffectedBySteering_MetaData[] = {
		{ "Category", "SteeringSetup" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewProp_bAffectedBySteering_SetBit(void* Obj)
	{
		((FVehicleAxleSteeringConfig*)Obj)->bAffectedBySteering = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewProp_bAffectedBySteering = { "bAffectedBySteering", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FVehicleAxleSteeringConfig), &Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewProp_bAffectedBySteering_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewProp_bAffectedBySteering_MetaData), Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewProp_bAffectedBySteering_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewProp_MaxSteeringAngle_MetaData[] = {
		{ "Category", "SteeringSetup" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewProp_MaxSteeringAngle = { "MaxSteeringAngle", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleSteeringConfig, MaxSteeringAngle), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewProp_MaxSteeringAngle_MetaData), Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewProp_MaxSteeringAngle_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewProp_bAckermannSteering_MetaData[] = {
		{ "Category", "SteeringSetup" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewProp_bAckermannSteering_SetBit(void* Obj)
	{
		((FVehicleAxleSteeringConfig*)Obj)->bAckermannSteering = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewProp_bAckermannSteering = { "bAckermannSteering", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FVehicleAxleSteeringConfig), &Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewProp_bAckermannSteering_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewProp_bAckermannSteering_MetaData), Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewProp_bAckermannSteering_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewProp_AckermannRatio_MetaData[] = {
		{ "Category", "SteeringSetup" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewProp_AckermannRatio = { "AckermannRatio", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleSteeringConfig, AckermannRatio), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewProp_AckermannRatio_MetaData), Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewProp_AckermannRatio_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewProp_bAffectedBySteering,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewProp_MaxSteeringAngle,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewProp_bAckermannSteering,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewProp_AckermannRatio,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		&NewStructOps,
		"VehicleAxleSteeringConfig",
		Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::PropPointers),
		sizeof(FVehicleAxleSteeringConfig),
		alignof(FVehicleAxleSteeringConfig),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig()
	{
		if (!Z_Registration_Info_UScriptStruct_VehicleAxleSteeringConfig.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_VehicleAxleSteeringConfig.InnerSingleton, Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_VehicleAxleSteeringConfig.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_VehicleSteeringAssistConfig;
class UScriptStruct* FVehicleSteeringAssistConfig::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_VehicleSteeringAssistConfig.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_VehicleSteeringAssistConfig.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("VehicleSteeringAssistConfig"));
	}
	return Z_Registration_Info_UScriptStruct_VehicleSteeringAssistConfig.OuterSingleton;
}
template<> KINETIFORGE_API UScriptStruct* StaticStruct<FVehicleSteeringAssistConfig>()
{
	return FVehicleSteeringAssistConfig::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bSteeringAssistEnabled_MetaData[];
#endif
		static void NewProp_bSteeringAssistEnabled_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bSteeringAssistEnabled;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Level_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_Level;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Smoothing_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_Smoothing;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ActivationSpeed_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ActivationSpeed;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ActivationAngle_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ActivationAngle;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FVehicleSteeringAssistConfig>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_bSteeringAssistEnabled_MetaData[] = {
		{ "Category", "SteeringSetup" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "reduce steering angle when understeering or oversteering" },
#endif
	};
#endif
	void Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_bSteeringAssistEnabled_SetBit(void* Obj)
	{
		((FVehicleSteeringAssistConfig*)Obj)->bSteeringAssistEnabled = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_bSteeringAssistEnabled = { "bSteeringAssistEnabled", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FVehicleSteeringAssistConfig), &Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_bSteeringAssistEnabled_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_bSteeringAssistEnabled_MetaData), Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_bSteeringAssistEnabled_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_Level_MetaData[] = {
		{ "Category", "SteeringSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_Level = { "Level", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSteeringAssistConfig, Level), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_Level_MetaData), Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_Level_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_Smoothing_MetaData[] = {
		{ "Category", "SteeringSetup" },
		{ "ClampMax", "0.99999" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "steering assist smoothing(0 = immediately, 1 = never)" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_Smoothing = { "Smoothing", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSteeringAssistConfig, Smoothing), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_Smoothing_MetaData), Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_Smoothing_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_ActivationSpeed_MetaData[] = {
		{ "Category", "SteeringSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "minimum speed(m / s) to activate assist" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_ActivationSpeed = { "ActivationSpeed", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSteeringAssistConfig, ActivationSpeed), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_ActivationSpeed_MetaData), Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_ActivationSpeed_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_ActivationAngle_MetaData[] = {
		{ "Category", "SteeringSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "minimum angle(degrees) to activate assist" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_ActivationAngle = { "ActivationAngle", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleSteeringAssistConfig, ActivationAngle), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_ActivationAngle_MetaData), Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_ActivationAngle_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_bSteeringAssistEnabled,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_Level,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_Smoothing,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_ActivationSpeed,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewProp_ActivationAngle,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		&NewStructOps,
		"VehicleSteeringAssistConfig",
		Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::PropPointers),
		sizeof(FVehicleSteeringAssistConfig),
		alignof(FVehicleSteeringAssistConfig),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig()
	{
		if (!Z_Registration_Info_UScriptStruct_VehicleSteeringAssistConfig.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_VehicleSteeringAssistConfig.InnerSingleton, Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_VehicleSteeringAssistConfig.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_VehicleTCSConfig;
class UScriptStruct* FVehicleTCSConfig::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_VehicleTCSConfig.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_VehicleTCSConfig.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FVehicleTCSConfig, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("VehicleTCSConfig"));
	}
	return Z_Registration_Info_UScriptStruct_VehicleTCSConfig.OuterSingleton;
}
template<> KINETIFORGE_API UScriptStruct* StaticStruct<FVehicleTCSConfig>()
{
	return FVehicleTCSConfig::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bTractionControlSystemEnabled_MetaData[];
#endif
		static void NewProp_bTractionControlSystemEnabled_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bTractionControlSystemEnabled;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MaxSlipRatio_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_MaxSlipRatio;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ActivationSpeed_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ActivationSpeed;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_InterpSpeed_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InterpSpeed;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FVehicleTCSConfig>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::NewProp_bTractionControlSystemEnabled_MetaData[] = {
		{ "Category", "TCSSetup" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::NewProp_bTractionControlSystemEnabled_SetBit(void* Obj)
	{
		((FVehicleTCSConfig*)Obj)->bTractionControlSystemEnabled = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::NewProp_bTractionControlSystemEnabled = { "bTractionControlSystemEnabled", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FVehicleTCSConfig), &Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::NewProp_bTractionControlSystemEnabled_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::NewProp_bTractionControlSystemEnabled_MetaData), Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::NewProp_bTractionControlSystemEnabled_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::NewProp_MaxSlipRatio_MetaData[] = {
		{ "Category", "TCSSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::NewProp_MaxSlipRatio = { "MaxSlipRatio", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleTCSConfig, MaxSlipRatio), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::NewProp_MaxSlipRatio_MetaData), Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::NewProp_MaxSlipRatio_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::NewProp_ActivationSpeed_MetaData[] = {
		{ "Category", "TCSSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "minimum speed(m / s) to activate assist" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::NewProp_ActivationSpeed = { "ActivationSpeed", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleTCSConfig, ActivationSpeed), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::NewProp_ActivationSpeed_MetaData), Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::NewProp_ActivationSpeed_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::NewProp_InterpSpeed_MetaData[] = {
		{ "Category", "TCSSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::NewProp_InterpSpeed = { "InterpSpeed", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleTCSConfig, InterpSpeed), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::NewProp_InterpSpeed_MetaData), Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::NewProp_InterpSpeed_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::NewProp_bTractionControlSystemEnabled,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::NewProp_MaxSlipRatio,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::NewProp_ActivationSpeed,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::NewProp_InterpSpeed,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		&NewStructOps,
		"VehicleTCSConfig",
		Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::PropPointers),
		sizeof(FVehicleTCSConfig),
		alignof(FVehicleTCSConfig),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FVehicleTCSConfig()
	{
		if (!Z_Registration_Info_UScriptStruct_VehicleTCSConfig.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_VehicleTCSConfig.InnerSingleton, Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_VehicleTCSConfig.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_VehicleAxleSimData;
class UScriptStruct* FVehicleAxleSimData::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_VehicleAxleSimData.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_VehicleAxleSimData.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FVehicleAxleSimData, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("VehicleAxleSimData"));
	}
	return Z_Registration_Info_UScriptStruct_VehicleAxleSimData.OuterSingleton;
}
template<> KINETIFORGE_API UScriptStruct* StaticStruct<FVehicleAxleSimData>()
{
	return FVehicleAxleSimData::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bTCSTriggered_MetaData[];
#endif
		static void NewProp_bTCSTriggered_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bTCSTriggered;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_NumOfWheelOnGround_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_NumOfWheelOnGround;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_PhysicsDeltaTime_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_PhysicsDeltaTime;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_LeftWheelSteeringAngle_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_LeftWheelSteeringAngle;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RightWheelSteeringAngle_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_RightWheelSteeringAngle;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AxleDriveTorque_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_AxleDriveTorque;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_LeftDriveTorque_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_LeftDriveTorque;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RightDriveTorque_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_RightDriveTorque;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SwaybarForce_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_SwaybarForce;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ReflectedInertiaOnWheel_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ReflectedInertiaOnWheel;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BrakeTorque_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_BrakeTorque;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_HandbrakeTorque_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_HandbrakeTorque;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SteeringAssistInput_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_SteeringAssistInput;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RealSteeringValue_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_RealSteeringValue;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AxleTotalInertia_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_AxleTotalInertia;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AxleAngularVelocity_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_AxleAngularVelocity;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_P3MotorTorque_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_P3MotorTorque;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_WheelBase_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_WheelBase;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_WorldLinearVelocity_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_WorldLinearVelocity;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_LocalLinearVelocity_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_LocalLinearVelocity;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_CachedAxleCenter_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_CachedAxleCenter;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FVehicleAxleSimData>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_bTCSTriggered_MetaData[] = {
		{ "Category", "TractionControll" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_bTCSTriggered_SetBit(void* Obj)
	{
		((FVehicleAxleSimData*)Obj)->bTCSTriggered = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_bTCSTriggered = { "bTCSTriggered", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FVehicleAxleSimData), &Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_bTCSTriggered_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_bTCSTriggered_MetaData), Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_bTCSTriggered_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_NumOfWheelOnGround_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_NumOfWheelOnGround = { "NumOfWheelOnGround", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleSimData, NumOfWheelOnGround), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_NumOfWheelOnGround_MetaData), Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_NumOfWheelOnGround_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_PhysicsDeltaTime_MetaData[] = {
		{ "Category", "DeltaTime" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_PhysicsDeltaTime = { "PhysicsDeltaTime", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleSimData, PhysicsDeltaTime), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_PhysicsDeltaTime_MetaData), Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_PhysicsDeltaTime_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_LeftWheelSteeringAngle_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_LeftWheelSteeringAngle = { "LeftWheelSteeringAngle", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleSimData, LeftWheelSteeringAngle), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_LeftWheelSteeringAngle_MetaData), Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_LeftWheelSteeringAngle_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_RightWheelSteeringAngle_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_RightWheelSteeringAngle = { "RightWheelSteeringAngle", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleSimData, RightWheelSteeringAngle), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_RightWheelSteeringAngle_MetaData), Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_RightWheelSteeringAngle_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_AxleDriveTorque_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_AxleDriveTorque = { "AxleDriveTorque", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleSimData, AxleDriveTorque), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_AxleDriveTorque_MetaData), Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_AxleDriveTorque_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_LeftDriveTorque_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_LeftDriveTorque = { "LeftDriveTorque", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleSimData, LeftDriveTorque), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_LeftDriveTorque_MetaData), Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_LeftDriveTorque_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_RightDriveTorque_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_RightDriveTorque = { "RightDriveTorque", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleSimData, RightDriveTorque), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_RightDriveTorque_MetaData), Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_RightDriveTorque_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_SwaybarForce_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_SwaybarForce = { "SwaybarForce", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleSimData, SwaybarForce), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_SwaybarForce_MetaData), Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_SwaybarForce_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_ReflectedInertiaOnWheel_MetaData[] = {
		{ "Category", "Movement" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//for left wheel\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "for left wheel" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_ReflectedInertiaOnWheel = { "ReflectedInertiaOnWheel", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleSimData, ReflectedInertiaOnWheel), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_ReflectedInertiaOnWheel_MetaData), Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_ReflectedInertiaOnWheel_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_BrakeTorque_MetaData[] = {
		{ "Category", "Force" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//on single wheel\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "on single wheel" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_BrakeTorque = { "BrakeTorque", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleSimData, BrakeTorque), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_BrakeTorque_MetaData), Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_BrakeTorque_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_HandbrakeTorque_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_HandbrakeTorque = { "HandbrakeTorque", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleSimData, HandbrakeTorque), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_HandbrakeTorque_MetaData), Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_HandbrakeTorque_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_SteeringAssistInput_MetaData[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_SteeringAssistInput = { "SteeringAssistInput", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleSimData, SteeringAssistInput), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_SteeringAssistInput_MetaData), Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_SteeringAssistInput_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_RealSteeringValue_MetaData[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_RealSteeringValue = { "RealSteeringValue", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleSimData, RealSteeringValue), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_RealSteeringValue_MetaData), Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_RealSteeringValue_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_AxleTotalInertia_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_AxleTotalInertia = { "AxleTotalInertia", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleSimData, AxleTotalInertia), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_AxleTotalInertia_MetaData), Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_AxleTotalInertia_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_AxleAngularVelocity_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_AxleAngularVelocity = { "AxleAngularVelocity", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleSimData, AxleAngularVelocity), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_AxleAngularVelocity_MetaData), Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_AxleAngularVelocity_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_P3MotorTorque_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_P3MotorTorque = { "P3MotorTorque", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleSimData, P3MotorTorque), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_P3MotorTorque_MetaData), Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_P3MotorTorque_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_WheelBase_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_WheelBase = { "WheelBase", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleSimData, WheelBase), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_WheelBase_MetaData), Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_WheelBase_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_WorldLinearVelocity_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_WorldLinearVelocity = { "WorldLinearVelocity", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleSimData, WorldLinearVelocity), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_WorldLinearVelocity_MetaData), Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_WorldLinearVelocity_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_LocalLinearVelocity_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_LocalLinearVelocity = { "LocalLinearVelocity", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleSimData, LocalLinearVelocity), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_LocalLinearVelocity_MetaData), Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_LocalLinearVelocity_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_CachedAxleCenter_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleChassisParams.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_CachedAxleCenter = { "CachedAxleCenter", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleAxleSimData, CachedAxleCenter), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_CachedAxleCenter_MetaData), Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_CachedAxleCenter_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_bTCSTriggered,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_NumOfWheelOnGround,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_PhysicsDeltaTime,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_LeftWheelSteeringAngle,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_RightWheelSteeringAngle,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_AxleDriveTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_LeftDriveTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_RightDriveTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_SwaybarForce,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_ReflectedInertiaOnWheel,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_BrakeTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_HandbrakeTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_SteeringAssistInput,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_RealSteeringValue,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_AxleTotalInertia,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_AxleAngularVelocity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_P3MotorTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_WheelBase,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_WorldLinearVelocity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_LocalLinearVelocity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewProp_CachedAxleCenter,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		&NewStructOps,
		"VehicleAxleSimData",
		Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::PropPointers),
		sizeof(FVehicleAxleSimData),
		alignof(FVehicleAxleSimData),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FVehicleAxleSimData()
	{
		if (!Z_Registration_Info_UScriptStruct_VehicleAxleSimData.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_VehicleAxleSimData.InnerSingleton, Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_VehicleAxleSimData.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleChassisParams_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleChassisParams_h_Statics::EnumInfo[] = {
		{ ESuspensionType_StaticEnum, TEXT("ESuspensionType"), &Z_Registration_Info_UEnum_ESuspensionType, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 3408812646U) },
		{ ESuspensionRayCastMode_StaticEnum, TEXT("ESuspensionRayCastMode"), &Z_Registration_Info_UEnum_ESuspensionRayCastMode, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 1888833822U) },
		{ EPositionToApplyForce_StaticEnum, TEXT("EPositionToApplyForce"), &Z_Registration_Info_UEnum_EPositionToApplyForce, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 1459486099U) },
		{ ETireFrictionCombineMode_StaticEnum, TEXT("ETireFrictionCombineMode"), &Z_Registration_Info_UEnum_ETireFrictionCombineMode, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 869290566U) },
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleChassisParams_h_Statics::ScriptStructInfo[] = {
		{ FVehicleWheelConfig::StaticStruct, Z_Construct_UScriptStruct_FVehicleWheelConfig_Statics::NewStructOps, TEXT("VehicleWheelConfig"), &Z_Registration_Info_UScriptStruct_VehicleWheelConfig, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FVehicleWheelConfig), 3280903524U) },
		{ FVehicleTireConfig::StaticStruct, Z_Construct_UScriptStruct_FVehicleTireConfig_Statics::NewStructOps, TEXT("VehicleTireConfig"), &Z_Registration_Info_UScriptStruct_VehicleTireConfig, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FVehicleTireConfig), 3925339287U) },
		{ FVehicleABSConfig::StaticStruct, Z_Construct_UScriptStruct_FVehicleABSConfig_Statics::NewStructOps, TEXT("VehicleABSConfig"), &Z_Registration_Info_UScriptStruct_VehicleABSConfig, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FVehicleABSConfig), 2378670773U) },
		{ FVehicleWheelSimData::StaticStruct, Z_Construct_UScriptStruct_FVehicleWheelSimData_Statics::NewStructOps, TEXT("VehicleWheelSimData"), &Z_Registration_Info_UScriptStruct_VehicleWheelSimData, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FVehicleWheelSimData), 173970849U) },
		{ FVehicleSuspensionKinematicsConfig::StaticStruct, Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig_Statics::NewStructOps, TEXT("VehicleSuspensionKinematicsConfig"), &Z_Registration_Info_UScriptStruct_VehicleSuspensionKinematicsConfig, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FVehicleSuspensionKinematicsConfig), 2293807127U) },
		{ FVehicleSuspensionSpringConfig::StaticStruct, Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig_Statics::NewStructOps, TEXT("VehicleSuspensionSpringConfig"), &Z_Registration_Info_UScriptStruct_VehicleSuspensionSpringConfig, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FVehicleSuspensionSpringConfig), 1248612756U) },
		{ FVehicleSuspensionSimData::StaticStruct, Z_Construct_UScriptStruct_FVehicleSuspensionSimData_Statics::NewStructOps, TEXT("VehicleSuspensionSimData"), &Z_Registration_Info_UScriptStruct_VehicleSuspensionSimData, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FVehicleSuspensionSimData), 4192578155U) },
		{ FVehicleAxleConfig::StaticStruct, Z_Construct_UScriptStruct_FVehicleAxleConfig_Statics::NewStructOps, TEXT("VehicleAxleConfig"), &Z_Registration_Info_UScriptStruct_VehicleAxleConfig, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FVehicleAxleConfig), 71916922U) },
		{ FVehicleAxleSteeringConfig::StaticStruct, Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig_Statics::NewStructOps, TEXT("VehicleAxleSteeringConfig"), &Z_Registration_Info_UScriptStruct_VehicleAxleSteeringConfig, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FVehicleAxleSteeringConfig), 1863805507U) },
		{ FVehicleSteeringAssistConfig::StaticStruct, Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig_Statics::NewStructOps, TEXT("VehicleSteeringAssistConfig"), &Z_Registration_Info_UScriptStruct_VehicleSteeringAssistConfig, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FVehicleSteeringAssistConfig), 3644416740U) },
		{ FVehicleTCSConfig::StaticStruct, Z_Construct_UScriptStruct_FVehicleTCSConfig_Statics::NewStructOps, TEXT("VehicleTCSConfig"), &Z_Registration_Info_UScriptStruct_VehicleTCSConfig, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FVehicleTCSConfig), 1730587740U) },
		{ FVehicleAxleSimData::StaticStruct, Z_Construct_UScriptStruct_FVehicleAxleSimData_Statics::NewStructOps, TEXT("VehicleAxleSimData"), &Z_Registration_Info_UScriptStruct_VehicleAxleSimData, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FVehicleAxleSimData), 952266153U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleChassisParams_h_463171475(TEXT("/Script/KinetiForge"),
		nullptr, 0,
		Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleChassisParams_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleChassisParams_h_Statics::ScriptStructInfo),
		Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleChassisParams_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleChassisParams_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
