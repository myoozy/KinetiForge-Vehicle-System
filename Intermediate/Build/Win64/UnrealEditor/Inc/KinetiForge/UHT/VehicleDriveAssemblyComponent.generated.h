// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "VehicleDriveAssemblyComponent.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class USceneComponent;
class UVehicleAxleAssemblyComponent;
class UVehicleClutchComponent;
class UVehicleEngineComponent;
class UVehicleGearboxComponent;
class UVehicleWheelComponent;
enum class EEngineState : uint8;
struct FVehicleInputValue;
#ifdef KINETIFORGE_VehicleDriveAssemblyComponent_generated_h
#error "VehicleDriveAssemblyComponent.generated.h already included, missing '#pragma once' in VehicleDriveAssemblyComponent.h"
#endif
#define KINETIFORGE_VehicleDriveAssemblyComponent_generated_h

#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_21_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FAxleAssemblyConfig_Statics; \
	KINETIFORGE_API static class UScriptStruct* StaticStruct();


template<> KINETIFORGE_API UScriptStruct* StaticStruct<struct FAxleAssemblyConfig>();

#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_54_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FVehiclInputInterpSpeed_Statics; \
	KINETIFORGE_API static class UScriptStruct* StaticStruct();


template<> KINETIFORGE_API UScriptStruct* StaticStruct<struct FVehiclInputInterpSpeed>();

#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_83_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FAutoGearboxConfig_Statics; \
	KINETIFORGE_API static class UScriptStruct* StaticStruct();


template<> KINETIFORGE_API UScriptStruct* StaticStruct<struct FAutoGearboxConfig>();

#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_122_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FVehicleInputValue_Statics; \
	KINETIFORGE_API static class UScriptStruct* StaticStruct();


template<> KINETIFORGE_API UScriptStruct* StaticStruct<struct FVehicleInputValue>();

#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_167_SPARSE_DATA
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_167_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_167_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_167_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execCalculateSpeedRangeOfEachGear); \
	DECLARE_FUNCTION(execGetVehicleSpeed); \
	DECLARE_FUNCTION(execDestroyTargetAxle); \
	DECLARE_FUNCTION(execGetWheels); \
	DECLARE_FUNCTION(execGetGearbox); \
	DECLARE_FUNCTION(execGetClutch); \
	DECLARE_FUNCTION(execGetEngine); \
	DECLARE_FUNCTION(execGetAxles); \
	DECLARE_FUNCTION(execGetInputValues); \
	DECLARE_FUNCTION(execUpdateDriftCamera); \
	DECLARE_FUNCTION(execRotateCamera); \
	DECLARE_FUNCTION(execInterpInputValue); \
	DECLARE_FUNCTION(execInterpInputValueConstant); \
	DECLARE_FUNCTION(execShutVehicleEngine); \
	DECLARE_FUNCTION(execStartVehicleEngine); \
	DECLARE_FUNCTION(execShiftDown); \
	DECLARE_FUNCTION(execShiftUp); \
	DECLARE_FUNCTION(execShiftToTargetGear); \
	DECLARE_FUNCTION(execInputHandbrake); \
	DECLARE_FUNCTION(execInputSteering); \
	DECLARE_FUNCTION(execInputClutch); \
	DECLARE_FUNCTION(execInputBrake); \
	DECLARE_FUNCTION(execInputThrottle); \
	DECLARE_FUNCTION(execUpdatePhysics);


#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_167_ACCESSORS
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_167_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUVehicleDriveAssemblyComponent(); \
	friend struct Z_Construct_UClass_UVehicleDriveAssemblyComponent_Statics; \
public: \
	DECLARE_CLASS(UVehicleDriveAssemblyComponent, USceneComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/KinetiForge"), NO_API) \
	DECLARE_SERIALIZER(UVehicleDriveAssemblyComponent)


#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_167_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UVehicleDriveAssemblyComponent(UVehicleDriveAssemblyComponent&&); \
	NO_API UVehicleDriveAssemblyComponent(const UVehicleDriveAssemblyComponent&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UVehicleDriveAssemblyComponent); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UVehicleDriveAssemblyComponent); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UVehicleDriveAssemblyComponent) \
	NO_API virtual ~UVehicleDriveAssemblyComponent();


#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_164_PROLOG
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_167_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_167_SPARSE_DATA \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_167_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_167_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_167_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_167_ACCESSORS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_167_INCLASS_NO_PURE_DECLS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h_167_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> KINETIFORGE_API UClass* StaticClass<class UVehicleDriveAssemblyComponent>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleDriveAssemblyComponent_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
