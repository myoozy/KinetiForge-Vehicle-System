// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "VehicleAuxiliaryLaneTestComponents.h"
#include "VehicleAsyncTickComponent.h"
#include "VehicleAxleAssemblyComponent.h"
#include "VehicleDriveAssemblyComponent.h"
#include "VehicleWheelComponent.h"
#include "VehicleWheelBridge.h"
#include "VehiclePhysicsScene.h"
#include "KinetiForge.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "HAL/PlatformTLS.h"
#include "HAL/PlatformAtomics.h"
#include "Misc/AutomationTest.h"
#include "Modules/ModuleManager.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "PBDRigidsSolver.h"
#include "Misc/ScopeExit.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleAuxiliaryFrozenGameThreadLaneTest,
	"KinetiForge.Drivetrain.AuxiliaryFrozenGameThreadLane",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleAuxiliaryFrozenGameThreadLaneTest::RunTest(const FString& Parameters)
{
	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false, TEXT("VehicleAuxiliaryFrozenGameThreadLane"), nullptr,
		true, ERHIFeatureLevel::Num, nullptr, true);
	if (!TestNotNull(TEXT("TaskGraph physics world created"), World)) return false;
	ON_SCOPE_EXIT { World->DestroyWorld(false); };
	World->InitWorld(UWorld::InitializationValues().AllowAudioPlayback(false).CreatePhysicsScene(true)
		.RequiresHitProxies(false).CreateNavigation(false).CreateAISystem(false).ShouldSimulatePhysics(true).SetTransactional(false));
	if (!TestNotNull(TEXT("Chaos scene exists"), World->GetPhysicsScene())) return false;
	World->GetPhysicsScene()->GetSolver()->DisableAsyncMode();
	World->GetPhysicsScene()->GetSolver()->SetThreadingMode_External(Chaos::EThreadingModeTemp::TaskGraph);
	const auto Flush = [World]()
	{
		const FVector Gravity(0.f, 0.f, -980.f);
		World->GetPhysicsScene()->SetUpForFrame(&Gravity, 1.f / 60.f, 0.f, 1.f / 60.f, 1.f / 60.f, 1, false);
		World->GetPhysicsScene()->StartFrame();
		World->GetPhysicsScene()->WaitPhysScenes();
		World->GetPhysicsScene()->EndFrame();
	};
	APawn* Pawn = World->SpawnActor<APawn>(FVector(0.f, 0.f, 1000.f), FRotator::ZeroRotator);
	if (!Pawn) return false;
	UBoxComponent* Body = NewObject<UBoxComponent>(Pawn, TEXT("AuxLaneBody"));
	Pawn->AddInstanceComponent(Body);
	Pawn->SetRootComponent(Body);
	Body->SetBoxExtent(FVector(50.f));
	Body->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Body->SetSimulatePhysics(true);
	Body->RegisterComponent();
	Body->RecreatePhysicsState();
	UVehicleAxleAssemblyComponent* Axle = NewObject<UVehicleAxleAssemblyComponent>(Pawn, TEXT("AuxLaneAxle"));
	Pawn->AddInstanceComponent(Axle);
	Axle->SetupAttachment(Body);
	Axle->RegisterComponent();
	UVehicleDriveAssemblyComponent* Drive = NewObject<UVehicleDriveAssemblyComponent>(Pawn, TEXT("AuxLaneDrive"));
	Pawn->AddInstanceComponent(Drive);
	Drive->SetupAttachment(Body);
	Drive->bUseIsolatedVehicleRuntime = true;
	Drive->bUpdatePhysicsAutomatically = true;
	Drive->AutoGearboxConfig.bAutomaticGearbox = false;
	FVehicleAxleSpawnTemplate Template;
	Template.bUseExistingComponent = true;
	Template.AxleComponentName = Axle->GetFName();
	Drive->AxleTemplates = {Template};
	Drive->RegisterComponent();
	UVehicleAuxiliaryLaneTestAero* Aero = NewObject<UVehicleAuxiliaryLaneTestAero>(Pawn, TEXT("AuxLaneAero"));
	Pawn->AddInstanceComponent(Aero);
	Aero->SetupAttachment(Body);
	Aero->RegisterComponent();
	UVehicleAuxiliaryLaneTestSpringArm* SpringArm = NewObject<UVehicleAuxiliaryLaneTestSpringArm>(Pawn, TEXT("AuxLaneSpringArm"));
	Pawn->AddInstanceComponent(SpringArm);
	SpringArm->SetupAttachment(Body);
	SpringArm->RegisterComponent();
	Pawn->DispatchBeginPlay();
	UVehicleAsyncTickComponent* Ticker = UVehicleAsyncTickComponent::FindVehicleAsyncTickComponent(Pawn);
	if (!TestNotNull(TEXT("Vehicle ticker registered"), Ticker)) return false;
	TestFalse(TEXT("Default ticker uses the worker lane"), Ticker->bUpdatePhysicsOnGameThread);
	Flush(); // Consume initial scene registration before observing per-frame work.
	Drive->TickComponent(1.f / 60.f, LEVELTICK_All, nullptr);
	Drive->TickComponent(1.f / 60.f, LEVELTICK_All, nullptr);
	auto Host = FModuleManager::Get().GetModuleChecked<FKinetiForgeModule>(TEXT("KinetiForge")).AcquirePhysicsScene(World->GetPhysicsScene());
	if (!TestNotNull(TEXT("Physics scene host available"), Host.Get())) return false;
	volatile int32 WitnessCalls = 0;
	volatile int32 WitnessThread = -1;
	const int32 GT = static_cast<int32>(FPlatformTLS::GetCurrentThreadId());
	auto Witness = Host->RegisterStep([&](float, const FVehiclePhysicsExecutionContext&)
	{
		FPlatformAtomics::InterlockedIncrement(&WitnessCalls);
		FPlatformAtomics::InterlockedCompareExchange(&WitnessThread,
			static_cast<int32>(FPlatformTLS::GetCurrentThreadId()), -1);
	});
	if (!TestNotNull(TEXT("FixedDrive taskgraph witness registered"), Witness.Get())) return false;
	ON_SCOPE_EXIT { Host->UnregisterStep(Witness); };
	TArray<UVehicleWheelComponent*> Wheels = Drive->GetWheels();
	if (!TestTrue(TEXT("Fixture has a wheel"), Wheels.Num() > 0)) return false;
	const auto Bridge = Wheels[0]->GetWheelBridge();
	if (!TestTrue(TEXT("Wheel result bridge available"), Bridge.IsValid())) return false;
	uint64 Seq = Bridge->ReadResult().PublicationSequence;
	const int32 CallsBefore = Aero->Calls.Load();
	for (int32 Frame = 0; Frame < 3; ++Frame)
	{
		SpringArm->SetPhysicsStateSentinels();
		Flush();
		TestEqual(TEXT("Aero runs once on each physical frame"), Aero->Calls.Load(), CallsBefore + Frame + 1);
		TestEqual(TEXT("Wheel result publishes once per physical frame"), Bridge->ReadResult().PublicationSequence, ++Seq);
		TestEqual(TEXT("Spring arm physics delta is refreshed"), SpringArm->ReadLastPhysicsDeltaTime(), 1.f / 60.f);
		TestEqual(TEXT("Spring arm frame accumulator is reset"), SpringArm->ReadTimeSinceLastPhysTick(), 0.f);
		TestTrue(TEXT("Unlagged spring arm follows the same current PT body pose observed by aero"),
			SpringArm->ReadPhysicsOriginWorldTransform().Equals(Aero->LastParentWorldTransform, 1.e-3f));
	}
	TestEqual(TEXT("Aero runs on game thread frozen callback"), Aero->ThreadId.Load(), GT);
	TestTrue(TEXT("FixedDrive witness ran on TaskGraph worker"),
		FPlatformAtomics::InterlockedCompareExchange(&WitnessCalls, 0, 0) >= 3
		&& FPlatformAtomics::InterlockedCompareExchange(&WitnessThread, -1, -1) != GT);

	const int32 BeforeUnregister = Aero->Calls.Load();
	Ticker->UnregisterComponent();
	Flush();
	TestEqual(TEXT("Unregistered ticker stops auxiliary callback"), Aero->Calls.Load(), BeforeUnregister);
	Ticker->RegisterComponent();
	Flush();
	TestEqual(TEXT("Re-registered ticker resumes exactly once"), Aero->Calls.Load(), BeforeUnregister + 1);
	Ticker->UnregisterComponent();
	Ticker->RegisterComponent();
	Flush();
	TestEqual(TEXT("Repeated ticker registration does not duplicate aero"), Aero->Calls.Load(), BeforeUnregister + 2);
	return true;
}

#endif
