// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "VehicleDriveAssemblyComponent.h"
#include "VehicleAsyncTickComponent.h"
#include "VehicleEngineComponent.h"
#include "VehicleClutchComponent.h"
#include "VehicleGearboxComponent.h"
#include "VehicleDifferentialComponent.h"
#include "VehicleAxleAssemblyComponent.h"
#include "VehicleAxleBridge.h"
#include "VehicleUtilities.h"
#include "VehiclePowerUnitBridge.h"
#include "VehiclePhysicsStep.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Misc/AutomationTest.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "PBDRigidsSolver.h"

struct FVehiclePowerLifetimeProbe
{
	static TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe> Step(const UVehicleDriveAssemblyComponent& Drive)
	{
		return Drive.GetPhysicsStep();
	}
	static TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> Bridge(const UVehicleDriveAssemblyComponent& Drive)
	{
		return Drive.GetPowerUnitBridge();
	}
	static bool InitializeBridge(UVehicleDriveAssemblyComponent& Drive) { return Drive.InitializePowerUnitBridge(); }
	static bool Prepare(UVehicleDriveAssemblyComponent& Drive) { return Drive.PreparePhysicsStepRegistration(); }
	static void Tick(UVehicleDriveAssemblyComponent& Drive) { Drive.TickComponent(1.f / 60.f, LEVELTICK_All, nullptr); }
	static void SetPowerComponents(UVehicleDriveAssemblyComponent& Drive,
		UVehicleEngineComponent* Engine, UVehicleClutchComponent* Clutch, UVehicleGearboxComponent* Gearbox)
	{
		Drive.Engine = Engine;
		Drive.Clutch = Clutch;
		Drive.Gearbox = Gearbox;
	}
	static bool HasOwner(const UVehicleEngineComponent& Component) { return Component.PhysicsOwnership.HasActiveOwner(); }
	static bool HasOwner(const UVehicleClutchComponent& Component) { return Component.PhysicsOwnership.HasActiveOwner(); }
	static bool HasOwner(const UVehicleGearboxComponent& Component) { return Component.PhysicsOwnership.HasActiveOwner(); }
	static TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe> ComponentBridge(const UVehicleEngineComponent& Component)
	{
		return Component.PowerUnitBridge.Pin();
	}
	static void SetTransferCase(UVehicleDriveAssemblyComponent& Drive, UVehicleDifferentialComponent* Diff)
	{
		Drive.TransferCase = Diff;
	}
};

namespace VehiclePowerLifetimeTest
{
	struct FVehicle
	{
		APawn* Actor = nullptr;
		UBoxComponent* Chassis = nullptr;
		UVehicleDriveAssemblyComponent* Drive = nullptr;
	};

	FVehicle Create(UWorld& World, const FVector& Location, bool bBindAxles = false)
	{
		FVehicle Vehicle;
		Vehicle.Actor = World.SpawnActor<APawn>(Location, FRotator::ZeroRotator);
		Vehicle.Chassis = NewObject<UBoxComponent>(Vehicle.Actor, TEXT("PowerLifetimeChassis"));
		Vehicle.Actor->AddInstanceComponent(Vehicle.Chassis);
		Vehicle.Actor->SetRootComponent(Vehicle.Chassis);
		Vehicle.Chassis->SetBoxExtent(FVector(50.f));
		Vehicle.Chassis->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Vehicle.Chassis->SetSimulatePhysics(true);
		Vehicle.Chassis->RegisterComponent();
		Vehicle.Chassis->RecreatePhysicsState();
		Vehicle.Chassis->SetWorldLocation(Location);

		Vehicle.Drive = NewObject<UVehicleDriveAssemblyComponent>(Vehicle.Actor, TEXT("PowerLifetimeDrive"));
		Vehicle.Actor->AddInstanceComponent(Vehicle.Drive);
		Vehicle.Drive->SetupAttachment(Vehicle.Chassis);
		Vehicle.Drive->bUpdatePhysicsAutomatically = false;
		Vehicle.Drive->AutoGearboxConfig.bAutomaticGearbox = false;
		FVehicleAxleSpawnTemplate Front;
		Front.AxlePosition = FVector(100.f, 0.f, -50.f);
		FVehicleAxleSpawnTemplate Rear;
		Rear.AxlePosition = FVector(-100.f, 0.f, -50.f);
		Vehicle.Drive->AxleTemplates = {Front, Rear};
		Vehicle.Drive->RegisterComponent();
		if (bBindAxles)
		{
			for (auto* Axle : Vehicle.Drive->GetAxles()) if (!Axle->BindIsolatedAxleRuntime()) return Vehicle;
		}
		Vehicle.Actor->DispatchBeginPlay();
		FVehiclePowerLifetimeProbe::Tick(*Vehicle.Drive); // Reclaim after generated children run BeginPlay invalidation.
		return Vehicle;
	}

