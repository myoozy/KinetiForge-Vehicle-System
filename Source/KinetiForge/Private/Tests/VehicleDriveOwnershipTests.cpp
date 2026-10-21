// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "VehicleDriveAssemblyComponent.h"
#include "VehicleAxleAssemblyComponent.h"
#include "VehicleWheelComponent.h"
#include "VehicleAsyncTickComponent.h"
#include "VehiclePhysicsStep.h"
#include "VehicleUtilities.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "PBDRigidsSolver.h"
#include "Misc/AutomationTest.h"

struct FVehicleDriveOwnershipProbe
{
	static TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe> GetStep(const UVehicleDriveAssemblyComponent& Drive)
	{
		return Drive.GetPhysicsStep();
	}
	static bool Prepare(UVehicleDriveAssemblyComponent& Drive) { return Drive.PreparePhysicsStepRegistration(); }
	static void SetAxles(UVehicleDriveAssemblyComponent& Drive, std::initializer_list<UVehicleAxleAssemblyComponent*> Axles)
	{
		Drive.Axles.Reset();
		for (UVehicleAxleAssemblyComponent* Axle : Axles) Drive.Axles.Add(Axle);
	}
	static void SetWheels(UVehicleAxleAssemblyComponent& Axle, UVehicleWheelComponent* Left, UVehicleWheelComponent* Right)
	{
		Axle.LeftWheel = Left;
		Axle.RightWheel = Right;
	}
	static bool HasOwner(const UVehicleAxleAssemblyComponent& Axle) { return Axle.PhysicsOwnership.HasActiveOwner(); }
	static bool HasOwner(const UVehicleWheelComponent& Wheel) { return Wheel.PhysicsOwnership.HasActiveOwner(); }
	static UVehicleAsyncTickComponent* GetTick(const UVehicleDriveAssemblyComponent& Drive)
	{
		return Drive.VehicleAsyncTickComponent.Get();
	}
	static void Tick(UVehicleDriveAssemblyComponent& Drive)
	{
		Drive.TickComponent(1.f / 60.f, LEVELTICK_All, nullptr);
	}
};

namespace VehicleDriveOwnershipTest
{
	struct FVehicle
	{
		APawn* Actor = nullptr;
		UBoxComponent* Chassis = nullptr;
		UVehicleDriveAssemblyComponent* Drive = nullptr;
	};

