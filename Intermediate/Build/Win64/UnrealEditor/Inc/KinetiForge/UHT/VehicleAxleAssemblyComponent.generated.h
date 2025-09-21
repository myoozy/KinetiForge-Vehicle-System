// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "VehicleAxleAssemblyComponent.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UVehicleDifferentialComponent;
class UVehicleWheelComponent;
struct FVehicleAxleSimData;
#ifdef KINETIFORGE_VehicleAxleAssemblyComponent_generated_h
#error "VehicleAxleAssemblyComponent.generated.h already included, missing '#pragma once' in VehicleAxleAssemblyComponent.h"
#endif
#define KINETIFORGE_VehicleAxleAssemblyComponent_generated_h

#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleAxleAssemblyComponent_h_17_SPARSE_DATA
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleAxleAssemblyComponent_h_17_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleAxleAssemblyComponent_h_17_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleAxleAssemblyComponent_h_17_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execGetP3MotorTorque); \
	DECLARE_FUNCTION(execSetP3MotorTorque); \
	DECLARE_FUNCTION(execGetAxleMovement); \
	DECLARE_FUNCTION(execGetDifferential); \
	DECLARE_FUNCTION(execGetWheels); \
	DECLARE_FUNCTION(execUpdatePhysics);


#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleAxleAssemblyComponent_h_17_ACCESSORS
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleAxleAssemblyComponent_h_17_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUVehicleAxleAssemblyComponent(); \
	friend struct Z_Construct_UClass_UVehicleAxleAssemblyComponent_Statics; \
public: \
	DECLARE_CLASS(UVehicleAxleAssemblyComponent, USceneComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/KinetiForge"), NO_API) \
	DECLARE_SERIALIZER(UVehicleAxleAssemblyComponent)


#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleAxleAssemblyComponent_h_17_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UVehicleAxleAssemblyComponent(UVehicleAxleAssemblyComponent&&); \
	NO_API UVehicleAxleAssemblyComponent(const UVehicleAxleAssemblyComponent&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UVehicleAxleAssemblyComponent); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UVehicleAxleAssemblyComponent); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UVehicleAxleAssemblyComponent) \
	NO_API virtual ~UVehicleAxleAssemblyComponent();


#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleAxleAssemblyComponent_h_14_PROLOG
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleAxleAssemblyComponent_h_17_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleAxleAssemblyComponent_h_17_SPARSE_DATA \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleAxleAssemblyComponent_h_17_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleAxleAssemblyComponent_h_17_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleAxleAssemblyComponent_h_17_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleAxleAssemblyComponent_h_17_ACCESSORS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleAxleAssemblyComponent_h_17_INCLASS_NO_PURE_DECLS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleAxleAssemblyComponent_h_17_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> KINETIFORGE_API UClass* StaticClass<class UVehicleAxleAssemblyComponent>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleAxleAssemblyComponent_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
