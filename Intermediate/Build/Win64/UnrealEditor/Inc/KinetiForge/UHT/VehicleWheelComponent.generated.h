// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "VehicleWheelComponent.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class AActor;
class UPrimitiveComponent;
class USceneComponent;
class UStaticMesh;
class UStaticMeshComponent;
class UVehicleWheelCoordinatorComponent;
struct FHitResult;
struct FVehicleSuspensionSimData;
struct FVehicleWheelSimData;
#ifdef KINETIFORGE_VehicleWheelComponent_generated_h
#error "VehicleWheelComponent.generated.h already included, missing '#pragma once' in VehicleWheelComponent.h"
#endif
#define KINETIFORGE_VehicleWheelComponent_generated_h

#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelComponent_h_18_SPARSE_DATA
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelComponent_h_18_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelComponent_h_18_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelComponent_h_18_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execAttachWheelHubMeshToSuspension); \
	DECLARE_FUNCTION(execUpdateSuspensionSpringAnim); \
	DECLARE_FUNCTION(execUpdateSuspensionArmAnim); \
	DECLARE_FUNCTION(execGetSkidMarkWorldTransform); \
	DECLARE_FUNCTION(execGetWheelHubComponent); \
	DECLARE_FUNCTION(execRefreshWheelMesh); \
	DECLARE_FUNCTION(execGetWheelAndBrakeMeshes); \
	DECLARE_FUNCTION(execGetWheelAndBrakeMeshComponents); \
	DECLARE_FUNCTION(execSetMesh); \
	DECLARE_FUNCTION(execDrawWheelForce); \
	DECLARE_FUNCTION(execDrawSuspensionForce); \
	DECLARE_FUNCTION(execDrawSuspension); \
	DECLARE_FUNCTION(execGetWheelTransform); \
	DECLARE_FUNCTION(execGetRayCastResult); \
	DECLARE_FUNCTION(execGetAttachedComponent); \
	DECLARE_FUNCTION(execGetSuspensionMovement); \
	DECLARE_FUNCTION(execGetWheelMovement); \
	DECLARE_FUNCTION(execGetP4MotorTorque); \
	DECLARE_FUNCTION(execSetP4MotorTorque); \
	DECLARE_FUNCTION(execGetAsyncCarbodyTransform); \
	DECLARE_FUNCTION(execComputeFeedBackTorque); \
	DECLARE_FUNCTION(execGetWheelCoordinator); \
	DECLARE_FUNCTION(execUpdatePhysics);


#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelComponent_h_18_ACCESSORS
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelComponent_h_18_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUVehicleWheelComponent(); \
	friend struct Z_Construct_UClass_UVehicleWheelComponent_Statics; \
public: \
	DECLARE_CLASS(UVehicleWheelComponent, USceneComponent, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/KinetiForge"), NO_API) \
	DECLARE_SERIALIZER(UVehicleWheelComponent)


#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelComponent_h_18_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UVehicleWheelComponent(UVehicleWheelComponent&&); \
	NO_API UVehicleWheelComponent(const UVehicleWheelComponent&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UVehicleWheelComponent); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UVehicleWheelComponent); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UVehicleWheelComponent) \
	NO_API virtual ~UVehicleWheelComponent();


#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelComponent_h_15_PROLOG
#define FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelComponent_h_18_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelComponent_h_18_SPARSE_DATA \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelComponent_h_18_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelComponent_h_18_EDITOR_ONLY_SPARSE_DATA_PROPERTY_ACCESSORS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelComponent_h_18_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelComponent_h_18_ACCESSORS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelComponent_h_18_INCLASS_NO_PURE_DECLS \
	FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelComponent_h_18_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> KINETIFORGE_API UClass* StaticClass<class UVehicleWheelComponent>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelComponent_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
