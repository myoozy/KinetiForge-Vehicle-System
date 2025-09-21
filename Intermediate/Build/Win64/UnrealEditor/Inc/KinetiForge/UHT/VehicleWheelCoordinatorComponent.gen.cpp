// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "KinetiForge/Public/VehicleWheelCoordinatorComponent.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeVehicleWheelCoordinatorComponent() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_UPrimitiveComponent_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_USceneComponent();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleAxleAssemblyComponent_NoRegister();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleDriveAssemblyComponent_NoRegister();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleWheelComponent_NoRegister();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleWheelCoordinatorComponent();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleWheelCoordinatorComponent_NoRegister();
	UPackage* Z_Construct_UPackage__Script_KinetiForge();
// End Cross Module References
	void UVehicleWheelCoordinatorComponent::StaticRegisterNativesUVehicleWheelCoordinatorComponent()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UVehicleWheelCoordinatorComponent);
	UClass* Z_Construct_UClass_UVehicleWheelCoordinatorComponent_NoRegister()
	{
		return UVehicleWheelCoordinatorComponent::StaticClass();
	}
	struct Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Carbody_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Carbody;
		static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_RegisteredWheels_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RegisteredWheels_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_RegisteredWheels;
		static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_RegisteredAxles_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RegisteredAxles_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_RegisteredAxles;
		static const UECodeGen_Private::FWeakObjectPropertyParams NewProp_RegisteredDriveAssemblies_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RegisteredDriveAssemblies_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_RegisteredDriveAssemblies;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_USceneComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::DependentSingletons) < 16);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Trigger PhysicsVolume" },
		{ "IncludePath", "VehicleWheelCoordinatorComponent.h" },
		{ "ModuleRelativePath", "Public/VehicleWheelCoordinatorComponent.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_Carbody_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/VehicleWheelCoordinatorComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_Carbody = { "Carbody", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleWheelCoordinatorComponent, Carbody), Z_Construct_UClass_UPrimitiveComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_Carbody_MetaData), Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_Carbody_MetaData) };
	const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_RegisteredWheels_Inner = { "RegisteredWheels", nullptr, (EPropertyFlags)0x0004000000080008, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UVehicleWheelComponent_NoRegister, METADATA_PARAMS(0, nullptr) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_RegisteredWheels_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcd\xb9\xef\xbf\xbd\xef\xbf\xbd\xd6\xb8\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleWheelCoordinatorComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcd\xb9\xef\xbf\xbd\xef\xbf\xbd\xd6\xb8\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_RegisteredWheels = { "RegisteredWheels", nullptr, (EPropertyFlags)0x0024088000000008, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleWheelCoordinatorComponent, RegisteredWheels), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_RegisteredWheels_MetaData), Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_RegisteredWheels_MetaData) };
	const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_RegisteredAxles_Inner = { "RegisteredAxles", nullptr, (EPropertyFlags)0x0004000000080008, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UVehicleAxleAssemblyComponent_NoRegister, METADATA_PARAMS(0, nullptr) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_RegisteredAxles_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "//if wheel position changed, or if wheel is added or destoryed \n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleWheelCoordinatorComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "if wheel position changed, or if wheel is added or destoryed" },
#endif
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_RegisteredAxles = { "RegisteredAxles", nullptr, (EPropertyFlags)0x0024088000000008, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleWheelCoordinatorComponent, RegisteredAxles), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_RegisteredAxles_MetaData), Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_RegisteredAxles_MetaData) };
	const UECodeGen_Private::FWeakObjectPropertyParams Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_RegisteredDriveAssemblies_Inner = { "RegisteredDriveAssemblies", nullptr, (EPropertyFlags)0x0004000000080008, UECodeGen_Private::EPropertyGenFlags::WeakObject, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UVehicleDriveAssemblyComponent_NoRegister, METADATA_PARAMS(0, nullptr) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_RegisteredDriveAssemblies_MetaData[] = {
		{ "ModuleRelativePath", "Public/VehicleWheelCoordinatorComponent.h" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_RegisteredDriveAssemblies = { "RegisteredDriveAssemblies", nullptr, (EPropertyFlags)0x0024088000000008, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleWheelCoordinatorComponent, RegisteredDriveAssemblies), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_RegisteredDriveAssemblies_MetaData), Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_RegisteredDriveAssemblies_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_Carbody,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_RegisteredWheels_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_RegisteredWheels,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_RegisteredAxles_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_RegisteredAxles,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_RegisteredDriveAssemblies_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::NewProp_RegisteredDriveAssemblies,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVehicleWheelCoordinatorComponent>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::ClassParams = {
		&UVehicleWheelCoordinatorComponent::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::PropPointers),
		0,
		0x00B000A4u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::Class_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::PropPointers) < 2048);
	UClass* Z_Construct_UClass_UVehicleWheelCoordinatorComponent()
	{
		if (!Z_Registration_Info_UClass_UVehicleWheelCoordinatorComponent.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVehicleWheelCoordinatorComponent.OuterSingleton, Z_Construct_UClass_UVehicleWheelCoordinatorComponent_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UVehicleWheelCoordinatorComponent.OuterSingleton;
	}
	template<> KINETIFORGE_API UClass* StaticClass<UVehicleWheelCoordinatorComponent>()
	{
		return UVehicleWheelCoordinatorComponent::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UVehicleWheelCoordinatorComponent);
	UVehicleWheelCoordinatorComponent::~UVehicleWheelCoordinatorComponent() {}
	struct Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelCoordinatorComponent_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelCoordinatorComponent_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UVehicleWheelCoordinatorComponent, UVehicleWheelCoordinatorComponent::StaticClass, TEXT("UVehicleWheelCoordinatorComponent"), &Z_Registration_Info_UClass_UVehicleWheelCoordinatorComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVehicleWheelCoordinatorComponent), 13207171U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelCoordinatorComponent_h_1538030167(TEXT("/Script/KinetiForge"),
		Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelCoordinatorComponent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelCoordinatorComponent_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
