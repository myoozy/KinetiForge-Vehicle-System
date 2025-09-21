// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "KinetiForge/Public/VehicleClutchComponent.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeVehicleClutchComponent() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleClutchComponent();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleClutchComponent_NoRegister();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleEngineComponent_NoRegister();
	KINETIFORGE_API UEnum* Z_Construct_UEnum_KinetiForge_EClutchSimMode();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleClutchConfig();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleClutchSimData();
	UPackage* Z_Construct_UPackage__Script_KinetiForge();
// End Cross Module References
	static FEnumRegistrationInfo Z_Registration_Info_UEnum_EClutchSimMode;
	static UEnum* EClutchSimMode_StaticEnum()
	{
		if (!Z_Registration_Info_UEnum_EClutchSimMode.OuterSingleton)
		{
			Z_Registration_Info_UEnum_EClutchSimMode.OuterSingleton = GetStaticEnum(Z_Construct_UEnum_KinetiForge_EClutchSimMode, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("EClutchSimMode"));
		}
		return Z_Registration_Info_UEnum_EClutchSimMode.OuterSingleton;
	}
	template<> KINETIFORGE_API UEnum* StaticEnum<EClutchSimMode>()
	{
		return EClutchSimMode_StaticEnum();
	}
	struct Z_Construct_UEnum_KinetiForge_EClutchSimMode_Statics
	{
		static const UECodeGen_Private::FEnumeratorParam Enumerators[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[];
#endif
		static const UECodeGen_Private::FEnumParams EnumParams;
	};
	const UECodeGen_Private::FEnumeratorParam Z_Construct_UEnum_KinetiForge_EClutchSimMode_Statics::Enumerators[] = {
		{ "EClutchSimMode::SpringModel", (int64)EClutchSimMode::SpringModel },
		{ "EClutchSimMode::DampingModel", (int64)EClutchSimMode::DampingModel },
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UEnum_KinetiForge_EClutchSimMode_Statics::Enum_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "DampingModel.Name", "EClutchSimMode::DampingModel" },
		{ "DampingModel.ToolTip", "always safe to use" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
		{ "SpringModel.Name", "EClutchSimMode::SpringModel" },
		{ "SpringModel.ToolTip", "more realistic, but not recommended for Arcade/EVs/Large physics time step" },
	};
#endif
	const UECodeGen_Private::FEnumParams Z_Construct_UEnum_KinetiForge_EClutchSimMode_Statics::EnumParams = {
		(UObject*(*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		"EClutchSimMode",
		"EClutchSimMode",
		Z_Construct_UEnum_KinetiForge_EClutchSimMode_Statics::Enumerators,
		RF_Public|RF_Transient|RF_MarkAsNative,
		UE_ARRAY_COUNT(Z_Construct_UEnum_KinetiForge_EClutchSimMode_Statics::Enumerators),
		EEnumFlags::None,
		(uint8)UEnum::ECppForm::EnumClass,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UEnum_KinetiForge_EClutchSimMode_Statics::Enum_MetaDataParams), Z_Construct_UEnum_KinetiForge_EClutchSimMode_Statics::Enum_MetaDataParams)
	};
	UEnum* Z_Construct_UEnum_KinetiForge_EClutchSimMode()
	{
		if (!Z_Registration_Info_UEnum_EClutchSimMode.InnerSingleton)
		{
			UECodeGen_Private::ConstructUEnum(Z_Registration_Info_UEnum_EClutchSimMode.InnerSingleton, Z_Construct_UEnum_KinetiForge_EClutchSimMode_Statics::EnumParams);
		}
		return Z_Registration_Info_UEnum_EClutchSimMode.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_VehicleClutchConfig;
class UScriptStruct* FVehicleClutchConfig::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_VehicleClutchConfig.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_VehicleClutchConfig.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FVehicleClutchConfig, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("VehicleClutchConfig"));
	}
	return Z_Registration_Info_UScriptStruct_VehicleClutchConfig.OuterSingleton;
}
template<> KINETIFORGE_API UScriptStruct* StaticStruct<FVehicleClutchConfig>()
{
	return FVehicleClutchConfig::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_NaturalFrequency_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_NaturalFrequency;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Damping_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_Damping;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Capacity_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_Capacity;
		static const UECodeGen_Private::FBytePropertyParams NewProp_SimMode_Underlying;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SimMode_MetaData[];
#endif
		static const UECodeGen_Private::FEnumPropertyParams NewProp_SimMode;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FVehicleClutchConfig>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::NewProp_NaturalFrequency_MetaData[] = {
		{ "Category", "ClutchSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "The natural frequency that the clutch tries to reach, it should be lower than game physics frequency" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::NewProp_NaturalFrequency = { "NaturalFrequency", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleClutchConfig, NaturalFrequency), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::NewProp_NaturalFrequency_MetaData), Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::NewProp_NaturalFrequency_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::NewProp_Damping_MetaData[] = {
		{ "Category", "ClutchSetup" },
		{ "ClampMax", "1.0" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Attention!!! If SimMode is SpringModel, then Damping refers to damping ratio of the spring. If SimMode is DampingModel, then Damping refers to smoothing factor!" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::NewProp_Damping = { "Damping", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleClutchConfig, Damping), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::NewProp_Damping_MetaData), Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::NewProp_Damping_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::NewProp_Capacity_MetaData[] = {
		{ "Category", "ClutchSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::NewProp_Capacity = { "Capacity", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleClutchConfig, Capacity), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::NewProp_Capacity_MetaData), Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::NewProp_Capacity_MetaData) };
	const UECodeGen_Private::FBytePropertyParams Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::NewProp_SimMode_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::NewProp_SimMode_MetaData[] = {
		{ "Category", "ClutchSetup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "SpringModel: Using a spring to transmit torque. Using this model will cause engine speed fluctuations (especially when shifting gears with a sequential transmission and low damping ratio). It is recommended to use this model when the physical step size is short (because the natural angular frequency of the spring cannot exceed the game's physical simulation frequency). If the physical step size is large, stiffness can be increased by increasing damping ratio; DampingModel: Use critical damping to transfer torque. Critical damping is calculated based on the current rotational inertia and game physics frequency. The torque is then smoothed to avoid numerical jitter. The torque values are very smooth and do not cause speed jitter during gear changes. This model is recommended for electric vehicles (as electric vehicles typically do not have clutches)." },
#endif
	};
#endif
	const UECodeGen_Private::FEnumPropertyParams Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::NewProp_SimMode = { "SimMode", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleClutchConfig, SimMode), Z_Construct_UEnum_KinetiForge_EClutchSimMode, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::NewProp_SimMode_MetaData), Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::NewProp_SimMode_MetaData) }; // 385656090
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::NewProp_NaturalFrequency,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::NewProp_Damping,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::NewProp_Capacity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::NewProp_SimMode_Underlying,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::NewProp_SimMode,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		&NewStructOps,
		"VehicleClutchConfig",
		Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::PropPointers),
		sizeof(FVehicleClutchConfig),
		alignof(FVehicleClutchConfig),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FVehicleClutchConfig()
	{
		if (!Z_Registration_Info_UScriptStruct_VehicleClutchConfig.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_VehicleClutchConfig.InnerSingleton, Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_VehicleClutchConfig.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_VehicleClutchSimData;
class UScriptStruct* FVehicleClutchSimData::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_VehicleClutchSimData.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_VehicleClutchSimData.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FVehicleClutchSimData, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("VehicleClutchSimData"));
	}
	return Z_Registration_Info_UScriptStruct_VehicleClutchSimData.OuterSingleton;
}
template<> KINETIFORGE_API UScriptStruct* StaticStruct<FVehicleClutchSimData>()
{
	return FVehicleClutchSimData::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_PhysicsDeltaTime_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_PhysicsDeltaTime;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SpringStiffness_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_SpringStiffness;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SpringDamping_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_SpringDamping;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_CriticalDamping_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_CriticalDamping;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MaxNaturalFrequency_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_MaxNaturalFrequency;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_EngineInertia_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_EngineInertia;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_EngineAngularVelocity_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_EngineAngularVelocity;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ClutchAngularVelocity_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ClutchAngularVelocity;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ClutchSlip_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ClutchSlip;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AngleDiff_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_AngleDiff;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ClutchLock_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ClutchLock;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MaxClutchTorque_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_MaxClutchTorque;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SmoothenedTorque_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_SmoothenedTorque;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ClutchTorque_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ClutchTorque;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TotalInertia_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_TotalInertia;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ReflectedInertia_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ReflectedInertia;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_FirstGearReflectedInertia_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_FirstGearReflectedInertia;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ClutchSlipping_MetaData[];
#endif
		static void NewProp_ClutchSlipping_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_ClutchSlipping;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FVehicleClutchSimData>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_PhysicsDeltaTime_MetaData[] = {
		{ "Category", "DeltaTime" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_PhysicsDeltaTime = { "PhysicsDeltaTime", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleClutchSimData, PhysicsDeltaTime), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_PhysicsDeltaTime_MetaData), Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_PhysicsDeltaTime_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_SpringStiffness_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_SpringStiffness = { "SpringStiffness", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleClutchSimData, SpringStiffness), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_SpringStiffness_MetaData), Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_SpringStiffness_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_SpringDamping_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_SpringDamping = { "SpringDamping", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleClutchSimData, SpringDamping), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_SpringDamping_MetaData), Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_SpringDamping_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_CriticalDamping_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_CriticalDamping = { "CriticalDamping", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleClutchSimData, CriticalDamping), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_CriticalDamping_MetaData), Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_CriticalDamping_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_MaxNaturalFrequency_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_MaxNaturalFrequency = { "MaxNaturalFrequency", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleClutchSimData, MaxNaturalFrequency), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_MaxNaturalFrequency_MetaData), Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_MaxNaturalFrequency_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_EngineInertia_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_EngineInertia = { "EngineInertia", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleClutchSimData, EngineInertia), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_EngineInertia_MetaData), Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_EngineInertia_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_EngineAngularVelocity_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_EngineAngularVelocity = { "EngineAngularVelocity", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleClutchSimData, EngineAngularVelocity), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_EngineAngularVelocity_MetaData), Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_EngineAngularVelocity_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchAngularVelocity_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchAngularVelocity = { "ClutchAngularVelocity", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleClutchSimData, ClutchAngularVelocity), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchAngularVelocity_MetaData), Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchAngularVelocity_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchSlip_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchSlip = { "ClutchSlip", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleClutchSimData, ClutchSlip), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchSlip_MetaData), Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchSlip_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_AngleDiff_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_AngleDiff = { "AngleDiff", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleClutchSimData, AngleDiff), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_AngleDiff_MetaData), Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_AngleDiff_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchLock_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchLock = { "ClutchLock", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleClutchSimData, ClutchLock), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchLock_MetaData), Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchLock_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_MaxClutchTorque_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_MaxClutchTorque = { "MaxClutchTorque", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleClutchSimData, MaxClutchTorque), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_MaxClutchTorque_MetaData), Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_MaxClutchTorque_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_SmoothenedTorque_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_SmoothenedTorque = { "SmoothenedTorque", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleClutchSimData, SmoothenedTorque), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_SmoothenedTorque_MetaData), Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_SmoothenedTorque_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchTorque_MetaData[] = {
		{ "Category", "Force" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchTorque = { "ClutchTorque", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleClutchSimData, ClutchTorque), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchTorque_MetaData), Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchTorque_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_TotalInertia_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_TotalInertia = { "TotalInertia", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleClutchSimData, TotalInertia), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_TotalInertia_MetaData), Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_TotalInertia_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ReflectedInertia_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ReflectedInertia = { "ReflectedInertia", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleClutchSimData, ReflectedInertia), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ReflectedInertia_MetaData), Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ReflectedInertia_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_FirstGearReflectedInertia_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_FirstGearReflectedInertia = { "FirstGearReflectedInertia", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleClutchSimData, FirstGearReflectedInertia), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_FirstGearReflectedInertia_MetaData), Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_FirstGearReflectedInertia_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchSlipping_MetaData[] = {
		{ "Category", "Movement" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchSlipping_SetBit(void* Obj)
	{
		((FVehicleClutchSimData*)Obj)->ClutchSlipping = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchSlipping = { "ClutchSlipping", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FVehicleClutchSimData), &Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchSlipping_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchSlipping_MetaData), Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchSlipping_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_PhysicsDeltaTime,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_SpringStiffness,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_SpringDamping,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_CriticalDamping,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_MaxNaturalFrequency,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_EngineInertia,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_EngineAngularVelocity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchAngularVelocity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchSlip,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_AngleDiff,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchLock,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_MaxClutchTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_SmoothenedTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_TotalInertia,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ReflectedInertia,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_FirstGearReflectedInertia,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewProp_ClutchSlipping,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		&NewStructOps,
		"VehicleClutchSimData",
		Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::PropPointers),
		sizeof(FVehicleClutchSimData),
		alignof(FVehicleClutchSimData),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FVehicleClutchSimData()
	{
		if (!Z_Registration_Info_UScriptStruct_VehicleClutchSimData.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_VehicleClutchSimData.InnerSingleton, Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_VehicleClutchSimData.InnerSingleton;
	}
	DEFINE_FUNCTION(UVehicleClutchComponent::execGetClutchMovement)
	{
		P_GET_STRUCT_REF(FVehicleClutchSimData,Z_Param_Out_Out);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->GetClutchMovement(Z_Param_Out_Out);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleClutchComponent::execGetCluchTorque)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=P_THIS->GetCluchTorque();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleClutchComponent::execUpdatePhysics)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_InDeltaTime);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InClutchValue);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InGearboxInputShaftVelocity);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InReflectedInertia);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InCurrentGearRatio);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InFirstGearReflectedInertia);
		P_GET_OBJECT(UVehicleEngineComponent,Z_Param_TargetEngine);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->UpdatePhysics(Z_Param_InDeltaTime,Z_Param_InClutchValue,Z_Param_InGearboxInputShaftVelocity,Z_Param_InReflectedInertia,Z_Param_InCurrentGearRatio,Z_Param_InFirstGearReflectedInertia,Z_Param_TargetEngine);
		P_NATIVE_END;
	}
	void UVehicleClutchComponent::StaticRegisterNativesUVehicleClutchComponent()
	{
		UClass* Class = UVehicleClutchComponent::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "GetCluchTorque", &UVehicleClutchComponent::execGetCluchTorque },
			{ "GetClutchMovement", &UVehicleClutchComponent::execGetClutchMovement },
			{ "UpdatePhysics", &UVehicleClutchComponent::execUpdatePhysics },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UVehicleClutchComponent_GetCluchTorque_Statics
	{
		struct VehicleClutchComponent_eventGetCluchTorque_Parms
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
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleClutchComponent_GetCluchTorque_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleClutchComponent_eventGetCluchTorque_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleClutchComponent_GetCluchTorque_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleClutchComponent_GetCluchTorque_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleClutchComponent_GetCluchTorque_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleClutchComponent_GetCluchTorque_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleClutchComponent, nullptr, "GetCluchTorque", nullptr, nullptr, Z_Construct_UFunction_UVehicleClutchComponent_GetCluchTorque_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleClutchComponent_GetCluchTorque_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleClutchComponent_GetCluchTorque_Statics::VehicleClutchComponent_eventGetCluchTorque_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleClutchComponent_GetCluchTorque_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleClutchComponent_GetCluchTorque_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleClutchComponent_GetCluchTorque_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleClutchComponent_GetCluchTorque_Statics::VehicleClutchComponent_eventGetCluchTorque_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleClutchComponent_GetCluchTorque()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleClutchComponent_GetCluchTorque_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleClutchComponent_GetClutchMovement_Statics
	{
		struct VehicleClutchComponent_eventGetClutchMovement_Parms
		{
			FVehicleClutchSimData Out;
		};
		static const UECodeGen_Private::FStructPropertyParams NewProp_Out;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleClutchComponent_GetClutchMovement_Statics::NewProp_Out = { "Out", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleClutchComponent_eventGetClutchMovement_Parms, Out), Z_Construct_UScriptStruct_FVehicleClutchSimData, METADATA_PARAMS(0, nullptr) }; // 843853420
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleClutchComponent_GetClutchMovement_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleClutchComponent_GetClutchMovement_Statics::NewProp_Out,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleClutchComponent_GetClutchMovement_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleClutchComponent_GetClutchMovement_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleClutchComponent, nullptr, "GetClutchMovement", nullptr, nullptr, Z_Construct_UFunction_UVehicleClutchComponent_GetClutchMovement_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleClutchComponent_GetClutchMovement_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleClutchComponent_GetClutchMovement_Statics::VehicleClutchComponent_eventGetClutchMovement_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleClutchComponent_GetClutchMovement_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleClutchComponent_GetClutchMovement_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleClutchComponent_GetClutchMovement_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleClutchComponent_GetClutchMovement_Statics::VehicleClutchComponent_eventGetClutchMovement_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleClutchComponent_GetClutchMovement()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleClutchComponent_GetClutchMovement_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics
	{
		struct VehicleClutchComponent_eventUpdatePhysics_Parms
		{
			float InDeltaTime;
			float InClutchValue;
			float InGearboxInputShaftVelocity;
			float InReflectedInertia;
			float InCurrentGearRatio;
			float InFirstGearReflectedInertia;
			UVehicleEngineComponent* TargetEngine;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InDeltaTime;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InClutchValue;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InGearboxInputShaftVelocity;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InReflectedInertia;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InCurrentGearRatio;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InFirstGearReflectedInertia;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TargetEngine_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_TargetEngine;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::NewProp_InDeltaTime = { "InDeltaTime", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleClutchComponent_eventUpdatePhysics_Parms, InDeltaTime), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::NewProp_InClutchValue = { "InClutchValue", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleClutchComponent_eventUpdatePhysics_Parms, InClutchValue), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::NewProp_InGearboxInputShaftVelocity = { "InGearboxInputShaftVelocity", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleClutchComponent_eventUpdatePhysics_Parms, InGearboxInputShaftVelocity), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::NewProp_InReflectedInertia = { "InReflectedInertia", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleClutchComponent_eventUpdatePhysics_Parms, InReflectedInertia), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::NewProp_InCurrentGearRatio = { "InCurrentGearRatio", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleClutchComponent_eventUpdatePhysics_Parms, InCurrentGearRatio), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::NewProp_InFirstGearReflectedInertia = { "InFirstGearReflectedInertia", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleClutchComponent_eventUpdatePhysics_Parms, InFirstGearReflectedInertia), METADATA_PARAMS(0, nullptr) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::NewProp_TargetEngine_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::NewProp_TargetEngine = { "TargetEngine", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleClutchComponent_eventUpdatePhysics_Parms, TargetEngine), Z_Construct_UClass_UVehicleEngineComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::NewProp_TargetEngine_MetaData), Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::NewProp_TargetEngine_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::NewProp_InDeltaTime,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::NewProp_InClutchValue,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::NewProp_InGearboxInputShaftVelocity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::NewProp_InReflectedInertia,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::NewProp_InCurrentGearRatio,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::NewProp_InFirstGearReflectedInertia,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::NewProp_TargetEngine,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleClutchComponent, nullptr, "UpdatePhysics", nullptr, nullptr, Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::VehicleClutchComponent_eventUpdatePhysics_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::VehicleClutchComponent_eventUpdatePhysics_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UVehicleClutchComponent);
	UClass* Z_Construct_UClass_UVehicleClutchComponent_NoRegister()
	{
		return UVehicleClutchComponent::StaticClass();
	}
	struct Z_Construct_UClass_UVehicleClutchComponent_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Config_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_Config;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UVehicleClutchComponent_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UActorComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleClutchComponent_Statics::DependentSingletons) < 16);
	const FClassFunctionLinkInfo Z_Construct_UClass_UVehicleClutchComponent_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UVehicleClutchComponent_GetCluchTorque, "GetCluchTorque" }, // 1678394777
		{ &Z_Construct_UFunction_UVehicleClutchComponent_GetClutchMovement, "GetClutchMovement" }, // 4019473583
		{ &Z_Construct_UFunction_UVehicleClutchComponent_UpdatePhysics, "UpdatePhysics" }, // 2709678150
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleClutchComponent_Statics::FuncInfo) < 2048);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleClutchComponent_Statics::Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "BlueprintType", "true" },
		{ "ClassGroupNames", "Custom" },
		{ "IncludePath", "VehicleClutchComponent.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleClutchComponent_Statics::NewProp_Config_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleClutchComponent.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UVehicleClutchComponent_Statics::NewProp_Config = { "Config", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleClutchComponent, Config), Z_Construct_UScriptStruct_FVehicleClutchConfig, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleClutchComponent_Statics::NewProp_Config_MetaData), Z_Construct_UClass_UVehicleClutchComponent_Statics::NewProp_Config_MetaData) }; // 2936430875
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UVehicleClutchComponent_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleClutchComponent_Statics::NewProp_Config,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UVehicleClutchComponent_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVehicleClutchComponent>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UVehicleClutchComponent_Statics::ClassParams = {
		&UVehicleClutchComponent::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_UVehicleClutchComponent_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleClutchComponent_Statics::PropPointers),
		0,
		0x00B000A4u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleClutchComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UVehicleClutchComponent_Statics::Class_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleClutchComponent_Statics::PropPointers) < 2048);
	UClass* Z_Construct_UClass_UVehicleClutchComponent()
	{
		if (!Z_Registration_Info_UClass_UVehicleClutchComponent.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVehicleClutchComponent.OuterSingleton, Z_Construct_UClass_UVehicleClutchComponent_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UVehicleClutchComponent.OuterSingleton;
	}
	template<> KINETIFORGE_API UClass* StaticClass<UVehicleClutchComponent>()
	{
		return UVehicleClutchComponent::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UVehicleClutchComponent);
	UVehicleClutchComponent::~UVehicleClutchComponent() {}
	struct Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_Statics
	{
		static const FEnumRegisterCompiledInInfo EnumInfo[];
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FEnumRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_Statics::EnumInfo[] = {
		{ EClutchSimMode_StaticEnum, TEXT("EClutchSimMode"), &Z_Registration_Info_UEnum_EClutchSimMode, CONSTRUCT_RELOAD_VERSION_INFO(FEnumReloadVersionInfo, 385656090U) },
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_Statics::ScriptStructInfo[] = {
		{ FVehicleClutchConfig::StaticStruct, Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics::NewStructOps, TEXT("VehicleClutchConfig"), &Z_Registration_Info_UScriptStruct_VehicleClutchConfig, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FVehicleClutchConfig), 2936430875U) },
		{ FVehicleClutchSimData::StaticStruct, Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics::NewStructOps, TEXT("VehicleClutchSimData"), &Z_Registration_Info_UScriptStruct_VehicleClutchSimData, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FVehicleClutchSimData), 843853420U) },
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UVehicleClutchComponent, UVehicleClutchComponent::StaticClass, TEXT("UVehicleClutchComponent"), &Z_Registration_Info_UClass_UVehicleClutchComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVehicleClutchComponent), 1988485965U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_574780416(TEXT("/Script/KinetiForge"),
		Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_Statics::ClassInfo),
		Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_Statics::ScriptStructInfo),
		Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_Statics::EnumInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_Statics::EnumInfo));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
