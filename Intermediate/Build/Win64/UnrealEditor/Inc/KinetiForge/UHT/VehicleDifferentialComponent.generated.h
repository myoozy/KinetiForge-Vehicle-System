// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "VehicleDifferentialComponent.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef KINETIFORGE_VehicleDifferentialComponent_generated_h
#error "VehicleDifferentialComponent.generated.h already included, missing '#pragma once' in VehicleDifferentialComponent.h"
#endif
#define KINETIFORGE_VehicleDifferentialComponent_generated_h

#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h_12_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FVehicleLimitedSlipDifferentialConfig_Statics; \
	KINETIFORGE_API static class UScriptStruct* StaticStruct();


template<> KINETIFORGE_API UScriptStruct* StaticStruct<struct FVehicleLimitedSlipDifferentialConfig>();

#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h_23_SPARSE_DATA
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h_23_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h_23_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h_23_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execGetInputShaftVelocity); \
	DECLARE_FUNCTION(execGetOpenDiffOutputTorque); \
	DECLARE_FUNCTION(execGetOutputTorque); \
	DECLARE_FUNCTION(execUpdateInputShaft); \
	DECLARE_FUNCTION(execUpdateOutputShaft);


#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h_23_ACCESSORS
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h_23_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUVehicleDifferentialComponent(); \
	friend struct Z_Construct_UClass_UVehicleDifferentialComponent_Statics; \
public: \
	DECLARE_CLASS(UVehicleDifferentialComponent, UActorComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/KinetiForge"), NO_API) \
	DECLARE_SERIALIZER(UVehicleDifferentialComponent)


#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h_23_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UVehicleDifferentialComponent(UVehicleDifferentialComponent&&); \
	NO_API UVehicleDifferentialComponent(const UVehicleDifferentialComponent&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UVehicleDifferentialComponent); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UVehicleDifferentialComponent); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UVehicleDifferentialComponent) \
	NO_API virtual ~UVehicleDifferentialComponent();


#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h_20_PROLOG
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h_23_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h_23_SPARSE_DATA \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h_23_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h_23_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h_23_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h_23_ACCESSORS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h_23_INCLASS_NO_PURE_DECLS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h_23_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> KINETIFORGE_API UClass* StaticClass<class UVehicleDifferentialComponent>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDifferentialComponent_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
