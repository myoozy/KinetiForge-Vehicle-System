// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AsyncTickPhysics/Public/AsyncTickFunctions.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeAsyncTickFunctions() {}
// Cross Module References
	ASYNCTICKPHYSICS_API UClass* Z_Construct_UClass_UAsyncTickFunctions();
	ASYNCTICKPHYSICS_API UClass* Z_Construct_UClass_UAsyncTickFunctions_NoRegister();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FRotator();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FTransform();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
	ENGINE_API UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();
	ENGINE_API UClass* Z_Construct_UClass_UPrimitiveComponent_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_USceneComponent_NoRegister();
	UPackage* Z_Construct_UPackage__Script_AsyncTickPhysics();
// End Cross Module References
	DEFINE_FUNCTION(UAsyncTickFunctions::execATP_GetLinearVelocityAtPoint)
	{
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_Component);
		P_GET_STRUCT(FVector,Z_Param_Point);
		P_GET_PROPERTY(FNameProperty,Z_Param_BoneName);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FVector*)Z_Param__Result=UAsyncTickFunctions::ATP_GetLinearVelocityAtPoint(Z_Param_Component,Z_Param_Point,Z_Param_BoneName);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UAsyncTickFunctions::execATP_SetWorldLocationAndRotation)
	{
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_Component);
		P_GET_STRUCT(FVector,Z_Param_Location);
		P_GET_STRUCT(FRotator,Z_Param_Rotation);
		P_FINISH;
		P_NATIVE_BEGIN;
		UAsyncTickFunctions::ATP_SetWorldLocationAndRotation(Z_Param_Component,Z_Param_Location,Z_Param_Rotation);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UAsyncTickFunctions::execATP_SetWorldRotation)
	{
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_Component);
		P_GET_STRUCT(FRotator,Z_Param_Rotation);
		P_FINISH;
		P_NATIVE_BEGIN;
		UAsyncTickFunctions::ATP_SetWorldRotation(Z_Param_Component,Z_Param_Rotation);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UAsyncTickFunctions::execATP_SetWorldLocation)
	{
		P_GET_OBJECT(USceneComponent,Z_Param_Component);
		P_GET_STRUCT(FVector,Z_Param_Location);
		P_FINISH;
		P_NATIVE_BEGIN;
		UAsyncTickFunctions::ATP_SetWorldLocation(Z_Param_Component,Z_Param_Location);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UAsyncTickFunctions::execATP_SetAngularVelocityInDegrees)
	{
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_Component);
		P_GET_STRUCT(FVector,Z_Param_AngVelocity);
		P_GET_UBOOL(Z_Param_bAddToCurrent);
		P_GET_PROPERTY(FNameProperty,Z_Param_BoneName);
		P_FINISH;
		P_NATIVE_BEGIN;
		UAsyncTickFunctions::ATP_SetAngularVelocityInDegrees(Z_Param_Component,Z_Param_AngVelocity,Z_Param_bAddToCurrent,Z_Param_BoneName);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UAsyncTickFunctions::execATP_SetAngularVelocityInRadians)
	{
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_Component);
		P_GET_STRUCT(FVector,Z_Param_AngVelocity);
		P_GET_UBOOL(Z_Param_bAddToCurrent);
		P_GET_PROPERTY(FNameProperty,Z_Param_BoneName);
		P_FINISH;
		P_NATIVE_BEGIN;
		UAsyncTickFunctions::ATP_SetAngularVelocityInRadians(Z_Param_Component,Z_Param_AngVelocity,Z_Param_bAddToCurrent,Z_Param_BoneName);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UAsyncTickFunctions::execATP_SetLinearVelocity)
	{
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_Component);
		P_GET_STRUCT(FVector,Z_Param_Velocity);
		P_GET_UBOOL(Z_Param_bAddToCurrent);
		P_GET_PROPERTY(FNameProperty,Z_Param_BoneName);
		P_FINISH;
		P_NATIVE_BEGIN;
		UAsyncTickFunctions::ATP_SetLinearVelocity(Z_Param_Component,Z_Param_Velocity,Z_Param_bAddToCurrent,Z_Param_BoneName);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UAsyncTickFunctions::execATP_GetAngularVelocity)
	{
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_Component);
		P_GET_PROPERTY(FNameProperty,Z_Param_BoneName);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FVector*)Z_Param__Result=UAsyncTickFunctions::ATP_GetAngularVelocity(Z_Param_Component,Z_Param_BoneName);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UAsyncTickFunctions::execATP_GetCoMPosition)
	{
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_Component);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FVector*)Z_Param__Result=UAsyncTickFunctions::ATP_GetCoMPosition(Z_Param_Component);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UAsyncTickFunctions::execATP_GetLinearVelocity)
	{
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_Component);
		P_GET_PROPERTY(FNameProperty,Z_Param_BoneName);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FVector*)Z_Param__Result=UAsyncTickFunctions::ATP_GetLinearVelocity(Z_Param_Component,Z_Param_BoneName);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UAsyncTickFunctions::execATP_GetTransform)
	{
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_Component);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FTransform*)Z_Param__Result=UAsyncTickFunctions::ATP_GetTransform(Z_Param_Component);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UAsyncTickFunctions::execATP_AddAngularImpulseInDegrees)
	{
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_Component);
		P_GET_STRUCT(FVector,Z_Param_Torque);
		P_GET_UBOOL(Z_Param_bVelChange);
		P_GET_PROPERTY(FNameProperty,Z_Param_BoneName);
		P_FINISH;
		P_NATIVE_BEGIN;
		UAsyncTickFunctions::ATP_AddAngularImpulseInDegrees(Z_Param_Component,Z_Param_Torque,Z_Param_bVelChange,Z_Param_BoneName);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UAsyncTickFunctions::execATP_AddAngularImpulseInRadians)
	{
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_Component);
		P_GET_STRUCT(FVector,Z_Param_Torque);
		P_GET_UBOOL(Z_Param_bVelChange);
		P_GET_PROPERTY(FNameProperty,Z_Param_BoneName);
		P_FINISH;
		P_NATIVE_BEGIN;
		UAsyncTickFunctions::ATP_AddAngularImpulseInRadians(Z_Param_Component,Z_Param_Torque,Z_Param_bVelChange,Z_Param_BoneName);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UAsyncTickFunctions::execATP_AddImpulseAtPosition)
	{
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_Component);
		P_GET_STRUCT(FVector,Z_Param_Position);
		P_GET_STRUCT(FVector,Z_Param_Impulse);
		P_GET_PROPERTY(FNameProperty,Z_Param_BoneName);
		P_FINISH;
		P_NATIVE_BEGIN;
		UAsyncTickFunctions::ATP_AddImpulseAtPosition(Z_Param_Component,Z_Param_Position,Z_Param_Impulse,Z_Param_BoneName);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UAsyncTickFunctions::execATP_AddImpulse)
	{
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_Component);
		P_GET_STRUCT(FVector,Z_Param_Impulse);
		P_GET_UBOOL(Z_Param_bVelChange);
		P_GET_PROPERTY(FNameProperty,Z_Param_BoneName);
		P_FINISH;
		P_NATIVE_BEGIN;
		UAsyncTickFunctions::ATP_AddImpulse(Z_Param_Component,Z_Param_Impulse,Z_Param_bVelChange,Z_Param_BoneName);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UAsyncTickFunctions::execATP_AddTorque)
	{
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_Component);
		P_GET_STRUCT(FVector,Z_Param_Torque);
		P_GET_UBOOL(Z_Param_bAccelChange);
		P_GET_PROPERTY(FNameProperty,Z_Param_BoneName);
		P_FINISH;
		P_NATIVE_BEGIN;
		UAsyncTickFunctions::ATP_AddTorque(Z_Param_Component,Z_Param_Torque,Z_Param_bAccelChange,Z_Param_BoneName);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UAsyncTickFunctions::execATP_AddForceAtPosition)
	{
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_Component);
		P_GET_STRUCT(FVector,Z_Param_Position);
		P_GET_STRUCT(FVector,Z_Param_Force);
		P_GET_PROPERTY(FNameProperty,Z_Param_BoneName);
		P_FINISH;
		P_NATIVE_BEGIN;
		UAsyncTickFunctions::ATP_AddForceAtPosition(Z_Param_Component,Z_Param_Position,Z_Param_Force,Z_Param_BoneName);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UAsyncTickFunctions::execATP_AddForce)
	{
		P_GET_OBJECT(UPrimitiveComponent,Z_Param_Component);
		P_GET_STRUCT(FVector,Z_Param_Force);
		P_GET_UBOOL(Z_Param_bAccelChange);
		P_GET_PROPERTY(FNameProperty,Z_Param_BoneName);
		P_FINISH;
		P_NATIVE_BEGIN;
		UAsyncTickFunctions::ATP_AddForce(Z_Param_Component,Z_Param_Force,Z_Param_bAccelChange,Z_Param_BoneName);
		P_NATIVE_END;
	}
	void UAsyncTickFunctions::StaticRegisterNativesUAsyncTickFunctions()
	{
		UClass* Class = UAsyncTickFunctions::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "ATP_AddAngularImpulseInDegrees", &UAsyncTickFunctions::execATP_AddAngularImpulseInDegrees },
			{ "ATP_AddAngularImpulseInRadians", &UAsyncTickFunctions::execATP_AddAngularImpulseInRadians },
			{ "ATP_AddForce", &UAsyncTickFunctions::execATP_AddForce },
			{ "ATP_AddForceAtPosition", &UAsyncTickFunctions::execATP_AddForceAtPosition },
			{ "ATP_AddImpulse", &UAsyncTickFunctions::execATP_AddImpulse },
			{ "ATP_AddImpulseAtPosition", &UAsyncTickFunctions::execATP_AddImpulseAtPosition },
			{ "ATP_AddTorque", &UAsyncTickFunctions::execATP_AddTorque },
			{ "ATP_GetAngularVelocity", &UAsyncTickFunctions::execATP_GetAngularVelocity },
			{ "ATP_GetCoMPosition", &UAsyncTickFunctions::execATP_GetCoMPosition },
			{ "ATP_GetLinearVelocity", &UAsyncTickFunctions::execATP_GetLinearVelocity },
			{ "ATP_GetLinearVelocityAtPoint", &UAsyncTickFunctions::execATP_GetLinearVelocityAtPoint },
			{ "ATP_GetTransform", &UAsyncTickFunctions::execATP_GetTransform },
			{ "ATP_SetAngularVelocityInDegrees", &UAsyncTickFunctions::execATP_SetAngularVelocityInDegrees },
			{ "ATP_SetAngularVelocityInRadians", &UAsyncTickFunctions::execATP_SetAngularVelocityInRadians },
			{ "ATP_SetLinearVelocity", &UAsyncTickFunctions::execATP_SetLinearVelocity },
			{ "ATP_SetWorldLocation", &UAsyncTickFunctions::execATP_SetWorldLocation },
			{ "ATP_SetWorldLocationAndRotation", &UAsyncTickFunctions::execATP_SetWorldLocationAndRotation },
			{ "ATP_SetWorldRotation", &UAsyncTickFunctions::execATP_SetWorldRotation },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees_Statics
	{
		struct AsyncTickFunctions_eventATP_AddAngularImpulseInDegrees_Parms
		{
			UPrimitiveComponent* Component;
			FVector Torque;
			bool bVelChange;
			FName BoneName;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
		static const UECodeGen_Private::FStructPropertyParams NewProp_Torque;
		static void NewProp_bVelChange_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bVelChange;
		static const UECodeGen_Private::FNamePropertyParams NewProp_BoneName;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees_Statics::NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_AddAngularImpulseInDegrees_Parms, Component), Z_Construct_UClass_UPrimitiveComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees_Statics::NewProp_Component_MetaData), Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees_Statics::NewProp_Component_MetaData) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees_Statics::NewProp_Torque = { "Torque", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_AddAngularImpulseInDegrees_Parms, Torque), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
	void Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees_Statics::NewProp_bVelChange_SetBit(void* Obj)
	{
		((AsyncTickFunctions_eventATP_AddAngularImpulseInDegrees_Parms*)Obj)->bVelChange = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees_Statics::NewProp_bVelChange = { "bVelChange", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AsyncTickFunctions_eventATP_AddAngularImpulseInDegrees_Parms), &Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees_Statics::NewProp_bVelChange_SetBit, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees_Statics::NewProp_BoneName = { "BoneName", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_AddAngularImpulseInDegrees_Parms, BoneName), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees_Statics::NewProp_Component,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees_Statics::NewProp_Torque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees_Statics::NewProp_bVelChange,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees_Statics::NewProp_BoneName,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees_Statics::Function_MetaDataParams[] = {
		{ "Category", "ATP - AsyncTickPhysics" },
		{ "CPP_Default_BoneName", "None" },
		{ "ModuleRelativePath", "Public/AsyncTickFunctions.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UAsyncTickFunctions, nullptr, "ATP_AddAngularImpulseInDegrees", nullptr, nullptr, Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees_Statics::AsyncTickFunctions_eventATP_AddAngularImpulseInDegrees_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04822401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees_Statics::AsyncTickFunctions_eventATP_AddAngularImpulseInDegrees_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians_Statics
	{
		struct AsyncTickFunctions_eventATP_AddAngularImpulseInRadians_Parms
		{
			UPrimitiveComponent* Component;
			FVector Torque;
			bool bVelChange;
			FName BoneName;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
		static const UECodeGen_Private::FStructPropertyParams NewProp_Torque;
		static void NewProp_bVelChange_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bVelChange;
		static const UECodeGen_Private::FNamePropertyParams NewProp_BoneName;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians_Statics::NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_AddAngularImpulseInRadians_Parms, Component), Z_Construct_UClass_UPrimitiveComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians_Statics::NewProp_Component_MetaData), Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians_Statics::NewProp_Component_MetaData) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians_Statics::NewProp_Torque = { "Torque", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_AddAngularImpulseInRadians_Parms, Torque), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
	void Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians_Statics::NewProp_bVelChange_SetBit(void* Obj)
	{
		((AsyncTickFunctions_eventATP_AddAngularImpulseInRadians_Parms*)Obj)->bVelChange = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians_Statics::NewProp_bVelChange = { "bVelChange", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AsyncTickFunctions_eventATP_AddAngularImpulseInRadians_Parms), &Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians_Statics::NewProp_bVelChange_SetBit, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians_Statics::NewProp_BoneName = { "BoneName", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_AddAngularImpulseInRadians_Parms, BoneName), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians_Statics::NewProp_Component,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians_Statics::NewProp_Torque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians_Statics::NewProp_bVelChange,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians_Statics::NewProp_BoneName,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians_Statics::Function_MetaDataParams[] = {
		{ "Category", "ATP - AsyncTickPhysics" },
		{ "CPP_Default_BoneName", "None" },
		{ "ModuleRelativePath", "Public/AsyncTickFunctions.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UAsyncTickFunctions, nullptr, "ATP_AddAngularImpulseInRadians", nullptr, nullptr, Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians_Statics::AsyncTickFunctions_eventATP_AddAngularImpulseInRadians_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04822401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians_Statics::AsyncTickFunctions_eventATP_AddAngularImpulseInRadians_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce_Statics
	{
		struct AsyncTickFunctions_eventATP_AddForce_Parms
		{
			UPrimitiveComponent* Component;
			FVector Force;
			bool bAccelChange;
			FName BoneName;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
		static const UECodeGen_Private::FStructPropertyParams NewProp_Force;
		static void NewProp_bAccelChange_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bAccelChange;
		static const UECodeGen_Private::FNamePropertyParams NewProp_BoneName;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce_Statics::NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_AddForce_Parms, Component), Z_Construct_UClass_UPrimitiveComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce_Statics::NewProp_Component_MetaData), Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce_Statics::NewProp_Component_MetaData) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce_Statics::NewProp_Force = { "Force", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_AddForce_Parms, Force), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
	void Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce_Statics::NewProp_bAccelChange_SetBit(void* Obj)
	{
		((AsyncTickFunctions_eventATP_AddForce_Parms*)Obj)->bAccelChange = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce_Statics::NewProp_bAccelChange = { "bAccelChange", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AsyncTickFunctions_eventATP_AddForce_Parms), &Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce_Statics::NewProp_bAccelChange_SetBit, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce_Statics::NewProp_BoneName = { "BoneName", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_AddForce_Parms, BoneName), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce_Statics::NewProp_Component,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce_Statics::NewProp_Force,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce_Statics::NewProp_bAccelChange,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce_Statics::NewProp_BoneName,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce_Statics::Function_MetaDataParams[] = {
		{ "Category", "ATP - AsyncTickPhysics" },
		{ "CPP_Default_BoneName", "None" },
		{ "ModuleRelativePath", "Public/AsyncTickFunctions.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UAsyncTickFunctions, nullptr, "ATP_AddForce", nullptr, nullptr, Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce_Statics::AsyncTickFunctions_eventATP_AddForce_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04822401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce_Statics::AsyncTickFunctions_eventATP_AddForce_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForceAtPosition_Statics
	{
		struct AsyncTickFunctions_eventATP_AddForceAtPosition_Parms
		{
			UPrimitiveComponent* Component;
			FVector Position;
			FVector Force;
			FName BoneName;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
		static const UECodeGen_Private::FStructPropertyParams NewProp_Position;
		static const UECodeGen_Private::FStructPropertyParams NewProp_Force;
		static const UECodeGen_Private::FNamePropertyParams NewProp_BoneName;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForceAtPosition_Statics::NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForceAtPosition_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_AddForceAtPosition_Parms, Component), Z_Construct_UClass_UPrimitiveComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForceAtPosition_Statics::NewProp_Component_MetaData), Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForceAtPosition_Statics::NewProp_Component_MetaData) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForceAtPosition_Statics::NewProp_Position = { "Position", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_AddForceAtPosition_Parms, Position), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForceAtPosition_Statics::NewProp_Force = { "Force", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_AddForceAtPosition_Parms, Force), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForceAtPosition_Statics::NewProp_BoneName = { "BoneName", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_AddForceAtPosition_Parms, BoneName), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForceAtPosition_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForceAtPosition_Statics::NewProp_Component,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForceAtPosition_Statics::NewProp_Position,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForceAtPosition_Statics::NewProp_Force,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForceAtPosition_Statics::NewProp_BoneName,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForceAtPosition_Statics::Function_MetaDataParams[] = {
		{ "Category", "ATP - AsyncTickPhysics" },
		{ "CPP_Default_BoneName", "None" },
		{ "ModuleRelativePath", "Public/AsyncTickFunctions.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForceAtPosition_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UAsyncTickFunctions, nullptr, "ATP_AddForceAtPosition", nullptr, nullptr, Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForceAtPosition_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForceAtPosition_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForceAtPosition_Statics::AsyncTickFunctions_eventATP_AddForceAtPosition_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04822401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForceAtPosition_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForceAtPosition_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForceAtPosition_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForceAtPosition_Statics::AsyncTickFunctions_eventATP_AddForceAtPosition_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForceAtPosition()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForceAtPosition_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse_Statics
	{
		struct AsyncTickFunctions_eventATP_AddImpulse_Parms
		{
			UPrimitiveComponent* Component;
			FVector Impulse;
			bool bVelChange;
			FName BoneName;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
		static const UECodeGen_Private::FStructPropertyParams NewProp_Impulse;
		static void NewProp_bVelChange_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bVelChange;
		static const UECodeGen_Private::FNamePropertyParams NewProp_BoneName;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse_Statics::NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_AddImpulse_Parms, Component), Z_Construct_UClass_UPrimitiveComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse_Statics::NewProp_Component_MetaData), Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse_Statics::NewProp_Component_MetaData) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse_Statics::NewProp_Impulse = { "Impulse", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_AddImpulse_Parms, Impulse), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
	void Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse_Statics::NewProp_bVelChange_SetBit(void* Obj)
	{
		((AsyncTickFunctions_eventATP_AddImpulse_Parms*)Obj)->bVelChange = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse_Statics::NewProp_bVelChange = { "bVelChange", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AsyncTickFunctions_eventATP_AddImpulse_Parms), &Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse_Statics::NewProp_bVelChange_SetBit, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse_Statics::NewProp_BoneName = { "BoneName", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_AddImpulse_Parms, BoneName), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse_Statics::NewProp_Component,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse_Statics::NewProp_Impulse,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse_Statics::NewProp_bVelChange,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse_Statics::NewProp_BoneName,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse_Statics::Function_MetaDataParams[] = {
		{ "Category", "ATP - AsyncTickPhysics" },
		{ "CPP_Default_BoneName", "None" },
		{ "ModuleRelativePath", "Public/AsyncTickFunctions.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UAsyncTickFunctions, nullptr, "ATP_AddImpulse", nullptr, nullptr, Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse_Statics::AsyncTickFunctions_eventATP_AddImpulse_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04822401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse_Statics::AsyncTickFunctions_eventATP_AddImpulse_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulseAtPosition_Statics
	{
		struct AsyncTickFunctions_eventATP_AddImpulseAtPosition_Parms
		{
			UPrimitiveComponent* Component;
			FVector Position;
			FVector Impulse;
			FName BoneName;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
		static const UECodeGen_Private::FStructPropertyParams NewProp_Position;
		static const UECodeGen_Private::FStructPropertyParams NewProp_Impulse;
		static const UECodeGen_Private::FNamePropertyParams NewProp_BoneName;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulseAtPosition_Statics::NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulseAtPosition_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_AddImpulseAtPosition_Parms, Component), Z_Construct_UClass_UPrimitiveComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulseAtPosition_Statics::NewProp_Component_MetaData), Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulseAtPosition_Statics::NewProp_Component_MetaData) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulseAtPosition_Statics::NewProp_Position = { "Position", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_AddImpulseAtPosition_Parms, Position), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulseAtPosition_Statics::NewProp_Impulse = { "Impulse", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_AddImpulseAtPosition_Parms, Impulse), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulseAtPosition_Statics::NewProp_BoneName = { "BoneName", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_AddImpulseAtPosition_Parms, BoneName), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulseAtPosition_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulseAtPosition_Statics::NewProp_Component,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulseAtPosition_Statics::NewProp_Position,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulseAtPosition_Statics::NewProp_Impulse,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulseAtPosition_Statics::NewProp_BoneName,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulseAtPosition_Statics::Function_MetaDataParams[] = {
		{ "Category", "ATP - AsyncTickPhysics" },
		{ "CPP_Default_BoneName", "None" },
		{ "ModuleRelativePath", "Public/AsyncTickFunctions.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulseAtPosition_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UAsyncTickFunctions, nullptr, "ATP_AddImpulseAtPosition", nullptr, nullptr, Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulseAtPosition_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulseAtPosition_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulseAtPosition_Statics::AsyncTickFunctions_eventATP_AddImpulseAtPosition_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04822401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulseAtPosition_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulseAtPosition_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulseAtPosition_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulseAtPosition_Statics::AsyncTickFunctions_eventATP_AddImpulseAtPosition_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulseAtPosition()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulseAtPosition_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque_Statics
	{
		struct AsyncTickFunctions_eventATP_AddTorque_Parms
		{
			UPrimitiveComponent* Component;
			FVector Torque;
			bool bAccelChange;
			FName BoneName;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
		static const UECodeGen_Private::FStructPropertyParams NewProp_Torque;
		static void NewProp_bAccelChange_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bAccelChange;
		static const UECodeGen_Private::FNamePropertyParams NewProp_BoneName;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque_Statics::NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_AddTorque_Parms, Component), Z_Construct_UClass_UPrimitiveComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque_Statics::NewProp_Component_MetaData), Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque_Statics::NewProp_Component_MetaData) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque_Statics::NewProp_Torque = { "Torque", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_AddTorque_Parms, Torque), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
	void Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque_Statics::NewProp_bAccelChange_SetBit(void* Obj)
	{
		((AsyncTickFunctions_eventATP_AddTorque_Parms*)Obj)->bAccelChange = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque_Statics::NewProp_bAccelChange = { "bAccelChange", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AsyncTickFunctions_eventATP_AddTorque_Parms), &Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque_Statics::NewProp_bAccelChange_SetBit, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque_Statics::NewProp_BoneName = { "BoneName", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_AddTorque_Parms, BoneName), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque_Statics::NewProp_Component,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque_Statics::NewProp_Torque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque_Statics::NewProp_bAccelChange,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque_Statics::NewProp_BoneName,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque_Statics::Function_MetaDataParams[] = {
		{ "Category", "ATP - AsyncTickPhysics" },
		{ "CPP_Default_BoneName", "None" },
		{ "ModuleRelativePath", "Public/AsyncTickFunctions.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UAsyncTickFunctions, nullptr, "ATP_AddTorque", nullptr, nullptr, Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque_Statics::AsyncTickFunctions_eventATP_AddTorque_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04822401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque_Statics::AsyncTickFunctions_eventATP_AddTorque_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetAngularVelocity_Statics
	{
		struct AsyncTickFunctions_eventATP_GetAngularVelocity_Parms
		{
			UPrimitiveComponent* Component;
			FName BoneName;
			FVector ReturnValue;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
		static const UECodeGen_Private::FNamePropertyParams NewProp_BoneName;
		static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetAngularVelocity_Statics::NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetAngularVelocity_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_GetAngularVelocity_Parms, Component), Z_Construct_UClass_UPrimitiveComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetAngularVelocity_Statics::NewProp_Component_MetaData), Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetAngularVelocity_Statics::NewProp_Component_MetaData) };
	const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetAngularVelocity_Statics::NewProp_BoneName = { "BoneName", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_GetAngularVelocity_Parms, BoneName), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetAngularVelocity_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_GetAngularVelocity_Parms, ReturnValue), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetAngularVelocity_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetAngularVelocity_Statics::NewProp_Component,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetAngularVelocity_Statics::NewProp_BoneName,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetAngularVelocity_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetAngularVelocity_Statics::Function_MetaDataParams[] = {
		{ "Category", "ATP - AsyncTickPhysics" },
		{ "CPP_Default_BoneName", "None" },
		{ "ModuleRelativePath", "Public/AsyncTickFunctions.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetAngularVelocity_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UAsyncTickFunctions, nullptr, "ATP_GetAngularVelocity", nullptr, nullptr, Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetAngularVelocity_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetAngularVelocity_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetAngularVelocity_Statics::AsyncTickFunctions_eventATP_GetAngularVelocity_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04822401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetAngularVelocity_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetAngularVelocity_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetAngularVelocity_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetAngularVelocity_Statics::AsyncTickFunctions_eventATP_GetAngularVelocity_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetAngularVelocity()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetAngularVelocity_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetCoMPosition_Statics
	{
		struct AsyncTickFunctions_eventATP_GetCoMPosition_Parms
		{
			UPrimitiveComponent* Component;
			FVector ReturnValue;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
		static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetCoMPosition_Statics::NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetCoMPosition_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_GetCoMPosition_Parms, Component), Z_Construct_UClass_UPrimitiveComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetCoMPosition_Statics::NewProp_Component_MetaData), Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetCoMPosition_Statics::NewProp_Component_MetaData) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetCoMPosition_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_GetCoMPosition_Parms, ReturnValue), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetCoMPosition_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetCoMPosition_Statics::NewProp_Component,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetCoMPosition_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetCoMPosition_Statics::Function_MetaDataParams[] = {
		{ "Category", "ATP - AsyncTickPhysics" },
		{ "ModuleRelativePath", "Public/AsyncTickFunctions.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetCoMPosition_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UAsyncTickFunctions, nullptr, "ATP_GetCoMPosition", nullptr, nullptr, Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetCoMPosition_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetCoMPosition_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetCoMPosition_Statics::AsyncTickFunctions_eventATP_GetCoMPosition_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04822401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetCoMPosition_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetCoMPosition_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetCoMPosition_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetCoMPosition_Statics::AsyncTickFunctions_eventATP_GetCoMPosition_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetCoMPosition()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetCoMPosition_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocity_Statics
	{
		struct AsyncTickFunctions_eventATP_GetLinearVelocity_Parms
		{
			UPrimitiveComponent* Component;
			FName BoneName;
			FVector ReturnValue;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
		static const UECodeGen_Private::FNamePropertyParams NewProp_BoneName;
		static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocity_Statics::NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocity_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_GetLinearVelocity_Parms, Component), Z_Construct_UClass_UPrimitiveComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocity_Statics::NewProp_Component_MetaData), Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocity_Statics::NewProp_Component_MetaData) };
	const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocity_Statics::NewProp_BoneName = { "BoneName", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_GetLinearVelocity_Parms, BoneName), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocity_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_GetLinearVelocity_Parms, ReturnValue), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocity_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocity_Statics::NewProp_Component,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocity_Statics::NewProp_BoneName,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocity_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocity_Statics::Function_MetaDataParams[] = {
		{ "Category", "ATP - AsyncTickPhysics" },
		{ "CPP_Default_BoneName", "None" },
		{ "ModuleRelativePath", "Public/AsyncTickFunctions.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocity_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UAsyncTickFunctions, nullptr, "ATP_GetLinearVelocity", nullptr, nullptr, Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocity_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocity_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocity_Statics::AsyncTickFunctions_eventATP_GetLinearVelocity_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04822401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocity_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocity_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocity_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocity_Statics::AsyncTickFunctions_eventATP_GetLinearVelocity_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocity()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocity_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocityAtPoint_Statics
	{
		struct AsyncTickFunctions_eventATP_GetLinearVelocityAtPoint_Parms
		{
			UPrimitiveComponent* Component;
			FVector Point;
			FName BoneName;
			FVector ReturnValue;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
		static const UECodeGen_Private::FStructPropertyParams NewProp_Point;
		static const UECodeGen_Private::FNamePropertyParams NewProp_BoneName;
		static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocityAtPoint_Statics::NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocityAtPoint_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_GetLinearVelocityAtPoint_Parms, Component), Z_Construct_UClass_UPrimitiveComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocityAtPoint_Statics::NewProp_Component_MetaData), Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocityAtPoint_Statics::NewProp_Component_MetaData) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocityAtPoint_Statics::NewProp_Point = { "Point", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_GetLinearVelocityAtPoint_Parms, Point), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocityAtPoint_Statics::NewProp_BoneName = { "BoneName", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_GetLinearVelocityAtPoint_Parms, BoneName), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocityAtPoint_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_GetLinearVelocityAtPoint_Parms, ReturnValue), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocityAtPoint_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocityAtPoint_Statics::NewProp_Component,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocityAtPoint_Statics::NewProp_Point,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocityAtPoint_Statics::NewProp_BoneName,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocityAtPoint_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocityAtPoint_Statics::Function_MetaDataParams[] = {
		{ "Category", "ATP - AsyncTickPhysics" },
		{ "CPP_Default_BoneName", "None" },
		{ "ModuleRelativePath", "Public/AsyncTickFunctions.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocityAtPoint_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UAsyncTickFunctions, nullptr, "ATP_GetLinearVelocityAtPoint", nullptr, nullptr, Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocityAtPoint_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocityAtPoint_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocityAtPoint_Statics::AsyncTickFunctions_eventATP_GetLinearVelocityAtPoint_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04822401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocityAtPoint_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocityAtPoint_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocityAtPoint_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocityAtPoint_Statics::AsyncTickFunctions_eventATP_GetLinearVelocityAtPoint_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocityAtPoint()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocityAtPoint_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetTransform_Statics
	{
		struct AsyncTickFunctions_eventATP_GetTransform_Parms
		{
			UPrimitiveComponent* Component;
			FTransform ReturnValue;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
		static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetTransform_Statics::NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetTransform_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_GetTransform_Parms, Component), Z_Construct_UClass_UPrimitiveComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetTransform_Statics::NewProp_Component_MetaData), Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetTransform_Statics::NewProp_Component_MetaData) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetTransform_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_GetTransform_Parms, ReturnValue), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetTransform_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetTransform_Statics::NewProp_Component,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetTransform_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetTransform_Statics::Function_MetaDataParams[] = {
		{ "Category", "ATP - AsyncTickPhysics" },
		{ "ModuleRelativePath", "Public/AsyncTickFunctions.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetTransform_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UAsyncTickFunctions, nullptr, "ATP_GetTransform", nullptr, nullptr, Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetTransform_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetTransform_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetTransform_Statics::AsyncTickFunctions_eventATP_GetTransform_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04822401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetTransform_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetTransform_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetTransform_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetTransform_Statics::AsyncTickFunctions_eventATP_GetTransform_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetTransform()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetTransform_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees_Statics
	{
		struct AsyncTickFunctions_eventATP_SetAngularVelocityInDegrees_Parms
		{
			UPrimitiveComponent* Component;
			FVector AngVelocity;
			bool bAddToCurrent;
			FName BoneName;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
		static const UECodeGen_Private::FStructPropertyParams NewProp_AngVelocity;
		static void NewProp_bAddToCurrent_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bAddToCurrent;
		static const UECodeGen_Private::FNamePropertyParams NewProp_BoneName;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees_Statics::NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_SetAngularVelocityInDegrees_Parms, Component), Z_Construct_UClass_UPrimitiveComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees_Statics::NewProp_Component_MetaData), Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees_Statics::NewProp_Component_MetaData) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees_Statics::NewProp_AngVelocity = { "AngVelocity", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_SetAngularVelocityInDegrees_Parms, AngVelocity), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
	void Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees_Statics::NewProp_bAddToCurrent_SetBit(void* Obj)
	{
		((AsyncTickFunctions_eventATP_SetAngularVelocityInDegrees_Parms*)Obj)->bAddToCurrent = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees_Statics::NewProp_bAddToCurrent = { "bAddToCurrent", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AsyncTickFunctions_eventATP_SetAngularVelocityInDegrees_Parms), &Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees_Statics::NewProp_bAddToCurrent_SetBit, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees_Statics::NewProp_BoneName = { "BoneName", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_SetAngularVelocityInDegrees_Parms, BoneName), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees_Statics::NewProp_Component,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees_Statics::NewProp_AngVelocity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees_Statics::NewProp_bAddToCurrent,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees_Statics::NewProp_BoneName,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees_Statics::Function_MetaDataParams[] = {
		{ "Category", "ATP - AsyncTickPhysics" },
		{ "CPP_Default_BoneName", "None" },
		{ "ModuleRelativePath", "Public/AsyncTickFunctions.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UAsyncTickFunctions, nullptr, "ATP_SetAngularVelocityInDegrees", nullptr, nullptr, Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees_Statics::AsyncTickFunctions_eventATP_SetAngularVelocityInDegrees_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04822401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees_Statics::AsyncTickFunctions_eventATP_SetAngularVelocityInDegrees_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians_Statics
	{
		struct AsyncTickFunctions_eventATP_SetAngularVelocityInRadians_Parms
		{
			UPrimitiveComponent* Component;
			FVector AngVelocity;
			bool bAddToCurrent;
			FName BoneName;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
		static const UECodeGen_Private::FStructPropertyParams NewProp_AngVelocity;
		static void NewProp_bAddToCurrent_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bAddToCurrent;
		static const UECodeGen_Private::FNamePropertyParams NewProp_BoneName;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians_Statics::NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_SetAngularVelocityInRadians_Parms, Component), Z_Construct_UClass_UPrimitiveComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians_Statics::NewProp_Component_MetaData), Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians_Statics::NewProp_Component_MetaData) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians_Statics::NewProp_AngVelocity = { "AngVelocity", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_SetAngularVelocityInRadians_Parms, AngVelocity), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
	void Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians_Statics::NewProp_bAddToCurrent_SetBit(void* Obj)
	{
		((AsyncTickFunctions_eventATP_SetAngularVelocityInRadians_Parms*)Obj)->bAddToCurrent = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians_Statics::NewProp_bAddToCurrent = { "bAddToCurrent", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AsyncTickFunctions_eventATP_SetAngularVelocityInRadians_Parms), &Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians_Statics::NewProp_bAddToCurrent_SetBit, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians_Statics::NewProp_BoneName = { "BoneName", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_SetAngularVelocityInRadians_Parms, BoneName), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians_Statics::NewProp_Component,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians_Statics::NewProp_AngVelocity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians_Statics::NewProp_bAddToCurrent,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians_Statics::NewProp_BoneName,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians_Statics::Function_MetaDataParams[] = {
		{ "Category", "ATP - AsyncTickPhysics" },
		{ "CPP_Default_BoneName", "None" },
		{ "ModuleRelativePath", "Public/AsyncTickFunctions.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UAsyncTickFunctions, nullptr, "ATP_SetAngularVelocityInRadians", nullptr, nullptr, Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians_Statics::AsyncTickFunctions_eventATP_SetAngularVelocityInRadians_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04822401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians_Statics::AsyncTickFunctions_eventATP_SetAngularVelocityInRadians_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity_Statics
	{
		struct AsyncTickFunctions_eventATP_SetLinearVelocity_Parms
		{
			UPrimitiveComponent* Component;
			FVector Velocity;
			bool bAddToCurrent;
			FName BoneName;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
		static const UECodeGen_Private::FStructPropertyParams NewProp_Velocity;
		static void NewProp_bAddToCurrent_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bAddToCurrent;
		static const UECodeGen_Private::FNamePropertyParams NewProp_BoneName;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity_Statics::NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_SetLinearVelocity_Parms, Component), Z_Construct_UClass_UPrimitiveComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity_Statics::NewProp_Component_MetaData), Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity_Statics::NewProp_Component_MetaData) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity_Statics::NewProp_Velocity = { "Velocity", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_SetLinearVelocity_Parms, Velocity), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
	void Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity_Statics::NewProp_bAddToCurrent_SetBit(void* Obj)
	{
		((AsyncTickFunctions_eventATP_SetLinearVelocity_Parms*)Obj)->bAddToCurrent = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity_Statics::NewProp_bAddToCurrent = { "bAddToCurrent", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(AsyncTickFunctions_eventATP_SetLinearVelocity_Parms), &Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity_Statics::NewProp_bAddToCurrent_SetBit, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity_Statics::NewProp_BoneName = { "BoneName", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_SetLinearVelocity_Parms, BoneName), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity_Statics::NewProp_Component,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity_Statics::NewProp_Velocity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity_Statics::NewProp_bAddToCurrent,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity_Statics::NewProp_BoneName,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity_Statics::Function_MetaDataParams[] = {
		{ "Category", "ATP - AsyncTickPhysics" },
		{ "CPP_Default_BoneName", "None" },
		{ "ModuleRelativePath", "Public/AsyncTickFunctions.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UAsyncTickFunctions, nullptr, "ATP_SetLinearVelocity", nullptr, nullptr, Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity_Statics::AsyncTickFunctions_eventATP_SetLinearVelocity_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04822401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity_Statics::AsyncTickFunctions_eventATP_SetLinearVelocity_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocation_Statics
	{
		struct AsyncTickFunctions_eventATP_SetWorldLocation_Parms
		{
			USceneComponent* Component;
			FVector Location;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
		static const UECodeGen_Private::FStructPropertyParams NewProp_Location;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocation_Statics::NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocation_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_SetWorldLocation_Parms, Component), Z_Construct_UClass_USceneComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocation_Statics::NewProp_Component_MetaData), Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocation_Statics::NewProp_Component_MetaData) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocation_Statics::NewProp_Location = { "Location", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_SetWorldLocation_Parms, Location), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocation_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocation_Statics::NewProp_Component,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocation_Statics::NewProp_Location,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocation_Statics::Function_MetaDataParams[] = {
		{ "Category", "ATP - AsyncTickPhysics" },
		{ "ModuleRelativePath", "Public/AsyncTickFunctions.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocation_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UAsyncTickFunctions, nullptr, "ATP_SetWorldLocation", nullptr, nullptr, Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocation_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocation_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocation_Statics::AsyncTickFunctions_eventATP_SetWorldLocation_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04822401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocation_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocation_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocation_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocation_Statics::AsyncTickFunctions_eventATP_SetWorldLocation_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocation()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocation_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocationAndRotation_Statics
	{
		struct AsyncTickFunctions_eventATP_SetWorldLocationAndRotation_Parms
		{
			UPrimitiveComponent* Component;
			FVector Location;
			FRotator Rotation;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
		static const UECodeGen_Private::FStructPropertyParams NewProp_Location;
		static const UECodeGen_Private::FStructPropertyParams NewProp_Rotation;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocationAndRotation_Statics::NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocationAndRotation_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_SetWorldLocationAndRotation_Parms, Component), Z_Construct_UClass_UPrimitiveComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocationAndRotation_Statics::NewProp_Component_MetaData), Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocationAndRotation_Statics::NewProp_Component_MetaData) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocationAndRotation_Statics::NewProp_Location = { "Location", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_SetWorldLocationAndRotation_Parms, Location), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocationAndRotation_Statics::NewProp_Rotation = { "Rotation", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_SetWorldLocationAndRotation_Parms, Rotation), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocationAndRotation_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocationAndRotation_Statics::NewProp_Component,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocationAndRotation_Statics::NewProp_Location,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocationAndRotation_Statics::NewProp_Rotation,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocationAndRotation_Statics::Function_MetaDataParams[] = {
		{ "Category", "ATP - AsyncTickPhysics" },
		{ "ModuleRelativePath", "Public/AsyncTickFunctions.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocationAndRotation_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UAsyncTickFunctions, nullptr, "ATP_SetWorldLocationAndRotation", nullptr, nullptr, Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocationAndRotation_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocationAndRotation_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocationAndRotation_Statics::AsyncTickFunctions_eventATP_SetWorldLocationAndRotation_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04822401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocationAndRotation_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocationAndRotation_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocationAndRotation_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocationAndRotation_Statics::AsyncTickFunctions_eventATP_SetWorldLocationAndRotation_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocationAndRotation()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocationAndRotation_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldRotation_Statics
	{
		struct AsyncTickFunctions_eventATP_SetWorldRotation_Parms
		{
			UPrimitiveComponent* Component;
			FRotator Rotation;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Component_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Component;
		static const UECodeGen_Private::FStructPropertyParams NewProp_Rotation;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldRotation_Statics::NewProp_Component_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldRotation_Statics::NewProp_Component = { "Component", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_SetWorldRotation_Parms, Component), Z_Construct_UClass_UPrimitiveComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldRotation_Statics::NewProp_Component_MetaData), Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldRotation_Statics::NewProp_Component_MetaData) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldRotation_Statics::NewProp_Rotation = { "Rotation", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickFunctions_eventATP_SetWorldRotation_Parms, Rotation), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldRotation_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldRotation_Statics::NewProp_Component,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldRotation_Statics::NewProp_Rotation,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldRotation_Statics::Function_MetaDataParams[] = {
		{ "Category", "ATP - AsyncTickPhysics" },
		{ "ModuleRelativePath", "Public/AsyncTickFunctions.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldRotation_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UAsyncTickFunctions, nullptr, "ATP_SetWorldRotation", nullptr, nullptr, Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldRotation_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldRotation_Statics::PropPointers), sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldRotation_Statics::AsyncTickFunctions_eventATP_SetWorldRotation_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04822401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldRotation_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldRotation_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldRotation_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldRotation_Statics::AsyncTickFunctions_eventATP_SetWorldRotation_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldRotation()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldRotation_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UAsyncTickFunctions);
	UClass* Z_Construct_UClass_UAsyncTickFunctions_NoRegister()
	{
		return UAsyncTickFunctions::StaticClass();
	}
	struct Z_Construct_UClass_UAsyncTickFunctions_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UAsyncTickFunctions_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UBlueprintFunctionLibrary,
		(UObject* (*)())Z_Construct_UPackage__Script_AsyncTickPhysics,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UAsyncTickFunctions_Statics::DependentSingletons) < 16);
	const FClassFunctionLinkInfo Z_Construct_UClass_UAsyncTickFunctions_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInDegrees, "ATP_AddAngularImpulseInDegrees" }, // 3240935524
		{ &Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddAngularImpulseInRadians, "ATP_AddAngularImpulseInRadians" }, // 1156408414
		{ &Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForce, "ATP_AddForce" }, // 2368583618
		{ &Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddForceAtPosition, "ATP_AddForceAtPosition" }, // 1828281649
		{ &Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulse, "ATP_AddImpulse" }, // 3750343825
		{ &Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddImpulseAtPosition, "ATP_AddImpulseAtPosition" }, // 83512915
		{ &Z_Construct_UFunction_UAsyncTickFunctions_ATP_AddTorque, "ATP_AddTorque" }, // 2375652481
		{ &Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetAngularVelocity, "ATP_GetAngularVelocity" }, // 2062982296
		{ &Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetCoMPosition, "ATP_GetCoMPosition" }, // 2498200289
		{ &Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocity, "ATP_GetLinearVelocity" }, // 1670008353
		{ &Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetLinearVelocityAtPoint, "ATP_GetLinearVelocityAtPoint" }, // 2005498533
		{ &Z_Construct_UFunction_UAsyncTickFunctions_ATP_GetTransform, "ATP_GetTransform" }, // 827251312
		{ &Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInDegrees, "ATP_SetAngularVelocityInDegrees" }, // 4083673307
		{ &Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetAngularVelocityInRadians, "ATP_SetAngularVelocityInRadians" }, // 955853458
		{ &Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetLinearVelocity, "ATP_SetLinearVelocity" }, // 1634095384
		{ &Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocation, "ATP_SetWorldLocation" }, // 3178742183
		{ &Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldLocationAndRotation, "ATP_SetWorldLocationAndRotation" }, // 1124236486
		{ &Z_Construct_UFunction_UAsyncTickFunctions_ATP_SetWorldRotation, "ATP_SetWorldRotation" }, // 70630254
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UAsyncTickFunctions_Statics::FuncInfo) < 2048);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UAsyncTickFunctions_Statics::Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "AsyncTickFunctions.h" },
		{ "ModuleRelativePath", "Public/AsyncTickFunctions.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UAsyncTickFunctions_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UAsyncTickFunctions>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UAsyncTickFunctions_Statics::ClassParams = {
		&UAsyncTickFunctions::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		0,
		0,
		0x001000A0u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UAsyncTickFunctions_Statics::Class_MetaDataParams), Z_Construct_UClass_UAsyncTickFunctions_Statics::Class_MetaDataParams)
	};
	UClass* Z_Construct_UClass_UAsyncTickFunctions()
	{
		if (!Z_Registration_Info_UClass_UAsyncTickFunctions.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UAsyncTickFunctions.OuterSingleton, Z_Construct_UClass_UAsyncTickFunctions_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UAsyncTickFunctions.OuterSingleton;
	}
	template<> ASYNCTICKPHYSICS_API UClass* StaticClass<UAsyncTickFunctions>()
	{
		return UAsyncTickFunctions::StaticClass();
	}
	UAsyncTickFunctions::UAsyncTickFunctions(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UAsyncTickFunctions);
	UAsyncTickFunctions::~UAsyncTickFunctions() {}
	struct Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_AsyncTickPhysics_Public_AsyncTickFunctions_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_AsyncTickPhysics_Public_AsyncTickFunctions_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UAsyncTickFunctions, UAsyncTickFunctions::StaticClass, TEXT("UAsyncTickFunctions"), &Z_Registration_Info_UClass_UAsyncTickFunctions, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UAsyncTickFunctions), 4158748261U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_AsyncTickPhysics_Public_AsyncTickFunctions_h_2480154778(TEXT("/Script/AsyncTickPhysics"),
		Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_AsyncTickPhysics_Public_AsyncTickFunctions_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_AsyncTickPhysics_Public_AsyncTickFunctions_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
