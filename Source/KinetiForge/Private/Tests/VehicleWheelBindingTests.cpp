// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "VehicleWheelComponent.h"
#include "VehicleWheelBridge.h"
#include "VehicleAxleAssemblyComponent.h"
#include "VehicleAxleSolver.h"
#include "VehicleDifferentialComponent.h"
#include "VehicleUtilities.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "PBDRigidsSolver.h"
#include "Misc/AutomationTest.h"
#include "UObject/StrongObjectPtr.h"

namespace VehicleWheelBindingTest
{
	struct FWorldOwner
	{
		UWorld* World = nullptr;
		~FWorldOwner() { if (World) World->DestroyWorld(false); }
	};

	struct FRegisteredWheel
	{
		AActor* Actor = nullptr;
		UBoxComponent* Chassis = nullptr;
		UVehicleWheelComponent* Wheel = nullptr;
	};

	UWorld* CreateWorld(FAutomationTestBase& Test, FWorldOwner& Owner)
	{
		Owner.World = UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr,
			true, ERHIFeatureLevel::Num, nullptr, true);
		if (!Test.TestNotNull(TEXT("Wheel binding world created"), Owner.World)) return nullptr;
		Owner.World->InitWorld(UWorld::InitializationValues().AllowAudioPlayback(false)
			.CreatePhysicsScene(true).RequiresHitProxies(false).CreateNavigation(false)
			.CreateAISystem(false).ShouldSimulatePhysics(false).SetTransactional(false));
		if (!Test.TestNotNull(TEXT("Wheel binding physics scene created"), Owner.World->GetPhysicsScene())) return nullptr;
		Owner.World->GetPhysicsScene()->GetSolver()->DisableAsyncMode();
		Owner.World->GetPhysicsScene()->GetSolver()->SetThreadingMode_External(Chaos::EThreadingModeTemp::SingleThread);
		return Owner.World;
	}

	void FlushPhysicsScene(UWorld& World)
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
		UBoxComponent* Ground = NewObject<UBoxComponent>(Actor, TEXT("WheelBindingGround"));
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

	FRegisteredWheel CreateRegisteredWheel(UWorld& World, const FVector& Location, const bool bIsolated)
	{
		FRegisteredWheel Vehicle;
		Vehicle.Actor = World.SpawnActor<AActor>(Location, FRotator::ZeroRotator);
		if (!Vehicle.Actor) return Vehicle;
		Vehicle.Chassis = NewObject<UBoxComponent>(Vehicle.Actor, TEXT("WheelBindingChassis"));
		Vehicle.Actor->AddInstanceComponent(Vehicle.Chassis);
		Vehicle.Actor->SetRootComponent(Vehicle.Chassis);
		Vehicle.Chassis->SetBoxExtent(FVector(50.f));
		Vehicle.Chassis->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Vehicle.Chassis->SetCollisionObjectType(ECC_PhysicsBody);
		Vehicle.Chassis->SetCollisionResponseToAllChannels(ECR_Block);
		Vehicle.Chassis->SetSimulatePhysics(true);
		Vehicle.Chassis->SetEnableGravity(false);
		Vehicle.Chassis->RegisterComponent();
		Vehicle.Chassis->RecreatePhysicsState();
		Vehicle.Chassis->SetWorldLocation(Location, false, nullptr, ETeleportType::TeleportPhysics);

		Vehicle.Wheel = NewObject<UVehicleWheelComponent>(Vehicle.Actor, TEXT("WheelBindingWheel"));
		Vehicle.Actor->AddInstanceComponent(Vehicle.Wheel);
		Vehicle.Wheel->SetupAttachment(Vehicle.Chassis);
		Vehicle.Wheel->SetRelativeLocation(FVector(0.f, 0.f, -55.f));
		Vehicle.Wheel->bUseIsolatedWheelRuntime = bIsolated;
		FVehicleWheelConfig WheelConfig = Vehicle.Wheel->GetWheelConfig();
		WheelConfig.Radius = 33.f;
		WheelConfig.Width = 20.f;
		WheelConfig.Inertia = 1.3f;
		Vehicle.Wheel->SetWheelConfig(WheelConfig);
		FVehicleTireConfig TireConfig = Vehicle.Wheel->GetTireConfig();
		TireConfig.RelaxationLength = FVector2f(0.18f, 0.28f);
		Vehicle.Wheel->SetTireConfig(TireConfig);
		FVehicleSuspensionKinematicsConfig Kinematics = Vehicle.Wheel->GetSuspensionKinematicsConfig();
		Kinematics.SuspensionType = EVehicleIndependentSuspensionType::StraightLine;
		Kinematics.TraceChannel = ECC_WorldStatic;
		Kinematics.RayCastMode = EVehicleSuspensionRayCastMode::SphereTrace;
		Vehicle.Wheel->SetSuspensionKinematicsConfig(Kinematics);
		Vehicle.Wheel->RegisterComponent();
		Vehicle.Wheel->SetSprungMass(300.f);
		return Vehicle;
	}

	bool WheelStatesMatch(const FVehicleWheelSimState& A, const FVehicleWheelSimState& B, const float Tolerance = 1.e-3f)
	{
		return FMath::IsNearlyEqual(A.EffectiveInertia, B.EffectiveInertia, Tolerance)
			&& FMath::IsNearlyEqual(A.AngularVelocity, B.AngularVelocity, Tolerance)
			&& FMath::IsNearlyEqual(A.AngularAcceleration, B.AngularAcceleration, Tolerance)
			&& FMath::IsNearlyEqual(A.LongSlipVelocity, B.LongSlipVelocity, Tolerance)
			&& FMath::IsNearlyEqual(A.LatSlipVelocity, B.LatSlipVelocity, Tolerance)
			&& FMath::IsNearlyEqual(A.SlipRatio, B.SlipRatio, Tolerance)
			&& FMath::IsNearlyEqual(A.SlipAngle, B.SlipAngle, Tolerance)
			&& FMath::IsNearlyEqual(A.PredictedSlipRatio, B.PredictedSlipRatio, Tolerance)
			&& FMath::IsNearlyEqual(A.SignedCamberDegree, B.SignedCamberDegree, Tolerance)
			&& FMath::IsNearlyEqual(A.DynFrictionMultiplier, B.DynFrictionMultiplier, Tolerance)
			&& FMath::IsNearlyEqual(A.WheelLoad, B.WheelLoad, Tolerance)
			&& FMath::IsNearlyEqual(A.P4MotorTorque, B.P4MotorTorque, Tolerance)
			&& FMath::IsNearlyEqual(A.DriveTorque, B.DriveTorque, Tolerance)
			&& FMath::IsNearlyEqual(A.BrakeTorque, B.BrakeTorque, Tolerance)
			&& FMath::IsNearlyEqual(A.BrakeTorqueFromBrake, B.BrakeTorqueFromBrake, Tolerance)
			&& FMath::IsNearlyEqual(A.BrakeTorqueFromHandbrake, B.BrakeTorqueFromHandbrake, Tolerance)
			&& FMath::IsNearlyEqual(A.BrakeTorqueFromESP, B.BrakeTorqueFromESP, Tolerance)
			&& A.LocalLinearVelocity.Equals(B.LocalLinearVelocity, Tolerance)
			&& A.TransientSlip.Equals(B.TransientSlip, Tolerance)
			&& A.TireForce2D.Equals(B.TireForce2D, Tolerance)
			&& A.TireForce.Equals(B.TireForce, Tolerance)
			&& A.bIsLocked == B.bIsLocked && A.bABSTriggered == B.bABSTriggered;
	}

	bool SuspensionStatesMatch(const FVehicleSuspensionSimState& A, const FVehicleSuspensionSimState& B,
		const float Tolerance = 1.e-3f, const FVector& BWorldOffset = FVector::ZeroVector)
	{
		return A.bIsRightWheel == B.bIsRightWheel && A.bWheelOnGround == B.bWheelOnGround
			&& A.bHitGround == B.bHitGround && A.bValidKinematicsConfig == B.bValidKinematicsConfig
			&& FMath::IsNearlyEqual(A.SteeringAngle, B.SteeringAngle, Tolerance)
			&& FMath::IsNearlyEqual(A.StrutCurrentLength, B.StrutCurrentLength, Tolerance)
			&& FMath::IsNearlyEqual(A.StrutCurrentVelocity, B.StrutCurrentVelocity, Tolerance)
			&& FMath::IsNearlyEqual(A.TransientStrutLengthLimit, B.TransientStrutLengthLimit, Tolerance)
			&& FMath::IsNearlyEqual(A.StaticSprungMass, B.StaticSprungMass, Tolerance)
			&& FMath::IsNearlyEqual(A.VirtualUnsprungMass, B.VirtualUnsprungMass, Tolerance)
			&& FMath::IsNearlyEqual(A.EffectiveSprungMassNormal, B.EffectiveSprungMassNormal, Tolerance)
			&& FMath::IsNearlyEqual(A.EffectiveSprungMassLong, B.EffectiveSprungMassLong, Tolerance)
			&& FMath::IsNearlyEqual(A.EffectiveSprungMassLat, B.EffectiveSprungMassLat, Tolerance)
			&& FMath::IsNearlyEqual(A.ImpactFriction, B.ImpactFriction, Tolerance)
			&& FMath::IsNearlyEqual(A.ForceAlongImpactNormal, B.ForceAlongImpactNormal, Tolerance)
			&& FMath::IsNearlyEqual(A.AntiPitchScale, B.AntiPitchScale, Tolerance)
			&& FMath::IsNearlyEqual(A.AntiRollScale, B.AntiRollScale, Tolerance)
			&& A.WheelWorldRightVector.Equals(B.WheelWorldRightVector, Tolerance)
			&& A.HubChassisLocation.Equals(B.HubChassisLocation, Tolerance)
			&& A.HubChassisRotation.Equals(B.HubChassisRotation, Tolerance)
			&& A.TopMountChassisLocation.Equals(B.TopMountChassisLocation, Tolerance)
			&& A.UpperBallJointChassisLocation.Equals(B.UpperBallJointChassisLocation, Tolerance)
			&& A.LowerBallJointChassisLocation.Equals(B.LowerBallJointChassisLocation, Tolerance)
			&& A.SteerAxisChassisDirection.Equals(B.SteerAxisChassisDirection, Tolerance)
			&& A.ImpactWorldVelocity.Equals(B.ImpactWorldVelocity, Tolerance)
			&& A.ImpactWorldNormal.Equals(B.ImpactWorldNormal, Tolerance)
			&& A.ImpactWorldLocation.Equals(B.ImpactWorldLocation - (A.bHitGround ? BWorldOffset : FVector::ZeroVector), Tolerance);
	}
}

