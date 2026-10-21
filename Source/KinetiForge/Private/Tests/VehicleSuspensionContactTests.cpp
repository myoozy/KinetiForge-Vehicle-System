// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "VehicleSuspensionSolver.h"
#include "VehicleWheelComponent.h"
#include "VehicleWheelBridge.h"
#include "VehicleUtilities.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "HAL/PlatformFileManager.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "PBDRigidsSolver.h"
#include "Misc/AutomationTest.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "UObject/StrongObjectPtr.h"

struct FVehicleSuspensionContactLegacyProbe
{
	static void RayCast(FVehicleSuspensionSolver& Solver, FVehicleSuspensionSimContext& Context,
		const UWorld* World, const float WheelRadius, const float HalfWheelWidth,
		const float WheelInertia, const FCollisionQueryParams& QueryParams,
		const FCollisionResponseParams& ResponseParams, const FVehicleSuspensionKinematicsConfig& Config)
	{
		Solver.SuspensionRayCast(Context, World, WheelRadius, HalfWheelWidth, WheelInertia,
			QueryParams, ResponseParams, Config);
	}

	static void CalculateImpactVelocity(FVehicleSuspensionSimContext& Context,
		const FVehicleChassisSimState& ChassisState)
	{
		FVehicleSuspensionSolver::CalculateImpactPointWorldVelocity(Context, ChassisState);
	}
};

struct FVehicleWheelContactProbe
{
	static void ApplyWheelForce(UVehicleWheelComponent& Wheel, Chaos::FRigidBodyHandle_Internal* ChassisHandle)
	{
		Wheel.ApplyWheelForce(ChassisHandle);
	}

	static FVehicleSuspensionSimState& SuspensionState(UVehicleWheelComponent& Wheel) { return Wheel.GetWheelBridge()->GetRuntimeForSimulation().SuspensionState; }
	static FVehicleWheelSimState& WheelState(UVehicleWheelComponent& Wheel) { return Wheel.GetWheelBridge()->GetRuntimeForSimulation().WheelState; }
	static FVehicleWheelSimContext& WheelContext(UVehicleWheelComponent& Wheel) { return Wheel.GetWheelBridge()->GetRuntimeForSimulation().WheelContext; }
	static FVehicleSuspensionHitResult& HitResult(UVehicleWheelComponent& Wheel) { return Wheel.GetWheelBridge()->GetContactForSimulation(); }
	static FTransform& ChassisTransform(UVehicleWheelComponent& Wheel) { return Wheel.GetWheelBridge()->GetFrameForSimulation().ChassisWorldTransform; }
	static FVehicleSuspensionKinematicsConfig& KinematicsConfig(UVehicleWheelComponent& Wheel) { return Wheel.GetWheelBridge()->GetRuntimeForSimulation().Settings.KinematicsConfig; }
};

namespace VehicleSuspensionContactTest
{
	struct FWorldOwner
	{
		UWorld* World = nullptr;
		~FWorldOwner() { if (World) World->DestroyWorld(false); }
	};

