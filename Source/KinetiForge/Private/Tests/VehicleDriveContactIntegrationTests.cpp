// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "VehicleDriveAssemblyComponent.h"
#include "VehicleFrozenBaseline.h"
#include "VehicleAxleAssemblyComponent.h"
#include "VehicleWheelComponent.h"
#include "VehicleEngineComponent.h"
#include "VehicleGearboxComponent.h"
#include "VehiclePhysicsBodyBinding.h"
#include "VehiclePhysicsScene.h"
#include "VehiclePhysicsStep.h"
#include "VehicleUtilities.h"
#include "KinetiForge.h"
#include "Components/BoxComponent.h"
#include "Chaos/ParticleHandle.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "Misc/AutomationTest.h"
#include "Misc/ScopeExit.h"
#include "Modules/ModuleManager.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "PBDRigidsSolver.h"

struct FVehicleDriveContactIntegrationProbe
{
	static TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe> Step(UVehicleDriveAssemblyComponent& Drive)
	{
		return Drive.GetPhysicsStep();
	}
};

namespace VehicleDriveContactIntegration
{
	struct FWorldOwner
	{
		UWorld* World = nullptr;
		~FWorldOwner() { if (World) World->DestroyWorld(false); }
	};

	UWorld* CreateWorld(FAutomationTestBase& Test, FWorldOwner& Owner)
	{
		Owner.World = UWorld::CreateWorld(EWorldType::Game, false, TEXT("VehicleDriveContactIntegration"),
			nullptr, true, ERHIFeatureLevel::Num, nullptr, true);
		if (!Test.TestNotNull(TEXT("Contact integration world created"), Owner.World)) return nullptr;
		Owner.World->InitWorld(UWorld::InitializationValues().AllowAudioPlayback(false).CreatePhysicsScene(true)
			.RequiresHitProxies(false).CreateNavigation(false).CreateAISystem(false).ShouldSimulatePhysics(false).SetTransactional(false));
		if (!Test.TestNotNull(TEXT("Contact integration Chaos scene created"), Owner.World->GetPhysicsScene())) return nullptr;
		Owner.World->GetPhysicsScene()->GetSolver()->DisableAsyncMode();
		Owner.World->GetPhysicsScene()->GetSolver()->SetThreadingMode_External(Chaos::EThreadingModeTemp::SingleThread);
		return Owner.World;
	}

	void Flush(UWorld& World)
	{
		const FVector Gravity(0.f, 0.f, -980.f);
		World.GetPhysicsScene()->SetUpForFrame(&Gravity, 1.f / 60.f, 0.f, 1.f / 60.f, 1.f / 60.f, 1, false);
		World.GetPhysicsScene()->StartFrame();
		World.GetPhysicsScene()->EndFrame();
		World.GetPhysicsScene()->WaitPhysScenes();
	}

	UBoxComponent* AddBox(UWorld& World, const TCHAR* Name, const FVector& Location,
		const FVector& Extent, const bool bDynamic)
	{
		AActor* Actor = World.SpawnActor<AActor>(Location, FRotator::ZeroRotator);
		if (!Actor) return nullptr;
		UBoxComponent* Box = NewObject<UBoxComponent>(Actor, Name);
		Actor->AddInstanceComponent(Box);
		Actor->SetRootComponent(Box);
		Box->SetBoxExtent(Extent);
		Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Box->SetCollisionObjectType(bDynamic ? ECC_PhysicsBody : ECC_WorldStatic);
		Box->SetCollisionResponseToAllChannels(ECR_Block);
		Box->SetMobility(bDynamic ? EComponentMobility::Movable : EComponentMobility::Static);
		Box->SetSimulatePhysics(bDynamic);
		Box->SetEnableGravity(false);
		if (bDynamic) Box->SetMassOverrideInKg(NAME_None, 10000.f, true);
		Box->RegisterComponent();
		Box->RecreatePhysicsState();
		Box->SetWorldLocation(Location, false, nullptr, ETeleportType::TeleportPhysics);
		return Box;
	}

	struct FVehicle
	{
		APawn* Actor = nullptr;
		UBoxComponent* Chassis = nullptr;
		UVehicleAxleAssemblyComponent* Axle = nullptr;
		UVehicleDriveAssemblyComponent* Drive = nullptr;
	};