	FVehicle Create(UWorld& World, const FVector& Location, bool bDispatchBeginPlay)
	{
		FVehicle Vehicle;
		Vehicle.Actor = World.SpawnActor<APawn>(Location, FRotator::ZeroRotator);
		Vehicle.Chassis = NewObject<UBoxComponent>(Vehicle.Actor, TEXT("OwnershipTestChassis"));
		Vehicle.Actor->AddInstanceComponent(Vehicle.Chassis);
		Vehicle.Actor->SetRootComponent(Vehicle.Chassis);
		Vehicle.Chassis->SetBoxExtent(FVector(50.f));
		Vehicle.Chassis->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Vehicle.Chassis->SetSimulatePhysics(true);
		Vehicle.Chassis->RegisterComponent();
		Vehicle.Chassis->RecreatePhysicsState();
		Vehicle.Chassis->SetWorldLocation(Location);

		Vehicle.Drive = NewObject<UVehicleDriveAssemblyComponent>(Vehicle.Actor, TEXT("OwnershipTestDrive"));
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
		if (bDispatchBeginPlay) Vehicle.Actor->DispatchBeginPlay();
		return Vehicle;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleDriveOwnershipPreflightTest,
	"KinetiForge.Lifecycle.DriveOwnershipPreflight",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleDriveOwnershipPreflightTest::RunTest(const FString& Parameters)
{
	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true, ERHIFeatureLevel::Num, nullptr, true);
	if (!TestNotNull(TEXT("Ownership world created"), World)) return false;
	World->InitWorld(UWorld::InitializationValues().AllowAudioPlayback(false)
		.CreatePhysicsScene(true).RequiresHitProxies(false).CreateNavigation(false)
		.CreateAISystem(false).ShouldSimulatePhysics(false).SetTransactional(false));
	struct FWorldCleanup { UWorld* World; ~FWorldCleanup() { World->DestroyWorld(false); } } Cleanup{World};
	World->GetPhysicsScene()->GetSolver()->DisableAsyncMode();
	World->GetPhysicsScene()->GetSolver()->SetThreadingMode_External(Chaos::EThreadingModeTemp::SingleThread);
	const auto Owner = VehicleDriveOwnershipTest::Create(*World, FVector(0.f, 0.f, 800.f), true);
	const auto Spare = VehicleDriveOwnershipTest::Create(*World, FVector(1000.f, 0.f, 800.f), true);
	FVehicleDriveOwnershipProbe::Tick(*Owner.Drive);
	FVehicleDriveOwnershipProbe::Tick(*Spare.Drive);
	TArray<UVehicleAxleAssemblyComponent*> OwnerAxles = Owner.Drive->GetAxles();
	TArray<UVehicleAxleAssemblyComponent*> SpareAxles = Spare.Drive->GetAxles();
	if (!TestEqual(TEXT("Owner has two generated axles"), OwnerAxles.Num(), 2)
		|| !TestEqual(TEXT("Spare has two generated axles"), SpareAxles.Num(), 2)) return false;
	TestTrue(TEXT("First drive claimed its registered axle and wheels"),
		FVehicleDriveOwnershipProbe::HasOwner(*OwnerAxles[0]));
	UVehicleWheelComponent* OwnerLeft = nullptr;
	UVehicleWheelComponent* OwnerRight = nullptr;
	OwnerAxles[0]->GetWheels(OwnerLeft, OwnerRight);
	if (!TestNotNull(TEXT("Owner left wheel exists"), OwnerLeft)) return false;
	TestTrue(TEXT("First drive claimed its wheel"), FVehicleDriveOwnershipProbe::HasOwner(*OwnerLeft));

	// Stop the spare assembly so its registered dependencies are available for a fresh candidate.
	const auto SpareOldStep = FVehicleDriveOwnershipProbe::GetStep(*Spare.Drive);
	SpareOldStep->StopAndWait();
	const auto CandidateVehicle = VehicleDriveOwnershipTest::Create(*World, FVector(2000.f, 0.f, 800.f), true);
	UVehicleDriveAssemblyComponent* Candidate = CandidateVehicle.Drive;
	FVehicleDriveOwnershipProbe::Tick(*Candidate); // Finish the fixed-runtime dependency recovery.
	FVehicleDriveOwnershipProbe::GetStep(*Candidate)->StopAndWait();

	// A real fixed-runtime drive can claim an available registered assembly.
	FVehicleDriveOwnershipProbe::SetAxles(*Candidate, {SpareAxles[0]});
	TestTrue(TEXT("Fixed-runtime candidate claims an available axle and its wheels"), FVehicleDriveOwnershipProbe::Prepare(*Candidate));
	TestTrue(TEXT("Positive control publishes ownership"), FVehicleDriveOwnershipProbe::HasOwner(*SpareAxles[0]));
	FVehicleDriveOwnershipProbe::GetStep(*Candidate)->StopAndWait();

	// An available axle precedes the already-owned axle: failure must happen before any claim.
	FVehicleDriveOwnershipProbe::SetAxles(*Candidate, {SpareAxles[0], OwnerAxles[0]});
	TestFalse(TEXT("Cross-drive shared axle is rejected"), FVehicleDriveOwnershipProbe::Prepare(*Candidate));
	TestFalse(TEXT("Failed preflight did not partially claim the earlier axle"), FVehicleDriveOwnershipProbe::HasOwner(*SpareAxles[0]));
	TestTrue(TEXT("Original axle owner remains active"), FVehicleDriveOwnershipProbe::HasOwner(*OwnerAxles[0]));

	// Use a free axle but point it at a wheel already owned by the first drive.
	FVehicleDriveOwnershipProbe::SetWheels(*SpareAxles[1], OwnerLeft, nullptr);
	FVehicleDriveOwnershipProbe::SetAxles(*Candidate, {SpareAxles[1]});
	TestFalse(TEXT("Cross-drive shared wheel is rejected"), FVehicleDriveOwnershipProbe::Prepare(*Candidate));
	TestFalse(TEXT("Shared-wheel rejection leaves its axle unclaimed"), FVehicleDriveOwnershipProbe::HasOwner(*SpareAxles[1]));
	TestTrue(TEXT("Shared wheel remains with its original owner"), FVehicleDriveOwnershipProbe::HasOwner(*OwnerLeft));

	// Same-candidate duplicate wheel is rejected in preflight, before any of its free axles claim.
	UVehicleWheelComponent* SpareLeft0 = nullptr;
	UVehicleWheelComponent* SpareRight0 = nullptr;
	SpareAxles[0]->GetWheels(SpareLeft0, SpareRight0);
	FVehicleDriveOwnershipProbe::SetWheels(*SpareAxles[1], SpareLeft0, nullptr);
	FVehicleDriveOwnershipProbe::SetAxles(*Candidate, {SpareAxles[0], SpareAxles[1]});
	TestFalse(TEXT("One drive cannot list the same wheel through two axles"), FVehicleDriveOwnershipProbe::Prepare(*Candidate));
	TestFalse(TEXT("Duplicate-wheel failure leaves first axle unclaimed"), FVehicleDriveOwnershipProbe::HasOwner(*SpareAxles[0]));
	TestFalse(TEXT("Duplicate-wheel failure leaves second axle unclaimed"), FVehicleDriveOwnershipProbe::HasOwner(*SpareAxles[1]));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleDriveDependencyRecoveryTest,
	"KinetiForge.Lifecycle.DriveDependencyRecovery",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleDriveDependencyRecoveryTest::RunTest(const FString& Parameters)
{
	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true, ERHIFeatureLevel::Num, nullptr, true);
	if (!TestNotNull(TEXT("Recovery world created"), World)) return false;
	World->InitWorld(UWorld::InitializationValues().AllowAudioPlayback(false)
		.CreatePhysicsScene(true).RequiresHitProxies(false).CreateNavigation(false)
		.CreateAISystem(false).ShouldSimulatePhysics(false).SetTransactional(false));
	struct FWorldCleanup { UWorld* World; ~FWorldCleanup() { World->DestroyWorld(false); } } Cleanup{World};
	World->GetPhysicsScene()->GetSolver()->DisableAsyncMode();
	World->GetPhysicsScene()->GetSolver()->SetThreadingMode_External(Chaos::EThreadingModeTemp::SingleThread);
	const auto Vehicle = VehicleDriveOwnershipTest::Create(*World, FVector(0.f, 0.f, 800.f), true);
	TArray<UVehicleAxleAssemblyComponent*> Axles = Vehicle.Drive->GetAxles();
	if (!TestEqual(TEXT("Recovery fixture generated two axles"), Axles.Num(), 2)) return false;
	const TArray<UVehicleWheelComponent*> Wheels = Vehicle.Drive->GetWheels();
	if (!TestEqual(TEXT("Recovery fixture generated four wheels"), Wheels.Num(), 4)) return false;
	UVehicleAsyncTickComponent* Tick = FVehicleDriveOwnershipProbe::GetTick(*Vehicle.Drive);
	if (!TestNotNull(TEXT("Async tick was registered at BeginPlay"), Tick)) return false;
	FVehicleDriveOwnershipProbe::Tick(*Vehicle.Drive); // Finish the initial dependency-invalidated claim.
	const auto InitialStep = FVehicleDriveOwnershipProbe::GetStep(*Vehicle.Drive);
	TestFalse(TEXT("Initial step is active after dependency recovery"), InitialStep->IsStopped());
	for (UVehicleAxleAssemblyComponent* Axle : Axles)
		TestTrue(TEXT("Initial axle has the drive as active owner"), FVehicleDriveOwnershipProbe::HasOwner(*Axle));

	Wheels[0]->UnregisterComponent();
	TestTrue(TEXT("Wheel unregister invalidates the current entry"), InitialStep->IsBindingInvalidated());
	FVehicleDriveOwnershipProbe::Tick(*Vehicle.Drive);
	TestTrue(TEXT("Drive does not rebind while a dependency is unregistered"),
		FVehicleDriveOwnershipProbe::GetStep(*Vehicle.Drive) == InitialStep);
	TestFalse(TEXT("Unregistered wheel has no active owner"), FVehicleDriveOwnershipProbe::HasOwner(*Wheels[0]));

	Wheels[0]->RegisterComponent();
	FVehicleDriveOwnershipProbe::Tick(*Vehicle.Drive);
	const auto ReboundStep = FVehicleDriveOwnershipProbe::GetStep(*Vehicle.Drive);
	TestTrue(TEXT("Wheel re-registration creates a fresh drive entry"), ReboundStep != InitialStep && !ReboundStep->IsStopped());
	TestTrue(TEXT("Old entry stays stopped after scene unregistration"), InitialStep->IsStopped());
	TestTrue(TEXT("Recovered wheel is actively owned"), FVehicleDriveOwnershipProbe::HasOwner(*Wheels[0]));
	TestTrue(TEXT("Generated wheel identities survive recovery"), Vehicle.Drive->GetWheels() == Wheels);

	Wheels[1]->DestroyComponent();
	TestTrue(TEXT("Wheel destruction invalidates its owner entry"), ReboundStep->IsBindingInvalidated());
	FVehicleDriveOwnershipProbe::Tick(*Vehicle.Drive);
	const auto ReducedStep = FVehicleDriveOwnershipProbe::GetStep(*Vehicle.Drive);
	TestTrue(TEXT("Destroyed dependency permits recovery of the surviving assembly"),
		ReducedStep != ReboundStep && !ReducedStep->IsStopped());
	TestTrue(TEXT("Previous dependency-bound entry stays stopped after scene unregistration"), ReboundStep->IsStopped());
	TestTrue(TEXT("Surviving wheel remains owned after recovery"), FVehicleDriveOwnershipProbe::HasOwner(*Wheels[2]));
	ReducedStep->StopAndWait();
	FVehicleDriveOwnershipProbe::Tick(*Vehicle.Drive);
	TestTrue(TEXT("GT recovery never reopens an explicitly stopped drive"),
		FVehicleDriveOwnershipProbe::GetStep(*Vehicle.Drive) == ReducedStep
		&& ReducedStep->IsStopped() && !ReducedStep->IsBindingInvalidated());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleManualOwnershipTest,
	"KinetiForge.Lifecycle.ManualWheelAndAxleOwnership",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleManualOwnershipTest::RunTest(const FString& Parameters)
{
	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true, ERHIFeatureLevel::Num, nullptr, true);
	if (!TestNotNull(TEXT("Manual physics world created"), World)) return false;
	World->InitWorld(UWorld::InitializationValues().AllowAudioPlayback(false)
		.CreatePhysicsScene(true).RequiresHitProxies(false).CreateNavigation(false)
		.CreateAISystem(false).ShouldSimulatePhysics(false).SetTransactional(false));
	struct FWorldCleanup { UWorld* World; ~FWorldCleanup() { World->DestroyWorld(false); } } Cleanup{World};
	World->GetPhysicsScene()->GetSolver()->DisableAsyncMode();
	World->GetPhysicsScene()->GetSolver()->SetThreadingMode_External(Chaos::EThreadingModeTemp::SingleThread);
	const auto Vehicle = VehicleDriveOwnershipTest::Create(*World, FVector(0.f, 0.f, 800.f), true);
	FVehicleDriveOwnershipProbe::Tick(*Vehicle.Drive);
	const auto OwnedAxles = Vehicle.Drive->GetAxles();
	const auto OwnedWheels = Vehicle.Drive->GetWheels();
	if (OwnedAxles.IsEmpty() || OwnedWheels.IsEmpty()) return false;

	UVehicleWheelComponent* StandaloneWheel = NewObject<UVehicleWheelComponent>(Vehicle.Actor, TEXT("StandaloneManualWheel"));
	Vehicle.Actor->AddInstanceComponent(StandaloneWheel);
	StandaloneWheel->SetupAttachment(Vehicle.Chassis);
	StandaloneWheel->RegisterComponent();
	UVehicleAxleAssemblyComponent* StandaloneAxle = NewObject<UVehicleAxleAssemblyComponent>(Vehicle.Actor, TEXT("StandaloneManualAxle"));
	Vehicle.Actor->AddInstanceComponent(StandaloneAxle);
	StandaloneAxle->SetupAttachment(Vehicle.Chassis);
	StandaloneAxle->RegisterComponent();
	TestFalse(TEXT("Standalone wheel is not claimed by another drive on the same body"), FVehicleDriveOwnershipProbe::HasOwner(*StandaloneWheel));
	TestFalse(TEXT("Standalone axle is not claimed by another drive on the same body"), FVehicleDriveOwnershipProbe::HasOwner(*StandaloneAxle));
	Vehicle.Chassis->RecreatePhysicsState();
	const FVector Gravity = FVector::ZeroVector;
	World->GetPhysicsScene()->SetUpForFrame(&Gravity, 1.f / 60.f, 0.f, 1.f / 60.f, 1.f / 60.f, 1, false);
	World->GetPhysicsScene()->StartFrame();
	World->GetPhysicsScene()->EndFrame();
	World->GetPhysicsScene()->WaitPhysScenes();
	if (!TestNotNull(TEXT("Manual fixture has a real chassis body"), UVehicleUtilities::GetInternalHandle(Vehicle.Chassis))) return false;
	const float OwnedWheelOmega = OwnedWheels[0]->GetWheelState().AngularVelocity;
	float OwnedInertia = -1.f, OwnedOmega = -1.f, OwnedStiffness = -1.f;
	float ManualInertia = 0.f, ManualOmega = 0.f, ManualStiffness = 0.f;
	{
#if PHYSICS_THREAD_CONTEXT
		Chaos::FPhysicsThreadContextScope PhysicsContext(true);
#endif
		OwnedWheels[0]->UpdatePhysics(1.f / 120.f, 100.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f);
		OwnedAxles[0]->UpdatePhysics(1.f / 120.f, 100.f, 0.f, 0.f, 0.f, OwnedInertia, OwnedOmega, OwnedStiffness);
		StandaloneWheel->UpdatePhysics(1.f / 120.f, 100.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f);
		StandaloneAxle->UpdatePhysics(1.f / 120.f, 100.f, 0.f, 0.f, 0.f, ManualInertia, ManualOmega, ManualStiffness);
	}
	TestTrue(TEXT("Attached wheel manual wrapper cannot add a second update"),
		FMath::IsNearlyEqual(OwnedWheels[0]->GetWheelState().AngularVelocity, OwnedWheelOmega));
	TestTrue(TEXT("Attached axle manual wrapper was rejected before computing outputs"),
		OwnedInertia == -1.f && OwnedOmega == -1.f && OwnedStiffness == -1.f);
	TestTrue(TEXT("Standalone wheel really integrates applied torque"), StandaloneWheel->GetWheelState().AngularVelocity > 0.f);
	TestTrue(TEXT("Standalone axle really integrates and returns shaft feedback"), ManualInertia > 0.f && ManualOmega > 0.f);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
