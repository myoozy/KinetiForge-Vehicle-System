// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "KinetiForge/Public/VehicleWheelComponent.h"
#include "../../Source/Runtime/Engine/Classes/Engine/HitResult.h"
#include "KinetiForge/Public/VehicleChassisParams.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeVehicleWheelComponent() {}
// Cross Module References
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FQuat();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FRotator();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FTransform();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector2D();
	ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UPrimitiveComponent_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_USceneComponent();
	ENGINE_API UClass* Z_Construct_UClass_USceneComponent_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UStaticMesh_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UStaticMeshComponent_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UWorld_NoRegister();
	ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FHitResult();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleWheelComponent();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleWheelComponent_NoRegister();
	KINETIFORGE_API UClass* Z_Construct_UClass_UVehicleWheelCoordinatorComponent_NoRegister();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleABSConfig();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleSuspensionSimData();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleTireConfig();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleWheelConfig();
	KINETIFORGE_API UScriptStruct* Z_Construct_UScriptStruct_FVehicleWheelSimData();
	UPackage* Z_Construct_UPackage__Script_KinetiForge();
// End Cross Module References
	DEFINE_FUNCTION(UVehicleWheelComponent::execAttachWheelHubMeshToSuspension)
	{
		P_GET_OBJECT(USceneComponent,Z_Param_InWheelHub);
		P_GET_STRUCT(FTransform,Z_Param_InTransform);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->AttachWheelHubMeshToSuspension(Z_Param_InWheelHub,Z_Param_InTransform);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleWheelComponent::execUpdateSuspensionSpringAnim)
	{
		P_GET_OBJECT(USceneComponent,Z_Param_InSpringMesh);
		P_GET_STRUCT(FVector,Z_Param_InScaleAxis);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InOffsetAlongArm);
		P_GET_STRUCT(FVector2D,Z_Param_InBallJointOffset);
		P_GET_STRUCT(FRotator,Z_Param_InRotationOffset);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InLengthBias);
		P_GET_STRUCT(FVector,Z_Param_InInitialScale);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FTransform*)Z_Param__Result=P_THIS->UpdateSuspensionSpringAnim(Z_Param_InSpringMesh,Z_Param_InScaleAxis,Z_Param_InOffsetAlongArm,Z_Param_InBallJointOffset,Z_Param_InRotationOffset,Z_Param_InLengthBias,Z_Param_InInitialScale);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleWheelComponent::execUpdateSuspensionArmAnim)
	{
		P_GET_OBJECT(USceneComponent,Z_Param_InArmMesh);
		P_GET_STRUCT(FRotator,Z_Param_InRotationOffset);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FQuat*)Z_Param__Result=P_THIS->UpdateSuspensionArmAnim(Z_Param_InArmMesh,Z_Param_InRotationOffset);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleWheelComponent::execGetSkidMarkWorldTransform)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_InSkidMarkBias);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InSkidMarkScale);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FTransform*)Z_Param__Result=P_THIS->GetSkidMarkWorldTransform(Z_Param_InSkidMarkBias,Z_Param_InSkidMarkScale);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleWheelComponent::execGetWheelHubComponent)
	{
		P_GET_OBJECT_REF(USceneComponent,Z_Param_Out_OutHub);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->GetWheelHubComponent(Z_Param_Out_OutHub);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleWheelComponent::execRefreshWheelMesh)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(bool*)Z_Param__Result=P_THIS->RefreshWheelMesh();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleWheelComponent::execGetWheelAndBrakeMeshes)
	{
		P_GET_OBJECT_REF(UStaticMesh,Z_Param_Out_OutWheelMesh);
		P_GET_OBJECT_REF(UStaticMesh,Z_Param_Out_OutBrakeMesh);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->GetWheelAndBrakeMeshes(Z_Param_Out_OutWheelMesh,Z_Param_Out_OutBrakeMesh);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleWheelComponent::execGetWheelAndBrakeMeshComponents)
	{
		P_GET_OBJECT_REF(UStaticMeshComponent,Z_Param_Out_OutWheel);
		P_GET_OBJECT_REF(UStaticMeshComponent,Z_Param_Out_OutBrake);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->GetWheelAndBrakeMeshComponents(Z_Param_Out_OutWheel,Z_Param_Out_OutBrake);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleWheelComponent::execSetMesh)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_SteeringAxleOffset);
		P_GET_OBJECT(UStaticMesh,Z_Param_NewWheelMesh);
		P_GET_STRUCT(FTransform,Z_Param_WheelMeshRelatvieTransform);
		P_GET_OBJECT(UStaticMesh,Z_Param_NewBrakeMesh);
		P_GET_STRUCT(FTransform,Z_Param_BrakeMeshRelativeTransform);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(bool*)Z_Param__Result=P_THIS->SetMesh(Z_Param_SteeringAxleOffset,Z_Param_NewWheelMesh,Z_Param_WheelMeshRelatvieTransform,Z_Param_NewBrakeMesh,Z_Param_BrakeMeshRelativeTransform);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleWheelComponent::execDrawWheelForce)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_Duration);
		P_GET_PROPERTY(FFloatProperty,Z_Param_Thickness);
		P_GET_PROPERTY(FFloatProperty,Z_Param_Length);
		P_GET_UBOOL(Z_Param_bDrawVelocity);
		P_GET_UBOOL(Z_Param_bDrawSlip);
		P_GET_UBOOL(Z_Param_bDrawInertia);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->DrawWheelForce(Z_Param_Duration,Z_Param_Thickness,Z_Param_Length,Z_Param_bDrawVelocity,Z_Param_bDrawSlip,Z_Param_bDrawInertia);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleWheelComponent::execDrawSuspensionForce)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_Duration);
		P_GET_PROPERTY(FFloatProperty,Z_Param_Thickness);
		P_GET_PROPERTY(FFloatProperty,Z_Param_Length);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->DrawSuspensionForce(Z_Param_Duration,Z_Param_Thickness,Z_Param_Length);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleWheelComponent::execDrawSuspension)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_Duration);
		P_GET_PROPERTY(FFloatProperty,Z_Param_Thickness);
		P_GET_UBOOL(Z_Param_bDrawSuspension);
		P_GET_UBOOL(Z_Param_bDrawWheel);
		P_GET_UBOOL(Z_Param_bDrawRayCast);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->DrawSuspension(Z_Param_Duration,Z_Param_Thickness,Z_Param_bDrawSuspension,Z_Param_bDrawWheel,Z_Param_bDrawRayCast);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleWheelComponent::execGetWheelTransform)
	{
		P_GET_STRUCT_REF(FTransform,Z_Param_Out_OutComponentRelativeTransform);
		P_GET_STRUCT_REF(FTransform,Z_Param_Out_OutRaycastTransform);
		P_GET_STRUCT_REF(FTransform,Z_Param_Out_OutParentTransform);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->GetWheelTransform(Z_Param_Out_OutComponentRelativeTransform,Z_Param_Out_OutRaycastTransform,Z_Param_Out_OutParentTransform);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleWheelComponent::execGetRayCastResult)
	{
		P_GET_STRUCT_REF(FHitResult,Z_Param_Out_OutHitResult);
		P_GET_UBOOL_REF(Z_Param_Out_OutRevised);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(bool*)Z_Param__Result=P_THIS->GetRayCastResult(Z_Param_Out_OutHitResult,Z_Param_Out_OutRevised);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleWheelComponent::execGetAttachedComponent)
	{
		P_GET_OBJECT_REF(AActor,Z_Param_Out_OutParentActor);
		P_GET_OBJECT_REF(UPrimitiveComponent,Z_Param_Out_OutParentComponent);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(bool*)Z_Param__Result=P_THIS->GetAttachedComponent(Z_Param_Out_OutParentActor,Z_Param_Out_OutParentComponent);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleWheelComponent::execGetSuspensionMovement)
	{
		P_GET_STRUCT_REF(FVehicleSuspensionSimData,Z_Param_Out_Out);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->GetSuspensionMovement(Z_Param_Out_Out);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleWheelComponent::execGetWheelMovement)
	{
		P_GET_STRUCT_REF(FVehicleWheelSimData,Z_Param_Out_Out);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->GetWheelMovement(Z_Param_Out_Out);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleWheelComponent::execGetP4MotorTorque)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=P_THIS->GetP4MotorTorque();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleWheelComponent::execSetP4MotorTorque)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_NewTorque);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->SetP4MotorTorque(Z_Param_NewTorque);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleWheelComponent::execGetAsyncCarbodyTransform)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(FTransform*)Z_Param__Result=P_THIS->GetAsyncCarbodyTransform();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleWheelComponent::execComputeFeedBackTorque)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(float*)Z_Param__Result=P_THIS->ComputeFeedBackTorque();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleWheelComponent::execGetWheelCoordinator)
	{
		P_GET_OBJECT_REF(UVehicleWheelCoordinatorComponent,Z_Param_Out_OutWheelCoordinator);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->GetWheelCoordinator(Z_Param_Out_OutWheelCoordinator);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UVehicleWheelComponent::execUpdatePhysics)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_InPhysicsDeltaTime);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InDriveTorque);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InBrakeTorque);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InHandbrakeTorque);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InSteeringAngle);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InSwaybarForce);
		P_GET_PROPERTY(FFloatProperty,Z_Param_InReflectedInertia);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->UpdatePhysics(Z_Param_InPhysicsDeltaTime,Z_Param_InDriveTorque,Z_Param_InBrakeTorque,Z_Param_InHandbrakeTorque,Z_Param_InSteeringAngle,Z_Param_InSwaybarForce,Z_Param_InReflectedInertia);
		P_NATIVE_END;
	}
	void UVehicleWheelComponent::StaticRegisterNativesUVehicleWheelComponent()
	{
		UClass* Class = UVehicleWheelComponent::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "AttachWheelHubMeshToSuspension", &UVehicleWheelComponent::execAttachWheelHubMeshToSuspension },
			{ "ComputeFeedBackTorque", &UVehicleWheelComponent::execComputeFeedBackTorque },
			{ "DrawSuspension", &UVehicleWheelComponent::execDrawSuspension },
			{ "DrawSuspensionForce", &UVehicleWheelComponent::execDrawSuspensionForce },
			{ "DrawWheelForce", &UVehicleWheelComponent::execDrawWheelForce },
			{ "GetAsyncCarbodyTransform", &UVehicleWheelComponent::execGetAsyncCarbodyTransform },
			{ "GetAttachedComponent", &UVehicleWheelComponent::execGetAttachedComponent },
			{ "GetP4MotorTorque", &UVehicleWheelComponent::execGetP4MotorTorque },
			{ "GetRayCastResult", &UVehicleWheelComponent::execGetRayCastResult },
			{ "GetSkidMarkWorldTransform", &UVehicleWheelComponent::execGetSkidMarkWorldTransform },
			{ "GetSuspensionMovement", &UVehicleWheelComponent::execGetSuspensionMovement },
			{ "GetWheelAndBrakeMeshComponents", &UVehicleWheelComponent::execGetWheelAndBrakeMeshComponents },
			{ "GetWheelAndBrakeMeshes", &UVehicleWheelComponent::execGetWheelAndBrakeMeshes },
			{ "GetWheelCoordinator", &UVehicleWheelComponent::execGetWheelCoordinator },
			{ "GetWheelHubComponent", &UVehicleWheelComponent::execGetWheelHubComponent },
			{ "GetWheelMovement", &UVehicleWheelComponent::execGetWheelMovement },
			{ "GetWheelTransform", &UVehicleWheelComponent::execGetWheelTransform },
			{ "RefreshWheelMesh", &UVehicleWheelComponent::execRefreshWheelMesh },
			{ "SetMesh", &UVehicleWheelComponent::execSetMesh },
			{ "SetP4MotorTorque", &UVehicleWheelComponent::execSetP4MotorTorque },
			{ "UpdatePhysics", &UVehicleWheelComponent::execUpdatePhysics },
			{ "UpdateSuspensionArmAnim", &UVehicleWheelComponent::execUpdateSuspensionArmAnim },
			{ "UpdateSuspensionSpringAnim", &UVehicleWheelComponent::execUpdateSuspensionSpringAnim },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UVehicleWheelComponent_AttachWheelHubMeshToSuspension_Statics
	{
		struct VehicleWheelComponent_eventAttachWheelHubMeshToSuspension_Parms
		{
			USceneComponent* InWheelHub;
			FTransform InTransform;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_InWheelHub_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_InWheelHub;
		static const UECodeGen_Private::FStructPropertyParams NewProp_InTransform;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_AttachWheelHubMeshToSuspension_Statics::NewProp_InWheelHub_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_AttachWheelHubMeshToSuspension_Statics::NewProp_InWheelHub = { "InWheelHub", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventAttachWheelHubMeshToSuspension_Parms, InWheelHub), Z_Construct_UClass_USceneComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_AttachWheelHubMeshToSuspension_Statics::NewProp_InWheelHub_MetaData), Z_Construct_UFunction_UVehicleWheelComponent_AttachWheelHubMeshToSuspension_Statics::NewProp_InWheelHub_MetaData) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_AttachWheelHubMeshToSuspension_Statics::NewProp_InTransform = { "InTransform", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventAttachWheelHubMeshToSuspension_Parms, InTransform), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleWheelComponent_AttachWheelHubMeshToSuspension_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_AttachWheelHubMeshToSuspension_Statics::NewProp_InWheelHub,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_AttachWheelHubMeshToSuspension_Statics::NewProp_InTransform,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_AttachWheelHubMeshToSuspension_Statics::Function_MetaDataParams[] = {
		{ "Category", "Animation" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleWheelComponent_AttachWheelHubMeshToSuspension_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleWheelComponent, nullptr, "AttachWheelHubMeshToSuspension", nullptr, nullptr, Z_Construct_UFunction_UVehicleWheelComponent_AttachWheelHubMeshToSuspension_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_AttachWheelHubMeshToSuspension_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleWheelComponent_AttachWheelHubMeshToSuspension_Statics::VehicleWheelComponent_eventAttachWheelHubMeshToSuspension_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_AttachWheelHubMeshToSuspension_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleWheelComponent_AttachWheelHubMeshToSuspension_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_AttachWheelHubMeshToSuspension_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleWheelComponent_AttachWheelHubMeshToSuspension_Statics::VehicleWheelComponent_eventAttachWheelHubMeshToSuspension_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleWheelComponent_AttachWheelHubMeshToSuspension()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleWheelComponent_AttachWheelHubMeshToSuspension_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleWheelComponent_ComputeFeedBackTorque_Statics
	{
		struct VehicleWheelComponent_eventComputeFeedBackTorque_Parms
		{
			float ReturnValue;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_ComputeFeedBackTorque_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventComputeFeedBackTorque_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleWheelComponent_ComputeFeedBackTorque_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_ComputeFeedBackTorque_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_ComputeFeedBackTorque_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleWheelComponent_ComputeFeedBackTorque_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleWheelComponent, nullptr, "ComputeFeedBackTorque", nullptr, nullptr, Z_Construct_UFunction_UVehicleWheelComponent_ComputeFeedBackTorque_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_ComputeFeedBackTorque_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleWheelComponent_ComputeFeedBackTorque_Statics::VehicleWheelComponent_eventComputeFeedBackTorque_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_ComputeFeedBackTorque_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleWheelComponent_ComputeFeedBackTorque_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_ComputeFeedBackTorque_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleWheelComponent_ComputeFeedBackTorque_Statics::VehicleWheelComponent_eventComputeFeedBackTorque_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleWheelComponent_ComputeFeedBackTorque()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleWheelComponent_ComputeFeedBackTorque_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics
	{
		struct VehicleWheelComponent_eventDrawSuspension_Parms
		{
			float Duration;
			float Thickness;
			bool bDrawSuspension;
			bool bDrawWheel;
			bool bDrawRayCast;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_Duration;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_Thickness;
		static void NewProp_bDrawSuspension_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bDrawSuspension;
		static void NewProp_bDrawWheel_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bDrawWheel;
		static void NewProp_bDrawRayCast_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bDrawRayCast;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::NewProp_Duration = { "Duration", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventDrawSuspension_Parms, Duration), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::NewProp_Thickness = { "Thickness", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventDrawSuspension_Parms, Thickness), METADATA_PARAMS(0, nullptr) };
	void Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::NewProp_bDrawSuspension_SetBit(void* Obj)
	{
		((VehicleWheelComponent_eventDrawSuspension_Parms*)Obj)->bDrawSuspension = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::NewProp_bDrawSuspension = { "bDrawSuspension", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VehicleWheelComponent_eventDrawSuspension_Parms), &Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::NewProp_bDrawSuspension_SetBit, METADATA_PARAMS(0, nullptr) };
	void Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::NewProp_bDrawWheel_SetBit(void* Obj)
	{
		((VehicleWheelComponent_eventDrawSuspension_Parms*)Obj)->bDrawWheel = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::NewProp_bDrawWheel = { "bDrawWheel", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VehicleWheelComponent_eventDrawSuspension_Parms), &Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::NewProp_bDrawWheel_SetBit, METADATA_PARAMS(0, nullptr) };
	void Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::NewProp_bDrawRayCast_SetBit(void* Obj)
	{
		((VehicleWheelComponent_eventDrawSuspension_Parms*)Obj)->bDrawRayCast = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::NewProp_bDrawRayCast = { "bDrawRayCast", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VehicleWheelComponent_eventDrawSuspension_Parms), &Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::NewProp_bDrawRayCast_SetBit, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::NewProp_Duration,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::NewProp_Thickness,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::NewProp_bDrawSuspension,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::NewProp_bDrawWheel,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::NewProp_bDrawRayCast,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::Function_MetaDataParams[] = {
		{ "Category", "Debug" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//debug draw\n" },
#endif
		{ "CPP_Default_bDrawRayCast", "true" },
		{ "CPP_Default_bDrawSuspension", "true" },
		{ "CPP_Default_bDrawWheel", "true" },
		{ "CPP_Default_Duration", "-1.000000" },
		{ "CPP_Default_Thickness", "0.000000" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "debug draw" },
#endif
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleWheelComponent, nullptr, "DrawSuspension", nullptr, nullptr, Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::VehicleWheelComponent_eventDrawSuspension_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::VehicleWheelComponent_eventDrawSuspension_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspensionForce_Statics
	{
		struct VehicleWheelComponent_eventDrawSuspensionForce_Parms
		{
			float Duration;
			float Thickness;
			float Length;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_Duration;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_Thickness;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_Length;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspensionForce_Statics::NewProp_Duration = { "Duration", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventDrawSuspensionForce_Parms, Duration), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspensionForce_Statics::NewProp_Thickness = { "Thickness", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventDrawSuspensionForce_Parms, Thickness), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspensionForce_Statics::NewProp_Length = { "Length", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventDrawSuspensionForce_Parms, Length), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspensionForce_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspensionForce_Statics::NewProp_Duration,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspensionForce_Statics::NewProp_Thickness,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspensionForce_Statics::NewProp_Length,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspensionForce_Statics::Function_MetaDataParams[] = {
		{ "Category", "Debug" },
		{ "CPP_Default_Duration", "-1.000000" },
		{ "CPP_Default_Length", "1.000000" },
		{ "CPP_Default_Thickness", "5.000000" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspensionForce_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleWheelComponent, nullptr, "DrawSuspensionForce", nullptr, nullptr, Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspensionForce_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspensionForce_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspensionForce_Statics::VehicleWheelComponent_eventDrawSuspensionForce_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspensionForce_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspensionForce_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspensionForce_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspensionForce_Statics::VehicleWheelComponent_eventDrawSuspensionForce_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspensionForce()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspensionForce_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics
	{
		struct VehicleWheelComponent_eventDrawWheelForce_Parms
		{
			float Duration;
			float Thickness;
			float Length;
			bool bDrawVelocity;
			bool bDrawSlip;
			bool bDrawInertia;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_Duration;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_Thickness;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_Length;
		static void NewProp_bDrawVelocity_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bDrawVelocity;
		static void NewProp_bDrawSlip_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bDrawSlip;
		static void NewProp_bDrawInertia_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bDrawInertia;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::NewProp_Duration = { "Duration", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventDrawWheelForce_Parms, Duration), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::NewProp_Thickness = { "Thickness", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventDrawWheelForce_Parms, Thickness), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::NewProp_Length = { "Length", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventDrawWheelForce_Parms, Length), METADATA_PARAMS(0, nullptr) };
	void Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::NewProp_bDrawVelocity_SetBit(void* Obj)
	{
		((VehicleWheelComponent_eventDrawWheelForce_Parms*)Obj)->bDrawVelocity = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::NewProp_bDrawVelocity = { "bDrawVelocity", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VehicleWheelComponent_eventDrawWheelForce_Parms), &Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::NewProp_bDrawVelocity_SetBit, METADATA_PARAMS(0, nullptr) };
	void Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::NewProp_bDrawSlip_SetBit(void* Obj)
	{
		((VehicleWheelComponent_eventDrawWheelForce_Parms*)Obj)->bDrawSlip = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::NewProp_bDrawSlip = { "bDrawSlip", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VehicleWheelComponent_eventDrawWheelForce_Parms), &Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::NewProp_bDrawSlip_SetBit, METADATA_PARAMS(0, nullptr) };
	void Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::NewProp_bDrawInertia_SetBit(void* Obj)
	{
		((VehicleWheelComponent_eventDrawWheelForce_Parms*)Obj)->bDrawInertia = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::NewProp_bDrawInertia = { "bDrawInertia", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VehicleWheelComponent_eventDrawWheelForce_Parms), &Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::NewProp_bDrawInertia_SetBit, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::NewProp_Duration,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::NewProp_Thickness,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::NewProp_Length,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::NewProp_bDrawVelocity,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::NewProp_bDrawSlip,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::NewProp_bDrawInertia,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::Function_MetaDataParams[] = {
		{ "Category", "Debug" },
		{ "CPP_Default_bDrawInertia", "true" },
		{ "CPP_Default_bDrawSlip", "true" },
		{ "CPP_Default_bDrawVelocity", "true" },
		{ "CPP_Default_Duration", "-1.000000" },
		{ "CPP_Default_Length", "1.000000" },
		{ "CPP_Default_Thickness", "5.000000" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleWheelComponent, nullptr, "DrawWheelForce", nullptr, nullptr, Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::VehicleWheelComponent_eventDrawWheelForce_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::VehicleWheelComponent_eventDrawWheelForce_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleWheelComponent_GetAsyncCarbodyTransform_Statics
	{
		struct VehicleWheelComponent_eventGetAsyncCarbodyTransform_Parms
		{
			FTransform ReturnValue;
		};
		static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_GetAsyncCarbodyTransform_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventGetAsyncCarbodyTransform_Parms, ReturnValue), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleWheelComponent_GetAsyncCarbodyTransform_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_GetAsyncCarbodyTransform_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_GetAsyncCarbodyTransform_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleWheelComponent_GetAsyncCarbodyTransform_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleWheelComponent, nullptr, "GetAsyncCarbodyTransform", nullptr, nullptr, Z_Construct_UFunction_UVehicleWheelComponent_GetAsyncCarbodyTransform_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetAsyncCarbodyTransform_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleWheelComponent_GetAsyncCarbodyTransform_Statics::VehicleWheelComponent_eventGetAsyncCarbodyTransform_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetAsyncCarbodyTransform_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleWheelComponent_GetAsyncCarbodyTransform_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetAsyncCarbodyTransform_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleWheelComponent_GetAsyncCarbodyTransform_Statics::VehicleWheelComponent_eventGetAsyncCarbodyTransform_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleWheelComponent_GetAsyncCarbodyTransform()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleWheelComponent_GetAsyncCarbodyTransform_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleWheelComponent_GetAttachedComponent_Statics
	{
		struct VehicleWheelComponent_eventGetAttachedComponent_Parms
		{
			AActor* OutParentActor;
			UPrimitiveComponent* OutParentComponent;
			bool ReturnValue;
		};
		static const UECodeGen_Private::FObjectPropertyParams NewProp_OutParentActor;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_OutParentComponent_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_OutParentComponent;
		static void NewProp_ReturnValue_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_GetAttachedComponent_Statics::NewProp_OutParentActor = { "OutParentActor", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventGetAttachedComponent_Parms, OutParentActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_GetAttachedComponent_Statics::NewProp_OutParentComponent_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_GetAttachedComponent_Statics::NewProp_OutParentComponent = { "OutParentComponent", nullptr, (EPropertyFlags)0x0010000000080180, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventGetAttachedComponent_Parms, OutParentComponent), Z_Construct_UClass_UPrimitiveComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetAttachedComponent_Statics::NewProp_OutParentComponent_MetaData), Z_Construct_UFunction_UVehicleWheelComponent_GetAttachedComponent_Statics::NewProp_OutParentComponent_MetaData) };
	void Z_Construct_UFunction_UVehicleWheelComponent_GetAttachedComponent_Statics::NewProp_ReturnValue_SetBit(void* Obj)
	{
		((VehicleWheelComponent_eventGetAttachedComponent_Parms*)Obj)->ReturnValue = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_GetAttachedComponent_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VehicleWheelComponent_eventGetAttachedComponent_Parms), &Z_Construct_UFunction_UVehicleWheelComponent_GetAttachedComponent_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleWheelComponent_GetAttachedComponent_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_GetAttachedComponent_Statics::NewProp_OutParentActor,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_GetAttachedComponent_Statics::NewProp_OutParentComponent,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_GetAttachedComponent_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_GetAttachedComponent_Statics::Function_MetaDataParams[] = {
		{ "Category", "ReadOnly" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleWheelComponent_GetAttachedComponent_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleWheelComponent, nullptr, "GetAttachedComponent", nullptr, nullptr, Z_Construct_UFunction_UVehicleWheelComponent_GetAttachedComponent_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetAttachedComponent_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleWheelComponent_GetAttachedComponent_Statics::VehicleWheelComponent_eventGetAttachedComponent_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetAttachedComponent_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleWheelComponent_GetAttachedComponent_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetAttachedComponent_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleWheelComponent_GetAttachedComponent_Statics::VehicleWheelComponent_eventGetAttachedComponent_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleWheelComponent_GetAttachedComponent()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleWheelComponent_GetAttachedComponent_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleWheelComponent_GetP4MotorTorque_Statics
	{
		struct VehicleWheelComponent_eventGetP4MotorTorque_Parms
		{
			float ReturnValue;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_GetP4MotorTorque_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventGetP4MotorTorque_Parms, ReturnValue), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleWheelComponent_GetP4MotorTorque_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_GetP4MotorTorque_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_GetP4MotorTorque_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleWheelComponent_GetP4MotorTorque_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleWheelComponent, nullptr, "GetP4MotorTorque", nullptr, nullptr, Z_Construct_UFunction_UVehicleWheelComponent_GetP4MotorTorque_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetP4MotorTorque_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleWheelComponent_GetP4MotorTorque_Statics::VehicleWheelComponent_eventGetP4MotorTorque_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetP4MotorTorque_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleWheelComponent_GetP4MotorTorque_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetP4MotorTorque_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleWheelComponent_GetP4MotorTorque_Statics::VehicleWheelComponent_eventGetP4MotorTorque_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleWheelComponent_GetP4MotorTorque()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleWheelComponent_GetP4MotorTorque_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleWheelComponent_GetRayCastResult_Statics
	{
		struct VehicleWheelComponent_eventGetRayCastResult_Parms
		{
			FHitResult OutHitResult;
			bool OutRevised;
			bool ReturnValue;
		};
		static const UECodeGen_Private::FStructPropertyParams NewProp_OutHitResult;
		static void NewProp_OutRevised_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_OutRevised;
		static void NewProp_ReturnValue_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_GetRayCastResult_Statics::NewProp_OutHitResult = { "OutHitResult", nullptr, (EPropertyFlags)0x0010008000000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventGetRayCastResult_Parms, OutHitResult), Z_Construct_UScriptStruct_FHitResult, METADATA_PARAMS(0, nullptr) }; // 1891709922
	void Z_Construct_UFunction_UVehicleWheelComponent_GetRayCastResult_Statics::NewProp_OutRevised_SetBit(void* Obj)
	{
		((VehicleWheelComponent_eventGetRayCastResult_Parms*)Obj)->OutRevised = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_GetRayCastResult_Statics::NewProp_OutRevised = { "OutRevised", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VehicleWheelComponent_eventGetRayCastResult_Parms), &Z_Construct_UFunction_UVehicleWheelComponent_GetRayCastResult_Statics::NewProp_OutRevised_SetBit, METADATA_PARAMS(0, nullptr) };
	void Z_Construct_UFunction_UVehicleWheelComponent_GetRayCastResult_Statics::NewProp_ReturnValue_SetBit(void* Obj)
	{
		((VehicleWheelComponent_eventGetRayCastResult_Parms*)Obj)->ReturnValue = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_GetRayCastResult_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VehicleWheelComponent_eventGetRayCastResult_Parms), &Z_Construct_UFunction_UVehicleWheelComponent_GetRayCastResult_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleWheelComponent_GetRayCastResult_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_GetRayCastResult_Statics::NewProp_OutHitResult,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_GetRayCastResult_Statics::NewProp_OutRevised,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_GetRayCastResult_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_GetRayCastResult_Statics::Function_MetaDataParams[] = {
		{ "Category", "ReadOnly" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleWheelComponent_GetRayCastResult_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleWheelComponent, nullptr, "GetRayCastResult", nullptr, nullptr, Z_Construct_UFunction_UVehicleWheelComponent_GetRayCastResult_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetRayCastResult_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleWheelComponent_GetRayCastResult_Statics::VehicleWheelComponent_eventGetRayCastResult_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetRayCastResult_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleWheelComponent_GetRayCastResult_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetRayCastResult_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleWheelComponent_GetRayCastResult_Statics::VehicleWheelComponent_eventGetRayCastResult_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleWheelComponent_GetRayCastResult()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleWheelComponent_GetRayCastResult_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleWheelComponent_GetSkidMarkWorldTransform_Statics
	{
		struct VehicleWheelComponent_eventGetSkidMarkWorldTransform_Parms
		{
			float InSkidMarkBias;
			float InSkidMarkScale;
			FTransform ReturnValue;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InSkidMarkBias;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InSkidMarkScale;
		static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_GetSkidMarkWorldTransform_Statics::NewProp_InSkidMarkBias = { "InSkidMarkBias", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventGetSkidMarkWorldTransform_Parms, InSkidMarkBias), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_GetSkidMarkWorldTransform_Statics::NewProp_InSkidMarkScale = { "InSkidMarkScale", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventGetSkidMarkWorldTransform_Parms, InSkidMarkScale), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_GetSkidMarkWorldTransform_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventGetSkidMarkWorldTransform_Parms, ReturnValue), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleWheelComponent_GetSkidMarkWorldTransform_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_GetSkidMarkWorldTransform_Statics::NewProp_InSkidMarkBias,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_GetSkidMarkWorldTransform_Statics::NewProp_InSkidMarkScale,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_GetSkidMarkWorldTransform_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_GetSkidMarkWorldTransform_Statics::Function_MetaDataParams[] = {
		{ "Category", "Animation" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Compute and return the world location and world rotation of skid mark, the scale is local linear velocity of the wheel" },
#endif
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleWheelComponent_GetSkidMarkWorldTransform_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleWheelComponent, nullptr, "GetSkidMarkWorldTransform", nullptr, nullptr, Z_Construct_UFunction_UVehicleWheelComponent_GetSkidMarkWorldTransform_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetSkidMarkWorldTransform_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleWheelComponent_GetSkidMarkWorldTransform_Statics::VehicleWheelComponent_eventGetSkidMarkWorldTransform_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetSkidMarkWorldTransform_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleWheelComponent_GetSkidMarkWorldTransform_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetSkidMarkWorldTransform_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleWheelComponent_GetSkidMarkWorldTransform_Statics::VehicleWheelComponent_eventGetSkidMarkWorldTransform_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleWheelComponent_GetSkidMarkWorldTransform()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleWheelComponent_GetSkidMarkWorldTransform_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleWheelComponent_GetSuspensionMovement_Statics
	{
		struct VehicleWheelComponent_eventGetSuspensionMovement_Parms
		{
			FVehicleSuspensionSimData Out;
		};
		static const UECodeGen_Private::FStructPropertyParams NewProp_Out;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_GetSuspensionMovement_Statics::NewProp_Out = { "Out", nullptr, (EPropertyFlags)0x0010008000000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventGetSuspensionMovement_Parms, Out), Z_Construct_UScriptStruct_FVehicleSuspensionSimData, METADATA_PARAMS(0, nullptr) }; // 4192578155
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleWheelComponent_GetSuspensionMovement_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_GetSuspensionMovement_Statics::NewProp_Out,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_GetSuspensionMovement_Statics::Function_MetaDataParams[] = {
		{ "Category", "ReadOnly" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleWheelComponent_GetSuspensionMovement_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleWheelComponent, nullptr, "GetSuspensionMovement", nullptr, nullptr, Z_Construct_UFunction_UVehicleWheelComponent_GetSuspensionMovement_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetSuspensionMovement_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleWheelComponent_GetSuspensionMovement_Statics::VehicleWheelComponent_eventGetSuspensionMovement_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetSuspensionMovement_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleWheelComponent_GetSuspensionMovement_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetSuspensionMovement_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleWheelComponent_GetSuspensionMovement_Statics::VehicleWheelComponent_eventGetSuspensionMovement_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleWheelComponent_GetSuspensionMovement()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleWheelComponent_GetSuspensionMovement_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshComponents_Statics
	{
		struct VehicleWheelComponent_eventGetWheelAndBrakeMeshComponents_Parms
		{
			UStaticMeshComponent* OutWheel;
			UStaticMeshComponent* OutBrake;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_OutWheel_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_OutWheel;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_OutBrake_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_OutBrake;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshComponents_Statics::NewProp_OutWheel_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshComponents_Statics::NewProp_OutWheel = { "OutWheel", nullptr, (EPropertyFlags)0x0010000000080180, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventGetWheelAndBrakeMeshComponents_Parms, OutWheel), Z_Construct_UClass_UStaticMeshComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshComponents_Statics::NewProp_OutWheel_MetaData), Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshComponents_Statics::NewProp_OutWheel_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshComponents_Statics::NewProp_OutBrake_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshComponents_Statics::NewProp_OutBrake = { "OutBrake", nullptr, (EPropertyFlags)0x0010000000080180, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventGetWheelAndBrakeMeshComponents_Parms, OutBrake), Z_Construct_UClass_UStaticMeshComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshComponents_Statics::NewProp_OutBrake_MetaData), Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshComponents_Statics::NewProp_OutBrake_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshComponents_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshComponents_Statics::NewProp_OutWheel,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshComponents_Statics::NewProp_OutBrake,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshComponents_Statics::Function_MetaDataParams[] = {
		{ "Category", "Mesh" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshComponents_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleWheelComponent, nullptr, "GetWheelAndBrakeMeshComponents", nullptr, nullptr, Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshComponents_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshComponents_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshComponents_Statics::VehicleWheelComponent_eventGetWheelAndBrakeMeshComponents_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshComponents_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshComponents_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshComponents_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshComponents_Statics::VehicleWheelComponent_eventGetWheelAndBrakeMeshComponents_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshComponents()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshComponents_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshes_Statics
	{
		struct VehicleWheelComponent_eventGetWheelAndBrakeMeshes_Parms
		{
			UStaticMesh* OutWheelMesh;
			UStaticMesh* OutBrakeMesh;
		};
		static const UECodeGen_Private::FObjectPropertyParams NewProp_OutWheelMesh;
		static const UECodeGen_Private::FObjectPropertyParams NewProp_OutBrakeMesh;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshes_Statics::NewProp_OutWheelMesh = { "OutWheelMesh", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventGetWheelAndBrakeMeshes_Parms, OutWheelMesh), Z_Construct_UClass_UStaticMesh_NoRegister, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshes_Statics::NewProp_OutBrakeMesh = { "OutBrakeMesh", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventGetWheelAndBrakeMeshes_Parms, OutBrakeMesh), Z_Construct_UClass_UStaticMesh_NoRegister, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshes_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshes_Statics::NewProp_OutWheelMesh,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshes_Statics::NewProp_OutBrakeMesh,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshes_Statics::Function_MetaDataParams[] = {
		{ "Category", "Mesh" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshes_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleWheelComponent, nullptr, "GetWheelAndBrakeMeshes", nullptr, nullptr, Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshes_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshes_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshes_Statics::VehicleWheelComponent_eventGetWheelAndBrakeMeshes_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshes_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshes_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshes_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshes_Statics::VehicleWheelComponent_eventGetWheelAndBrakeMeshes_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshes()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshes_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleWheelComponent_GetWheelCoordinator_Statics
	{
		struct VehicleWheelComponent_eventGetWheelCoordinator_Parms
		{
			UVehicleWheelCoordinatorComponent* OutWheelCoordinator;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_OutWheelCoordinator_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_OutWheelCoordinator;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_GetWheelCoordinator_Statics::NewProp_OutWheelCoordinator_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_GetWheelCoordinator_Statics::NewProp_OutWheelCoordinator = { "OutWheelCoordinator", nullptr, (EPropertyFlags)0x0010000000080180, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventGetWheelCoordinator_Parms, OutWheelCoordinator), Z_Construct_UClass_UVehicleWheelCoordinatorComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelCoordinator_Statics::NewProp_OutWheelCoordinator_MetaData), Z_Construct_UFunction_UVehicleWheelComponent_GetWheelCoordinator_Statics::NewProp_OutWheelCoordinator_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleWheelComponent_GetWheelCoordinator_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_GetWheelCoordinator_Statics::NewProp_OutWheelCoordinator,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_GetWheelCoordinator_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleWheelComponent_GetWheelCoordinator_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleWheelComponent, nullptr, "GetWheelCoordinator", nullptr, nullptr, Z_Construct_UFunction_UVehicleWheelComponent_GetWheelCoordinator_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelCoordinator_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelCoordinator_Statics::VehicleWheelComponent_eventGetWheelCoordinator_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelCoordinator_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleWheelComponent_GetWheelCoordinator_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelCoordinator_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelCoordinator_Statics::VehicleWheelComponent_eventGetWheelCoordinator_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleWheelComponent_GetWheelCoordinator()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleWheelComponent_GetWheelCoordinator_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleWheelComponent_GetWheelHubComponent_Statics
	{
		struct VehicleWheelComponent_eventGetWheelHubComponent_Parms
		{
			USceneComponent* OutHub;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_OutHub_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_OutHub;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_GetWheelHubComponent_Statics::NewProp_OutHub_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_GetWheelHubComponent_Statics::NewProp_OutHub = { "OutHub", nullptr, (EPropertyFlags)0x0010000000080180, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventGetWheelHubComponent_Parms, OutHub), Z_Construct_UClass_USceneComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelHubComponent_Statics::NewProp_OutHub_MetaData), Z_Construct_UFunction_UVehicleWheelComponent_GetWheelHubComponent_Statics::NewProp_OutHub_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleWheelComponent_GetWheelHubComponent_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_GetWheelHubComponent_Statics::NewProp_OutHub,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_GetWheelHubComponent_Statics::Function_MetaDataParams[] = {
		{ "Category", "Animation" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleWheelComponent_GetWheelHubComponent_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleWheelComponent, nullptr, "GetWheelHubComponent", nullptr, nullptr, Z_Construct_UFunction_UVehicleWheelComponent_GetWheelHubComponent_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelHubComponent_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelHubComponent_Statics::VehicleWheelComponent_eventGetWheelHubComponent_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelHubComponent_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleWheelComponent_GetWheelHubComponent_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelHubComponent_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelHubComponent_Statics::VehicleWheelComponent_eventGetWheelHubComponent_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleWheelComponent_GetWheelHubComponent()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleWheelComponent_GetWheelHubComponent_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleWheelComponent_GetWheelMovement_Statics
	{
		struct VehicleWheelComponent_eventGetWheelMovement_Parms
		{
			FVehicleWheelSimData Out;
		};
		static const UECodeGen_Private::FStructPropertyParams NewProp_Out;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_GetWheelMovement_Statics::NewProp_Out = { "Out", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventGetWheelMovement_Parms, Out), Z_Construct_UScriptStruct_FVehicleWheelSimData, METADATA_PARAMS(0, nullptr) }; // 173970849
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleWheelComponent_GetWheelMovement_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_GetWheelMovement_Statics::NewProp_Out,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_GetWheelMovement_Statics::Function_MetaDataParams[] = {
		{ "Category", "ReadOnly" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleWheelComponent_GetWheelMovement_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleWheelComponent, nullptr, "GetWheelMovement", nullptr, nullptr, Z_Construct_UFunction_UVehicleWheelComponent_GetWheelMovement_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelMovement_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelMovement_Statics::VehicleWheelComponent_eventGetWheelMovement_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14420401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelMovement_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleWheelComponent_GetWheelMovement_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelMovement_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelMovement_Statics::VehicleWheelComponent_eventGetWheelMovement_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleWheelComponent_GetWheelMovement()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleWheelComponent_GetWheelMovement_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleWheelComponent_GetWheelTransform_Statics
	{
		struct VehicleWheelComponent_eventGetWheelTransform_Parms
		{
			FTransform OutComponentRelativeTransform;
			FTransform OutRaycastTransform;
			FTransform OutParentTransform;
		};
		static const UECodeGen_Private::FStructPropertyParams NewProp_OutComponentRelativeTransform;
		static const UECodeGen_Private::FStructPropertyParams NewProp_OutRaycastTransform;
		static const UECodeGen_Private::FStructPropertyParams NewProp_OutParentTransform;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_GetWheelTransform_Statics::NewProp_OutComponentRelativeTransform = { "OutComponentRelativeTransform", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventGetWheelTransform_Parms, OutComponentRelativeTransform), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_GetWheelTransform_Statics::NewProp_OutRaycastTransform = { "OutRaycastTransform", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventGetWheelTransform_Parms, OutRaycastTransform), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_GetWheelTransform_Statics::NewProp_OutParentTransform = { "OutParentTransform", nullptr, (EPropertyFlags)0x0010000000000180, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventGetWheelTransform_Parms, OutParentTransform), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleWheelComponent_GetWheelTransform_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_GetWheelTransform_Statics::NewProp_OutComponentRelativeTransform,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_GetWheelTransform_Statics::NewProp_OutRaycastTransform,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_GetWheelTransform_Statics::NewProp_OutParentTransform,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_GetWheelTransform_Statics::Function_MetaDataParams[] = {
		{ "Category", "ReadOnly" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleWheelComponent_GetWheelTransform_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleWheelComponent, nullptr, "GetWheelTransform", nullptr, nullptr, Z_Construct_UFunction_UVehicleWheelComponent_GetWheelTransform_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelTransform_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelTransform_Statics::VehicleWheelComponent_eventGetWheelTransform_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04C20401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelTransform_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleWheelComponent_GetWheelTransform_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelTransform_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleWheelComponent_GetWheelTransform_Statics::VehicleWheelComponent_eventGetWheelTransform_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleWheelComponent_GetWheelTransform()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleWheelComponent_GetWheelTransform_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleWheelComponent_RefreshWheelMesh_Statics
	{
		struct VehicleWheelComponent_eventRefreshWheelMesh_Parms
		{
			bool ReturnValue;
		};
		static void NewProp_ReturnValue_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	void Z_Construct_UFunction_UVehicleWheelComponent_RefreshWheelMesh_Statics::NewProp_ReturnValue_SetBit(void* Obj)
	{
		((VehicleWheelComponent_eventRefreshWheelMesh_Parms*)Obj)->ReturnValue = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_RefreshWheelMesh_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VehicleWheelComponent_eventRefreshWheelMesh_Parms), &Z_Construct_UFunction_UVehicleWheelComponent_RefreshWheelMesh_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleWheelComponent_RefreshWheelMesh_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_RefreshWheelMesh_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_RefreshWheelMesh_Statics::Function_MetaDataParams[] = {
		{ "Category", "Mesh" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleWheelComponent_RefreshWheelMesh_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleWheelComponent, nullptr, "RefreshWheelMesh", nullptr, nullptr, Z_Construct_UFunction_UVehicleWheelComponent_RefreshWheelMesh_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_RefreshWheelMesh_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleWheelComponent_RefreshWheelMesh_Statics::VehicleWheelComponent_eventRefreshWheelMesh_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_RefreshWheelMesh_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleWheelComponent_RefreshWheelMesh_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_RefreshWheelMesh_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleWheelComponent_RefreshWheelMesh_Statics::VehicleWheelComponent_eventRefreshWheelMesh_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleWheelComponent_RefreshWheelMesh()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleWheelComponent_RefreshWheelMesh_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics
	{
		struct VehicleWheelComponent_eventSetMesh_Parms
		{
			float SteeringAxleOffset;
			UStaticMesh* NewWheelMesh;
			FTransform WheelMeshRelatvieTransform;
			UStaticMesh* NewBrakeMesh;
			FTransform BrakeMeshRelativeTransform;
			bool ReturnValue;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_SteeringAxleOffset;
		static const UECodeGen_Private::FObjectPropertyParams NewProp_NewWheelMesh;
		static const UECodeGen_Private::FStructPropertyParams NewProp_WheelMeshRelatvieTransform;
		static const UECodeGen_Private::FObjectPropertyParams NewProp_NewBrakeMesh;
		static const UECodeGen_Private::FStructPropertyParams NewProp_BrakeMeshRelativeTransform;
		static void NewProp_ReturnValue_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics::NewProp_SteeringAxleOffset = { "SteeringAxleOffset", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventSetMesh_Parms, SteeringAxleOffset), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics::NewProp_NewWheelMesh = { "NewWheelMesh", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventSetMesh_Parms, NewWheelMesh), Z_Construct_UClass_UStaticMesh_NoRegister, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics::NewProp_WheelMeshRelatvieTransform = { "WheelMeshRelatvieTransform", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventSetMesh_Parms, WheelMeshRelatvieTransform), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics::NewProp_NewBrakeMesh = { "NewBrakeMesh", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventSetMesh_Parms, NewBrakeMesh), Z_Construct_UClass_UStaticMesh_NoRegister, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics::NewProp_BrakeMeshRelativeTransform = { "BrakeMeshRelativeTransform", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventSetMesh_Parms, BrakeMeshRelativeTransform), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(0, nullptr) };
	void Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics::NewProp_ReturnValue_SetBit(void* Obj)
	{
		((VehicleWheelComponent_eventSetMesh_Parms*)Obj)->ReturnValue = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(VehicleWheelComponent_eventSetMesh_Parms), &Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics::NewProp_SteeringAxleOffset,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics::NewProp_NewWheelMesh,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics::NewProp_WheelMeshRelatvieTransform,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics::NewProp_NewBrakeMesh,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics::NewProp_BrakeMeshRelativeTransform,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics::Function_MetaDataParams[] = {
		{ "Category", "Mesh" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleWheelComponent, nullptr, "SetMesh", nullptr, nullptr, Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics::VehicleWheelComponent_eventSetMesh_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics::VehicleWheelComponent_eventSetMesh_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleWheelComponent_SetMesh()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleWheelComponent_SetMesh_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleWheelComponent_SetP4MotorTorque_Statics
	{
		struct VehicleWheelComponent_eventSetP4MotorTorque_Parms
		{
			float NewTorque;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_NewTorque;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_SetP4MotorTorque_Statics::NewProp_NewTorque = { "NewTorque", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventSetP4MotorTorque_Parms, NewTorque), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleWheelComponent_SetP4MotorTorque_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_SetP4MotorTorque_Statics::NewProp_NewTorque,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_SetP4MotorTorque_Statics::Function_MetaDataParams[] = {
		{ "Category", "Physics" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleWheelComponent_SetP4MotorTorque_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleWheelComponent, nullptr, "SetP4MotorTorque", nullptr, nullptr, Z_Construct_UFunction_UVehicleWheelComponent_SetP4MotorTorque_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_SetP4MotorTorque_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleWheelComponent_SetP4MotorTorque_Statics::VehicleWheelComponent_eventSetP4MotorTorque_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_SetP4MotorTorque_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleWheelComponent_SetP4MotorTorque_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_SetP4MotorTorque_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleWheelComponent_SetP4MotorTorque_Statics::VehicleWheelComponent_eventSetP4MotorTorque_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleWheelComponent_SetP4MotorTorque()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleWheelComponent_SetP4MotorTorque_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics
	{
		struct VehicleWheelComponent_eventUpdatePhysics_Parms
		{
			float InPhysicsDeltaTime;
			float InDriveTorque;
			float InBrakeTorque;
			float InHandbrakeTorque;
			float InSteeringAngle;
			float InSwaybarForce;
			float InReflectedInertia;
		};
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InPhysicsDeltaTime;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InDriveTorque;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InBrakeTorque;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InHandbrakeTorque;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InSteeringAngle;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InSwaybarForce;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InReflectedInertia;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics::NewProp_InPhysicsDeltaTime = { "InPhysicsDeltaTime", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventUpdatePhysics_Parms, InPhysicsDeltaTime), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics::NewProp_InDriveTorque = { "InDriveTorque", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventUpdatePhysics_Parms, InDriveTorque), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics::NewProp_InBrakeTorque = { "InBrakeTorque", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventUpdatePhysics_Parms, InBrakeTorque), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics::NewProp_InHandbrakeTorque = { "InHandbrakeTorque", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventUpdatePhysics_Parms, InHandbrakeTorque), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics::NewProp_InSteeringAngle = { "InSteeringAngle", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventUpdatePhysics_Parms, InSteeringAngle), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics::NewProp_InSwaybarForce = { "InSwaybarForce", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventUpdatePhysics_Parms, InSwaybarForce), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics::NewProp_InReflectedInertia = { "InReflectedInertia", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventUpdatePhysics_Parms, InReflectedInertia), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics::NewProp_InPhysicsDeltaTime,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics::NewProp_InDriveTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics::NewProp_InBrakeTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics::NewProp_InHandbrakeTorque,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics::NewProp_InSteeringAngle,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics::NewProp_InSwaybarForce,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics::NewProp_InReflectedInertia,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics::Function_MetaDataParams[] = {
		{ "Category", "PhysicsThreadOnly" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleWheelComponent, nullptr, "UpdatePhysics", nullptr, nullptr, Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics::VehicleWheelComponent_eventUpdatePhysics_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics::VehicleWheelComponent_eventUpdatePhysics_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionArmAnim_Statics
	{
		struct VehicleWheelComponent_eventUpdateSuspensionArmAnim_Parms
		{
			USceneComponent* InArmMesh;
			FRotator InRotationOffset;
			FQuat ReturnValue;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_InArmMesh_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_InArmMesh;
		static const UECodeGen_Private::FStructPropertyParams NewProp_InRotationOffset;
		static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionArmAnim_Statics::NewProp_InArmMesh_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionArmAnim_Statics::NewProp_InArmMesh = { "InArmMesh", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventUpdateSuspensionArmAnim_Parms, InArmMesh), Z_Construct_UClass_USceneComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionArmAnim_Statics::NewProp_InArmMesh_MetaData), Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionArmAnim_Statics::NewProp_InArmMesh_MetaData) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionArmAnim_Statics::NewProp_InRotationOffset = { "InRotationOffset", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventUpdateSuspensionArmAnim_Parms, InRotationOffset), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionArmAnim_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventUpdateSuspensionArmAnim_Parms, ReturnValue), Z_Construct_UScriptStruct_FQuat, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionArmAnim_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionArmAnim_Statics::NewProp_InArmMesh,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionArmAnim_Statics::NewProp_InRotationOffset,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionArmAnim_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionArmAnim_Statics::Function_MetaDataParams[] = {
		{ "Category", "Animation" },
		{ "CPP_Default_InRotationOffset", "0.000000,0.000000,0.000000" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionArmAnim_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleWheelComponent, nullptr, "UpdateSuspensionArmAnim", nullptr, nullptr, Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionArmAnim_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionArmAnim_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionArmAnim_Statics::VehicleWheelComponent_eventUpdateSuspensionArmAnim_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionArmAnim_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionArmAnim_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionArmAnim_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionArmAnim_Statics::VehicleWheelComponent_eventUpdateSuspensionArmAnim_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionArmAnim()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionArmAnim_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics
	{
		struct VehicleWheelComponent_eventUpdateSuspensionSpringAnim_Parms
		{
			USceneComponent* InSpringMesh;
			FVector InScaleAxis;
			float InOffsetAlongArm;
			FVector2D InBallJointOffset;
			FRotator InRotationOffset;
			float InLengthBias;
			FVector InInitialScale;
			FTransform ReturnValue;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_InSpringMesh_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_InSpringMesh;
		static const UECodeGen_Private::FStructPropertyParams NewProp_InScaleAxis;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InOffsetAlongArm;
		static const UECodeGen_Private::FStructPropertyParams NewProp_InBallJointOffset;
		static const UECodeGen_Private::FStructPropertyParams NewProp_InRotationOffset;
		static const UECodeGen_Private::FFloatPropertyParams NewProp_InLengthBias;
		static const UECodeGen_Private::FStructPropertyParams NewProp_InInitialScale;
		static const UECodeGen_Private::FStructPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::NewProp_InSpringMesh_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::NewProp_InSpringMesh = { "InSpringMesh", nullptr, (EPropertyFlags)0x0010000000080080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventUpdateSuspensionSpringAnim_Parms, InSpringMesh), Z_Construct_UClass_USceneComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::NewProp_InSpringMesh_MetaData), Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::NewProp_InSpringMesh_MetaData) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::NewProp_InScaleAxis = { "InScaleAxis", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventUpdateSuspensionSpringAnim_Parms, InScaleAxis), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::NewProp_InOffsetAlongArm = { "InOffsetAlongArm", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventUpdateSuspensionSpringAnim_Parms, InOffsetAlongArm), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::NewProp_InBallJointOffset = { "InBallJointOffset", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventUpdateSuspensionSpringAnim_Parms, InBallJointOffset), Z_Construct_UScriptStruct_FVector2D, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::NewProp_InRotationOffset = { "InRotationOffset", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventUpdateSuspensionSpringAnim_Parms, InRotationOffset), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::NewProp_InLengthBias = { "InLengthBias", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventUpdateSuspensionSpringAnim_Parms, InLengthBias), METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::NewProp_InInitialScale = { "InInitialScale", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventUpdateSuspensionSpringAnim_Parms, InInitialScale), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(VehicleWheelComponent_eventUpdateSuspensionSpringAnim_Parms, ReturnValue), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(0, nullptr) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::NewProp_InSpringMesh,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::NewProp_InScaleAxis,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::NewProp_InOffsetAlongArm,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::NewProp_InBallJointOffset,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::NewProp_InRotationOffset,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::NewProp_InLengthBias,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::NewProp_InInitialScale,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::Function_MetaDataParams[] = {
		{ "Category", "Animation" },
		{ "CPP_Default_InBallJointOffset", "(X=0.000,Y=0.000)" },
		{ "CPP_Default_InInitialScale", "1.000000,1.000000,1.000000" },
		{ "CPP_Default_InLengthBias", "0.000000" },
		{ "CPP_Default_InOffsetAlongArm", "0.000000" },
		{ "CPP_Default_InRotationOffset", "0.000000,0.000000,0.000000" },
		{ "CPP_Default_InScaleAxis", "0.000000,0.000000,1.000000" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UVehicleWheelComponent, nullptr, "UpdateSuspensionSpringAnim", nullptr, nullptr, Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::PropPointers), sizeof(Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::VehicleWheelComponent_eventUpdateSuspensionSpringAnim_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04820401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::Function_MetaDataParams), Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::Function_MetaDataParams) };
	static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::PropPointers) < 2048);
	static_assert(sizeof(Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::VehicleWheelComponent_eventUpdateSuspensionSpringAnim_Parms) < MAX_uint16);
	UFunction* Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UVehicleWheelComponent);
	UClass* Z_Construct_UClass_UVehicleWheelComponent_NoRegister()
	{
		return UVehicleWheelComponent::StaticClass();
	}
	struct Z_Construct_UClass_UVehicleWheelComponent_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_WheelConfig_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_WheelConfig;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TireConfig_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_TireConfig;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ABSConfig_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_ABSConfig;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SuspensionKinematicsConfig_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_SuspensionKinematicsConfig;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SuspensionSpringConfig_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_SuspensionSpringConfig;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_bUpdateAnimAutomatically_MetaData[];
#endif
		static void NewProp_bUpdateAnimAutomatically_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_bUpdateAnimAutomatically;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_WheelMesh_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_WheelMesh;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_WheelMeshTransform_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_WheelMeshTransform;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BrakeMesh_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_BrakeMesh;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BrakeMeshTransform_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_BrakeMeshTransform;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_WheelHubComponent_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_WheelHubComponent;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_WheelMeshComponent_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_WheelMeshComponent;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_BrakeMeshComponent_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_BrakeMeshComponent;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_CurrentWorld_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_CurrentWorld;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Carbody_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Carbody;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ParentActor_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_ParentActor;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_WheelCoordinator_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_WheelCoordinator;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UVehicleWheelComponent_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_USceneComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_KinetiForge,
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelComponent_Statics::DependentSingletons) < 16);
	const FClassFunctionLinkInfo Z_Construct_UClass_UVehicleWheelComponent_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UVehicleWheelComponent_AttachWheelHubMeshToSuspension, "AttachWheelHubMeshToSuspension" }, // 2020872436
		{ &Z_Construct_UFunction_UVehicleWheelComponent_ComputeFeedBackTorque, "ComputeFeedBackTorque" }, // 3461768655
		{ &Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspension, "DrawSuspension" }, // 3864483565
		{ &Z_Construct_UFunction_UVehicleWheelComponent_DrawSuspensionForce, "DrawSuspensionForce" }, // 478945341
		{ &Z_Construct_UFunction_UVehicleWheelComponent_DrawWheelForce, "DrawWheelForce" }, // 2741089377
		{ &Z_Construct_UFunction_UVehicleWheelComponent_GetAsyncCarbodyTransform, "GetAsyncCarbodyTransform" }, // 3648088784
		{ &Z_Construct_UFunction_UVehicleWheelComponent_GetAttachedComponent, "GetAttachedComponent" }, // 1057390776
		{ &Z_Construct_UFunction_UVehicleWheelComponent_GetP4MotorTorque, "GetP4MotorTorque" }, // 3174681322
		{ &Z_Construct_UFunction_UVehicleWheelComponent_GetRayCastResult, "GetRayCastResult" }, // 3064239394
		{ &Z_Construct_UFunction_UVehicleWheelComponent_GetSkidMarkWorldTransform, "GetSkidMarkWorldTransform" }, // 1952741255
		{ &Z_Construct_UFunction_UVehicleWheelComponent_GetSuspensionMovement, "GetSuspensionMovement" }, // 3118140906
		{ &Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshComponents, "GetWheelAndBrakeMeshComponents" }, // 3298618562
		{ &Z_Construct_UFunction_UVehicleWheelComponent_GetWheelAndBrakeMeshes, "GetWheelAndBrakeMeshes" }, // 1869616841
		{ &Z_Construct_UFunction_UVehicleWheelComponent_GetWheelCoordinator, "GetWheelCoordinator" }, // 3716696335
		{ &Z_Construct_UFunction_UVehicleWheelComponent_GetWheelHubComponent, "GetWheelHubComponent" }, // 1602989245
		{ &Z_Construct_UFunction_UVehicleWheelComponent_GetWheelMovement, "GetWheelMovement" }, // 1675461164
		{ &Z_Construct_UFunction_UVehicleWheelComponent_GetWheelTransform, "GetWheelTransform" }, // 3827576173
		{ &Z_Construct_UFunction_UVehicleWheelComponent_RefreshWheelMesh, "RefreshWheelMesh" }, // 2024369471
		{ &Z_Construct_UFunction_UVehicleWheelComponent_SetMesh, "SetMesh" }, // 1301102918
		{ &Z_Construct_UFunction_UVehicleWheelComponent_SetP4MotorTorque, "SetP4MotorTorque" }, // 2670902096
		{ &Z_Construct_UFunction_UVehicleWheelComponent_UpdatePhysics, "UpdatePhysics" }, // 2707841889
		{ &Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionArmAnim, "UpdateSuspensionArmAnim" }, // 3593385030
		{ &Z_Construct_UFunction_UVehicleWheelComponent_UpdateSuspensionSpringAnim, "UpdateSuspensionSpringAnim" }, // 633105480
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelComponent_Statics::FuncInfo) < 2048);
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleWheelComponent_Statics::Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "BlueprintType", "true" },
		{ "ClassGroupNames", "Custom" },
		{ "HideCategories", "Trigger PhysicsVolume" },
		{ "IncludePath", "VehicleWheelComponent.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelConfig_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelConfig = { "WheelConfig", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleWheelComponent, WheelConfig), Z_Construct_UScriptStruct_FVehicleWheelConfig, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelConfig_MetaData), Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelConfig_MetaData) }; // 3280903524
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_TireConfig_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_TireConfig = { "TireConfig", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleWheelComponent, TireConfig), Z_Construct_UScriptStruct_FVehicleTireConfig, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_TireConfig_MetaData), Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_TireConfig_MetaData) }; // 3925339287
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_ABSConfig_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_ABSConfig = { "ABSConfig", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleWheelComponent, ABSConfig), Z_Construct_UScriptStruct_FVehicleABSConfig, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_ABSConfig_MetaData), Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_ABSConfig_MetaData) }; // 2378670773
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_SuspensionKinematicsConfig_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_SuspensionKinematicsConfig = { "SuspensionKinematicsConfig", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleWheelComponent, SuspensionKinematicsConfig), Z_Construct_UScriptStruct_FVehicleSuspensionKinematicsConfig, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_SuspensionKinematicsConfig_MetaData), Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_SuspensionKinematicsConfig_MetaData) }; // 2293807127
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_SuspensionSpringConfig_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_SuspensionSpringConfig = { "SuspensionSpringConfig", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleWheelComponent, SuspensionSpringConfig), Z_Construct_UScriptStruct_FVehicleSuspensionSpringConfig, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_SuspensionSpringConfig_MetaData), Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_SuspensionSpringConfig_MetaData) }; // 1248612756
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_bUpdateAnimAutomatically_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	void Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_bUpdateAnimAutomatically_SetBit(void* Obj)
	{
		((UVehicleWheelComponent*)Obj)->bUpdateAnimAutomatically = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_bUpdateAnimAutomatically = { "bUpdateAnimAutomatically", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UVehicleWheelComponent), &Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_bUpdateAnimAutomatically_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_bUpdateAnimAutomatically_MetaData), Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_bUpdateAnimAutomatically_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelMesh_MetaData[] = {
		{ "Category", "Setup" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Mesh Setup\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Mesh Setup" },
#endif
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelMesh = { "WheelMesh", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleWheelComponent, WheelMesh), Z_Construct_UClass_UStaticMesh_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelMesh_MetaData), Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelMesh_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelMeshTransform_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelMeshTransform = { "WheelMeshTransform", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleWheelComponent, WheelMeshTransform), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelMeshTransform_MetaData), Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelMeshTransform_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_BrakeMesh_MetaData[] = {
		{ "Category", "Setup" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//relative to VehicleWheelComponent\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "relative to VehicleWheelComponent" },
#endif
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_BrakeMesh = { "BrakeMesh", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleWheelComponent, BrakeMesh), Z_Construct_UClass_UStaticMesh_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_BrakeMesh_MetaData), Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_BrakeMesh_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_BrakeMeshTransform_MetaData[] = {
		{ "Category", "Setup" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_BrakeMeshTransform = { "BrakeMeshTransform", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleWheelComponent, BrakeMeshTransform), Z_Construct_UScriptStruct_FTransform, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_BrakeMeshTransform_MetaData), Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_BrakeMeshTransform_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelHubComponent_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelHubComponent = { "WheelHubComponent", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleWheelComponent, WheelHubComponent), Z_Construct_UClass_USceneComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelHubComponent_MetaData), Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelHubComponent_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelMeshComponent_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelMeshComponent = { "WheelMeshComponent", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleWheelComponent, WheelMeshComponent), Z_Construct_UClass_UStaticMeshComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelMeshComponent_MetaData), Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelMeshComponent_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_BrakeMeshComponent_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_BrakeMeshComponent = { "BrakeMeshComponent", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleWheelComponent, BrakeMeshComponent), Z_Construct_UClass_UStaticMeshComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_BrakeMeshComponent_MetaData), Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_BrakeMeshComponent_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_CurrentWorld_MetaData[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Parent\n" },
#endif
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Parent" },
#endif
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_CurrentWorld = { "CurrentWorld", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleWheelComponent, CurrentWorld), Z_Construct_UClass_UWorld_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_CurrentWorld_MetaData), Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_CurrentWorld_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_Carbody_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_Carbody = { "Carbody", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleWheelComponent, Carbody), Z_Construct_UClass_UPrimitiveComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_Carbody_MetaData), Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_Carbody_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_ParentActor_MetaData[] = {
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_ParentActor = { "ParentActor", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleWheelComponent, ParentActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_ParentActor_MetaData), Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_ParentActor_MetaData) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelCoordinator_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/VehicleWheelComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelCoordinator = { "WheelCoordinator", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UVehicleWheelComponent, WheelCoordinator), Z_Construct_UClass_UVehicleWheelCoordinatorComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelCoordinator_MetaData), Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelCoordinator_MetaData) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UVehicleWheelComponent_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelConfig,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_TireConfig,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_ABSConfig,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_SuspensionKinematicsConfig,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_SuspensionSpringConfig,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_bUpdateAnimAutomatically,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelMesh,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelMeshTransform,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_BrakeMesh,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_BrakeMeshTransform,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelHubComponent,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelMeshComponent,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_BrakeMeshComponent,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_CurrentWorld,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_Carbody,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_ParentActor,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVehicleWheelComponent_Statics::NewProp_WheelCoordinator,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UVehicleWheelComponent_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVehicleWheelComponent>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UVehicleWheelComponent_Statics::ClassParams = {
		&UVehicleWheelComponent::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_UVehicleWheelComponent_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelComponent_Statics::PropPointers),
		0,
		0x00B000A4u,
		METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelComponent_Statics::Class_MetaDataParams), Z_Construct_UClass_UVehicleWheelComponent_Statics::Class_MetaDataParams)
	};
	static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVehicleWheelComponent_Statics::PropPointers) < 2048);
	UClass* Z_Construct_UClass_UVehicleWheelComponent()
	{
		if (!Z_Registration_Info_UClass_UVehicleWheelComponent.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVehicleWheelComponent.OuterSingleton, Z_Construct_UClass_UVehicleWheelComponent_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UVehicleWheelComponent.OuterSingleton;
	}
	template<> KINETIFORGE_API UClass* StaticClass<UVehicleWheelComponent>()
	{
		return UVehicleWheelComponent::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UVehicleWheelComponent);
	UVehicleWheelComponent::~UVehicleWheelComponent() {}
	struct Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelComponent_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelComponent_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UVehicleWheelComponent, UVehicleWheelComponent::StaticClass, TEXT("UVehicleWheelComponent"), &Z_Registration_Info_UClass_UVehicleWheelComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVehicleWheelComponent), 100298893U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelComponent_h_1912476703(TEXT("/Script/KinetiForge"),
		Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelComponent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Files_UnrealEngine_Unreal_Projects_KinetiForgeVehicles_Plugins_KinetiForgeVehicles_Source_KinetiForge_Public_VehicleWheelComponent_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