	FVehicle CreateVehicle(UWorld& World, const FVector& Location)
	{
		FVehicle Vehicle;
		Vehicle.Actor = World.SpawnActor<APawn>(Location, FRotator::ZeroRotator);
		if (!Vehicle.Actor) return Vehicle;

		Vehicle.Chassis = NewObject<UBoxComponent>(Vehicle.Actor, TEXT("ContactChassis"));
		Vehicle.Actor->AddInstanceComponent(Vehicle.Chassis);
		Vehicle.Actor->SetRootComponent(Vehicle.Chassis);
		Vehicle.Chassis->SetBoxExtent(FVector(50.f));
		Vehicle.Chassis->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Vehicle.Chassis->SetCollisionObjectType(ECC_PhysicsBody);
		Vehicle.Chassis->SetCollisionResponseToAllChannels(ECR_Block);
		Vehicle.Chassis->SetSimulatePhysics(true);
		Vehicle.Chassis->SetEnableGravity(true);
		Vehicle.Chassis->SetMassOverrideInKg(NAME_None, 600.f, true);
		Vehicle.Chassis->RegisterComponent();
		Vehicle.Chassis->RecreatePhysicsState();
		Vehicle.Chassis->SetWorldLocation(Location, false, nullptr, ETeleportType::TeleportPhysics);

		Vehicle.Axle = NewObject<UVehicleAxleAssemblyComponent>(Vehicle.Actor, TEXT("ContactAxle"));
		Vehicle.Actor->AddInstanceComponent(Vehicle.Axle);
		Vehicle.Axle->SetupAttachment(Vehicle.Chassis);
		Vehicle.Axle->SetRelativeLocation(FVector(0.f, 0.f, -55.f));
		FVehicleAxleConfig AxleConfig = Vehicle.Axle->GetAxleConfig();
		AxleConfig.SwaybarStiffness = 14000.f;
		Vehicle.Axle->SetAxleConfig(AxleConfig);
		Vehicle.Axle->SetAxleSuspensionType(EVehicleAxleSuspensionType::Independent);
		Vehicle.Axle->RegisterComponent();
		UVehicleWheelComponent* LeftWheel = nullptr;
		UVehicleWheelComponent* RightWheel = nullptr;
		Vehicle.Axle->GetWheels(LeftWheel, RightWheel);
		for (UVehicleWheelComponent* Wheel : {LeftWheel, RightWheel})
		{
			if (!Wheel) continue;
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

		Vehicle.Drive = NewObject<UVehicleDriveAssemblyComponent>(Vehicle.Actor, TEXT("ContactDrive"));
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

	void Prepare(FVehicle& Vehicle)
	{
		Vehicle.Actor->DispatchBeginPlay();
		// The power bridge is deferred until BeginPlay; the second GT refresh publishes the bound entry.
		Vehicle.Drive->TickComponent(1.f / 60.f, LEVELTICK_All, nullptr);
		Vehicle.Drive->TickComponent(1.f / 60.f, LEVELTICK_All, nullptr);
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

	TSharedPtr<FVehiclePhysicsScene, ESPMode::ThreadSafe> GetScene(UWorld& World)
	{
		FKinetiForgeModule& Module = FModuleManager::Get().GetModuleChecked<FKinetiForgeModule>(TEXT("KinetiForge"));
		return Module.AcquirePhysicsScene(World.GetPhysicsScene());
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleDriveGroundContactTest,
	"KinetiForge.Drivetrain.FixedDrive.StaticContactFrozenBaseline",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleDriveGroundContactTest::RunTest(const FString& Parameters)
{
	using namespace VehicleDriveContactIntegration;
	FVehicleFrozenBaseline Baseline;
	if (!Baseline.Load(*this, TEXT("static-contact.csv"), 24, 20)) return false;
	FWorldOwner Owner;
	UWorld* World = CreateWorld(*this, Owner);
	if (!World) return false;
	if (!TestNotNull(TEXT("Static floor created"), AddBox(*World, TEXT("ContactFloor"),
		FVector(250.f, 0.f, -20.f), FVector(1000.f, 300.f, 20.f), false))) return false;
	// Raise only the left wheel track to exercise independent suspension and anti-roll coupling.
	if (!TestNotNull(TEXT("Raised left road strip created"), AddBox(*World, TEXT("RaisedStrip"),
		FVector(0.f, -80.f, 3.f), FVector(500.f, 25.f, 3.f), false))) return false;
	FVehicle Vehicle = CreateVehicle(*World, FVector(0.f, 0.f, 80.f));
	if (!TestTrue(TEXT("Complete contact vehicle registered"), Vehicle.Drive && Vehicle.Axle && Vehicle.Chassis)) return false;
	Flush(*World);
	Prepare(Vehicle);
	const auto Entry = FVehicleDriveContactIntegrationProbe::Step(*Vehicle.Drive);
	if (!TestTrue(TEXT("Contact runtime entry is active"), Entry && !Entry->IsStopped())) return false;
	if (!TestEqual(TEXT("Contact fixture has two wheels"), Vehicle.Drive->GetWheels().Num(), 2)) return false;
	int32 GroundSamples = 0;
	for (int32 Frame = 0; Frame < 24; ++Frame)
	{
		Vehicle.Drive->InputThrottle(0.35f, Frame == 0);
		Vehicle.Drive->InputBrake(Frame > 15 ? 0.1f : 0.f);
		Flush(*World);
		RefreshPublishedState(Vehicle);
		const auto Wheels = Vehicle.Drive->GetWheels();
		const auto& Left = Wheels[0]->GetSuspensionState();
		const auto& Right = Wheels[1]->GetSuspensionState();
		GroundSamples += int32(Left.bHitGround) + int32(Right.bHitGround);
		Baseline.CheckRow(*this, Frame, {
			Vehicle.Drive->GetEngine()->GetRPM(), Vehicle.Axle->GetAngularVelocity(),
			Vehicle.Chassis->GetComponentLocation().Z, Vehicle.Chassis->GetPhysicsLinearVelocity().Z,
			double(Left.bHitGround), Left.ForceAlongImpactNormal, Left.HubChassisLocation.X,
			Left.HubChassisLocation.Y, Left.HubChassisLocation.Z, Left.StrutCurrentLength,
			Wheels[0]->GetAngularVelocity(), Left.ImpactWorldLocation.Z,
			double(Right.bHitGround), Right.ForceAlongImpactNormal, Right.HubChassisLocation.X,
			Right.HubChassisLocation.Y, Right.HubChassisLocation.Z, Right.StrutCurrentLength,
			Wheels[1]->GetAngularVelocity(), Right.ImpactWorldLocation.Z},
			{0.02,0.02,0.15,0.15,0,1,0.05,0.05,0.05,0.05,0.05,0.05,0,1,0.05,0.05,0.05,0.05,0.05,0.05});
		if (Frame < 2)
		{
			TestTrue(TEXT("Both wheels have startup contact"), Left.bHitGround && Right.bHitGround);
			TestTrue(TEXT("Both wheels have startup load"), FMath::Abs(Left.ForceAlongImpactNormal) > 1.f
				&& FMath::Abs(Right.ForceAlongImpactNormal) > 1.f);
		}
		if (Frame == 0)
			TestTrue(TEXT("Left contact sees the raised surface"), Left.ImpactWorldLocation.Z > Right.ImpactWorldLocation.Z + 2.f);
	}
	TestTrue(TEXT("Ground-to-air run included both-wheel contact"), GroundSamples >= 4);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleDriveDynamicContactReactionTest,
	"KinetiForge.Drivetrain.FixedDrive.DynamicContactReactionSamePhase",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleDriveDynamicContactReactionTest::RunTest(const FString& Parameters)
{
	using namespace VehicleDriveContactIntegration;
	FVehicleFrozenBaseline Baseline;
	if (!Baseline.Load(*this, TEXT("dynamic-contact.csv"), 12, 11)) return false;
	FWorldOwner Owner;
	UWorld* World = CreateWorld(*this, Owner);
	if (!World) return false;
	FVehicle Vehicle = CreateVehicle(*World, FVector(0.f, 0.f, 80.f));
	UBoxComponent* Platform = AddBox(*World, TEXT("DynamicPlatform"), FVector(0.f, 0.f, -20.f), FVector(140.f, 120.f, 20.f), true);
	if (!TestTrue(TEXT("Complete vehicle and dynamic platform created"), Vehicle.Drive && Vehicle.Axle && Vehicle.Chassis && Platform)) return false;
	Platform->SetPhysicsLinearVelocity(FVector(0.f, 35.f, 0.f));
	Flush(*World);
	Prepare(Vehicle);
	if (!TestEqual(TEXT("Dynamic fixture has two wheels"), Vehicle.Drive->GetWheels().Num(), 2)) return false;
	const auto Entry = FVehicleDriveContactIntegrationProbe::Step(*Vehicle.Drive);
	if (!TestTrue(TEXT("Dynamic-contact runtime entry is active"), Entry && !Entry->IsStopped())) return false;
	const auto Scene = GetScene(*World);
	if (!TestNotNull(TEXT("Dynamic contact scene host"), Scene.Get())) return false;
	const auto PlatformIdentity = FVehiclePhysicsBodyIdentity::Capture(Platform->GetBodyInstance());
	if (!TestTrue(TEXT("Platform body identity captured"), PlatformIdentity.IsValid())) return false;
	TArray<FVector> PhaseImpulses;
	PhaseImpulses.Reserve(12);
	const auto Observer = Scene->RegisterStep([&PhaseImpulses, PlatformIdentity]
		(float, const FVehiclePhysicsExecutionContext& Context)
	{
		const auto* Handle = Context.ResolveBody(PlatformIdentity);
		PhaseImpulses.Add(Handle ? FVector(Handle->LinearImpulse()) : FVector::ZeroVector);
	});
	if (!TestNotNull(TEXT("Same-phase observer registered after vehicle"), Observer.Get())) return false;
	ON_SCOPE_EXIT { Scene->UnregisterStep(Observer); };
	int32 ContactSamples = 0;
	float PlatformDeltaY = 0.f;
	bool bSawLoadedReaction = false;
	bool bComparedLoadedReaction = false;
	for (int32 Frame = 0; Frame < 12; ++Frame)
	{
		Vehicle.Drive->InputThrottle(0.45f, Frame == 0);
		const float PreviousY = Platform->GetPhysicsLinearVelocity().Y;
		Flush(*World);
		RefreshPublishedState(Vehicle);
		PlatformDeltaY += Platform->GetPhysicsLinearVelocity().Y - PreviousY;
		const auto Wheels = Vehicle.Drive->GetWheels();
		const auto& Left = Wheels[0]->GetSuspensionState();
		const auto& Right = Wheels[1]->GetSuspensionState();
		const int32 Contacts = int32(Left.bHitGround) + int32(Right.bHitGround);
		const float Load = Left.ForceAlongImpactNormal + Right.ForceAlongImpactNormal;
		ContactSamples += Contacts;
		TestEqual(TEXT("Same-phase observer ran once per step"), PhaseImpulses.Num(), Frame + 1);
		if (!PhaseImpulses.IsValidIndex(Frame)) return false;
		TestTrue(TEXT("Left load matches frozen reference"), FMath::Abs(Left.ForceAlongImpactNormal - Baseline.Get(Frame,0)) <= 1.0);
		TestTrue(TEXT("Right load matches frozen reference"), FMath::Abs(Right.ForceAlongImpactNormal - Baseline.Get(Frame,1)) <= 1.0);
		if (Contacts > 0 && Load > 1.f)
			bSawLoadedReaction |= PhaseImpulses[Frame].Size() > 1.f;
		const FVector ExpectedImpulse(Baseline.Get(Frame,4), Baseline.Get(Frame,5), Baseline.Get(Frame,6));
		if (Contacts > 0 && Load > 1.f && Baseline.Get(Frame,2) > 0 && Baseline.Get(Frame,3) > 1.0
			&& PhaseImpulses[Frame].Size() > 1.f && ExpectedImpulse.Size() > 1.f)
		{
			bComparedLoadedReaction = true;
			TestTrue(TEXT("Loaded reaction matches frozen same-phase impulse"),
				PhaseImpulses[Frame].Equals(ExpectedImpulse, FMath::Max(1.0, ExpectedImpulse.Size() * 0.1)));
		}
	}
	TestTrue(TEXT("Vehicle had dynamic-platform contacts"), ContactSamples >= 8);
	TestTrue(TEXT("Loaded contacts produced same-phase reaction"), bSawLoadedReaction);
	TestTrue(TEXT("At least one loaded reaction was compared with frozen data"), bComparedLoadedReaction);
	TestTrue(TEXT("Platform velocity changed under contact"), FMath::Abs(PlatformDeltaY) > 0.01f);
	const double ExpectedDeltaY = Baseline.Get(11,7);
	TestTrue(TEXT("Accumulated platform response matches frozen reference"),
		FMath::Abs(PlatformDeltaY - ExpectedDeltaY) <= FMath::Max(0.1, FMath::Abs(ExpectedDeltaY) * 0.1));
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
