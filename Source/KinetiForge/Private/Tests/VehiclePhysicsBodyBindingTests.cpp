// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "Chaos/SimCallbackObject.h"
#include "Chaos/PBDRigidsEvolution.h"
#include "Chaos/PBDRigidsSOAs.h"
#include "Chaos/ParticleHandle.h"
#include "Chaos/Framework/PhysicsProxyBase.h"
#include "PhysicsProxy/SingleParticlePhysicsProxy.h"
#include "PBDRigidsSolver.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "Misc/AutomationTest.h"
#include "UObject/StrongObjectPtr.h"

namespace VehiclePhysicsBodyBindingTest
{
	struct FBindingInput : Chaos::FSimCallbackInput
	{
		TSharedPtr<FProxyTimestampBase, ESPMode::ThreadSafe> Timestamp;
		bool bApplyImpulse = false;
		uint64 RequestId = 0;
		void Reset() { Timestamp.Reset(); bApplyImpulse = false; RequestId = 0; }
	};

	struct FBindingOutput : Chaos::FSimCallbackOutput
	{
		bool bFound = false;
		bool bPhysicsApiValid = false;
		bool bImpulseApplied = false;
		uint64 RequestId = 0;
		int32 SolverParticleCount = 0;
		FVector Position = FVector::ZeroVector;
		FVector VelocityBefore = FVector::ZeroVector;
		FVector ImpulseVelocityBefore = FVector::ZeroVector;
		FVector ImpulseVelocityAfter = FVector::ZeroVector;
		void Reset()
		{
			bFound = false;
			bPhysicsApiValid = false;
			bImpulseApplied = false;
			RequestId = 0;
			SolverParticleCount = 0;
			Position = FVector::ZeroVector;
			VelocityBefore = FVector::ZeroVector;
			ImpulseVelocityBefore = FVector::ZeroVector;
			ImpulseVelocityAfter = FVector::ZeroVector;
		}
	};

	// Test-only prototype: the only cross-thread identity is the retained timestamp value.
	// Proxy pointers and FWeakParticleHandle are created/read exclusively in PreSimulate_Internal.
	class FBodyBindingCallback final : public Chaos::TSimCallbackObject<FBindingInput, FBindingOutput>
	{
	public:
		uint64 Submit(const TSharedPtr<FProxyTimestampBase, ESPMode::ThreadSafe>& Timestamp, const bool bApplyImpulse)
		{
			FBindingInput* Input = GetProducerInputData_External();
			Input->Timestamp = Timestamp;
			Input->bApplyImpulse = bApplyImpulse;
			Input->RequestId = ++LastSubmittedId;
			return Input->RequestId;
		}

