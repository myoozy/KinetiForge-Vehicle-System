// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "VehicleDriveAssemblyComponent.h"
#include "VehicleAsyncTickComponent.h"
#include "VehicleWheelCoordinatorComponent.h"
#include "VehicleAsyncSpringArmComponent.h"
#include "VehicleEngineComponent.h"
#include "VehicleClutchComponent.h"
#include "VehicleGearboxComponent.h"
#include "VehicleWheelComponent.h"
#include "VehiclePowerUnitBridge.h"
#include "VehiclePhysicsStep.h"
#include "VehicleUtilities.h"
#include "Components/BoxComponent.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "PBDRigidsSolver.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Misc/AutomationTest.h"
#include "UObject/StrongObjectPtr.h"

struct FVehicleDriveAssemblyRuntimeProbe
{
	static bool InitializeAndRefresh(UVehicleDriveAssemblyComponent& Drive)
	{
		if (!Drive.InitializePowerUnitBridge()) return false;
		Drive.RefreshPowerUnitBridge();
		return true;
	}
	static void Refresh(UVehicleDriveAssemblyComponent& Drive) { Drive.RefreshPowerUnitBridge(); }
	static TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe> Step(UVehicleDriveAssemblyComponent& Drive)
	{
		return Drive.GetPhysicsStep();
	}
	static UVehicleAsyncTickComponent* Tick(UVehicleDriveAssemblyComponent& Drive) { return Drive.VehicleAsyncTickComponent.Get(); }
	static float PhysicsDelta(const UVehicleDriveAssemblyComponent& Drive) { return Drive.PhysicsDeltaTime; }
};

