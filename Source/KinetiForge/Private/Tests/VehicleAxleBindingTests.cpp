// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "VehicleAxleAssemblyComponent.h"
#include "VehicleAxleBridge.h"
#include "VehicleAxleSolver.h"
#include "VehicleDifferentialComponent.h"
#include "VehicleDifferentialSolver.h"
#include "VehicleWheelBridge.h"
#include "VehicleWheelComponent.h"
#include "VehicleUtilities.h"
#include "VehiclePhysicsScene.h"
#include "KinetiForge.h"
#include "Modules/ModuleManager.h"
#include "Misc/ScopeExit.h"
#include <limits>
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "PBDRigidsSolver.h"
#include "Misc/AutomationTest.h"

struct FVehicleAxleBindingProbe
{
	static void EndManualPlay(UVehicleAxleAssemblyComponent& Axle, UVehicleWheelComponent& Wheel)
	{
		Wheel.EndPlay(EEndPlayReason::RemovedFromWorld);
		Axle.EndPlay(EEndPlayReason::RemovedFromWorld);
	}
	static void Configure(UVehicleAxleAssemblyComponent& Axle, UVehicleDifferentialComponent* Diff,
		EVehicleAxleSuspensionType SuspensionType)
	{
		Axle.Differential = Diff;
		Axle.AxleLayout = EVehicleAxleLayout::TwoWheels;
		Axle.SuspensionType = SuspensionType;
		Axle.AxleConfig.TrackWidth = 120.f;
		Axle.AxleConfig.TorqueWeight = 1.f;
		Axle.AxleConfig.DriveShaftInertia = 0.4f;
		Axle.AxleConfig.DriveShaftTorsionalStiffness = 5200.f;
		Axle.AxleSteeringConfig.bAffectedBySteering = true;
		Axle.AxleSteeringConfig.MaxSteeringAngle = 32.f;
		Axle.AxleSteeringConfig.bAckermannSteering = true;
		Axle.AxleSteeringConfig.AckermannRatio = 0.8f;
		Axle.SteeringAssistConfig.bSteeringAssistEnabled = true;
		Axle.TCSConfig.bTractionControlSystemEnabled = true;
		Axle.TCSConfig.OptimalSlip = 0.15f;
		Axle.TCSConfig.ActivationSpeed = 0.1f;
		Axle.TCSConfig.Sensitivity = 4.f;
	}

	static void SeedState(UVehicleAxleAssemblyComponent& Axle, float AngularVelocity, float EffectiveInertia,
		float TorqueWeight)
	{
		FVehicleAxleSimState& RuntimeState = Axle.GetAxleBridge()->GetRuntimeForSimulation().State;
		RuntimeState.AxleAngularVelocity = AngularVelocity;
		RuntimeState.AxleEffectiveInertia = EffectiveInertia;
		RuntimeState.NumOfWheels = 2;
		RuntimeState.NumOfWheelOnGround = 2;
		Axle.State.AxleAngularVelocity = AngularVelocity;
		Axle.State.AxleEffectiveInertia = EffectiveInertia;
		Axle.State.NumOfWheels = 2;
		Axle.State.NumOfWheelOnGround = 2;
		Axle.AxleConfig.TorqueWeight = TorqueWeight;
	}
};

