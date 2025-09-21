// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AsyncTickPhysics/Public/AsyncTickPawn.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeAsyncTickPawn() {}
// Cross Module References
	ASYNCTICKPHYSICS_API UClass* Z_Construct_UClass_AAsyncTickPawn();
	ASYNCTICKPHYSICS_API UClass* Z_Construct_UClass_AAsyncTickPawn_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_APawn();
	UPackage* Z_Construct_UPackage__Script_AsyncTickPhysics();
// End Cross Module References
	struct AsyncTickPawn_eventAsyncTick_Parms
	{
		float DeltaTime;
	};
	static FName NAME_AAsyncTickPawn_AsyncTick = FName(TEXT("AsyncTick"));
	void AAsyncTickPawn::AsyncTick(float DeltaTime)
	{
		AsyncTickPawn_eventAsyncTick_Parms Parms;
		Parms.DeltaTime=DeltaTime;
		ProcessEvent(FindFunctionChecked(NAME_AAsyncTickPawn_AsyncTick),&Parms);
	}
	void AAsyncTickPawn::StaticRegisterNativesAAsyncTickPawn()
	{
	}
	struct Z_Construct_UFunction_AAsyncTickPawn_AsyncTick_Statics
	{
		static const UECodeGen_Private::FFloatPropertyParams NewProp_DeltaTime;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_AAsyncTickPawn_AsyncTick_Statics::NewProp_DeltaTime = { "DeltaTime", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickPawn_eventAsyncTick_Parms, DeltaTime), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AAsyncTickPawn_AsyncTick_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AAsyncTickPawn_AsyncTick_Statics::NewProp_DeltaTime,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_AAsyncTickPawn_AsyncTick_Statics::Function_MetaDataParams[] = {
		{ "Category", "AsyncTick" },
		{ "ModuleRelativePath", "Public/AsyncTickPawn.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_AAsyncTickPawn_AsyncTick_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_AAsyncTickPawn, nullptr, "AsyncTick", nullptr, nullptr, Z_Construct_UFunction_AAsyncTickPawn_AsyncTick_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_AAsyncTickPawn_AsyncTick_Statics::PropPointers), sizeof(AsyncTickPawn_eventAsyncTick_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020800, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_AAsyncTickPawn_AsyncTick_Statics::Function_MetaDataParams), Z_Construct_UFunction_AAsyncTickPawn_AsyncTick_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_AAsyncTickPawn_AsyncTick_Statics::PropPointers) < 2048);
	static_assert(sizeof(AsyncTickPawn_eventAsyncTick_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_AAsyncTickPawn_AsyncTick()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_AAsyncTickPawn_AsyncTick_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AAsyncTickPawn);
	UClass* Z_Construct_UClass_AAsyncTickPawn_NoRegister()
	{
		return AAsyncTickPawn::StaticClass();
	}
	struct Z_Construct_UClass_AAsyncTickPawn_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AAsyncTickPawn_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_APawn,
		(UObject* (*)())Z_Construct_UPackage__Script_AsyncTickPhysics,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AAsyncTickPawn_Statics::DependentSingletons) < 16);
	const FClassFunctionLinkInfo Z_Construct_UClass_AAsyncTickPawn_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_AAsyncTickPawn_AsyncTick, "AsyncTick" }, // 3515983661
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AAsyncTickPawn_Statics::FuncInfo) < 2048);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AAsyncTickPawn_Statics::Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "AsyncTickPawn.h" },
		{ "ModuleRelativePath", "Public/AsyncTickPawn.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AAsyncTickPawn_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AAsyncTickPawn>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AAsyncTickPawn_Statics::ClassParams = {
		&AAsyncTickPawn::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		0,
		0,
		0x009000A4u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AAsyncTickPawn_Statics::Class_MetaDataParams), Z_Construct_UClass_AAsyncTickPawn_Statics::Class_MetaDataParams)
	};
	UClass* Z_Construct_UClass_AAsyncTickPawn()
	{
		if (!Z_Registration_Info_UClass_AAsyncTickPawn.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AAsyncTickPawn.OuterSingleton, Z_Construct_UClass_AAsyncTickPawn_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AAsyncTickPawn.OuterSingleton;
	}
	template<> ASYNCTICKPHYSICS_API UClass* StaticClass<AAsyncTickPawn>()
	{
		return AAsyncTickPawn::StaticClass();
	}
	AAsyncTickPawn::AAsyncTickPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AAsyncTickPawn);
	AAsyncTickPawn::~AAsyncTickPawn() {}
	struct Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_AsyncTickPhysics_Public_AsyncTickPawn_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_AsyncTickPhysics_Public_AsyncTickPawn_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AAsyncTickPawn, AAsyncTickPawn::StaticClass, TEXT("AAsyncTickPawn"), &Z_Registration_Info_UClass_AAsyncTickPawn, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AAsyncTickPawn), 4222810048U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_AsyncTickPhysics_Public_AsyncTickPawn_h_2070628234(TEXT("/Script/AsyncTickPhysics"),
		Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_AsyncTickPhysics_Public_AsyncTickPawn_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_AsyncTickPhysics_Public_AsyncTickPawn_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
