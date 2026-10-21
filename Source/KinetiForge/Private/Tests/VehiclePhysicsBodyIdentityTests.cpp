// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "KinetiForge.h"
#include "Chaos/ParticleHandle.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Misc/AutomationTest.h"
#include "Modules/ModuleManager.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "PhysicsEngine/BodyInstance.h"
#include "PhysicsProxy/SingleParticlePhysicsProxy.h"
#include "PBDRigidsSolver.h"
#include "VehiclePhysicsBodyBinding.h"
#include "VehiclePhysicsStep.h"

namespace VehiclePhysicsBodyIdentityTests
{
	struct FWorldOwner
	{
		UWorld* World = nullptr;
		~FWorldOwner() { if (World) World->DestroyWorld(false); }
	};

	UWorld* CreateWorld(FAutomationTestBase& Test, FWorldOwner& Owner, const FName Name)
	{
		Owner.World = UWorld::CreateWorld(EWorldType::Game, false, Name, nullptr, true, ERHIFeatureLevel::Num, nullptr, true);
		if (!Test.TestNotNull(TEXT("Body identity world created"), Owner.World)) return nullptr;
		Owner.World->InitWorld(UWorld::InitializationValues().AllowAudioPlayback(false)
			.CreatePhysicsScene(true).RequiresHitProxies(false).CreateNavigation(false)
			.CreateAISystem(false).ShouldSimulatePhysics(false).SetTransactional(false));
		if (!Test.TestNotNull(TEXT("Body identity world has Chaos scene"), Owner.World->GetPhysicsScene())) return nullptr;
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

	UBoxComponent* AddDynamicBody(UWorld& World, const FName Name, const FVector Location)
	{
		AActor* Actor = World.SpawnActor<AActor>(Location, FRotator::ZeroRotator);
		if (!Actor) return nullptr;
		UBoxComponent* Body = NewObject<UBoxComponent>(Actor, Name);
		Actor->AddInstanceComponent(Body);
		Actor->SetRootComponent(Body);
		Body->SetBoxExtent(FVector(20.f));
		Body->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Body->SetCollisionObjectType(ECC_PhysicsBody);
		Body->SetCollisionResponseToAllChannels(ECR_Block);
		Body->SetSimulatePhysics(true);
		Body->SetEnableGravity(false);
		Body->SetMassOverrideInKg(NAME_None, 2.f, true);
		Body->RegisterComponent();
		Body->RecreatePhysicsState();
		Body->SetWorldLocation(Location, false, nullptr, ETeleportType::TeleportPhysics);
		return Body;
	}

	struct FObservation
	{
		int32 Calls = 0;
		bool bResolved = false;
		FVector Position = FVector::ZeroVector;
		FVector VelocityBefore = FVector::ZeroVector;
		FVector ImpulseVelocityBefore = FVector::ZeroVector;
		FVector ImpulseVelocityAfter = FVector::ZeroVector;
	};

	FVehiclePhysicsScene::FStepPtr RegisterProbe(const TSharedPtr<FVehiclePhysicsScene, ESPMode::ThreadSafe>& Host,
		const FVehiclePhysicsBodyIdentity Identity, FObservation& Observation, const bool bApplyImpulse)
	{
		return Host->RegisterStep([Identity, &Observation, bApplyImpulse](float, const FVehiclePhysicsExecutionContext& Context)
		{
			++Observation.Calls;
			Chaos::FRigidBodyHandle_Internal* Handle = Context.ResolveBody(Identity);
			Observation.bResolved = Handle != nullptr;
			if (!Handle) return;
			Observation.Position = FVector(Handle->X());
			Observation.VelocityBefore = FVector(Handle->V());
			if (bApplyImpulse)
			{
				Observation.ImpulseVelocityBefore = FVector(Handle->LinearImpulseVelocity());
				Handle->SetLinearImpulse(Handle->LinearImpulse() + Chaos::FVec3(2.f, 0.f, 0.f), false);
				Observation.ImpulseVelocityAfter = FVector(Handle->LinearImpulseVelocity());
			}
		});
	}

	void RemoveStep(const TSharedPtr<FVehiclePhysicsScene, ESPMode::ThreadSafe>& Host,
		FVehiclePhysicsScene::FStepPtr& Step)
	{
		if (Host && Step) Host->UnregisterStep(Step);
		Step.Reset();
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehiclePhysicsBodyIdentityTest,
	"KinetiForge.Physics.SceneHost.BodyIdentityResolution",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehiclePhysicsBodyIdentityTest::RunTest(const FString& Parameters)
{
	using namespace VehiclePhysicsBodyIdentityTests;
	FKinetiForgeModule& Module = FModuleManager::Get().GetModuleChecked<FKinetiForgeModule>(TEXT("KinetiForge"));
	TestFalse(TEXT("Null BodyInstance captures no identity"), FVehiclePhysicsBodyIdentity::Capture(nullptr).IsValid());
	FBodyInstance UninitializedBodyInstance;
	TestFalse(TEXT("BodyInstance without a physics actor captures no identity"), FVehiclePhysicsBodyIdentity::Capture(&UninitializedBodyInstance).IsValid());

	FWorldOwner WorldAOwner;
	FWorldOwner WorldBOwner;
	UWorld* WorldA = CreateWorld(*this, WorldAOwner, TEXT("BodyIdentitySceneA"));
	UWorld* WorldB = CreateWorld(*this, WorldBOwner, TEXT("BodyIdentitySceneB"));
	if (!WorldA || !WorldB) return false;
	UBoxComponent* ExistingBody = AddDynamicBody(*WorldA, TEXT("ExistingBody"), FVector(0.f, 0.f, 100.f));
	UBoxComponent* OtherWorldBody = AddDynamicBody(*WorldB, TEXT("OtherWorldBody"), FVector(1000.f, 0.f, 100.f));
	if (!TestNotNull(TEXT("Pre-existing dynamic body registered"), ExistingBody)
		|| !TestNotNull(TEXT("Other-scene dynamic body registered"), OtherWorldBody)) return false;
	const FVehiclePhysicsBodyIdentity ExistingIdentity = FVehiclePhysicsBodyIdentity::Capture(ExistingBody->GetBodyInstance());
	const FVehiclePhysicsBodyIdentity OtherWorldIdentity = FVehiclePhysicsBodyIdentity::Capture(OtherWorldBody->GetBodyInstance());
	if (!TestTrue(TEXT("Valid bodies yield immutable identities"), ExistingIdentity.IsValid() && OtherWorldIdentity.IsValid())) return false;
	Flush(*WorldA);
	Flush(*WorldB); // Ensure the pre-existing particles are owned by their solvers before host registration.

	auto HostA = Module.AcquirePhysicsScene(WorldA->GetPhysicsScene());
	auto HostB = Module.AcquirePhysicsScene(WorldB->GetPhysicsScene());
	if (!TestTrue(TEXT("Both worlds acquired explicit scene hosts"), HostA.IsValid() && HostB.IsValid())) return false;
	FObservation ExistingObservation;
	auto ExistingStep = RegisterProbe(HostA, ExistingIdentity, ExistingObservation, true);
	if (!TestNotNull(TEXT("Existing identity step registered"), ExistingStep.Get())) return false;
	Flush(*WorldA);
	TestTrue(TEXT("Host resolves a body registered before its callback"), ExistingObservation.bResolved);
	TestTrue(TEXT("Resolved handle matches captured body's position"), FMath::IsNearlyEqual(ExistingObservation.Position.Z, 100.f, 1.f));
	TestTrue(TEXT("Current-solver handle receives impulse"), FMath::IsNearlyEqual(
		ExistingObservation.ImpulseVelocityAfter.X - ExistingObservation.ImpulseVelocityBefore.X, 1.f, 1.e-4f));
	RemoveStep(HostA, ExistingStep);
	const float VelocityAfterImpulse = ExistingObservation.VelocityBefore.X;
	FObservation NextObservation;
	auto NextStep = RegisterProbe(HostA, ExistingIdentity, NextObservation, false);
	Flush(*WorldA);
	TestTrue(TEXT("Body resolves on a following callback after step replacement"), NextObservation.bResolved
		&& FMath::IsNearlyEqual(NextObservation.VelocityBefore.X - VelocityAfterImpulse, 1.f, 1.e-3f));
	RemoveStep(HostA, NextStep);

	FObservation WrongSceneObservation;
	auto WrongSceneStep = RegisterProbe(HostB, ExistingIdentity, WrongSceneObservation, false);
	Flush(*WorldB);
	TestFalse(TEXT("Identity from another solver cannot resolve in this scene"), WrongSceneObservation.bResolved);
	RemoveStep(HostB, WrongSceneStep);
	FObservation OtherSceneObservation;
	auto OtherSceneStep = RegisterProbe(HostB, OtherWorldIdentity, OtherSceneObservation, false);
	Flush(*WorldB);
	TestTrue(TEXT("Other solver resolves only its own body identity"), OtherSceneObservation.bResolved
		&& FMath::IsNearlyEqual(OtherSceneObservation.Position.X, 1000.f, 1.f));
	RemoveStep(HostB, OtherSceneStep);

	// A body registered after callback creation is found from that callback's current solver view.
	UBoxComponent* LateBody = AddDynamicBody(*WorldA, TEXT("LateBody"), FVector(300.f, 0.f, 100.f));
	if (!TestNotNull(TEXT("Late body registered after scene host creation"), LateBody)) return false;
	const FVehiclePhysicsBodyIdentity OldIdentity = FVehiclePhysicsBodyIdentity::Capture(LateBody->GetBodyInstance());
	FObservation LateObservation;
	auto LateStep = RegisterProbe(HostA, OldIdentity, LateObservation, false);
	Flush(*WorldA);
	TestTrue(TEXT("Current callback discovers a late registered body"), LateObservation.bResolved
		&& FMath::IsNearlyEqual(LateObservation.Position.X, 300.f, 1.f));
	RemoveStep(HostA, LateStep);

	TArray<int32> ResolverMissCounts;
	TArray<bool> ResolverResults;
	auto ResolverStep = HostA->RegisterStep(
		[ExistingIdentity, OldIdentity, OtherWorldIdentity, &ResolverMissCounts, &ResolverResults]
		(float, const FVehiclePhysicsExecutionContext& Context)
	{
		FVehiclePhysicsMacrostepBodyResolver Resolver(Context);
		auto* ExistingFirst = Resolver.ResolveBody(ExistingIdentity);
		auto* ExistingAgain = Resolver.ResolveBody(ExistingIdentity);
		auto* LateFirst = Resolver.ResolveBody(OldIdentity);
		auto* LateAgain = Resolver.ResolveBody(OldIdentity);
		auto* WrongSceneFirst = Resolver.ResolveBody(OtherWorldIdentity);
		auto* WrongSceneAgain = Resolver.ResolveBody(OtherWorldIdentity);
		ResolverResults.Add(ExistingFirst && ExistingFirst == ExistingAgain
			&& LateFirst && LateFirst == LateAgain && !WrongSceneFirst && !WrongSceneAgain);
		ResolverMissCounts.Add(Resolver.GetUnderlyingResolveCount());
	});
	if (!TestNotNull(TEXT("Macrostep resolver regression step registered"), ResolverStep.Get())) return false;
	Flush(*WorldA);
	Flush(*WorldA);
	TestTrue(TEXT("Repeated body and failed identity reuse their macrostep lookup"),
		ResolverResults.Num() >= 2 && ResolverResults[0] && ResolverResults[1]);
	TestTrue(TEXT("Each callback scans once per distinct identity, including cached misses"),
		ResolverMissCounts.Num() >= 2 && ResolverMissCounts[0] == 3 && ResolverMissCounts[1] == 3);

	LateBody->RecreatePhysicsState();
	const FVehiclePhysicsBodyIdentity NewIdentity = FVehiclePhysicsBodyIdentity::Capture(LateBody->GetBodyInstance());
	TestTrue(TEXT("Recreated physics state yields a valid identity"), NewIdentity.IsValid());
	Flush(*WorldA);
	TestTrue(TEXT("A fresh callback resolver does not reuse the prior body's cached handle"),
		ResolverResults.Num() >= 3 && !ResolverResults.Last() && ResolverMissCounts.Last() == 3);
	RemoveStep(HostA, ResolverStep);
	FObservation StaleObservation;
	auto StaleStep = RegisterProbe(HostA, OldIdentity, StaleObservation, false);
	Flush(*WorldA);
	TestFalse(TEXT("Old identity does not resolve after physics-state recreation"), StaleObservation.bResolved);
	RemoveStep(HostA, StaleStep);
	FObservation RecreatedObservation;
	auto RecreatedStep = RegisterProbe(HostA, NewIdentity, RecreatedObservation, false);
	Flush(*WorldA);
	TestTrue(TEXT("New identity resolves the recreated body"), RecreatedObservation.bResolved);
	RemoveStep(HostA, RecreatedStep);

	FObservation NullIdentityObservation;
	auto NullIdentityStep = RegisterProbe(HostA, FVehiclePhysicsBodyIdentity(), NullIdentityObservation, false);
	Flush(*WorldA);
	TestFalse(TEXT("Empty identity does not resolve a body"), NullIdentityObservation.bResolved);
	RemoveStep(HostA, NullIdentityStep);

	FObservation UnregisteredObservation;
	auto UnregisteredStep = RegisterProbe(HostA, NewIdentity, UnregisteredObservation, false);
	TestTrue(TEXT("Unregister removes the old identity entry"), HostA->UnregisterStep(UnregisteredStep));
	Flush(*WorldA);
	TestEqual(TEXT("Removed old entry never executes on a later solver step"), UnregisteredObservation.Calls, 0);

	LateBody->DestroyComponent();
	FObservation DestroyedObservation;
	auto DestroyedStep = RegisterProbe(HostA, NewIdentity, DestroyedObservation, false);
	Flush(*WorldA);
	TestFalse(TEXT("Destroyed body identity does not resolve"), DestroyedObservation.bResolved);
	RemoveStep(HostA, DestroyedStep);
	HostA->Close();
	HostB->Close();
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