// This translation-unit-local probe intentionally differs from the axle-runtime test probe.
struct FVehicleWheelBindingProbe
{
	static void PrepareSimulationStepInputs(UVehicleWheelComponent& Wheel) { Wheel.PrepareSimulationStepInputs(); }
	static FVehicleAxleWheelInput CaptureSimulationTransmissionInput(UVehicleWheelComponent& Wheel) { return Wheel.CaptureSimulationTransmissionInput(); }
	static float DisplayPhysicsDeltaTimeInv(const UVehicleWheelComponent& Wheel) { return Wheel.GetPhysicsDeltaTimeInvForCaller(); }

	static void LinkAxle(UVehicleAxleAssemblyComponent& Axle, UVehicleWheelComponent* Left,
		UVehicleWheelComponent* Right, UVehicleDifferentialComponent* Differential)
	{
		Axle.LeftWheel = Left;
		Axle.RightWheel = Right;
		Axle.Differential = Differential;
		Axle.AxleLayout = EVehicleAxleLayout::TwoWheels;
		Axle.AxleSteeringConfig.bAffectedBySteering = false;
		Axle.AxleConfig.DriveShaftInertia = 0.4f;
		Axle.AxleConfig.DriveShaftTorsionalStiffness = 5200.f;
		Axle.AxleConfig.TorqueWeight = 1.f;
		Axle.TCSConfig.bTractionControlSystemEnabled = false;
		Axle.State = FVehicleAxleSimState();
		Axle.SubmitRuntimeSettings();
		Axle.GetAxleBridge()->ConsumeInputs();
	}

