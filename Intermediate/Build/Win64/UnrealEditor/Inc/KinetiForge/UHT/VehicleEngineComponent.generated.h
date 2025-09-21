// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "VehicleEngineComponent.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
enum class EEngineState : uint8;
struct FVehicleEngineSimData;
#ifdef KINETIFORGE_VehicleEngineComponent_generated_h
#error "VehicleEngineComponent.generated.h already included, missing '#pragma once' in VehicleEngineComponent.h"
#endif
#define KINETIFORGE_VehicleEngineComponent_generated_h

#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_24_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FVehicleNaturallyAspiratedEngineConfig_Statics; \
	KINETIFORGE_API static class UScriptStruct* StaticStruct();


template<> KINETIFORGE_API UScriptStruct* StaticStruct<struct FVehicleNaturallyAspiratedEngineConfig>();

#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_50_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FVehicleEngineTurboConfig_Statics; \
	KINETIFORGE_API static class UScriptStruct* StaticStruct();


template<> KINETIFORGE_API UScriptStruct* StaticStruct<struct FVehicleEngineTurboConfig>();

#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_73_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FVehicleEngineSimData_Statics; \
	KINETIFORGE_API static class UScriptStruct* StaticStruct();


template<> KINETIFORGE_API UScriptStruct* StaticStruct<struct FVehicleEngineSimData>();

#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_115_SPARSE_DATA
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_115_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_115_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_115_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execShutVehicleEngine); \
	DECLARE_FUNCTION(execStartVehicleEngine); \
	DECLARE_FUNCTION(execInitialize); \
	DECLARE_FUNCTION(execGetP1MotorTorque); \
	DECLARE_FUNCTION(execSetP1MotorTorque); \
	DECLARE_FUNCTION(execGetEngineMovement); \
	DECLARE_FUNCTION(execUpdatePhysics);


#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_115_ACCESSORS
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_115_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUVehicleEngineComponent(); \
	friend struct Z_Construct_UClass_UVehicleEngineComponent_Statics; \
public: \
	DECLARE_CLASS(UVehicleEngineComponent, UActorComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/KinetiForge"), NO_API) \
	DECLARE_SERIALIZER(UVehicleEngineComponent)


#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_115_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UVehicleEngineComponent(UVehicleEngineComponent&&); \
	NO_API UVehicleEngineComponent(const UVehicleEngineComponent&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UVehicleEngineComponent); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UVehicleEngineComponent); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UVehicleEngineComponent) \
	NO_API virtual ~UVehicleEngineComponent();


#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_112_PROLOG
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_115_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_115_SPARSE_DATA \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_115_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_115_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_115_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_115_ACCESSORS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_115_INCLASS_NO_PURE_DECLS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h_115_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> KINETIFORGE_API UClass* StaticClass<class UVehicleEngineComponent>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleEngineComponent_h


#define FOREACH_ENUM_EENGINESTATE(op) \
	op(EEngineState::On) \
	op(EEngineState::Off) \
	op(EEngineState::Starting) \
	op(EEngineState::Shutting) 

enum class EEngineState : uint8;
template<> struct TIsUEnumClass<EEngineState> { enum { Value = true }; };
template<> KINETIFORGE_API UEnum* StaticEnum<EEngineState>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