struct FVehicleAsyncTickRegistrationProbe
{
	static int32 Drives(const UVehicleAsyncTickComponent& Tick) { return Tick.DriveAssemblies.Num(); }
	static int32 Coordinators(const UVehicleAsyncTickComponent& Tick) { return Tick.WheelCoordinators.Num(); }
	static int32 SpringArms(const UVehicleAsyncTickComponent& Tick) { return Tick.AsyncSpringArms.Num(); }
};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleAsyncTickUnregisterTest,
	"KinetiForge.Lifecycle.AuxiliaryRegistration",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleAsyncTickUnregisterTest::RunTest(const FString& Parameters)
{
	TStrongObjectPtr<UVehicleAsyncTickComponent> Tick(NewObject<UVehicleAsyncTickComponent>());
	TStrongObjectPtr<UVehicleWheelCoordinatorComponent> Coordinator(NewObject<UVehicleWheelCoordinatorComponent>());
	TStrongObjectPtr<UVehicleAsyncSpringArmComponent> SpringArm(NewObject<UVehicleAsyncSpringArmComponent>());
	Tick->RegisterWheelCoordinator(Coordinator.Get());
	Tick->RegisterWheelCoordinator(Coordinator.Get());
	Tick->RegisterAsyncSpringArm(SpringArm.Get());
	Tick->RegisterAsyncSpringArm(SpringArm.Get());
	TestEqual(TEXT("Repeated registration has one coordinator"), FVehicleAsyncTickRegistrationProbe::Coordinators(*Tick), 1);
	TestEqual(TEXT("Repeated registration has one spring arm"), FVehicleAsyncTickRegistrationProbe::SpringArms(*Tick), 1);
	Tick->UnRegisterWheelCoordinator(Coordinator.Get());
	Tick->UnRegisterAsyncSpringArm(SpringArm.Get());
	Tick->UnRegisterWheelCoordinator(Coordinator.Get());
	Tick->UnRegisterAsyncSpringArm(SpringArm.Get());
	TestEqual(TEXT("Repeated removal leaves no coordinator"), FVehicleAsyncTickRegistrationProbe::Coordinators(*Tick), 0);
	TestEqual(TEXT("Repeated removal leaves no spring arm"), FVehicleAsyncTickRegistrationProbe::SpringArms(*Tick), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleDefaultDriveSceneBindingTest,
	"KinetiForge.Lifecycle.DefaultDriveRequiresSceneBinding",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleDefaultDriveSceneBindingTest::RunTest(const FString& Parameters)
{
	TStrongObjectPtr<UVehicleAsyncTickComponent> Tick(NewObject<UVehicleAsyncTickComponent>());
	TStrongObjectPtr<UVehicleDriveAssemblyComponent> Drive(NewObject<UVehicleDriveAssemblyComponent>());
	TestTrue(TEXT("Whole-vehicle runtime is the native default"), Drive->bUseIsolatedVehicleRuntime);
	Drive->bUseIsolatedVehicleRuntime = false; // Simulate an obsolete serialized override; it cannot select a runtime.
	Drive->bUseIsolatedPowerUnitRuntime = false;
	Tick->RegisterDriveAssembly(Drive.Get());
	TestEqual(TEXT("A default component without a scene is not published as a legacy Drive"),
		FVehicleAsyncTickRegistrationProbe::Drives(*Tick), 0);
	const auto PendingEntry = FVehicleDriveAssemblyRuntimeProbe::Step(*Drive);
	if (!TestNotNull(TEXT("Incomplete default Drive retains a pending lifecycle entry"), PendingEntry.Get())) return false;
	TestTrue(TEXT("Incomplete default binding is stopped and retryable"),
		PendingEntry->IsStopped() && PendingEntry->IsBindingInvalidated());
	TestFalse(TEXT("Pending binding cannot execute before a complete scene binding"), PendingEntry->TryExecute(1.f / 60.f));
	Tick->RegisterDriveAssembly(Drive.Get());
	TestEqual(TEXT("Repeated incomplete registration never publishes a legacy fallback"),
		FVehicleAsyncTickRegistrationProbe::Drives(*Tick), 0);
	return true;
}

namespace VehicleDriveAssemblyTest
{
	struct FRegisteredVehicle
	{
		APawn* Actor = nullptr;
		UBoxComponent* Chassis = nullptr;
		UVehicleDriveAssemblyComponent* Drive = nullptr;
	};

	FRegisteredVehicle Create(UWorld& World, const FVector& Location)
	{
		FRegisteredVehicle Vehicle;
		Vehicle.Actor = World.SpawnActor<APawn>(Location, FRotator::ZeroRotator);
		Vehicle.Chassis = NewObject<UBoxComponent>(Vehicle.Actor, TEXT("TestChassis"));
		Vehicle.Actor->AddInstanceComponent(Vehicle.Chassis);
		Vehicle.Actor->SetRootComponent(Vehicle.Chassis);
		Vehicle.Chassis->SetBoxExtent(FVector(50.f));
		Vehicle.Chassis->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Vehicle.Chassis->SetSimulatePhysics(true);
		Vehicle.Chassis->RegisterComponent();
		Vehicle.Chassis->RecreatePhysicsState(); // Resolve any deferred body creation before the test's Chaos flush.
		Vehicle.Chassis->SetWorldLocation(Location);
		Vehicle.Drive = NewObject<UVehicleDriveAssemblyComponent>(Vehicle.Actor, TEXT("TestDrive"));
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
		// Component lifecycle fixture; actual worker and numerical coverage live in the scene-runtime tests.
		Vehicle.Actor->DispatchBeginPlay();
		Vehicle.Drive->TickComponent(1.f / 60.f, LEVELTICK_All, nullptr);
		return Vehicle;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleRegisteredDriveStopTest,
	"KinetiForge.Lifecycle.RegisteredDriveStop",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleRegisteredDriveStopTest::RunTest(const FString& Parameters)
{
	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true, ERHIFeatureLevel::Num, nullptr, true);
	if (!TestNotNull(TEXT("Lifecycle world created"), World)) return false;
	World->InitWorld(UWorld::InitializationValues().AllowAudioPlayback(false)
		.CreatePhysicsScene(true).RequiresHitProxies(false).CreateNavigation(false)
		.CreateAISystem(false).ShouldSimulatePhysics(false).SetTransactional(false));
	struct FWorldCleanup { UWorld* World; ~FWorldCleanup() { World->DestroyWorld(false); } } Cleanup{World};
	World->GetPhysicsScene()->GetSolver()->DisableAsyncMode();
	World->GetPhysicsScene()->GetSolver()->SetThreadingMode_External(Chaos::EThreadingModeTemp::SingleThread);
	const auto Vehicle = VehicleDriveAssemblyTest::Create(*World, FVector(0.f, 0.f, 1000.f));
	const auto OldEntry = FVehicleDriveAssemblyRuntimeProbe::Step(*Vehicle.Drive);
	UVehicleAsyncTickComponent* Tick = FVehicleDriveAssemblyRuntimeProbe::Tick(*Vehicle.Drive);
	if (!TestNotNull(TEXT("Vehicle async ticker registered"), Tick)) return false;
	Tick->RegisterDriveAssembly(Vehicle.Drive);
	Tick->RegisterDriveAssembly(Vehicle.Drive);
	TestEqual(TEXT("Repeated complete Drive registration has one entry"), FVehicleAsyncTickRegistrationProbe::Drives(*Tick), 1);
	TestTrue(TEXT("Repeated Drive registration preserves the active scene entry"), FVehicleDriveAssemblyRuntimeProbe::Step(*Vehicle.Drive) == OldEntry && !OldEntry->IsStopped());
	const auto OriginalWheels = Vehicle.Drive->GetWheels();
	TestEqual(TEXT("Lifecycle vehicle starts with four generated wheels"), OriginalWheels.Num(), 4);
	Vehicle.Drive->UnregisterComponent();
	TestEqual(TEXT("Unregistered drive is removed from automatic list"), FVehicleAsyncTickRegistrationProbe::Drives(*Tick), 0);
	TestTrue(TEXT("OnUnregister closes the entry"), OldEntry->IsStopped());
	TestFalse(TEXT("Queued automatic callback cannot enter an unregistered drive"), OldEntry->TryExecute(1.f / 60.f, true));
	Vehicle.Drive->RegisterComponent();
	const auto NewEntry = FVehicleDriveAssemblyRuntimeProbe::Step(*Vehicle.Drive);
	TestTrue(TEXT("OnRegister publishes a fresh live entry"), NewEntry != OldEntry && !NewEntry->IsStopped());
	TestEqual(TEXT("Re-registered drive returns to automatic list once"), FVehicleAsyncTickRegistrationProbe::Drives(*Tick), 1);
	TestTrue(TEXT("Repeated registration preserves generated wheel identities"), Vehicle.Drive->GetWheels() == OriginalWheels);
	TestFalse(TEXT("Old queued callback remains closed after re-registration"), OldEntry->TryExecute(1.f / 60.f));
	Tick->UnregisterComponent();
	TestTrue(TEXT("Ticker unregister drains its drive entry"), NewEntry->IsStopped());
	Tick->RegisterComponent();
	const auto RestoredEntry = FVehicleDriveAssemblyRuntimeProbe::Step(*Vehicle.Drive);
	TestTrue(TEXT("Ticker re-register restores a fresh drive entry"), RestoredEntry != NewEntry && !RestoredEntry->IsStopped());
	TestEqual(TEXT("Ticker re-register retains a single automatic drive"), FVehicleAsyncTickRegistrationProbe::Drives(*Tick), 1);
	TestTrue(TEXT("Ticker re-register restores its active state"), Tick->IsActive());
	TestFalse(TEXT("Old ticker snapshot remains inert"), NewEntry->TryExecute(1.f / 60.f));
	Vehicle.Drive->DestroyComponent();
	TestTrue(TEXT("Destroy closes the current entry before releasing generated children"), RestoredEntry->IsStopped());
	TestFalse(TEXT("Retained callback cannot dereference a destroyed drive"), RestoredEntry->TryExecute(1.f / 60.f));
	for (UVehicleWheelComponent* Wheel : OriginalWheels)
		TestTrue(TEXT("Generated wheels have been destroyed"), !IsValid(Wheel) || Wheel->IsBeingDestroyed());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleUnregisteredDriveCollectionTest,
	"KinetiForge.Lifecycle.UnregisteredDriveCollection",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleUnregisteredDriveCollectionTest::RunTest(const FString& Parameters)
{
	TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe> RetainedEntry;
	TWeakObjectPtr<UVehicleDriveAssemblyComponent> CollectedDrive;
	{
		TStrongObjectPtr<UVehicleDriveAssemblyComponent> Drive(NewObject<UVehicleDriveAssemblyComponent>());
		CollectedDrive = Drive.Get();
		RetainedEntry = FVehicleDriveAssemblyRuntimeProbe::Step(*Drive);
	}
	CollectGarbage(RF_NoFlags);
	TestFalse(TEXT("Unregistered drive was collected"), CollectedDrive.IsValid());
	TestTrue(TEXT("BeginDestroy closes a retained entry without registration or BeginPlay"), RetainedEntry->IsStopped());
	TestFalse(TEXT("Old snapshot cannot enter a collected UObject"), RetainedEntry->TryExecute(1.f / 60.f));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