	static void LinkSolidAxle(UVehicleAxleAssemblyComponent& Axle, UVehicleWheelComponent* Left,
		UVehicleWheelComponent* Right, UVehicleDifferentialComponent* Differential)
	{
		LinkAxle(Axle, Left, Right, Differential);
		Axle.SuspensionType = EVehicleAxleSuspensionType::Solid;
		Axle.SubmitRuntimeSettings();
		Axle.GetAxleBridge()->ConsumeInputs();
	}
};

namespace VehicleWheelBindingTest
{
	struct FRegisteredSolidVehicle
	{
		AActor* Actor = nullptr;
		UBoxComponent* Chassis = nullptr;
		UVehicleWheelComponent* Left = nullptr;
		UVehicleWheelComponent* Right = nullptr;
	};

	FRegisteredSolidVehicle CreateRegisteredSolidVehicle(UWorld& World, const FVector& Location, bool bIsolated)
	{
		FRegisteredSolidVehicle Vehicle;
		Vehicle.Actor = World.SpawnActor<AActor>(Location, FRotator::ZeroRotator);
		if (!Vehicle.Actor) return Vehicle;
		Vehicle.Chassis = NewObject<UBoxComponent>(Vehicle.Actor, TEXT("SolidBindingChassis"));
		Vehicle.Actor->AddInstanceComponent(Vehicle.Chassis);
		Vehicle.Actor->SetRootComponent(Vehicle.Chassis);
		Vehicle.Chassis->SetBoxExtent(FVector(50.f));
		Vehicle.Chassis->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Vehicle.Chassis->SetCollisionObjectType(ECC_PhysicsBody);
		Vehicle.Chassis->SetCollisionResponseToAllChannels(ECR_Block);
		Vehicle.Chassis->SetSimulatePhysics(true);
		Vehicle.Chassis->SetEnableGravity(false);
		Vehicle.Chassis->RegisterComponent();
		Vehicle.Chassis->RecreatePhysicsState();
		Vehicle.Chassis->SetWorldLocation(Location, false, nullptr, ETeleportType::TeleportPhysics);

		Vehicle.Left = NewObject<UVehicleWheelComponent>(Vehicle.Actor, TEXT("SolidBindingLeft"));
		Vehicle.Actor->AddInstanceComponent(Vehicle.Left);
		Vehicle.Left->SetupAttachment(Vehicle.Chassis);
		Vehicle.Left->SetRelativeLocation(FVector(0.f, 0.f, -55.f));
		Vehicle.Left->bUseIsolatedWheelRuntime = bIsolated;
		FVehicleWheelConfig WheelConfig = Vehicle.Left->GetWheelConfig();
		WheelConfig.Radius = 33.f;
		WheelConfig.Width = 20.f;
		WheelConfig.Inertia = 1.3f;
		Vehicle.Left->SetWheelConfig(WheelConfig);
		FVehicleTireConfig TireConfig = Vehicle.Left->GetTireConfig();
		TireConfig.RelaxationLength = FVector2f(0.18f, 0.28f);
		Vehicle.Left->SetTireConfig(TireConfig);
		FVehicleSuspensionKinematicsConfig Kinematics = Vehicle.Left->GetSuspensionKinematicsConfig();
		Kinematics.SuspensionType = EVehicleIndependentSuspensionType::StraightLine;
		Kinematics.TraceChannel = ECC_WorldStatic;
		Kinematics.RayCastMode = EVehicleSuspensionRayCastMode::SphereTrace;
		Vehicle.Left->SetSuspensionKinematicsConfig(Kinematics);
		Vehicle.Left->RegisterComponent();
		// Recompute the initialized center-side wheel after assigning its actual left-side location.
		Vehicle.Left->SetRelativeLocation(FVector(0.f, -60.f, -55.f));
		Vehicle.Left->InitializeWheel();
		Vehicle.Left->SetSprungMass(300.f);

		Vehicle.Right = NewObject<UVehicleWheelComponent>(Vehicle.Actor, TEXT("SolidBindingRight"));
		Vehicle.Actor->AddInstanceComponent(Vehicle.Right);
		Vehicle.Right->SetupAttachment(Vehicle.Chassis);
		UVehicleWheelComponent::CopyWheelConfig(Vehicle.Left, Vehicle.Right);
		Vehicle.Right->bUseIsolatedWheelRuntime = bIsolated;
		Vehicle.Right->SetRelativeLocation(FVector(0.f, 60.f, -55.f));
		Vehicle.Right->RegisterComponent();
		Vehicle.Right->SetSprungMass(300.f);
		return Vehicle;
	}