namespace VehicleAxleBindingTest
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
		UVehicleDifferentialComponent* Differential = nullptr;
		UVehicleWheelComponent* Left = nullptr;
		UVehicleWheelComponent* Right = nullptr;
	};

	UWorld* CreateWorld(FAutomationTestBase& Test, FWorldOwner& Owner)
	{
		Owner.World = UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr,
			true, ERHIFeatureLevel::Num, nullptr, true);
		if (!Test.TestNotNull(TEXT("World created"), Owner.World)) return nullptr;
		Owner.World->InitWorld(UWorld::InitializationValues().AllowAudioPlayback(false)
			.CreatePhysicsScene(true).RequiresHitProxies(false).CreateNavigation(false)
			.CreateAISystem(false).ShouldSimulatePhysics(false).SetTransactional(false));
		if (!Test.TestNotNull(TEXT("Physics scene created"), Owner.World->GetPhysicsScene())) return nullptr;
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

	UBoxComponent* AddGround(UWorld& World)
	{
		AActor* Actor = World.SpawnActor<AActor>(FVector(500.f, 0.f, -20.f), FRotator::ZeroRotator);
		if (!Actor) return nullptr;
		UBoxComponent* Ground = NewObject<UBoxComponent>(Actor, TEXT("AxleBindingGround"));
		Actor->AddInstanceComponent(Ground);
		Actor->SetRootComponent(Ground);
		Ground->SetBoxExtent(FVector(1600.f, 300.f, 20.f));
		Ground->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Ground->SetCollisionObjectType(ECC_WorldStatic);
		Ground->SetCollisionResponseToAllChannels(ECR_Block);
		Ground->SetMobility(EComponentMobility::Static);
		Ground->RegisterComponent();
		Ground->SetWorldLocation(FVector(500.f, 0.f, -20.f), false, nullptr, ETeleportType::TeleportPhysics);
		Ground->RecreatePhysicsState();
		return Ground;
	}

	FVehicle CreateVehicle(UWorld& World, const FVector& Location, bool bIsolated,
		EVehicleAxleSuspensionType SuspensionType)
	{
		FVehicle Vehicle;
		Vehicle.Actor = World.SpawnActor<AActor>(Location, FRotator::ZeroRotator);
		if (!Vehicle.Actor) return Vehicle;
		Vehicle.Body = NewObject<UBoxComponent>(Vehicle.Actor, TEXT("AxleBindingBody"));
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

		Vehicle.Differential = NewObject<UVehicleDifferentialComponent>(Vehicle.Actor, TEXT("AxleBindingDiff"));
		Vehicle.Actor->AddInstanceComponent(Vehicle.Differential);
		FVehicleLimitedSlipDifferentialConfig DiffConfig;
		DiffConfig.GearRatio = 3.f;
		DiffConfig.DriveLockRatio = 0.55f;
		DiffConfig.CoastLockRatio = 0.25f;
		Vehicle.Differential->SetConfig(DiffConfig);

		Vehicle.Axle = NewObject<UVehicleAxleAssemblyComponent>(Vehicle.Actor, TEXT("AxleBindingAxle"));
		Vehicle.Actor->AddInstanceComponent(Vehicle.Axle);
		Vehicle.Axle->SetupAttachment(Vehicle.Body);
		// Place the designed hubs near the road; a chassis-height axle leaves both rays above it.
		Vehicle.Axle->SetRelativeLocation(FVector(0.f, 0.f, -55.f));
		FVehicleAxleBindingProbe::Configure(*Vehicle.Axle, Vehicle.Differential, SuspensionType);
		Vehicle.Axle->RegisterComponent(); // OnRegister generates, attaches and initializes both real wheel components.
		Vehicle.Axle->GetWheels(Vehicle.Left, Vehicle.Right);
		if (!Vehicle.Left || !Vehicle.Right) return Vehicle;
		// Preserve the old serialized preference in this fixture; bridge ownership is unconditional now.
		Vehicle.Left->bUseIsolatedWheelRuntime = bIsolated;
		Vehicle.Right->bUseIsolatedWheelRuntime = bIsolated;
		Vehicle.Left->SetSprungMass(300.f);
		Vehicle.Right->SetSprungMass(300.f);
		return Vehicle;
	}

	bool WheelStatesMatch(const FVehicleWheelSimState& A, const FVehicleWheelSimState& B, float Tolerance = 1.e-2f)
	{
		return FMath::IsNearlyEqual(A.AngularVelocity, B.AngularVelocity, Tolerance)
			&& FMath::IsNearlyEqual(A.EffectiveInertia, B.EffectiveInertia, Tolerance)
			&& FMath::IsNearlyEqual(A.PredictedSlipRatio, B.PredictedSlipRatio, Tolerance)
			&& A.TireForce.Equals(B.TireForce, Tolerance)
			&& FMath::IsNearlyEqual(A.DriveTorque, B.DriveTorque, Tolerance)
			&& FMath::IsNearlyEqual(A.BrakeTorque, B.BrakeTorque, Tolerance)
			&& A.bABSTriggered == B.bABSTriggered;
	}

	bool SuspensionStatesMatch(const FVehicleSuspensionSimState& A, const FVehicleSuspensionSimState& B,
		float Tolerance = 1.e-2f, const FVector& BOffset = FVector::ZeroVector)
	{
		return A.bIsRightWheel == B.bIsRightWheel && A.bHitGround == B.bHitGround
			&& A.bWheelOnGround == B.bWheelOnGround
			&& FMath::IsNearlyEqual(A.SteeringAngle, B.SteeringAngle, Tolerance)
			&& FMath::IsNearlyEqual(A.StrutCurrentLength, B.StrutCurrentLength, Tolerance)
			&& FMath::IsNearlyEqual(A.StaticSprungMass, B.StaticSprungMass, Tolerance)
			&& FMath::IsNearlyEqual(A.ForceAlongImpactNormal, B.ForceAlongImpactNormal, Tolerance)
			&& A.HubChassisLocation.Equals(B.HubChassisLocation, Tolerance)
			&& A.HubChassisRotation.Equals(B.HubChassisRotation, Tolerance)
			&& A.ImpactWorldVelocity.Equals(B.ImpactWorldVelocity, Tolerance)
			&& A.ImpactWorldNormal.Equals(B.ImpactWorldNormal, Tolerance)
			&& A.ImpactWorldLocation.Equals(B.ImpactWorldLocation - (A.bHitGround ? BOffset : FVector::ZeroVector), Tolerance);
	}

	bool AxleStatesMatch(const FVehicleAxleSimState& A, const FVehicleAxleSimState& B, float Tolerance = 1.e-2f)
	{
		return A.NumOfWheels == B.NumOfWheels && A.NumOfWheelOnGround == B.NumOfWheelOnGround
			&& FMath::IsNearlyEqual(A.LeftWheelSteeringAngle, B.LeftWheelSteeringAngle, Tolerance)
			&& FMath::IsNearlyEqual(A.RightWheelSteeringAngle, B.RightWheelSteeringAngle, Tolerance)
			&& FMath::IsNearlyEqual(A.AxleDriveTorque, B.AxleDriveTorque, Tolerance)
			&& FMath::IsNearlyEqual(A.LeftDriveTorque, B.LeftDriveTorque, Tolerance)
			&& FMath::IsNearlyEqual(A.RightDriveTorque, B.RightDriveTorque, Tolerance)
			&& FMath::IsNearlyEqual(A.AxleEffectiveInertia, B.AxleEffectiveInertia, Tolerance)
			&& FMath::IsNearlyEqual(A.AxleAngularVelocity, B.AxleAngularVelocity, Tolerance)
			&& FMath::IsNearlyEqual(A.P3MotorTorque, B.P3MotorTorque, Tolerance)
			&& FMath::IsNearlyEqual(A.WheelBase, B.WheelBase, Tolerance)
			&& A.WorldLinearVelocity.Equals(B.WorldLinearVelocity, Tolerance)
			&& A.LocalLinearVelocity.Equals(B.LocalLinearVelocity, Tolerance)
			&& A.bTCSTriggered == B.bTCSTriggered;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleAxleRuntimeBindingTest,
	"KinetiForge.AxleRuntime.Binding.RegisteredComparison",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleAxleRuntimeBindingTest::RunTest(const FString& Parameters)
{
	using namespace VehicleAxleBindingTest;
	(void)Parameters;
	FWorldOwner WorldOwner;
	UWorld* World = CreateWorld(*this, WorldOwner);
	if (!World || !TestNotNull(TEXT("Static ground created"), AddGround(*World))) return false;

	FVehicle Legacy = CreateVehicle(*World, FVector(0.f, 0.f, 80.f), false, EVehicleAxleSuspensionType::Independent);
	FVehicle Isolated = CreateVehicle(*World, FVector(1000.f, 0.f, 80.f), true, EVehicleAxleSuspensionType::Independent);
	if (!TestNotNull(TEXT("Legacy body"), Legacy.Body) || !TestNotNull(TEXT("Legacy axle"), Legacy.Axle)
		|| !TestNotNull(TEXT("Legacy left wheel"), Legacy.Left) || !TestNotNull(TEXT("Legacy right wheel"), Legacy.Right)
		|| !TestNotNull(TEXT("Isolated body"), Isolated.Body) || !TestNotNull(TEXT("Isolated axle"), Isolated.Axle)
		|| !TestNotNull(TEXT("Isolated left wheel"), Isolated.Left) || !TestNotNull(TEXT("Isolated right wheel"), Isolated.Right)) return false;
	FlushPhysics(*World);
	Chaos::FRigidBodyHandle_Internal* LegacyHandle = UVehicleUtilities::GetInternalHandle(Legacy.Body);
	Chaos::FRigidBodyHandle_Internal* IsolatedHandle = UVehicleUtilities::GetInternalHandle(Isolated.Body);
	if (!TestNotNull(TEXT("Legacy body handle"), LegacyHandle) || !TestNotNull(TEXT("Isolated body handle"), IsolatedHandle)) return false;
	TestTrue(TEXT("Isolated axle binds registered axle and both generated wheels before BeginPlay"), Isolated.Axle->BindIsolatedAxleRuntime());
	TestTrue(TEXT("Isolated wheel bridge bound by axle"), Isolated.Left->GetWheelBridge().IsValid() && Isolated.Right->GetWheelBridge().IsValid());
	TestTrue(TEXT("Axle bridge exists regardless of the old wheel runtime preference"), Legacy.Axle->GetAxleBridge().IsValid());
	TestTrue(TEXT("Wheel bridges exist regardless of the old runtime preference"), Legacy.Left->GetWheelBridge().IsValid() && Legacy.Right->GetWheelBridge().IsValid());
	TestTrue(TEXT("Repeated axle bind is idempotent"), Legacy.Axle->BindIsolatedAxleRuntime());
	Isolated.Axle->GetAxleBridge()->ConsumeInputs(); // Quiescent fixture macro-boundary consumption.
	TestTrue(TEXT("Axle settings snapshot contains TCS and differential configuration"),
		Isolated.Axle->GetAxleBridge()->GetRuntimeForSimulation().Settings.TCSConfig.bTractionControlSystemEnabled
		&& Isolated.Axle->GetAxleBridge()->GetRuntimeForSimulation().Settings.bHasDifferential
		&& FMath::IsNearlyEqual(Isolated.Axle->GetAxleBridge()->GetRuntimeForSimulation().Settings.DifferentialConfig.GearRatio, 3.f));

	int32 ContactFrames = 0, AirFrames = 0;
	for (int32 Step = 0; Step < 6; ++Step)
	{
		const float Height = Step < 4 ? 80.f : 2000.f;
		if (Step == 3)
		{
			Legacy.Axle->SetAxleSuspensionType(EVehicleAxleSuspensionType::Solid);
			Isolated.Axle->SetAxleSuspensionType(EVehicleAxleSuspensionType::Solid);
		}
		Legacy.Body->SetWorldLocation(FVector(0.f, 0.f, Height), false, nullptr, ETeleportType::TeleportPhysics);
		Isolated.Body->SetWorldLocation(FVector(1000.f, 0.f, Height), false, nullptr, ETeleportType::TeleportPhysics);
		FlushPhysics(*World);
		const float Steering = Step % 3 == 0 ? 0.55f : Step % 3 == 1 ? 0.f : -0.55f;
		const float P3 = Step < 3 ? 70.f : -25.f;
		const float WheelBase = Step < 3 ? 280.f : 310.f;
		Legacy.Axle->SetP3MotorTorque(P3);
		Isolated.Axle->SetP3MotorTorque(P3);
		Legacy.Axle->SetWheelBase(WheelBase);
		Isolated.Axle->SetWheelBase(WheelBase);
		float LegacyOutI = 0.f, LegacyOutOmega = 0.f, LegacyOutK = 0.f;
		float IsolatedOutI = 0.f, IsolatedOutOmega = 0.f, IsolatedOutK = 0.f;
		FVector LegacyImpulseBefore, LegacyAngularBefore, IsolatedImpulseBefore, IsolatedAngularBefore;
		FVector LegacyImpulseAfter, LegacyAngularAfter, IsolatedImpulseAfter, IsolatedAngularAfter;
		{
#if PHYSICS_THREAD_CONTEXT
			Chaos::FPhysicsThreadContextScope PhysicsContext(true);
#endif
			LegacyImpulseBefore = LegacyHandle->LinearImpulse();
			LegacyAngularBefore = LegacyHandle->AngularImpulse();
			IsolatedImpulseBefore = IsolatedHandle->LinearImpulse();
			IsolatedAngularBefore = IsolatedHandle->AngularImpulse();
			Isolated.Axle->PreStepAxle(1.f / 120.f, Steering);
			Legacy.Axle->UpdatePhysics(1.f / 120.f, 330.f, 0.2f, 0.15f, Steering, LegacyOutI, LegacyOutOmega, LegacyOutK);
			Isolated.Axle->SubstepAxle(1.f / 120.f, 330.f, 0.2f, 0.15f, IsolatedOutI, IsolatedOutOmega, IsolatedOutK);
			Isolated.Axle->PostStepAxle();
			LegacyImpulseAfter = LegacyHandle->LinearImpulse();
			LegacyAngularAfter = LegacyHandle->AngularImpulse();
			IsolatedImpulseAfter = IsolatedHandle->LinearImpulse();
			IsolatedAngularAfter = IsolatedHandle->AngularImpulse();
		}
		TestTrue(TEXT("Manual UpdatePhysics returns current-step axle speed and inertia to existing getters"),
			FMath::IsNearlyEqual(Legacy.Axle->GetAngularVelocity(), LegacyOutOmega)
			&& FMath::IsNearlyEqual(Legacy.Axle->GetTotalAxleInertia(), LegacyOutI));
		const FVehicleAxleSimState& IsolatedRuntimeState = Isolated.Axle->GetAxleBridge()->GetRuntimeForSimulation().State;
		const FVehicleWheelRuntime& IsolatedLeftRuntime = Isolated.Left->GetWheelBridge()->GetRuntimeForSimulation();
		const FVehicleWheelRuntime& IsolatedRightRuntime = Isolated.Right->GetWheelBridge()->GetRuntimeForSimulation();
		TestTrue(FString::Printf(TEXT("Step %d compatibility axle outputs match"), Step),
			AxleStatesMatch(Legacy.Axle->GetAxleState(), IsolatedRuntimeState));
		if (!FMath::IsNearlyZero(Steering))
		{
			TestFalse(FString::Printf(TEXT("Step %d Ackermann gives distinct inside/outside angles"), Step),
				FMath::IsNearlyEqual(IsolatedRuntimeState.LeftWheelSteeringAngle,
					IsolatedRuntimeState.RightWheelSteeringAngle, 1.e-3f));
		}
		TestTrue(FString::Printf(TEXT("Step %d left wheel compatibility output matches"), Step),
			WheelStatesMatch(Legacy.Left->GetWheelState(), IsolatedLeftRuntime.WheelState));
		TestTrue(FString::Printf(TEXT("Step %d right wheel compatibility output matches"), Step),
			WheelStatesMatch(Legacy.Right->GetWheelState(), IsolatedRightRuntime.WheelState));
		TestTrue(FString::Printf(TEXT("Step %d left suspension matches"), Step),
			SuspensionStatesMatch(Legacy.Left->GetSuspensionState(), IsolatedLeftRuntime.SuspensionState,
				1.e-2f, FVector(1000.f, 0.f, 0.f)));
		TestTrue(FString::Printf(TEXT("Step %d right suspension matches"), Step),
			SuspensionStatesMatch(Legacy.Right->GetSuspensionState(), IsolatedRightRuntime.SuspensionState,
				1.e-2f, FVector(1000.f, 0.f, 0.f)));
		TestTrue(FString::Printf(TEXT("Step %d reflected axle outputs match"), Step),
			FMath::IsNearlyEqual(LegacyOutI, IsolatedOutI, 1.e-2f)
			&& FMath::IsNearlyEqual(LegacyOutOmega, IsolatedOutOmega, 1.e-2f)
			&& FMath::IsNearlyEqual(LegacyOutK, IsolatedOutK, 1.e-2f));
		TestTrue(FString::Printf(TEXT("Step %d body impulse matches"), Step),
			(LegacyImpulseAfter - LegacyImpulseBefore).Equals(IsolatedImpulseAfter - IsolatedImpulseBefore, 1.e-2f));
		TestTrue(FString::Printf(TEXT("Step %d angular impulse matches"), Step),
			(LegacyAngularAfter - LegacyAngularBefore).Equals(IsolatedAngularAfter - IsolatedAngularBefore, 1.e-1f));
		ContactFrames += IsolatedLeftRuntime.SuspensionState.bHitGround && IsolatedRightRuntime.SuspensionState.bHitGround ? 1 : 0;
		AirFrames += !IsolatedLeftRuntime.SuspensionState.bHitGround && !IsolatedRightRuntime.SuspensionState.bHitGround ? 1 : 0;
		Isolated.Axle->RefreshPublishedAxleState();
		Isolated.Left->RefreshPublishedWheelState();
		Isolated.Right->RefreshPublishedWheelState();
		TestTrue(FString::Printf(TEXT("Step %d GT refresh matches published axle state"), Step),
			AxleStatesMatch(Isolated.Axle->GetAxleState(), Isolated.Axle->GetAxleBridge()->GetRuntimeForSimulation().State));
	}
	TestTrue(TEXT("Axle binding comparison includes two-wheel contact"), ContactFrames > 0);
	TestTrue(TEXT("Axle binding comparison includes two-wheel airborne frames"), AirFrames > 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleAxleBridgeValueTest,
	"KinetiForge.AxleRuntime.Binding.BridgeValueHandoff",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleAxleBridgeValueTest::RunTest(const FString& Parameters)
{
	(void)Parameters;
	FVehicleAxleRuntime Initial;
	Initial.Settings.AxleConfig.DriveShaftInertia = 1.f;
	Initial.State.AxleAngularVelocity = 13.f;
	Initial.State.AxleEffectiveInertia = 1.f;
	FVehicleAxleBridge Bridge(Initial);
	FVehicleAxleRuntimeSettings NewSettings = Initial.Settings;
	NewSettings.AxleConfig.DriveShaftInertia = 2.5f;
	Bridge.SubmitSettings(NewSettings);
	const uint64 P3Sequence = Bridge.RequestP3MotorTorque(7.f);
	const uint64 WheelBaseSequence = Bridge.RequestWheelBase(240.f);
	Bridge.ConsumeInputs();
	FVehicleAxleRuntime& Runtime = Bridge.GetRuntimeForSimulation();
	TestTrue(TEXT("Applying config and commands preserves physical axle velocity"),
		FMath::IsNearlyEqual(Runtime.State.AxleAngularVelocity, 13.f));
	TestTrue(TEXT("Config snapshot changes the solver parameter"),
		FMath::IsNearlyEqual(Runtime.Settings.AxleConfig.DriveShaftInertia, 2.5f));
	TestTrue(TEXT("P3 and wheelbase commands are applied"),
		FMath::IsNearlyEqual(Runtime.State.P3MotorTorque, 7.f) && FMath::IsNearlyEqual(Runtime.State.WheelBase, 240.f));
	TestTrue(TEXT("Requested command sequences advance"), P3Sequence == 1 && WheelBaseSequence == 1);

	FVehicleAxleSolver::PrepareTransmissionStep(Runtime.State, Runtime.Settings.AxleConfig,
		Runtime.Settings.TCSConfig, EVehicleAxleLayout::TwoWheels, nullptr,
		nullptr, nullptr, 0.1f, 10.f, 0.f, 0.f);
	TestTrue(TEXT("Real no-differential shaft update advances the caller-owned state"),
		FMath::IsNearlyEqual(Runtime.State.AxleAngularVelocity, 13.4f, 1.e-5f));
	Bridge.PublishResult();
	const FVehicleAxleResult FirstPublished = Bridge.ReadResult();
	TestTrue(TEXT("Published result contains current physical and command values"),
		FMath::IsNearlyEqual(FirstPublished.State.AxleAngularVelocity, 13.4f, 1.e-5f)
		&& FMath::IsNearlyEqual(FirstPublished.State.P3MotorTorque, 7.f)
		&& FirstPublished.AppliedP3Sequence == P3Sequence
		&& FirstPublished.AppliedWheelBaseSequence == WheelBaseSequence);

	Bridge.RequestP3MotorTorque(9.f);
	Bridge.RequestWheelBase(260.f);
	const FVehicleAxleResult StillOld = Bridge.ReadResult();
	TestTrue(TEXT("Unread publication remains a snapshot while new commands are pending"),
		StillOld.PublicationSequence == FirstPublished.PublicationSequence
		&& FMath::IsNearlyEqual(StillOld.State.P3MotorTorque, 7.f));
	Bridge.ConsumeInputs();
	TestTrue(TEXT("New commands do not rewind the simulated shaft state"),
		FMath::IsNearlyEqual(Runtime.State.AxleAngularVelocity, 13.4f, 1.e-5f)
		&& FMath::IsNearlyEqual(Runtime.State.P3MotorTorque, 9.f)
		&& FMath::IsNearlyEqual(Runtime.State.WheelBase, 260.f));
	FVehicleAxleSolver::PrepareTransmissionStep(Runtime.State, Runtime.Settings.AxleConfig,
		Runtime.Settings.TCSConfig, EVehicleAxleLayout::TwoWheels, nullptr,
		nullptr, nullptr, 0.1f, 10.f, 0.f, 0.f);
	Bridge.ConsumeInputs(); // Empty consume cannot restore values from the earlier publication.
	TestTrue(TEXT("Empty consume preserves the newly advanced state"),
		FMath::IsNearlyEqual(Runtime.State.AxleAngularVelocity, 13.8f, 1.e-5f)
		&& FMath::IsNearlyEqual(Runtime.State.P3MotorTorque, 9.f));
	Bridge.PublishResult();
	const FVehicleAxleResult Latest = Bridge.ReadResult();
	TestTrue(TEXT("Latest publication acknowledges the newest commands and shaft output"),
		FMath::IsNearlyEqual(Latest.State.AxleAngularVelocity, 13.8f, 1.e-5f)
		&& FMath::IsNearlyEqual(Latest.State.WheelBase, 260.f)
		&& Latest.AppliedP3Sequence == 2 && Latest.AppliedWheelBaseSequence == 2);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleAxleCentralDiffBindingTest,
	"KinetiForge.AxleRuntime.Binding.CentralDiffUsesSimulationSnapshot",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleAxleCentralDiffBindingTest::RunTest(const FString& Parameters)
{
	using namespace VehicleAxleBindingTest;
	(void)Parameters;
	FWorldOwner WorldOwner;
	UWorld* World = CreateWorld(*this, WorldOwner);
	if (!World || !TestNotNull(TEXT("Static ground created"), AddGround(*World))) return false;

	FVehicle LegacyA = CreateVehicle(*World, FVector(0.f, 0.f, 80.f), false, EVehicleAxleSuspensionType::Independent);
	FVehicle LegacyB = CreateVehicle(*World, FVector(300.f, 0.f, 80.f), false, EVehicleAxleSuspensionType::Independent);
	FVehicle IsolatedA = CreateVehicle(*World, FVector(1000.f, 0.f, 80.f), true, EVehicleAxleSuspensionType::Independent);
	FVehicle IsolatedB = CreateVehicle(*World, FVector(1300.f, 0.f, 80.f), true, EVehicleAxleSuspensionType::Independent);
	if (!TestNotNull(TEXT("Legacy axle A"), LegacyA.Axle) || !TestNotNull(TEXT("Legacy axle B"), LegacyB.Axle)
		|| !TestNotNull(TEXT("Bound axle A"), IsolatedA.Axle) || !TestNotNull(TEXT("Bound axle B"), IsolatedB.Axle)) return false;
	FlushPhysics(*World);
	TestTrue(TEXT("Bind central-diff axle A"), IsolatedA.Axle->BindIsolatedAxleRuntime());
	TestTrue(TEXT("Bind central-diff axle B"), IsolatedB.Axle->BindIsolatedAxleRuntime());

	FVehicleAxleConfig ConfigA = LegacyA.Axle->GetAxleConfig();
	ConfigA.TorqueWeight = 1.f;
	FVehicleAxleConfig ConfigB = LegacyB.Axle->GetAxleConfig();
	ConfigB.TorqueWeight = 3.f;
	LegacyA.Axle->SetAxleConfig(ConfigA);
	LegacyB.Axle->SetAxleConfig(ConfigB);
	IsolatedA.Axle->SetAxleConfig(ConfigA);
	IsolatedB.Axle->SetAxleConfig(ConfigB);
	LegacyA.Axle->GetAxleBridge()->ConsumeInputs();
	LegacyB.Axle->GetAxleBridge()->ConsumeInputs();
	// Drain the submitted config first. Then intentionally diverge only the GT display/config
	// fields from the frozen simulation snapshot so the central differential must read the latter.
	IsolatedA.Axle->GetAxleBridge()->ConsumeInputs();
	IsolatedB.Axle->GetAxleBridge()->ConsumeInputs();
	FVehicleAxleBindingProbe::SeedState(*LegacyA.Axle, 10.f, 4.f, 1.f);
	FVehicleAxleBindingProbe::SeedState(*LegacyB.Axle, 30.f, 8.f, 3.f);
	FVehicleAxleBindingProbe::SeedState(*IsolatedA.Axle, 80.f, 6.f, 9.f);
	FVehicleAxleBindingProbe::SeedState(*IsolatedB.Axle, -10.f, 3.f, 1.f);
	FVehicleAxleRuntime& SimA = IsolatedA.Axle->GetAxleBridge()->GetRuntimeForSimulation();
	FVehicleAxleRuntime& SimB = IsolatedB.Axle->GetAxleBridge()->GetRuntimeForSimulation();
	SimA.State.AxleAngularVelocity = 10.f;
	SimA.State.AxleEffectiveInertia = 4.f;
	SimA.State.NumOfWheels = SimA.State.NumOfWheelOnGround = 2;
	SimB.State.AxleAngularVelocity = 30.f;
	SimB.State.AxleEffectiveInertia = 8.f;
	SimB.State.NumOfWheels = SimB.State.NumOfWheelOnGround = 2;
	SimA.Settings.AxleConfig.TorqueWeight = 1.f;
	SimB.Settings.AxleConfig.TorqueWeight = 3.f;
	const float StateVelocityGap = FMath::Abs(IsolatedA.Axle->GetAxleState().AxleAngularVelocity - SimA.State.AxleAngularVelocity)
		+ FMath::Abs(IsolatedB.Axle->GetAxleState().AxleAngularVelocity - SimB.State.AxleAngularVelocity);
	const float WeightGap = FMath::Abs(IsolatedA.Axle->GetAxleConfig().TorqueWeight - SimA.Settings.AxleConfig.TorqueWeight)
		+ FMath::Abs(IsolatedB.Axle->GetAxleConfig().TorqueWeight - SimB.Settings.AxleConfig.TorqueWeight);
	TestTrue(TEXT("Fixture has nonzero stale axle velocity/inertia and torque-weight differences"), StateVelocityGap > 1.f && WeightGap > 1.f);

	FVehicleLimitedSlipDifferentialConfig CentralConfig;
	CentralConfig.GearRatio = 2.f;
	CentralConfig.DriveLockRatio = 0.75f;
	CentralConfig.CoastLockRatio = 0.25f;
	UVehicleDifferentialComponent* LegacyCentral = NewObject<UVehicleDifferentialComponent>();
	UVehicleDifferentialComponent* IsolatedCentral = NewObject<UVehicleDifferentialComponent>();
	if (!TestNotNull(TEXT("Legacy central differential"), LegacyCentral)
		|| !TestNotNull(TEXT("Isolated central differential"), IsolatedCentral)) return false;
	LegacyCentral->SetConfig(CentralConfig);
	IsolatedCentral->SetConfig(CentralConfig);
	const FVehicleLimitedSlipDifferentialConfig OriginalConfigA = IsolatedA.Differential->GetConfig();
	const FVehicleLimitedSlipDifferentialConfig OriginalConfigB = IsolatedB.Differential->GetConfig();
	FVehicleLimitedSlipDifferentialConfig UpdatedConfig = OriginalConfigA;
	UpdatedConfig.GearRatio = 4.5f;
	UpdatedConfig.DriveLockRatio = 0.9f;
	IsolatedA.Differential->SetConfig(UpdatedConfig);
	IsolatedB.Differential->SetConfig(UpdatedConfig);
	// The public component setter must notify bound axle consumers. No component Tick occurs
	// between the edit and consumption of the queued runtime settings.
	IsolatedA.Axle->GetAxleBridge()->ConsumeInputs();
	IsolatedB.Axle->GetAxleBridge()->ConsumeInputs();
	TestTrue(TEXT("Bound axle A consumes the new differential config on its next macrostep"),
		FMath::IsNearlyEqual(IsolatedA.Axle->GetAxleBridge()->GetRuntimeForSimulation().Settings.DifferentialConfig.GearRatio, 4.5f));
	TestTrue(TEXT("Bound axle B consumes the new differential config on its next macrostep"),
		FMath::IsNearlyEqual(IsolatedB.Axle->GetAxleBridge()->GetRuntimeForSimulation().Settings.DifferentialConfig.DriveLockRatio, 0.9f));
	TestTrue(TEXT("Differential settings refresh preserves simulated axle velocity"),
		FMath::IsNearlyEqual(IsolatedA.Axle->GetAxleBridge()->GetRuntimeForSimulation().State.AxleAngularVelocity, 10.f)
		&& FMath::IsNearlyEqual(IsolatedB.Axle->GetAxleBridge()->GetRuntimeForSimulation().State.AxleAngularVelocity, 30.f));
	IsolatedA.Differential->SetConfig(OriginalConfigA);
	IsolatedB.Differential->SetConfig(OriginalConfigB);
	IsolatedA.Axle->GetAxleBridge()->ConsumeInputs();
	IsolatedB.Axle->GetAxleBridge()->ConsumeInputs();
	// Restore the deliberate GT/runtime difference used by the original shaft-feedback checks below.
	SimA.Settings.AxleConfig.TorqueWeight = 1.f;
	SimB.Settings.AxleConfig.TorqueWeight = 3.f;
	TArray<UVehicleAxleAssemblyComponent*> LegacyAxles{ LegacyA.Axle, LegacyB.Axle };
	TArray<UVehicleAxleAssemblyComponent*> IsolatedAxles{ IsolatedA.Axle, IsolatedB.Axle };
	float LegacyOmega = 0.f, LegacyInertia = 0.f, LegacyStiffness = 0.f;
	float IsolatedOmega = 0.f, IsolatedInertia = 0.f, IsolatedStiffness = 0.f;
	int32 LegacyDriveAxles = 0, IsolatedDriveAxles = 0;
	{
#if PHYSICS_THREAD_CONTEXT
		Chaos::FPhysicsThreadContextScope PhysicsContext(true);
#endif
		LegacyDriveAxles = LegacyCentral->SubstepTransferCase(LegacyAxles, 0.01f, 100.f, 0.f, 0.f, false,
			LegacyOmega, LegacyInertia, LegacyStiffness);
		IsolatedDriveAxles = IsolatedCentral->SubstepTransferCase(IsolatedAxles, 0.01f, 100.f, 0.f, 0.f, false,
			IsolatedOmega, IsolatedInertia, IsolatedStiffness);
	}
	TestEqual(TEXT("Both transfer cases find two powered axles"), IsolatedDriveAxles, 2);
	TestEqual(TEXT("Legacy transfer case finds two powered axles"), LegacyDriveAxles, 2);
	const FVehicleAxleSimState& ActualA = IsolatedA.Axle->GetAxleBridge()->GetRuntimeForSimulation().State;
	const FVehicleAxleSimState& ActualB = IsolatedB.Axle->GetAxleBridge()->GetRuntimeForSimulation().State;
	const FVehicleAxleSimState& LegacyStateA = LegacyA.Axle->GetAxleBridge()->GetRuntimeForSimulation().State;
	const FVehicleAxleSimState& LegacyStateB = LegacyB.Axle->GetAxleBridge()->GetRuntimeForSimulation().State;
	TestTrue(TEXT("Central diff drives bound axle A from runtime snapshot"),
		FMath::IsNearlyEqual(ActualA.AxleDriveTorque, LegacyStateA.AxleDriveTorque, 1.e-2f));
	TestTrue(TEXT("Central diff drives bound axle B from runtime snapshot"),
		FMath::IsNearlyEqual(ActualB.AxleDriveTorque, LegacyStateB.AxleDriveTorque, 1.e-2f));
	TestTrue(TEXT("Central diff output feedback matches legacy"),
		FMath::IsNearlyEqual(IsolatedOmega, LegacyOmega, 1.e-2f)
		&& FMath::IsNearlyEqual(IsolatedInertia, LegacyInertia, 1.e-2f)
		&& FMath::IsNearlyEqual(IsolatedStiffness, LegacyStiffness, 1.e-2f));

	// Predict the result a central differential would produce from the intentionally stale GT copies.
	// This confirms that the legacy-looking values are observably different from the path just exercised.
	const float StaleTotalInertia = 6.f + 3.f;
	const float StaleLockedOmega = FVehicleDifferentialSolver::CalculateLockedAngularVelocity(
		6.f * 80.f + 3.f * -10.f, StaleTotalInertia);
	const FVehicleTransferCaseAxleOutput StaleOutputA = FVehicleDifferentialSolver::CalculateTransferCaseAxleOutput(
		CentralConfig, 200.f, StaleLockedOmega, 80.f, 6.f, 9.f, 10.f, 0.01f);
	const FVehicleTransferCaseAxleOutput StaleOutputB = FVehicleDifferentialSolver::CalculateTransferCaseAxleOutput(
		CentralConfig, 200.f, StaleLockedOmega, -10.f, 3.f, 1.f, 10.f, 0.01f);
	TestTrue(TEXT("Bound central diff output does not match stale GT state/config prediction"),
		!FMath::IsNearlyEqual(ActualA.AxleDriveTorque, StaleOutputA.DriveTorque, 1.f)
		&& !FMath::IsNearlyEqual(ActualB.AxleDriveTorque, StaleOutputB.DriveTorque, 1.f));
	TestTrue(TEXT("Runtime torque weights produce a nontrivial split"),
		!FMath::IsNearlyEqual(ActualA.AxleDriveTorque, ActualB.AxleDriveTorque, 1.f));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleManualFrozenLifecycleTest,
	"KinetiForge.Lifecycle.ManualFrozenWheelAndAxle",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleManualFrozenLifecycleTest::RunTest(const FString& Parameters)
{
	using namespace VehicleAxleBindingTest;
	FWorldOwner Owner;
	UWorld* World = CreateWorld(*this, Owner);
	if (!World) return false;
	World->GetPhysicsScene()->GetSolver()->SetThreadingMode_External(Chaos::EThreadingModeTemp::TaskGraph);
	const auto FlushFrozenPhysics = [World]()
	{
		const FVector Gravity = FVector::ZeroVector;
		World->GetPhysicsScene()->SetUpForFrame(&Gravity, 1.f / 60.f, 0.f, 1.f / 60.f, 1.f / 60.f, 1, false);
		World->GetPhysicsScene()->StartFrame();
		World->GetPhysicsScene()->WaitPhysScenes();
		World->GetPhysicsScene()->EndFrame();
	};
	const FVehicle Vehicle = CreateVehicle(*World, FVector(0.f, 0.f, 800.f), false,
		EVehicleAxleSuspensionType::Independent);
	if (!TestNotNull(TEXT("Manual axle generated both wheels"), Vehicle.Right)) return false;
	UVehicleWheelComponent* Wheel = NewObject<UVehicleWheelComponent>(Vehicle.Actor, TEXT("FrozenManualWheel"));
	Vehicle.Actor->AddInstanceComponent(Wheel);
	Wheel->SetupAttachment(Vehicle.Body);
	Wheel->RegisterComponent();
	Wheel->SetSprungMass(300.f);
	Vehicle.Actor->DispatchBeginPlay();
	FlushFrozenPhysics();
	auto Host = FModuleManager::Get().GetModuleChecked<FKinetiForgeModule>(TEXT("KinetiForge"))
		.AcquirePhysicsScene(World->GetPhysicsScene());
	if (!TestNotNull(TEXT("Manual scene host exists"), Host.Get())) return false;
	int32 Calls = 0;
	bool bEveryCallOnGameThread = true;
	bool bImmediateFeedbackMatches = true;
	float DeltaTime = 1.f / 120.f;
	float Inertia = -1.f, Omega = -1.f, Stiffness = -1.f;
	auto Step = Host->RegisterStep([&](float, const FVehiclePhysicsExecutionContext&)
	{
		++Calls;
		bEveryCallOnGameThread &= IsInGameThread();
		Inertia = Omega = Stiffness = -1.f;
		Wheel->UpdatePhysics(DeltaTime, 100.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f);
		Vehicle.Axle->UpdatePhysics(DeltaTime, 100.f, 0.f, 0.f, 0.f, Inertia, Omega, Stiffness);
		if (Inertia > 0.f)
			bImmediateFeedbackMatches &= FMath::IsNearlyEqual(Vehicle.Axle->GetAngularVelocity(), Omega)
				&& FMath::IsNearlyEqual(Vehicle.Axle->GetTotalAxleInertia(), Inertia);
	}, true, EVehiclePhysicsExecutionLane::FrozenGameThread);
	if (!TestNotNull(TEXT("Real frozen callback registered"), Step.Get())) return false;
	ON_SCOPE_EXIT { Host->UnregisterStep(Step); };
	const auto WheelBridge = Wheel->GetWheelBridge();
	const auto AxleBridge = Vehicle.Axle->GetAxleBridge();
	const auto LeftBridge = Vehicle.Left->GetWheelBridge();
	const auto RightBridge = Vehicle.Right->GetWheelBridge();
	const auto CheckAdvance = [&](const TCHAR* Label, bool bWheelAdvances, bool bAxleAdvances)
	{
		const uint64 WheelSequence = WheelBridge->ReadResult().PublicationSequence;
		const uint64 AxleSequence = AxleBridge->ReadResult().PublicationSequence;
		const uint64 LeftSequence = LeftBridge->ReadResult().PublicationSequence;
		const uint64 RightSequence = RightBridge->ReadResult().PublicationSequence;
		const int32 BeforeCalls = Calls;
		FlushFrozenPhysics();
		TestEqual(FString(Label) + TEXT(" callback actually executes once"), Calls, BeforeCalls + 1);
		TestEqual(FString(Label) + TEXT(" single wheel publication"), WheelBridge->ReadResult().PublicationSequence,
			WheelSequence + uint64(bWheelAdvances));
		TestEqual(FString(Label) + TEXT(" axle publication"), AxleBridge->ReadResult().PublicationSequence,
			AxleSequence + uint64(bAxleAdvances));
		TestEqual(FString(Label) + TEXT(" left publication"), LeftBridge->ReadResult().PublicationSequence,
			LeftSequence + uint64(bAxleAdvances));
		TestEqual(FString(Label) + TEXT(" right publication"), RightBridge->ReadResult().PublicationSequence,
			RightSequence + uint64(bAxleAdvances));
		TestTrue(FString(Label) + TEXT(" rejected axle leaves outputs untouched; accepted axle returns feedback"),
			bAxleAdvances ? Inertia > 0.f && FMath::IsFinite(Omega) : Inertia == -1.f && Omega == -1.f && Stiffness == -1.f);
	};
	CheckAdvance(TEXT("Registered"), true, true);
	TestTrue(TEXT("Standalone wheel actually integrates torque"), Wheel->GetAngularVelocity() > 0.f);
	DeltaTime = 0.f;
	CheckAdvance(TEXT("Zero delta"), false, false);
	DeltaTime = -0.01f;
	CheckAdvance(TEXT("Negative delta"), false, false);
	DeltaTime = std::numeric_limits<float>::quiet_NaN();
	CheckAdvance(TEXT("Nonfinite delta"), false, false);
	DeltaTime = 1.f / 120.f;
	Wheel->UnregisterComponent();
	Vehicle.Left->UnregisterComponent();
	CheckAdvance(TEXT("Unregistered wheel rejects entire axle before right wheel advances"), false, false);
	Wheel->RegisterComponent();
	Vehicle.Left->RegisterComponent();
	CheckAdvance(TEXT("Reregistered"), true, true);
	Vehicle.Axle->UnregisterComponent();
	CheckAdvance(TEXT("Unregistered axle"), true, false);
	Vehicle.Axle->RegisterComponent();
	CheckAdvance(TEXT("Reregistered axle"), true, true);
	Vehicle.Body->DestroyPhysicsState();
	CheckAdvance(TEXT("Missing body"), false, false);
	Vehicle.Body->RecreatePhysicsState();
	CheckAdvance(TEXT("Recreated body"), true, true);
	FVehicleAxleBindingProbe::EndManualPlay(*Vehicle.Axle, *Wheel);
	CheckAdvance(TEXT("EndPlay closes manual admission"), false, false);
	Wheel->DestroyComponent();
	Vehicle.Axle->DestroyComponent();
	// Retained pointers model a stale external caller; no GC runs while these pointers are used.
	CheckAdvance(TEXT("Destroyed components"), false, false);
	TestTrue(TEXT("All manual calls ran in engine frozen GT window"), bEveryCallOnGameThread);
	TestTrue(TEXT("Accepted calls expose same-step feedback through old getters"), bImmediateFeedbackMatches);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
