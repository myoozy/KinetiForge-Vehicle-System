// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/WorldSettings.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "VehicleWheelComponent.h"
#include "VehicleWheelBridge.h"
#include "Misc/AutomationTest.h"
#include "Misc/ScopeExit.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "UObject/StrongObjectPtr.h"
#if WITH_EDITOR
#include "AssetCompilingManager.h"
#endif

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleForcedGCLifecycleTest,
	"KinetiForge.Lifecycle.ForcedGCRandomDestruction",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleForcedGCLifecycleTest::RunTest(const FString& Parameters)
{
	FString ClassPath = TEXT("/KinetiForge/Template/Vehicles/RWD/SportsCar/BP_SportsCar.BP_SportsCar_C");
	FParse::Value(FCommandLine::Get(), TEXT("KinetiForgeGCStressVehicle="), ClassPath);
	int32 Cycles = 128;
	FParse::Value(FCommandLine::Get(), TEXT("KinetiForgeGCStressIterations="), Cycles);
	if (!TestTrue(TEXT("Stress has enough cycles for both teardown routes"), Cycles >= 16)) return false;
	TStrongObjectPtr<UClass> VehicleClass(LoadClass<AActor>(nullptr, *ClassPath));
	if (!TestNotNull(TEXT("Stress vehicle class loads"), VehicleClass.Get())) return false;
#if WITH_EDITOR
	FAssetCompilingManager::Get().FinishAllCompilation();
#endif
	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false, TEXT("VehicleForcedGC"),
		nullptr, true, ERHIFeatureLevel::Num, nullptr, true);
	if (!TestNotNull(TEXT("Stress world created"), World)) return false;
	GEngine->CreateNewWorldContext(EWorldType::Game).SetCurrentWorld(World);
	ON_SCOPE_EXIT { World->DestroyWorld(false); GEngine->DestroyWorldContext(World); };
	World->InitWorld(UWorld::InitializationValues().AllowAudioPlayback(false).CreatePhysicsScene(true)
		.RequiresHitProxies(false).CreateNavigation(false).CreateAISystem(false)
		.ShouldSimulatePhysics(true).SetTransactional(false));
	World->InitializeActorsForPlay(FURL());
	World->GetWorldSettings()->NotifyBeginPlay();
	FRandomStream Random(17319);
	TArray<TWeakObjectPtr<AActor>> Vehicles;
	int32 DestroyedActors = 0, ComponentTeardowns = 0, WholeActorTeardowns = 0;
	bool bObservedWheelPublication = false;
	for (int32 Cycle = 0; Cycle < Cycles; ++Cycle)
	{
		FActorSpawnParameters Spawn;
		Spawn.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* Vehicle = World->SpawnActor<AActor>(VehicleClass.Get(), FVector(Cycle * 500.f, 0.f, 10000.f),
			FRotator::ZeroRotator, Spawn);
		if (!TestNotNull(TEXT("Stress vehicle spawned"), Vehicle)) return false;
		if (!TestTrue(TEXT("Stress actor BeginPlay ran"), Vehicle->HasActorBegunPlay())) return false;
		Vehicles.Add(Vehicle);

		// A later-tick native component is the control: timer-triggered obj gc can destroy
		// its queued FTickFunction before TG_PostUpdateWork. The Blueprint GC node must defer.
		AActor* NativeControl = World->SpawnActor<AActor>();
		USceneComponent* NativeComponent = NewObject<USceneComponent>(NativeControl);
		NativeControl->AddInstanceComponent(NativeComponent);
		NativeComponent->PrimaryComponentTick.bCanEverTick = true;
		NativeComponent->PrimaryComponentTick.TickGroup = TG_PostUpdateWork;
		NativeComponent->RegisterComponent();
		if (!TestTrue(TEXT("Native later-tick control is registered"),
			NativeComponent->PrimaryComponentTick.IsTickFunctionRegistered())) return false;
		TWeakObjectPtr<USceneComponent> NativeIdentity(NativeComponent);
		TArray<TWeakObjectPtr<UActorComponent>> RetiredComponents;
		TArray<TPair<TSharedPtr<FVehicleWheelBridge, ESPMode::ThreadSafe>, uint64>> RetiredWheels;
		bool bTimerRan = false;
		World->GetTimerManager().SetTimerForNextTick([&]()
		{
			bTimerRan = true;
			NativeControl->Destroy();
			if (Vehicles.Num() > 3)
			{
				const int32 Index = Random.RandRange(0, Vehicles.Num() - 1);
				AActor* Victim = Vehicles[Index].Get();
				Vehicles.RemoveAtSwap(Index);
				if (!TestNotNull(TEXT("Live stress victim"), Victim)) return;
				TArray<UActorComponent*> Components;
				Victim->GetComponents(Components);
				for (auto* Component : Components)
				{
					RetiredComponents.Add(Component);
					if (auto* Wheel = Cast<UVehicleWheelComponent>(Component))
					{
						if (auto Bridge = Wheel->GetWheelBridge())
						{
							const uint64 Sequence = Bridge->ReadResult().PublicationSequence;
							bObservedWheelPublication |= Sequence > 1;
							RetiredWheels.Emplace(Bridge, Sequence);
						}
					}
				}
				if (Random.RandRange(0, 1))
				{
					++ComponentTeardowns;
					while (Components.Num())
					{
						const int32 ComponentIndex = Random.RandRange(0, Components.Num() - 1);
						UActorComponent* Component = Components[ComponentIndex];
						Components.RemoveAtSwap(ComponentIndex);
						if (!IsValid(Component)) continue;
						Component->DestroyComponent();
						TestFalse(TEXT("Destroyed component primary tick is unregistered"),
							Component->PrimaryComponentTick.IsTickFunctionRegistered());
					}
				}
				else ++WholeActorTeardowns;
				Victim->Destroy();
				// Stop/Invalidate waits for admitted work. Record the completed teardown boundary,
				// rather than rejecting a legitimate publication that finished while stopping.
				for (auto& Wheel : RetiredWheels) Wheel.Value = Wheel.Key->ReadResult().PublicationSequence;
				++DestroyedActors;
			}
			UKismetSystemLibrary::CollectGarbage(); // Same implementation as the Blueprint Collect Garbage node.
			TestNotNull(TEXT("GC request preserves queued native tick storage until frame end"), NativeIdentity.Get(true));
		});
		++GFrameCounter;
		World->Tick(LEVELTICK_All, 1.f / 60.f);
		if (!TestTrue(TEXT("Timer mutation actually executed"), bTimerRan)) return false;
		// The test owns a nested World tick; explicitly perform the engine's normal frame-end GC here.
		GEngine->ConditionalCollectGarbage();
		TestNull(TEXT("Frame-end full purge actually reclaims native control"), NativeIdentity.Get(true));
		for (const auto& Component : RetiredComponents)
			TestNull(TEXT("Frame-end full purge actually reclaims retired vehicle component"), Component.Get(true));
		for (const auto& Wheel : RetiredWheels)
			TestEqual(TEXT("Retired runtime remains inert after teardown and GC"), Wheel.Key->ReadResult().PublicationSequence, Wheel.Value);
	}
	for (const auto& Vehicle : Vehicles)
	{
		if (Vehicle.IsValid()) { Vehicle->Destroy(); ++DestroyedActors; }
	}
	UKismetSystemLibrary::CollectGarbage();
	++GFrameCounter;
	World->Tick(LEVELTICK_All, 1.f / 60.f);
	GEngine->ConditionalCollectGarbage();
	TestEqual(TEXT("All spawned vehicles destroyed"), DestroyedActors, Cycles);
	TestTrue(TEXT("Random component-first destruction exercised"), ComponentTeardowns > 0);
	TestTrue(TEXT("Whole actor destruction exercised"), WholeActorTeardowns > 0);
	TestTrue(TEXT("Actual wheel runtime published before destruction"), bObservedWheelPublication);
	AddInfo(FString::Printf(TEXT("%s: %d spawn/destruction cycles, %d component-first, %d whole actor, %d deferred GC boundaries"),
		*ClassPath, Cycles, ComponentTeardowns, WholeActorTeardowns, Cycles + 1));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
