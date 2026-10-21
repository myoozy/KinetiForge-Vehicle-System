// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "VehicleDriveAssemblyComponent.h"
#include "VehicleWheelComponent.h"
#include "VehicleWheelBridge.h"
#include "VehicleEngineComponent.h"
#include "VehiclePowerUnitBridge.h"
#include "VehiclePhysicsScene.h"
#include "KinetiForge.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "HAL/PlatformTLS.h"
#include "Modules/ModuleManager.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "PBDRigidsSolver.h"
#if WITH_EDITOR
#include "AssetCompilingManager.h"
#endif
#include "Misc/AutomationTest.h"
#include "Misc/ScopeExit.h"
#include "UObject/Package.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleExistingBlueprintPreviewTest,
	"KinetiForge.Lifecycle.ExistingBlueprintPreviewRegistration",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleExistingBlueprintPreviewTest::RunTest(const FString& Parameters)
{
	UWorld* World = UWorld::CreateWorld(EWorldType::EditorPreview, false, TEXT("VehicleExistingBlueprintPreview"),
		nullptr, true, ERHIFeatureLevel::Num, nullptr, true);
	if (!TestNotNull(TEXT("Blueprint preview world created"), World)) return false;
	ON_SCOPE_EXIT { World->DestroyWorld(false); };
	World->InitWorld(UWorld::InitializationValues().AllowAudioPlayback(false).CreatePhysicsScene(true)
		.RequiresHitProxies(false).CreateNavigation(false).CreateAISystem(false)
		.ShouldSimulatePhysics(false).SetTransactional(false));

	for (const TCHAR* ClassPath : {
		TEXT("/KinetiForge/Template/Vehicles/RWD/SportsCar/BP_SportsCar.BP_SportsCar_C"),
		TEXT("/KinetiForge/Template/Vehicles/RWD/Bus/BP_Bus.BP_Bus_C")})
	{
		UClass* VehicleClass = LoadClass<AActor>(nullptr, ClassPath);
		if (!TestNotNull(FString::Printf(TEXT("Existing asset loads: %s"), ClassPath), VehicleClass)) return false;
		const bool bAssetWasDirty = VehicleClass->GetOutermost()->IsDirty();
		AActor* Vehicle = World->SpawnActor<AActor>(VehicleClass);
		if (!TestNotNull(TEXT("Existing vehicle construction succeeds"), Vehicle)) return false;
		TArray<UVehicleDriveAssemblyComponent*> Drives;
		Vehicle->GetComponents(Drives);
		if (!TestTrue(TEXT("Existing blueprint contains Drive components"), Drives.Num() > 0)) return false;
		for (auto* Drive : Drives)
			TestTrue(TEXT("Existing blueprint inherits the new whole-vehicle default"), Drive->bUseIsolatedVehicleRuntime);
		TArray<UVehicleWheelComponent*> InitialWheels;
		Vehicle->GetComponents(InitialWheels);
		if (!TestTrue(TEXT("OnRegister creates preview wheels before BeginPlay"), InitialWheels.Num() > 0)) return false;
		struct FWheelPreviewIdentity
		{
			UVehicleWheelComponent* Component;
			FName Name;
			UClass* Class;
			EComponentCreationMethod CreationMethod;
			float Radius;
		};
		TArray<FWheelPreviewIdentity> InitialIdentities;
		for (auto* Wheel : InitialWheels)
			InitialIdentities.Add({Wheel, Wheel->GetFName(), Wheel->GetClass(), Wheel->CreationMethod,
				Wheel->GetWheelConfig().Radius});

		// Exercise the actual serialized classes and construction-generated components.
		// These transient instances inherit the new runtime default; no package is saved.
		for (int32 Registration = 0; Registration < 2; ++Registration)
		{
			for (auto* Drive : Drives)
			{
				Drive->UnregisterComponent();
				Drive->RegisterComponent();
			}
			TArray<UVehicleWheelComponent*> CurrentWheels;
			Vehicle->GetComponents(CurrentWheels);
			TestEqual(TEXT("Re-registering preview does not create extra wheels"), CurrentWheels.Num(), InitialWheels.Num());
			for (const auto& Initial : InitialIdentities)
			{
				TestTrue(TEXT("Preview wheel UObject identity is retained"), CurrentWheels.Contains(Initial.Component));
				TestEqual(TEXT("Preview wheel name is retained"), Initial.Component->GetFName(), Initial.Name);
				TestTrue(TEXT("Preview wheel class and Actor ownership are retained"),
					Initial.Component->GetClass() == Initial.Class && Initial.Component->GetOwner() == Vehicle
					&& Initial.Component->GetOuter() == Vehicle);
				TestTrue(TEXT("Preview wheel creation category is retained"), Initial.Component->CreationMethod == Initial.CreationMethod);
				TestEqual(TEXT("Serialized wheel radius is retained"), Initial.Component->GetWheelConfig().Radius, Initial.Radius);
			}
		}
		TestEqual(TEXT("Preview exercise does not dirty the source vehicle asset"),
			VehicleClass->GetOutermost()->IsDirty(), bAssetWasDirty);
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleExistingBlueprintRuntimeTest,
	"KinetiForge.Lifecycle.ExistingBlueprintRuntime",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleExistingBlueprintRuntimeTest::RunTest(const FString& Parameters)
{
	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false, TEXT("VehicleExistingBlueprintRuntime"),
		nullptr, true, ERHIFeatureLevel::Num, nullptr, true);
	if (!TestNotNull(TEXT("Blueprint runtime world created"), World)) return false;
	GEngine->CreateNewWorldContext(EWorldType::Game).SetCurrentWorld(World);
	ON_SCOPE_EXIT { World->DestroyWorld(false); GEngine->DestroyWorldContext(World); };
	World->InitWorld(UWorld::InitializationValues().AllowAudioPlayback(false).CreatePhysicsScene(true)
		.RequiresHitProxies(false).CreateNavigation(false).CreateAISystem(false)
		.ShouldSimulatePhysics(true).SetTransactional(false));
	auto* Solver = World->GetPhysicsScene()->GetSolver();
	Solver->DisableAsyncMode(); // Wait at frame boundaries; the actual solver callback still runs on TaskGraph.
	Solver->SetThreadingMode_External(Chaos::EThreadingModeTemp::TaskGraph);
	auto AdvancePhysics = [&]()
	{
		const FVector Gravity(0.f, 0.f, -980.f);
		World->GetPhysicsScene()->SetUpForFrame(&Gravity, 1.f / 60.f, 0.f, 1.f / 60.f, 1.f / 60.f, 1, false);
		World->GetPhysicsScene()->StartFrame();
		World->GetPhysicsScene()->WaitPhysScenes();
		World->GetPhysicsScene()->EndFrame();
	};

	for (const TCHAR* ClassPath : {
		TEXT("/KinetiForge/Template/Vehicles/RWD/SportsCar/BP_SportsCar.BP_SportsCar_C"),
		TEXT("/KinetiForge/Template/Vehicles/RWD/Bus/BP_Bus.BP_Bus_C")})
	{
		AddInfo(FString::Printf(TEXT("Running original vehicle asset: %s"), ClassPath));
		UClass* VehicleClass = LoadClass<AActor>(nullptr, ClassPath);
		if (!TestNotNull(TEXT("Existing runtime asset loads"), VehicleClass)) return false;
		const bool bAssetWasDirty = VehicleClass->GetOutermost()->IsDirty();
		// Static-mesh collision bodies cannot be created while editor asset compilation is pending.
#if WITH_EDITOR
		FAssetCompilingManager::Get().FinishAllCompilation();
#endif
		AActor* Vehicle = World->SpawnActor<AActor>(VehicleClass, FVector(0.f, 0.f, 10000.f), FRotator::ZeroRotator);
		if (!TestNotNull(TEXT("Original vehicle spawns in game world"), Vehicle)) return false;
		Vehicle->DispatchBeginPlay(); // Includes the original Blueprint BeginPlay graph, without a player/controller.
		TestTrue(TEXT("Original vehicle completes BeginPlay"), Vehicle->HasActorBegunPlay());
		TArray<UVehicleDriveAssemblyComponent*> Drives;
		Vehicle->GetComponents(Drives);
		if (!TestTrue(TEXT("Original vehicle has a Drive"), Drives.Num() > 0)) return false;
		for (auto* Drive : Drives) Drive->TickComponent(1.f / 60.f, LEVELTICK_All, nullptr);
		for (auto* Drive : Drives)
		{
			const auto Step = Drive->GetPhysicsStep();
			if (!TestTrue(TEXT("Original asset binds active automatic whole-vehicle entry"),
				Drive->bUpdatePhysicsAutomatically && Step && !Step->IsStopped())) return false;
		}
		AdvancePhysics(); // Consume construction/BeginPlay registrations before recording sequences.

		auto Host = FModuleManager::Get().GetModuleChecked<FKinetiForgeModule>(TEXT("KinetiForge"))
			.AcquirePhysicsScene(World->GetPhysicsScene());
		if (!TestTrue(TEXT("Original vehicle has a live scene host"), Host.IsValid() && !Host->IsClosed())) return false;
		uint32 CallbackThreadId = FPlatformTLS::GetCurrentThreadId();
		int32 CallbackCount = 0;
		auto Witness = Host->RegisterStep([&](float, const FVehiclePhysicsExecutionContext&)
		{
			CallbackThreadId = FPlatformTLS::GetCurrentThreadId();
			++CallbackCount;
		});
		if (!TestNotNull(TEXT("Worker witness registers"), Witness.Get())) return false;
		ON_SCOPE_EXIT { Host->UnregisterStep(Witness); };
		TArray<UVehicleWheelComponent*> InitialWheels;
		Vehicle->GetComponents(InitialWheels);
		if (!TestTrue(TEXT("Original vehicle has generated wheels"), InitialWheels.Num() > 0)) return false;
		TArray<uint64> Sequences;
		TArray<TSharedPtr<FVehicleWheelBridge, ESPMode::ThreadSafe>> WheelBridges;
		for (auto* Wheel : InitialWheels)
		{
			if (!TestTrue(TEXT("Every original wheel has a runtime bridge"), Wheel->GetWheelBridge().IsValid())) return false;
			Sequences.Add(Wheel->GetWheelBridge()->ReadResult().PublicationSequence);
			WheelBridges.Add(Wheel->GetWheelBridge());
		}
		for (auto* Drive : Drives)
		{
			if (!TestTrue(TEXT("Original Drive binds complete runtime"), Drive->GetPowerUnitBridge().IsValid())) return false;
			Drive->InputThrottle(0.65f, true);
			Drive->InputSteering(0.2f, true);
			Drive->GetEngine()->StartVehicleEngine();
		}
		for (int32 Frame = 0; Frame < 6; ++Frame)
		{
			if (Frame == 3)
			{
				// Re-register live original components, preserving their serialized names and wheel objects.
				for (auto* Drive : Drives)
				{
					Drive->UnregisterComponent();
					Drive->RegisterComponent();
				}
			}
			for (auto* Drive : Drives) Drive->TickComponent(1.f / 60.f, LEVELTICK_All, nullptr);
			AdvancePhysics();
			for (int32 Index = 0; Index < InitialWheels.Num(); ++Index)
			{
				TestTrue(TEXT("Re-registration retains the wheel result bridge"), InitialWheels[Index]->GetWheelBridge() == WheelBridges[Index]);
				TestEqual(TEXT("Original asset wheel publishes exactly once per solver step"),
					WheelBridges[Index]->ReadResult().PublicationSequence, ++Sequences[Index]);
			}
			for (auto* Drive : Drives)
			{
				const auto Result = Drive->GetPowerUnitBridge()->ReadResult();
				TestEqual(TEXT("Player throttle reaches runtime raw input"), Result.InputValues.Raw.Throttle, 0.65f);
				TestEqual(TEXT("Player steering reaches runtime raw input"), Result.InputValues.Raw.Steering, 0.2f);
			}
		}
		TestEqual(TEXT("Actual worker callback ran for every observed step"), CallbackCount, 6);
		TestTrue(TEXT("Original assets advance on TaskGraph worker"), CallbackThreadId != FPlatformTLS::GetCurrentThreadId());
		TArray<UVehicleWheelComponent*> CurrentWheels;
		Vehicle->GetComponents(CurrentWheels);
		TestEqual(TEXT("Live re-registration creates no extra wheels"), CurrentWheels.Num(), InitialWheels.Num());
		for (auto* Wheel : InitialWheels) TestTrue(TEXT("Original wheel identities survive BeginPlay and re-registration"), CurrentWheels.Contains(Wheel));
		TestEqual(TEXT("Runtime exercise does not dirty the original asset"), VehicleClass->GetOutermost()->IsDirty(), bAssetWasDirty);
		Vehicle->Destroy();
		TestTrue(TEXT("Normal actor destruction enters teardown"), Vehicle->IsActorBeingDestroyed());
		AdvancePhysics(); // A stale scene snapshot must remain inert after normal actor teardown.
		for (int32 Index = 0; Index < InitialWheels.Num(); ++Index)
			TestEqual(TEXT("Destroyed vehicle no longer publishes wheel state"),
				WheelBridges[Index]->ReadResult().PublicationSequence, Sequences[Index]);
	}
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
