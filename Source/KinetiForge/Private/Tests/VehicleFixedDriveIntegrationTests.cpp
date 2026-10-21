// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "VehicleDriveAssemblyComponent.h"
#include "VehicleFrozenBaseline.h"
#include "VehicleAsyncTickComponent.h"
#include "VehicleAxleAssemblyComponent.h"
#include "VehicleWheelComponent.h"
#include "VehicleWheelBridge.h"
#include "VehicleEngineComponent.h"
#include "VehicleGearboxComponent.h"
#include "VehiclePhysicsStep.h"
#include "VehiclePhysicsScene.h"
#include "VehicleUtilities.h"
#include "Components/BoxComponent.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "PBDRigidsSolver.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Misc/AutomationTest.h"
#include "Modules/ModuleManager.h"
#include "UObject/StrongObjectPtr.h"
#include "Misc/ScopeExit.h"

struct FVehicleFixedDriveAssemblyIntegrationProbe
{
	static TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe> Step(UVehicleDriveAssemblyComponent& Drive)
	{
		return Drive.GetPhysicsStep();
	}
	static void Tick(UVehicleDriveAssemblyComponent& Drive)
	{
		Drive.TickComponent(1.f / 60.f, LEVELTICK_All, nullptr);
	}
};

namespace VehicleFixedDriveIntegrationTests
{
	struct FWorldOwner
	{
		UWorld* World = nullptr;
		~FWorldOwner() { if (World) World->DestroyWorld(false); }
	};

	UWorld* CreateWorld(FAutomationTestBase& Test, FWorldOwner& Owner)
	{
		Owner.World = UWorld::CreateWorld(EWorldType::Game, false, TEXT("VehicleFixedDriveIntegration"), nullptr,
			true, ERHIFeatureLevel::Num, nullptr, true);
		if (!Test.TestNotNull(TEXT("Integration world created"), Owner.World)) return nullptr;
		Owner.World->InitWorld(UWorld::InitializationValues().AllowAudioPlayback(false)
			.CreatePhysicsScene(true).RequiresHitProxies(false).CreateNavigation(false)
			.CreateAISystem(false).ShouldSimulatePhysics(false).SetTransactional(false));
		if (!Test.TestNotNull(TEXT("Integration world has a Chaos scene"), Owner.World->GetPhysicsScene())) return nullptr;
		Owner.World->GetPhysicsScene()->GetSolver()->DisableAsyncMode();
		Owner.World->GetPhysicsScene()->GetSolver()->SetThreadingMode_External(Chaos::EThreadingModeTemp::SingleThread);
		return Owner.World;
	}

	void Flush(UWorld& World)
	{
		const FVector Gravity = FVector::ZeroVector;
		World.GetPhysicsScene()->SetUpForFrame(&Gravity, 1.f / 60.f, 0.f, 1.f / 60.f, 1.f / 60.f, 1, false);
		World.GetPhysicsScene()->StartFrame();
		World.GetPhysicsScene()->EndFrame();
		World.GetPhysicsScene()->WaitPhysScenes();
	}

	struct FVehicle
	{
		APawn* Actor = nullptr;
		UBoxComponent* Chassis = nullptr;
		UVehicleAxleAssemblyComponent* Axle = nullptr;
		UVehicleDriveAssemblyComponent* Drive = nullptr;
	};

