// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "VehicleGearboxComponent.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef KINETIFORGE_VehicleGearboxComponent_generated_h
#error "VehicleGearboxComponent.generated.h already included, missing '#pragma once' in VehicleGearboxComponent.h"
#endif
#define KINETIFORGE_VehicleGearboxComponent_generated_h

#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h_14_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FVehicleGearboxConfig_Statics; \
	KINETIFORGE_API static class UScriptStruct* StaticStruct();


template<> KINETIFORGE_API UScriptStruct* StaticStruct<struct FVehicleGearboxConfig>();

#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h_39_SPARSE_DATA
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h_39_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h_39_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h_39_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execCalculateGearRatios); \
	DECLARE_FUNCTION(execGetP2MotorTorque); \
	DECLARE_FUNCTION(execSetP2MotorTorque); \
	DECLARE_FUNCTION(execGetIsInGear); \
	DECLARE_FUNCTION(execGetCurrentGearRatio); \
	DECLARE_FUNCTION(execGetCurrentGear); \
	DECLARE_FUNCTION(execGetGearRatio); \
	DECLARE_FUNCTION(execUpdateInputShaft); \
	DECLARE_FUNCTION(execUpdateOutputShaft); \
	DECLARE_FUNCTION(execShiftToTargetGear); \
	DECLARE_FUNCTION(execFinalizeShift); \
	DECLARE_FUNCTION(execPrepareShift);


#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h_39_ACCESSORS
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h_39_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUVehicleGearboxComponent(); \
	friend struct Z_Construct_UClass_UVehicleGearboxComponent_Statics; \
public: \
	DECLARE_CLASS(UVehicleGearboxComponent, UActorComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/KinetiForge"), NO_API) \
	DECLARE_SERIALIZER(UVehicleGearboxComponent)


#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h_39_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UVehicleGearboxComponent(UVehicleGearboxComponent&&); \
	NO_API UVehicleGearboxComponent(const UVehicleGearboxComponent&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UVehicleGearboxComponent); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UVehicleGearboxComponent); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UVehicleGearboxComponent) \
	NO_API virtual ~UVehicleGearboxComponent();


#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h_36_PROLOG
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h_39_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h_39_SPARSE_DATA \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h_39_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h_39_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h_39_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h_39_ACCESSORS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h_39_INCLASS_NO_PURE_DECLS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h_39_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> KINETIFORGE_API UClass* StaticClass<class UVehicleGearboxComponent>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleGearboxComponent_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
