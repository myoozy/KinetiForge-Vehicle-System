// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "VehicleSuspensionSolver.h"
#include "VehicleWheelPhysics.h"
#include "VehicleWheelComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/AutomationTest.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Physics/Experimental/PhysScene_Chaos.h"
#include "PBDRigidsSolver.h"

namespace VehicleSuspensionStageTest
{
	struct FWorldOwner
	{
		UWorld* World = nullptr;
		~FWorldOwner() { if (World) World->DestroyWorld(false); }
	};

	struct FWheelFixture
	{
		AActor* Owner = nullptr;
		UBoxComponent* Root = nullptr;
		UVehicleWheelComponent* Wheel = nullptr;
		FVehicleSuspensionSolver Solver;
		FTransform RelativeTransform = FTransform::Identity;
	};

	UWorld* CreateWorld(FAutomationTestBase& Test, FWorldOwner& Owner)
	{
		Owner.World = UWorld::CreateWorld(EWorldType::Game, false, NAME_None, nullptr, true, ERHIFeatureLevel::Num, nullptr, true);
		if (!Test.TestNotNull(TEXT("Suspension stage world created"), Owner.World)) return nullptr;
		Owner.World->InitWorld(UWorld::InitializationValues().AllowAudioPlayback(false)
			.CreatePhysicsScene(true).RequiresHitProxies(false).CreateNavigation(false)
			.CreateAISystem(false).ShouldSimulatePhysics(false).SetTransactional(false));
		if (!Test.TestNotNull(TEXT("Suspension stage physics scene created"), Owner.World->GetPhysicsScene())) return nullptr;
		Owner.World->GetPhysicsScene()->GetSolver()->DisableAsyncMode();
		Owner.World->GetPhysicsScene()->GetSolver()->SetThreadingMode_External(Chaos::EThreadingModeTemp::SingleThread);
		return Owner.World;
	}

	UBoxComponent* AddGround(UWorld& World, const TCHAR* Name, const FVector& Location, const bool bDynamic)
	{
		AActor* Actor = World.SpawnActor<AActor>(Location, FRotator::ZeroRotator);
		if (!Actor) return nullptr;
		UBoxComponent* Ground = NewObject<UBoxComponent>(Actor, Name);
		Actor->AddInstanceComponent(Ground);
		Actor->SetRootComponent(Ground);
		Ground->SetWorldLocation(Location);
		Ground->SetBoxExtent(FVector(400.f, 400.f, 20.f));
		Ground->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Ground->SetCollisionObjectType(bDynamic ? ECC_PhysicsBody : ECC_WorldStatic);
		Ground->SetCollisionResponseToAllChannels(ECR_Block);
		Ground->SetSimulatePhysics(bDynamic);
		Ground->SetEnableGravity(false);
		if (!bDynamic) Ground->SetMobility(EComponentMobility::Static);
		Ground->RegisterComponent();
		Ground->RecreatePhysicsState();
		return Ground;
	}

	void FlushPhysicsScene(UWorld& World)
	{
		const FVector Gravity = FVector::ZeroVector;
		World.GetPhysicsScene()->SetUpForFrame(&Gravity, 1.f / 60.f, 0.f, 1.f / 60.f, 1.f / 60.f, 1, false);
		World.GetPhysicsScene()->StartFrame();
		World.GetPhysicsScene()->EndFrame();
		World.GetPhysicsScene()->WaitPhysScenes();
	}

	FWheelFixture CreateWheel(UWorld& World, const FVector& WorldOrigin, const float SideY,
		const EVehicleIndependentSuspensionType Geometry, const float UnsprungMass)
	{
		FWheelFixture Fixture;
		Fixture.Owner = World.SpawnActor<AActor>(WorldOrigin, FRotator::ZeroRotator);
		if (!Fixture.Owner) return Fixture;
		Fixture.Root = NewObject<UBoxComponent>(Fixture.Owner, TEXT("SuspensionTestRoot"));
		Fixture.Owner->AddInstanceComponent(Fixture.Root);
		Fixture.Owner->SetRootComponent(Fixture.Root);
		Fixture.Root->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Fixture.Root->RegisterComponent();
		Fixture.Root->SetWorldLocation(WorldOrigin);

		Fixture.Wheel = NewObject<UVehicleWheelComponent>(Fixture.Owner, TEXT("SuspensionTestWheel"));
		Fixture.Owner->AddInstanceComponent(Fixture.Wheel);
		Fixture.Wheel->SetupAttachment(Fixture.Root);
		Fixture.Wheel->SetRelativeLocation(FVector(0.f, SideY, -55.f));
		FVehicleWheelConfig WheelConfig = Fixture.Wheel->GetWheelConfig();
		WheelConfig.Radius = 33.f;
		WheelConfig.Width = 20.f;
		WheelConfig.Inertia = 1.f;
		Fixture.Wheel->SetWheelConfig(WheelConfig);

		FVehicleSuspensionKinematicsConfig Kinematics;
		Kinematics.SuspensionType = Geometry;
		Kinematics.RayCastMode = EVehicleSuspensionRayCastMode::SphereTrace;
		Kinematics.TraceChannel = ECC_WorldDynamic;
		Kinematics.SuspensionAndBrakeMass = UnsprungMass;
		Fixture.Wheel->SetSuspensionKinematicsConfig(Kinematics);
		Fixture.Wheel->RegisterComponent();
		Fixture.RelativeTransform = Fixture.Wheel->GetRelativeTransform();
		Fixture.Solver.Initialize(Fixture.Wheel);
		return Fixture;
	}

	FVehicleChassisSimState MakeChassisState(const FVector& CoM)
	{
		FVehicleChassisSimState State;
		State.Mass = 1200.f; // kg
		State.CoMWorldLocation = Chaos::FVec3(CoM.X, CoM.Y, CoM.Z); // cm, world space
		State.LinearVelocity = Chaos::FVec3(0.f);
		State.AngularVelocity = Chaos::FVec3(0.f);
		State.LinearAcceleration = Chaos::FVec3(0.f);
		State.AngularAcceleration = Chaos::FVec3(0.f);
		// Synthetic diagonal inverse inertia in kg^-1 m^-2; no chassis body force is applied.
		State.WorldInvInertiaTensor = Chaos::FMatrix33(
			1.f / 1200.f, 0.f, 0.f,
			0.f, 1.f / 1200.f, 0.f,
			0.f, 0.f, 1.f / 1200.f);
		return State;
	}

