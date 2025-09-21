// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "KinetiForge/Public/VehicleAxleAssemblyComponent.h"
#include "KinetiForge/Public/VehicleChassisParams.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeVehicleAxleAssemblyComponent() {}
// Cross Module References
	COREUOBJECT_API UClass* Z_Construct_UClass_UClass();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FRotator();
	ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UPrimitiveComponent_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_USceneComponent();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleAxleAssemblyComponent();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleAxleAssemblyComponent_NoRegister();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleDifferentialComponent_NoRegister();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleWheelComponent_NoRegister();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleWheelCoordinatorComponent_NoRegister();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleAxleConfig();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleAxleSimData();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleTCSConfig();
	UPackage* Z_Construct_UPackage__Script_KinetiForge();
// End Cross Module References
	DEFINE_FUNCTION(UVehicleAxleAssemblyComponent::execGetP3MotorTorque)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=P_THIS->GetP3MotorTorque();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleAxleAssemblyComponent::execSetP3MotorTorque)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_NewTorque);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->SetP3MotorTorque(Z_Param_NewTorque);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleAxleAssemblyComponent::execGetAxleMovement)
	{
		P_GET_STRUCT_REF(FVehicleAxleSimData,Z_Param_Out_Out);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->GetAxleMovement(Z_Param_Out_Out);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleAxleAssemblyComponent::execGetDifferential)
	{
		P_GET_OBJECT_REF(UVehicleDifferentialComponent,Z_Param_Out_OutDifferential);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->GetDifferential(Z_Param_Out_OutDifferential);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleAxleAssemblyComponent::execGetWheels)
	{
		P_GET_OBJECT_REF(UVehicleWheelComponent,Z_Param_Out_OutLeftWheel);
		P_GET_OBJECT_REF(UVehicleWheelComponent,Z_Param_Out_OutRightWheel);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->GetWheels(Z_Param_Out_OutLeftWheel,Z_Param_Out_OutRightWheel);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleAxleAssemblyComponent::execUpdatePhysics)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_InPhysicsDeltaTime);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InDriveTorque);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InBrakeInput);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InHandbrakeInput);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InSteeringInput);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InReflectedInertia);
		P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_OutAxleTotalInertia);
		P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_OutAngularVelocity);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->UpdatePhysics(Z_Param_InPhysicsDeltaTime,Z_Param_InDriveTorque,Z_Param_InBrakeInput,Z_Param_InHandbrakeInput,Z_Param_InSteeringInput,Z_Param_InReflectedInertia,Z_Param_Out_OutAxleTotalInertia,Z_Param_Out_OutAngularVelocity);
		P_NATIVE_END;
	}
	void UVehicleAxleAssemblyComponent::StaticRegisterNativesUVehicleAxleAssemblyComponent()
	{
		UClass* Class = UVehicleAxleAssemblyComponent::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "GetAxleMovement", &UVehicleAxleAssemblyComponent::execGetAxleMovement },
			{ "GetDifferential", &UVehicleAxleAssemblyComponent::execGetDifferential },
			{ "GetP3MotorTorque", &UVehicleAxleAssemblyComponent::execGetP3MotorTorque },
			{ "GetWheels", &UVehicleAxleAssemblyComponent::execGetWheels },
			{ "SetP3MotorTorque", &UVehicleAxleAssemblyComponent::execSetP3MotorTorque },
			{ "UpdatePhysics", &UVehicleAxleAssemblyComponent::execUpdatePhysics },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetAxleMovement_Statics
	{
		struct VehicleAxleAssemblyComponent_eventGetAxleMovement_Parms
		{
			FVehicleAxleSimData Out;
		};
		static const UECodeGen_Private::FStructPropertyParams NewProp_Out;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetAxleMovement_Statics::NewProp_Out = { "Out", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleAxleAssemblyComponent_eventGetAxleMovement_Parms, Out), Z_Construct_UScriptStruct_FVehicleAxleSimData, METADATA_PARAMS(0, nullptr) }; // 952266153
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetAxleMovement_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetAxleMovement_Statics::NewProp_Out,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetAxleMovement_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleAxleAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetAxleMovement_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleAxleAssemblyComponent, nullptr, "GetAxleMovement", nullptr, nullptr, Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetAxleMovement_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetAxleMovement_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetAxleMovement_Statics::VehicleAxleAssemblyComponent_eventGetAxleMovement_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetAxleMovement_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetAxleMovement_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetAxleMovement_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetAxleMovement_Statics::VehicleAxleAssemblyComponent_eventGetAxleMovement_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetAxleMovement()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetAxleMovement_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetDifferential_Statics
	{
		struct VehicleAxleAssemblyComponent_eventGetDifferential_Parms
		{
			UVehicleDifferentialComponent* OutDifferential;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_OutDifferential_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_OutDifferential;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetDifferential_Statics::NewProp_OutDifferential_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetDifferential_Statics::NewProp_OutDifferential = { "OutDifferential", nullptr, (EPropertyFlags)0x0010000000080180, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleAxleAssemblyComponent_eventGetDifferential_Parms, OutDifferential), Z_Construct_UClass_UVehicleDifferentialComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetDifferential_Statics::NewProp_OutDifferential_MetaData), Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetDifferential_Statics::NewProp_OutDifferential_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetDifferential_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetDifferential_Statics::NewProp_OutDifferential,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetDifferential_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleAxleAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetDifferential_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleAxleAssemblyComponent, nullptr, "GetDifferential", nullptr, nullptr, Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetDifferential_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetDifferential_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetDifferential_Statics::VehicleAxleAssemblyComponent_eventGetDifferential_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetDifferential_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetDifferential_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetDifferential_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetDifferential_Statics::VehicleAxleAssemblyComponent_eventGetDifferential_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetDifferential()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetDifferential_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetP3MotorTorque_Statics
	{
		struct VehicleAxleAssemblyComponent_eventGetP3MotorTorque_Parms
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
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetP3MotorTorque_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleAxleAssemblyComponent_eventGetP3MotorTorque_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetP3MotorTorque_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetP3MotorTorque_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetP3MotorTorque_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleAxleAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetP3MotorTorque_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleAxleAssemblyComponent, nullptr, "GetP3MotorTorque", nullptr, nullptr, Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetP3MotorTorque_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetP3MotorTorque_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetP3MotorTorque_Statics::VehicleAxleAssemblyComponent_eventGetP3MotorTorque_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetP3MotorTorque_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetP3MotorTorque_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetP3MotorTorque_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetP3MotorTorque_Statics::VehicleAxleAssemblyComponent_eventGetP3MotorTorque_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetP3MotorTorque()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetP3MotorTorque_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetWheels_Statics
	{
		struct VehicleAxleAssemblyComponent_eventGetWheels_Parms
		{
			UVehicleWheelComponent* OutLeftWheel;
			UVehicleWheelComponent* OutRightWheel;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_OutLeftWheel_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_OutLeftWheel;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_OutRightWheel_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_OutRightWheel;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetWheels_Statics::NewProp_OutLeftWheel_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetWheels_Statics::NewProp_OutLeftWheel = { "OutLeftWheel", nullptr, (EPropertyFlags)0x0010000000080180, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleAxleAssemblyComponent_eventGetWheels_Parms, OutLeftWheel), Z_Construct_UClass_UVehicleWheelComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetWheels_Statics::NewProp_OutLeftWheel_MetaData), Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetWheels_Statics::NewProp_OutLeftWheel_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetWheels_Statics::NewProp_OutRightWheel_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetWheels_Statics::NewProp_OutRightWheel = { "OutRightWheel", nullptr, (EPropertyFlags)0x0010000000080180, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleAxleAssemblyComponent_eventGetWheels_Parms, OutRightWheel), Z_Construct_UClass_UVehicleWheelComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetWheels_Statics::NewProp_OutRightWheel_MetaData), Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetWheels_Statics::NewProp_OutRightWheel_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetWheels_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetWheels_Statics::NewProp_OutLeftWheel,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetWheels_Statics::NewProp_OutRightWheel,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetWheels_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleAxleAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetWheels_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleAxleAssemblyComponent, nullptr, "GetWheels", nullptr, nullptr, Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetWheels_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetWheels_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetWheels_Statics::VehicleAxleAssemblyComponent_eventGetWheels_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetWheels_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetWheels_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetWheels_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetWheels_Statics::VehicleAxleAssemblyComponent_eventGetWheels_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetWheels()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetWheels_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleAxleAssemblyComponent_SetP3MotorTorque_Statics
	{
		struct VehicleAxleAssemblyComponent_eventSetP3MotorTorque_Parms
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
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleAxleAssemblyComponent_SetP3MotorTorque_Statics::NewProp_NewTorque = { "NewTorque", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleAxleAssemblyComponent_eventSetP3MotorTorque_Parms, NewTorque), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleAxleAssemblyComponent_SetP3MotorTorque_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleAxleAssemblyComponent_SetP3MotorTorque_Statics::NewProp_NewTorque,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleAxleAssemblyComponent_SetP3MotorTorque_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleAxleAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleAxleAssemblyComponent_SetP3MotorTorque_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleAxleAssemblyComponent, nullptr, "SetP3MotorTorque", nullptr, nullptr, Z_Construct_UFunction_UVehicleAxleAssemblyComponent_SetP3MotorTorque_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_SetP3MotorTorque_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_SetP3MotorTorque_Statics::VehicleAxleAssemblyComponent_eventSetP3MotorTorque_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_SetP3MotorTorque_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleAxleAssemblyComponent_SetP3MotorTorque_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_SetP3MotorTorque_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_SetP3MotorTorque_Statics::VehicleAxleAssemblyComponent_eventSetP3MotorTorque_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleAxleAssemblyComponent_SetP3MotorTorque()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleAxleAssemblyComponent_SetP3MotorTorque_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics
	{
		struct VehicleAxleAssemblyComponent_eventUpdatePhysics_Parms
		{
			float InPhysicsDeltaTime;
			float InDriveTorque;
			float InBrakeInput;
			float InHandbrakeInput;
			float InSteeringInput;
			float InReflectedInertia;
			float OutAxleTotalInertia;
			float OutAngularVelocity;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InPhysicsDeltaTime;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InDriveTorque;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InBrakeInput;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InHandbrakeInput;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InSteeringInput;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InReflectedInertia;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_OutAxleTotalInertia;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_OutAngularVelocity;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::NewProp_InPhysicsDeltaTime = { "InPhysicsDeltaTime", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleAxleAssemblyComponent_eventUpdatePhysics_Parms, InPhysicsDeltaTime), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::NewProp_InDriveTorque = { "InDriveTorque", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleAxleAssemblyComponent_eventUpdatePhysics_Parms, InDriveTorque), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::NewProp_InBrakeInput = { "InBrakeInput", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleAxleAssemblyComponent_eventUpdatePhysics_Parms, InBrakeInput), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::NewProp_InHandbrakeInput = { "InHandbrakeInput", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleAxleAssemblyComponent_eventUpdatePhysics_Parms, InHandbrakeInput), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::NewProp_InSteeringInput = { "InSteeringInput", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleAxleAssemblyComponent_eventUpdatePhysics_Parms, InSteeringInput), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::NewProp_InReflectedInertia = { "InReflectedInertia", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleAxleAssemblyComponent_eventUpdatePhysics_Parms, InReflectedInertia), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::NewProp_OutAxleTotalInertia = { "OutAxleTotalInertia", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleAxleAssemblyComponent_eventUpdatePhysics_Parms, OutAxleTotalInertia), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::NewProp_OutAngularVelocity = { "OutAngularVelocity", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleAxleAssemblyComponent_eventUpdatePhysics_Parms, OutAngularVelocity), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::NewProp_InPhysicsDeltaTime,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::NewProp_InDriveTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::NewProp_InBrakeInput,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::NewProp_InHandbrakeInput,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::NewProp_InSteeringInput,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::NewProp_InReflectedInertia,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::NewProp_OutAxleTotalInertia,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::NewProp_OutAngularVelocity,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::Function_MetaDataParams[] = {
		{ "Category", "PhysicsThreadOnly" },
		{ "ModuleRelativePath", "Public/VehicleAxleAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleAxleAssemblyComponent, nullptr, "UpdatePhysics", nullptr, nullptr, Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::VehicleAxleAssemblyComponent_eventUpdatePhysics_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::VehicleAxleAssemblyComponent_eventUpdatePhysics_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UVehicleAxleAssemblyComponent);
	UClass* Z_Construct_UClass_UVehicleAxleAssemblyComponent_NoRegister()
	{
		return UVehicleAxleAssemblyComponent::StaticClass();
	}
	struct Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_WheelConfig_MetaData[];
#endif
		static const UECodeGen_Private::FClassPropertyParams NewProp_WheelConfig;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_VehicleWheelComponentSetupRotation_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_VehicleWheelComponentSetupRotation;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_DifferentialConfig_MetaData[];
#endif
		static const UECodeGen_Private::FClassPropertyParams NewProp_DifferentialConfig;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AxleConfig_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_AxleConfig;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_AxleSteeringConfig_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_AxleSteeringConfig;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SteeringAssistConfig_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_SteeringAssistConfig;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TCSConfig_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_TCSConfig;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ParentActor_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_ParentActor;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Carbody_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Carbody;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_WheelCoordinator_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_WheelCoordinator;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_LeftWheel_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_LeftWheel;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RightWheel_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_RightWheel;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Differential_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Differential;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_USceneComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::DependentSingletons) < 16);
	const FClassFunctionLinkInfo Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetAxleMovement, "GetAxleMovement" }, // 3419595087
		{ &Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetDifferential, "GetDifferential" }, // 83649253
		{ &Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetP3MotorTorque, "GetP3MotorTorque" }, // 3768793228
		{ &Z_Construct_UFunction_UVehicleAxleAssemblyComponent_GetWheels, "GetWheels" }, // 1177604081
		{ &Z_Construct_UFunction_UVehicleAxleAssemblyComponent_SetP3MotorTorque, "SetP3MotorTorque" }, // 3955514480
		{ &Z_Construct_UFunction_UVehicleAxleAssemblyComponent_UpdatePhysics, "UpdatePhysics" }, // 1413963733
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::FuncInfo) < 2048);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "BlueprintType", "true" },
		{ "ClassGroupNames", "Custom" },
		{ "HideCategories", "Trigger PhysicsVolume" },
		{ "IncludePath", "VehicleAxleAssemblyComponent.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/VehicleAxleAssemblyComponent.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_WheelConfig_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleAxleAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_WheelConfig = { "WheelConfig", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleAxleAssemblyComponent, WheelConfig), Z_Construct_UClass_UClass, Z_Construct_UClass_UVehicleWheelComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_WheelConfig_MetaData), Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_WheelConfig_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_VehicleWheelComponentSetupRotation_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleAxleAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_VehicleWheelComponentSetupRotation = { "VehicleWheelComponentSetupRotation", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleAxleAssemblyComponent, VehicleWheelComponentSetupRotation), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_VehicleWheelComponentSetupRotation_MetaData), Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_VehicleWheelComponentSetupRotation_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_DifferentialConfig_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleAxleAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FClassPropertyParams Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_DifferentialConfig = { "DifferentialConfig", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::Class, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleAxleAssemblyComponent, DifferentialConfig), Z_Construct_UClass_UClass, Z_Construct_UClass_UVehicleDifferentialComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_DifferentialConfig_MetaData), Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_DifferentialConfig_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_AxleConfig_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleAxleAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_AxleConfig = { "AxleConfig", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleAxleAssemblyComponent, AxleConfig), Z_Construct_UScriptStruct_FVehicleAxleConfig, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_AxleConfig_MetaData), Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_AxleConfig_MetaData) }; // 71916922
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_AxleSteeringConfig_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleAxleAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_AxleSteeringConfig = { "AxleSteeringConfig", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleAxleAssemblyComponent, AxleSteeringConfig), Z_Construct_UScriptStruct_FVehicleAxleSteeringConfig, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_AxleSteeringConfig_MetaData), Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_AxleSteeringConfig_MetaData) }; // 1863805507
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_SteeringAssistConfig_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleAxleAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_SteeringAssistConfig = { "SteeringAssistConfig", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleAxleAssemblyComponent, SteeringAssistConfig), Z_Construct_UScriptStruct_FVehicleSteeringAssistConfig, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_SteeringAssistConfig_MetaData), Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_SteeringAssistConfig_MetaData) }; // 3644416740
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_TCSConfig_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleAxleAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_TCSConfig = { "TCSConfig", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleAxleAssemblyComponent, TCSConfig), Z_Construct_UScriptStruct_FVehicleTCSConfig, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_TCSConfig_MetaData), Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_TCSConfig_MetaData) }; // 1730587740
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_ParentActor_MetaData[] = {
		{ "ModuleRelativePath", "Public/VehicleAxleAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_ParentActor = { "ParentActor", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleAxleAssemblyComponent, ParentActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_ParentActor_MetaData), Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_ParentActor_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_Carbody_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/VehicleAxleAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_Carbody = { "Carbody", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleAxleAssemblyComponent, Carbody), Z_Construct_UClass_UPrimitiveComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_Carbody_MetaData), Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_Carbody_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_WheelCoordinator_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/VehicleAxleAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_WheelCoordinator = { "WheelCoordinator", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleAxleAssemblyComponent, WheelCoordinator), Z_Construct_UClass_UVehicleWheelCoordinatorComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_WheelCoordinator_MetaData), Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_WheelCoordinator_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_LeftWheel_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/VehicleAxleAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_LeftWheel = { "LeftWheel", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleAxleAssemblyComponent, LeftWheel), Z_Construct_UClass_UVehicleWheelComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_LeftWheel_MetaData), Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_LeftWheel_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_RightWheel_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/VehicleAxleAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_RightWheel = { "RightWheel", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleAxleAssemblyComponent, RightWheel), Z_Construct_UClass_UVehicleWheelComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_RightWheel_MetaData), Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_RightWheel_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_Differential_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/VehicleAxleAssemblyComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_Differential = { "Differential", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleAxleAssemblyComponent, Differential), Z_Construct_UClass_UVehicleDifferentialComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_Differential_MetaData), Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_Differential_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_WheelConfig,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_VehicleWheelComponentSetupRotation,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_DifferentialConfig,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_AxleConfig,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_AxleSteeringConfig,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_SteeringAssistConfig,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_TCSConfig,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_ParentActor,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_Carbody,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_WheelCoordinator,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_LeftWheel,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_RightWheel,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::NewProp_Differential,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVehicleAxleAssemblyComponent>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::ClassParams = {
		&UVehicleAxleAssemblyComponent::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::PropPointers),
		0,
		0x00B000A4u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::Class_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::PropPointers) < 2048);
	UClass* Z_Construct_UClass_UVehicleAxleAssemblyComponent()
	{
		if (!Z_Registration_Info_UClass_UVehicleAxleAssemblyComponent.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVehicleAxleAssemblyComponent.OuterSingleton, Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UVehicleAxleAssemblyComponent.OuterSingleton;
	}
	template<> KINETIFORGE_API UClass* StaticClass<UVehicleAxleAssemblyComponent>()
	{
		return UVehicleAxleAssemblyComponent::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UVehicleAxleAssemblyComponent);
	UVehicleAxleAssemblyComponent::~UVehicleAxleAssemblyComponent() {}
	struct Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleAxleAssemblyComponent_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleAxleAssemblyComponent_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UVehicleAxleAssemblyComponent, UVehicleAxleAssemblyComponent::StaticClass, TEXT("UVehicleAxleAssemblyComponent"), &Z_Registration_Info_UClass_UVehicleAxleAssemblyComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVehicleAxleAssemblyComponent), 2040534636U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleAxleAssemblyComponent_h_3681588592(TEXT("/Script/KinetiForge"),
		Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleAxleAssemblyComponent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleAxleAssemblyComponent_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
