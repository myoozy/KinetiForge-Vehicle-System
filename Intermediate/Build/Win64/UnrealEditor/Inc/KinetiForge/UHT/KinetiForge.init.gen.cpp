// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeKinetiForge_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_KinetiForge;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_KinetiForge()
	{
		if (!Z_Registration_Info_UPackage__Script_KinetiForge.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/KinetiForge",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000000,
				0x1C03D6F6,
				0x2F058796,
				METADATA_PARAMS(0, nullptr)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_KinetiForge.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_KinetiForge.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_KinetiForge(Z_Construct_UPackage__Script_KinetiForge, TEXT("/Script/KinetiForge"), Z_Registration_Info_UPackage__Script_KinetiForge, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x1C03D6F6, 0x2F058796));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