	private:
		void OnPreSimulate_Internal() override
		{
			const FBindingInput* Input = GetConsumerInput_Internal();
			FBindingOutput& Output = GetProducerOutputData_Internal();
			if (!Input || !Input->Timestamp.IsValid()) return;

			Output.RequestId = Input->RequestId;
			Chaos::FPBDRigidsSolver* SolverRef = static_cast<Chaos::FPBDRigidsSolver*>(GetSolver());
			if (!SolverRef || !SolverRef->GetEvolution()) return;
			const auto& ParticleHandles = SolverRef->GetEvolution()->GetParticles().GetParticleHandles();
			const int32 NumParticleHandles = static_cast<int32>(ParticleHandles.Size());
			Output.SolverParticleCount = NumParticleHandles;

			Chaos::FGeometryParticleHandle* Resolved = nullptr;
			if (Chaos::FGeometryParticleHandle* Cached = CachedHandle.GetHandle())
			{
				IPhysicsProxyBase* Proxy = Cached->PhysicsProxy();
				auto* SingleProxy = Proxy && Proxy->GetType() == EPhysicsProxyType::SingleParticleProxy
					? static_cast<Chaos::FSingleParticlePhysicsProxy*>(Proxy) : nullptr;
				if (SingleProxy && SingleProxy->GetPhysicsThreadAPI()
					&& static_cast<void*>(SingleProxy->GetHandle_LowLevel()) == static_cast<void*>(Cached)
					&& SingleProxy->GetSyncTimestamp().Get() == Input->Timestamp.Get())
				{
					Resolved = Cached;
				}
				else
				{
					CachedHandle = Chaos::FWeakParticleHandle();
				}
			}

			if (!Resolved)
			{
				for (int32 ParticleIndex = 0; ParticleIndex < NumParticleHandles; ++ParticleIndex)
				{
					Chaos::FGeometryParticleHandle* Particle = ParticleHandles.Handle(ParticleIndex).Get();
					if (!Particle) continue;
					IPhysicsProxyBase* Proxy = Particle->PhysicsProxy();
					if (!Proxy || Proxy->GetSyncTimestamp().Get() != Input->Timestamp.Get()) continue;
					if (Proxy->GetType() != EPhysicsProxyType::SingleParticleProxy) continue;
					auto* SingleProxy = static_cast<Chaos::FSingleParticlePhysicsProxy*>(Proxy);
					if (!SingleProxy->GetPhysicsThreadAPI()
						|| static_cast<void*>(SingleProxy->GetHandle_LowLevel()) != static_cast<void*>(Particle)) continue;
					Resolved = Particle;
					CachedHandle = Particle->WeakParticleHandle();
					break;
				}
			}

			if (!Resolved) return;
			IPhysicsProxyBase* Proxy = Resolved->PhysicsProxy();
			auto* SingleProxy = Proxy && Proxy->GetType() == EPhysicsProxyType::SingleParticleProxy
				? static_cast<Chaos::FSingleParticlePhysicsProxy*>(Proxy) : nullptr;
			Chaos::FRigidBodyHandle_Internal* PhysicsHandle = SingleProxy ? SingleProxy->GetPhysicsThreadAPI() : nullptr;
			if (!PhysicsHandle || static_cast<void*>(SingleProxy->GetHandle_LowLevel()) != static_cast<void*>(Resolved)
				|| SingleProxy->GetSyncTimestamp().Get() != Input->Timestamp.Get()) return;

			Output.bFound = true;
			Output.bPhysicsApiValid = true;
			Output.Position = FVector(PhysicsHandle->X());
			Output.VelocityBefore = FVector(PhysicsHandle->V());
			if (Input->bApplyImpulse)
			{
				const Chaos::FVec3 AddedImpulse(2.f, 0.f, 0.f); // 2 kg body: this impulse adds 1 cm/s.
				Output.ImpulseVelocityBefore = FVector(PhysicsHandle->LinearImpulseVelocity());
				PhysicsHandle->SetLinearImpulse(PhysicsHandle->LinearImpulse() + AddedImpulse, false);
				Output.ImpulseVelocityAfter = FVector(PhysicsHandle->LinearImpulseVelocity());
				Output.bImpulseApplied = true;
			}
		}

		uint64 LastSubmittedId = 0;
		Chaos::FWeakParticleHandle CachedHandle;
	};

	struct FWorldOwner
	{
		UWorld* World = nullptr;
		~FWorldOwner() { if (World) World->DestroyWorld(false); }
	};

