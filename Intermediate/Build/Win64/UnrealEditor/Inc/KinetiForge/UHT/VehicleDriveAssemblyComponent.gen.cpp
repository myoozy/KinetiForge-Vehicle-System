// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "KinetiForge/Public/VehicleDriveAssemblyComponent.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeVehicleDriveAssemblyComponent() {}
// Cross Module References
	COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector2D();
	ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UCurveFloat_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UPrimitiveComponent_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_USceneComponent();
	ENGINE_API UClass* Z_Construct_UClass_USceneComponent_NoRegister();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleAxleAssemblyComponent_NoRegister();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleClutchComponent_NoRegister();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleDifferentialComponent_NoRegister();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleDriveAssemblyComponent();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleDriveAssemblyComponent_NoRegister();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleEngineComponent_NoRegister();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleGearboxComponent_NoRegister();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleWheelComponent_NoRegister();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleWheelCoordinatorComponent_NoRegister();
	KINETIFORGE_API UEnum* Z_Construct_UEnum_KinetiForge_EEngineState();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FAutoGearboxConfig();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FAxleAssemblyConfig();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleInputValue();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehiclInputInterpSpeed();
	UPackage* Z_Construct_UPackage__Script_KinetiForge();
// End Cross Module References
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_AxleAssemblyConfig;
class UScriptStruct* FAxleAssemblyConfig::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_AxleAssemblyConfig.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_AxleAssemblyConfig.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FAxleAssemblyConfig, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("AxleAssemblyConfig"));
	}
	return Z_Registration_Info_UScriptStruct_AxleAssemblyConfig.OuterSingleton;
}
template<> KINETIFORGE_API UScriptStruct* StaticStruct<FAxleAssemblyConfig>()
{
	return FAxleAssemblyConfig::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AxleConfig_MetaData[];
#endif
		static const UECodeGen_Private::FClassPropertyParams NewProp_AxleConfig;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AxlePosition_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_AxlePosition;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bDiasbleSteering_MetaData[];
#endif
		static void NewProp_bDiasbleSteering_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bDiasbleSteering;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bDisableHandbrake_MetaData[];
#endif
		static void NewProp_bDisableHandbrake_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bDisableHandbrake;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bDisableTractionControl_MetaData[];
#endif
		static void NewProp_bDisableTractionControl_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bDisableTractionControl;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TorqueWeightOverride_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_TorqueWeightOverride;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TrackWidthOverride_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_TrackWidthOverride;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_WheelOverride_MetaData[];
#endif
		static const UECodeGen_Private::FClassPropertyParams NewProp_WheelOverride;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_DifferentialOverride_MetaData[];
#endif
		static const UECodeGen_Private::FClassPropertyParams NewProp_DifferentialOverride;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FAxleAssemblyConfig>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_AxleConfig_MetaData[] = {
		{ "Category", "AxleAssemblyConfig" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FClassPropertyParams Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_AxleConfig = { "AxleConfig", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FAxleAssemblyConfig, AxleConfig), Z_Construct_UClass_UClass, Z_Construct_UClass_UVehicleAxleAssemblyComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_AxleConfig_MetaData), Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_AxleConfig_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_AxlePosition_MetaData[] = {
		{ "Category", "AxleAssemblyConfig" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_AxlePosition = { "AxlePosition", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FAxleAssemblyConfig, AxlePosition), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_AxlePosition_MetaData), Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_AxlePosition_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_bDiasbleSteering_MetaData[] = {
		{ "Category", "AxleAssemblyConfig" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_bDiasbleSteering_SetBit(void* Obj)
	{
		((FAxleAssemblyConfig*)Obj)->bDiasbleSteering = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_bDiasbleSteering = { "bDiasbleSteering", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FAxleAssemblyConfig), &Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_bDiasbleSteering_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_bDiasbleSteering_MetaData), Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_bDiasbleSteering_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_bDisableHandbrake_MetaData[] = {
		{ "Category", "AxleAssemblyConfig" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_bDisableHandbrake_SetBit(void* Obj)
	{
		((FAxleAssemblyConfig*)Obj)->bDisableHandbrake = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_bDisableHandbrake = { "bDisableHandbrake", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FAxleAssemblyConfig), &Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_bDisableHandbrake_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_bDisableHandbrake_MetaData), Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_bDisableHandbrake_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_bDisableTractionControl_MetaData[] = {
		{ "Category", "AxleAssemblyConfig" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_bDisableTractionControl_SetBit(void* Obj)
	{
		((FAxleAssemblyConfig*)Obj)->bDisableTractionControl = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_bDisableTractionControl = { "bDisableTractionControl", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FAxleAssemblyConfig), &Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_bDisableTractionControl_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_bDisableTractionControl_MetaData), Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_bDisableTractionControl_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_TorqueWeightOverride_MetaData[] = {
		{ "Category", "AxleAssemblyConfig" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "if < 0, won't override" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_TorqueWeightOverride = { "TorqueWeightOverride", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FAxleAssemblyConfig, TorqueWeightOverride), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_TorqueWeightOverride_MetaData), Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_TorqueWeightOverride_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_TrackWidthOverride_MetaData[] = {
		{ "Category", "AxleAssemblyConfig" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "if < 0, won't override" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_TrackWidthOverride = { "TrackWidthOverride", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FAxleAssemblyConfig, TrackWidthOverride), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_TrackWidthOverride_MetaData), Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_TrackWidthOverride_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_WheelOverride_MetaData[] = {
		{ "Category", "AxleAssemblyConfig" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FClassPropertyParams Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_WheelOverride = { "WheelOverride", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FAxleAssemblyConfig, WheelOverride), Z_Construct_UClass_UClass, Z_Construct_UClass_UVehicleWheelComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_WheelOverride_MetaData), Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_WheelOverride_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_DifferentialOverride_MetaData[] = {
		{ "Category", "AxleAssemblyConfig" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FClassPropertyParams Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_DifferentialOverride = { "DifferentialOverride", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FAxleAssemblyConfig, DifferentialOverride), Z_Construct_UClass_UClass, Z_Construct_UClass_UVehicleDifferentialComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_DifferentialOverride_MetaData), Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_DifferentialOverride_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_AxleConfig,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_AxlePosition,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_bDiasbleSteering,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_bDisableHandbrake,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_bDisableTractionControl,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_TorqueWeightOverride,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_TrackWidthOverride,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_WheelOverride,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewProp_DifferentialOverride,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		&NewStructOps,
		"AxleAssemblyConfig",
		Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::PropPointers),
		sizeof(FAxleAssemblyConfig),
		alignof(FAxleAssemblyConfig),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FAxleAssemblyConfig()
	{
		if (!Z_Registration_Info_UScriptStruct_AxleAssemblyConfig.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_AxleAssemblyConfig.InnerSingleton, Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_AxleAssemblyConfig.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_VehiclInputInterpSpeed;
class UScriptStruct* FVehiclInputInterpSpeed::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_VehiclInputInterpSpeed.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_VehiclInputInterpSpeed.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FVehiclInputInterpSpeed, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("VehiclInputInterpSpeed"));
	}
	return Z_Registration_Info_UScriptStruct_VehiclInputInterpSpeed.OuterSingleton;
}
template<> KINETIFORGE_API UScriptStruct* StaticStruct<FVehiclInputInterpSpeed>()
{
	return FVehiclInputInterpSpeed::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ThrottleInterpSpeed_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_ThrottleInterpSpeed;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ThrottleCurve_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_ThrottleCurve;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BrakeInterpSpeed_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_BrakeInterpSpeed;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BrakeCurve_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_BrakeCurve;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ClutchInterpSpeed_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_ClutchInterpSpeed;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ClutchCurve_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_ClutchCurve;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_HandbrakeInterpSpeed_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_HandbrakeInterpSpeed;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_HandbrakeCurve_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_HandbrakeCurve;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SteeringInterpSpeed_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_SteeringInterpSpeed;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SteeringCurve_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_SteeringCurve;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_HighSpeedSteeringScale_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_HighSpeedSteeringScale;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FVehiclInputInterpSpeed>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_ThrottleInterpSpeed_MetaData[] = {
		{ "Category", "VehiclInputInterpSpeed" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_ThrottleInterpSpeed = { "ThrottleInterpSpeed", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehiclInputInterpSpeed, ThrottleInterpSpeed), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_ThrottleInterpSpeed_MetaData), Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_ThrottleInterpSpeed_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_ThrottleCurve_MetaData[] = {
		{ "Category", "VehiclInputInterpSpeed" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_ThrottleCurve = { "ThrottleCurve", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehiclInputInterpSpeed, ThrottleCurve), Z_Construct_UClass_UCurveFloat_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_ThrottleCurve_MetaData), Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_ThrottleCurve_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_BrakeInterpSpeed_MetaData[] = {
		{ "Category", "VehiclInputInterpSpeed" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_BrakeInterpSpeed = { "BrakeInterpSpeed", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehiclInputInterpSpeed, BrakeInterpSpeed), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_BrakeInterpSpeed_MetaData), Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_BrakeInterpSpeed_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_BrakeCurve_MetaData[] = {
		{ "Category", "VehiclInputInterpSpeed" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_BrakeCurve = { "BrakeCurve", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehiclInputInterpSpeed, BrakeCurve), Z_Construct_UClass_UCurveFloat_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_BrakeCurve_MetaData), Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_BrakeCurve_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_ClutchInterpSpeed_MetaData[] = {
		{ "Category", "VehiclInputInterpSpeed" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_ClutchInterpSpeed = { "ClutchInterpSpeed", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehiclInputInterpSpeed, ClutchInterpSpeed), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_ClutchInterpSpeed_MetaData), Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_ClutchInterpSpeed_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_ClutchCurve_MetaData[] = {
		{ "Category", "VehiclInputInterpSpeed" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_ClutchCurve = { "ClutchCurve", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehiclInputInterpSpeed, ClutchCurve), Z_Construct_UClass_UCurveFloat_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_ClutchCurve_MetaData), Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_ClutchCurve_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_HandbrakeInterpSpeed_MetaData[] = {
		{ "Category", "VehiclInputInterpSpeed" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_HandbrakeInterpSpeed = { "HandbrakeInterpSpeed", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehiclInputInterpSpeed, HandbrakeInterpSpeed), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_HandbrakeInterpSpeed_MetaData), Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_HandbrakeInterpSpeed_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_HandbrakeCurve_MetaData[] = {
		{ "Category", "VehiclInputInterpSpeed" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_HandbrakeCurve = { "HandbrakeCurve", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehiclInputInterpSpeed, HandbrakeCurve), Z_Construct_UClass_UCurveFloat_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_HandbrakeCurve_MetaData), Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_HandbrakeCurve_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_SteeringInterpSpeed_MetaData[] = {
		{ "Category", "VehiclInputInterpSpeed" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_SteeringInterpSpeed = { "SteeringInterpSpeed", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehiclInputInterpSpeed, SteeringInterpSpeed), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_SteeringInterpSpeed_MetaData), Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_SteeringInterpSpeed_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_SteeringCurve_MetaData[] = {
		{ "Category", "VehiclInputInterpSpeed" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_SteeringCurve = { "SteeringCurve", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehiclInputInterpSpeed, SteeringCurve), Z_Construct_UClass_UCurveFloat_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_SteeringCurve_MetaData), Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_SteeringCurve_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_HighSpeedSteeringScale_MetaData[] = {
		{ "Category", "VehiclInputInterpSpeed" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_HighSpeedSteeringScale = { "HighSpeedSteeringScale", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehiclInputInterpSpeed, HighSpeedSteeringScale), Z_Construct_UClass_UCurveFloat_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_HighSpeedSteeringScale_MetaData), Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_HighSpeedSteeringScale_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_ThrottleInterpSpeed,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_ThrottleCurve,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_BrakeInterpSpeed,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_BrakeCurve,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_ClutchInterpSpeed,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_ClutchCurve,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_HandbrakeInterpSpeed,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_HandbrakeCurve,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_SteeringInterpSpeed,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_SteeringCurve,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewProp_HighSpeedSteeringScale,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		&NewStructOps,
		"VehiclInputInterpSpeed",
		Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::PropPointers),
		sizeof(FVehiclInputInterpSpeed),
		alignof(FVehiclInputInterpSpeed),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FVehiclInputInterpSpeed()
	{
		if (!Z_Registration_Info_UScriptStruct_VehiclInputInterpSpeed.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_VehiclInputInterpSpeed.InnerSingleton, Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_VehiclInputInterpSpeed.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_AutoGearboxConfig;
class UScriptStruct* FAutoGearboxConfig::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_AutoGearboxConfig.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_AutoGearboxConfig.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FAutoGearboxConfig, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("AutoGearboxConfig"));
	}
	return Z_Registration_Info_UScriptStruct_AutoGearboxConfig.OuterSingleton;
}
template<> KINETIFORGE_API UScriptStruct* StaticStruct<FAutoGearboxConfig>()
{
	return FAutoGearboxConfig::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bAutomaticClutch_MetaData[];
#endif
		static void NewProp_bAutomaticClutch_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bAutomaticClutch;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bEVClutchLogic_MetaData[];
#endif
		static void NewProp_bEVClutchLogic_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bEVClutchLogic;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AutoClutchRange_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_AutoClutchRange;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RevMatchMaxThrottle_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_RevMatchMaxThrottle;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bAutomaticGearbox_MetaData[];
#endif
		static void NewProp_bAutomaticGearbox_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bAutomaticGearbox;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bArcadeAutoGearbox_MetaData[];
#endif
		static void NewProp_bArcadeAutoGearbox_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bArcadeAutoGearbox;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bArcadeShiftInstant_MetaData[];
#endif
		static void NewProp_bArcadeShiftInstant_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bArcadeShiftInstant;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bSportMode_MetaData[];
#endif
		static void NewProp_bSportMode_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bSportMode;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AutomaticGearboxRefreshTime_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_AutomaticGearboxRefreshTime;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AutoShiftCoolDown_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_AutoShiftCoolDown;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MaxUpShiftSteps_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_MaxUpShiftSteps;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MaxDownShiftSteps_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_MaxDownShiftSteps;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AutoGearboxShiftFactor_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_AutoGearboxShiftFactor;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AutoGearboxShiftFactorCurve_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_AutoGearboxShiftFactorCurve;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bAutoHold_MetaData[];
#endif
		static void NewProp_bAutoHold_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bAutoHold;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bBurnOutAssist_MetaData[];
#endif
		static void NewProp_bBurnOutAssist_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bBurnOutAssist;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FAutoGearboxConfig>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bAutomaticClutch_MetaData[] = {
		{ "Category", "AutoGearboxConfig" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "engage clutch when eg. changing gear or low rpm" },
#endif
	};
#endif
	void Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bAutomaticClutch_SetBit(void* Obj)
	{
		((FAutoGearboxConfig*)Obj)->bAutomaticClutch = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bAutomaticClutch = { "bAutomaticClutch", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FAutoGearboxConfig), &Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bAutomaticClutch_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bAutomaticClutch_MetaData), Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bAutomaticClutch_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bEVClutchLogic_MetaData[] = {
		{ "Category", "AutoGearboxConfig" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "disable AutomaticClutch, and disable throttle when in N gear" },
#endif
	};
#endif
	void Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bEVClutchLogic_SetBit(void* Obj)
	{
		((FAutoGearboxConfig*)Obj)->bEVClutchLogic = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bEVClutchLogic = { "bEVClutchLogic", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FAutoGearboxConfig), &Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bEVClutchLogic_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bEVClutchLogic_MetaData), Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bEVClutchLogic_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_AutoClutchRange_MetaData[] = {
		{ "Category", "AutoGearboxConfig" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "at which rpm the clutch should be (gradually) released" },
#endif
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_AutoClutchRange = { "AutoClutchRange", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FAutoGearboxConfig, AutoClutchRange), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_AutoClutchRange_MetaData), Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_AutoClutchRange_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_RevMatchMaxThrottle_MetaData[] = {
		{ "Category", "AutoGearboxConfig" },
		{ "ClampMax", "1.0" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_RevMatchMaxThrottle = { "RevMatchMaxThrottle", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FAutoGearboxConfig, RevMatchMaxThrottle), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_RevMatchMaxThrottle_MetaData), Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_RevMatchMaxThrottle_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bAutomaticGearbox_MetaData[] = {
		{ "Category", "AutoGearboxConfig" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "this simulates the logic of a real automatic gearbox. depending on the vehicle speed and the throttle/brake input. this is actually a AMT gearbox." },
#endif
	};
#endif
	void Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bAutomaticGearbox_SetBit(void* Obj)
	{
		((FAutoGearboxConfig*)Obj)->bAutomaticGearbox = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bAutomaticGearbox = { "bAutomaticGearbox", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FAutoGearboxConfig), &Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bAutomaticGearbox_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bAutomaticGearbox_MetaData), Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bAutomaticGearbox_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bArcadeAutoGearbox_MetaData[] = {
		{ "Category", "AutoGearboxConfig" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "this helps shifting from D to R / R to D / N to D / D to N / N to R / R to N (automatic gearboxes in reallife will never do this)" },
#endif
	};
#endif
	void Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bArcadeAutoGearbox_SetBit(void* Obj)
	{
		((FAutoGearboxConfig*)Obj)->bArcadeAutoGearbox = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bArcadeAutoGearbox = { "bArcadeAutoGearbox", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FAutoGearboxConfig), &Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bArcadeAutoGearbox_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bArcadeAutoGearbox_MetaData), Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bArcadeAutoGearbox_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bArcadeShiftInstant_MetaData[] = {
		{ "Category", "AutoGearboxConfig" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "if true, the arcade gearbox will be updated without any delay. eg. shift from D to N immediately, then in the next frame, shift to D again immediately." },
#endif
	};
#endif
	void Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bArcadeShiftInstant_SetBit(void* Obj)
	{
		((FAutoGearboxConfig*)Obj)->bArcadeShiftInstant = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bArcadeShiftInstant = { "bArcadeShiftInstant", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FAutoGearboxConfig), &Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bArcadeShiftInstant_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bArcadeShiftInstant_MetaData), Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bArcadeShiftInstant_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bSportMode_MetaData[] = {
		{ "Category", "AutoGearboxConfig" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "if using sport mode, the auto gearbox will shift up as late as possible and shift down as quick as possible. Also, automatic rev-matching." },
#endif
	};
#endif
	void Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bSportMode_SetBit(void* Obj)
	{
		((FAutoGearboxConfig*)Obj)->bSportMode = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bSportMode = { "bSportMode", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FAutoGearboxConfig), &Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bSportMode_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bSportMode_MetaData), Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bSportMode_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_AutomaticGearboxRefreshTime_MetaData[] = {
		{ "Category", "AutoGearboxConfig" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "this roughly decides the refershrate of the auto gearbox. eg. if AutoGearboxRefreshTime = 0.5 seconds, the auto gearbox will be refreshed 2 times per second." },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_AutomaticGearboxRefreshTime = { "AutomaticGearboxRefreshTime", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FAutoGearboxConfig, AutomaticGearboxRefreshTime), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_AutomaticGearboxRefreshTime_MetaData), Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_AutomaticGearboxRefreshTime_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_AutoShiftCoolDown_MetaData[] = {
		{ "Category", "AutoGearboxConfig" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "this roughly decides how long the auto gearbox needs to cool down after one shift." },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_AutoShiftCoolDown = { "AutoShiftCoolDown", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FAutoGearboxConfig, AutoShiftCoolDown), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_AutoShiftCoolDown_MetaData), Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_AutoShiftCoolDown_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_MaxUpShiftSteps_MetaData[] = {
		{ "Category", "AutoGearboxConfig" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_MaxUpShiftSteps = { "MaxUpShiftSteps", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FAutoGearboxConfig, MaxUpShiftSteps), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_MaxUpShiftSteps_MetaData), Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_MaxUpShiftSteps_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_MaxDownShiftSteps_MetaData[] = {
		{ "Category", "AutoGearboxConfig" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_MaxDownShiftSteps = { "MaxDownShiftSteps", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FAutoGearboxConfig, MaxDownShiftSteps), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_MaxDownShiftSteps_MetaData), Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_MaxDownShiftSteps_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_AutoGearboxShiftFactor_MetaData[] = {
		{ "Category", "AutoGearboxConfig" },
		{ "ClampMax", "0.99" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "this decides how aggressive the auto gearbox could be. larger number makes the auto gearbox more aggressive." },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_AutoGearboxShiftFactor = { "AutoGearboxShiftFactor", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FAutoGearboxConfig, AutoGearboxShiftFactor), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_AutoGearboxShiftFactor_MetaData), Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_AutoGearboxShiftFactor_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_AutoGearboxShiftFactorCurve_MetaData[] = {
		{ "Category", "AutoGearboxConfig" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "this also decides how aggressive the auto gearbox is. Normally, larger throttle input causes more aggressive auto gearbox. But if the curve is set, the throttle input will be mapped to this curve." },
#endif
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_AutoGearboxShiftFactorCurve = { "AutoGearboxShiftFactorCurve", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FAutoGearboxConfig, AutoGearboxShiftFactorCurve), Z_Construct_UClass_UCurveFloat_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_AutoGearboxShiftFactorCurve_MetaData), Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_AutoGearboxShiftFactorCurve_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bAutoHold_MetaData[] = {
		{ "Category", "AutoGearboxConfig" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bAutoHold_SetBit(void* Obj)
	{
		((FAutoGearboxConfig*)Obj)->bAutoHold = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bAutoHold = { "bAutoHold", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FAutoGearboxConfig), &Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bAutoHold_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bAutoHold_MetaData), Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bAutoHold_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bBurnOutAssist_MetaData[] = {
		{ "Category", "AutoGearboxConfig" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Releases the brake of the axle if the torque weight(normalized) is > 0.5" },
#endif
	};
#endif
	void Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bBurnOutAssist_SetBit(void* Obj)
	{
		((FAutoGearboxConfig*)Obj)->bBurnOutAssist = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bBurnOutAssist = { "bBurnOutAssist", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FAutoGearboxConfig), &Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bBurnOutAssist_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bBurnOutAssist_MetaData), Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bBurnOutAssist_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bAutomaticClutch,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bEVClutchLogic,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_AutoClutchRange,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_RevMatchMaxThrottle,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bAutomaticGearbox,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bArcadeAutoGearbox,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bArcadeShiftInstant,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bSportMode,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_AutomaticGearboxRefreshTime,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_AutoShiftCoolDown,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_MaxUpShiftSteps,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_MaxDownShiftSteps,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_AutoGearboxShiftFactor,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_AutoGearboxShiftFactorCurve,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bAutoHold,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewProp_bBurnOutAssist,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		&NewStructOps,
		"AutoGearboxConfig",
		Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::PropPointers),
		sizeof(FAutoGearboxConfig),
		alignof(FAutoGearboxConfig),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FAutoGearboxConfig()
	{
		if (!Z_Registration_Info_UScriptStruct_AutoGearboxConfig.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_AutoGearboxConfig.InnerSingleton, Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_AutoGearboxConfig.InnerSingleton;
	}
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_VehicleInputValue;
class UScriptStruct* FVehicleInputValue::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_VehicleInputValue.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_VehicleInputValue.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FVehicleInputValue, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("VehicleInputValue"));
	}
	return Z_Registration_Info_UScriptStruct_VehicleInputValue.OuterSingleton;
}
template<> KINETIFORGE_API UScriptStruct* StaticStruct<FVehicleInputValue>()
{
	return FVehicleInputValue::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FVehicleInputValue_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ThrottleValue_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ThrottleValue;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BrakeValue_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_BrakeValue;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ClutchValue_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ClutchValue;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SteeringValue_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_SteeringValue;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_HandbrakeValue_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_HandbrakeValue;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ThrottleInput_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ThrottleInput;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BrakeInput_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_BrakeInput;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ClutchInput_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ClutchInput;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SteeringInput_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_SteeringInput;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_HandbrakeInput_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_HandbrakeInput;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RealThrottleValue_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_RealThrottleValue;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RealBrakeValue_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_RealBrakeValue;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RealClutchValue_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_RealClutchValue;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RealSteeringValue_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_RealSteeringValue;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RealHandbrakeValue_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_RealHandbrakeValue;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bSwitchThrottleAndBrake_MetaData[];
#endif
		static void NewProp_bSwitchThrottleAndBrake_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bSwitchThrottleAndBrake;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleInputValue_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FVehicleInputValue>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_ThrottleValue_MetaData[] = {
		{ "Category", "VehicleInputValue" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//input\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "input" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_ThrottleValue = { "ThrottleValue", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleInputValue, ThrottleValue), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_ThrottleValue_MetaData), Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_ThrottleValue_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_BrakeValue_MetaData[] = {
		{ "Category", "VehicleInputValue" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_BrakeValue = { "BrakeValue", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleInputValue, BrakeValue), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_BrakeValue_MetaData), Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_BrakeValue_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_ClutchValue_MetaData[] = {
		{ "Category", "VehicleInputValue" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_ClutchValue = { "ClutchValue", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleInputValue, ClutchValue), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_ClutchValue_MetaData), Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_ClutchValue_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_SteeringValue_MetaData[] = {
		{ "Category", "VehicleInputValue" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_SteeringValue = { "SteeringValue", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleInputValue, SteeringValue), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_SteeringValue_MetaData), Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_SteeringValue_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_HandbrakeValue_MetaData[] = {
		{ "Category", "VehicleInputValue" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_HandbrakeValue = { "HandbrakeValue", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleInputValue, HandbrakeValue), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_HandbrakeValue_MetaData), Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_HandbrakeValue_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_ThrottleInput_MetaData[] = {
		{ "Category", "VehicleInputValue" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//player input\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "player input" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_ThrottleInput = { "ThrottleInput", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleInputValue, ThrottleInput), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_ThrottleInput_MetaData), Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_ThrottleInput_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_BrakeInput_MetaData[] = {
		{ "Category", "VehicleInputValue" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_BrakeInput = { "BrakeInput", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleInputValue, BrakeInput), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_BrakeInput_MetaData), Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_BrakeInput_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_ClutchInput_MetaData[] = {
		{ "Category", "VehicleInputValue" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_ClutchInput = { "ClutchInput", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleInputValue, ClutchInput), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_ClutchInput_MetaData), Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_ClutchInput_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_SteeringInput_MetaData[] = {
		{ "Category", "VehicleInputValue" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_SteeringInput = { "SteeringInput", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleInputValue, SteeringInput), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_SteeringInput_MetaData), Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_SteeringInput_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_HandbrakeInput_MetaData[] = {
		{ "Category", "VehicleInputValue" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_HandbrakeInput = { "HandbrakeInput", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleInputValue, HandbrakeInput), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_HandbrakeInput_MetaData), Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_HandbrakeInput_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_RealThrottleValue_MetaData[] = {
		{ "Category", "VehicleInputValue" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//real input\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "real input" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_RealThrottleValue = { "RealThrottleValue", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleInputValue, RealThrottleValue), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_RealThrottleValue_MetaData), Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_RealThrottleValue_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_RealBrakeValue_MetaData[] = {
		{ "Category", "VehicleInputValue" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_RealBrakeValue = { "RealBrakeValue", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleInputValue, RealBrakeValue), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_RealBrakeValue_MetaData), Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_RealBrakeValue_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_RealClutchValue_MetaData[] = {
		{ "Category", "VehicleInputValue" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_RealClutchValue = { "RealClutchValue", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleInputValue, RealClutchValue), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_RealClutchValue_MetaData), Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_RealClutchValue_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_RealSteeringValue_MetaData[] = {
		{ "Category", "VehicleInputValue" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_RealSteeringValue = { "RealSteeringValue", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleInputValue, RealSteeringValue), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_RealSteeringValue_MetaData), Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_RealSteeringValue_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_RealHandbrakeValue_MetaData[] = {
		{ "Category", "VehicleInputValue" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_RealHandbrakeValue = { "RealHandbrakeValue", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleInputValue, RealHandbrakeValue), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_RealHandbrakeValue_MetaData), Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_RealHandbrakeValue_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_bSwitchThrottleAndBrake_MetaData[] = {
		{ "Category", "VehicleInputValue" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_bSwitchThrottleAndBrake_SetBit(void* Obj)
	{
		((FVehicleInputValue*)Obj)->bSwitchThrottleAndBrake = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_bSwitchThrottleAndBrake = { "bSwitchThrottleAndBrake", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FVehicleInputValue), &Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_bSwitchThrottleAndBrake_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_bSwitchThrottleAndBrake_MetaData), Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_bSwitchThrottleAndBrake_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FVehicleInputValue_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_ThrottleValue,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_BrakeValue,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_ClutchValue,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_SteeringValue,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_HandbrakeValue,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_ThrottleInput,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_BrakeInput,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_ClutchInput,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_SteeringInput,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_HandbrakeInput,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_RealThrottleValue,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_RealBrakeValue,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_RealClutchValue,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_RealSteeringValue,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_RealHandbrakeValue,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewProp_bSwitchThrottleAndBrake,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FVehicleInputValue_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		&NewStructOps,
		"VehicleInputValue",
		Z_Construct_UScriptStruct_FVehicleInputValue_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleInputValue_Statics::PropPointers),
		sizeof(FVehicleInputValue),
		alignof(FVehicleInputValue),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleInputValue_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FVehicleInputValue_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleInputValue_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FVehicleInputValue()
	{
		if (!Z_Registration_Info_UScriptStruct_VehicleInputValue.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_VehicleInputValue.InnerSingleton, Z_Construct_UScriptStruct_FVehicleInputValue_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_VehicleInputValue.InnerSingleton;
	}
	DEFINE_FUNCTION(UVehicleDriveAssemblyComponent::execCalculateSpeedRangeOfEachGear)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(TArray<FVector2D>*)Z_Param__Result=P_THIS->CalculateSpeedRangeOfEachGear();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDriveAssemblyComponent::execGetVehicleSpeed)
	{
		P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_OutKph);
		P_GET_STRUCT_REF(FVector,Z_Param_Out_OutWorldLinearVelocity);
		P_GET_STRUCT_REF(FVector,Z_Param_Out_OutLocalLinearVelocity);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->GetVehicleSpeed(Z_Param_Out_OutKph,Z_Param_Out_OutWorldLinearVelocity,Z_Param_Out_OutLocalLinearVelocity);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDriveAssemblyComponent::execDestroyTargetAxle)
	{
		P_GET_OBJECT(UVehicleAxleAssemblyComponent,Z_Param_InTargetAxle);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->DestroyTargetAxle(Z_Param_InTargetAxle);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDriveAssemblyComponent::execGetWheels)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(TArray<UVehicleWheelComponent*>*)Z_Param__Result=P_THIS->GetWheels();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDriveAssemblyComponent::execGetGearbox)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(UVehicleGearboxComponent**)Z_Param__Result=P_THIS->GetGearbox();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDriveAssemblyComponent::execGetClutch)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(UVehicleClutchComponent**)Z_Param__Result=P_THIS->GetClutch();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDriveAssemblyComponent::execGetEngine)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(UVehicleEngineComponent**)Z_Param__Result=P_THIS->GetEngine();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDriveAssemblyComponent::execGetAxles)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(TArray<UVehicleAxleAssemblyComponent*>*)Z_Param__Result=P_THIS->GetAxles();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDriveAssemblyComponent::execGetInputValues)
	{
		P_GET_STRUCT_REF(FVehicleInputValue,Z_Param_Out_Out);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->GetInputValues(Z_Param_Out_Out);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDriveAssemblyComponent::execUpdateDriftCamera)
	{
		P_GET_OBJECT(USceneComponent,Z_Param_InSpringArm);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InPitch);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InDriftCamRate);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InDriftCamInterpSpeed);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InDriftCamStartSpeed_mps);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->UpdateDriftCamera(Z_Param_InSpringArm,Z_Param_InPitch,Z_Param_InDriftCamRate,Z_Param_InDriftCamInterpSpeed,Z_Param_InDriftCamStartSpeed_mps);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDriveAssemblyComponent::execRotateCamera)
	{
		P_GET_OBJECT(USceneComponent,Z_Param_InSpringArm);
		P_GET_STRUCT(FVector2D,Z_Param_InMouseInput);
		P_GET_UBOOL(Z_Param_bInvertYAxis);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InMaxPitch);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->RotateCamera(Z_Param_InSpringArm,Z_Param_InMouseInput,Z_Param_bInvertYAxis,Z_Param_InMaxPitch);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDriveAssemblyComponent::execInterpInputValue)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_Current);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InTarget);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InDeltaTime);
		P_GET_STRUCT(FVector2D,Z_Param_InInterpSpeed);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=P_THIS->InterpInputValue(Z_Param_Current,Z_Param_InTarget,Z_Param_InDeltaTime,Z_Param_InInterpSpeed);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDriveAssemblyComponent::execInterpInputValueConstant)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_Current);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InTarget);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InDeltaTime);
		P_GET_STRUCT(FVector2D,Z_Param_InInterpSpeed);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=P_THIS->InterpInputValueConstant(Z_Param_Current,Z_Param_InTarget,Z_Param_InDeltaTime,Z_Param_InInterpSpeed);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDriveAssemblyComponent::execShutVehicleEngine)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(EEngineState*)Z_Param__Result=P_THIS->ShutVehicleEngine();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDriveAssemblyComponent::execStartVehicleEngine)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(EEngineState*)Z_Param__Result=P_THIS->StartVehicleEngine();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDriveAssemblyComponent::execShiftDown)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_InAutoShiftCoolDown);
		P_GET_UBOOL(Z_Param_bImmediate);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->ShiftDown(Z_Param_InAutoShiftCoolDown,Z_Param_bImmediate);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDriveAssemblyComponent::execShiftUp)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_InAutoShiftCoolDown);
		P_GET_UBOOL(Z_Param_bImmediate);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->ShiftUp(Z_Param_InAutoShiftCoolDown,Z_Param_bImmediate);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDriveAssemblyComponent::execShiftToTargetGear)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_InTargetGear);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InAutoShiftCoolDown);
		P_GET_UBOOL(Z_Param_bImmediate);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->ShiftToTargetGear(Z_Param_InTargetGear,Z_Param_InAutoShiftCoolDown,Z_Param_bImmediate);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDriveAssemblyComponent::execInputHandbrake)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_InValue);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->InputHandbrake(Z_Param_InValue);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDriveAssemblyComponent::execInputSteering)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_InValue);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->InputSteering(Z_Param_InValue);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDriveAssemblyComponent::execInputClutch)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_InValue);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->InputClutch(Z_Param_InValue);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDriveAssemblyComponent::execInputBrake)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_InValue);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->InputBrake(Z_Param_InValue);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDriveAssemblyComponent::execInputThrottle)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_InValue);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->InputThrottle(Z_Param_InValue);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDriveAssemblyComponent::execUpdatePhysics)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_InDeltaTime);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->UpdatePhysics(Z_Param_InDeltaTime);
		P_NATIVE_END;
	}
	void UVehicleDriveAssemblyComponent::StaticRegisterNativesUVehicleDriveAssemblyComponent()
	{
		UClass* Class = UVehicleDriveAssemblyComponent::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "CalculateSpeedRangeOfEachGear", &UVehicleDriveAssemblyComponent::execCalculateSpeedRangeOfEachGear },
			{ "DestroyTargetAxle", &UVehicleDriveAssemblyComponent::execDestroyTargetAxle },
			{ "GetAxles", &UVehicleDriveAssemblyComponent::execGetAxles },
			{ "GetClutch", &UVehicleDriveAssemblyComponent::execGetClutch },
			{ "GetEngine", &UVehicleDriveAssemblyComponent::execGetEngine },
			{ "GetGearbox", &UVehicleDriveAssemblyComponent::execGetGearbox },
			{ "GetInputValues", &UVehicleDriveAssemblyComponent::execGetInputValues },
			{ "GetVehicleSpeed", &UVehicleDriveAssemblyComponent::execGetVehicleSpeed },
			{ "GetWheels", &UVehicleDriveAssemblyComponent::execGetWheels },
			{ "InputBrake", &UVehicleDriveAssemblyComponent::execInputBrake },
			{ "InputClutch", &UVehicleDriveAssemblyComponent::execInputClutch },
			{ "InputHandbrake", &UVehicleDriveAssemblyComponent::execInputHandbrake },
			{ "InputSteering", &UVehicleDriveAssemblyComponent::execInputSteering },
			{ "InputThrottle", &UVehicleDriveAssemblyComponent::execInputThrottle },
			{ "InterpInputValue", &UVehicleDriveAssemblyComponent::execInterpInputValue },
			{ "InterpInputValueConstant", &UVehicleDriveAssemblyComponent::execInterpInputValueConstant },
			{ "RotateCamera", &UVehicleDriveAssemblyComponent::execRotateCamera },
			{ "ShiftDown", &UVehicleDriveAssemblyComponent::execShiftDown },
			{ "ShiftToTargetGear", &UVehicleDriveAssemblyComponent::execShiftToTargetGear },
			{ "ShiftUp", &UVehicleDriveAssemblyComponent::execShiftUp },
			{ "ShutVehicleEngine", &UVehicleDriveAssemblyComponent::execShutVehicleEngine },
			{ "StartVehicleEngine", &UVehicleDriveAssemblyComponent::execStartVehicleEngine },
			{ "UpdateDriftCamera", &UVehicleDriveAssemblyComponent::execUpdateDriftCamera },
			{ "UpdatePhysics", &UVehicleDriveAssemblyComponent::execUpdatePhysics },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UVehicleDriveAssemblyComponent_CalculateSpeedRangeOfEachGear_Statics
	{
		struct VehicleDriveAssemblyComponent_eventCalculateSpeedRangeOfEachGear_Parms
		{
			TArray<FVector2D> ReturnValue;
		};
		static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue_Inner;
		static const UECodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_CalculateSpeedRangeOfEachGear_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_CalculateSpeedRangeOfEachGear_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventCalculateSpeedRangeOfEachGear_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDriveAssemblyComponent_CalculateSpeedRangeOfEachGear_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_CalculateSpeedRangeOfEachGear_Statics::NewProp_ReturnValue_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_CalculateSpeedRangeOfEachGear_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_CalculateSpeedRangeOfEachGear_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_CalculateSpeedRangeOfEachGear_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDriveAssemblyComponent, nullptr, "CalculateSpeedRangeOfEachGear", nullptr, nullptr, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_CalculateSpeedRangeOfEachGear_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_CalculateSpeedRangeOfEachGear_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_CalculateSpeedRangeOfEachGear_Statics::VehicleDriveAssemblyComponent_eventCalculateSpeedRangeOfEachGear_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_CalculateSpeedRangeOfEachGear_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_CalculateSpeedRangeOfEachGear_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_CalculateSpeedRangeOfEachGear_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_CalculateSpeedRangeOfEachGear_Statics::VehicleDriveAssemblyComponent_eventCalculateSpeedRangeOfEachGear_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDriveAssemblyComponent_CalculateSpeedRangeOfEachGear()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_CalculateSpeedRangeOfEachGear_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDriveAssemblyComponent_DestroyTargetAxle_Statics
	{
		struct VehicleDriveAssemblyComponent_eventDestroyTargetAxle_Parms
		{
			UVehicleAxleAssemblyComponent* InTargetAxle;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_InTargetAxle_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_InTargetAxle;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_DestroyTargetAxle_Statics::NewProp_InTargetAxle_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_DestroyTargetAxle_Statics::NewProp_InTargetAxle = { "InTargetAxle", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventDestroyTargetAxle_Parms, InTargetAxle), Z_Construct_UClass_UVehicleAxleAssemblyComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_DestroyTargetAxle_Statics::NewProp_InTargetAxle_MetaData), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_DestroyTargetAxle_Statics::NewProp_InTargetAxle_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDriveAssemblyComponent_DestroyTargetAxle_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_DestroyTargetAxle_Statics::NewProp_InTargetAxle,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_DestroyTargetAxle_Statics::Function_MetaDataParams[] = {
		{ "Category", "Components" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_DestroyTargetAxle_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDriveAssemblyComponent, nullptr, "DestroyTargetAxle", nullptr, nullptr, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_DestroyTargetAxle_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_DestroyTargetAxle_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_DestroyTargetAxle_Statics::VehicleDriveAssemblyComponent_eventDestroyTargetAxle_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_DestroyTargetAxle_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_DestroyTargetAxle_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_DestroyTargetAxle_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_DestroyTargetAxle_Statics::VehicleDriveAssemblyComponent_eventDestroyTargetAxle_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDriveAssemblyComponent_DestroyTargetAxle()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_DestroyTargetAxle_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetAxles_Statics
	{
		struct VehicleDriveAssemblyComponent_eventGetAxles_Parms
		{
			TArray<UVehicleAxleAssemblyComponent*> ReturnValue;
		};
		static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetAxles_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UVehicleAxleAssemblyComponent_NoRegister, METADATA_PARAMS(0, nullptr) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetAxles_Statics::NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetAxles_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010008000000588, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventGetAxles_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetAxles_Statics::NewProp_ReturnValue_MetaData), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetAxles_Statics::NewProp_ReturnValue_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetAxles_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetAxles_Statics::NewProp_ReturnValue_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetAxles_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetAxles_Statics::Function_MetaDataParams[] = {
		{ "Category", "Components" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetAxles_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDriveAssemblyComponent, nullptr, "GetAxles", nullptr, nullptr, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetAxles_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetAxles_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetAxles_Statics::VehicleDriveAssemblyComponent_eventGetAxles_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetAxles_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetAxles_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetAxles_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetAxles_Statics::VehicleDriveAssemblyComponent_eventGetAxles_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetAxles()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetAxles_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetClutch_Statics
	{
		struct VehicleDriveAssemblyComponent_eventGetClutch_Parms
		{
			UVehicleClutchComponent* ReturnValue;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetClutch_Statics::NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetClutch_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventGetClutch_Parms, ReturnValue), Z_Construct_UClass_UVehicleClutchComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetClutch_Statics::NewProp_ReturnValue_MetaData), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetClutch_Statics::NewProp_ReturnValue_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetClutch_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetClutch_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetClutch_Statics::Function_MetaDataParams[] = {
		{ "Category", "Components" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetClutch_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDriveAssemblyComponent, nullptr, "GetClutch", nullptr, nullptr, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetClutch_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetClutch_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetClutch_Statics::VehicleDriveAssemblyComponent_eventGetClutch_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetClutch_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetClutch_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetClutch_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetClutch_Statics::VehicleDriveAssemblyComponent_eventGetClutch_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetClutch()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetClutch_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetEngine_Statics
	{
		struct VehicleDriveAssemblyComponent_eventGetEngine_Parms
		{
			UVehicleEngineComponent* ReturnValue;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetEngine_Statics::NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetEngine_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventGetEngine_Parms, ReturnValue), Z_Construct_UClass_UVehicleEngineComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetEngine_Statics::NewProp_ReturnValue_MetaData), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetEngine_Statics::NewProp_ReturnValue_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetEngine_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetEngine_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetEngine_Statics::Function_MetaDataParams[] = {
		{ "Category", "Components" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetEngine_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDriveAssemblyComponent, nullptr, "GetEngine", nullptr, nullptr, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetEngine_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetEngine_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetEngine_Statics::VehicleDriveAssemblyComponent_eventGetEngine_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetEngine_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetEngine_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetEngine_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetEngine_Statics::VehicleDriveAssemblyComponent_eventGetEngine_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetEngine()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetEngine_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetGearbox_Statics
	{
		struct VehicleDriveAssemblyComponent_eventGetGearbox_Parms
		{
			UVehicleGearboxComponent* ReturnValue;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetGearbox_Statics::NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetGearbox_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventGetGearbox_Parms, ReturnValue), Z_Construct_UClass_UVehicleGearboxComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetGearbox_Statics::NewProp_ReturnValue_MetaData), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetGearbox_Statics::NewProp_ReturnValue_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetGearbox_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetGearbox_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetGearbox_Statics::Function_MetaDataParams[] = {
		{ "Category", "Components" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetGearbox_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDriveAssemblyComponent, nullptr, "GetGearbox", nullptr, nullptr, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetGearbox_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetGearbox_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetGearbox_Statics::VehicleDriveAssemblyComponent_eventGetGearbox_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetGearbox_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetGearbox_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetGearbox_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetGearbox_Statics::VehicleDriveAssemblyComponent_eventGetGearbox_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetGearbox()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetGearbox_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetInputValues_Statics
	{
		struct VehicleDriveAssemblyComponent_eventGetInputValues_Parms
		{
			FVehicleInputValue Out;
		};
		static const UECodeGen_Private::FStructPropertyParams NewProp_Out;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetInputValues_Statics::NewProp_Out = { "Out", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventGetInputValues_Parms, Out), Z_Construct_UScriptStruct_FVehicleInputValue, METADATA_PARAMS(0, nullptr) }; // 726577315
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetInputValues_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetInputValues_Statics::NewProp_Out,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetInputValues_Statics::Function_MetaDataParams[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetInputValues_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDriveAssemblyComponent, nullptr, "GetInputValues", nullptr, nullptr, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetInputValues_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetInputValues_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetInputValues_Statics::VehicleDriveAssemblyComponent_eventGetInputValues_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetInputValues_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetInputValues_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetInputValues_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetInputValues_Statics::VehicleDriveAssemblyComponent_eventGetInputValues_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetInputValues()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetInputValues_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetVehicleSpeed_Statics
	{
		struct VehicleDriveAssemblyComponent_eventGetVehicleSpeed_Parms
		{
			float OutKph;
			FVector OutWorldLinearVelocity;
			FVector OutLocalLinearVelocity;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_OutKph;
		static const UECodeGen_Private::FStructPropertyParams NewProp_OutWorldLinearVelocity;
		static const UECodeGen_Private::FStructPropertyParams NewProp_OutLocalLinearVelocity;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetVehicleSpeed_Statics::NewProp_OutKph = { "OutKph", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventGetVehicleSpeed_Parms, OutKph), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetVehicleSpeed_Statics::NewProp_OutWorldLinearVelocity = { "OutWorldLinearVelocity", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventGetVehicleSpeed_Parms, OutWorldLinearVelocity), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetVehicleSpeed_Statics::NewProp_OutLocalLinearVelocity = { "OutLocalLinearVelocity", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventGetVehicleSpeed_Parms, OutLocalLinearVelocity), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetVehicleSpeed_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetVehicleSpeed_Statics::NewProp_OutKph,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetVehicleSpeed_Statics::NewProp_OutWorldLinearVelocity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetVehicleSpeed_Statics::NewProp_OutLocalLinearVelocity,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetVehicleSpeed_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetVehicleSpeed_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDriveAssemblyComponent, nullptr, "GetVehicleSpeed", nullptr, nullptr, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetVehicleSpeed_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetVehicleSpeed_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetVehicleSpeed_Statics::VehicleDriveAssemblyComponent_eventGetVehicleSpeed_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14C20401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetVehicleSpeed_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetVehicleSpeed_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetVehicleSpeed_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetVehicleSpeed_Statics::VehicleDriveAssemblyComponent_eventGetVehicleSpeed_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetVehicleSpeed()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetVehicleSpeed_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetWheels_Statics
	{
		struct VehicleDriveAssemblyComponent_eventGetWheels_Parms
		{
			TArray<UVehicleWheelComponent*> ReturnValue;
		};
		static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetWheels_Statics::NewProp_ReturnValue_Inner = { "ReturnValue", nullptr, (EPropertyFlags)0x0000000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UVehicleWheelComponent_NoRegister, METADATA_PARAMS(0, nullptr) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetWheels_Statics::NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetWheels_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010008000000588, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventGetWheels_Parms, ReturnValue), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetWheels_Statics::NewProp_ReturnValue_MetaData), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetWheels_Statics::NewProp_ReturnValue_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetWheels_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetWheels_Statics::NewProp_ReturnValue_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetWheels_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetWheels_Statics::Function_MetaDataParams[] = {
		{ "Category", "Components" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetWheels_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDriveAssemblyComponent, nullptr, "GetWheels", nullptr, nullptr, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetWheels_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetWheels_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetWheels_Statics::VehicleDriveAssemblyComponent_eventGetWheels_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetWheels_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetWheels_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetWheels_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetWheels_Statics::VehicleDriveAssemblyComponent_eventGetWheels_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetWheels()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetWheels_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputBrake_Statics
	{
		struct VehicleDriveAssemblyComponent_eventInputBrake_Parms
		{
			float InValue;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputBrake_Statics::NewProp_InValue = { "InValue", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventInputBrake_Parms, InValue), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputBrake_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputBrake_Statics::NewProp_InValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputBrake_Statics::Function_MetaDataParams[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputBrake_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDriveAssemblyComponent, nullptr, "InputBrake", nullptr, nullptr, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputBrake_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputBrake_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputBrake_Statics::VehicleDriveAssemblyComponent_eventInputBrake_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputBrake_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputBrake_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputBrake_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputBrake_Statics::VehicleDriveAssemblyComponent_eventInputBrake_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputBrake()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputBrake_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputClutch_Statics
	{
		struct VehicleDriveAssemblyComponent_eventInputClutch_Parms
		{
			float InValue;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputClutch_Statics::NewProp_InValue = { "InValue", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventInputClutch_Parms, InValue), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputClutch_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputClutch_Statics::NewProp_InValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputClutch_Statics::Function_MetaDataParams[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputClutch_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDriveAssemblyComponent, nullptr, "InputClutch", nullptr, nullptr, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputClutch_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputClutch_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputClutch_Statics::VehicleDriveAssemblyComponent_eventInputClutch_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputClutch_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputClutch_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputClutch_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputClutch_Statics::VehicleDriveAssemblyComponent_eventInputClutch_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputClutch()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputClutch_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputHandbrake_Statics
	{
		struct VehicleDriveAssemblyComponent_eventInputHandbrake_Parms
		{
			float InValue;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputHandbrake_Statics::NewProp_InValue = { "InValue", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventInputHandbrake_Parms, InValue), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputHandbrake_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputHandbrake_Statics::NewProp_InValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputHandbrake_Statics::Function_MetaDataParams[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputHandbrake_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDriveAssemblyComponent, nullptr, "InputHandbrake", nullptr, nullptr, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputHandbrake_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputHandbrake_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputHandbrake_Statics::VehicleDriveAssemblyComponent_eventInputHandbrake_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputHandbrake_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputHandbrake_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputHandbrake_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputHandbrake_Statics::VehicleDriveAssemblyComponent_eventInputHandbrake_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputHandbrake()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputHandbrake_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputSteering_Statics
	{
		struct VehicleDriveAssemblyComponent_eventInputSteering_Parms
		{
			float InValue;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputSteering_Statics::NewProp_InValue = { "InValue", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventInputSteering_Parms, InValue), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputSteering_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputSteering_Statics::NewProp_InValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputSteering_Statics::Function_MetaDataParams[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputSteering_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDriveAssemblyComponent, nullptr, "InputSteering", nullptr, nullptr, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputSteering_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputSteering_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputSteering_Statics::VehicleDriveAssemblyComponent_eventInputSteering_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputSteering_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputSteering_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputSteering_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputSteering_Statics::VehicleDriveAssemblyComponent_eventInputSteering_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputSteering()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputSteering_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputThrottle_Statics
	{
		struct VehicleDriveAssemblyComponent_eventInputThrottle_Parms
		{
			float InValue;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputThrottle_Statics::NewProp_InValue = { "InValue", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventInputThrottle_Parms, InValue), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputThrottle_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputThrottle_Statics::NewProp_InValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputThrottle_Statics::Function_MetaDataParams[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputThrottle_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDriveAssemblyComponent, nullptr, "InputThrottle", nullptr, nullptr, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputThrottle_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputThrottle_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputThrottle_Statics::VehicleDriveAssemblyComponent_eventInputThrottle_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputThrottle_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputThrottle_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputThrottle_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputThrottle_Statics::VehicleDriveAssemblyComponent_eventInputThrottle_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputThrottle()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputThrottle_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValue_Statics
	{
		struct VehicleDriveAssemblyComponent_eventInterpInputValue_Parms
		{
			float Current;
			float InTarget;
			float InDeltaTime;
			FVector2D InInterpSpeed;
			float ReturnValue;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_Current;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InTarget;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InDeltaTime;
		static const UECodeGen_Private::FStructPropertyParams NewProp_InInterpSpeed;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValue_Statics::NewProp_Current = { "Current", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventInterpInputValue_Parms, Current), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValue_Statics::NewProp_InTarget = { "InTarget", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventInterpInputValue_Parms, InTarget), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValue_Statics::NewProp_InDeltaTime = { "InDeltaTime", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventInterpInputValue_Parms, InDeltaTime), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValue_Statics::NewProp_InInterpSpeed = { "InInterpSpeed", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventInterpInputValue_Parms, InInterpSpeed), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValue_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventInterpInputValue_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValue_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValue_Statics::NewProp_Current,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValue_Statics::NewProp_InTarget,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValue_Statics::NewProp_InDeltaTime,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValue_Statics::NewProp_InInterpSpeed,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValue_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValue_Statics::Function_MetaDataParams[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValue_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDriveAssemblyComponent, nullptr, "InterpInputValue", nullptr, nullptr, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValue_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValue_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValue_Statics::VehicleDriveAssemblyComponent_eventInterpInputValue_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValue_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValue_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValue_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValue_Statics::VehicleDriveAssemblyComponent_eventInterpInputValue_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValue()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValue_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValueConstant_Statics
	{
		struct VehicleDriveAssemblyComponent_eventInterpInputValueConstant_Parms
		{
			float Current;
			float InTarget;
			float InDeltaTime;
			FVector2D InInterpSpeed;
			float ReturnValue;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_Current;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InTarget;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InDeltaTime;
		static const UECodeGen_Private::FStructPropertyParams NewProp_InInterpSpeed;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValueConstant_Statics::NewProp_Current = { "Current", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventInterpInputValueConstant_Parms, Current), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValueConstant_Statics::NewProp_InTarget = { "InTarget", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventInterpInputValueConstant_Parms, InTarget), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValueConstant_Statics::NewProp_InDeltaTime = { "InDeltaTime", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventInterpInputValueConstant_Parms, InDeltaTime), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValueConstant_Statics::NewProp_InInterpSpeed = { "InInterpSpeed", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventInterpInputValueConstant_Parms, InInterpSpeed), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValueConstant_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventInterpInputValueConstant_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValueConstant_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValueConstant_Statics::NewProp_Current,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValueConstant_Statics::NewProp_InTarget,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValueConstant_Statics::NewProp_InDeltaTime,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValueConstant_Statics::NewProp_InInterpSpeed,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValueConstant_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValueConstant_Statics::Function_MetaDataParams[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValueConstant_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDriveAssemblyComponent, nullptr, "InterpInputValueConstant", nullptr, nullptr, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValueConstant_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValueConstant_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValueConstant_Statics::VehicleDriveAssemblyComponent_eventInterpInputValueConstant_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValueConstant_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValueConstant_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValueConstant_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValueConstant_Statics::VehicleDriveAssemblyComponent_eventInterpInputValueConstant_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValueConstant()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValueConstant_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera_Statics
	{
		struct VehicleDriveAssemblyComponent_eventRotateCamera_Parms
		{
			USceneComponent* InSpringArm;
			FVector2D InMouseInput;
			bool bInvertYAxis;
			float InMaxPitch;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_InSpringArm_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_InSpringArm;
		static const UECodeGen_Private::FStructPropertyParams NewProp_InMouseInput;
		static void NewProp_bInvertYAxis_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bInvertYAxis;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InMaxPitch;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera_Statics::NewProp_InSpringArm_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera_Statics::NewProp_InSpringArm = { "InSpringArm", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventRotateCamera_Parms, InSpringArm), Z_Construct_UClass_USceneComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera_Statics::NewProp_InSpringArm_MetaData), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera_Statics::NewProp_InSpringArm_MetaData) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera_Statics::NewProp_InMouseInput = { "InMouseInput", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventRotateCamera_Parms, InMouseInput), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(0, nullptr) };
	void Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera_Statics::NewProp_bInvertYAxis_SetBit(void* Obj)
	{
		((VehicleDriveAssemblyComponent_eventRotateCamera_Parms*)Obj)->bInvertYAxis = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera_Statics::NewProp_bInvertYAxis = { "bInvertYAxis", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VehicleDriveAssemblyComponent_eventRotateCamera_Parms), &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera_Statics::NewProp_bInvertYAxis_SetBit, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera_Statics::NewProp_InMaxPitch = { "InMaxPitch", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventRotateCamera_Parms, InMaxPitch), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera_Statics::NewProp_InSpringArm,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera_Statics::NewProp_InMouseInput,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera_Statics::NewProp_bInvertYAxis,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera_Statics::NewProp_InMaxPitch,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera_Statics::Function_MetaDataParams[] = {
		{ "Category", "Input" },
		{ "CPP_Default_bInvertYAxis", "false" },
		{ "CPP_Default_InMaxPitch", "80.000000" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDriveAssemblyComponent, nullptr, "RotateCamera", nullptr, nullptr, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera_Statics::VehicleDriveAssemblyComponent_eventRotateCamera_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera_Statics::VehicleDriveAssemblyComponent_eventRotateCamera_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftDown_Statics
	{
		struct VehicleDriveAssemblyComponent_eventShiftDown_Parms
		{
			float InAutoShiftCoolDown;
			bool bImmediate;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InAutoShiftCoolDown;
		static void NewProp_bImmediate_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bImmediate;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftDown_Statics::NewProp_InAutoShiftCoolDown = { "InAutoShiftCoolDown", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventShiftDown_Parms, InAutoShiftCoolDown), METADATA_PARAMS(0, nullptr) };
	void Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftDown_Statics::NewProp_bImmediate_SetBit(void* Obj)
	{
		((VehicleDriveAssemblyComponent_eventShiftDown_Parms*)Obj)->bImmediate = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftDown_Statics::NewProp_bImmediate = { "bImmediate", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VehicleDriveAssemblyComponent_eventShiftDown_Parms), &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftDown_Statics::NewProp_bImmediate_SetBit, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftDown_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftDown_Statics::NewProp_InAutoShiftCoolDown,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftDown_Statics::NewProp_bImmediate,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftDown_Statics::Function_MetaDataParams[] = {
		{ "Category", "Input" },
		{ "CPP_Default_bImmediate", "false" },
		{ "CPP_Default_InAutoShiftCoolDown", "5.000000" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftDown_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDriveAssemblyComponent, nullptr, "ShiftDown", nullptr, nullptr, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftDown_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftDown_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftDown_Statics::VehicleDriveAssemblyComponent_eventShiftDown_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftDown_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftDown_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftDown_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftDown_Statics::VehicleDriveAssemblyComponent_eventShiftDown_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftDown()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftDown_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftToTargetGear_Statics
	{
		struct VehicleDriveAssemblyComponent_eventShiftToTargetGear_Parms
		{
			int32 InTargetGear;
			float InAutoShiftCoolDown;
			bool bImmediate;
		};
		static const UECodeGen_Private::FIntPropertyParams NewProp_InTargetGear;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InAutoShiftCoolDown;
		static void NewProp_bImmediate_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bImmediate;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftToTargetGear_Statics::NewProp_InTargetGear = { "InTargetGear", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventShiftToTargetGear_Parms, InTargetGear), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftToTargetGear_Statics::NewProp_InAutoShiftCoolDown = { "InAutoShiftCoolDown", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventShiftToTargetGear_Parms, InAutoShiftCoolDown), METADATA_PARAMS(0, nullptr) };
	void Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftToTargetGear_Statics::NewProp_bImmediate_SetBit(void* Obj)
	{
		((VehicleDriveAssemblyComponent_eventShiftToTargetGear_Parms*)Obj)->bImmediate = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftToTargetGear_Statics::NewProp_bImmediate = { "bImmediate", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VehicleDriveAssemblyComponent_eventShiftToTargetGear_Parms), &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftToTargetGear_Statics::NewProp_bImmediate_SetBit, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftToTargetGear_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftToTargetGear_Statics::NewProp_InTargetGear,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftToTargetGear_Statics::NewProp_InAutoShiftCoolDown,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftToTargetGear_Statics::NewProp_bImmediate,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftToTargetGear_Statics::Function_MetaDataParams[] = {
		{ "Category", "Input" },
		{ "CPP_Default_bImmediate", "false" },
		{ "CPP_Default_InAutoShiftCoolDown", "5.000000" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftToTargetGear_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDriveAssemblyComponent, nullptr, "ShiftToTargetGear", nullptr, nullptr, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftToTargetGear_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftToTargetGear_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftToTargetGear_Statics::VehicleDriveAssemblyComponent_eventShiftToTargetGear_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftToTargetGear_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftToTargetGear_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftToTargetGear_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftToTargetGear_Statics::VehicleDriveAssemblyComponent_eventShiftToTargetGear_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftToTargetGear()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftToTargetGear_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftUp_Statics
	{
		struct VehicleDriveAssemblyComponent_eventShiftUp_Parms
		{
			float InAutoShiftCoolDown;
			bool bImmediate;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InAutoShiftCoolDown;
		static void NewProp_bImmediate_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bImmediate;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftUp_Statics::NewProp_InAutoShiftCoolDown = { "InAutoShiftCoolDown", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventShiftUp_Parms, InAutoShiftCoolDown), METADATA_PARAMS(0, nullptr) };
	void Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftUp_Statics::NewProp_bImmediate_SetBit(void* Obj)
	{
		((VehicleDriveAssemblyComponent_eventShiftUp_Parms*)Obj)->bImmediate = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftUp_Statics::NewProp_bImmediate = { "bImmediate", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VehicleDriveAssemblyComponent_eventShiftUp_Parms), &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftUp_Statics::NewProp_bImmediate_SetBit, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftUp_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftUp_Statics::NewProp_InAutoShiftCoolDown,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftUp_Statics::NewProp_bImmediate,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftUp_Statics::Function_MetaDataParams[] = {
		{ "Category", "Input" },
		{ "CPP_Default_bImmediate", "false" },
		{ "CPP_Default_InAutoShiftCoolDown", "5.000000" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftUp_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDriveAssemblyComponent, nullptr, "ShiftUp", nullptr, nullptr, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftUp_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftUp_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftUp_Statics::VehicleDriveAssemblyComponent_eventShiftUp_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftUp_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftUp_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftUp_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftUp_Statics::VehicleDriveAssemblyComponent_eventShiftUp_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftUp()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftUp_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShutVehicleEngine_Statics
	{
		struct VehicleDriveAssemblyComponent_eventShutVehicleEngine_Parms
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
	const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShutVehicleEngine_Statics::NewProp_ReturnValue_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShutVehicleEngine_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventShutVehicleEngine_Parms, ReturnValue), Z_Construct_UEnum_KinetiForge_EEngineState, METADATA_PARAMS(0, nullptr) }; // 529851397
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShutVehicleEngine_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShutVehicleEngine_Statics::NewProp_ReturnValue_Underlying,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShutVehicleEngine_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShutVehicleEngine_Statics::Function_MetaDataParams[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShutVehicleEngine_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDriveAssemblyComponent, nullptr, "ShutVehicleEngine", nullptr, nullptr, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShutVehicleEngine_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShutVehicleEngine_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShutVehicleEngine_Statics::VehicleDriveAssemblyComponent_eventShutVehicleEngine_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShutVehicleEngine_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShutVehicleEngine_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShutVehicleEngine_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShutVehicleEngine_Statics::VehicleDriveAssemblyComponent_eventShutVehicleEngine_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShutVehicleEngine()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShutVehicleEngine_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDriveAssemblyComponent_StartVehicleEngine_Statics
	{
		struct VehicleDriveAssemblyComponent_eventStartVehicleEngine_Parms
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
	const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_StartVehicleEngine_Statics::NewProp_ReturnValue_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, nullptr, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_StartVehicleEngine_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventStartVehicleEngine_Parms, ReturnValue), Z_Construct_UEnum_KinetiForge_EEngineState, METADATA_PARAMS(0, nullptr) }; // 529851397
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDriveAssemblyComponent_StartVehicleEngine_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_StartVehicleEngine_Statics::NewProp_ReturnValue_Underlying,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_StartVehicleEngine_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_StartVehicleEngine_Statics::Function_MetaDataParams[] = {
		{ "Category", "Input" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_StartVehicleEngine_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDriveAssemblyComponent, nullptr, "StartVehicleEngine", nullptr, nullptr, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_StartVehicleEngine_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_StartVehicleEngine_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_StartVehicleEngine_Statics::VehicleDriveAssemblyComponent_eventStartVehicleEngine_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_StartVehicleEngine_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_StartVehicleEngine_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_StartVehicleEngine_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_StartVehicleEngine_Statics::VehicleDriveAssemblyComponent_eventStartVehicleEngine_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDriveAssemblyComponent_StartVehicleEngine()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_StartVehicleEngine_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera_Statics
	{
		struct VehicleDriveAssemblyComponent_eventUpdateDriftCamera_Parms
		{
			USceneComponent* InSpringArm;
			float InPitch;
			float InDriftCamRate;
			float InDriftCamInterpSpeed;
			float InDriftCamStartSpeed_mps;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_InSpringArm_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_InSpringArm;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InPitch;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InDriftCamRate;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InDriftCamInterpSpeed;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InDriftCamStartSpeed_mps;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera_Statics::NewProp_InSpringArm_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera_Statics::NewProp_InSpringArm = { "InSpringArm", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventUpdateDriftCamera_Parms, InSpringArm), Z_Construct_UClass_USceneComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera_Statics::NewProp_InSpringArm_MetaData), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera_Statics::NewProp_InSpringArm_MetaData) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera_Statics::NewProp_InPitch = { "InPitch", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventUpdateDriftCamera_Parms, InPitch), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera_Statics::NewProp_InDriftCamRate = { "InDriftCamRate", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventUpdateDriftCamera_Parms, InDriftCamRate), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera_Statics::NewProp_InDriftCamInterpSpeed = { "InDriftCamInterpSpeed", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventUpdateDriftCamera_Parms, InDriftCamInterpSpeed), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera_Statics::NewProp_InDriftCamStartSpeed_mps = { "InDriftCamStartSpeed_mps", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventUpdateDriftCamera_Parms, InDriftCamStartSpeed_mps), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera_Statics::NewProp_InSpringArm,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera_Statics::NewProp_InPitch,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera_Statics::NewProp_InDriftCamRate,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera_Statics::NewProp_InDriftCamInterpSpeed,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera_Statics::NewProp_InDriftCamStartSpeed_mps,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera_Statics::Function_MetaDataParams[] = {
		{ "Category", "Input" },
		{ "CPP_Default_InDriftCamInterpSpeed", "5.000000" },
		{ "CPP_Default_InDriftCamRate", "1.000000" },
		{ "CPP_Default_InDriftCamStartSpeed_mps", "5.000000" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDriveAssemblyComponent, nullptr, "UpdateDriftCamera", nullptr, nullptr, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera_Statics::VehicleDriveAssemblyComponent_eventUpdateDriftCamera_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera_Statics::VehicleDriveAssemblyComponent_eventUpdateDriftCamera_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdatePhysics_Statics
	{
		struct VehicleDriveAssemblyComponent_eventUpdatePhysics_Parms
		{
			float InDeltaTime;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InDeltaTime;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdatePhysics_Statics::NewProp_InDeltaTime = { "InDeltaTime", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDriveAssemblyComponent_eventUpdatePhysics_Parms, InDeltaTime), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdatePhysics_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdatePhysics_Statics::NewProp_InDeltaTime,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdatePhysics_Statics::Function_MetaDataParams[] = {
		{ "Category", "PhysicsThreadOnly" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdatePhysics_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDriveAssemblyComponent, nullptr, "UpdatePhysics", nullptr, nullptr, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdatePhysics_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdatePhysics_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdatePhysics_Statics::VehicleDriveAssemblyComponent_eventUpdatePhysics_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdatePhysics_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdatePhysics_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdatePhysics_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdatePhysics_Statics::VehicleDriveAssemblyComponent_eventUpdatePhysics_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdatePhysics()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdatePhysics_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UVehicleDriveAssemblyComponent);
	UClass* Z_Construct_UClass_UVehicleDriveAssemblyComponent_NoRegister()
	{
		return UVehicleDriveAssemblyComponent::StaticClass();
	}
	struct Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_EngineConfig_MetaData[];
#endif
		static const UECodeGen_Private::FClassPropertyParams NewProp_EngineConfig;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ClutchConfig_MetaData[];
#endif
		static const UECodeGen_Private::FClassPropertyParams NewProp_ClutchConfig;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_GearboxConfig_MetaData[];
#endif
		static const UECodeGen_Private::FClassPropertyParams NewProp_GearboxConfig;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_CentralDiffLockRatio_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_CentralDiffLockRatio;
		static const UECodeGen_Private::FStructPropertyParams NewProp_AxleConfigs_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AxleConfigs_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_AxleConfigs;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_InputConfig_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_InputConfig;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AutoGearboxConfig_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_AutoGearboxConfig;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bUpdatePhysicsAutomatically_MetaData[];
#endif
		static void NewProp_bUpdatePhysicsAutomatically_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bUpdatePhysicsAutomatically;
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Axles_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Axles_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_Axles;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Engine_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Engine;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Clutch_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Clutch;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Gearbox_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Gearbox;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_WheelCoordinator_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_WheelCoordinator;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Carbody_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Carbody;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ParentActor_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_ParentActor;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_USceneComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::DependentSingletons) < 16);
	const FClassFunctionLinkInfo Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_CalculateSpeedRangeOfEachGear, "CalculateSpeedRangeOfEachGear" }, // 653708568
		{ &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_DestroyTargetAxle, "DestroyTargetAxle" }, // 499208193
		{ &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetAxles, "GetAxles" }, // 3862569290
		{ &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetClutch, "GetClutch" }, // 4042685352
		{ &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetEngine, "GetEngine" }, // 1775469616
		{ &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetGearbox, "GetGearbox" }, // 1849400855
		{ &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetInputValues, "GetInputValues" }, // 3147637805
		{ &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetVehicleSpeed, "GetVehicleSpeed" }, // 1579889801
		{ &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_GetWheels, "GetWheels" }, // 1755646180
		{ &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputBrake, "InputBrake" }, // 3934547672
		{ &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputClutch, "InputClutch" }, // 714288735
		{ &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputHandbrake, "InputHandbrake" }, // 3073745028
		{ &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputSteering, "InputSteering" }, // 1031481996
		{ &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InputThrottle, "InputThrottle" }, // 767637280
		{ &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValue, "InterpInputValue" }, // 1019413543
		{ &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_InterpInputValueConstant, "InterpInputValueConstant" }, // 3170796308
		{ &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_RotateCamera, "RotateCamera" }, // 46257219
		{ &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftDown, "ShiftDown" }, // 3886621155
		{ &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftToTargetGear, "ShiftToTargetGear" }, // 2063295353
		{ &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShiftUp, "ShiftUp" }, // 2184010561
		{ &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_ShutVehicleEngine, "ShutVehicleEngine" }, // 1156520105
		{ &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_StartVehicleEngine, "StartVehicleEngine" }, // 95861620
		{ &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdateDriftCamera, "UpdateDriftCamera" }, // 975778921
		{ &Z_Construct_UFunction_UVehicleDriveAssemblyComponent_UpdatePhysics, "UpdatePhysics" }, // 545694312
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::FuncInfo) < 2048);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "BlueprintType", "true" },
		{ "ClassGroupNames", "Custom" },
		{ "HideCategories", "Trigger PhysicsVolume" },
		{ "IncludePath", "VehicleDriveAssemblyComponent.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_EngineConfig_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_EngineConfig = { "EngineConfig", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleDriveAssemblyComponent, EngineConfig), Z_Construct_UClass_UClass, Z_Construct_UClass_UVehicleEngineComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_EngineConfig_MetaData), Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_EngineConfig_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_ClutchConfig_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_ClutchConfig = { "ClutchConfig", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleDriveAssemblyComponent, ClutchConfig), Z_Construct_UClass_UClass, Z_Construct_UClass_UVehicleClutchComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_ClutchConfig_MetaData), Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_ClutchConfig_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_GearboxConfig_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_GearboxConfig = { "GearboxConfig", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleDriveAssemblyComponent, GearboxConfig), Z_Construct_UClass_UClass, Z_Construct_UClass_UVehicleGearboxComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_GearboxConfig_MetaData), Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_GearboxConfig_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_CentralDiffLockRatio_MetaData[] = {
		{ "Category", "Setup" },
		{ "ClampMax", "1.0" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
		{ "UIMax", "1.0" },
		{ "UIMin", "0.0" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_CentralDiffLockRatio = { "CentralDiffLockRatio", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleDriveAssemblyComponent, CentralDiffLockRatio), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_CentralDiffLockRatio_MetaData), Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_CentralDiffLockRatio_MetaData) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_AxleConfigs_Inner = { "AxleConfigs", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UScriptStruct_FAxleAssemblyConfig, METADATA_PARAMS(0, nullptr) }; // 878949439
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_AxleConfigs_MetaData[] = {
		{ "Category", "Setup" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//range: 0 - 1\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "range: 0 - 1" },
#endif
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_AxleConfigs = { "AxleConfigs", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleDriveAssemblyComponent, AxleConfigs), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_AxleConfigs_MetaData), Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_AxleConfigs_MetaData) }; // 878949439
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_InputConfig_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_InputConfig = { "InputConfig", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleDriveAssemblyComponent, InputConfig), Z_Construct_UScriptStruct_FVehiclInputInterpSpeed, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_InputConfig_MetaData), Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_InputConfig_MetaData) }; // 998592232
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_AutoGearboxConfig_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_AutoGearboxConfig = { "AutoGearboxConfig", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleDriveAssemblyComponent, AutoGearboxConfig), Z_Construct_UScriptStruct_FAutoGearboxConfig, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_AutoGearboxConfig_MetaData), Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_AutoGearboxConfig_MetaData) }; // 2962091377
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_bUpdatePhysicsAutomatically_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	void Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_bUpdatePhysicsAutomatically_SetBit(void* Obj)
	{
		((UVehicleDriveAssemblyComponent*)Obj)->bUpdatePhysicsAutomatically = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_bUpdatePhysicsAutomatically = { "bUpdatePhysicsAutomatically", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UVehicleDriveAssemblyComponent), &Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_bUpdatePhysicsAutomatically_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_bUpdatePhysicsAutomatically_MetaData), Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_bUpdatePhysicsAutomatically_MetaData) };
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Axles_Inner = { "Axles", nullptr, (EPropertyFlags)0x0000000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UVehicleAxleAssemblyComponent_NoRegister, METADATA_PARAMS(0, nullptr) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Axles_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Axles = { "Axles", nullptr, (EPropertyFlags)0x0020088000000008, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleDriveAssemblyComponent, Axles), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Axles_MetaData), Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Axles_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Engine_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Engine = { "Engine", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleDriveAssemblyComponent, Engine), Z_Construct_UClass_UVehicleEngineComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Engine_MetaData), Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Engine_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Clutch_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Clutch = { "Clutch", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleDriveAssemblyComponent, Clutch), Z_Construct_UClass_UVehicleClutchComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Clutch_MetaData), Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Clutch_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Gearbox_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Gearbox = { "Gearbox", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleDriveAssemblyComponent, Gearbox), Z_Construct_UClass_UVehicleGearboxComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Gearbox_MetaData), Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Gearbox_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_WheelCoordinator_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_WheelCoordinator = { "WheelCoordinator", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleDriveAssemblyComponent, WheelCoordinator), Z_Construct_UClass_UVehicleWheelCoordinatorComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_WheelCoordinator_MetaData), Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_WheelCoordinator_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Carbody_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Carbody = { "Carbody", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleDriveAssemblyComponent, Carbody), Z_Construct_UClass_UPrimitiveComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Carbody_MetaData), Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Carbody_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_ParentActor_MetaData[] = {
		{ "ModuleRelativePath", "Public/VehicleDriveAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_ParentActor = { "ParentActor", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleDriveAssemblyComponent, ParentActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_ParentActor_MetaData), Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_ParentActor_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_EngineConfig,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_ClutchConfig,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_GearboxConfig,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_CentralDiffLockRatio,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_AxleConfigs_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_AxleConfigs,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_InputConfig,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_AutoGearboxConfig,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_bUpdatePhysicsAutomatically,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Axles_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Axles,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Engine,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Clutch,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Gearbox,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_WheelCoordinator,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_Carbody,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::NewProp_ParentActor,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVehicleDriveAssemblyComponent>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::ClassParams = {
		&UVehicleDriveAssemblyComponent::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::PropPointers),
		0,
		0x00B000A4u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::Class_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::PropPointers) < 2048);
	UClass* Z_Construct_UClass_UVehicleDriveAssemblyComponent()
	{
		if (!Z_Registration_Info_UClass_UVehicleDriveAssemblyComponent.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVehicleDriveAssemblyComponent.OuterSingleton, Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UVehicleDriveAssemblyComponent.OuterSingleton;
	}
	template<> KINETIFORGE_API UClass* StaticClass<UVehicleDriveAssemblyComponent>()
	{
		return UVehicleDriveAssemblyComponent::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UVehicleDriveAssemblyComponent);
	UVehicleDriveAssemblyComponent::~UVehicleDriveAssemblyComponent() {}
	struct Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_Statics
	{
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_Statics::ScriptStructInfo[] = {
		{ FAxleAssemblyConfig::StaticStruct, Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics::NewStructOps, TEXT("AxleAssemblyConfig"), &Z_Registration_Info_UScriptStruct_AxleAssemblyConfig, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FAxleAssemblyConfig), 878949439U) },
		{ FVehiclInputInterpSpeed::StaticStruct, Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics::NewStructOps, TEXT("VehiclInputInterpSpeed"), &Z_Registration_Info_UScriptStruct_VehiclInputInterpSpeed, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FVehiclInputInterpSpeed), 998592232U) },
		{ FAutoGearboxConfig::StaticStruct, Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics::NewStructOps, TEXT("AutoGearboxConfig"), &Z_Registration_Info_UScriptStruct_AutoGearboxConfig, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FAutoGearboxConfig), 2962091377U) },
		{ FVehicleInputValue::StaticStruct, Z_Construct_UScriptStruct_FVehicleInputValue_Statics::NewStructOps, TEXT("VehicleInputValue"), &Z_Registration_Info_UScriptStruct_VehicleInputValue, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FVehicleInputValue), 726577315U) },
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UVehicleDriveAssemblyComponent, UVehicleDriveAssemblyComponent::StaticClass, TEXT("UVehicleDriveAssemblyComponent"), &Z_Registration_Info_UClass_UVehicleDriveAssemblyComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVehicleDriveAssemblyComponent), 1930499560U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_1507822901(TEXT("/Script/KinetiForge"),
		Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_Statics::ClassInfo),
		Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_Statics::ScriptStructInfo),
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