	bool IsFinite(const FVehicleSuspensionSimState& State)
	{
		return FMath::IsFinite(State.StrutCurrentLength)
			&& FMath::IsFinite(State.StrutCurrentVelocity)
			&& FMath::IsFinite(State.TransientStrutLengthLimit)
			&& FMath::IsFinite(State.ImpactFriction)
			&& FMath::IsFinite(State.ForceAlongImpactNormal)
			&& FMath::IsFinite(State.StaticSprungMass)
			&& FMath::IsFinite(State.VirtualUnsprungMass)
			&& !State.HubChassisLocation.ContainsNaN()
			&& !State.ImpactWorldVelocity.ContainsNaN()
			&& !State.ImpactWorldNormal.ContainsNaN()
			&& State.HubChassisRotation.IsNormalized();
	}

	bool StateValuesMatch(const FVehicleSuspensionSimState& A, const FVehicleSuspensionSimState& B)
	{
		return A.bIsRightWheel == B.bIsRightWheel
			&& A.bWheelOnGround == B.bWheelOnGround && A.bHitGround == B.bHitGround
			&& A.bValidKinematicsConfig == B.bValidKinematicsConfig
			&& A.SteeringAngle == B.SteeringAngle
			&& A.StrutCurrentLength == B.StrutCurrentLength
			&& A.StrutCurrentVelocity == B.StrutCurrentVelocity
			&& A.TransientStrutLengthLimit == B.TransientStrutLengthLimit
			&& A.StaticSprungMass == B.StaticSprungMass
			&& A.VirtualUnsprungMass == B.VirtualUnsprungMass
			&& A.EffectiveSprungMassNormal == B.EffectiveSprungMassNormal
			&& A.EffectiveSprungMassLong == B.EffectiveSprungMassLong
			&& A.EffectiveSprungMassLat == B.EffectiveSprungMassLat
			&& A.ImpactFriction == B.ImpactFriction
			&& A.ForceAlongImpactNormal == B.ForceAlongImpactNormal
			&& A.AntiPitchScale == B.AntiPitchScale && A.AntiRollScale == B.AntiRollScale
			&& A.WheelWorldRightVector.Equals(B.WheelWorldRightVector, 0.f)
			&& A.HubChassisLocation.Equals(B.HubChassisLocation, 0.f)
			&& A.HubChassisRotation.Equals(B.HubChassisRotation, 0.f)
			&& A.TopMountChassisLocation.Equals(B.TopMountChassisLocation, 0.f)
			&& A.UpperBallJointChassisLocation.Equals(B.UpperBallJointChassisLocation, 0.f)
			&& A.LowerBallJointChassisLocation.Equals(B.LowerBallJointChassisLocation, 0.f)
			&& A.SteerAxisChassisDirection.Equals(B.SteerAxisChassisDirection, 0.f)
			&& A.ImpactWorldVelocity.Equals(B.ImpactWorldVelocity, 0.f)
			&& A.ImpactWorldNormal.Equals(B.ImpactWorldNormal, 0.f)
			&& A.ImpactWorldLocation.Equals(B.ImpactWorldLocation, 0.f);
	}

	bool ContactValuesMatch(const FVehicleSuspensionHitResult& A, const FVehicleSuspensionHitResult& B)
	{
		return A.bBlockingHit == B.bBlockingHit
			&& A.Component.Get() == B.Component.Get()
			&& A.PhysMaterial.Get() == B.PhysMaterial.Get()
			&& A.BoneName == B.BoneName
			&& A.TraceStart.Equals(B.TraceStart, 0.f)
			&& A.TraceEnd.Equals(B.TraceEnd, 0.f)
			&& A.Location.Equals(B.Location, 0.f)
			&& A.TraceRot.Equals(B.TraceRot, 0.f);
	}

	FCollisionQueryParams MakeQueryParams(AActor* Owner)
	{
		FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
		Params.AddIgnoredActor(Owner);
		Params.bReturnPhysicalMaterial = true;
		Params.bReturnFaceIndex = false;
		Params.bTraceComplex = false;
		return Params;
	}

	void AppendState(FString& Csv, const TCHAR* Geometry, const TCHAR* Side,
		const float Mass, const float Dt, const int32 Step, const float ChassisHeight,
		const FVehicleSuspensionSimState& State)
	{
		Csv += FString::Printf(TEXT("%s,%s,%.9g,%.9g,%d,%.9g,%d,%d,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g\n"),
			Geometry, Side, Mass, Dt, Step, ChassisHeight,
			State.bHitGround ? 1 : 0, State.bWheelOnGround ? 1 : 0,
			State.StrutCurrentLength, State.StrutCurrentVelocity, State.TransientStrutLengthLimit,
			State.ImpactFriction, State.ForceAlongImpactNormal, State.HubChassisLocation.X,
			State.HubChassisLocation.Y, State.HubChassisLocation.Z, State.ImpactWorldVelocity.X);
	}

