// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "KinetiForge/Public/VehicleDifferentialComponent.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeVehicleDifferentialComponent() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleDifferentialComponent();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleDifferentialComponent_NoRegister();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig();
	UPackage* Z_Construct_UPackage__Script_KinetiForge();
// End Cross Module References
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_VehicleLimitedSlipDifferentialConfig;
class UScriptStruct* FVehicleLimitedSlipDifferentialConfig::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_VehicleLimitedSlipDifferentialConfig.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_VehicleLimitedSlipDifferentialConfig.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig, (UObject*)Z_Construct_UPackage__Script_KinetiForge(), TEXT("VehicleLimitedSlipDifferentialConfig"));
	}
	return Z_Registration_Info_UScriptStruct_VehicleLimitedSlipDifferentialConfig.OuterSingleton;
}
template<> KINETIFORGE_API UScriptStruct* StaticStruct<FVehicleLimitedSlipDifferentialConfig>()
{
	return FVehicleLimitedSlipDifferentialConfig::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_GearRatio_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_GearRatio;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_LockRatio_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_LockRatio;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/VehicleDifferentialComponent.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FVehicleLimitedSlipDifferentialConfig>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig_Statics::NewProp_GearRatio_MetaData[] = {
		{ "Category", "Setup" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleDifferentialComponent.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig_Statics::NewProp_GearRatio = { "GearRatio", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleLimitedSlipDifferentialConfig, GearRatio), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig_Statics::NewProp_GearRatio_MetaData), Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig_Statics::NewProp_GearRatio_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig_Statics::NewProp_LockRatio_MetaData[] = {
		{ "Category", "Setup" },
		{ "ClampMax", "1.0" },
		{ "ClampMin", "0.0" },
		{ "ModuleRelativePath", "Public/VehicleDifferentialComponent.h" },
		{ "UIMax", "1.0" },
		{ "UIMin", "0.0" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig_Statics::NewProp_LockRatio = { "LockRatio", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FVehicleLimitedSlipDifferentialConfig, LockRatio), METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig_Statics::NewProp_LockRatio_MetaData), Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig_Statics::NewProp_LockRatio_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig_Statics::NewProp_GearRatio,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig_Statics::NewProp_LockRatio,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
		nullptr,
		&NewStructOps,
		"VehicleLimitedSlipDifferentialConfig",
		Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig_Statics::PropPointers),
		sizeof(FVehicleLimitedSlipDifferentialConfig),
		alignof(FVehicleLimitedSlipDifferentialConfig),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig_Statics::Struct_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig_Statics::PropPointers) < 2048);
	UScriptStruct* Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig()
	{
		if (!Z_Registration_Info_UScriptStruct_VehicleLimitedSlipDifferentialConfig.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_VehicleLimitedSlipDifferentialConfig.InnerSingleton, Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_VehicleLimitedSlipDifferentialConfig.InnerSingleton;
	}
	DEFINE_FUNCTION(UVehicleDifferentialComponent::execGetInputShaftVelocity)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_OutputShaftAngularVelocityLeft);
		P_GET_PROPERTY(FFloatProperty,Z_Param_OutputShaftAngularVelocityRight);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=P_THIS->GetInputShaftVelocity(Z_Param_OutputShaftAngularVelocityLeft,Z_Param_OutputShaftAngularVelocityRight);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDifferentialComponent::execGetOpenDiffOutputTorque)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_InTorque);
		P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_OutTorqueLeft);
		P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_OutTorqueRight);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->GetOpenDiffOutputTorque(Z_Param_InTorque,Z_Param_Out_OutTorqueLeft,Z_Param_Out_OutTorqueRight);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDifferentialComponent::execGetOutputTorque)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_InTorque);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InLeftAngularVelocity);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InRightAngularVelocity);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InLeftTotalInertia);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InRightTotalInertia);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InDeltaTime);
		P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_OutLeftTorque);
		P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_OutRightTorque);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->GetOutputTorque(Z_Param_InTorque,Z_Param_InLeftAngularVelocity,Z_Param_InRightAngularVelocity,Z_Param_InLeftTotalInertia,Z_Param_InRightTotalInertia,Z_Param_InDeltaTime,Z_Param_Out_OutLeftTorque,Z_Param_Out_OutRightTorque);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDifferentialComponent::execUpdateInputShaft)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_InLeftOutputShaftAngularVelocity);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InRightOutputShaftAngularVelocity);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InLeftWheelInertia);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InRightWheelInertia);
		P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_OutInputShaftVelocity);
		P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_OutReflectedInertia);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->UpdateInputShaft(Z_Param_InLeftOutputShaftAngularVelocity,Z_Param_InRightOutputShaftAngularVelocity,Z_Param_InLeftWheelInertia,Z_Param_InRightWheelInertia,Z_Param_Out_OutInputShaftVelocity,Z_Param_Out_OutReflectedInertia);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleDifferentialComponent::execUpdateOutputShaft)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_InDriveTorque);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InLeftAngularVelocity);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InRightAngularVelocity);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InLeftTotalInertia);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InRightTotalInertia);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InDeltaTime);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InReflectedInertia);
		P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_OutLeftTorque);
		P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_OutRightTorque);
		P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_OutReflectedInertiaEachWheel);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->UpdateOutputShaft(Z_Param_InDriveTorque,Z_Param_InLeftAngularVelocity,Z_Param_InRightAngularVelocity,Z_Param_InLeftTotalInertia,Z_Param_InRightTotalInertia,Z_Param_InDeltaTime,Z_Param_InReflectedInertia,Z_Param_Out_OutLeftTorque,Z_Param_Out_OutRightTorque,Z_Param_Out_OutReflectedInertiaEachWheel);
		P_NATIVE_END;
	}
	void UVehicleDifferentialComponent::StaticRegisterNativesUVehicleDifferentialComponent()
	{
		UClass* Class = UVehicleDifferentialComponent::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "GetInputShaftVelocity", &UVehicleDifferentialComponent::execGetInputShaftVelocity },
			{ "GetOpenDiffOutputTorque", &UVehicleDifferentialComponent::execGetOpenDiffOutputTorque },
			{ "GetOutputTorque", &UVehicleDifferentialComponent::execGetOutputTorque },
			{ "UpdateInputShaft", &UVehicleDifferentialComponent::execUpdateInputShaft },
			{ "UpdateOutputShaft", &UVehicleDifferentialComponent::execUpdateOutputShaft },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UVehicleDifferentialComponent_GetInputShaftVelocity_Statics
	{
		struct VehicleDifferentialComponent_eventGetInputShaftVelocity_Parms
		{
			float OutputShaftAngularVelocityLeft;
			float OutputShaftAngularVelocityRight;
			float ReturnValue;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_OutputShaftAngularVelocityLeft;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_OutputShaftAngularVelocityRight;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_GetInputShaftVelocity_Statics::NewProp_OutputShaftAngularVelocityLeft = { "OutputShaftAngularVelocityLeft", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventGetInputShaftVelocity_Parms, OutputShaftAngularVelocityLeft), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_GetInputShaftVelocity_Statics::NewProp_OutputShaftAngularVelocityRight = { "OutputShaftAngularVelocityRight", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventGetInputShaftVelocity_Parms, OutputShaftAngularVelocityRight), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_GetInputShaftVelocity_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventGetInputShaftVelocity_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDifferentialComponent_GetInputShaftVelocity_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_GetInputShaftVelocity_Statics::NewProp_OutputShaftAngularVelocityLeft,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_GetInputShaftVelocity_Statics::NewProp_OutputShaftAngularVelocityRight,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_GetInputShaftVelocity_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDifferentialComponent_GetInputShaftVelocity_Statics::Function_MetaDataParams[] = {
		{ "Category", "PhysicsLogic" },
		{ "ModuleRelativePath", "Public/VehicleDifferentialComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDifferentialComponent_GetInputShaftVelocity_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDifferentialComponent, nullptr, "GetInputShaftVelocity", nullptr, nullptr, Z_Construct_UFunction_UVehicleDifferentialComponent_GetInputShaftVelocity_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDifferentialComponent_GetInputShaftVelocity_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDifferentialComponent_GetInputShaftVelocity_Statics::VehicleDifferentialComponent_eventGetInputShaftVelocity_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04040401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDifferentialComponent_GetInputShaftVelocity_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDifferentialComponent_GetInputShaftVelocity_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDifferentialComponent_GetInputShaftVelocity_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDifferentialComponent_GetInputShaftVelocity_Statics::VehicleDifferentialComponent_eventGetInputShaftVelocity_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDifferentialComponent_GetInputShaftVelocity()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDifferentialComponent_GetInputShaftVelocity_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDifferentialComponent_GetOpenDiffOutputTorque_Statics
	{
		struct VehicleDifferentialComponent_eventGetOpenDiffOutputTorque_Parms
		{
			float InTorque;
			float OutTorqueLeft;
			float OutTorqueRight;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InTorque;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_OutTorqueLeft;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_OutTorqueRight;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_GetOpenDiffOutputTorque_Statics::NewProp_InTorque = { "InTorque", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventGetOpenDiffOutputTorque_Parms, InTorque), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_GetOpenDiffOutputTorque_Statics::NewProp_OutTorqueLeft = { "OutTorqueLeft", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventGetOpenDiffOutputTorque_Parms, OutTorqueLeft), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_GetOpenDiffOutputTorque_Statics::NewProp_OutTorqueRight = { "OutTorqueRight", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventGetOpenDiffOutputTorque_Parms, OutTorqueRight), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDifferentialComponent_GetOpenDiffOutputTorque_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_GetOpenDiffOutputTorque_Statics::NewProp_InTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_GetOpenDiffOutputTorque_Statics::NewProp_OutTorqueLeft,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_GetOpenDiffOutputTorque_Statics::NewProp_OutTorqueRight,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDifferentialComponent_GetOpenDiffOutputTorque_Statics::Function_MetaDataParams[] = {
		{ "Category", "PhysicsLogic" },
		{ "ModuleRelativePath", "Public/VehicleDifferentialComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDifferentialComponent_GetOpenDiffOutputTorque_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDifferentialComponent, nullptr, "GetOpenDiffOutputTorque", nullptr, nullptr, Z_Construct_UFunction_UVehicleDifferentialComponent_GetOpenDiffOutputTorque_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDifferentialComponent_GetOpenDiffOutputTorque_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDifferentialComponent_GetOpenDiffOutputTorque_Statics::VehicleDifferentialComponent_eventGetOpenDiffOutputTorque_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04440401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDifferentialComponent_GetOpenDiffOutputTorque_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDifferentialComponent_GetOpenDiffOutputTorque_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDifferentialComponent_GetOpenDiffOutputTorque_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDifferentialComponent_GetOpenDiffOutputTorque_Statics::VehicleDifferentialComponent_eventGetOpenDiffOutputTorque_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDifferentialComponent_GetOpenDiffOutputTorque()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDifferentialComponent_GetOpenDiffOutputTorque_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics
	{
		struct VehicleDifferentialComponent_eventGetOutputTorque_Parms
		{
			float InTorque;
			float InLeftAngularVelocity;
			float InRightAngularVelocity;
			float InLeftTotalInertia;
			float InRightTotalInertia;
			float InDeltaTime;
			float OutLeftTorque;
			float OutRightTorque;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InTorque;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InLeftAngularVelocity;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InRightAngularVelocity;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InLeftTotalInertia;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InRightTotalInertia;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InDeltaTime;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_OutLeftTorque;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_OutRightTorque;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::NewProp_InTorque = { "InTorque", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventGetOutputTorque_Parms, InTorque), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::NewProp_InLeftAngularVelocity = { "InLeftAngularVelocity", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventGetOutputTorque_Parms, InLeftAngularVelocity), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::NewProp_InRightAngularVelocity = { "InRightAngularVelocity", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventGetOutputTorque_Parms, InRightAngularVelocity), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::NewProp_InLeftTotalInertia = { "InLeftTotalInertia", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventGetOutputTorque_Parms, InLeftTotalInertia), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::NewProp_InRightTotalInertia = { "InRightTotalInertia", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventGetOutputTorque_Parms, InRightTotalInertia), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::NewProp_InDeltaTime = { "InDeltaTime", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventGetOutputTorque_Parms, InDeltaTime), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::NewProp_OutLeftTorque = { "OutLeftTorque", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventGetOutputTorque_Parms, OutLeftTorque), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::NewProp_OutRightTorque = { "OutRightTorque", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventGetOutputTorque_Parms, OutRightTorque), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::NewProp_InTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::NewProp_InLeftAngularVelocity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::NewProp_InRightAngularVelocity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::NewProp_InLeftTotalInertia,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::NewProp_InRightTotalInertia,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::NewProp_InDeltaTime,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::NewProp_OutLeftTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::NewProp_OutRightTorque,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::Function_MetaDataParams[] = {
		{ "Category", "PhysicsLogic" },
		{ "ModuleRelativePath", "Public/VehicleDifferentialComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDifferentialComponent, nullptr, "GetOutputTorque", nullptr, nullptr, Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::VehicleDifferentialComponent_eventGetOutputTorque_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04440401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::VehicleDifferentialComponent_eventGetOutputTorque_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft_Statics
	{
		struct VehicleDifferentialComponent_eventUpdateInputShaft_Parms
		{
			float InLeftOutputShaftAngularVelocity;
			float InRightOutputShaftAngularVelocity;
			float InLeftWheelInertia;
			float InRightWheelInertia;
			float OutInputShaftVelocity;
			float OutReflectedInertia;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InLeftOutputShaftAngularVelocity;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InRightOutputShaftAngularVelocity;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InLeftWheelInertia;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InRightWheelInertia;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_OutInputShaftVelocity;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_OutReflectedInertia;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft_Statics::NewProp_InLeftOutputShaftAngularVelocity = { "InLeftOutputShaftAngularVelocity", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventUpdateInputShaft_Parms, InLeftOutputShaftAngularVelocity), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft_Statics::NewProp_InRightOutputShaftAngularVelocity = { "InRightOutputShaftAngularVelocity", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventUpdateInputShaft_Parms, InRightOutputShaftAngularVelocity), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft_Statics::NewProp_InLeftWheelInertia = { "InLeftWheelInertia", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventUpdateInputShaft_Parms, InLeftWheelInertia), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft_Statics::NewProp_InRightWheelInertia = { "InRightWheelInertia", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventUpdateInputShaft_Parms, InRightWheelInertia), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft_Statics::NewProp_OutInputShaftVelocity = { "OutInputShaftVelocity", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventUpdateInputShaft_Parms, OutInputShaftVelocity), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft_Statics::NewProp_OutReflectedInertia = { "OutReflectedInertia", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventUpdateInputShaft_Parms, OutReflectedInertia), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft_Statics::NewProp_InLeftOutputShaftAngularVelocity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft_Statics::NewProp_InRightOutputShaftAngularVelocity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft_Statics::NewProp_InLeftWheelInertia,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft_Statics::NewProp_InRightWheelInertia,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft_Statics::NewProp_OutInputShaftVelocity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft_Statics::NewProp_OutReflectedInertia,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft_Statics::Function_MetaDataParams[] = {
		{ "Category", "PhysicsLogic" },
		{ "ModuleRelativePath", "Public/VehicleDifferentialComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDifferentialComponent, nullptr, "UpdateInputShaft", nullptr, nullptr, Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft_Statics::VehicleDifferentialComponent_eventUpdateInputShaft_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft_Statics::VehicleDifferentialComponent_eventUpdateInputShaft_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics
	{
		struct VehicleDifferentialComponent_eventUpdateOutputShaft_Parms
		{
			float InDriveTorque;
			float InLeftAngularVelocity;
			float InRightAngularVelocity;
			float InLeftTotalInertia;
			float InRightTotalInertia;
			float InDeltaTime;
			float InReflectedInertia;
			float OutLeftTorque;
			float OutRightTorque;
			float OutReflectedInertiaEachWheel;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InDriveTorque;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InLeftAngularVelocity;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InRightAngularVelocity;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InLeftTotalInertia;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InRightTotalInertia;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InDeltaTime;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InReflectedInertia;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_OutLeftTorque;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_OutRightTorque;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_OutReflectedInertiaEachWheel;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::NewProp_InDriveTorque = { "InDriveTorque", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventUpdateOutputShaft_Parms, InDriveTorque), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::NewProp_InLeftAngularVelocity = { "InLeftAngularVelocity", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventUpdateOutputShaft_Parms, InLeftAngularVelocity), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::NewProp_InRightAngularVelocity = { "InRightAngularVelocity", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventUpdateOutputShaft_Parms, InRightAngularVelocity), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::NewProp_InLeftTotalInertia = { "InLeftTotalInertia", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventUpdateOutputShaft_Parms, InLeftTotalInertia), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::NewProp_InRightTotalInertia = { "InRightTotalInertia", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventUpdateOutputShaft_Parms, InRightTotalInertia), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::NewProp_InDeltaTime = { "InDeltaTime", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventUpdateOutputShaft_Parms, InDeltaTime), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::NewProp_InReflectedInertia = { "InReflectedInertia", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventUpdateOutputShaft_Parms, InReflectedInertia), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::NewProp_OutLeftTorque = { "OutLeftTorque", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventUpdateOutputShaft_Parms, OutLeftTorque), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::NewProp_OutRightTorque = { "OutRightTorque", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventUpdateOutputShaft_Parms, OutRightTorque), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::NewProp_OutReflectedInertiaEachWheel = { "OutReflectedInertiaEachWheel", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleDifferentialComponent_eventUpdateOutputShaft_Parms, OutReflectedInertiaEachWheel), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::NewProp_InDriveTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::NewProp_InLeftAngularVelocity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::NewProp_InRightAngularVelocity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::NewProp_InLeftTotalInertia,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::NewProp_InRightTotalInertia,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::NewProp_InDeltaTime,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::NewProp_InReflectedInertia,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::NewProp_OutLeftTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::NewProp_OutRightTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::NewProp_OutReflectedInertiaEachWheel,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::Function_MetaDataParams[] = {
		{ "Category", "PhysicsLogic" },
		{ "ModuleRelativePath", "Public/VehicleDifferentialComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleDifferentialComponent, nullptr, "UpdateOutputShaft", nullptr, nullptr, Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::VehicleDifferentialComponent_eventUpdateOutputShaft_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::VehicleDifferentialComponent_eventUpdateOutputShaft_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UVehicleDifferentialComponent);
	UClass* Z_Construct_UClass_UVehicleDifferentialComponent_NoRegister()
	{
		return UVehicleDifferentialComponent::StaticClass();
	}
	struct Z_Construct_UClass_UVehicleDifferentialComponent_Statics
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
	UObject* (*const Z_Construct_UClass_UVehicleDifferentialComponent_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UActorComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDifferentialComponent_Statics::DependentSingletons) < 16);
	const FClassFunctionLinkInfo Z_Construct_UClass_UVehicleDifferentialComponent_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UVehicleDifferentialComponent_GetInputShaftVelocity, "GetInputShaftVelocity" }, // 775141617
		{ &Z_Construct_UFunction_UVehicleDifferentialComponent_GetOpenDiffOutputTorque, "GetOpenDiffOutputTorque" }, // 4279318032
		{ &Z_Construct_UFunction_UVehicleDifferentialComponent_GetOutputTorque, "GetOutputTorque" }, // 3688577622
		{ &Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateInputShaft, "UpdateInputShaft" }, // 1501203708
		{ &Z_Construct_UFunction_UVehicleDifferentialComponent_UpdateOutputShaft, "UpdateOutputShaft" }, // 554649832
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDifferentialComponent_Statics::FuncInfo) < 2048);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleDifferentialComponent_Statics::Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "BlueprintType", "true" },
		{ "ClassGroupNames", "Custom" },
		{ "IncludePath", "VehicleDifferentialComponent.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/VehicleDifferentialComponent.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleDifferentialComponent_Statics::NewProp_Config_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleDifferentialComponent.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UVehicleDifferentialComponent_Statics::NewProp_Config = { "Config", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleDifferentialComponent, Config), Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDifferentialComponent_Statics::NewProp_Config_MetaData), Z_Construct_UClass_UVehicleDifferentialComponent_Statics::NewProp_Config_MetaData) }; // 4294160802
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UVehicleDifferentialComponent_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleDifferentialComponent_Statics::NewProp_Config,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UVehicleDifferentialComponent_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVehicleDifferentialComponent>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UVehicleDifferentialComponent_Statics::ClassParams = {
		&UVehicleDifferentialComponent::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_UVehicleDifferentialComponent_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDifferentialComponent_Statics::PropPointers),
		0,
		0x00B000A4u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDifferentialComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UVehicleDifferentialComponent_Statics::Class_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleDifferentialComponent_Statics::PropPointers) < 2048);
	UClass* Z_Construct_UClass_UVehicleDifferentialComponent()
	{
		if (!Z_Registration_Info_UClass_UVehicleDifferentialComponent.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVehicleDifferentialComponent.OuterSingleton, Z_Construct_UClass_UVehicleDifferentialComponent_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UVehicleDifferentialComponent.OuterSingleton;
	}
	template<> KINETIFORGE_API UClass* StaticClass<UVehicleDifferentialComponent>()
	{
		return UVehicleDifferentialComponent::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UVehicleDifferentialComponent);
	UVehicleDifferentialComponent::~UVehicleDifferentialComponent() {}
	struct Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h_Statics
	{
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h_Statics::ScriptStructInfo[] = {
		{ FVehicleLimitedSlipDifferentialConfig::StaticStruct, Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig_Statics::NewStructOps, TEXT("VehicleLimitedSlipDifferentialConfig"), &Z_Registration_Info_UScriptStruct_VehicleLimitedSlipDifferentialConfig, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FVehicleLimitedSlipDifferentialConfig), 4294160802U) },
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UVehicleDifferentialComponent, UVehicleDifferentialComponent::StaticClass, TEXT("UVehicleDifferentialComponent"), &Z_Registration_Info_UClass_UVehicleDifferentialComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVehicleDifferentialComponent), 1218457105U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h_4294327509(TEXT("/Script/KinetiForge"),
		Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h_Statics::ClassInfo),
		Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h_Statics::ScriptStructInfo),
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
