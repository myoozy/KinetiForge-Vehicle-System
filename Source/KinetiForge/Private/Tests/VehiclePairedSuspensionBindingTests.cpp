// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "VehicleAxleAssemblyComponent.h"
#include "VehicleAxleBridge.h"
#include "VehicleWheelBridge.h"
#include "VehicleWheelComponent.h"
#include "VehicleUtilities.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "PBDRigidsSolver.h"
#include "Misc/AutomationTest.h"

namespace VehiclePairedSuspensionBindingTest
{
	struct FWorldOwner
	{
		UWorld* World = nullptr;
		~FWorldOwner() { if (World) World->DestroyWorld(false); }
	};

	struct FVehicle
	{
		AActor* Actor = nullptr;
		UBoxComponent* Body = nullptr;
		UVehicleAxleAssemblyComponent* Axle = nullptr;
		UVehicleWheelComponent* Left = nullptr;
		UVehicleWheelComponent* Right = nullptr;
	};

	UWorld* CreateWorld(FAutomationTestBase& Test, FWorldOwner& Owner)
	{
		Owner.World = UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr,
			true, ERHIFeatureLevel::Num, nullptr, true);
		if (!Test.TestNotNull(TEXT("Paired suspension world"), Owner.World)) return nullptr;
		Owner.World->InitWorld(UWorld::InitializationValues().AllowAudioPlayback(false)
			.CreatePhysicsScene(true).RequiresHitProxies(false).CreateNavigation(false)
			.CreateAISystem(false).ShouldSimulatePhysics(false).SetTransactional(false));
		if (!Test.TestNotNull(TEXT("Paired suspension physics scene"), Owner.World->GetPhysicsScene())) return nullptr;
		Owner.World->GetPhysicsScene()->GetSolver()->DisableAsyncMode();
		Owner.World->GetPhysicsScene()->GetSolver()->SetThreadingMode_External(Chaos::EThreadingModeTemp::SingleThread);
		return Owner.World;
	}

	void FlushPhysics(UWorld& World)
	{
		const FVector Gravity = FVector::ZeroVector;
		World.GetPhysicsScene()->SetUpForFrame(&Gravity, 1.f / 60.f, 0.f, 1.f / 60.f, 1.f / 60.f, 1, false);
		World.GetPhysicsScene()->StartFrame();
		World.GetPhysicsScene()->EndFrame();
		World.GetPhysicsScene()->WaitPhysScenes();
	}

	void AddGround(UWorld& World)
	{
		AActor* GroundActor = World.SpawnActor<AActor>(FVector(500.f, 0.f, -20.f), FRotator::ZeroRotator);
		if (!GroundActor) return;
		UBoxComponent* Ground = NewObject<UBoxComponent>(GroundActor, TEXT("PairedSuspensionGround"));
		GroundActor->AddInstanceComponent(Ground);
		GroundActor->SetRootComponent(Ground);
		Ground->SetBoxExtent(FVector(3000.f, 300.f, 20.f));
		Ground->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Ground->SetCollisionObjectType(ECC_WorldStatic);
		Ground->SetCollisionResponseToAllChannels(ECR_Block);
		Ground->SetMobility(EComponentMobility::Static);
		Ground->RegisterComponent();
		Ground->SetWorldLocation(FVector(500.f, 0.f, -20.f), false, nullptr, ETeleportType::TeleportPhysics);
		Ground->RecreatePhysicsState();
	}

	FVehicle CreateVehicle(UWorld& World, const FVector& Location, bool bCallAxleBind, bool bCallPartialWheelBind = false)
	{
		FVehicle Vehicle;
		Vehicle.Actor = World.SpawnActor<AActor>(Location, FRotator::ZeroRotator);
		if (!Vehicle.Actor) return Vehicle;
		Vehicle.Body = NewObject<UBoxComponent>(Vehicle.Actor, TEXT("PairedSuspensionBody"));
		Vehicle.Actor->AddInstanceComponent(Vehicle.Body);
		Vehicle.Actor->SetRootComponent(Vehicle.Body);
		Vehicle.Body->SetBoxExtent(FVector(50.f));
		Vehicle.Body->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Vehicle.Body->SetCollisionObjectType(ECC_PhysicsBody);
		Vehicle.Body->SetCollisionResponseToAllChannels(ECR_Block);
		Vehicle.Body->SetSimulatePhysics(true);
		Vehicle.Body->SetEnableGravity(false);
		Vehicle.Body->RegisterComponent();
		Vehicle.Body->RecreatePhysicsState();
		Vehicle.Body->SetWorldLocation(Location, false, nullptr, ETeleportType::TeleportPhysics);

		Vehicle.Axle = NewObject<UVehicleAxleAssemblyComponent>(Vehicle.Actor, TEXT("PairedSuspensionAxle"));
		Vehicle.Actor->AddInstanceComponent(Vehicle.Axle);
		Vehicle.Axle->SetupAttachment(Vehicle.Body);
		Vehicle.Axle->SetRelativeLocation(FVector(0.f, 0.f, -55.f));
		FVehicleAxleConfig AxleConfig = Vehicle.Axle->GetAxleConfig();
		AxleConfig.SwaybarStiffness = 5000.f;
		Vehicle.Axle->SetAxleConfig(AxleConfig);
		Vehicle.Axle->RegisterComponent();
		Vehicle.Axle->GetWheels(Vehicle.Left, Vehicle.Right);
		if (!Vehicle.Left || !Vehicle.Right) return Vehicle;

		for (UVehicleWheelComponent* Wheel : { Vehicle.Left, Vehicle.Right })
		{
			FVehicleWheelConfig WheelConfig = Wheel->GetWheelConfig();
			WheelConfig.Radius = 33.f;
			WheelConfig.Width = 20.f;
			WheelConfig.Inertia = 1.3f;
			Wheel->SetWheelConfig(WheelConfig);
			FVehicleSuspensionKinematicsConfig Kinematics = Wheel->GetSuspensionKinematicsConfig();
			Kinematics.SuspensionType = EVehicleIndependentSuspensionType::StraightLine;
			Kinematics.TraceChannel = ECC_WorldStatic;
			Kinematics.RayCastMode = EVehicleSuspensionRayCastMode::SphereTrace;
			Wheel->SetSuspensionKinematicsConfig(Kinematics);
			Wheel->SetSprungMass(300.f);
		}
		// An asymmetric wheel mount exercises the coupled coordinates and final swaybar feedback.
		Vehicle.Left->SetRelativeLocation(Vehicle.Left->GetRelativeLocation() + FVector(0.f, 0.f, 14.f));
		Vehicle.Left->InitializeWheel();
		if (bCallAxleBind) Vehicle.Axle->BindIsolatedAxleRuntime();
		else if (bCallPartialWheelBind) Vehicle.Left->BindIsolatedWheelRuntime();
		return Vehicle;
	}

	const FVehicleSuspensionSimState& GetSuspensionState(const FVehicle& Vehicle, UVehicleWheelComponent* Wheel)
	{
		if (TSharedPtr<FVehicleWheelBridge, ESPMode::ThreadSafe> Bridge = Wheel->GetWheelBridge())
			return Bridge->GetRuntimeForSimulation().SuspensionState;
		return Wheel->GetSuspensionState();
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehiclePairedSuspensionAxleBindingTest,
	"KinetiForge.Suspension.Paired.RegisteredAxleBinding",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehiclePairedSuspensionAxleBindingTest::RunTest(const FString& Parameters)
{
	using namespace VehiclePairedSuspensionBindingTest;
	(void)Parameters;
	FWorldOwner WorldOwner;
	UWorld* World = CreateWorld(*this, WorldOwner);
	if (!World) return false;
	AddGround(*World);
	FVehicle Legacy = CreateVehicle(*World, FVector(0.f, 0.f, 80.f), false);
	FVehicle Isolated = CreateVehicle(*World, FVector(1000.f, 0.f, 80.f), true);
	FVehicle Mixed = CreateVehicle(*World, FVector(2000.f, 0.f, 80.f), false, true);
	if (!TestNotNull(TEXT("Legacy axle"), Legacy.Axle) || !TestNotNull(TEXT("Legacy left wheel"), Legacy.Left)
		|| !TestNotNull(TEXT("Legacy right wheel"), Legacy.Right) || !TestNotNull(TEXT("Isolated axle"), Isolated.Axle)
		|| !TestNotNull(TEXT("Isolated left wheel"), Isolated.Left) || !TestNotNull(TEXT("Isolated right wheel"), Isolated.Right)
		|| !TestNotNull(TEXT("Mixed axle"), Mixed.Axle) || !TestNotNull(TEXT("Mixed left wheel"), Mixed.Left)
		|| !TestNotNull(TEXT("Mixed right wheel"), Mixed.Right)) return false;
	FlushPhysics(*World);
	Chaos::FRigidBodyHandle_Internal* LegacyHandle = UVehicleUtilities::GetInternalHandle(Legacy.Body);
	Chaos::FRigidBodyHandle_Internal* IsolatedHandle = UVehicleUtilities::GetInternalHandle(Isolated.Body);
	if (!TestNotNull(TEXT("Legacy body handle"), LegacyHandle) || !TestNotNull(TEXT("Isolated body handle"), IsolatedHandle)) return false;
	TestTrue(TEXT("Isolated axle owns both bound wheel bridges"),
		Isolated.Axle->GetAxleBridge().IsValid() && Isolated.Left->GetWheelBridge().IsValid()
		&& Isolated.Right->GetWheelBridge().IsValid());
	TestTrue(TEXT("Axle bridges exist with or without explicit bind calls"), Legacy.Axle->GetAxleBridge().IsValid()
		&& Isolated.Axle->GetAxleBridge().IsValid() && Mixed.Axle->GetAxleBridge().IsValid());
	TestTrue(TEXT("All wheels share axle-owned bridges after partial bind attempts"), Legacy.Left->GetWheelBridge().IsValid()
		&& Legacy.Right->GetWheelBridge().IsValid() && Isolated.Left->GetWheelBridge().IsValid()
		&& Isolated.Right->GetWheelBridge().IsValid() && Mixed.Left->GetWheelBridge().IsValid()
		&& Mixed.Right->GetWheelBridge().IsValid());
	TestTrue(TEXT("Repeated axle and wheel bind calls are idempotent"), Mixed.Axle->BindIsolatedAxleRuntime()
		&& Mixed.Left->BindIsolatedWheelRuntime());

	int32 LegacyContacts = 0;
	int32 IsolatedContacts = 0;
	for (int32 Step = 0; Step < 4; ++Step)
	{
		float LegacyOutI = 0.f, LegacyOutOmega = 0.f, LegacyOutK = 0.f;
		float IsolatedOutI = 0.f, IsolatedOutOmega = 0.f, IsolatedOutK = 0.f;
		{
#if PHYSICS_THREAD_CONTEXT
			Chaos::FPhysicsThreadContextScope PhysicsContext(true);
#endif
			Legacy.Axle->PreStepAxle(1.f / 60.f, 0.f);
			Isolated.Axle->PreStepAxle(1.f / 60.f, 0.f);
			Mixed.Axle->PreStepAxle(1.f / 60.f, 0.f);
			Legacy.Axle->SubstepAxle(1.f / 60.f, 100.f, 0.f, 0.f, LegacyOutI, LegacyOutOmega, LegacyOutK);
			Isolated.Axle->SubstepAxle(1.f / 60.f, 100.f, 0.f, 0.f, IsolatedOutI, IsolatedOutOmega, IsolatedOutK);
			Mixed.Axle->SubstepAxle(1.f / 60.f, 100.f, 0.f, 0.f, IsolatedOutI, IsolatedOutOmega, IsolatedOutK);
			Legacy.Axle->PostStepAxle();
			Isolated.Axle->PostStepAxle();
			Mixed.Axle->PostStepAxle();
		}
		const FVehicleSuspensionSimState& LegacyLeft = GetSuspensionState(Legacy, Legacy.Left);
		const FVehicleSuspensionSimState& LegacyRight = GetSuspensionState(Legacy, Legacy.Right);
		const FVehicleSuspensionSimState& IsolatedLeft = GetSuspensionState(Isolated, Isolated.Left);
		const FVehicleSuspensionSimState& IsolatedRight = GetSuspensionState(Isolated, Isolated.Right);
		const FVehicleSuspensionSimState& MixedLeft = GetSuspensionState(Mixed, Mixed.Left);
		const FVehicleSuspensionSimState& MixedRight = GetSuspensionState(Mixed, Mixed.Right);
		LegacyContacts += LegacyLeft.bHitGround && LegacyRight.bHitGround ? 1 : 0;
		IsolatedContacts += IsolatedLeft.bHitGround && IsolatedRight.bHitGround ? 1 : 0;
		TestTrue(FString::Printf(TEXT("Step %d both paths retain a two-wheel axle"), Step),
			Legacy.Axle->GetAxleBridge()->GetRuntimeForSimulation().State.NumOfWheels == 2
			&& Isolated.Axle->GetAxleBridge()->GetRuntimeForSimulation().State.NumOfWheels == 2);
		TestTrue(FString::Printf(TEXT("Step %d compatibility suspension outputs agree"), Step),
			LegacyLeft.HubChassisLocation.Equals(IsolatedLeft.HubChassisLocation, 1.e-2f)
			&& LegacyRight.HubChassisLocation.Equals(IsolatedRight.HubChassisLocation, 1.e-2f)
			&& FMath::IsNearlyEqual(LegacyLeft.ForceAlongImpactNormal, IsolatedLeft.ForceAlongImpactNormal, 1.e-1f)
			&& FMath::IsNearlyEqual(LegacyRight.ForceAlongImpactNormal, IsolatedRight.ForceAlongImpactNormal, 1.e-1f));
		TestTrue(FString::Printf(TEXT("Step %d compatibility output agrees after partial bind call"), Step),
			FMath::IsNearlyEqual(LegacyLeft.HubChassisLocation.Z, MixedLeft.HubChassisLocation.Z, 1.e-2f)
			&& FMath::IsNearlyEqual(LegacyRight.HubChassisLocation.Z, MixedRight.HubChassisLocation.Z, 1.e-2f)
			&& FMath::IsNearlyEqual(LegacyLeft.ForceAlongImpactNormal, MixedLeft.ForceAlongImpactNormal, 1.e-1f)
			&& FMath::IsNearlyEqual(LegacyRight.ForceAlongImpactNormal, MixedRight.ForceAlongImpactNormal, 1.e-1f));
		TestTrue(FString::Printf(TEXT("Step %d asymmetric mount produces nonzero suspension force"), Step),
			!FMath::IsNearlyZero(LegacyLeft.ForceAlongImpactNormal, 1.e-2f)
			&& !FMath::IsNearlyZero(LegacyRight.ForceAlongImpactNormal, 1.e-2f));
		Isolated.Axle->RefreshPublishedAxleState();
		Isolated.Left->RefreshPublishedWheelState();
		Isolated.Right->RefreshPublishedWheelState();
		Mixed.Left->RefreshPublishedWheelState();
	}
	TestTrue(TEXT("Legacy axle reached real two-wheel ground contact"), LegacyContacts > 0);
	TestTrue(TEXT("Isolated axle reached real two-wheel ground contact"), IsolatedContacts > 0);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