	bool AxleStatesMatch(const FVehicleAxleSimState& A, const FVehicleAxleSimState& B, float Tolerance = 1.e-3f)
	{
		return A.NumOfWheels == B.NumOfWheels && A.NumOfWheelOnGround == B.NumOfWheelOnGround
			&& FMath::IsNearlyEqual(A.LeftWheelSteeringAngle, B.LeftWheelSteeringAngle, Tolerance)
			&& FMath::IsNearlyEqual(A.RightWheelSteeringAngle, B.RightWheelSteeringAngle, Tolerance)
			&& FMath::IsNearlyEqual(A.AxleDriveTorque, B.AxleDriveTorque, Tolerance)
			&& FMath::IsNearlyEqual(A.LeftDriveTorque, B.LeftDriveTorque, Tolerance)
			&& FMath::IsNearlyEqual(A.RightDriveTorque, B.RightDriveTorque, Tolerance)
			&& FMath::IsNearlyEqual(A.ReflectedInertiaOnWheel, B.ReflectedInertiaOnWheel, Tolerance)
			&& FMath::IsNearlyEqual(A.BrakeTorque, B.BrakeTorque, Tolerance)
			&& FMath::IsNearlyEqual(A.HandbrakeTorque, B.HandbrakeTorque, Tolerance)
			&& FMath::IsNearlyEqual(A.AxleEffectiveInertia, B.AxleEffectiveInertia, Tolerance)
			&& FMath::IsNearlyEqual(A.AxleAngularVelocity, B.AxleAngularVelocity, Tolerance)
			&& FMath::IsNearlyEqual(A.P3MotorTorque, B.P3MotorTorque, Tolerance)
			&& FMath::IsNearlyEqual(A.WheelBase, B.WheelBase, Tolerance)
			&& A.WorldLinearVelocity.Equals(B.WorldLinearVelocity, Tolerance)
			&& A.LocalLinearVelocity.Equals(B.LocalLinearVelocity, Tolerance)
			&& A.bTCSTriggered == B.bTCSTriggered;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleWheelRuntimeBindingTest,
	"KinetiForge.WheelRuntime.Binding.RegisteredComparison",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleWheelRuntimeBindingTest::RunTest(const FString& Parameters)
{
	using namespace VehicleWheelBindingTest;
	(void)Parameters;
	FWorldOwner WorldOwner;
	UWorld* World = CreateWorld(*this, WorldOwner);
	if (!World) return false;
	UBoxComponent* Ground = AddGround(*World);
	if (!TestNotNull(TEXT("Static ground created"), Ground)) return false;
	FRegisteredWheel Legacy = CreateRegisteredWheel(*World, FVector(0.f, 0.f, 130.f), false);
	FRegisteredWheel Isolated = CreateRegisteredWheel(*World, FVector(1000.f, 0.f, 130.f), true);
	if (!TestNotNull(TEXT("Legacy registered chassis"), Legacy.Chassis)
		|| !TestNotNull(TEXT("Legacy registered wheel"), Legacy.Wheel)
		|| !TestNotNull(TEXT("Isolated registered chassis"), Isolated.Chassis)
		|| !TestNotNull(TEXT("Isolated registered wheel"), Isolated.Wheel)) return false;
	FlushPhysicsScene(*World);
	Chaos::FRigidBodyHandle_Internal* LegacyHandle = UVehicleUtilities::GetInternalHandle(Legacy.Chassis);
	Chaos::FRigidBodyHandle_Internal* IsolatedHandle = UVehicleUtilities::GetInternalHandle(Isolated.Chassis);
	if (!TestNotNull(TEXT("Legacy Chaos chassis body"), LegacyHandle)
		|| !TestNotNull(TEXT("Isolated Chaos chassis body"), IsolatedHandle)) return false;
	TestTrue(TEXT("Registered wheel accepts idempotent bridge binding"), Isolated.Wheel->BindIsolatedWheelRuntime());
	TestTrue(TEXT("Every wheel owns a runtime bridge"), Legacy.Wheel->GetWheelBridge().IsValid()
		&& Isolated.Wheel->GetWheelBridge().IsValid());

	int32 ContactSteps = 0;
	int32 AirSteps = 0;
	const int32 Steps = 15;
	for (int32 Step = 0; Step < Steps; ++Step)
	{
		const float Height = Step < 4 ? 130.f : Step < 10 ? 80.f : 2000.f;
		Legacy.Chassis->SetWorldLocation(FVector(0.f, 0.f, Height), false, nullptr, ETeleportType::TeleportPhysics);
		Isolated.Chassis->SetWorldLocation(FVector(1000.f, 0.f, Height), false, nullptr, ETeleportType::TeleportPhysics);
		FlushPhysicsScene(*World); // Transfer GT teleports before using internal physics handles.
		if (Step == 7)
		{
			FVehicleWheelConfig NewWheelConfig = Legacy.Wheel->GetWheelConfig();
			NewWheelConfig.Radius = 37.f;
			Legacy.Wheel->SetWheelConfig(NewWheelConfig);
			Isolated.Wheel->SetWheelConfig(NewWheelConfig);
			FVehicleTireConfig NewTireConfig = Legacy.Wheel->GetTireConfig();
			NewTireConfig.FrictionMultiplier = 1.35f;
			Legacy.Wheel->SetTireConfig(NewTireConfig);
			Isolated.Wheel->SetTireConfig(NewTireConfig);
		}
		const float P4 = Step < 8 ? 8.f : -3.f;
		const float Esp = Step < 8 ? 2.f : 6.f;
		const float Mass = Step < 8 ? 300.f : 360.f;
		Legacy.Wheel->SetP4MotorTorque(P4);
		Isolated.Wheel->SetP4MotorTorque(P4);
		Legacy.Wheel->SetEspBrakeTorque(Esp);
		Isolated.Wheel->SetEspBrakeTorque(Esp);
		Legacy.Wheel->SetSprungMass(Mass);
		Isolated.Wheel->SetSprungMass(Mass);

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
			Legacy.Wheel->UpdatePhysics(1.f / 120.f, 250.f, 0.22f, 0.12f, 0.f, 0.f, 0.f, 0.15f);
			Isolated.Wheel->UpdatePhysics(1.f / 120.f, 250.f, 0.22f, 0.12f, 0.f, 0.f, 0.f, 0.15f);
			LegacyImpulseAfter = LegacyHandle->LinearImpulse();
			LegacyAngularAfter = LegacyHandle->AngularImpulse();
			IsolatedImpulseAfter = IsolatedHandle->LinearImpulse();
			IsolatedAngularAfter = IsolatedHandle->AngularImpulse();
		}
		const FVehicleWheelRuntime& Runtime = Isolated.Wheel->GetWheelBridge()->GetRuntimeForSimulation();
		TestTrue(FString::Printf(TEXT("Step %d wheel state matches peer adapter"), Step),
			WheelStatesMatch(Legacy.Wheel->GetWheelState(), Runtime.WheelState));
		TestTrue(FString::Printf(TEXT("Step %d suspension state matches peer adapter"), Step),
			SuspensionStatesMatch(Legacy.Wheel->GetSuspensionState(), Runtime.SuspensionState, 1.e-3f, FVector(1000.f, 0.f, 0.f)));
		const bool bHitGround = Runtime.SuspensionState.bHitGround;
		ContactSteps += bHitGround ? 1 : 0;
		AirSteps += bHitGround ? 0 : 1;
		const FVehicleSuspensionHitResult PublishedContact = Isolated.Wheel->GetWheelBridge()->ReadContactResult();
		if (bHitGround)
		{
			TestTrue(FString::Printf(TEXT("Step %d contact resolves to real floor component"), Step),
				PublishedContact.Component.Get() == Ground);
		}
		if (Step >= 10)
		{
			TestFalse(FString::Printf(TEXT("Step %d high chassis is airborne"), Step), bHitGround);
		}
		const FVector LegacyImpulseDelta = LegacyImpulseAfter - LegacyImpulseBefore;
		const FVector IsolatedImpulseDelta = IsolatedImpulseAfter - IsolatedImpulseBefore;
		const FVector LegacyAngularDelta = LegacyAngularAfter - LegacyAngularBefore;
		const FVector IsolatedAngularDelta = IsolatedAngularAfter - IsolatedAngularBefore;
		TestTrue(FString::Printf(TEXT("Step %d chassis impulse matches"), Step), LegacyImpulseDelta.Equals(IsolatedImpulseDelta, 1.e-2f));
		TestTrue(FString::Printf(TEXT("Step %d chassis angular impulse matches"), Step), LegacyAngularDelta.Equals(IsolatedAngularDelta, 1.e-1f));
		const FVehicleWheelResult Published = Isolated.Wheel->GetWheelBridge()->ReadResult();
		TestTrue(FString::Printf(TEXT("Step %d publishes current wheel result"), Step),
			WheelStatesMatch(Published.WheelState, Runtime.WheelState));
		TestTrue(FString::Printf(TEXT("Step %d manual update publishes to GT before returning"), Step),
			WheelStatesMatch(Isolated.Wheel->GetWheelState(), Runtime.WheelState)
			&& SuspensionStatesMatch(Isolated.Wheel->GetSuspensionState(), Runtime.SuspensionState));
		TestTrue(FString::Printf(TEXT("Step %d GT contact getter receives published contact"), Step),
			Isolated.Wheel->GetSuspensionHitResult().Component == PublishedContact.Component);
		TestTrue(FString::Printf(TEXT("Step %d GT macro delta getter receives published value"), Step),
		FMath::IsNearlyEqual(FVehicleWheelBindingProbe::DisplayPhysicsDeltaTimeInv(*Isolated.Wheel),
			UVehicleUtilities::SafeDivide(1.f, Published.MacroDeltaTime)));
	}
	TestTrue(TEXT("Binding comparison includes real ground contact"), ContactSteps > 0);
	TestTrue(TEXT("Binding comparison includes airborne steps"), AirSteps > 0);

	TSharedPtr<FVehicleWheelBridge, ESPMode::ThreadSafe> Bridge = Isolated.Wheel->GetWheelBridge();
	FVehicleWheelRuntime& CommandRuntime = Bridge->GetRuntimeForSimulation();
	const float PublishedOmegaBeforeExternalPublish = Isolated.Wheel->GetWheelState().AngularVelocity;
	CommandRuntime.WheelState.AngularVelocity = PublishedOmegaBeforeExternalPublish + 23.f;
	Bridge->PublishResult();
	TestEqual(TEXT("Runtime publication does not mutate GT getters before refresh"),
		Isolated.Wheel->GetWheelState().AngularVelocity, PublishedOmegaBeforeExternalPublish);
	Isolated.Wheel->RefreshPublishedWheelState();
	TestTrue(TEXT("Explicit refresh copies published runtime state to GT"),
		FMath::IsNearlyEqual(Isolated.Wheel->GetWheelState().AngularVelocity, CommandRuntime.WheelState.AngularVelocity));
	Bridge->PublishResult(); // Leave a newer publication unread when GT submits commands.
	Isolated.Wheel->SetP4MotorTorque(99.f);
	Isolated.Wheel->SetEspBrakeTorque(11.f);
	Isolated.Wheel->SetSprungMass(200.f);
	Isolated.Wheel->RefreshPublishedWheelState();
	TestEqual(TEXT("Old publication does not undo pending P4"), Isolated.Wheel->GetP4MotorTorque(), 99.f);
	TestEqual(TEXT("Old publication does not undo pending ESP"), Isolated.Wheel->GetEspBrakeTorque(), 11.f);
	TestEqual(TEXT("Old publication does not undo pending mass"), Isolated.Wheel->GetSprungMass(), 200.f);
	Bridge->ConsumeInputs();
	CommandRuntime.WheelState.P4MotorTorque = 107.f; // Simulation controller changes the consumed command.
	Bridge->PublishResult();
	Isolated.Wheel->RefreshPublishedWheelState();
	TestEqual(TEXT("Acknowledged P4 request no longer hides simulation controller output"), Isolated.Wheel->GetP4MotorTorque(), 107.f);
	Bridge->PublishResult(); // Pending preview must also survive an unread, rather than just old, publication.

	// GT pose preview must survive an already-consumed publication; InitializeWheel queues a reset,
	// which the next macro boundary consumes before simulation can publish another result.
	Isolated.Wheel->ApplySuspensionStateDirect(0.25f, 13.f);
	const FVehicleSuspensionSimState PreviewState = Isolated.Wheel->GetSuspensionState();
	Isolated.Wheel->RefreshPublishedWheelState();
	TestTrue(TEXT("A previously consumed publication does not roll back direct pose preview"),
		SuspensionStatesMatch(PreviewState, Isolated.Wheel->GetSuspensionState(), 0.f));

	FVehicleWheelConfig ResetWheelConfig = Isolated.Wheel->GetWheelConfig();
	ResetWheelConfig.Radius = 41.f;
	Isolated.Wheel->SetWheelConfig(ResetWheelConfig);
	Legacy.Wheel->SetWheelConfig(ResetWheelConfig);
	Isolated.Wheel->InitializeWheel();
	Legacy.Wheel->InitializeWheel();
	const FVehicleSuspensionSimState ResetState = Isolated.Wheel->GetSuspensionState();
	Isolated.Wheel->RefreshPublishedWheelState();
	TestTrue(TEXT("Unconsumed pose reset blocks an older publication"),
		SuspensionStatesMatch(ResetState, Isolated.Wheel->GetSuspensionState(), 0.f));
	FVehicleWheelBindingProbe::PrepareSimulationStepInputs(*Isolated.Wheel);
	const FVehicleWheelRuntime& ResetRuntime = Isolated.Wheel->GetWheelBridge()->GetRuntimeForSimulation();
	TestTrue(TEXT("Macro boundary consumes new config snapshot"), FMath::IsNearlyEqual(ResetRuntime.Settings.WheelConfig.Radius, 41.f));
	TestTrue(TEXT("Macro boundary consumes explicit suspension reset"),
		SuspensionStatesMatch(ResetState, ResetRuntime.SuspensionState, 0.f));
	TestTrue(TEXT("Reset also consumes latest P4/ESP values"),
		FMath::IsNearlyEqual(ResetRuntime.WheelState.P4MotorTorque, Isolated.Wheel->GetP4MotorTorque())
		&& FMath::IsNearlyEqual(ResetRuntime.WheelState.BrakeTorqueFromESP, Isolated.Wheel->GetEspBrakeTorque()));

	FRegisteredWheel Automatic = CreateRegisteredWheel(*World, FVector(2000.f, 0.f, 1000.f), true);
	if (!TestNotNull(TEXT("Automatic binding wheel created"), Automatic.Wheel)) return false;
	Automatic.Actor->DispatchBeginPlay();
	TestTrue(TEXT("Pre-BeginPlay opt-in binds automatically"), Automatic.Wheel->GetWheelBridge().IsValid());
	TestTrue(TEXT("Automatic binding preserves component BeginPlay"), Automatic.Wheel->HasBegunPlay());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleWheelRuntimeAxleFeedbackBindingTest,
	"KinetiForge.WheelRuntime.Binding.AxleUsesSimulationFeedback",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleWheelRuntimeAxleFeedbackBindingTest::RunTest(const FString& Parameters)
{
	using namespace VehicleWheelBindingTest;
	(void)Parameters;
	FWorldOwner WorldOwner;
	UWorld* World = CreateWorld(*this, WorldOwner);
	if (!World) return false;
	if (!TestNotNull(TEXT("Static ground created"), AddGround(*World))) return false;

	FRegisteredWheel Left = CreateRegisteredWheel(*World, FVector(1300.f, -70.f, 130.f), true);
	FRegisteredWheel Right = CreateRegisteredWheel(*World, FVector(1300.f, 70.f, 130.f), true);
	if (!TestNotNull(TEXT("Left chassis registered"), Left.Chassis)
		|| !TestNotNull(TEXT("Left wheel registered"), Left.Wheel)
		|| !TestNotNull(TEXT("Right chassis registered"), Right.Chassis)
		|| !TestNotNull(TEXT("Right wheel registered"), Right.Wheel)) return false;
	FlushPhysicsScene(*World);
	TestTrue(TEXT("Left wheel bridge binding is idempotent"), Left.Wheel->BindIsolatedWheelRuntime());
	TestTrue(TEXT("Right wheel bridge binding is idempotent"), Right.Wheel->BindIsolatedWheelRuntime());

	UVehicleDifferentialComponent* Differential = NewObject<UVehicleDifferentialComponent>();
	UVehicleAxleAssemblyComponent* Axle = NewObject<UVehicleAxleAssemblyComponent>();
	TStrongObjectPtr<UVehicleDifferentialComponent> DifferentialGuard(Differential);
	TStrongObjectPtr<UVehicleAxleAssemblyComponent> AxleGuard(Axle);
	if (!TestNotNull(TEXT("Differential allocated"), Differential)
		|| !TestNotNull(TEXT("Axle allocated"), Axle)) return false;
	FVehicleLimitedSlipDifferentialConfig DiffConfig;
	DiffConfig.GearRatio = 3.f;
	DiffConfig.DriveLockRatio = 0.9f;
	DiffConfig.CoastLockRatio = 0.4f;
	Differential->SetConfig(DiffConfig);
	FVehicleWheelBindingProbe::LinkAxle(*Axle, Left.Wheel, Right.Wheel, Differential);

	int32 PublishedDisplaySteps = 0;
	for (int32 Step = 0; Step < 2; ++Step)
	{
		const float LeftOmega = Step == 0 ? 12.f : 18.f;
		const float RightOmega = Step == 0 ? 42.f : 51.f;
		Left.Wheel->GetWheelBridge()->GetRuntimeForSimulation().WheelState.AngularVelocity = LeftOmega;
		Right.Wheel->GetWheelBridge()->GetRuntimeForSimulation().WheelState.AngularVelocity = RightOmega;
		const float LeftDisplayOmega = Left.Wheel->GetWheelState().AngularVelocity;
		const float RightDisplayOmega = Right.Wheel->GetWheelState().AngularVelocity;
		TestFalse(FString::Printf(TEXT("Step %d left display remains stale before axle step"), Step),
			FMath::IsNearlyEqual(LeftDisplayOmega, LeftOmega, 1.e-3f));
		TestFalse(FString::Printf(TEXT("Step %d right display remains stale before axle step"), Step),
			FMath::IsNearlyEqual(RightDisplayOmega, RightOmega, 1.e-3f));

		constexpr float Dt = 1.f / 120.f;
		float OutInertia = 0.f, OutOmega = 0.f, OutStiffness = 0.f;
		{
#if PHYSICS_THREAD_CONTEXT
			Chaos::FPhysicsThreadContextScope PhysicsContext(true);
#endif
			Axle->PreStepAxle(Dt, 0.f);
			FVehicleAxleSimState RuntimeExpected = Axle->GetAxleBridge()->GetRuntimeForSimulation().State;
			const FVehicleAxleWheelInput RuntimeLeft = FVehicleWheelBindingProbe::CaptureSimulationTransmissionInput(*Left.Wheel);
			const FVehicleAxleWheelInput RuntimeRight = FVehicleWheelBindingProbe::CaptureSimulationTransmissionInput(*Right.Wheel);
			FVehicleAxleSolver::PrepareTransmissionStep(RuntimeExpected, Axle->GetAxleConfig(), Axle->GetTCSConfig(),
				EVehicleAxleLayout::TwoWheels, &DiffConfig, &RuntimeLeft, &RuntimeRight,
				Dt, 600.f, 0.f, 0.f);

			FVehicleAxleSimState DisplayExpected = Axle->GetAxleBridge()->GetRuntimeForSimulation().State;
			const FVehicleWheelConfig LeftConfig = Left.Wheel->GetWheelConfig();
			const FVehicleWheelConfig RightConfig = Right.Wheel->GetWheelConfig();
			const FVehicleWheelSimState& LeftDisplayState = Left.Wheel->GetWheelState();
			const FVehicleWheelSimState& RightDisplayState = Right.Wheel->GetWheelState();
			const FVehicleAxleWheelInput DisplayLeft{ LeftDisplayState.AngularVelocity, LeftDisplayState.EffectiveInertia,
				LeftConfig.Inertia, LeftDisplayState.PredictedSlipRatio };
			const FVehicleAxleWheelInput DisplayRight{ RightDisplayState.AngularVelocity, RightDisplayState.EffectiveInertia,
				RightConfig.Inertia, RightDisplayState.PredictedSlipRatio };
			FVehicleAxleSolver::PrepareTransmissionStep(DisplayExpected, Axle->GetAxleConfig(), Axle->GetTCSConfig(),
				EVehicleAxleLayout::TwoWheels, &DiffConfig, &DisplayLeft, &DisplayRight,
				Dt, 600.f, 0.f, 0.f);

			Axle->SubstepAxle(Dt, 600.f, 0.f, 0.f, OutInertia, OutOmega, OutStiffness);
			const FVehicleAxleSimState& Actual = Axle->GetAxleBridge()->GetRuntimeForSimulation().State;
			TestTrue(FString::Printf(TEXT("Step %d left axle torque follows runtime wheel input"), Step),
				FMath::IsNearlyEqual(Actual.LeftDriveTorque, RuntimeExpected.LeftDriveTorque, 1.e-3f));
			TestTrue(FString::Printf(TEXT("Step %d right axle torque follows runtime wheel input"), Step),
				FMath::IsNearlyEqual(Actual.RightDriveTorque, RuntimeExpected.RightDriveTorque, 1.e-3f));
			TestTrue(FString::Printf(TEXT("Step %d torque differs from stale display prediction"), Step),
				!FMath::IsNearlyEqual(Actual.LeftDriveTorque, DisplayExpected.LeftDriveTorque, 1.e-2f)
				|| !FMath::IsNearlyEqual(Actual.RightDriveTorque, DisplayExpected.RightDriveTorque, 1.e-2f));
			Axle->PostStepAxle();
		}

		TestTrue(FString::Printf(TEXT("Step %d axle post-step publishes wheel state to GT"), Step),
			FMath::IsNearlyEqual(Left.Wheel->GetWheelState().AngularVelocity,
				Left.Wheel->GetWheelBridge()->GetRuntimeForSimulation().WheelState.AngularVelocity, 1.e-3f)
			&& FMath::IsNearlyEqual(Right.Wheel->GetWheelState().AngularVelocity,
				Right.Wheel->GetWheelBridge()->GetRuntimeForSimulation().WheelState.AngularVelocity, 1.e-3f));
		++PublishedDisplaySteps;
	}
	TestEqual(TEXT("Axle publishes both completed substeps to GT"), PublishedDisplaySteps, 2);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleWheelRuntimeSolidAxleBindingTest,
	"KinetiForge.WheelRuntime.Binding.RegisteredSolidAxleComparison",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleWheelRuntimeSolidAxleBindingTest::RunTest(const FString& Parameters)
{
	using namespace VehicleWheelBindingTest;
	(void)Parameters;
	FWorldOwner WorldOwner;
	UWorld* World = CreateWorld(*this, WorldOwner);
	if (!World) return false;
	if (!TestNotNull(TEXT("Static ground created"), AddGround(*World))) return false;

	FRegisteredSolidVehicle Legacy = CreateRegisteredSolidVehicle(*World, FVector(0.f, 0.f, 80.f), false);
	FRegisteredSolidVehicle Isolated = CreateRegisteredSolidVehicle(*World, FVector(1000.f, 0.f, 80.f), true);
	if (!TestNotNull(TEXT("Legacy solid chassis"), Legacy.Chassis)
		|| !TestNotNull(TEXT("Legacy left wheel"), Legacy.Left)
		|| !TestNotNull(TEXT("Legacy right wheel"), Legacy.Right)
		|| !TestNotNull(TEXT("Isolated solid chassis"), Isolated.Chassis)
		|| !TestNotNull(TEXT("Isolated left wheel"), Isolated.Left)
		|| !TestNotNull(TEXT("Isolated right wheel"), Isolated.Right)) return false;
	FlushPhysicsScene(*World);
	Chaos::FRigidBodyHandle_Internal* LegacyHandle = UVehicleUtilities::GetInternalHandle(Legacy.Chassis);
	Chaos::FRigidBodyHandle_Internal* IsolatedHandle = UVehicleUtilities::GetInternalHandle(Isolated.Chassis);
	if (!TestNotNull(TEXT("Legacy solid body handle"), LegacyHandle)
		|| !TestNotNull(TEXT("Isolated solid body handle"), IsolatedHandle)) return false;
	TestTrue(TEXT("Legacy wheels initialize correct axle sides"),
		!Legacy.Left->GetSuspensionState().bIsRightWheel && Legacy.Right->GetSuspensionState().bIsRightWheel);
	TestTrue(TEXT("Isolated wheels initialize correct axle sides"),
		!Isolated.Left->GetSuspensionState().bIsRightWheel && Isolated.Right->GetSuspensionState().bIsRightWheel);
	TestTrue(TEXT("Bind left wheel bridge idempotently"), Isolated.Left->BindIsolatedWheelRuntime());
	TestTrue(TEXT("Bind right wheel bridge idempotently"), Isolated.Right->BindIsolatedWheelRuntime());

	UVehicleDifferentialComponent* LegacyDiff = NewObject<UVehicleDifferentialComponent>(Legacy.Actor);
	UVehicleDifferentialComponent* IsolatedDiff = NewObject<UVehicleDifferentialComponent>(Isolated.Actor);
	UVehicleAxleAssemblyComponent* LegacyAxle = NewObject<UVehicleAxleAssemblyComponent>(Legacy.Actor);
	UVehicleAxleAssemblyComponent* IsolatedAxle = NewObject<UVehicleAxleAssemblyComponent>(Isolated.Actor);
	TStrongObjectPtr<UVehicleDifferentialComponent> LegacyDiffGuard(LegacyDiff), IsolatedDiffGuard(IsolatedDiff);
	TStrongObjectPtr<UVehicleAxleAssemblyComponent> LegacyAxleGuard(LegacyAxle), IsolatedAxleGuard(IsolatedAxle);
	if (!TestNotNull(TEXT("Legacy differential"), LegacyDiff)
		|| !TestNotNull(TEXT("Isolated differential"), IsolatedDiff)
		|| !TestNotNull(TEXT("Legacy axle"), LegacyAxle)
		|| !TestNotNull(TEXT("Isolated axle"), IsolatedAxle)) return false;
	FVehicleLimitedSlipDifferentialConfig DiffConfig;
	DiffConfig.GearRatio = 3.f;
	DiffConfig.DriveLockRatio = 0.65f;
	DiffConfig.CoastLockRatio = 0.3f;
	LegacyDiff->SetConfig(DiffConfig);
	IsolatedDiff->SetConfig(DiffConfig);
	FVehicleWheelBindingProbe::LinkSolidAxle(*LegacyAxle, Legacy.Left, Legacy.Right, LegacyDiff);
	FVehicleWheelBindingProbe::LinkSolidAxle(*IsolatedAxle, Isolated.Left, Isolated.Right, IsolatedDiff);

	int32 ContactWheelSteps = 0;
	int32 AirWheelSteps = 0;
	for (int32 Step = 0; Step < 6; ++Step)
	{
		const float Height = Step < 4 ? 80.f : 2000.f;
		Legacy.Chassis->SetWorldLocation(FVector(0.f, 0.f, Height), false, nullptr, ETeleportType::TeleportPhysics);
		Isolated.Chassis->SetWorldLocation(FVector(1000.f, 0.f, Height), false, nullptr, ETeleportType::TeleportPhysics);
		FlushPhysicsScene(*World);
		Legacy.Left->SetSprungMass(300.f);
		Legacy.Right->SetSprungMass(300.f);
		Isolated.Left->SetSprungMass(300.f);
		Isolated.Right->SetSprungMass(300.f);

		FVector LegacyImpulseBefore, LegacyAngularBefore, IsolatedImpulseBefore, IsolatedAngularBefore;
		FVector LegacyImpulseAfter, LegacyAngularAfter, IsolatedImpulseAfter, IsolatedAngularAfter;
		float LegacyInertia = 0.f, LegacyOmega = 0.f, LegacyStiffness = 0.f;
		float IsolatedInertia = 0.f, IsolatedOmega = 0.f, IsolatedStiffness = 0.f;
		{
#if PHYSICS_THREAD_CONTEXT
			Chaos::FPhysicsThreadContextScope PhysicsContext(true);
#endif
			LegacyImpulseBefore = LegacyHandle->LinearImpulse();
			LegacyAngularBefore = LegacyHandle->AngularImpulse();
			IsolatedImpulseBefore = IsolatedHandle->LinearImpulse();
			IsolatedAngularBefore = IsolatedHandle->AngularImpulse();
			LegacyAxle->PreStepAxle(1.f / 120.f, 0.f);
			IsolatedAxle->PreStepAxle(1.f / 120.f, 0.f);
			LegacyAxle->SubstepAxle(1.f / 120.f, 350.f, 0.15f, 0.1f, LegacyInertia, LegacyOmega, LegacyStiffness);
			IsolatedAxle->SubstepAxle(1.f / 120.f, 350.f, 0.15f, 0.1f, IsolatedInertia, IsolatedOmega, IsolatedStiffness);
			LegacyAxle->PostStepAxle();
			IsolatedAxle->PostStepAxle();
			LegacyImpulseAfter = LegacyHandle->LinearImpulse();
			LegacyAngularAfter = LegacyHandle->AngularImpulse();
			IsolatedImpulseAfter = IsolatedHandle->LinearImpulse();
			IsolatedAngularAfter = IsolatedHandle->AngularImpulse();
		}

		const FVehicleWheelRuntime& LRuntime = Isolated.Left->GetWheelBridge()->GetRuntimeForSimulation();
		const FVehicleWheelRuntime& RRuntime = Isolated.Right->GetWheelBridge()->GetRuntimeForSimulation();
		TestTrue(FString::Printf(TEXT("Step %d left wheel state matches peer solid adapter"), Step),
			WheelStatesMatch(Legacy.Left->GetWheelState(), LRuntime.WheelState, 1.e-2f));
		TestTrue(FString::Printf(TEXT("Step %d right wheel state matches peer solid adapter"), Step),
			WheelStatesMatch(Legacy.Right->GetWheelState(), RRuntime.WheelState, 1.e-2f));
		TestTrue(FString::Printf(TEXT("Step %d left suspension state matches"), Step),
			SuspensionStatesMatch(Legacy.Left->GetSuspensionState(), LRuntime.SuspensionState, 1.e-2f, FVector(1000.f, 0.f, 0.f)));
		TestTrue(FString::Printf(TEXT("Step %d right suspension state matches"), Step),
			SuspensionStatesMatch(Legacy.Right->GetSuspensionState(), RRuntime.SuspensionState, 1.e-2f, FVector(1000.f, 0.f, 0.f)));
		TestTrue(FString::Printf(TEXT("Step %d axle state matches"), Step),
			AxleStatesMatch(LegacyAxle->GetAxleState(), IsolatedAxle->GetAxleState(), 1.e-2f));
		TestTrue(FString::Printf(TEXT("Step %d transmission feedback matches"), Step),
			FMath::IsNearlyEqual(LegacyInertia, IsolatedInertia, 1.e-2f)
			&& FMath::IsNearlyEqual(LegacyOmega, IsolatedOmega, 1.e-2f)
			&& FMath::IsNearlyEqual(LegacyStiffness, IsolatedStiffness, 1.e-2f));
		TestTrue(FString::Printf(TEXT("Step %d solid chassis impulse matches"), Step),
			(LegacyImpulseAfter - LegacyImpulseBefore).Equals(IsolatedImpulseAfter - IsolatedImpulseBefore, 1.e-2f));
		TestTrue(FString::Printf(TEXT("Step %d solid chassis angular impulse matches"), Step),
			(LegacyAngularAfter - LegacyAngularBefore).Equals(IsolatedAngularAfter - IsolatedAngularBefore, 1.e-1f));
		ContactWheelSteps += (LRuntime.SuspensionState.bHitGround && RRuntime.SuspensionState.bHitGround) ? 1 : 0;
		AirWheelSteps += (!LRuntime.SuspensionState.bHitGround && !RRuntime.SuspensionState.bHitGround) ? 1 : 0;
		Isolated.Left->RefreshPublishedWheelState();
		Isolated.Right->RefreshPublishedWheelState();
	}
	TestTrue(TEXT("Solid comparison includes both wheels in real contact"), ContactWheelSteps > 0);
	TestTrue(TEXT("Solid comparison includes both wheels airborne"), AirWheelSteps > 0);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
