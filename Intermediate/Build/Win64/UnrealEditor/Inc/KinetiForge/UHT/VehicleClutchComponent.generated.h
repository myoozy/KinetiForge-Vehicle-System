// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "VehicleClutchComponent.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UVehicleEngineComponent;
struct FVehicleClutchSimData;
#ifdef KINETIFORGE_VehicleClutchComponent_generated_h
#error "VehicleClutchComponent.generated.h already included, missing '#pragma once' in VehicleClutchComponent.h"
#endif
#define KINETIFORGE_VehicleClutchComponent_generated_h

#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_21_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FVehicleClutchConfig_Statics; \
	KINETIFORGE_API static class UScriptStruct* StaticStruct();


template<> KINETIFORGE_API UScriptStruct* StaticStruct<struct FVehicleClutchConfig>();

#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_36_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FVehicleClutchSimData_Statics; \
	KINETIFORGE_API static class UScriptStruct* StaticStruct();


template<> KINETIFORGE_API UScriptStruct* StaticStruct<struct FVehicleClutchSimData>();

#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_79_SPARSE_DATA
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_79_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_79_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_79_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execGetClutchMovement); \
	DECLARE_FUNCTION(execGetCluchTorque); \
	DECLARE_FUNCTION(execUpdatePhysics);


#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_79_ACCESSORS
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_79_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUVehicleClutchComponent(); \
	friend struct Z_Construct_UClass_UVehicleClutchComponent_Statics; \
public: \
	DECLARE_CLASS(UVehicleClutchComponent, UActorComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/KinetiForge"), NO_API) \
	DECLARE_SERIALIZER(UVehicleClutchComponent)


#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_79_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UVehicleClutchComponent(UVehicleClutchComponent&&); \
	NO_API UVehicleClutchComponent(const UVehicleClutchComponent&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UVehicleClutchComponent); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UVehicleClutchComponent); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UVehicleClutchComponent) \
	NO_API virtual ~UVehicleClutchComponent();


#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_76_PROLOG
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_79_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_79_SPARSE_DATA \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_79_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_79_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_79_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_79_ACCESSORS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_79_INCLASS_NO_PURE_DECLS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h_79_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> KINETIFORGE_API UClass* StaticClass<class UVehicleClutchComponent>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleClutchComponent_h


#define FOREACH_ENUM_ECLUTCHSIMMODE(op) \
	op(EClutchSimMode::SpringModel) \
	op(EClutchSimMode::DampingModel) 

enum class EClutchSimMode : uint8;
template<> struct TIsUEnumClass<EClutchSimMode> { enum { Value = true }; };
template<> KINETIFORGE_API UEnum* StaticEnum<EClutchSimMode>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