	FVehicle CreateVehicle(UWorld& World, const FVector& Location,
		const bool bSolidAxle, const float SwaybarStiffness, const bool bFrozenLane = false)
	{
		FVehicle Vehicle;
		Vehicle.Actor = World.SpawnActor<APawn>(Location, FRotator::ZeroRotator);
		if (!Vehicle.Actor) return Vehicle;

		Vehicle.Chassis = NewObject<UBoxComponent>(Vehicle.Actor, TEXT("IntegrationChassis"));
		Vehicle.Actor->AddInstanceComponent(Vehicle.Chassis);
		Vehicle.Actor->SetRootComponent(Vehicle.Chassis);
		Vehicle.Chassis->SetBoxExtent(FVector(50.f));
		Vehicle.Chassis->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Vehicle.Chassis->SetSimulatePhysics(true);
		Vehicle.Chassis->RegisterComponent();
		Vehicle.Chassis->RecreatePhysicsState();

		// Configure the generated wheel pair before the isolated axle bridge captures its settings.
		Vehicle.Axle = NewObject<UVehicleAxleAssemblyComponent>(Vehicle.Actor, TEXT("IntegrationAxle"));
		Vehicle.Actor->AddInstanceComponent(Vehicle.Axle);
		Vehicle.Axle->SetupAttachment(Vehicle.Chassis);
		FVehicleAxleConfig AxleConfig = Vehicle.Axle->GetAxleConfig();
		AxleConfig.SwaybarStiffness = SwaybarStiffness;
		Vehicle.Axle->SetAxleConfig(AxleConfig);
		Vehicle.Axle->SetAxleSuspensionType(bSolidAxle ? EVehicleAxleSuspensionType::Solid : EVehicleAxleSuspensionType::Independent);
		Vehicle.Axle->RegisterComponent();
		if (bFrozenLane)
		{
			UVehicleAsyncTickComponent* Tick = NewObject<UVehicleAsyncTickComponent>(Vehicle.Actor, TEXT("FrozenIntegrationTicker"));
			Vehicle.Actor->AddInstanceComponent(Tick);
			Tick->bUpdatePhysicsOnGameThread = true;
			Tick->RegisterComponent();
		}

		Vehicle.Drive = NewObject<UVehicleDriveAssemblyComponent>(Vehicle.Actor, TEXT("IntegrationDrive"));
		Vehicle.Actor->AddInstanceComponent(Vehicle.Drive);
		Vehicle.Drive->SetupAttachment(Vehicle.Chassis);
		Vehicle.Drive->bUpdatePhysicsAutomatically = true;
		Vehicle.Drive->AutoGearboxConfig.bAutomaticGearbox = false;
		FVehicleAxleSpawnTemplate AxleTemplate;
		AxleTemplate.bUseExistingComponent = true;
		AxleTemplate.AxleComponentName = Vehicle.Axle->GetFName();
		Vehicle.Drive->AxleTemplates = {AxleTemplate};
		Vehicle.Drive->RegisterComponent();
		return Vehicle;
	}

	void PrepareVehicle(FVehicle& Vehicle)
	{
		Vehicle.Actor->DispatchBeginPlay();
		// BeginPlay may precede generated physics bodies and child BeginPlay. Two GT refreshes
		// let the deferred power bridge initialize and then publish a body-bound scene step.
		FVehicleFixedDriveAssemblyIntegrationProbe::Tick(*Vehicle.Drive);
		FVehicleFixedDriveAssemblyIntegrationProbe::Tick(*Vehicle.Drive);
		FVehicleNaturallyAspiratedEngineConfig EngineConfig = Vehicle.Drive->GetEngine()->GetNaturallyAspiratedEngineConfig();
		EngineConfig.UnburntFuelAccumulationRate = 0.f;
		Vehicle.Drive->GetEngine()->SetNaturallyAspiratedEngineConfig(EngineConfig);
		Vehicle.Drive->InputAssistConfig.bAutomaticClutch = false;
		Vehicle.Drive->InputAssistConfig.bAutoHold = false;
		Vehicle.Drive->GetEngine()->StartVehicleEngine();
		Vehicle.Drive->GetGearbox()->ShiftToTargetGear(1, true);
	}

