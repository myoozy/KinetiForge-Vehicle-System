// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AsyncTickPhysics/Public/AsyncTickActorComponent.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeAsyncTickActorComponent() {}
// Cross Module References
	ASYNCTICKPHYSICS_API UClass* Z_Construct_UClass_UAsyncTickActorComponent();
	ASYNCTICKPHYSICS_API UClass* Z_Construct_UClass_UAsyncTickActorComponent_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
	UPackage* Z_Construct_UPackage__Script_AsyncTickPhysics();
// End Cross Module References
	struct AsyncTickActorComponent_eventAsyncTick_Parms
	{
		float DeltaTime;
	};
	static FName NAME_UAsyncTickActorComponent_AsyncTick = FName(TEXT("AsyncTick"));
	void UAsyncTickActorComponent::AsyncTick(float DeltaTime)
	{
		AsyncTickActorComponent_eventAsyncTick_Parms Parms;
		Parms.DeltaTime=DeltaTime;
		ProcessEvent(FindFunctionChecked(NAME_UAsyncTickActorComponent_AsyncTick),&Parms);
	}
	void UAsyncTickActorComponent::StaticRegisterNativesUAsyncTickActorComponent()
	{
	}
	struct Z_Construct_UFunction_UAsyncTickActorComponent_AsyncTick_Statics
	{
		static const UECodeGen_Private::FFloatPropertyParams NewProp_DeltaTime;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UAsyncTickActorComponent_AsyncTick_Statics::NewProp_DeltaTime = { "DeltaTime", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(AsyncTickActorComponent_eventAsyncTick_Parms, DeltaTime), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UAsyncTickActorComponent_AsyncTick_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UAsyncTickActorComponent_AsyncTick_Statics::NewProp_DeltaTime,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UAsyncTickActorComponent_AsyncTick_Statics::Function_MetaDataParams[] = {
		{ "Category", "AsyncTick" },
		{ "ModuleRelativePath", "Public/AsyncTickActorComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UAsyncTickActorComponent_AsyncTick_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UAsyncTickActorComponent, nullptr, "AsyncTick", nullptr, nullptr, Z_Construct_UFunction_UAsyncTickActorComponent_AsyncTick_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickActorComponent_AsyncTick_Statics::PropPointers), sizeof(AsyncTickActorComponent_eventAsyncTick_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020800, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickActorComponent_AsyncTick_Statics::Function_MetaDataParams), Z_Construct_UFunction_UAsyncTickActorComponent_AsyncTick_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UAsyncTickActorComponent_AsyncTick_Statics::PropPointers) < 2048);
	static_assert(sizeof(AsyncTickActorComponent_eventAsyncTick_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UAsyncTickActorComponent_AsyncTick()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UAsyncTickActorComponent_AsyncTick_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UAsyncTickActorComponent);
	UClass* Z_Construct_UClass_UAsyncTickActorComponent_NoRegister()
	{
		return UAsyncTickActorComponent::StaticClass();
	}
	struct Z_Construct_UClass_UAsyncTickActorComponent_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UAsyncTickActorComponent_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UActorComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_AsyncTickPhysics,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UAsyncTickActorComponent_Statics::DependentSingletons) < 16);
	const FClassFunctionLinkInfo Z_Construct_UClass_UAsyncTickActorComponent_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UAsyncTickActorComponent_AsyncTick, "AsyncTick" }, // 1378161553
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UAsyncTickActorComponent_Statics::FuncInfo) < 2048);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UAsyncTickActorComponent_Statics::Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "AsyncTickActorComponent.h" },
		{ "ModuleRelativePath", "Public/AsyncTickActorComponent.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UAsyncTickActorComponent_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UAsyncTickActorComponent>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UAsyncTickActorComponent_Statics::ClassParams = {
		&UAsyncTickActorComponent::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		0,
		0,
		0x00B000A4u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UAsyncTickActorComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UAsyncTickActorComponent_Statics::Class_MetaDataParams)
	};
	UClass* Z_Construct_UClass_UAsyncTickActorComponent()
	{
		if (!Z_Registration_Info_UClass_UAsyncTickActorComponent.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UAsyncTickActorComponent.OuterSingleton, Z_Construct_UClass_UAsyncTickActorComponent_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UAsyncTickActorComponent.OuterSingleton;
	}
	template<> ASYNCTICKPHYSICS_API UClass* StaticClass<UAsyncTickActorComponent>()
	{
		return UAsyncTickActorComponent::StaticClass();
	}
	UAsyncTickActorComponent::UAsyncTickActorComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UAsyncTickActorComponent);
	UAsyncTickActorComponent::~UAsyncTickActorComponent() {}
	struct Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_AsyncTickPhysics_Public_AsyncTickActorComponent_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_AsyncTickPhysics_Public_AsyncTickActorComponent_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UAsyncTickActorComponent, UAsyncTickActorComponent::StaticClass, TEXT("UAsyncTickActorComponent"), &Z_Registration_Info_UClass_UAsyncTickActorComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UAsyncTickActorComponent), 440152275U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_AsyncTickPhysics_Public_AsyncTickActorComponent_h_3666805519(TEXT("/Script/AsyncTickPhysics"),
		Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_AsyncTickPhysics_Public_AsyncTickActorComponent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_AsyncTickPhysics_Public_AsyncTickActorComponent_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