	void SaveCsv(FAutomationTestBase& Test, const FString& RelativePath, const FString& Csv)
	{
		const FString OutputPath = FPaths::ProjectSavedDir() / RelativePath;
		IFileManager::Get().MakeDirectory(*FPaths::GetPath(OutputPath), true);
		Test.TestTrue(FString::Printf(TEXT("Save baseline %s"), *RelativePath), FFileHelper::SaveStringToFile(Csv, *OutputPath));
		Test.AddInfo(FString::Printf(TEXT("Baseline capture saved: %s"), *OutputPath));
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleSuspensionIndependentStageCaptureTest,
	"KinetiForge.Suspension.Stages.IndependentLegacyCapture",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleSuspensionIndependentStageCaptureTest::RunTest(const FString& Parameters)
{
	using namespace VehicleSuspensionStageTest;
	FWorldOwner WorldOwner;
	UWorld* World = CreateWorld(*this, WorldOwner);
	if (!World) return false;
	UBoxComponent* StaticGround = AddGround(*World, TEXT("StaticSuspensionGround"), FVector(0.f, 0.f, -20.f), false);
	UBoxComponent* MovingGround = AddGround(*World, TEXT("MovingSuspensionGround"), FVector(1000.f, 0.f, -20.f), true);
	if (!TestNotNull(TEXT("Static suspension ground created"), StaticGround)
		|| !TestNotNull(TEXT("Moving suspension ground created"), MovingGround)) return false;
	MovingGround->SetPhysicsLinearVelocity(FVector(25.f, 0.f, 0.f));
	FlushPhysicsScene(*World);

	const EVehicleIndependentSuspensionType Geometries[] = {
		EVehicleIndependentSuspensionType::StraightLine,
		EVehicleIndependentSuspensionType::Macpherson,
		EVehicleIndependentSuspensionType::DoubleWishbone
	};
	const TCHAR* GeometryNames[] = { TEXT("Straight"), TEXT("Macpherson"), TEXT("DoubleWishbone") };
	const float Rates[] = { 1.f / 60.f, 1.f / 120.f, 1.f / 240.f };
	const TCHAR* Sides[] = { TEXT("Left"), TEXT("Right") };
	FString Csv = TEXT("Geometry,Side,UnsprungMassKg,DeltaTimeSec,Step,ChassisHeightCm,Hit,WheelOnGround,StrutLengthCm,StrutVelocityCmPerSec,GroundLimitCm,Friction,NormalForceN,HubXcm,HubYcm,HubZcm,ImpactVelocityXms\n");
	int32 TotalGroundHits = 0;
	int32 TotalAirborneSamples = 0;
	int32 MovingGroundVelocitySamples = 0;
	int32 ZeroMassSamples = 0;
	for (int32 GeometryIndex = 0; GeometryIndex < UE_ARRAY_COUNT(Geometries); ++GeometryIndex)
	{
		for (int32 SideIndex = 0; SideIndex < UE_ARRAY_COUNT(Sides); ++SideIndex)
		{
			for (const float UnsprungMass : {0.f, 40.f})
			{
				// The 40 kg cases query the moving body; zero-mass cases query the static floor.
				const FVector WorldOrigin(UnsprungMass > 0.f ? 1000.f : 0.f, 0.f, 1000.f);
				FWheelFixture Fixture = CreateWheel(*World, WorldOrigin, SideIndex == 0 ? -60.f : 60.f,
					Geometries[GeometryIndex], UnsprungMass);
				if (!TestNotNull(TEXT("Registered suspension wheel fixture"), Fixture.Wheel)) return false;
				const FVehicleWheelConfig WheelConfig = Fixture.Wheel->GetWheelConfig();
				const FVehicleSuspensionKinematicsConfig KineConfig = Fixture.Wheel->GetSuspensionKinematicsConfig();
				const FVehicleSuspensionSpringConfig SpringConfig = Fixture.Wheel->GetSuspensionSpringConfig();
				for (const float Dt : Rates)
				{
					Fixture.Solver.Initialize(Fixture.Wheel);
					Fixture.Solver.SetSprungMass(SpringConfig, 300.f);
					FVehicleChassisSimState ChassisState = MakeChassisState(FVector(WorldOrigin.X, 0.f, 100.f));
					for (int32 Step = 0; Step < 30; ++Step)
					{
						const float Height = Step < 10 ? 130.f : Step < 20 ? 80.f : 130.f;
						const float Steering = Step < 10 ? 0.f : Step < 20 ? 12.f : -6.f;
						ChassisState.CoMWorldLocation = Chaos::FVec3(WorldOrigin.X, 0.f, Height);
						const FVector3f TireForce(Step < 10 ? 0.f : 120.f, Step < 10 ? 0.f : -80.f, 0.f);
						const float OtherHubZ = Step == 0 ? 0.f : Fixture.Solver.State.HubChassisLocation.Z + (SideIndex == 0 ? 5.f : -5.f);
						const float Swaybar = Step < 10 ? 0.f : 500.f;
						{
#if PHYSICS_THREAD_CONTEXT
							Chaos::FPhysicsThreadContextScope PhysicsContext(UnsprungMass > 0.f);
#endif
							Fixture.Solver.UpdateSuspension(WheelConfig.Radius, WheelConfig.Width, WheelConfig.Inertia,
								TireForce, KineConfig, SpringConfig, Fixture.RelativeTransform,
								FTransform(FQuat::Identity, FVector(WorldOrigin.X, 0.f, Height)),
								ChassisState, World, 9.81f, Dt, Steering, Swaybar, OtherHubZ);
						}
						const FVehicleSuspensionSimState& State = Fixture.Solver.State;
						TestTrue(FString::Printf(TEXT("%s %s mass %.0f dt %.5f step %d stays finite"),
							GeometryNames[GeometryIndex], Sides[SideIndex], UnsprungMass, Dt, Step), IsFinite(State));
						TotalGroundHits += State.bHitGround ? 1 : 0;
						TotalAirborneSamples += State.bHitGround ? 0 : 1;
						if (UnsprungMass > 0.f && State.bHitGround)
						{
							++MovingGroundVelocitySamples;
							TestTrue(TEXT("Moving-ground velocity is included in impact-relative speed"),
								FMath::IsNearlyEqual(State.ImpactWorldVelocity.X, -0.25f, 1.e-3f));
						}
						ZeroMassSamples += UnsprungMass <= 0.f ? 1 : 0;
						if (UnsprungMass <= 0.f)
							TestTrue(TEXT("Zero unsprung mass disables virtual unsprung mass"), FMath::IsNearlyZero(State.VirtualUnsprungMass));
						AppendState(Csv, GeometryNames[GeometryIndex], Sides[SideIndex], UnsprungMass, Dt, Step, Height, State);
					}
				}
			}
		}
	}
	TestTrue(TEXT("Independent capture exercises at least one real ground contact"), TotalGroundHits > 0);
	TestTrue(TEXT("Independent capture exercises airborne suspension queries"), TotalAirborneSamples > 0);
	TestTrue(TEXT("Independent capture samples moving-ground velocity"), MovingGroundVelocitySamples > 0);
	TestTrue(TEXT("Independent capture includes zero-unsprung-mass samples"), ZeroMassSamples > 0);
	SaveCsv(*this, TEXT("Automation/KinetiForge/SuspensionStages/independent.csv"), Csv);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleSuspensionSolidAxleStageCaptureTest,
	"KinetiForge.Suspension.Stages.SolidAxleLegacyCapture",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleSuspensionSolidAxleStageCaptureTest::RunTest(const FString& Parameters)
{
	using namespace VehicleSuspensionStageTest;
	FWorldOwner WorldOwner;
	UWorld* World = CreateWorld(*this, WorldOwner);
	if (!World) return false;
	UBoxComponent* Ground = AddGround(*World, TEXT("SolidAxleSuspensionGround"), FVector(0.f, 0.f, -20.f), false);
	if (!TestNotNull(TEXT("Solid axle ground created"), Ground)) return false;
	FlushPhysicsScene(*World);

	FWheelFixture Left = CreateWheel(*World, FVector(0.f, 0.f, 1000.f), -60.f,
		EVehicleIndependentSuspensionType::StraightLine, 40.f);
	FWheelFixture Right = CreateWheel(*World, FVector(0.f, 0.f, 1000.f), 60.f,
		EVehicleIndependentSuspensionType::StraightLine, 40.f);
	if (!TestNotNull(TEXT("Left solid-axle wheel created"), Left.Wheel)
		|| !TestNotNull(TEXT("Right solid-axle wheel created"), Right.Wheel)) return false;
	const FVehicleWheelConfig LeftWheelConfig = Left.Wheel->GetWheelConfig();
	const FVehicleWheelConfig RightWheelConfig = Right.Wheel->GetWheelConfig();
	const FVehicleSuspensionKinematicsConfig LeftKine = Left.Wheel->GetSuspensionKinematicsConfig();
	const FVehicleSuspensionKinematicsConfig RightKine = Right.Wheel->GetSuspensionKinematicsConfig();
	const FVehicleSuspensionSpringConfig LeftSpring = Left.Wheel->GetSuspensionSpringConfig();
	const FVehicleSuspensionSpringConfig RightSpring = Right.Wheel->GetSuspensionSpringConfig();
	const float Rates[] = { 1.f / 60.f, 1.f / 120.f, 1.f / 240.f };
	FString Csv = TEXT("DeltaTimeSec,Step,HeightCm,LeftHit,RightHit,LeftLengthCm,RightLengthCm,LeftVelocityCmPerSec,RightVelocityCmPerSec,LeftHubXcm,LeftHubYcm,LeftHubZcm,RightHubXcm,RightHubYcm,RightHubZcm\n");
	int32 SolidGroundHits = 0;
	int32 SolidAirborneSamples = 0;
	for (const float Dt : Rates)
	{
		Left.Solver.Initialize(Left.Wheel);
		Right.Solver.Initialize(Right.Wheel);
		Left.Solver.SetSprungMass(LeftSpring, 300.f);
		Right.Solver.SetSprungMass(RightSpring, 300.f);
		for (int32 Step = 0; Step < 30; ++Step)
		{
			const float Height = Step < 10 ? 130.f : Step < 20 ? 80.f : 130.f;
			const FTransform ChassisTransform(FQuat::Identity, FVector(0.f, 0.f, Height));
			FVehicleChassisSimState LeftChassisState = MakeChassisState(ChassisTransform.GetLocation());
			FVehicleChassisSimState RightChassisState = LeftChassisState;
			FVehicleSuspensionSimContext LeftContext;
			FVehicleSuspensionSimContext RightContext;
			// StartUpdateSolidAxle now takes the macrostep up front; the integrator needs it.
			const float LeftOtherHubZ = Right.Solver.State.HubChassisLocation.Z;
			const float RightOtherHubZ = Left.Solver.State.HubChassisLocation.Z;
			{
#if PHYSICS_THREAD_CONTEXT
				Chaos::FPhysicsThreadContextScope PhysicsContext(false);
#endif
				Left.Solver.StartUpdateSolidAxle(LeftWheelConfig.Radius, LeftWheelConfig.Width, LeftWheelConfig.Inertia,
					LeftKine, LeftSpring, Left.RelativeTransform, ChassisTransform, LeftChassisState, World, 9.81f, Dt,
					0.f, 0.f, LeftOtherHubZ, LeftContext);
				Right.Solver.StartUpdateSolidAxle(RightWheelConfig.Radius, RightWheelConfig.Width, RightWheelConfig.Inertia,
					RightKine, RightSpring, Right.RelativeTransform, ChassisTransform, RightChassisState, World, 9.81f, Dt,
					0.f, 0.f, RightOtherHubZ, RightContext);
			}
			FVector3f AxleCenter = FVector3f::ZeroVector;
			FQuat4f AxleRotation = FQuat4f::Identity;
			FVehicleSuspensionSolver::SolveSolidAxlePosture(
				LeftContext.TopMountChassisLocation, RightContext.TopMountChassisLocation,
				LeftContext.StrutCurrentLength + LeftKine.MinStrutLength,
				RightContext.StrutCurrentLength + RightKine.MinStrutLength,
				60.f, AxleCenter, AxleRotation);
			{
#if PHYSICS_THREAD_CONTEXT
				Chaos::FPhysicsThreadContextScope PhysicsContext(false);
#endif
				Left.Solver.FinalizeUpdateSolidAxle(LeftWheelConfig.Radius, LeftKine, LeftSpring, ChassisTransform,
					LeftChassisState, 9.81f, Dt, 0.f, RightOtherHubZ, 60.f, AxleCenter, AxleRotation,
					FVector3f(0.f), LeftContext);
				Right.Solver.FinalizeUpdateSolidAxle(RightWheelConfig.Radius, RightKine, RightSpring, ChassisTransform,
					RightChassisState, 9.81f, Dt, 0.f, LeftOtherHubZ, 60.f, AxleCenter, AxleRotation,
					FVector3f(0.f), RightContext);
			}
			TestTrue(FString::Printf(TEXT("Solid axle dt %.5f step %d left state finite"), Dt, Step), IsFinite(Left.Solver.State));
			TestTrue(FString::Printf(TEXT("Solid axle dt %.5f step %d right state finite"), Dt, Step), IsFinite(Right.Solver.State));
			SolidGroundHits += (Left.Solver.State.bHitGround ? 1 : 0) + (Right.Solver.State.bHitGround ? 1 : 0);
			SolidAirborneSamples += (Left.Solver.State.bHitGround ? 0 : 1) + (Right.Solver.State.bHitGround ? 0 : 1);
			Csv += FString::Printf(TEXT("%.9g,%d,%.9g,%d,%d,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g,%.9g\n"),
				Dt, Step, Height, Left.Solver.State.bHitGround ? 1 : 0, Right.Solver.State.bHitGround ? 1 : 0,
				Left.Solver.State.StrutCurrentLength, Right.Solver.State.StrutCurrentLength,
				Left.Solver.State.StrutCurrentVelocity, Right.Solver.State.StrutCurrentVelocity,
				Left.Solver.State.HubChassisLocation.X, Left.Solver.State.HubChassisLocation.Y, Left.Solver.State.HubChassisLocation.Z,
				Right.Solver.State.HubChassisLocation.X, Right.Solver.State.HubChassisLocation.Y, Right.Solver.State.HubChassisLocation.Z);
		}
	}
	TestTrue(TEXT("Solid axle capture includes real ground contacts"), SolidGroundHits > 0);
	TestTrue(TEXT("Solid axle capture includes airborne queries"), SolidAirborneSamples > 0);
	SaveCsv(*this, TEXT("Automation/KinetiForge/SuspensionStages/solid.csv"), Csv);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleSuspensionCallerOwnedPhasesTest,
	"KinetiForge.Suspension.Stages.CallerOwnedPhases",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleSuspensionCallerOwnedPhasesTest::RunTest(const FString& Parameters)
{
	using namespace VehicleSuspensionStageTest;

	FVehicleSuspensionKinematicsConfig KinematicsA;
	KinematicsA.SuspensionType = EVehicleIndependentSuspensionType::StraightLine;
	KinematicsA.SuspensionAndBrakeMass = 40.f;
	FVehicleSuspensionKinematicsConfig KinematicsB;
	KinematicsB.SuspensionType = EVehicleIndependentSuspensionType::Macpherson;
	KinematicsB.SuspensionAndBrakeMass = 0.f;
	FVehicleSuspensionSpringConfig SpringA;
	SpringA.bUseDampingRatio = false;
	FVehicleSuspensionSpringConfig SpringB = SpringA;
	FVehicleSuspensionCachedLUTs CachedLUTA;
	FVehicleSuspensionCachedLUTs CachedLUTB;
	FVehicleSuspensionSolver::CacheLUTs(CachedLUTA, KinematicsA);
	FVehicleSuspensionSolver::CacheLUTs(CachedLUTB, KinematicsB);

	const float WheelRadius = 33.f;
	const float WheelInertia = 1.f;
	const FTransform ComponentA(FQuat::Identity, FVector(0.f, -60.f, -55.f));
	const FTransform ComponentB(FQuat::Identity, FVector(0.f, 60.f, -55.f));
	const FTransform ChassisTransformA(FQuat::Identity, FVector(0.f, 0.f, 130.f));
	const FTransform ChassisTransformB(FQuat::Identity, FVector(0.f, 0.f, 80.f));
	FVehicleSuspensionSimState PreviousA = FVehicleSuspensionSolver::SolveKinematicsAtExtension(
		WheelRadius, KinematicsA, ComponentA, 0.4f, -3.f, 2, &CachedLUTA);
	FVehicleSuspensionSimState PreviousB = FVehicleSuspensionSolver::SolveKinematicsAtExtension(
		WheelRadius, KinematicsB, ComponentB, 0.7f, 5.f, 2, &CachedLUTB);
	PreviousA.StaticSprungMass = 300.f;
	PreviousA.StrutCurrentLength = 7.f;
	PreviousA.StrutCurrentVelocity = -2.f;
	PreviousA.TransientStrutLengthLimit = 8.f;
	PreviousB.StaticSprungMass = 300.f;
	PreviousB.StrutCurrentLength = 15.f;
	PreviousB.StrutCurrentVelocity = 1.f;
	PreviousB.TransientStrutLengthLimit = 13.f;

	FVehicleSuspensionSolver SolverA;
	FVehicleSuspensionSolver SolverB;
	SolverA.State = PreviousA;
	SolverB.State = PreviousB;
	const FVehicleSuspensionSimState SolverStateBeforeA = SolverA.State;
	const FVehicleSuspensionSimState SolverStateBeforeB = SolverB.State;
	const FVehicleSuspensionSimState CallerStateBeforeA = PreviousA;
	const FVehicleSuspensionSimState CallerStateBeforeB = PreviousB;
	FVehicleSuspensionSimContext ContextA;
	FVehicleSuspensionSimContext ContextB;
	ContextA.PhysicsDeltaTime = 1.f / 120.f;
	ContextB.PhysicsDeltaTime = 1.f / 240.f;

	FVehicleSuspensionSolver::PrepareSuspensionContext(PreviousA, ContextA, ComponentA,
		ChassisTransformA, KinematicsA, -3.f);
	FVehicleSuspensionSolver::PrepareSuspensionContext(PreviousB, ContextB, ComponentB,
		ChassisTransformB, KinematicsB, 5.f);
	TestTrue(TEXT("Prepare leaves source caller state A unchanged"), StateValuesMatch(PreviousA, CallerStateBeforeA));
	TestTrue(TEXT("Prepare leaves source caller state B unchanged"), StateValuesMatch(PreviousB, CallerStateBeforeB));

	// Synthetic contact geometry drives phase isolation only; this test does not claim raycast equivalence.
	auto InjectSyntheticContact = [WheelRadius](FVehicleSuspensionSimContext& Context, const FVector& ImpactPoint)
	{
		Context.bHitGround = true;
		Context.HitDistance = WheelRadius + 8.f;
		Context.ImpactFriction = 0.73f;
		Context.ImpactWorldVelocity = FVector3f(-0.25f, 0.f, 0.f);
		Context.HitResult.bBlockingHit = true;
		Context.HitResult.Distance = 8.f;
		Context.HitResult.ImpactPoint = ImpactPoint;
		Context.HitResult.Location = ImpactPoint;
		Context.HitResult.Normal = FVector::UpVector;
	};
	InjectSyntheticContact(ContextA, FVector(0.f, -60.f, 0.f));
	InjectSyntheticContact(ContextB, FVector(0.f, 60.f, 0.f));

	const FVehicleChassisSimState ChassisStateA = MakeChassisState(ChassisTransformA.GetLocation());
	const FVehicleChassisSimState ChassisStateB = MakeChassisState(ChassisTransformB.GetLocation());
	// Interleave the distinct contexts to verify caller-owned history does not alias between wheels.
	FVehicleSuspensionSolver::IntegrateSingleWheelSuspension(ContextB, ChassisStateB, 9.81f,
		WheelRadius, WheelInertia, KinematicsB, SpringB, CachedLUTB, 0.f, PreviousA.HubChassisLocation.Z);
	FVehicleSuspensionSolver::IntegrateSingleWheelSuspension(ContextA, ChassisStateA, 9.81f,
		WheelRadius, WheelInertia, KinematicsA, SpringA, CachedLUTA, 500.f, PreviousB.HubChassisLocation.Z);
	TestTrue(TEXT("Integrate leaves solver-owned state A unchanged"), StateValuesMatch(SolverA.State, SolverStateBeforeA));
	TestTrue(TEXT("Integrate leaves solver-owned state B unchanged"), StateValuesMatch(SolverB.State, SolverStateBeforeB));
	TestTrue(TEXT("Integrate leaves caller state A unchanged"), StateValuesMatch(PreviousA, CallerStateBeforeA));
	TestTrue(TEXT("Integrate leaves caller state B unchanged"), StateValuesMatch(PreviousB, CallerStateBeforeB));
	TestTrue(TEXT("Interleaved contexts remain finite"), FMath::IsFinite(ContextA.StrutCurrentLength)
		&& FMath::IsFinite(ContextB.StrutCurrentLength)
		&& !ContextA.HubChassisTransform.GetLocation().ContainsNaN()
		&& !ContextB.HubChassisTransform.GetLocation().ContainsNaN());
	TestTrue(TEXT("Interleaved contexts retain distinct unsprung-mass behavior"),
		ContextA.VirtualUnsprungMass > 0.f && FMath::IsNearlyZero(ContextB.VirtualUnsprungMass));

	FVehicleSuspensionSimState OutputA = PreviousA;
	FVehicleSuspensionSimState OutputB = PreviousB;
	FVehicleSuspensionSolver::FinalizeIndependentSuspension(ContextA, OutputA, WheelRadius,
		FVector3f(100.f, -50.f, 0.f), KinematicsA, SpringA, ChassisTransformA,
		ChassisStateA, 9.81f, CachedLUTA, 500.f, PreviousB.HubChassisLocation.Z);
	TestTrue(TEXT("Finalize A writes only its initialized caller output"), IsFinite(OutputA)
		&& OutputA.SteeringAngle == -3.f && OutputA.StaticSprungMass == PreviousA.StaticSprungMass);
	TestTrue(TEXT("Finalize A leaves output B and solver-owned states alone"),
		StateValuesMatch(OutputB, PreviousB)
		&& StateValuesMatch(SolverA.State, SolverStateBeforeA)
		&& StateValuesMatch(SolverB.State, SolverStateBeforeB));
	FVehicleSuspensionSolver::FinalizeIndependentSuspension(ContextB, OutputB, WheelRadius,
		FVector3f(-25.f, 80.f, 0.f), KinematicsB, SpringB, ChassisTransformB,
		ChassisStateB, 9.81f, CachedLUTB, 0.f, PreviousA.HubChassisLocation.Z);
	TestTrue(TEXT("Finalize B writes its own initialized caller output"), IsFinite(OutputB)
		&& OutputB.SteeringAngle == 5.f && OutputB.StaticSprungMass == PreviousB.StaticSprungMass);
	TestTrue(TEXT("Both finalizations leave solver-owned states unchanged"),
		StateValuesMatch(SolverA.State, SolverStateBeforeA)
		&& StateValuesMatch(SolverB.State, SolverStateBeforeB));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleSuspensionOwnedIndependentContactTest,
	"KinetiForge.Suspension.Stages.OwnedIndependentContact",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleSuspensionOwnedIndependentContactTest::RunTest(const FString& Parameters)
{
	using namespace VehicleSuspensionStageTest;
	(void)Parameters;
	FWorldOwner WorldOwner;
	UWorld* World = CreateWorld(*this, WorldOwner);
	if (!World) return false;
	UBoxComponent* StaticGround = AddGround(*World, TEXT("OwnedIndependentStaticGround"), FVector(0.f, 0.f, -20.f), false);
	UBoxComponent* MovingGround = AddGround(*World, TEXT("OwnedIndependentMovingGround"), FVector(1000.f, 0.f, -20.f), true);
	if (!TestNotNull(TEXT("Static ground created"), StaticGround)
		|| !TestNotNull(TEXT("Moving ground created"), MovingGround)) return false;
	MovingGround->SetPhysicsLinearVelocity(FVector(25.f, 0.f, 0.f));
	FlushPhysicsScene(*World);

	const EVehicleIndependentSuspensionType Geometries[] = {
		EVehicleIndependentSuspensionType::StraightLine,
		EVehicleIndependentSuspensionType::Macpherson,
		EVehicleIndependentSuspensionType::DoubleWishbone
	};
	const float Rates[] = { 1.f / 60.f, 1.f / 120.f, 1.f / 240.f };
	int32 GroundHits = 0;
	int32 AirSamples = 0;
	int32 MovingVelocityMatches = 0;
	for (int32 GeometryIndex = 0; GeometryIndex < UE_ARRAY_COUNT(Geometries); ++GeometryIndex)
	{
		for (const float UnsprungMass : {0.f, 40.f})
		{
			const FVector Origin(UnsprungMass > 0.f ? 1000.f : 0.f, 0.f, 1000.f);
			FWheelFixture Fixture = CreateWheel(*World, Origin, -60.f, Geometries[GeometryIndex], UnsprungMass);
			if (!TestNotNull(TEXT("Independent wheel fixture created"), Fixture.Wheel)) return false;
			for (const float Dt : Rates)
			{
				Fixture.Solver.Initialize(Fixture.Wheel);
				Fixture.Solver.SetSprungMass(Fixture.Wheel->GetSuspensionSpringConfig(), 300.f);
				FVehicleWheelRuntime Runtime;
				Runtime.Settings = Fixture.Wheel->CaptureRuntimeSettings();
				Runtime.SuspensionState = Fixture.Solver.State;
				const FCollisionQueryParams QueryParams = MakeQueryParams(Fixture.Owner);
				const FCollisionResponseParams ResponseParams = FCollisionResponseParams::DefaultResponseParam;
				FVehicleChassisSimState ChassisState = MakeChassisState(FVector(Origin.X, 0.f, 100.f));
				for (int32 Step = 0; Step < 30; ++Step)
				{
					const float Height = Step < 10 ? 130.f : Step < 20 ? 80.f : 130.f;
					const float Steering = Step < 10 ? 0.f : Step < 20 ? 12.f : -6.f;
					const FTransform ChassisTransform(FQuat::Identity, FVector(Origin.X, 0.f, Height));
					ChassisState.CoMWorldLocation = Chaos::FVec3(Origin.X, 0.f, Height);
					const FVector3f TireForce(Step < 10 ? 0.f : 120.f, Step < 10 ? 0.f : -80.f, 0.f);
					Runtime.WheelState.TireForce = TireForce;
					const float OtherHubZ = Step == 0 ? 0.f : Fixture.Solver.State.HubChassisLocation.Z + 5.f;
					FVehicleWheelPhysicsFrame Frame;
					Frame.ComponentRelativeTransform = Fixture.RelativeTransform;
					Frame.ChassisWorldTransform = ChassisTransform;
					Frame.ChassisState = ChassisState;
					Frame.WorldGravityZ = 9.81f;
					Frame.MacroDeltaTime = Dt;
					FVehicleSuspensionHitResult RuntimeContact;
					{
#if PHYSICS_THREAD_CONTEXT
						Chaos::FPhysicsThreadContextScope PhysicsContext(UnsprungMass > 0.f);
#endif
						Fixture.Solver.UpdateSuspension(Fixture.Wheel->GetWheelConfig().Radius,
							Fixture.Wheel->GetWheelConfig().Width, Fixture.Wheel->GetWheelConfig().Inertia,
							TireForce, Fixture.Wheel->GetSuspensionKinematicsConfig(),
							Fixture.Wheel->GetSuspensionSpringConfig(), Fixture.RelativeTransform,
							ChassisTransform, ChassisState, World, 9.81f, Dt, Steering, 500.f, OtherHubZ);
						FVehicleWheelPhysics::UpdateIndependentSuspension(Runtime, Frame, World,
							QueryParams, ResponseParams, Steering, 500.f, OtherHubZ, RuntimeContact);
					}
					const FString Context = FString::Printf(TEXT("Independent geometry %d mass %.0f dt %.5f step %d"),
						GeometryIndex, UnsprungMass, Dt, Step);
					TestTrue(Context + TEXT(" state matches legacy"), StateValuesMatch(Runtime.SuspensionState, Fixture.Solver.State));
					TestTrue(Context + TEXT(" hit result matches legacy"), ContactValuesMatch(RuntimeContact, Fixture.Solver.RayCastResult));
					TestTrue(Context + TEXT(" state finite"), IsFinite(Runtime.SuspensionState));
					GroundHits += Runtime.SuspensionState.bHitGround ? 1 : 0;
					AirSamples += Runtime.SuspensionState.bHitGround ? 0 : 1;
					if (UnsprungMass > 0.f && Runtime.SuspensionState.bHitGround)
					{
						++MovingVelocityMatches;
						TestTrue(Context + TEXT(" moving ground velocity matches"),
							FMath::IsNearlyEqual(Runtime.SuspensionState.ImpactWorldVelocity.X, -0.25f, 1.e-3f));
					}
				}
			}
		}
	}
	TestTrue(TEXT("Owned independent query sees real ground contact"), GroundHits > 0);
	TestTrue(TEXT("Owned independent query includes airborne samples"), AirSamples > 0);
	TestTrue(TEXT("Owned independent query includes moving-ground velocity"), MovingVelocityMatches > 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleSuspensionOwnedSolidAxleContactTest,
	"KinetiForge.Suspension.Stages.OwnedSolidAxleContact",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleSuspensionOwnedSolidAxleContactTest::RunTest(const FString& Parameters)
{
	using namespace VehicleSuspensionStageTest;
	(void)Parameters;
	FWorldOwner WorldOwner;
	UWorld* World = CreateWorld(*this, WorldOwner);
	if (!World) return false;
	UBoxComponent* Ground = AddGround(*World, TEXT("OwnedSolidAxleGround"), FVector(0.f, 0.f, -20.f), false);
	if (!TestNotNull(TEXT("Solid axle ground created"), Ground)) return false;
	FlushPhysicsScene(*World);
	FWheelFixture Left = CreateWheel(*World, FVector(0.f, 0.f, 1000.f), -60.f,
		EVehicleIndependentSuspensionType::StraightLine, 40.f);
	FWheelFixture Right = CreateWheel(*World, FVector(0.f, 0.f, 1000.f), 60.f,
		EVehicleIndependentSuspensionType::StraightLine, 40.f);
	if (!TestNotNull(TEXT("Left solid wheel created"), Left.Wheel)
		|| !TestNotNull(TEXT("Right solid wheel created"), Right.Wheel)) return false;
	FVehicleWheelRuntime LeftRuntime, RightRuntime;
	FVehicleSuspensionSolver* LegacySolvers[] = { &Left.Solver, &Right.Solver };
	FWheelFixture* Fixtures[] = { &Left, &Right };
	FVehicleWheelRuntime* Runtimes[] = { &LeftRuntime, &RightRuntime };
	for (int32 Side = 0; Side < 2; ++Side)
	{
		FWheelFixture& Fixture = *Fixtures[Side];
		FVehicleWheelRuntime& Runtime = *Runtimes[Side];
		Fixture.Solver.Initialize(Fixture.Wheel);
		Fixture.Solver.SetSprungMass(Fixture.Wheel->GetSuspensionSpringConfig(), 300.f);
		Runtime.Settings = Fixture.Wheel->CaptureRuntimeSettings();
		Runtime.SuspensionState = Fixture.Solver.State;
		Runtime.WheelState.TireForce = FVector3f(0.f);
	}
	const FCollisionQueryParams LeftQueryParams = MakeQueryParams(Left.Owner);
	const FCollisionQueryParams RightQueryParams = MakeQueryParams(Right.Owner);
	const FCollisionResponseParams ResponseParams = FCollisionResponseParams::DefaultResponseParam;
	int32 GroundHits = 0, AirSamples = 0;
	const float Rates[] = { 1.f / 60.f, 1.f / 120.f, 1.f / 240.f };
	for (const float Dt : Rates)
	{
		for (int32 Side = 0; Side < 2; ++Side)
		{
			FWheelFixture& Fixture = *Fixtures[Side];
			FVehicleWheelRuntime& Runtime = *Runtimes[Side];
			Fixture.Solver.Initialize(Fixture.Wheel);
			Fixture.Solver.SetSprungMass(Fixture.Wheel->GetSuspensionSpringConfig(), 300.f);
			Runtime.SuspensionState = Fixture.Solver.State;
		}
		for (int32 Step = 0; Step < 30; ++Step)
		{
			const float Height = Step < 10 ? 130.f : Step < 20 ? 80.f : 130.f;
			const FTransform ChassisTransform(FQuat::Identity, FVector(0.f, 0.f, Height));
			const FVehicleChassisSimState ChassisState = MakeChassisState(ChassisTransform.GetLocation());
			FVehicleWheelPhysicsFrame Frames[2];
			FVehicleSuspensionSimContext LegacyContexts[2];
			FVehicleSuspensionSimContext RuntimeContexts[2];
			for (int32 Side = 0; Side < 2; ++Side)
			{
				FWheelFixture& Fixture = *Fixtures[Side];
				FVehicleWheelRuntime& Runtime = *Runtimes[Side];
				Frames[Side].ComponentRelativeTransform = Fixture.RelativeTransform;
				Frames[Side].ChassisWorldTransform = ChassisTransform;
				Frames[Side].ChassisState = ChassisState;
				Frames[Side].WorldGravityZ = 9.81f;
				Frames[Side].MacroDeltaTime = Dt;
				const float OtherZ = LegacySolvers[1 - Side]->State.HubChassisLocation.Z;
				{
#if PHYSICS_THREAD_CONTEXT
					Chaos::FPhysicsThreadContextScope PhysicsContext(false);
#endif
					Fixture.Solver.StartUpdateSolidAxle(Fixture.Wheel->GetWheelConfig().Radius,
						Fixture.Wheel->GetWheelConfig().Width, Fixture.Wheel->GetWheelConfig().Inertia,
						Fixture.Wheel->GetSuspensionKinematicsConfig(), Fixture.Wheel->GetSuspensionSpringConfig(),
						Fixture.RelativeTransform, ChassisTransform, ChassisState, World, 9.81f, Dt,
						0.f, 0.f, OtherZ, LegacyContexts[Side]);
					FVehicleWheelPhysics::StartSolidAxleSuspension(Runtime, RuntimeContexts[Side], Frames[Side], World,
						Side == 0 ? LeftQueryParams : RightQueryParams, ResponseParams, 0.f, 0.f, OtherZ);
				}
				TestTrue(TEXT("Solid start uses the macro timestep for legacy and runtime paths"),
					FMath::IsNearlyEqual(RuntimeContexts[Side].PhysicsDeltaTime, Dt)
					&& FMath::IsNearlyEqual(LegacyContexts[Side].PhysicsDeltaTime, Dt));
			}
			FVector3f LegacyCenter, RuntimeCenter;
			FQuat4f LegacyRotation, RuntimeRotation;
			FVehicleSuspensionSolver::SolveSolidAxlePosture(
				LegacyContexts[0].TopMountChassisLocation, LegacyContexts[1].TopMountChassisLocation,
				LegacyContexts[0].StrutCurrentLength + LeftRuntime.Settings.KinematicsConfig.MinStrutLength,
				LegacyContexts[1].StrutCurrentLength + RightRuntime.Settings.KinematicsConfig.MinStrutLength,
				60.f, LegacyCenter, LegacyRotation);
			FVehicleSuspensionSolver::SolveSolidAxlePosture(
				RuntimeContexts[0].TopMountChassisLocation, RuntimeContexts[1].TopMountChassisLocation,
				RuntimeContexts[0].StrutCurrentLength + LeftRuntime.Settings.KinematicsConfig.MinStrutLength,
				RuntimeContexts[1].StrutCurrentLength + RightRuntime.Settings.KinematicsConfig.MinStrutLength,
				60.f, RuntimeCenter, RuntimeRotation);
			TestTrue(TEXT("Owned solid posture center matches legacy"), LegacyCenter.Equals(RuntimeCenter, 0.f));
			TestTrue(TEXT("Owned solid posture rotation matches legacy"), LegacyRotation.Equals(RuntimeRotation, 0.f));
			for (int32 Side = 0; Side < 2; ++Side)
			{
				FWheelFixture& Fixture = *Fixtures[Side];
				FVehicleWheelRuntime& Runtime = *Runtimes[Side];
				const float OtherZ = LegacySolvers[1 - Side]->State.HubChassisLocation.Z;
				FVehicleSuspensionHitResult RuntimeContact;
				{
#if PHYSICS_THREAD_CONTEXT
					Chaos::FPhysicsThreadContextScope PhysicsContext(false);
#endif
					Fixture.Solver.FinalizeUpdateSolidAxle(Fixture.Wheel->GetWheelConfig().Radius,
						Fixture.Wheel->GetSuspensionKinematicsConfig(), Fixture.Wheel->GetSuspensionSpringConfig(),
						Frames[Side].ChassisWorldTransform, Frames[Side].ChassisState, 9.81f, Dt,
						0.f, OtherZ, 60.f, LegacyCenter, LegacyRotation, FVector3f(0.f), LegacyContexts[Side]);
					FVehicleWheelPhysics::FinalizeSolidAxleSuspension(Runtime, RuntimeContexts[Side], Frames[Side],
						0.f, OtherZ, 60.f, RuntimeCenter, RuntimeRotation, RuntimeContact);
				}
				TestTrue(FString::Printf(TEXT("Solid side %d dt %.5f step %d state matches"), Side, Dt, Step),
					StateValuesMatch(Runtime.SuspensionState, Fixture.Solver.State));
				TestTrue(FString::Printf(TEXT("Solid side %d dt %.5f step %d contact matches"), Side, Dt, Step),
					ContactValuesMatch(RuntimeContact, Fixture.Solver.RayCastResult));
				GroundHits += Runtime.SuspensionState.bHitGround ? 1 : 0;
				AirSamples += Runtime.SuspensionState.bHitGround ? 0 : 1;
			}
		}
	}
	TestTrue(TEXT("Owned solid path exercises ground hits"), GroundHits > 0);
	TestTrue(TEXT("Owned solid path exercises airborne samples"), AirSamples > 0);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