	void RefreshPublishedState(FVehicle& Vehicle)
	{
		Vehicle.Drive->GetEngine()->RefreshPublishedRuntimeState();
		Vehicle.Axle->RefreshPublishedAxleState();
		for (UVehicleWheelComponent* Wheel : Vehicle.Drive->GetWheels()) Wheel->RefreshPublishedWheelState();
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleFixedDriveSceneIntegrationTest,
	"KinetiForge.Drivetrain.FixedDrive.IndependentFrozenBaseline",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleFixedDriveSceneIntegrationTest::RunTest(const FString& Parameters)
{
	using namespace VehicleFixedDriveIntegrationTests;
	FVehicleFrozenBaseline Baseline;
	if (!Baseline.Load(*this, TEXT("independent.csv"), 36, 10)) return false;
	FWorldOwner Owner;
	UWorld* World = CreateWorld(*this, Owner);
	if (!World) return false;
	FVehicle Vehicle = CreateVehicle(*World, FVector(0.f, 0.f, 1000.f), false, 12000.f);
	if (!TestTrue(TEXT("Complete independent vehicle registered"), Vehicle.Drive && Vehicle.Axle && Vehicle.Chassis)) return false;
	Flush(*World);
	PrepareVehicle(Vehicle);
	const auto Entry = FVehicleFixedDriveAssemblyIntegrationProbe::Step(*Vehicle.Drive);
	if (!TestTrue(TEXT("Vehicle owns an active scene entry"), Entry && !Entry->IsStopped())) return false;
	if (!TestEqual(TEXT("Independent fixture has two wheels"), Vehicle.Drive->GetWheels().Num(), 2)) return false;
	for (int32 Frame = 0; Frame < 36; ++Frame)
	{
		Vehicle.Drive->InputThrottle(Frame < 18 ? 0.65f : 0.25f, Frame == 0);
		Vehicle.Drive->InputBrake(Frame > 28 ? 0.2f : 0.f);
		Vehicle.Drive->InputSteering(0.2f);
		Flush(*World);
		RefreshPublishedState(Vehicle);
		const auto Wheels = Vehicle.Drive->GetWheels();
		TestTrue(TEXT("Engine actually advances"), Vehicle.Drive->GetEngine()->GetRPM() > 0.f);
		Baseline.CheckRow(*this, Frame, {
			Vehicle.Drive->GetEngine()->GetRPM(), Vehicle.Axle->GetAngularVelocity(),
			Wheels[0]->GetAngularVelocity(), Wheels[0]->GetSuspensionLength(),
			Wheels[0]->GetSuspensionState().StaticSprungMass, Wheels[0]->GetHubChassisLocation().Z,
			Wheels[1]->GetAngularVelocity(), Wheels[1]->GetSuspensionLength(),
			Wheels[1]->GetSuspensionState().StaticSprungMass, Wheels[1]->GetHubChassisLocation().Z},
			{0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01});
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleFixedDriveSolidAxleComparisonTest,
	"KinetiForge.Drivetrain.FixedDrive.SolidAxleFrozenBaseline",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleFixedDriveSolidAxleComparisonTest::RunTest(const FString& Parameters)
{
	using namespace VehicleFixedDriveIntegrationTests;
	FVehicleFrozenBaseline Baseline;
	if (!Baseline.Load(*this, TEXT("solid.csv"), 18, 3)) return false;
	FWorldOwner Owner;
	UWorld* World = CreateWorld(*this, Owner);
	if (!World) return false;
	FVehicle Vehicle = CreateVehicle(*World, FVector(0.f, 0.f, 1000.f), true, 2000.f);
	if (!TestTrue(TEXT("Complete solid-axle vehicle registered"), Vehicle.Drive && Vehicle.Axle)) return false;
	Flush(*World);
	PrepareVehicle(Vehicle);
	if (!TestEqual(TEXT("Solid axle fixture has two wheels"), Vehicle.Drive->GetWheels().Num(), 2)) return false;
	for (int32 Frame = 0; Frame < 18; ++Frame)
	{
		Vehicle.Drive->InputThrottle(0.55f, Frame == 0);
		Flush(*World);
		RefreshPublishedState(Vehicle);
		const auto Wheels = Vehicle.Drive->GetWheels();
		Baseline.CheckRow(*this, Frame, {Vehicle.Axle->GetAngularVelocity(),
			Wheels[0]->GetAngularVelocity(), Wheels[1]->GetAngularVelocity()}, {0.01,0.01,0.01});
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleFixedDriveLifecycleIntegrationTest,
	"KinetiForge.Lifecycle.FixedDriveBodyRecreationAndFrozenLane",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleFixedDriveLifecycleIntegrationTest::RunTest(const FString& Parameters)
{
	using namespace VehicleFixedDriveIntegrationTests;
	FWorldOwner WorldOwner;
	UWorld* World = CreateWorld(*this, WorldOwner);
	if (!World) return false;
	FVehicle Vehicle = CreateVehicle(*World, FVector(0.f, 0.f, 1000.f), true, 8000.f);
	if (!TestTrue(TEXT("Solid-axle fixed vehicle registered"), Vehicle.Actor && Vehicle.Drive && Vehicle.Axle)) return false;
	Flush(*World);
	PrepareVehicle(Vehicle);
	TestTrue(TEXT("Solid axle retains its configured suspension type"),
		Vehicle.Axle->GetAxleSuspensionType() == EVehicleAxleSuspensionType::Solid);
	TestEqual(TEXT("Solid axle retains both wheel bindings"), Vehicle.Drive->GetWheels().Num(), 2);
	const auto OldEntry = FVehicleFixedDriveAssemblyIntegrationProbe::Step(*Vehicle.Drive);
	if (!TestNotNull(TEXT("Fixed runtime entry published"), OldEntry.Get())) return false;
	TestFalse(TEXT("Public float-only manual update rejects a context-required fixed runtime"), OldEntry->TryExecute(1.f / 60.f));

	Vehicle.Chassis->RecreatePhysicsState();
	FVehicleFixedDriveAssemblyIntegrationProbe::Tick(*Vehicle.Drive);
	TestTrue(TEXT("Body recreation permanently closes the old runtime entry"), OldEntry->IsStopped());
	TestFalse(TEXT("Retained pre-recreation entry cannot advance"), OldEntry->TryExecute(1.f / 60.f));
	Flush(*World);
	FVehicleFixedDriveAssemblyIntegrationProbe::Tick(*Vehicle.Drive);
	const auto NewEntry = FVehicleFixedDriveAssemblyIntegrationProbe::Step(*Vehicle.Drive);
	TestTrue(TEXT("Body recreation publishes a fresh entry after rebinding"), NewEntry.IsValid() && NewEntry != OldEntry && !NewEntry->IsStopped());

	const auto CurrentEntry = FVehicleFixedDriveAssemblyIntegrationProbe::Step(*Vehicle.Drive);
	TestTrue(TEXT("Rebound entry is current and active"), CurrentEntry.IsValid() && CurrentEntry != OldEntry && !CurrentEntry->IsStopped());
	TestFalse(TEXT("Pre-body-recreation entry stays stopped after rebind"), OldEntry->TryExecute(1.f / 60.f));
	const TArray<UVehicleWheelComponent*> OriginalWheels = Vehicle.Drive->GetWheels();
	Vehicle.Drive->UnregisterComponent();
	TestTrue(TEXT("Drive unregister stops its current retained step"), CurrentEntry->IsStopped());
	Vehicle.Drive->RegisterComponent();
	const auto DriveRestoredEntry = FVehicleFixedDriveAssemblyIntegrationProbe::Step(*Vehicle.Drive);
	TestTrue(TEXT("Drive registration publishes another fresh fixed entry"), DriveRestoredEntry.IsValid()
		&& DriveRestoredEntry != CurrentEntry && !DriveRestoredEntry->IsStopped());
	TestTrue(TEXT("Drive registration preserves the generated wheel component identities"), Vehicle.Drive->GetWheels() == OriginalWheels);
	TestFalse(TEXT("Pre-unregister entry remains stopped"), CurrentEntry->TryExecute(1.f / 60.f));
	if (!TestEqual(TEXT("Wheel-loss fixture begins with two wheels"), OriginalWheels.Num(), 2)) return false;
	UVehicleWheelComponent* Survivor = OriginalWheels[0];
	UVehicleWheelComponent* RemovedWheel = OriginalWheels[1];
	const auto BeforeWheelLossEntry = FVehicleFixedDriveAssemblyIntegrationProbe::Step(*Vehicle.Drive);
	const auto SurvivorBridge = Survivor->GetWheelBridge();
	if (!TestNotNull(TEXT("Surviving wheel has a fixed-runtime bridge"), SurvivorBridge.Get())) return false;
	const uint64 BeforeWheelLossSequence = SurvivorBridge->ReadResult().PublicationSequence;
	RemovedWheel->DestroyComponent();
	TestTrue(TEXT("Wheel destruction closes the old fixed entry"), BeforeWheelLossEntry->IsStopped());
	FVehicleFixedDriveAssemblyIntegrationProbe::Tick(*Vehicle.Drive);
	const auto SingleWheelEntry = FVehicleFixedDriveAssemblyIntegrationProbe::Step(*Vehicle.Drive);
	if (!TestTrue(TEXT("Wheel loss rebinds one active fixed entry"), SingleWheelEntry.IsValid()
		&& SingleWheelEntry != BeforeWheelLossEntry && !SingleWheelEntry->IsStopped())) return false;
	int32 LiveWheelCount = 0;
	for (UVehicleWheelComponent* Wheel : Vehicle.Drive->GetWheels())
		if (IsValid(Wheel) && !Wheel->IsBeingDestroyed()) ++LiveWheelCount;
	if (!TestEqual(TEXT("Wheel loss leaves one live generated wheel binding"), LiveWheelCount, 1)) return false;
	uint64 PreviousPublicationSequence = BeforeWheelLossSequence;
	for (int32 Frame = 0; Frame < 3; ++Frame)
	{
		Flush(*World);
		Vehicle.Axle->RefreshPublishedAxleState();
		Survivor->RefreshPublishedWheelState();
		const uint64 PublishedSequence = SurvivorBridge->ReadResult().PublicationSequence;
		TestEqual(TEXT("Single-wheel axle state reports one wheel"), Vehicle.Axle->GetAxleState().NumOfWheels, 1);
		TestEqual(TEXT("Single-wheel runtime publishes exactly once per scene frame"), PublishedSequence, PreviousPublicationSequence + 1);
		PreviousPublicationSequence = PublishedSequence;
	}

	FVehicle FrozenVehicle = CreateVehicle(*World, FVector(1000.f, 0.f, 1000.f), true, 8000.f, true);
	if (!TestTrue(TEXT("Frozen-lane fixed vehicle registered"), FrozenVehicle.Actor && FrozenVehicle.Drive)) return false;
	Flush(*World);
	PrepareVehicle(FrozenVehicle);
	const auto FrozenEntry = FVehicleFixedDriveAssemblyIntegrationProbe::Step(*FrozenVehicle.Drive);
	if (!TestTrue(TEXT("Frozen GT lane publishes an active fixed-runtime entry"), FrozenEntry.IsValid() && !FrozenEntry->IsStopped())) return false;
	const auto FrozenWheelBridge = FrozenVehicle.Drive->GetWheels()[0]->GetWheelBridge();
	const uint64 BeforeFrozenStep = FrozenWheelBridge->ReadResult().PublicationSequence;
	Flush(*World);
	TestEqual(TEXT("Frozen fixed runtime publishes exactly once without legacy ticker duplication"),
		FrozenWheelBridge->ReadResult().PublicationSequence, BeforeFrozenStep + 1);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleFixedDriveManualCallbackTest,
	"KinetiForge.Drivetrain.FixedDrive.ManualCallbackContext",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleFixedDriveManualCallbackTest::RunTest(const FString& Parameters)
{
	using namespace VehicleFixedDriveIntegrationTests;
	FWorldOwner Owner;
	UWorld* World = CreateWorld(*this, Owner);
	if (!World) return false;
	FVehicle Vehicle = CreateVehicle(*World, FVector(0.f, 0.f, 1000.f), false, 8000.f);
	if (!Vehicle.Drive) return false;
	Flush(*World);
	PrepareVehicle(Vehicle);
	Vehicle.Drive->bUpdatePhysicsAutomatically = false;
	FVehicleFixedDriveAssemblyIntegrationProbe::Tick(*Vehicle.Drive);
	const auto WheelBridge = Vehicle.Drive->GetWheels()[0]->GetWheelBridge();
	const uint64 InitialSequence = WheelBridge->ReadResult().PublicationSequence;
	Vehicle.Drive->UpdatePhysics(1.f / 60.f);
	Flush(*World);
	TestEqual(TEXT("Ordinary GT manual call and disabled automatic entry do not advance fixed runtime"),
		WheelBridge->ReadResult().PublicationSequence, InitialSequence);
	TestNull(TEXT("No context retained on GT outside callback"), FVehiclePhysicsExecutionContext::GetCurrent());

	auto& Module = FModuleManager::Get().GetModuleChecked<FKinetiForgeModule>(TEXT("KinetiForge"));
	auto Host = Module.AcquirePhysicsScene(World->GetPhysicsScene());
	bool bScopedContextCorrect = false;
	bool bNestedContextRestored = false;
	// This test intentionally uses single-thread Chaos, then the real frozen lane. Both
	// execute actual callback dispatch; ExecutionLanes separately verifies worker placement.
	auto Caller = Host->RegisterStep([&](float DeltaTime, const FVehiclePhysicsExecutionContext& Context)
	{
		bScopedContextCorrect = FVehiclePhysicsExecutionContext::GetCurrent() == &Context;
		{
			const FVehiclePhysicsExecutionContext NestedContext(Context.GetSolver());
			const FVehiclePhysicsExecutionScope NestedScope(NestedContext);
			bNestedContextRestored = FVehiclePhysicsExecutionContext::GetCurrent() == &NestedContext;
		}
		bNestedContextRestored &= FVehiclePhysicsExecutionContext::GetCurrent() == &Context;
		Vehicle.Drive->UpdatePhysics(DeltaTime); // The existing Blueprint-visible signature.
	}, true, EVehiclePhysicsExecutionLane::FrozenGameThread);
	ON_SCOPE_EXIT { Host->UnregisterStep(Caller); };
	Flush(*World);
	TestTrue(TEXT("Actual callback exposes its scoped context"), bScopedContextCorrect);
	TestTrue(TEXT("Nested borrowed scope restores outer callback context"), bNestedContextRestored);
	TestEqual(TEXT("Float-only manual callback advances fixed runtime once"),
		WheelBridge->ReadResult().PublicationSequence, InitialSequence + 1);
	TestNull(TEXT("Callback scope clears borrowed context after return"), FVehiclePhysicsExecutionContext::GetCurrent());

	// A callback belonging to another scene must not resolve this vehicle's bodies.
	FWorldOwner OtherOwner;
	UWorld* OtherWorld = CreateWorld(*this, OtherOwner);
	if (!OtherWorld) return false;
	auto OtherHost = Module.AcquirePhysicsScene(OtherWorld->GetPhysicsScene());
	auto WrongSceneCaller = OtherHost->RegisterStep([&](float DeltaTime, const FVehiclePhysicsExecutionContext&)
	{
		Vehicle.Drive->UpdatePhysics(DeltaTime);
	});
	ON_SCOPE_EXIT { OtherHost->UnregisterStep(WrongSceneCaller); };
	Flush(*OtherWorld);
	TestEqual(TEXT("Another scene callback cannot advance this vehicle"),
		WheelBridge->ReadResult().PublicationSequence, InitialSequence + 1);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