	UWorld* CreateWorld(FAutomationTestBase& Test, FWorldOwner& Owner, const FName Name)
	{
		Owner.World = UWorld::CreateWorld(EWorldType::Game, false, Name, nullptr, true, ERHIFeatureLevel::Num, nullptr, true);
		if (!Test.TestNotNull(TEXT("Physics binding test world created"), Owner.World)) return nullptr;
		Owner.World->InitWorld(UWorld::InitializationValues().AllowAudioPlayback(false)
			.CreatePhysicsScene(true).RequiresHitProxies(false).CreateNavigation(false)
			.CreateAISystem(false).ShouldSimulatePhysics(false).SetTransactional(false));
		if (!Test.TestNotNull(TEXT("Physics binding world has Chaos scene"), Owner.World->GetPhysicsScene())) return nullptr;
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

	TSharedPtr<FProxyTimestampBase, ESPMode::ThreadSafe> CaptureTimestampOnGameThread(UBoxComponent& Body)
	{
		FPhysicsActorHandle& ActorHandle = Body.GetBodyInstance()->GetPhysicsActorHandle();
		if (!ActorHandle) return nullptr;
		return ActorHandle->GetSyncTimestamp();
	}

	bool StepAndRead(FAutomationTestBase& Test, UWorld& World, FBodyBindingCallback& Callback,
		const TSharedPtr<FProxyTimestampBase, ESPMode::ThreadSafe>& Timestamp, const bool bApplyImpulse,
		FBindingOutput& OutObservation)
	{
		const uint64 RequestId = Callback.Submit(Timestamp, bApplyImpulse);
		Flush(World);
		bool bGotRequest = false;
		while (auto Output = Callback.PopFutureOutputData_External())
		{
			const FBindingOutput& Result = *static_cast<const FBindingOutput*>(Output.Get());
			if (Result.RequestId != RequestId) continue;
			OutObservation.bFound = Result.bFound;
			OutObservation.bPhysicsApiValid = Result.bPhysicsApiValid;
			OutObservation.bImpulseApplied = Result.bImpulseApplied;
			OutObservation.RequestId = Result.RequestId;
			OutObservation.SolverParticleCount = Result.SolverParticleCount;
			OutObservation.Position = Result.Position;
			OutObservation.VelocityBefore = Result.VelocityBefore;
			OutObservation.ImpulseVelocityBefore = Result.ImpulseVelocityBefore;
			OutObservation.ImpulseVelocityAfter = Result.ImpulseVelocityAfter;
			bGotRequest = true;
			break;
		}
		return Test.TestTrue(TEXT("Pre-simulate callback produced the requested observation"), bGotRequest);
	}

	void UnregisterCallback(UWorld& World, FBodyBindingCallback*& Callback)
	{
		if (!Callback) return;
		World.GetPhysicsScene()->GetSolver()->UnregisterAndFreeSimCallbackObject_External(Callback);
		Callback = nullptr;
		Flush(World);
	}

	struct FCallbackOwner
	{
		UWorld* World = nullptr;
		FBodyBindingCallback* Callback = nullptr;
		~FCallbackOwner() { if (World) UnregisterCallback(*World, Callback); }
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehiclePhysicsBodyBindingTest,
	"KinetiForge.Physics.BodyBinding.TimestampIdentityAndLifetime",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehiclePhysicsBodyBindingTest::RunTest(const FString& Parameters)
{
	using namespace VehiclePhysicsBodyBindingTest;
	FWorldOwner WorldAOwner;
	FWorldOwner WorldBOwner;
	FCallbackOwner CallbackAOwner;
	FCallbackOwner CallbackBOwner;
	UWorld* WorldA = CreateWorld(*this, WorldAOwner, TEXT("BodyBindingWorldA"));
	UWorld* WorldB = CreateWorld(*this, WorldBOwner, TEXT("BodyBindingWorldB"));
	if (!WorldA || !WorldB) return false;
	CallbackAOwner.World = WorldA;
	CallbackBOwner.World = WorldB;

	UBoxComponent* ExistingBody = AddDynamicBody(*WorldA, TEXT("ExistingBody"), FVector(0.f, 0.f, 100.f));
	UBoxComponent* OtherWorldBody = AddDynamicBody(*WorldB, TEXT("OtherWorldBody"), FVector(1000.f, 0.f, 100.f));
	if (!TestNotNull(TEXT("Pre-existing target body registered"), ExistingBody)
		|| !TestNotNull(TEXT("Other-world target body registered"), OtherWorldBody)) return false;
	const auto ExistingToken = CaptureTimestampOnGameThread(*ExistingBody);
	const auto OtherWorldToken = CaptureTimestampOnGameThread(*OtherWorldBody);
	if (!TestTrue(TEXT("GT captured shared timestamp tokens while body instances are valid"), ExistingToken.IsValid() && OtherWorldToken.IsValid())) return false;
	// Complete PT registration before creating callbacks, so the first case proves late callback attachment.
	Flush(*WorldA);
	Flush(*WorldB);

	CallbackAOwner.Callback = WorldA->GetPhysicsScene()->GetSolver()->CreateAndRegisterSimCallbackObject_External<FBodyBindingCallback>();
	CallbackBOwner.Callback = WorldB->GetPhysicsScene()->GetSolver()->CreateAndRegisterSimCallbackObject_External<FBodyBindingCallback>();
	if (!TestNotNull(TEXT("Callback A registered"), CallbackAOwner.Callback) || !TestNotNull(TEXT("Callback B registered"), CallbackBOwner.Callback)) return false;

	FBindingOutput Observation;
	if (!StepAndRead(*this, *WorldA, *CallbackAOwner.Callback, ExistingToken, true, Observation)) return false;
	TestTrue(TEXT("Callback resolves a body that predates callback registration"), Observation.bFound && Observation.bPhysicsApiValid);
	TestTrue(TEXT("Resolved particle corresponds to captured body pose"), FMath::IsNearlyEqual(Observation.Position.Z, 100.f, 1.f));
	TestTrue(TEXT("PT impulse is applied to the matching body"), Observation.bImpulseApplied
		&& FMath::IsNearlyEqual(Observation.ImpulseVelocityAfter.X - Observation.ImpulseVelocityBefore.X, 1.f, 1.e-4f));
	const float VelocityBeforeImpulse = Observation.VelocityBefore.X;
	if (!StepAndRead(*this, *WorldA, *CallbackAOwner.Callback, ExistingToken, false, Observation)) return false;
	TestTrue(TEXT("The addressed body carries the impulse into the next PT step"),
		FMath::IsNearlyEqual(Observation.VelocityBefore.X - VelocityBeforeImpulse, 1.f, 1.e-3f));

	if (!StepAndRead(*this, *WorldB, *CallbackBOwner.Callback, ExistingToken, false, Observation)) return false;
	TestFalse(TEXT("A token from another solver/world cannot resolve"), Observation.bFound);
	if (!StepAndRead(*this, *WorldB, *CallbackBOwner.Callback, OtherWorldToken, false, Observation)) return false;
	TestTrue(TEXT("World B resolves only its own body token"), Observation.bFound && Observation.bPhysicsApiValid);

	// Register a new body after the callback has already run, then submit its GT-captured identity.
	UBoxComponent* LateBody = AddDynamicBody(*WorldA, TEXT("LateBody"), FVector(300.f, 0.f, 100.f));
	if (!TestNotNull(TEXT("Late body registered after callback started"), LateBody)) return false;
	const auto LateToken = CaptureTimestampOnGameThread(*LateBody);
	if (!StepAndRead(*this, *WorldA, *CallbackAOwner.Callback, LateToken, false, Observation)) return false;
	TestTrue(TEXT("Per-step scan discovers a body created after callback registration"), Observation.bFound && Observation.bPhysicsApiValid);
	if (!StepAndRead(*this, *WorldA, *CallbackAOwner.Callback, ExistingToken, false, Observation)) return false;
	TestTrue(TEXT("Switching the cached token does not clear the engine weak slot"), Observation.bFound && Observation.bPhysicsApiValid);
	if (!StepAndRead(*this, *WorldA, *CallbackAOwner.Callback, LateToken, false, Observation)) return false;
	TestTrue(TEXT("Late body remains resolvable on a repeated cached lookup"), Observation.bFound && Observation.bPhysicsApiValid);

	const auto OldRecreateToken = CaptureTimestampOnGameThread(*LateBody);
	LateBody->RecreatePhysicsState();
	const auto NewRecreateToken = CaptureTimestampOnGameThread(*LateBody);
	if (!TestTrue(TEXT("Recreated body receives a new shared identity token"), NewRecreateToken.IsValid() && NewRecreateToken.Get() != OldRecreateToken.Get())) return false;
	if (!StepAndRead(*this, *WorldA, *CallbackAOwner.Callback, OldRecreateToken, false, Observation)) return false;
	TestFalse(TEXT("Old identity stays unresolved after body recreation"), Observation.bFound);
	if (!StepAndRead(*this, *WorldA, *CallbackAOwner.Callback, NewRecreateToken, false, Observation)) return false;
	TestTrue(TEXT("New identity resolves recreated body"), Observation.bFound && Observation.bPhysicsApiValid);

	LateBody->DestroyComponent();
	if (!StepAndRead(*this, *WorldA, *CallbackAOwner.Callback, NewRecreateToken, false, Observation)) return false;
	TestFalse(TEXT("Destroyed body identity does not revive"), Observation.bFound);

	UnregisterCallback(*WorldA, CallbackAOwner.Callback);
	UnregisterCallback(*WorldB, CallbackBOwner.Callback);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
