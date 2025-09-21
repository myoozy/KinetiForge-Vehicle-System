// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "KinetiForge/Public/VehicleEngineComponent.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeVehicleEngineComponent() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
	ENGINE_API UClass* Z_Construct_UClass_UCurveFloat_NoRegister();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleEngineComponent();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleEngineComponent_NoRegister();
	KINETIFORGE_API UEnum* Z_Construct_UEnum_KinetiForge_EEngineState();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleEngineSimData();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleEngineTurboConfig();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig();
	UPackage* Z_Construct_UPackage__Script_KinetiForge();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_EEngineState;
	static UEnum* EEngineState_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_EEngineState.OuterSingleton)
		{
			Z_Registration_Info_UEnum_EEngineState.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_KinetiForge_EEngineState, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("EEngineState"));
		}
		return Z_Registration_Info_UEnum_EEngineState.OuterSingleton;
	}
	template<> KINETIFORGE_API UEnum* StaticEnum<EEngineState>()
	{
		return EEngineState_StaticEnum();
	}
	struct Z_Construct_UEnum_KinetiForge_EEngineState_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_KinetiForge_EEngineState_Statics::Enumerators[] = {
		{ "EEngineState::On", (int64)EEngineState::On },
		{ "EEngineState::Off", (int64)EEngineState::Off },
		{ "EEngineState::Starting", (int64)EEngineState::Starting },
		{ "EEngineState::Shutting", (int64)EEngineState::Shutting },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_KinetiForge_EEngineState_Statics::Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
		{ "Off.DisplayName", "EngineOFF" },
		{ "Off.Name", "EEngineState::Off" },
		{ "On.DisplayName", "EngineON" },
		{ "On.Name", "EEngineState::On" },
		{ "Shutting.DisplayName", "EngineSHUTTING" },
		{ "Shutting.Name", "EEngineState::Shutting" },
		{ "Starting.DisplayName", "EngineSTARTING" },
		{ "Starting.Name", "EEngineState::Starting" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_KinetiForge_EEngineState_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		"EEngineState",
		"EEngineState",
		Z_Construct_UEnum_KinetiForge_EEngineState_Statics::Enumerators,
		RF_Public|RF_Transient|RF_MarkAsNative,
		UE_ARRAY_COUNT(Z_Construct_UEnum_KinetiForge_EEngineState_Statics::Enumerators),
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_KinetiForge_EEngineState_Statics::Enum_MetaDataParams), Z_Construct_UEnum_KinetiForge_EEngineState_Statics::Enum_MetaDataParams)
	};
	UEnum* Z_Construct_UEnum_KinetiForge_EEngineState()
	{
		if (!Z_Registration_Info_UEnum_EEngineState.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EEngineState.InnerSingleton, Z_Construct_UEnum_KinetiForge_EEngineState_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_EEngineState.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_VehicleNaturallyAspiratedEngineConfig;
class UScriptStruct* FVehicleNaturallyAspiratedEngineConfig::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_VehicleNaturallyAspiratedEngineConfig.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_VehicleNaturallyAspiratedEngineConfig.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("VehicleNaturallyAspiratedEngineConfig"));
	}
	return Z_Registration_Info_UScriptStruct_VehicleNaturallyAspiratedEngineConfig.OuterSingleton;
}
template<> KINETIFORGE_API UScriptStruct* StaticStruct<FVehicleNaturallyAspiratedEngineConfig>()
{
	return FVehicleNaturallyAspiratedEngineConfig::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MaxEngineTorque_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_MaxEngineTorque;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_EngineTorqueCurve_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_EngineTorqueCurve;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_EngineIdleRPM_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_EngineIdleRPM;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_EngineMaxRPM_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_EngineMaxRPM;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_EngineInertia_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_EngineInertia;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_StartFriction_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_StartFriction;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_FrictionCoefficient_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_FrictionCoefficient;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_IdleThrottleInterpSpeed_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_IdleThrottleInterpSpeed;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RevLimiterTime_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_RevLimiterTime;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FVehicleNaturallyAspiratedEngineConfig>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_MaxEngineTorque_MetaData[] = {
		{ "Category", "EngineSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_MaxEngineTorque = { "MaxEngineTorque", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleNaturallyAspiratedEngineConfig, MaxEngineTorque), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_MaxEngineTorque_MetaData), Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_MaxEngineTorque_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_EngineTorqueCurve_MetaData[] = {
		{ "Category", "EngineSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_EngineTorqueCurve = { "EngineTorqueCurve", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleNaturallyAspiratedEngineConfig, EngineTorqueCurve), Z_Construct_UClass_UCurveFloat_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_EngineTorqueCurve_MetaData), Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_EngineTorqueCurve_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_EngineIdleRPM_MetaData[] = {
		{ "Category", "EngineSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_EngineIdleRPM = { "EngineIdleRPM", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleNaturallyAspiratedEngineConfig, EngineIdleRPM), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_EngineIdleRPM_MetaData), Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_EngineIdleRPM_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_EngineMaxRPM_MetaData[] = {
		{ "Category", "EngineSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_EngineMaxRPM = { "EngineMaxRPM", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleNaturallyAspiratedEngineConfig, EngineMaxRPM), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_EngineMaxRPM_MetaData), Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_EngineMaxRPM_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_EngineInertia_MetaData[] = {
		{ "Category", "EngineSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_EngineInertia = { "EngineInertia", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleNaturallyAspiratedEngineConfig, EngineInertia), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_EngineInertia_MetaData), Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_EngineInertia_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_StartFriction_MetaData[] = {
		{ "Category", "EngineSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_StartFriction = { "StartFriction", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleNaturallyAspiratedEngineConfig, StartFriction), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_StartFriction_MetaData), Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_StartFriction_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_FrictionCoefficient_MetaData[] = {
		{ "Category", "EngineSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_FrictionCoefficient = { "FrictionCoefficient", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleNaturallyAspiratedEngineConfig, FrictionCoefficient), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_FrictionCoefficient_MetaData), Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_FrictionCoefficient_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_IdleThrottleInterpSpeed_MetaData[] = {
		{ "Category", "EngineSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_IdleThrottleInterpSpeed = { "IdleThrottleInterpSpeed", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleNaturallyAspiratedEngineConfig, IdleThrottleInterpSpeed), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_IdleThrottleInterpSpeed_MetaData), Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_IdleThrottleInterpSpeed_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_RevLimiterTime_MetaData[] = {
		{ "Category", "EngineSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_RevLimiterTime = { "RevLimiterTime", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleNaturallyAspiratedEngineConfig, RevLimiterTime), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_RevLimiterTime_MetaData), Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_RevLimiterTime_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_MaxEngineTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_EngineTorqueCurve,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_EngineIdleRPM,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_EngineMaxRPM,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_EngineInertia,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_StartFriction,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_FrictionCoefficient,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_IdleThrottleInterpSpeed,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewProp_RevLimiterTime,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		&NewStructOps,
		"VehicleNaturallyAspiratedEngineConfig",
		Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::PropPointers),
		sizeof(FVehicleNaturallyAspiratedEngineConfig),
		alignof(FVehicleNaturallyAspiratedEngineConfig),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig()
	{
		if (!Z_Registration_Info_UScriptStruct_VehicleNaturallyAspiratedEngineConfig.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_VehicleNaturallyAspiratedEngineConfig.InnerSingleton, Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_VehicleNaturallyAspiratedEngineConfig.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_VehicleEngineTurboConfig;
class UScriptStruct* FVehicleEngineTurboConfig::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_VehicleEngineTurboConfig.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_VehicleEngineTurboConfig.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FVehicleEngineTurboConfig, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("VehicleEngineTurboConfig"));
	}
	return Z_Registration_Info_UScriptStruct_VehicleEngineTurboConfig.OuterSingleton;
}
template<> KINETIFORGE_API UScriptStruct* StaticStruct<FVehicleEngineTurboConfig>()
{
	return FVehicleEngineTurboConfig::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TurboBoostTorque_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_TurboBoostTorque;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TurboStartRPM_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_TurboStartRPM;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TurboFinishRPM_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_TurboFinishRPM;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TurboMaxPressure_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_TurboMaxPressure;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TurboNegPressure_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_TurboNegPressure;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TurboLag_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_TurboLag;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TurboWasteGateLag_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_TurboWasteGateLag;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bAntiLag_MetaData[];
#endif
		static void NewProp_bAntiLag_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bAntiLag;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FVehicleEngineTurboConfig>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboBoostTorque_MetaData[] = {
		{ "Category", "TurboSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "if boost torque > 0, will be considered as turbo charged" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboBoostTorque = { "TurboBoostTorque", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleEngineTurboConfig, TurboBoostTorque), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboBoostTorque_MetaData), Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboBoostTorque_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboStartRPM_MetaData[] = {
		{ "Category", "TurboSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboStartRPM = { "TurboStartRPM", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleEngineTurboConfig, TurboStartRPM), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboStartRPM_MetaData), Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboStartRPM_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboFinishRPM_MetaData[] = {
		{ "Category", "TurboSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboFinishRPM = { "TurboFinishRPM", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleEngineTurboConfig, TurboFinishRPM), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboFinishRPM_MetaData), Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboFinishRPM_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboMaxPressure_MetaData[] = {
		{ "Category", "TurboSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboMaxPressure = { "TurboMaxPressure", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleEngineTurboConfig, TurboMaxPressure), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboMaxPressure_MetaData), Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboMaxPressure_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboNegPressure_MetaData[] = {
		{ "Category", "TurboSetup" },
		{ "ClampMax", "0.0" },
		{ "ClampMin", "-1.0" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboNegPressure = { "TurboNegPressure", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleEngineTurboConfig, TurboNegPressure), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboNegPressure_MetaData), Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboNegPressure_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboLag_MetaData[] = {
		{ "Category", "TurboSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboLag = { "TurboLag", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleEngineTurboConfig, TurboLag), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboLag_MetaData), Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboLag_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboWasteGateLag_MetaData[] = {
		{ "Category", "TurboSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboWasteGateLag = { "TurboWasteGateLag", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleEngineTurboConfig, TurboWasteGateLag), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboWasteGateLag_MetaData), Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboWasteGateLag_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_bAntiLag_MetaData[] = {
		{ "Category", "TurboSetup" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_bAntiLag_SetBit(void* Obj)
	{
		((FVehicleEngineTurboConfig*)Obj)->bAntiLag = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_bAntiLag = { "bAntiLag", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FVehicleEngineTurboConfig), &Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_bAntiLag_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_bAntiLag_MetaData), Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_bAntiLag_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboBoostTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboStartRPM,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboFinishRPM,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboMaxPressure,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboNegPressure,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboLag,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_TurboWasteGateLag,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewProp_bAntiLag,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		&NewStructOps,
		"VehicleEngineTurboConfig",
		Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::PropPointers),
		sizeof(FVehicleEngineTurboConfig),
		alignof(FVehicleEngineTurboConfig),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FVehicleEngineTurboConfig()
	{
		if (!Z_Registration_Info_UScriptStruct_VehicleEngineTurboConfig.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_VehicleEngineTurboConfig.InnerSingleton, Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_VehicleEngineTurboConfig.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_VehicleEngineSimData;
class UScriptStruct* FVehicleEngineSimData::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_VehicleEngineSimData.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_VehicleEngineSimData.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FVehicleEngineSimData, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("VehicleEngineSimData"));
	}
	return Z_Registration_Info_UScriptStruct_VehicleEngineSimData.OuterSingleton;
}
template<> KINETIFORGE_API UScriptStruct* StaticStruct<FVehicleEngineSimData>()
{
	return FVehicleEngineSimData::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
		static const UECodeGen_Private::FBytePropertyParams NewProp_State_Underlying;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_State_MetaData[];
#endif
		static const UECodeGen_Private::FEnumPropertyParams NewProp_State;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_PhysicsDeltaTime_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_PhysicsDeltaTime;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_EngineAngularVelocity_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_EngineAngularVelocity;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RawThrottleInput_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_RawThrottleInput;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_IdleThrottle_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_IdleThrottle;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RealThrottle_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_RealThrottle;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_EffectiveTorque_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_EffectiveTorque;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_LoadTorque_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_LoadTorque;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TorqueRequiredToStartEngine_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_TorqueRequiredToStartEngine;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_StarterMotorTorque_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_StarterMotorTorque;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_EngineRPM_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_EngineRPM;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_EngineOffRPM_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_EngineOffRPM;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TurboPressure_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_TurboPressure;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RevLimiterTimer_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_RevLimiterTimer;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_P1MotorTorque_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_P1MotorTorque;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bSpark_MetaData[];
#endif
		static void NewProp_bSpark_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bSpark;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bIdleRPMInValid_MetaData[];
#endif
		static void NewProp_bIdleRPMInValid_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bIdleRPMInValid;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FVehicleEngineSimData>();
	}
	const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_State_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_State_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_State = { "State", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleEngineSimData, State), Z_Construct_UEnum_KinetiForge_EEngineState, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_State_MetaData), Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_State_MetaData) }; // 529851397
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_PhysicsDeltaTime_MetaData[] = {
		{ "Category", "DeltaTime" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_PhysicsDeltaTime = { "PhysicsDeltaTime", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleEngineSimData, PhysicsDeltaTime), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_PhysicsDeltaTime_MetaData), Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_PhysicsDeltaTime_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_EngineAngularVelocity_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_EngineAngularVelocity = { "EngineAngularVelocity", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleEngineSimData, EngineAngularVelocity), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_EngineAngularVelocity_MetaData), Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_EngineAngularVelocity_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_RawThrottleInput_MetaData[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_RawThrottleInput = { "RawThrottleInput", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleEngineSimData, RawThrottleInput), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_RawThrottleInput_MetaData), Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_RawThrottleInput_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_IdleThrottle_MetaData[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_IdleThrottle = { "IdleThrottle", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleEngineSimData, IdleThrottle), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_IdleThrottle_MetaData), Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_IdleThrottle_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_RealThrottle_MetaData[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_RealThrottle = { "RealThrottle", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleEngineSimData, RealThrottle), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_RealThrottle_MetaData), Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_RealThrottle_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_EffectiveTorque_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_EffectiveTorque = { "EffectiveTorque", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleEngineSimData, EffectiveTorque), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_EffectiveTorque_MetaData), Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_EffectiveTorque_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_LoadTorque_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_LoadTorque = { "LoadTorque", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleEngineSimData, LoadTorque), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_LoadTorque_MetaData), Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_LoadTorque_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_TorqueRequiredToStartEngine_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_TorqueRequiredToStartEngine = { "TorqueRequiredToStartEngine", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleEngineSimData, TorqueRequiredToStartEngine), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_TorqueRequiredToStartEngine_MetaData), Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_TorqueRequiredToStartEngine_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_StarterMotorTorque_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_StarterMotorTorque = { "StarterMotorTorque", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleEngineSimData, StarterMotorTorque), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_StarterMotorTorque_MetaData), Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_StarterMotorTorque_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_EngineRPM_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_EngineRPM = { "EngineRPM", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleEngineSimData, EngineRPM), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_EngineRPM_MetaData), Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_EngineRPM_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_EngineOffRPM_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_EngineOffRPM = { "EngineOffRPM", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleEngineSimData, EngineOffRPM), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_EngineOffRPM_MetaData), Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_EngineOffRPM_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_TurboPressure_MetaData[] = {
		{ "Category", "Turbo" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//under this rpm, the engine will be considered as off\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "under this rpm, the engine will be considered as off" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_TurboPressure = { "TurboPressure", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleEngineSimData, TurboPressure), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_TurboPressure_MetaData), Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_TurboPressure_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_RevLimiterTimer_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_RevLimiterTimer = { "RevLimiterTimer", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleEngineSimData, RevLimiterTimer), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_RevLimiterTimer_MetaData), Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_RevLimiterTimer_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_P1MotorTorque_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_P1MotorTorque = { "P1MotorTorque", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleEngineSimData, P1MotorTorque), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_P1MotorTorque_MetaData), Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_P1MotorTorque_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_bSpark_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_bSpark_SetBit(void* Obj)
	{
		((FVehicleEngineSimData*)Obj)->bSpark = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_bSpark = { "bSpark", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FVehicleEngineSimData), &Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_bSpark_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_bSpark_MetaData), Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_bSpark_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_bIdleRPMInValid_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_bIdleRPMInValid_SetBit(void* Obj)
	{
		((FVehicleEngineSimData*)Obj)->bIdleRPMInValid = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_bIdleRPMInValid = { "bIdleRPMInValid", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FVehicleEngineSimData), &Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_bIdleRPMInValid_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_bIdleRPMInValid_MetaData), Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_bIdleRPMInValid_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_State_Underlying,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_State,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_PhysicsDeltaTime,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_EngineAngularVelocity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_RawThrottleInput,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_IdleThrottle,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_RealThrottle,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_EffectiveTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_LoadTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_TorqueRequiredToStartEngine,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_StarterMotorTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_EngineRPM,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_EngineOffRPM,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_TurboPressure,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_RevLimiterTimer,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_P1MotorTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_bSpark,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewProp_bIdleRPMInValid,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		&NewStructOps,
		"VehicleEngineSimData",
		Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::PropPointers),
		sizeof(FVehicleEngineSimData),
		alignof(FVehicleEngineSimData),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FVehicleEngineSimData()
	{
		if (!Z_Registration_Info_UScriptStruct_VehicleEngineSimData.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_VehicleEngineSimData.InnerSingleton, Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_VehicleEngineSimData.InnerSingleton;
	}
	DEFINE_FUNCTION(UVehicleEngineComponent::execShutVehicleEngine)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(EEngineState*)Z_Param__Result=P_THIS->ShutVehicleEngine();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleEngineComponent::execStartVehicleEngine)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(EEngineState*)Z_Param__Result=P_THIS->StartVehicleEngine();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleEngineComponent::execInitialize)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->Initialize();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleEngineComponent::execGetP1MotorTorque)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=P_THIS->GetP1MotorTorque();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleEngineComponent::execSetP1MotorTorque)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_NewTorque);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->SetP1MotorTorque(Z_Param_NewTorque);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleEngineComponent::execGetEngineMovement)
	{
		P_GET_STRUCT_REF(FVehicleEngineSimData,Z_Param_Out_Out);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->GetEngineMovement(Z_Param_Out_Out);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleEngineComponent::execUpdatePhysics)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_InDeltaTime);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InThrottle);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InLoadTorque);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->UpdatePhysics(Z_Param_InDeltaTime,Z_Param_InThrottle,Z_Param_InLoadTorque);
		P_NATIVE_END;
	}
	void UVehicleEngineComponent::StaticRegisterNativesUVehicleEngineComponent()
	{
		UClass* Class = UVehicleEngineComponent::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "GetEngineMovement", &UVehicleEngineComponent::execGetEngineMovement },
			{ "GetP1MotorTorque", &UVehicleEngineComponent::execGetP1MotorTorque },
			{ "Initialize", &UVehicleEngineComponent::execInitialize },
			{ "SetP1MotorTorque", &UVehicleEngineComponent::execSetP1MotorTorque },
			{ "ShutVehicleEngine", &UVehicleEngineComponent::execShutVehicleEngine },
			{ "StartVehicleEngine", &UVehicleEngineComponent::execStartVehicleEngine },
			{ "UpdatePhysics", &UVehicleEngineComponent::execUpdatePhysics },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UVehicleEngineComponent_GetEngineMovement_Statics
	{
		struct VehicleEngineComponent_eventGetEngineMovement_Parms
		{
			FVehicleEngineSimData Out;
		};
		static const UECodeGen_Private::FStructPropertyParams NewProp_Out;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleEngineComponent_GetEngineMovement_Statics::NewProp_Out = { "Out", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleEngineComponent_eventGetEngineMovement_Parms, Out), Z_Construct_UScriptStruct_FVehicleEngineSimData, METADATA_PARAMS(0, nullptr) }; // 1044552124
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleEngineComponent_GetEngineMovement_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleEngineComponent_GetEngineMovement_Statics::NewProp_Out,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleEngineComponent_GetEngineMovement_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleEngineComponent_GetEngineMovement_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleEngineComponent, nullptr, "GetEngineMovement", nullptr, nullptr, Z_Construct_UFunction_UVehicleEngineComponent_GetEngineMovement_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleEngineComponent_GetEngineMovement_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleEngineComponent_GetEngineMovement_Statics::VehicleEngineComponent_eventGetEngineMovement_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleEngineComponent_GetEngineMovement_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleEngineComponent_GetEngineMovement_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleEngineComponent_GetEngineMovement_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleEngineComponent_GetEngineMovement_Statics::VehicleEngineComponent_eventGetEngineMovement_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleEngineComponent_GetEngineMovement()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleEngineComponent_GetEngineMovement_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleEngineComponent_GetP1MotorTorque_Statics
	{
		struct VehicleEngineComponent_eventGetP1MotorTorque_Parms
		{
			float ReturnValue;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleEngineComponent_GetP1MotorTorque_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleEngineComponent_eventGetP1MotorTorque_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleEngineComponent_GetP1MotorTorque_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleEngineComponent_GetP1MotorTorque_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleEngineComponent_GetP1MotorTorque_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleEngineComponent_GetP1MotorTorque_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleEngineComponent, nullptr, "GetP1MotorTorque", nullptr, nullptr, Z_Construct_UFunction_UVehicleEngineComponent_GetP1MotorTorque_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleEngineComponent_GetP1MotorTorque_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleEngineComponent_GetP1MotorTorque_Statics::VehicleEngineComponent_eventGetP1MotorTorque_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleEngineComponent_GetP1MotorTorque_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleEngineComponent_GetP1MotorTorque_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleEngineComponent_GetP1MotorTorque_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleEngineComponent_GetP1MotorTorque_Statics::VehicleEngineComponent_eventGetP1MotorTorque_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleEngineComponent_GetP1MotorTorque()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleEngineComponent_GetP1MotorTorque_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleEngineComponent_Initialize_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleEngineComponent_Initialize_Statics::Function_MetaDataParams[] = {
		{ "Category", "Initialize" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleEngineComponent_Initialize_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleEngineComponent, nullptr, "Initialize", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleEngineComponent_Initialize_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleEngineComponent_Initialize_Statics::Function_MetaDataParams) };
	UFunction* Z_Construct_UFunction_UVehicleEngineComponent_Initialize()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleEngineComponent_Initialize_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleEngineComponent_SetP1MotorTorque_Statics
	{
		struct VehicleEngineComponent_eventSetP1MotorTorque_Parms
		{
			float NewTorque;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_NewTorque;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleEngineComponent_SetP1MotorTorque_Statics::NewProp_NewTorque = { "NewTorque", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleEngineComponent_eventSetP1MotorTorque_Parms, NewTorque), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleEngineComponent_SetP1MotorTorque_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleEngineComponent_SetP1MotorTorque_Statics::NewProp_NewTorque,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleEngineComponent_SetP1MotorTorque_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleEngineComponent_SetP1MotorTorque_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleEngineComponent, nullptr, "SetP1MotorTorque", nullptr, nullptr, Z_Construct_UFunction_UVehicleEngineComponent_SetP1MotorTorque_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleEngineComponent_SetP1MotorTorque_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleEngineComponent_SetP1MotorTorque_Statics::VehicleEngineComponent_eventSetP1MotorTorque_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleEngineComponent_SetP1MotorTorque_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleEngineComponent_SetP1MotorTorque_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleEngineComponent_SetP1MotorTorque_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleEngineComponent_SetP1MotorTorque_Statics::VehicleEngineComponent_eventSetP1MotorTorque_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleEngineComponent_SetP1MotorTorque()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleEngineComponent_SetP1MotorTorque_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleEngineComponent_ShutVehicleEngine_Statics
	{
		struct VehicleEngineComponent_eventShutVehicleEngine_Parms
		{
			EEngineState ReturnValue;
		};
		static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Underlying;
		static const UECodeGen_Private::FEnumPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UVehicleEngineComponent_ShutVehicleEngine_Statics::NewProp_ReturnValue_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UVehicleEngineComponent_ShutVehicleEngine_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleEngineComponent_eventShutVehicleEngine_Parms, ReturnValue), Z_Construct_UEnum_KinetiForge_EEngineState, METADATA_PARAMS(0, nullptr) }; // 529851397
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleEngineComponent_ShutVehicleEngine_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleEngineComponent_ShutVehicleEngine_Statics::NewProp_ReturnValue_Underlying,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleEngineComponent_ShutVehicleEngine_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleEngineComponent_ShutVehicleEngine_Statics::Function_MetaDataParams[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleEngineComponent_ShutVehicleEngine_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleEngineComponent, nullptr, "ShutVehicleEngine", nullptr, nullptr, Z_Construct_UFunction_UVehicleEngineComponent_ShutVehicleEngine_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleEngineComponent_ShutVehicleEngine_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleEngineComponent_ShutVehicleEngine_Statics::VehicleEngineComponent_eventShutVehicleEngine_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleEngineComponent_ShutVehicleEngine_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleEngineComponent_ShutVehicleEngine_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleEngineComponent_ShutVehicleEngine_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleEngineComponent_ShutVehicleEngine_Statics::VehicleEngineComponent_eventShutVehicleEngine_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleEngineComponent_ShutVehicleEngine()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleEngineComponent_ShutVehicleEngine_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleEngineComponent_StartVehicleEngine_Statics
	{
		struct VehicleEngineComponent_eventStartVehicleEngine_Parms
		{
			EEngineState ReturnValue;
		};
		static const UECodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Underlying;
		static const UECodeGen_Private::FEnumPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UVehicleEngineComponent_StartVehicleEngine_Statics::NewProp_ReturnValue_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UVehicleEngineComponent_StartVehicleEngine_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleEngineComponent_eventStartVehicleEngine_Parms, ReturnValue), Z_Construct_UEnum_KinetiForge_EEngineState, METADATA_PARAMS(0, nullptr) }; // 529851397
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleEngineComponent_StartVehicleEngine_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleEngineComponent_StartVehicleEngine_Statics::NewProp_ReturnValue_Underlying,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleEngineComponent_StartVehicleEngine_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleEngineComponent_StartVehicleEngine_Statics::Function_MetaDataParams[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleEngineComponent_StartVehicleEngine_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleEngineComponent, nullptr, "StartVehicleEngine", nullptr, nullptr, Z_Construct_UFunction_UVehicleEngineComponent_StartVehicleEngine_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleEngineComponent_StartVehicleEngine_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleEngineComponent_StartVehicleEngine_Statics::VehicleEngineComponent_eventStartVehicleEngine_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleEngineComponent_StartVehicleEngine_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleEngineComponent_StartVehicleEngine_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleEngineComponent_StartVehicleEngine_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleEngineComponent_StartVehicleEngine_Statics::VehicleEngineComponent_eventStartVehicleEngine_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleEngineComponent_StartVehicleEngine()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleEngineComponent_StartVehicleEngine_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleEngineComponent_UpdatePhysics_Statics
	{
		struct VehicleEngineComponent_eventUpdatePhysics_Parms
		{
			float InDeltaTime;
			float InThrottle;
			float InLoadTorque;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InDeltaTime;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InThrottle;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InLoadTorque;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleEngineComponent_UpdatePhysics_Statics::NewProp_InDeltaTime = { "InDeltaTime", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleEngineComponent_eventUpdatePhysics_Parms, InDeltaTime), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleEngineComponent_UpdatePhysics_Statics::NewProp_InThrottle = { "InThrottle", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleEngineComponent_eventUpdatePhysics_Parms, InThrottle), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleEngineComponent_UpdatePhysics_Statics::NewProp_InLoadTorque = { "InLoadTorque", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleEngineComponent_eventUpdatePhysics_Parms, InLoadTorque), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleEngineComponent_UpdatePhysics_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleEngineComponent_UpdatePhysics_Statics::NewProp_InDeltaTime,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleEngineComponent_UpdatePhysics_Statics::NewProp_InThrottle,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleEngineComponent_UpdatePhysics_Statics::NewProp_InLoadTorque,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleEngineComponent_UpdatePhysics_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleEngineComponent_UpdatePhysics_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleEngineComponent, nullptr, "UpdatePhysics", nullptr, nullptr, Z_Construct_UFunction_UVehicleEngineComponent_UpdatePhysics_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleEngineComponent_UpdatePhysics_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleEngineComponent_UpdatePhysics_Statics::VehicleEngineComponent_eventUpdatePhysics_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleEngineComponent_UpdatePhysics_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleEngineComponent_UpdatePhysics_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleEngineComponent_UpdatePhysics_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleEngineComponent_UpdatePhysics_Statics::VehicleEngineComponent_eventUpdatePhysics_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleEngineComponent_UpdatePhysics()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleEngineComponent_UpdatePhysics_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UVehicleEngineComponent);
	UClass* Z_Construct_UClass_UVehicleEngineComponent_NoRegister()
	{
		return UVehicleEngineComponent::StaticClass();
	}
	struct Z_Construct_UClass_UVehicleEngineComponent_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_NAConfig_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_NAConfig;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TurboConfig_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_TurboConfig;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UVehicleEngineComponent_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UActorComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleEngineComponent_Statics::DependentSingletons) < 16);
	const FClassFunctionLinkInfo Z_Construct_UClass_UVehicleEngineComponent_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UVehicleEngineComponent_GetEngineMovement, "GetEngineMovement" }, // 850446645
		{ &Z_Construct_UFunction_UVehicleEngineComponent_GetP1MotorTorque, "GetP1MotorTorque" }, // 181103083
		{ &Z_Construct_UFunction_UVehicleEngineComponent_Initialize, "Initialize" }, // 1804421918
		{ &Z_Construct_UFunction_UVehicleEngineComponent_SetP1MotorTorque, "SetP1MotorTorque" }, // 944191678
		{ &Z_Construct_UFunction_UVehicleEngineComponent_ShutVehicleEngine, "ShutVehicleEngine" }, // 2185043605
		{ &Z_Construct_UFunction_UVehicleEngineComponent_StartVehicleEngine, "StartVehicleEngine" }, // 1289103465
		{ &Z_Construct_UFunction_UVehicleEngineComponent_UpdatePhysics, "UpdatePhysics" }, // 2284957937
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleEngineComponent_Statics::FuncInfo) < 2048);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleEngineComponent_Statics::Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "BlueprintType", "true" },
		{ "ClassGroupNames", "Custom" },
		{ "IncludePath", "VehicleEngineComponent.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleEngineComponent_Statics::NewProp_NAConfig_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UVehicleEngineComponent_Statics::NewProp_NAConfig = { "NAConfig", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleEngineComponent, NAConfig), Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleEngineComponent_Statics::NewProp_NAConfig_MetaData), Z_Construct_UClass_UVehicleEngineComponent_Statics::NewProp_NAConfig_MetaData) }; // 2089463789
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleEngineComponent_Statics::NewProp_TurboConfig_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleEngineComponent.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UVehicleEngineComponent_Statics::NewProp_TurboConfig = { "TurboConfig", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleEngineComponent, TurboConfig), Z_Construct_UScriptStruct_FVehicleEngineTurboConfig, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleEngineComponent_Statics::NewProp_TurboConfig_MetaData), Z_Construct_UClass_UVehicleEngineComponent_Statics::NewProp_TurboConfig_MetaData) }; // 1113193447
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UVehicleEngineComponent_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleEngineComponent_Statics::NewProp_NAConfig,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleEngineComponent_Statics::NewProp_TurboConfig,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UVehicleEngineComponent_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVehicleEngineComponent>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UVehicleEngineComponent_Statics::ClassParams = {
		&UVehicleEngineComponent::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_UVehicleEngineComponent_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleEngineComponent_Statics::PropPointers),
		0,
		0x00B000A4u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleEngineComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UVehicleEngineComponent_Statics::Class_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleEngineComponent_Statics::PropPointers) < 2048);
	UClass* Z_Construct_UClass_UVehicleEngineComponent()
	{
		if (!Z_Registration_Info_UClass_UVehicleEngineComponent.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVehicleEngineComponent.OuterSingleton, Z_Construct_UClass_UVehicleEngineComponent_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UVehicleEngineComponent.OuterSingleton;
	}
	template<> KINETIFORGE_API UClass* StaticClass<UVehicleEngineComponent>()
	{
		return UVehicleEngineComponent::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UVehicleEngineComponent);
	UVehicleEngineComponent::~UVehicleEngineComponent() {}
	struct Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_Statics::EnumInfo[] = {
		{ EEngineState_StaticEnum, TEXT("EEngineState"), &Z_Registration_Info_UEnum_EEngineState, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 529851397U) },
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_Statics::ScriptStructInfo[] = {
		{ FVehicleNaturallyAspiratedEngineConfig::StaticStruct, Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics::NewStructOps, TEXT("VehicleNaturallyAspiratedEngineConfig"), &Z_Registration_Info_UScriptStruct_VehicleNaturallyAspiratedEngineConfig, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FVehicleNaturallyAspiratedEngineConfig), 2089463789U) },
		{ FVehicleEngineTurboConfig::StaticStruct, Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics::NewStructOps, TEXT("VehicleEngineTurboConfig"), &Z_Registration_Info_UScriptStruct_VehicleEngineTurboConfig, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FVehicleEngineTurboConfig), 1113193447U) },
		{ FVehicleEngineSimData::StaticStruct, Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics::NewStructOps, TEXT("VehicleEngineSimData"), &Z_Registration_Info_UScriptStruct_VehicleEngineSimData, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FVehicleEngineSimData), 1044552124U) },
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UVehicleEngineComponent, UVehicleEngineComponent::StaticClass, TEXT("UVehicleEngineComponent"), &Z_Registration_Info_UClass_UVehicleEngineComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVehicleEngineComponent), 2447288489U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_4272119886(TEXT("/Script/KinetiForge"),
		Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_Statics::ClassInfo),
		Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_Statics::ScriptStructInfo),
		Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
