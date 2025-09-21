// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "KinetiForge/Public/VehicleGearboxComponent.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeVehicleGearboxComponent() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleGearboxComponent();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleGearboxComponent_NoRegister();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleGearboxConfig();
	UPackage* Z_Construct_UPackage__Script_KinetiForge();
// End Cross Module References
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_VehicleGearboxConfig;
class UScriptStruct* FVehicleGearboxConfig::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_VehicleGearboxConfig.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_VehicleGearboxConfig.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FVehicleGearboxConfig, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("VehicleGearboxConfig"));
	}
	return Z_Registration_Info_UScriptStruct_VehicleGearboxConfig.OuterSingleton;
}
template<> KINETIFORGE_API UScriptStruct* StaticStruct<FVehicleGearboxConfig>()
{
	return FVehicleGearboxConfig::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_InputShaftInertia_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InputShaftInertia;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ShiftDelay_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ShiftDelay;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_FirstGear_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_FirstGear;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TopGear_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_TopGear;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_GearRatioBias_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_GearRatioBias;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_NumberOfGears_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_NumberOfGears;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_NumOfReverseGears_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_NumOfReverseGears;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Efficiency_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_Efficiency;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bSequentialGearbox_MetaData[];
#endif
		static void NewProp_bSequentialGearbox_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bSequentialGearbox;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/VehicleGearboxComponent.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FVehicleGearboxConfig>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_InputShaftInertia_MetaData[] = {
		{ "Category", "Setup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleGearboxComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Please also consider the inertia of the clutch disk" },
#endif
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_InputShaftInertia = { "InputShaftInertia", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleGearboxConfig, InputShaftInertia), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_InputShaftInertia_MetaData), Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_InputShaftInertia_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_ShiftDelay_MetaData[] = {
		{ "Category", "Setup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleGearboxComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_ShiftDelay = { "ShiftDelay", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleGearboxConfig, ShiftDelay), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_ShiftDelay_MetaData), Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_ShiftDelay_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_FirstGear_MetaData[] = {
		{ "Category", "Setup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleGearboxComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_FirstGear = { "FirstGear", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleGearboxConfig, FirstGear), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_FirstGear_MetaData), Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_FirstGear_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_TopGear_MetaData[] = {
		{ "Category", "Setup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleGearboxComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_TopGear = { "TopGear", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleGearboxConfig, TopGear), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_TopGear_MetaData), Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_TopGear_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_GearRatioBias_MetaData[] = {
		{ "Category", "Setup" },
		{ "ClampMax", "1.0" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleGearboxComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_GearRatioBias = { "GearRatioBias", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleGearboxConfig, GearRatioBias), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_GearRatioBias_MetaData), Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_GearRatioBias_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_NumberOfGears_MetaData[] = {
		{ "Category", "Setup" },
		{ "ClampMin", "1.0" },
		{ "ModuleRelativePath", "Public/VehicleGearboxComponent.h" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_NumberOfGears = { "NumberOfGears", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleGearboxConfig, NumberOfGears), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_NumberOfGears_MetaData), Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_NumberOfGears_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_NumOfReverseGears_MetaData[] = {
		{ "Category", "Setup" },
		{ "ClampMin", "1.0" },
		{ "ModuleRelativePath", "Public/VehicleGearboxComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "the number of reverse gears should be smaller than non-reverse gears" },
#endif
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_NumOfReverseGears = { "NumOfReverseGears", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleGearboxConfig, NumOfReverseGears), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_NumOfReverseGears_MetaData), Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_NumOfReverseGears_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_Efficiency_MetaData[] = {
		{ "Category", "Setup" },
		{ "ClampMax", "1.0" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleGearboxComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_Efficiency = { "Efficiency", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleGearboxConfig, Efficiency), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_Efficiency_MetaData), Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_Efficiency_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_bSequentialGearbox_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleGearboxComponent.h" },
	};
#endif
	void Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_bSequentialGearbox_SetBit(void* Obj)
	{
		((FVehicleGearboxConfig*)Obj)->bSequentialGearbox = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_bSequentialGearbox = { "bSequentialGearbox", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FVehicleGearboxConfig), &Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_bSequentialGearbox_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_bSequentialGearbox_MetaData), Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_bSequentialGearbox_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_InputShaftInertia,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_ShiftDelay,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_FirstGear,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_TopGear,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_GearRatioBias,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_NumberOfGears,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_NumOfReverseGears,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_Efficiency,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewProp_bSequentialGearbox,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		&NewStructOps,
		"VehicleGearboxConfig",
		Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::PropPointers),
		sizeof(FVehicleGearboxConfig),
		alignof(FVehicleGearboxConfig),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FVehicleGearboxConfig()
	{
		if (!Z_Registration_Info_UScriptStruct_VehicleGearboxConfig.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_VehicleGearboxConfig.InnerSingleton, Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_VehicleGearboxConfig.InnerSingleton;
	}
	DEFINE_FUNCTION(UVehicleGearboxComponent::execCalculateGearRatios)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(bool*)Z_Param__Result=P_THIS->CalculateGearRatios();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleGearboxComponent::execGetP2MotorTorque)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=P_THIS->GetP2MotorTorque();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleGearboxComponent::execSetP2MotorTorque)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_NewTorque);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->SetP2MotorTorque(Z_Param_NewTorque);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleGearboxComponent::execGetIsInGear)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(bool*)Z_Param__Result=P_THIS->GetIsInGear();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleGearboxComponent::execGetCurrentGearRatio)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=P_THIS->GetCurrentGearRatio();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleGearboxComponent::execGetCurrentGear)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(int32*)Z_Param__Result=P_THIS->GetCurrentGear();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleGearboxComponent::execGetGearRatio)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_InTarget);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=P_THIS->GetGearRatio(Z_Param_InTarget);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleGearboxComponent::execUpdateInputShaft)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_InAxleVelocity);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InAxleInertia);
		P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_OutClutchVelocity);
		P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_OutReflectedInertia);
		P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_OutCurrentGearRatio);
		P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_OutFirstGearInertia);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->UpdateInputShaft(Z_Param_InAxleVelocity,Z_Param_InAxleInertia,Z_Param_Out_OutClutchVelocity,Z_Param_Out_OutReflectedInertia,Z_Param_Out_OutCurrentGearRatio,Z_Param_Out_OutFirstGearInertia);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleGearboxComponent::execUpdateOutputShaft)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_InClutchTorque);
		P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_OutTorque);
		P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_OutReflectedInertia);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->UpdateOutputShaft(Z_Param_InClutchTorque,Z_Param_Out_OutTorque,Z_Param_Out_OutReflectedInertia);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleGearboxComponent::execShiftToTargetGear)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_InTargetGear);
		P_GET_UBOOL(Z_Param_bImmediate);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->ShiftToTargetGear(Z_Param_InTargetGear,Z_Param_bImmediate);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleGearboxComponent::execFinalizeShift)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->FinalizeShift();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleGearboxComponent::execPrepareShift)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->PrepareShift();
		P_NATIVE_END;
	}
	void UVehicleGearboxComponent::StaticRegisterNativesUVehicleGearboxComponent()
	{
		UClass* Class = UVehicleGearboxComponent::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "CalculateGearRatios", &UVehicleGearboxComponent::execCalculateGearRatios },
			{ "FinalizeShift", &UVehicleGearboxComponent::execFinalizeShift },
			{ "GetCurrentGear", &UVehicleGearboxComponent::execGetCurrentGear },
			{ "GetCurrentGearRatio", &UVehicleGearboxComponent::execGetCurrentGearRatio },
			{ "GetGearRatio", &UVehicleGearboxComponent::execGetGearRatio },
			{ "GetIsInGear", &UVehicleGearboxComponent::execGetIsInGear },
			{ "GetP2MotorTorque", &UVehicleGearboxComponent::execGetP2MotorTorque },
			{ "PrepareShift", &UVehicleGearboxComponent::execPrepareShift },
			{ "SetP2MotorTorque", &UVehicleGearboxComponent::execSetP2MotorTorque },
			{ "ShiftToTargetGear", &UVehicleGearboxComponent::execShiftToTargetGear },
			{ "UpdateInputShaft", &UVehicleGearboxComponent::execUpdateInputShaft },
			{ "UpdateOutputShaft", &UVehicleGearboxComponent::execUpdateOutputShaft },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UVehicleGearboxComponent_CalculateGearRatios_Statics
	{
		struct VehicleGearboxComponent_eventCalculateGearRatios_Parms
		{
			bool ReturnValue;
		};
		static void NewProp_ReturnValue_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	void Z_Construct_UFunction_UVehicleGearboxComponent_CalculateGearRatios_Statics::NewProp_ReturnValue_SetBit(void* Obj)
	{
		((VehicleGearboxComponent_eventCalculateGearRatios_Parms*)Obj)->ReturnValue = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVehicleGearboxComponent_CalculateGearRatios_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VehicleGearboxComponent_eventCalculateGearRatios_Parms), &Z_Construct_UFunction_UVehicleGearboxComponent_CalculateGearRatios_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleGearboxComponent_CalculateGearRatios_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleGearboxComponent_CalculateGearRatios_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleGearboxComponent_CalculateGearRatios_Statics::Function_MetaDataParams[] = {
		{ "Category", "Initialize" },
		{ "ModuleRelativePath", "Public/VehicleGearboxComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleGearboxComponent_CalculateGearRatios_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleGearboxComponent, nullptr, "CalculateGearRatios", nullptr, nullptr, Z_Construct_UFunction_UVehicleGearboxComponent_CalculateGearRatios_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_CalculateGearRatios_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleGearboxComponent_CalculateGearRatios_Statics::VehicleGearboxComponent_eventCalculateGearRatios_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_CalculateGearRatios_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleGearboxComponent_CalculateGearRatios_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_CalculateGearRatios_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleGearboxComponent_CalculateGearRatios_Statics::VehicleGearboxComponent_eventCalculateGearRatios_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleGearboxComponent_CalculateGearRatios()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleGearboxComponent_CalculateGearRatios_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleGearboxComponent_FinalizeShift_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleGearboxComponent_FinalizeShift_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/VehicleGearboxComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleGearboxComponent_FinalizeShift_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleGearboxComponent, nullptr, "FinalizeShift", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_FinalizeShift_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleGearboxComponent_FinalizeShift_Statics::Function_MetaDataParams) };
	UFunction* Z_Construct_UFunction_UVehicleGearboxComponent_FinalizeShift()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleGearboxComponent_FinalizeShift_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGear_Statics
	{
		struct VehicleGearboxComponent_eventGetCurrentGear_Parms
		{
			int32 ReturnValue;
		};
		static const UECodeGen_Private::FIntPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGear_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleGearboxComponent_eventGetCurrentGear_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGear_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGear_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGear_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleGearboxComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGear_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleGearboxComponent, nullptr, "GetCurrentGear", nullptr, nullptr, Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGear_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGear_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGear_Statics::VehicleGearboxComponent_eventGetCurrentGear_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGear_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGear_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGear_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGear_Statics::VehicleGearboxComponent_eventGetCurrentGear_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGear()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGear_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGearRatio_Statics
	{
		struct VehicleGearboxComponent_eventGetCurrentGearRatio_Parms
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
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGearRatio_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleGearboxComponent_eventGetCurrentGearRatio_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGearRatio_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGearRatio_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGearRatio_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleGearboxComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGearRatio_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleGearboxComponent, nullptr, "GetCurrentGearRatio", nullptr, nullptr, Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGearRatio_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGearRatio_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGearRatio_Statics::VehicleGearboxComponent_eventGetCurrentGearRatio_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGearRatio_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGearRatio_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGearRatio_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGearRatio_Statics::VehicleGearboxComponent_eventGetCurrentGearRatio_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGearRatio()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGearRatio_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleGearboxComponent_GetGearRatio_Statics
	{
		struct VehicleGearboxComponent_eventGetGearRatio_Parms
		{
			int32 InTarget;
			float ReturnValue;
		};
		static const UECodeGen_Private::FIntPropertyParams NewProp_InTarget;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UVehicleGearboxComponent_GetGearRatio_Statics::NewProp_InTarget = { "InTarget", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleGearboxComponent_eventGetGearRatio_Parms, InTarget), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleGearboxComponent_GetGearRatio_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleGearboxComponent_eventGetGearRatio_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleGearboxComponent_GetGearRatio_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleGearboxComponent_GetGearRatio_Statics::NewProp_InTarget,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleGearboxComponent_GetGearRatio_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleGearboxComponent_GetGearRatio_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleGearboxComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleGearboxComponent_GetGearRatio_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleGearboxComponent, nullptr, "GetGearRatio", nullptr, nullptr, Z_Construct_UFunction_UVehicleGearboxComponent_GetGearRatio_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_GetGearRatio_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleGearboxComponent_GetGearRatio_Statics::VehicleGearboxComponent_eventGetGearRatio_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_GetGearRatio_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleGearboxComponent_GetGearRatio_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_GetGearRatio_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleGearboxComponent_GetGearRatio_Statics::VehicleGearboxComponent_eventGetGearRatio_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleGearboxComponent_GetGearRatio()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleGearboxComponent_GetGearRatio_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleGearboxComponent_GetIsInGear_Statics
	{
		struct VehicleGearboxComponent_eventGetIsInGear_Parms
		{
			bool ReturnValue;
		};
		static void NewProp_ReturnValue_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	void Z_Construct_UFunction_UVehicleGearboxComponent_GetIsInGear_Statics::NewProp_ReturnValue_SetBit(void* Obj)
	{
		((VehicleGearboxComponent_eventGetIsInGear_Parms*)Obj)->ReturnValue = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVehicleGearboxComponent_GetIsInGear_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VehicleGearboxComponent_eventGetIsInGear_Parms), &Z_Construct_UFunction_UVehicleGearboxComponent_GetIsInGear_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleGearboxComponent_GetIsInGear_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleGearboxComponent_GetIsInGear_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleGearboxComponent_GetIsInGear_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleGearboxComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleGearboxComponent_GetIsInGear_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleGearboxComponent, nullptr, "GetIsInGear", nullptr, nullptr, Z_Construct_UFunction_UVehicleGearboxComponent_GetIsInGear_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_GetIsInGear_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleGearboxComponent_GetIsInGear_Statics::VehicleGearboxComponent_eventGetIsInGear_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_GetIsInGear_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleGearboxComponent_GetIsInGear_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_GetIsInGear_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleGearboxComponent_GetIsInGear_Statics::VehicleGearboxComponent_eventGetIsInGear_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleGearboxComponent_GetIsInGear()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleGearboxComponent_GetIsInGear_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleGearboxComponent_GetP2MotorTorque_Statics
	{
		struct VehicleGearboxComponent_eventGetP2MotorTorque_Parms
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
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleGearboxComponent_GetP2MotorTorque_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleGearboxComponent_eventGetP2MotorTorque_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleGearboxComponent_GetP2MotorTorque_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleGearboxComponent_GetP2MotorTorque_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleGearboxComponent_GetP2MotorTorque_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleGearboxComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleGearboxComponent_GetP2MotorTorque_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleGearboxComponent, nullptr, "GetP2MotorTorque", nullptr, nullptr, Z_Construct_UFunction_UVehicleGearboxComponent_GetP2MotorTorque_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_GetP2MotorTorque_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleGearboxComponent_GetP2MotorTorque_Statics::VehicleGearboxComponent_eventGetP2MotorTorque_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_GetP2MotorTorque_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleGearboxComponent_GetP2MotorTorque_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_GetP2MotorTorque_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleGearboxComponent_GetP2MotorTorque_Statics::VehicleGearboxComponent_eventGetP2MotorTorque_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleGearboxComponent_GetP2MotorTorque()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleGearboxComponent_GetP2MotorTorque_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleGearboxComponent_PrepareShift_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleGearboxComponent_PrepareShift_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/VehicleGearboxComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleGearboxComponent_PrepareShift_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleGearboxComponent, nullptr, "PrepareShift", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00080401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_PrepareShift_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleGearboxComponent_PrepareShift_Statics::Function_MetaDataParams) };
	UFunction* Z_Construct_UFunction_UVehicleGearboxComponent_PrepareShift()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleGearboxComponent_PrepareShift_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleGearboxComponent_SetP2MotorTorque_Statics
	{
		struct VehicleGearboxComponent_eventSetP2MotorTorque_Parms
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
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleGearboxComponent_SetP2MotorTorque_Statics::NewProp_NewTorque = { "NewTorque", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleGearboxComponent_eventSetP2MotorTorque_Parms, NewTorque), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleGearboxComponent_SetP2MotorTorque_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleGearboxComponent_SetP2MotorTorque_Statics::NewProp_NewTorque,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleGearboxComponent_SetP2MotorTorque_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleGearboxComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleGearboxComponent_SetP2MotorTorque_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleGearboxComponent, nullptr, "SetP2MotorTorque", nullptr, nullptr, Z_Construct_UFunction_UVehicleGearboxComponent_SetP2MotorTorque_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_SetP2MotorTorque_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleGearboxComponent_SetP2MotorTorque_Statics::VehicleGearboxComponent_eventSetP2MotorTorque_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_SetP2MotorTorque_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleGearboxComponent_SetP2MotorTorque_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_SetP2MotorTorque_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleGearboxComponent_SetP2MotorTorque_Statics::VehicleGearboxComponent_eventSetP2MotorTorque_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleGearboxComponent_SetP2MotorTorque()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleGearboxComponent_SetP2MotorTorque_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleGearboxComponent_ShiftToTargetGear_Statics
	{
		struct VehicleGearboxComponent_eventShiftToTargetGear_Parms
		{
			int32 InTargetGear;
			bool bImmediate;
		};
		static const UECodeGen_Private::FIntPropertyParams NewProp_InTargetGear;
		static void NewProp_bImmediate_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bImmediate;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UVehicleGearboxComponent_ShiftToTargetGear_Statics::NewProp_InTargetGear = { "InTargetGear", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleGearboxComponent_eventShiftToTargetGear_Parms, InTargetGear), METADATA_PARAMS(0, nullptr) };
	void Z_Construct_UFunction_UVehicleGearboxComponent_ShiftToTargetGear_Statics::NewProp_bImmediate_SetBit(void* Obj)
	{
		((VehicleGearboxComponent_eventShiftToTargetGear_Parms*)Obj)->bImmediate = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVehicleGearboxComponent_ShiftToTargetGear_Statics::NewProp_bImmediate = { "bImmediate", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VehicleGearboxComponent_eventShiftToTargetGear_Parms), &Z_Construct_UFunction_UVehicleGearboxComponent_ShiftToTargetGear_Statics::NewProp_bImmediate_SetBit, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleGearboxComponent_ShiftToTargetGear_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleGearboxComponent_ShiftToTargetGear_Statics::NewProp_InTargetGear,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleGearboxComponent_ShiftToTargetGear_Statics::NewProp_bImmediate,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleGearboxComponent_ShiftToTargetGear_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "CPP_Default_bImmediate", "false" },
		{ "ModuleRelativePath", "Public/VehicleGearboxComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleGearboxComponent_ShiftToTargetGear_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleGearboxComponent, nullptr, "ShiftToTargetGear", nullptr, nullptr, Z_Construct_UFunction_UVehicleGearboxComponent_ShiftToTargetGear_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_ShiftToTargetGear_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleGearboxComponent_ShiftToTargetGear_Statics::VehicleGearboxComponent_eventShiftToTargetGear_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_ShiftToTargetGear_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleGearboxComponent_ShiftToTargetGear_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_ShiftToTargetGear_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleGearboxComponent_ShiftToTargetGear_Statics::VehicleGearboxComponent_eventShiftToTargetGear_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleGearboxComponent_ShiftToTargetGear()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleGearboxComponent_ShiftToTargetGear_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft_Statics
	{
		struct VehicleGearboxComponent_eventUpdateInputShaft_Parms
		{
			float InAxleVelocity;
			float InAxleInertia;
			float OutClutchVelocity;
			float OutReflectedInertia;
			float OutCurrentGearRatio;
			float OutFirstGearInertia;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InAxleVelocity;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InAxleInertia;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_OutClutchVelocity;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_OutReflectedInertia;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_OutCurrentGearRatio;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_OutFirstGearInertia;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft_Statics::NewProp_InAxleVelocity = { "InAxleVelocity", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleGearboxComponent_eventUpdateInputShaft_Parms, InAxleVelocity), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft_Statics::NewProp_InAxleInertia = { "InAxleInertia", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleGearboxComponent_eventUpdateInputShaft_Parms, InAxleInertia), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft_Statics::NewProp_OutClutchVelocity = { "OutClutchVelocity", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleGearboxComponent_eventUpdateInputShaft_Parms, OutClutchVelocity), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft_Statics::NewProp_OutReflectedInertia = { "OutReflectedInertia", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleGearboxComponent_eventUpdateInputShaft_Parms, OutReflectedInertia), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft_Statics::NewProp_OutCurrentGearRatio = { "OutCurrentGearRatio", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleGearboxComponent_eventUpdateInputShaft_Parms, OutCurrentGearRatio), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft_Statics::NewProp_OutFirstGearInertia = { "OutFirstGearInertia", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleGearboxComponent_eventUpdateInputShaft_Parms, OutFirstGearInertia), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft_Statics::NewProp_InAxleVelocity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft_Statics::NewProp_InAxleInertia,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft_Statics::NewProp_OutClutchVelocity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft_Statics::NewProp_OutReflectedInertia,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft_Statics::NewProp_OutCurrentGearRatio,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft_Statics::NewProp_OutFirstGearInertia,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleGearboxComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleGearboxComponent, nullptr, "UpdateInputShaft", nullptr, nullptr, Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft_Statics::VehicleGearboxComponent_eventUpdateInputShaft_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft_Statics::VehicleGearboxComponent_eventUpdateInputShaft_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleGearboxComponent_UpdateOutputShaft_Statics
	{
		struct VehicleGearboxComponent_eventUpdateOutputShaft_Parms
		{
			float InClutchTorque;
			float OutTorque;
			float OutReflectedInertia;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InClutchTorque;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_OutTorque;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_OutReflectedInertia;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleGearboxComponent_UpdateOutputShaft_Statics::NewProp_InClutchTorque = { "InClutchTorque", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleGearboxComponent_eventUpdateOutputShaft_Parms, InClutchTorque), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleGearboxComponent_UpdateOutputShaft_Statics::NewProp_OutTorque = { "OutTorque", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleGearboxComponent_eventUpdateOutputShaft_Parms, OutTorque), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleGearboxComponent_UpdateOutputShaft_Statics::NewProp_OutReflectedInertia = { "OutReflectedInertia", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleGearboxComponent_eventUpdateOutputShaft_Parms, OutReflectedInertia), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleGearboxComponent_UpdateOutputShaft_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleGearboxComponent_UpdateOutputShaft_Statics::NewProp_InClutchTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleGearboxComponent_UpdateOutputShaft_Statics::NewProp_OutTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleGearboxComponent_UpdateOutputShaft_Statics::NewProp_OutReflectedInertia,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleGearboxComponent_UpdateOutputShaft_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleGearboxComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleGearboxComponent_UpdateOutputShaft_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleGearboxComponent, nullptr, "UpdateOutputShaft", nullptr, nullptr, Z_Construct_UFunction_UVehicleGearboxComponent_UpdateOutputShaft_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_UpdateOutputShaft_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleGearboxComponent_UpdateOutputShaft_Statics::VehicleGearboxComponent_eventUpdateOutputShaft_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_UpdateOutputShaft_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleGearboxComponent_UpdateOutputShaft_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleGearboxComponent_UpdateOutputShaft_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleGearboxComponent_UpdateOutputShaft_Statics::VehicleGearboxComponent_eventUpdateOutputShaft_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleGearboxComponent_UpdateOutputShaft()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleGearboxComponent_UpdateOutputShaft_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UVehicleGearboxComponent);
	UClass* Z_Construct_UClass_UVehicleGearboxComponent_NoRegister()
	{
		return UVehicleGearboxComponent::StaticClass();
	}
	struct Z_Construct_UClass_UVehicleGearboxComponent_Statics
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
	UObject* (*const Z_Construct_UClass_UVehicleGearboxComponent_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UActorComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleGearboxComponent_Statics::DependentSingletons) < 16);
	const FClassFunctionLinkInfo Z_Construct_UClass_UVehicleGearboxComponent_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UVehicleGearboxComponent_CalculateGearRatios, "CalculateGearRatios" }, // 2041634728
		{ &Z_Construct_UFunction_UVehicleGearboxComponent_FinalizeShift, "FinalizeShift" }, // 836913331
		{ &Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGear, "GetCurrentGear" }, // 213801524
		{ &Z_Construct_UFunction_UVehicleGearboxComponent_GetCurrentGearRatio, "GetCurrentGearRatio" }, // 2700242948
		{ &Z_Construct_UFunction_UVehicleGearboxComponent_GetGearRatio, "GetGearRatio" }, // 3261531264
		{ &Z_Construct_UFunction_UVehicleGearboxComponent_GetIsInGear, "GetIsInGear" }, // 245562798
		{ &Z_Construct_UFunction_UVehicleGearboxComponent_GetP2MotorTorque, "GetP2MotorTorque" }, // 1996533455
		{ &Z_Construct_UFunction_UVehicleGearboxComponent_PrepareShift, "PrepareShift" }, // 1651814712
		{ &Z_Construct_UFunction_UVehicleGearboxComponent_SetP2MotorTorque, "SetP2MotorTorque" }, // 2536906582
		{ &Z_Construct_UFunction_UVehicleGearboxComponent_ShiftToTargetGear, "ShiftToTargetGear" }, // 1347353856
		{ &Z_Construct_UFunction_UVehicleGearboxComponent_UpdateInputShaft, "UpdateInputShaft" }, // 1275623568
		{ &Z_Construct_UFunction_UVehicleGearboxComponent_UpdateOutputShaft, "UpdateOutputShaft" }, // 1493287187
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleGearboxComponent_Statics::FuncInfo) < 2048);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleGearboxComponent_Statics::Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "BlueprintType", "true" },
		{ "ClassGroupNames", "Custom" },
		{ "IncludePath", "VehicleGearboxComponent.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/VehicleGearboxComponent.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleGearboxComponent_Statics::NewProp_Config_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleGearboxComponent.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UVehicleGearboxComponent_Statics::NewProp_Config = { "Config", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleGearboxComponent, Config), Z_Construct_UScriptStruct_FVehicleGearboxConfig, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleGearboxComponent_Statics::NewProp_Config_MetaData), Z_Construct_UClass_UVehicleGearboxComponent_Statics::NewProp_Config_MetaData) }; // 58634830
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UVehicleGearboxComponent_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleGearboxComponent_Statics::NewProp_Config,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UVehicleGearboxComponent_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVehicleGearboxComponent>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UVehicleGearboxComponent_Statics::ClassParams = {
		&UVehicleGearboxComponent::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_UVehicleGearboxComponent_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleGearboxComponent_Statics::PropPointers),
		0,
		0x00B000A4u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleGearboxComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UVehicleGearboxComponent_Statics::Class_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleGearboxComponent_Statics::PropPointers) < 2048);
	UClass* Z_Construct_UClass_UVehicleGearboxComponent()
	{
		if (!Z_Registration_Info_UClass_UVehicleGearboxComponent.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVehicleGearboxComponent.OuterSingleton, Z_Construct_UClass_UVehicleGearboxComponent_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UVehicleGearboxComponent.OuterSingleton;
	}
	template<> KINETIFORGE_API UClass* StaticClass<UVehicleGearboxComponent>()
	{
		return UVehicleGearboxComponent::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UVehicleGearboxComponent);
	UVehicleGearboxComponent::~UVehicleGearboxComponent() {}
	struct Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h_Statics
	{
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h_Statics::ScriptStructInfo[] = {
		{ FVehicleGearboxConfig::StaticStruct, Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics::NewStructOps, TEXT("VehicleGearboxConfig"), &Z_Registration_Info_UScriptStruct_VehicleGearboxConfig, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FVehicleGearboxConfig), 58634830U) },
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UVehicleGearboxComponent, UVehicleGearboxComponent::StaticClass, TEXT("UVehicleGearboxComponent"), &Z_Registration_Info_UClass_UVehicleGearboxComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVehicleGearboxComponent), 3429251215U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h_4054634831(TEXT("/Script/KinetiForge"),
		Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h_Statics::ClassInfo),
		Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h_Statics::ScriptStructInfo),
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