	UWorld* CreateWorld()
	{
		UWorld* World = UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true, ERHIFeatureLevel::Num, nullptr, true);
		if (!World) return nullptr;
		World->InitWorld(UWorld::InitializationValues().AllowAudioPlayback(false)
			.CreatePhysicsScene(true).RequiresHitProxies(false).CreateNavigation(false)
			.CreateAISystem(false).ShouldSimulatePhysics(false).SetTransactional(false));
		World->GetPhysicsScene()->GetSolver()->DisableAsyncMode();
		World->GetPhysicsScene()->GetSolver()->SetThreadingMode_External(Chaos::EThreadingModeTemp::SingleThread);
		return World;
	}

	struct FWorldCleanup
	{
		UWorld* World = nullptr;
		~FWorldCleanup() { if (World) World->DestroyWorld(false); }
	};

	void SetRuntimeSentinels(const TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe>& Bridge)
	{
		auto& Runtime = Bridge->GetRuntimeForSimulation();
		Runtime.EngineState.EngineAngularVelocity = 417.25f;
		Runtime.ClutchState.AngleDiff = 0.125f;
		Runtime.GearboxState.CurrentGear = 2;
	}

	bool RuntimeSentinelsMatch(const TSharedPtr<FVehiclePowerUnitBridge, ESPMode::ThreadSafe>& Bridge)
	{
		const auto& Runtime = Bridge->GetRuntimeForSimulation();
		return Runtime.EngineState.EngineAngularVelocity == 417.25f
			&& Runtime.ClutchState.AngleDiff == 0.125f && Runtime.GearboxState.CurrentGear == 2;
	}

	void FlushPhysicsScene(UWorld& World)
	{
		const FVector Gravity = FVector::ZeroVector;
		World.GetPhysicsScene()->SetUpForFrame(&Gravity, 1.f / 60.f, 0.f, 1.f / 60.f, 1.f / 60.f, 1, false);
		World.GetPhysicsScene()->StartFrame();
		World.GetPhysicsScene()->EndFrame();
		World.GetPhysicsScene()->WaitPhysScenes();
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehiclePowerComponentLifetimeTest,
	"KinetiForge.Lifecycle.PowerComponentRecovery",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehiclePowerComponentLifetimeTest::RunTest(const FString& Parameters)
{
	VehiclePowerLifetimeTest::FWorldCleanup Cleanup{VehiclePowerLifetimeTest::CreateWorld()};
	if (!TestNotNull(TEXT("Power lifetime world created"), Cleanup.World)) return false;
	const auto First = VehiclePowerLifetimeTest::Create(*Cleanup.World, FVector(0.f, 0.f, 800.f));
	const auto Second = VehiclePowerLifetimeTest::Create(*Cleanup.World, FVector(1000.f, 0.f, 800.f));
	const auto Bridge = FVehiclePowerLifetimeProbe::Bridge(*Second.Drive);
	if (!TestNotNull(TEXT("Fixed runtime bridge exists after component BeginPlay"), Bridge.Get())) return false;
	VehiclePowerLifetimeTest::SetRuntimeSentinels(Bridge);

	for (UVehicleDriveAssemblyComponent* Drive : {First.Drive, Second.Drive})
	{
		const auto StableBridge = FVehiclePowerLifetimeProbe::Bridge(*Drive);
		UActorComponent* Dependencies[] = {Drive->GetEngine(), Drive->GetClutch(), Drive->GetGearbox()};
		for (UActorComponent* Dependency : Dependencies)
		{
			if (!TestNotNull(TEXT("Power dependency exists"), Dependency)) return false;
			const auto OldStep = FVehiclePowerLifetimeProbe::Step(*Drive);
			Dependency->UnregisterComponent();
			TestTrue(TEXT("Power dependency unregister invalidates the owner entry"), OldStep->IsBindingInvalidated());
			FVehiclePowerLifetimeProbe::Tick(*Drive);
			TestTrue(TEXT("Owner cannot recover while power dependency is absent"),
				FVehiclePowerLifetimeProbe::Step(*Drive) == OldStep);
			Dependency->RegisterComponent();
			FVehiclePowerLifetimeProbe::Tick(*Drive);
			const auto NewStep = FVehiclePowerLifetimeProbe::Step(*Drive);
			TestTrue(TEXT("Dependency re-registration creates a fresh owner entry"), NewStep != OldStep && !NewStep->IsStopped());
			TestTrue(TEXT("Old power entry remains stopped after scene unregistration"), OldStep->IsStopped());
			TestTrue(TEXT("Rebind preserves the original bridge identity"), FVehiclePowerLifetimeProbe::Bridge(*Drive) == StableBridge);
			if (Drive == Second.Drive)
				TestTrue(TEXT("Rebind does not reseed integrated runtime state"), VehiclePowerLifetimeTest::RuntimeSentinelsMatch(StableBridge));
		}
		const auto BeforeEndPlay = FVehiclePowerLifetimeProbe::Step(*Drive);
		UActorComponent* EndedEngine = Drive->GetEngine();
		EndedEngine->EndPlay(EEndPlayReason::RemovedFromWorld);
		TestTrue(TEXT("EndPlay fixture remains registered but has ended play"), EndedEngine->IsRegistered() && !EndedEngine->HasBegunPlay());
		FVehiclePowerLifetimeProbe::Tick(*Drive);
		TestTrue(TEXT("A registered component that ended play cannot reopen its drive"),
			FVehiclePowerLifetimeProbe::Step(*Drive) == BeforeEndPlay && BeforeEndPlay->IsBindingInvalidated());
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleSharedDifferentialLifetimeTest,
	"KinetiForge.Lifecycle.SharedDifferentialRecovery",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleSharedDifferentialLifetimeTest::RunTest(const FString& Parameters)
{
	VehiclePowerLifetimeTest::FWorldCleanup Cleanup{VehiclePowerLifetimeTest::CreateWorld()};
	if (!TestNotNull(TEXT("Shared differential world created"), Cleanup.World)) return false;
	const auto A = VehiclePowerLifetimeTest::Create(*Cleanup.World, FVector(0.f, 0.f, 800.f));
	const auto B = VehiclePowerLifetimeTest::Create(*Cleanup.World, FVector(1000.f, 0.f, 800.f));
	UVehicleDifferentialComponent* SharedDiff = A.Drive->GetTransferCase();
	if (!TestNotNull(TEXT("First registered transfer case exists"), SharedDiff)) return false;
	FVehiclePowerLifetimeProbe::Step(*B.Drive)->InvalidateBindingAndWait();
	FVehiclePowerLifetimeProbe::SetTransferCase(*B.Drive, SharedDiff);
	TestTrue(TEXT("Two drives may subscribe to the stateless shared differential"), FVehiclePowerLifetimeProbe::Prepare(*B.Drive));
	const auto OldA = FVehiclePowerLifetimeProbe::Step(*A.Drive);
	const auto OldB = FVehiclePowerLifetimeProbe::Step(*B.Drive);
	SharedDiff->UnregisterComponent();
	TestTrue(TEXT("Shared differential invalidates first owner"), OldA->IsBindingInvalidated());
	TestTrue(TEXT("Shared differential invalidates second owner"), OldB->IsBindingInvalidated());
	FVehiclePowerLifetimeProbe::Tick(*A.Drive);
	FVehiclePowerLifetimeProbe::Tick(*B.Drive);
	TestTrue(TEXT("Neither owner resumes before the shared dependency registers"),
		FVehiclePowerLifetimeProbe::Step(*A.Drive) == OldA && FVehiclePowerLifetimeProbe::Step(*B.Drive) == OldB);
	SharedDiff->RegisterComponent();
	FVehiclePowerLifetimeProbe::Tick(*A.Drive);
	FVehiclePowerLifetimeProbe::Tick(*B.Drive);
	const auto NewA = FVehiclePowerLifetimeProbe::Step(*A.Drive);
	const auto NewB = FVehiclePowerLifetimeProbe::Step(*B.Drive);
	TestTrue(TEXT("Both owners get fresh entries after shared dependency recovery"),
		NewA != OldA && NewB != OldB && !NewA->IsStopped() && !NewB->IsStopped());
	TestTrue(TEXT("Shared dependency recovery leaves old snapshots stopped and inert"),
		OldA->IsStopped() && OldB->IsStopped());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehiclePowerOwnershipPreflightTest,
	"KinetiForge.Lifecycle.PowerOwnershipPreflight",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehiclePowerOwnershipPreflightTest::RunTest(const FString& Parameters)
{
	VehiclePowerLifetimeTest::FWorldCleanup Cleanup{VehiclePowerLifetimeTest::CreateWorld()};
	if (!TestNotNull(TEXT("Power ownership world created"), Cleanup.World)) return false;
	const auto Owner = VehiclePowerLifetimeTest::Create(*Cleanup.World, FVector(0.f, 0.f, 800.f));
	const auto Spare = VehiclePowerLifetimeTest::Create(*Cleanup.World, FVector(1000.f, 0.f, 800.f));
	const auto SpareStep = FVehiclePowerLifetimeProbe::Step(*Spare.Drive);
	SpareStep->StopAndWait(); // Its registered components are now available to test an all-or-nothing claim.
	const auto CandidateVehicle = VehiclePowerLifetimeTest::Create(*Cleanup.World, FVector(3000.f, 0.f, 800.f));
	UVehicleDriveAssemblyComponent* Candidate = CandidateVehicle.Drive;
	const auto CandidateInitialStep = FVehiclePowerLifetimeProbe::Step(*Candidate);
	CandidateInitialStep->StopAndWait();
	TestTrue(TEXT("Fixed-runtime candidate can claim its complete registered power unit"),
		FVehiclePowerLifetimeProbe::Prepare(*Candidate));
	FVehiclePowerLifetimeProbe::Step(*Candidate)->StopAndWait();
	UVehicleEngineComponent* SpareEngine = Spare.Drive->GetEngine();
	UVehicleClutchComponent* SpareClutch = Spare.Drive->GetClutch();
	UVehicleGearboxComponent* SpareGearbox = Spare.Drive->GetGearbox();
	if (!TestNotNull(TEXT("Spare engine registered"), SpareEngine)
		|| !TestNotNull(TEXT("Spare clutch registered"), SpareClutch)
		|| !TestNotNull(TEXT("Spare gearbox registered"), SpareGearbox)) return false;

	FVehiclePowerLifetimeProbe::SetPowerComponents(*Candidate, SpareEngine, Owner.Drive->GetClutch(), SpareGearbox);
	TestFalse(TEXT("A candidate sharing a Clutch is rejected"), FVehiclePowerLifetimeProbe::Prepare(*Candidate));
	TestFalse(TEXT("Failed power preflight does not partially claim preceding Engine"), FVehiclePowerLifetimeProbe::HasOwner(*SpareEngine));
	TestTrue(TEXT("Original Clutch owner remains active"), FVehiclePowerLifetimeProbe::HasOwner(*Owner.Drive->GetClutch()));

	FVehiclePowerLifetimeProbe::SetPowerComponents(*Candidate, SpareEngine, SpareClutch, Owner.Drive->GetGearbox());
	TestFalse(TEXT("A candidate sharing a Gearbox is rejected"), FVehiclePowerLifetimeProbe::Prepare(*Candidate));
	TestFalse(TEXT("Failed Gearbox preflight does not partially claim Engine"), FVehiclePowerLifetimeProbe::HasOwner(*SpareEngine));
	TestFalse(TEXT("Failed Gearbox preflight does not partially claim Clutch"), FVehiclePowerLifetimeProbe::HasOwner(*SpareClutch));
	TestTrue(TEXT("Original Gearbox owner remains active"), FVehiclePowerLifetimeProbe::HasOwner(*Owner.Drive->GetGearbox()));

	FVehiclePowerLifetimeProbe::SetPowerComponents(*Candidate, Owner.Drive->GetEngine(), SpareClutch, SpareGearbox);
	TestFalse(TEXT("A candidate sharing an Engine is rejected"), FVehiclePowerLifetimeProbe::Prepare(*Candidate));
	TestFalse(TEXT("Failed Engine preflight does not partially claim later components"),
		FVehiclePowerLifetimeProbe::HasOwner(*SpareClutch) || FVehiclePowerLifetimeProbe::HasOwner(*SpareGearbox));
	TestTrue(TEXT("Original Engine owner remains active"), FVehiclePowerLifetimeProbe::HasOwner(*Owner.Drive->GetEngine()));

	const auto Bound = VehiclePowerLifetimeTest::Create(*Cleanup.World, FVector(2000.f, 0.f, 800.f));
	const auto BoundBridge = FVehiclePowerLifetimeProbe::Bridge(*Bound.Drive);
	UVehicleEngineComponent* BoundEngine = Bound.Drive->GetEngine();
	if (!TestNotNull(TEXT("Bound engine exists"), BoundEngine)) return false;
	FVehiclePowerLifetimeProbe::Step(*Bound.Drive)->StopAndWait();
	FVehiclePowerLifetimeProbe::SetPowerComponents(*Candidate, BoundEngine, SpareClutch, SpareGearbox);
	TestFalse(TEXT("Candidate cannot replace its bridge-bound Engine after its prior entry stops"),
		FVehiclePowerLifetimeProbe::Prepare(*Candidate));
	TestTrue(TEXT("Engine remains attached to its original runtime bridge"),
		FVehiclePowerLifetimeProbe::ComponentBridge(*BoundEngine) == BoundBridge);
	TestFalse(TEXT("Rejected candidate did not acquire bound Engine"), FVehiclePowerLifetimeProbe::HasOwner(*BoundEngine));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleBoundPowerReplacementTest,
	"KinetiForge.Lifecycle.BoundPowerReplacement",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleBoundPowerReplacementTest::RunTest(const FString& Parameters)
{
	VehiclePowerLifetimeTest::FWorldCleanup Cleanup{VehiclePowerLifetimeTest::CreateWorld()};
	if (!TestNotNull(TEXT("Replacement world created"), Cleanup.World)) return false;
	const auto Vehicle = VehiclePowerLifetimeTest::Create(*Cleanup.World, FVector(0.f, 0.f, 800.f));
	if (!TestTrue(TEXT("Replacement fixture has a runtime bridge"), FVehiclePowerLifetimeProbe::InitializeBridge(*Vehicle.Drive))) return false;
	const auto Bridge = FVehiclePowerLifetimeProbe::Bridge(*Vehicle.Drive);
	VehiclePowerLifetimeTest::SetRuntimeSentinels(Bridge);
	UVehicleEngineComponent* OriginalEngine = Vehicle.Drive->GetEngine();
	UVehicleClutchComponent* OriginalClutch = Vehicle.Drive->GetClutch();
	UVehicleGearboxComponent* OriginalGearbox = Vehicle.Drive->GetGearbox();
	const auto OldStep = FVehiclePowerLifetimeProbe::Step(*Vehicle.Drive);
	OriginalEngine->UnregisterComponent();
	UVehicleEngineComponent* ReplacementEngine = NewObject<UVehicleEngineComponent>(Vehicle.Actor, TEXT("ReplacementPowerEngine"));
	Vehicle.Actor->AddInstanceComponent(ReplacementEngine);
	ReplacementEngine->RegisterComponent();
	FVehiclePowerLifetimeProbe::SetPowerComponents(*Vehicle.Drive, ReplacementEngine, OriginalClutch, OriginalGearbox);
	FVehiclePowerLifetimeProbe::Tick(*Vehicle.Drive);
	TestTrue(TEXT("A new engine cannot inherit the original bridge's integrating state"),
		FVehiclePowerLifetimeProbe::Step(*Vehicle.Drive) == OldStep && OldStep->IsStopped());
	TestFalse(TEXT("Rejected replacement is not partially claimed"), FVehiclePowerLifetimeProbe::HasOwner(*ReplacementEngine));
	TestTrue(TEXT("Rejected replacement does not reseed the old runtime"), VehiclePowerLifetimeTest::RuntimeSentinelsMatch(Bridge));
	FVehiclePowerLifetimeProbe::SetPowerComponents(*Vehicle.Drive, OriginalEngine, OriginalClutch, OriginalGearbox);
	OriginalEngine->RegisterComponent();
	FVehiclePowerLifetimeProbe::Tick(*Vehicle.Drive);
	const auto RestoredStep = FVehiclePowerLifetimeProbe::Step(*Vehicle.Drive);
	TestTrue(TEXT("Restoring the actual bound component permits a fresh entry"), RestoredStep != OldStep && !RestoredStep->IsStopped());
	TestTrue(TEXT("Restoring identity preserves both bridge and state"),
		FVehiclePowerLifetimeProbe::Bridge(*Vehicle.Drive) == Bridge && VehiclePowerLifetimeTest::RuntimeSentinelsMatch(Bridge));
	OriginalGearbox->DestroyComponent();
	FVehiclePowerLifetimeProbe::Tick(*Vehicle.Drive);
	TestTrue(TEXT("Destroyed essential power component keeps the drive paused"),
		FVehiclePowerLifetimeProbe::Step(*Vehicle.Drive) == RestoredStep && RestoredStep->IsBindingInvalidated());
	TestNull(TEXT("Recovery never auto-generates a replacement gearbox"), Vehicle.Drive->GetGearbox());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleAxleDifferentialLifetimeTest,
	"KinetiForge.Lifecycle.AxleDifferentialRecovery",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleAxleDifferentialLifetimeTest::RunTest(const FString& Parameters)
{
	VehiclePowerLifetimeTest::FWorldCleanup Cleanup{VehiclePowerLifetimeTest::CreateWorld()};
	if (!TestNotNull(TEXT("Axle differential world created"), Cleanup.World)) return false;
	const auto Vehicle = VehiclePowerLifetimeTest::Create(*Cleanup.World, FVector(0.f, 0.f, 800.f), true);
	const auto Axles = Vehicle.Drive->GetAxles();
	if (Axles.IsEmpty()) return false;
	UVehicleAxleAssemblyComponent* Axle = Axles[0];
	if (!TestTrue(TEXT("Axle fixture has a bound axle runtime"), Axle->GetAxleBridge().IsValid())) return false;
	UVehicleDifferentialComponent* Differential = nullptr;
	Axle->GetDifferential(Differential);
	if (!TestNotNull(TEXT("Axle has a generated differential"), Differential)) return false;
	Vehicle.Drive->bUpdatePhysicsAutomatically = true;
	FVehiclePowerLifetimeProbe::Tick(*Vehicle.Drive);
	VehiclePowerLifetimeTest::FlushPhysicsScene(*Cleanup.World);
	if (!TestNotNull(TEXT("Axle fixture has a real chassis handle"), UVehicleUtilities::GetInternalHandle(Vehicle.Chassis))) return false;
	VehiclePowerLifetimeTest::FlushPhysicsScene(*Cleanup.World);
	TestTrue(TEXT("First simulation step consumes the original differential snapshot"),
		Axle->GetAxleBridge()->GetRuntimeForSimulation().Settings.bHasDifferential);
	const auto OldStep = FVehiclePowerLifetimeProbe::Step(*Vehicle.Drive);
	Differential->UnregisterComponent();
	TestTrue(TEXT("Axle differential unregister invalidates its owner entry"), OldStep->IsBindingInvalidated());
	FVehiclePowerLifetimeProbe::Tick(*Vehicle.Drive);
	TestTrue(TEXT("Drive waits while its axle differential is unregistered"), FVehiclePowerLifetimeProbe::Step(*Vehicle.Drive) == OldStep);
	TestTrue(TEXT("Old axle scene entry stays stopped after unregister"), OldStep->IsStopped());
	Differential->RegisterComponent();
	FVehiclePowerLifetimeProbe::Tick(*Vehicle.Drive);
	const auto RestoredStep = FVehiclePowerLifetimeProbe::Step(*Vehicle.Drive);
	TestTrue(TEXT("Axle differential re-register permits a fresh entry"), RestoredStep != OldStep && !RestoredStep->IsStopped());
	Differential->DestroyComponent();
	TestTrue(TEXT("Destroying the axle differential invalidates its owner entry"), RestoredStep->IsBindingInvalidated());
	FVehiclePowerLifetimeProbe::Tick(*Vehicle.Drive);
	const auto BrokenDifferentialStep = FVehiclePowerLifetimeProbe::Step(*Vehicle.Drive);
	TestTrue(TEXT("Destroyed differential leaves the old scene entry stopped"), RestoredStep->IsStopped());
	TestTrue(TEXT("Existing broken-differential simulation path can resume"), BrokenDifferentialStep != RestoredStep && !BrokenDifferentialStep->IsStopped());
	VehiclePowerLifetimeTest::FlushPhysicsScene(*Cleanup.World);
	TestFalse(TEXT("Simulation does not retain parameters of a destroyed differential"),
		Axle->GetAxleBridge()->GetRuntimeForSimulation().Settings.bHasDifferential);
	const auto& AxleState = Axle->GetSimulationState();
	TestTrue(TEXT("Broken differential returns original undriven wheel/shaft feedback"),
		FMath::IsNearlyZero(AxleState.ReflectedInertiaOnWheel)
		&& FMath::IsNearlyEqual(AxleState.AxleEffectiveInertia, Axle->GetAxleConfig().DriveShaftInertia));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleDeferredPowerRegistrationTest,
	"KinetiForge.Lifecycle.DeferredPowerRegistration",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleDeferredPowerRegistrationTest::RunTest(const FString& Parameters)
{
	VehiclePowerLifetimeTest::FWorldCleanup Cleanup{VehiclePowerLifetimeTest::CreateWorld()};
	if (!TestNotNull(TEXT("Deferred registration world created"), Cleanup.World)) return false;
	{
		const auto Vehicle = VehiclePowerLifetimeTest::Create(*Cleanup.World, FVector(0.f, 0.f, 800.f));
		const auto OldEntry = FVehiclePowerLifetimeProbe::Step(*Vehicle.Drive);
		const auto OriginalBridge = FVehiclePowerLifetimeProbe::Bridge(*Vehicle.Drive);
		UVehicleEngineComponent* Engine = Vehicle.Drive->GetEngine();
		Engine->UnregisterComponent();
		Vehicle.Drive->UnregisterComponent();
		Vehicle.Drive->RegisterComponent(); // The engine is still absent when the new registration preflights.
		const auto PendingEntry = FVehiclePowerLifetimeProbe::Step(*Vehicle.Drive);
		TestTrue(TEXT("Explicit re-register publishes a fresh paused pending entry"),
			PendingEntry != OldEntry && PendingEntry->IsStopped() && PendingEntry->IsBindingInvalidated());
		FVehiclePowerLifetimeProbe::Tick(*Vehicle.Drive);
		TestTrue(TEXT("Pending entry stays paused until its engine is registered"),
			FVehiclePowerLifetimeProbe::Step(*Vehicle.Drive) == PendingEntry);
		Engine->RegisterComponent();
		FVehiclePowerLifetimeProbe::Tick(*Vehicle.Drive);
		const auto ResumedEntry = FVehiclePowerLifetimeProbe::Step(*Vehicle.Drive);
		TestTrue(TEXT("Child registration allows the pending drive to resume"),
			ResumedEntry != PendingEntry && !ResumedEntry->IsStopped());
		TestTrue(TEXT("Old explicit stop remains permanent across readiness retry"),
			OldEntry->IsStopped() && !OldEntry->IsBindingInvalidated() && !OldEntry->TryExecute(1.f / 120.f));
		TestTrue(TEXT("Readiness retry preserves the selected power bridge"),
			FVehiclePowerLifetimeProbe::Bridge(*Vehicle.Drive) == OriginalBridge);
	}
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
