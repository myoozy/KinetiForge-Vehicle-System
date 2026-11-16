// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehicleChassisMomentum.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "VehicleWheelCoordinatorComponent.h"
#include "VehicleWheelComponent.h"
#include "VehicleWheelPhysics.h"
#include "VehiclePhysicsScene.h"
#include "VehiclePhysicsStep.h"
#include "VehiclePhysicsBodyBinding.h"
#include "VehicleUtilities.h"
#include "KinetiForge.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Misc/AutomationTest.h"
#include "Misc/ScopeExit.h"
#include "Modules/ModuleManager.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "PBDRigidsSolver.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

namespace VehicleChassisMomentumTests
{
	struct FFixture
	{
		UWorld* World = nullptr;
		UBoxComponent* Body = nullptr;
		~FFixture() { if (World) World->DestroyWorld(false); }
		void Advance(float Dt)
		{
			const FVector Gravity = FVector::ZeroVector;
			auto* Scene = World->GetPhysicsScene();
			Scene->SetUpForFrame(&Gravity, Dt, 0.f, Dt, Dt, 1, false);
			Scene->StartFrame();
			Scene->WaitPhysScenes();
			Scene->EndFrame();
		}
		bool Create(FAutomationTestBase& Test, bool bStandaloneWheel)
		{
			World = UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true, ERHIFeatureLevel::Num, nullptr, true);
			if (!Test.TestNotNull(TEXT("Momentum test world"), World)) return false;
			World->InitWorld(UWorld::InitializationValues().AllowAudioPlayback(false).CreatePhysicsScene(true)
				.RequiresHitProxies(false).CreateNavigation(false).CreateAISystem(false)
				.ShouldSimulatePhysics(true).SetTransactional(false));
			World->GetPhysicsScene()->GetSolver()->DisableAsyncMode();
			World->GetPhysicsScene()->GetSolver()->SetThreadingMode_External(Chaos::EThreadingModeTemp::SingleThread);
			auto* Actor = World->SpawnActor<APawn>(FVector(0., 0., 10000.), FRotator(15., 35., 10.));
			Body = NewObject<UBoxComponent>(Actor);
			Actor->AddInstanceComponent(Body);
			Actor->SetRootComponent(Body);
			Body->SetBoxExtent(FVector(200., 80., 50.));
			Body->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			Body->SetSimulatePhysics(true);
			Body->SetEnableGravity(false);
			Body->SetAngularDamping(0.f);
			auto* Material = NewObject<UPhysicalMaterial>(Body);
			Material->SleepLinearVelocityThreshold = 0.f;
			Material->SleepAngularVelocityThreshold = 0.f;
			Material->SleepCounterThreshold = 100000;
			Body->SetPhysMaterialOverride(Material);
			Body->RegisterComponent();
			Body->RecreatePhysicsState();
			UVehicleUtilities::SetInertiaTensor(Body, FVector(500., 1200., 1500.));
			if (bStandaloneWheel)
			{
				// Creates the coordinator via the standalone wheel's normal BeginPlay path.
				auto* Wheel = NewObject<UVehicleWheelComponent>(Actor);
				Actor->AddInstanceComponent(Wheel);
				Wheel->SetupAttachment(Body);
				Wheel->RegisterComponent();
			}
			else
			{
				auto* Coordinator = NewObject<UVehicleWheelCoordinatorComponent>(Actor);
				Actor->AddInstanceComponent(Coordinator);
				Coordinator->SetupAttachment(Body);
				Coordinator->RegisterComponent();
			}
			Actor->DispatchBeginPlay();
			Advance(1.f / 120.f);
			return Test.TestNotNull(TEXT("Momentum test body handle"), UVehicleUtilities::GetInternalHandle(Body));
		}
	};

	FVector Momentum(Chaos::FRigidBodyHandle_Internal& Body)
	{
#if PHYSICS_THREAD_CONTEXT
		Chaos::FPhysicsThreadContextScope PhysicsContext(true);
#endif
		const FQuat Q = Body.R() * Body.RotationOfMass();
		return Q.RotateVector(FVector(Body.I()) * Q.UnrotateVector(FVector(Body.W())));
	}
	double Energy(Chaos::FRigidBodyHandle_Internal& Body)
	{
#if PHYSICS_THREAD_CONTEXT
		Chaos::FPhysicsThreadContextScope PhysicsContext(true);
#endif
		return 0.5 * FVector::DotProduct(FVector(Body.W()), Momentum(Body));
	}
	FVector Velocity(Chaos::FRigidBodyHandle_Internal& Body)
	{
#if PHYSICS_THREAD_CONTEXT
		Chaos::FPhysicsThreadContextScope PhysicsContext(true);
#endif
		return FVector(Body.W());
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleChassisFreeMomentumTest,
	"KinetiForge.Chassis.Momentum.NoWheels",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleChassisFreeMomentumTest::RunTest(const FString& Parameters)
{
	using namespace VehicleChassisMomentumTests;
	FFixture Fixture;
	if (!Fixture.Create(*this, false)) return false;
	for (int32 Index = 0; Index < 10; ++Index) Fixture.Advance(1.f / 120.f);
	auto* Body = UVehicleUtilities::GetInternalHandle(Fixture.Body);
	TestTrue(TEXT("Coordinator without wheels does not start a stationary body spinning"), Velocity(*Body).IsNearlyZero());
	{
#if PHYSICS_THREAD_CONTEXT
		Chaos::FPhysicsThreadContextScope PhysicsContext(true);
#endif
		Body->SetW(FVector(0.7, 1., 1.2));
	}
	const FVector InitialMomentum = Momentum(*Body);
	const double InitialEnergy = Energy(*Body);
	double MaxMomentumError = 0., MaxEnergyError = 0.;
	for (int32 Index = 0; Index < 240; ++Index)
	{
		Fixture.Advance(1.f / 120.f);
		MaxMomentumError = FMath::Max(MaxMomentumError, (Momentum(*Body) - InitialMomentum).Size() / InitialMomentum.Size());
		MaxEnergyError = FMath::Max(MaxEnergyError, FMath::Abs(Energy(*Body) / InitialEnergy - 1.));
	}
	AddInfo(FString::Printf(TEXT("No wheels: peak relative momentum error=%g, energy error=%g"), MaxMomentumError, MaxEnergyError));
	TestTrue(TEXT("Asymmetric chassis conserves world angular momentum within integration tolerance"), MaxMomentumError < 0.03);
	TestTrue(TEXT("Free-body energy drift stays bounded over the test"), MaxEnergyError < 0.03);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleChassisWheelCycleTest,
	"KinetiForge.Chassis.Momentum.SteerAccelerateBrake",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleChassisWheelCycleTest::RunTest(const FString& Parameters)
{
	using namespace VehicleChassisMomentumTests;
	// Negative control: the 5.3 engine without the coordinator correction must reproduce
	// the reported leftover rotation. Other engine versions may provide native gyros.
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION == 3
	const bool Modes[] = { false, true };
#else
	const bool Modes[] = { true };
#endif
	for (const bool bCorrection : Modes)
	for (const bool bCenterBeforeBraking : { false, true })
	{
		FFixture Fixture;
		if (!Fixture.Create(*this, true)) return false;
		if (!bCorrection)
		{
			auto* Coordinator = UVehicleWheelCoordinatorComponent::FindWheelCoordinator(Fixture.Body);
			Coordinator->UnregisterComponent();
		}
		auto Host = FModuleManager::GetModuleChecked<FKinetiForgeModule>(TEXT("KinetiForge")).AcquirePhysicsScene(Fixture.World->GetPhysicsScene());
		const auto Identity = FVehiclePhysicsBodyIdentity::Capture(Fixture.Body->GetBodyInstance());
		FVehicleWheelRuntime Wheels[4];
		for (auto& Wheel : Wheels)
		{
			Wheel.Settings.WheelConfig.Inertia = 1.5f;
			Wheel.SuspensionState.bHitGround = false;
			Wheel.SuspensionState.bWheelOnGround = false;
			Wheel.SuspensionState.ImpactWorldNormal = FVector3f::UpVector;
		}
		float Time = 0.f;
		int32 Calls = 0;
		double PeakSpeed = 0.;
		const float BrakeTime = bCenterBeforeBraking ? 2.5f : 2.f;
		auto Step = Host->RegisterStep([&](float Dt, const FVehiclePhysicsExecutionContext& Context)
		{
			auto* Body = Context.ResolveBody(Identity);
			if (!Body) return;
			++Calls;
			PeakSpeed = FMath::Max(PeakSpeed, FVector(Body->W()).Size());
			const float Speed = 300.f * FMath::Min(Time, 1.f) * FMath::Clamp(1.f - (Time - BrakeTime), 0.f, 1.f);
			const float Steer = bCenterBeforeBraking ? -0.6f * (1.f - FMath::Clamp((Time - 2.f) * 2.f, 0.f, 1.f)) : -0.6f;
			const FTransform Transform(Body->R(), Body->X());
			for (int32 Index = 0; Index < 4; ++Index)
			{
				auto& Wheel = Wheels[Index];
				const FVector LocalAxis = Index < 2 ? FVector(FMath::Sin(Steer), FMath::Cos(Steer), 0.) : FVector::RightVector;
				Wheel.SuspensionState.WheelWorldRightVector = FVector3f(Transform.TransformVectorNoScale(LocalAxis));
				Wheel.PrepareWheel(Dt, Transform);
				// Prescribe spin to isolate the actual wheel/chassis exchange from engine/clutch controls.
				Wheel.WheelState.AngularVelocity = Speed;
				Wheel.FinalizeWheel();
				FVehicleWheelPhysics::ApplyWheelForce(Wheel.WheelState, Wheel.WheelContext, Wheel.SuspensionState,
					FVehicleSuspensionHitResult(), EVehicleForceApplicationPoint::ImpactPoint, Transform, Body);
			}
			Time += Dt;
		});
		ON_SCOPE_EXIT { Host->UnregisterStep(Step); };
		for (int32 Index = 0; Index < 540; ++Index) Fixture.Advance(1.f / 120.f);
		auto* Body = UVehicleUtilities::GetInternalHandle(Fixture.Body);
		const double Residual = Velocity(*Body).Size();
		AddInfo(FString::Printf(TEXT("Correction=%d center before braking=%d: calls=%d time=%g peak=%g residual=%g rad/s"), bCorrection, bCenterBeforeBraking, Calls, Time, PeakSpeed, Residual));
		TestTrue(TEXT("Physics callback ran through the complete cycle"), Calls >= 500 && Time > 4.f && PeakSpeed > 0.5);
		TestTrue(TEXT("All four wheels reached zero speed"), Wheels[0].WheelState.AngularVelocity == 0.f && Wheels[3].WheelState.AngularVelocity == 0.f);
		if (bCorrection) TestTrue(TEXT("Steer/spin/brake cycle leaves no significant chassis rotation"), Residual < 0.005);
		else TestTrue(TEXT("Negative control reproduces the residual rotation without chassis correction"), Residual > 0.05);
	}
	return true;
}

#endif