	UWorld* CreateWorld(FAutomationTestBase& Test, FWorldOwner& Owner)
	{
		Owner.World = UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true, ERHIFeatureLevel::Num, nullptr, true);
		if (!Test.TestNotNull(TEXT("Contact test world created"), Owner.World)) return nullptr;
		Owner.World->InitWorld(UWorld::InitializationValues()
			.AllowAudioPlayback(false).CreatePhysicsScene(true).RequiresHitProxies(false)
			.CreateNavigation(false).CreateAISystem(false).ShouldSimulatePhysics(false).SetTransactional(false));
		if (!Test.TestNotNull(TEXT("Contact test physics scene created"), Owner.World->GetPhysicsScene())) return nullptr;
		Owner.World->GetPhysicsScene()->GetSolver()->DisableAsyncMode();
		Owner.World->GetPhysicsScene()->GetSolver()->SetThreadingMode_External(Chaos::EThreadingModeTemp::SingleThread);
		return Owner.World;
	}

	UBoxComponent* AddBox(UWorld& World, const TCHAR* Name, const FVector& Location,
		const FVector& Extent, const bool bSimulate, UPhysicalMaterial* PhysMaterial = nullptr)
	{
		AActor* Actor = World.SpawnActor<AActor>(Location, FRotator::ZeroRotator);
		if (!Actor) return nullptr;
		UBoxComponent* Box = NewObject<UBoxComponent>(Actor, Name);
		Actor->AddInstanceComponent(Box);
		Actor->SetRootComponent(Box);
		Box->SetBoxExtent(Extent);
		Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Box->SetCollisionObjectType(bSimulate ? ECC_PhysicsBody : ECC_WorldStatic);
		Box->SetCollisionResponseToAllChannels(ECR_Block);
		Box->SetSimulatePhysics(bSimulate);
		Box->SetEnableGravity(false);
		if (PhysMaterial) Box->SetPhysMaterialOverride(PhysMaterial);
		Box->RegisterComponent();
		Box->RecreatePhysicsState();
		Box->SetWorldLocation(Location);
		return Box;
	}

	void FlushPhysicsScene(UWorld& World)
	{
		const FVector Gravity = FVector::ZeroVector;
		World.GetPhysicsScene()->SetUpForFrame(&Gravity, 1.f / 60.f, 0.f, 1.f / 60.f, 1.f / 60.f, 1, false);
		World.GetPhysicsScene()->StartFrame();
		World.GetPhysicsScene()->EndFrame();
		World.GetPhysicsScene()->WaitPhysScenes();
	}

	FVehicleSuspensionSimContext MakeRayContext(const float X)
	{
		FVehicleSuspensionSimContext Context;
		Context.RayCastStartWorldLocation = FVector(X, 0.f, 100.f);
		Context.RayCastEndWorldLocation = FVector(X, 0.f, -50.f);
		Context.RayCastDirectionWorld = FVector(0.f, 0.f, -1.f);
		Context.RayCastLength = 150.f;
		Context.RayCastWorldTransform = FTransform(FQuat::Identity, FVector(X, 0.f, 0.f));
		Context.ChassisWorldTransform = FTransform::Identity;
		Context.HubChassisTransform = FTransform3f::Identity;
		return Context;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleSuspensionContactLegacyCaptureTest,
	"KinetiForge.Suspension.Contact.LegacyCapture",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleSuspensionContactLegacyCaptureTest::RunTest(const FString& Parameters)
{
	using namespace VehicleSuspensionContactTest;
	FWorldOwner WorldOwner;
	UWorld* World = CreateWorld(*this, WorldOwner);
	if (!World) return false;

	TStrongObjectPtr<UPhysicalMaterial> Material(NewObject<UPhysicalMaterial>(GetTransientPackage(), TEXT("ContactCaptureMaterial")));
	Material->Friction = 0.73f;
	UBoxComponent* StaticFloor = AddBox(*World, TEXT("StaticFloor"), FVector(0.f, 0.f, -20.f), FVector(200.f, 200.f, 20.f), false, Material.Get());
	UBoxComponent* DynamicFloor = AddBox(*World, TEXT("DynamicFloor"), FVector(1000.f, 0.f, -20.f), FVector(200.f, 200.f, 20.f), true, Material.Get());
	if (!TestNotNull(TEXT("Static query floor created"), StaticFloor)
		|| !TestNotNull(TEXT("Dynamic query floor created"), DynamicFloor)) return false;
	DynamicFloor->SetPhysicsLinearVelocity(FVector(50.f, 0.f, 0.f));
	FlushPhysicsScene(*World);

	FVehicleSuspensionSolver Solver;
	FCollisionQueryParams QueryParams = FCollisionQueryParams::DefaultQueryParam;
	QueryParams.bReturnPhysicalMaterial = true;
	QueryParams.bTraceComplex = false;
	FCollisionResponseParams ResponseParams = FCollisionResponseParams::DefaultResponseParam;
	FVehicleSuspensionKinematicsConfig Config;
	Config.TraceChannel = ECC_WorldDynamic;
	FVehicleChassisSimState ChassisState;
	ChassisState.CoMWorldLocation = Chaos::FVec3(0.f, 0.f, 100.f);
	ChassisState.LinearVelocity = Chaos::FVec3(100.f, 0.f, 0.f);

	const EVehicleSuspensionRayCastMode Modes[] = {
		EVehicleSuspensionRayCastMode::LineTrace,
		EVehicleSuspensionRayCastMode::SphereTrace,
		EVehicleSuspensionRayCastMode::BoxTrace,
		EVehicleSuspensionRayCastMode::SphereTraceNoRefinement,
		EVehicleSuspensionRayCastMode::MultiSphereTrace
	};
	const TCHAR* ModeNames[] = { TEXT("Line"), TEXT("Sphere"), TEXT("Box"), TEXT("SmallSphere"), TEXT("MultiSphere") };
	FString Csv = TEXT("Mode,Surface,Hit,Friction,HitDistance,VelocityX,VelocityY,VelocityZ\n");
	for (int32 ModeIndex = 0; ModeIndex < UE_ARRAY_COUNT(Modes); ++ModeIndex)
	{
		Config.RayCastMode = Modes[ModeIndex];
		for (int32 SurfaceIndex = 0; SurfaceIndex < 3; ++SurfaceIndex)
		{
			const TCHAR* Surface = SurfaceIndex == 0 ? TEXT("Static") : SurfaceIndex == 1 ? TEXT("Dynamic") : TEXT("Miss");
			const float X = SurfaceIndex == 0 ? 0.f : SurfaceIndex == 1 ? 1000.f : 2500.f;
			FVehicleSuspensionSimContext Context = MakeRayContext(X);
			FVehicleSuspensionContactLegacyProbe::RayCast(Solver, Context, World, 10.f, 5.f, 1.f,
				QueryParams, ResponseParams, Config);
			if (SurfaceIndex < 2)
			{
				TestTrue(FString::Printf(TEXT("%s %s trace hits real floor"), ModeNames[ModeIndex], Surface), Context.bHitGround);
				TestTrue(FString::Printf(TEXT("%s %s returns expected component"), ModeNames[ModeIndex], Surface),
					Context.HitResult.GetComponent() == (SurfaceIndex == 0 ? StaticFloor : DynamicFloor));
				TestTrue(FString::Printf(TEXT("%s %s reads physical friction"), ModeNames[ModeIndex], Surface),
					FMath::IsNearlyEqual(Context.ImpactFriction, 0.73f, 1.e-4f));
			}
			else
			{
				TestFalse(FString::Printf(TEXT("%s miss remains a miss"), ModeNames[ModeIndex]), Context.bHitGround);
				TestTrue(FString::Printf(TEXT("%s miss uses unit friction fallback"), ModeNames[ModeIndex]),
					FMath::IsNearlyEqual(Context.ImpactFriction, 1.f, 1.e-6f));
			}

			{
#if PHYSICS_THREAD_CONTEXT
				Chaos::FPhysicsThreadContextScope PhysicsContext(true);
#endif
				FVehicleSuspensionContactLegacyProbe::CalculateImpactVelocity(Context, ChassisState);
			}
			const FVector3f Velocity = Context.ImpactWorldVelocity;
			if (SurfaceIndex < 2)
			{
				const float ExpectedX = SurfaceIndex == 0 ? 1.f : 0.5f;
				TestTrue(FString::Printf(TEXT("%s %s contact relative velocity"), ModeNames[ModeIndex], Surface),
					FMath::IsNearlyEqual(Velocity.X, ExpectedX, 1.e-3f)
					&& FMath::IsNearlyZero(Velocity.Y, 1.e-3f) && FMath::IsNearlyZero(Velocity.Z, 1.e-3f));
			}
			else
			{
				TestTrue(FString::Printf(TEXT("%s miss velocity is zero"), ModeNames[ModeIndex]), Velocity.IsNearlyZero());
			}
			Csv += FString::Printf(TEXT("%s,%s,%d,%.9g,%.9g,%.9g,%.9g,%.9g\n"), ModeNames[ModeIndex], Surface,
				Context.bHitGround ? 1 : 0, Context.ImpactFriction, Context.HitDistance, Velocity.X, Velocity.Y, Velocity.Z);
		}
	}
	const FString OutputPath = FPaths::ProjectSavedDir() / TEXT("Automation/KinetiForge/SuspensionContact/contact.csv");
	IFileManager::Get().MakeDirectory(*FPaths::GetPath(OutputPath), true);
	TestTrue(TEXT("Stable contact baseline CSV saved"), FFileHelper::SaveStringToFile(Csv, *OutputPath));
	AddInfo(FString::Printf(TEXT("Suspension solver capture saved: %s"), *OutputPath));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleSuspensionReactionLegacyTest,
	"KinetiForge.Suspension.Contact.Reaction",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleSuspensionReactionLegacyTest::RunTest(const FString& Parameters)
{
	using namespace VehicleSuspensionContactTest;
	FWorldOwner WorldOwner;
	UWorld* World = CreateWorld(*this, WorldOwner);
	if (!World) return false;
	UBoxComponent* Chassis = AddBox(*World, TEXT("ReactionChassis"), FVector(5000.f, 0.f, 100.f), FVector(50.f), true);
	UBoxComponent* Contact = AddBox(*World, TEXT("ReactionContact"), FVector(5000.f, 300.f, 100.f), FVector(50.f), true);
	if (!TestNotNull(TEXT("Reaction chassis created"), Chassis)
		|| !TestNotNull(TEXT("Reaction contact body created"), Contact)) return false;
	FlushPhysicsScene(*World);
	Chaos::FRigidBodyHandle_Internal* ChassisHandle = UVehicleUtilities::GetInternalHandle(Chassis);
	Chaos::FRigidBodyHandle_Internal* ContactHandle = UVehicleUtilities::GetInternalHandle(Contact);
	if (!TestNotNull(TEXT("Chassis has Chaos body"), ChassisHandle)
		|| !TestNotNull(TEXT("Contact has Chaos body"), ContactHandle)) return false;

	TStrongObjectPtr<UVehicleWheelComponent> Wheel(NewObject<UVehicleWheelComponent>());
	FVehicleWheelContactProbe::ChassisTransform(*Wheel) = Chassis->GetComponentTransform();
	FVehicleWheelContactProbe::WheelContext(*Wheel).MacroDeltaTime = 0.01f;
	FVehicleWheelContactProbe::WheelState(*Wheel).TireForce = FVector3f(30.f, 40.f, 0.f);
	FVehicleSuspensionSimState& SuspensionState = FVehicleWheelContactProbe::SuspensionState(*Wheel);
	SuspensionState.bHitGround = true;
	SuspensionState.ImpactWorldNormal = FVector3f(0.f, 0.f, 1.f);
	SuspensionState.ImpactWorldLocation = Chassis->GetComponentLocation() + FVector(20.f, 0.f, 0.f);
	SuspensionState.HubChassisLocation = FVector3f(0.f, 0.f, 0.f);
	SuspensionState.ForceAlongImpactNormal = 100.f;
	FVehicleSuspensionHitResult& Hit = FVehicleWheelContactProbe::HitResult(*Wheel);
	Hit.Component = Contact;
	Hit.BoneName = NAME_None;

	const FVector ExpectedImpulse(30.f, 40.f, 100.f);
	const FVector ImpactPoint = SuspensionState.ImpactWorldLocation;
	const FVector ExpectedChassisAngularImpulse = FVector::CrossProduct(
		ImpactPoint - Chassis->GetCenterOfMass(), ExpectedImpulse);
	const FVector ExpectedContactAngularImpulse = FVector::CrossProduct(
		ImpactPoint - Contact->GetCenterOfMass(), -ExpectedImpulse);
	{
#if PHYSICS_THREAD_CONTEXT
		Chaos::FPhysicsThreadContextScope PhysicsContext(true);
#endif
		FVehicleWheelContactProbe::KinematicsConfig(*Wheel).LocationToApplyForce = EVehicleForceApplicationPoint::ImpactPoint;
		const FVector ChassisLinearBefore = ChassisHandle->LinearImpulse();
		const FVector ChassisAngularBefore = ChassisHandle->AngularImpulse();
		const FVector ContactLinearBefore = ContactHandle->LinearImpulse();
		const FVector ContactAngularBefore = ContactHandle->AngularImpulse();
		FVehicleWheelContactProbe::ApplyWheelForce(*Wheel, ChassisHandle);
		const FVector ChassisLinearDelta = ChassisHandle->LinearImpulse() - ChassisLinearBefore;
		const FVector ChassisAngularDelta = ChassisHandle->AngularImpulse() - ChassisAngularBefore;
		const FVector ContactLinearDelta = ContactHandle->LinearImpulse() - ContactLinearBefore;
		const FVector ContactAngularDelta = ContactHandle->AngularImpulse() - ContactAngularBefore;
		TestTrue(TEXT("Impact point applies expected chassis impulse"), ChassisLinearDelta.Equals(ExpectedImpulse, 1.e-3f));
		TestTrue(TEXT("Impact arm applies expected chassis angular impulse"), ChassisAngularDelta.Equals(ExpectedChassisAngularImpulse, 1.e-2f));
		TestTrue(TEXT("Contact body receives equal and opposite impulse"), ContactLinearDelta.Equals(-ExpectedImpulse, 1.e-3f));
		TestTrue(TEXT("Contact impact point produces expected reaction moment"), ContactAngularDelta.Equals(ExpectedContactAngularImpulse, 1.e-2f));

		FVehicleWheelContactProbe::KinematicsConfig(*Wheel).LocationToApplyForce = EVehicleForceApplicationPoint::WheelCenter;
		const FVector ChassisAngularBeforeWheelCenter = ChassisHandle->AngularImpulse();
		const FVector ContactLinearBeforeWheelCenter = ContactHandle->LinearImpulse();
		const FVector ContactAngularBeforeWheelCenter = ContactHandle->AngularImpulse();
		FVehicleWheelContactProbe::ApplyWheelForce(*Wheel, ChassisHandle);
		const FVector ChassisAngularDeltaWheelCenter = ChassisHandle->AngularImpulse() - ChassisAngularBeforeWheelCenter;
		const FVector ContactLinearDeltaWheelCenter = ContactHandle->LinearImpulse() - ContactLinearBeforeWheelCenter;
		const FVector ContactAngularDeltaWheelCenter = ContactHandle->AngularImpulse() - ContactAngularBeforeWheelCenter;
		TestTrue(TEXT("Wheel center changes chassis moment arm"), ChassisAngularDeltaWheelCenter.IsNearlyZero(1.e-2f));
		TestTrue(TEXT("Contact reaction remains applied at impact point"), ContactLinearDeltaWheelCenter.Equals(-ExpectedImpulse, 1.e-3f));
		TestTrue(TEXT("Contact reaction moment remains at impact point"), ContactAngularDeltaWheelCenter.Equals(ExpectedContactAngularImpulse, 1.e-2f));
	}
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
