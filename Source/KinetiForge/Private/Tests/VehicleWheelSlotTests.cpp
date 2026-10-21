// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "VehicleAxleAssemblyComponent.h"
#include "VehicleWheelComponent.h"
#include "VehicleAxleStructs.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Misc/AutomationTest.h"

/** Test-only access to the slot configuration that Blueprint cannot reach. */
struct FVehicleWheelSlotProbe
{
	static void ConfigureManual(UVehicleAxleAssemblyComponent& Axle, FName LeftName, FName RightName)
	{
		Axle.bUseExistingWheelComponent = true;
		Axle.LeftWheelComponentName = LeftName;
		Axle.RightWheelComponentName = RightName;
		Axle.AxleLayout = EVehicleAxleLayout::TwoWheels;
		Axle.SuspensionType = EVehicleAxleSuspensionType::Independent;
	}

	static void ConfigureGenerated(UVehicleAxleAssemblyComponent& Axle)
	{
		Axle.bUseExistingWheelComponent = false;
		Axle.LeftWheelComponentName = NAME_None;
		Axle.RightWheelComponentName = NAME_None;
		Axle.AxleLayout = EVehicleAxleLayout::TwoWheels;
		Axle.SuspensionType = EVehicleAxleSuspensionType::Independent;
	}

	static void SetLayout(UVehicleAxleAssemblyComponent& Axle, EVehicleAxleLayout Layout)
	{
		Axle.AxleLayout = Layout;
	}

	static bool IsLeftGenerated(const UVehicleAxleAssemblyComponent& Axle) { return Axle.bLeftWheelGenerated; }
	static bool IsRightGenerated(const UVehicleAxleAssemblyComponent& Axle) { return Axle.bRightWheelGenerated; }
	static void ClaimAxle(UVehicleAxleAssemblyComponent& Axle, const TSharedPtr<FVehiclePhysicsStep, ESPMode::ThreadSafe>& Step)
	{
		Axle.PhysicsOwnership.Claim(Step);
	}
};

namespace VehicleWheelSlotTest
{
	struct FWorldGuard
	{
		UWorld* World = nullptr;
		~FWorldGuard() { if (World) World->DestroyWorld(false); }
	};

	int32 CountWheelComponents(AActor* Actor)
	{
		TInlineComponentArray<UActorComponent*> Components;
		Actor->GetComponents(Components);
		int32 Count = 0;
		for (UActorComponent* Component : Components)
		{
			Count += (IsValid(Component) && Component->IsA(UVehicleWheelComponent::StaticClass())) ? 1 : 0;
		}
		return Count;
	}

	struct FFixture
	{
		FWorldGuard WorldGuard;
		AActor* Actor = nullptr;
		UBoxComponent* Chassis = nullptr;

		bool Build(FAutomationTestBase& Test)
		{
			WorldGuard.World = UWorld::CreateWorld(EWorldType::EditorPreview, false, NAME_None, nullptr,
				true, ERHIFeatureLevel::Num, nullptr, true);
			if (!Test.TestNotNull(TEXT("wheel slot world created"), WorldGuard.World)) return false;
			WorldGuard.World->InitWorld(UWorld::InitializationValues()
				.InitializeScenes(true)
				.AllowAudioPlayback(false)
				.RequiresHitProxies(false)
				.CreatePhysicsScene(true)
				.CreateNavigation(false)
				.CreateAISystem(false)
				.ShouldSimulatePhysics(false)
				.EnableTraceCollision(false)
				.SetTransactional(false)
				.CreateFXSystem(false));

			Actor = WorldGuard.World->SpawnActor<AActor>();
			if (!Test.TestNotNull(TEXT("wheel slot actor spawned"), Actor)) return false;

			Chassis = NewObject<UBoxComponent>(Actor, TEXT("SlotChassis"));
			Actor->AddInstanceComponent(Chassis);
			Actor->SetRootComponent(Chassis);
			Chassis->SetBoxExtent(FVector(60.f));
			Chassis->SetSimulatePhysics(false);
			Chassis->RegisterComponent();
			return true;
		}

		UVehicleWheelComponent* AddWheel(FName Name)
		{
			UVehicleWheelComponent* Wheel = NewObject<UVehicleWheelComponent>(Actor, Name);
			Actor->AddInstanceComponent(Wheel);
			Wheel->SetupAttachment(Chassis);
			Wheel->RegisterComponent();
			return Wheel;
		}

		UVehicleAxleAssemblyComponent* AddAxle(FName Name)
		{
			UVehicleAxleAssemblyComponent* Axle = NewObject<UVehicleAxleAssemblyComponent>(Actor, Name);
			Actor->AddInstanceComponent(Axle);
			Axle->SetupAttachment(Chassis);
			return Axle;
		}
	};
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleWheelSlotManualOwnershipTest,
	"KinetiForge.Vehicle.WheelSlots.ManualOwnership",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleWheelSlotManualOwnershipTest::RunTest(const FString& Parameters)
{
	using namespace VehicleWheelSlotTest;

	FFixture Fixture;
	if (!Fixture.Build(*this)) return false;

	UVehicleWheelComponent* ManualLeft = Fixture.AddWheel(TEXT("SlotManualLeft"));
	UVehicleWheelComponent* ManualRight = Fixture.AddWheel(TEXT("SlotManualRight"));
	UVehicleAxleAssemblyComponent* Axle = Fixture.AddAxle(TEXT("SlotManualAxle"));
	FVehicleWheelSlotProbe::ConfigureManual(*Axle, TEXT("SlotManualLeft"), TEXT("SlotManualRight"));
	Axle->RegisterComponent();

	UVehicleWheelComponent* Left = nullptr;
	UVehicleWheelComponent* Right = nullptr;
	Axle->GetWheels(Left, Right);
	TestTrue(TEXT("manual slots resolve the placed wheels"), Left == ManualLeft && Right == ManualRight);
	TestFalse(TEXT("referenced wheels are not owned"), FVehicleWheelSlotProbe::IsLeftGenerated(*Axle)
		|| FVehicleWheelSlotProbe::IsRightGenerated(*Axle));

	// Refreshing the axle must never dispose of wheels it did not create.
	for (int32 Refresh = 0; Refresh < 3; ++Refresh)
	{
		Axle->InitializeWheels();
	}

	Axle->GetWheels(Left, Right);
	TestTrue(TEXT("manual wheels survive repeated refresh"), Left == ManualLeft && Right == ManualRight);
	TestTrue(TEXT("manual wheel components remain registered"),
		IsValid(ManualLeft) && ManualLeft->IsRegistered() && IsValid(ManualRight) && ManualRight->IsRegistered());
	TestEqual(TEXT("no duplicate wheel was generated"), CountWheelComponents(Fixture.Actor), 2);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleWheelSlotLayoutTest,
	"KinetiForge.Vehicle.WheelSlots.Layout",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleWheelSlotLayoutTest::RunTest(const FString& Parameters)
{
	using namespace VehicleWheelSlotTest;

	FFixture Fixture;
	if (!Fixture.Build(*this)) return false;

	UVehicleWheelComponent* ManualLeft = Fixture.AddWheel(TEXT("SlotLayoutLeft"));
	UVehicleWheelComponent* ManualRight = Fixture.AddWheel(TEXT("SlotLayoutRight"));
	UVehicleAxleAssemblyComponent* Axle = Fixture.AddAxle(TEXT("SlotLayoutAxle"));
	FVehicleWheelSlotProbe::ConfigureManual(*Axle, TEXT("SlotLayoutLeft"), TEXT("SlotLayoutRight"));
	Axle->RegisterComponent();

	// A single-sided layout drops the other reference but must not destroy the component.
	FVehicleWheelSlotProbe::SetLayout(*Axle, EVehicleAxleLayout::SingleLeft);
	Axle->InitializeWheels();
	UVehicleWheelComponent* Left = nullptr;
	UVehicleWheelComponent* Right = nullptr;
	Axle->GetWheels(Left, Right);
	TestTrue(TEXT("single-left layout keeps only the left slot"), Left == ManualLeft);
	TestNull(TEXT("single-left layout releases the right slot"), Right);
	TestTrue(TEXT("the released right wheel is still alive and registered"),
		IsValid(ManualRight) && ManualRight->IsRegistered());

	// Switching back re-resolves the same component by name.
	FVehicleWheelSlotProbe::SetLayout(*Axle, EVehicleAxleLayout::TwoWheels);
	Axle->InitializeWheels();
	Axle->GetWheels(Left, Right);
	TestTrue(TEXT("two-wheel layout resolves both placed wheels again"), Left == ManualLeft && Right == ManualRight);
	TestEqual(TEXT("layout changes never duplicated wheels"), CountWheelComponents(Fixture.Actor), 2);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleWheelSlotGeneratedTest,
	"KinetiForge.Vehicle.WheelSlots.Generated",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleWheelSlotGeneratedTest::RunTest(const FString& Parameters)
{
	using namespace VehicleWheelSlotTest;

	FFixture Fixture;
	if (!Fixture.Build(*this)) return false;

	UVehicleAxleAssemblyComponent* Axle = Fixture.AddAxle(TEXT("SlotGeneratedAxle"));
	FVehicleWheelSlotProbe::ConfigureGenerated(*Axle);
	Axle->RegisterComponent();

	UVehicleWheelComponent* Left = nullptr;
	UVehicleWheelComponent* Right = nullptr;
	Axle->GetWheels(Left, Right);
	TestTrue(TEXT("generated slots create both wheels"), IsValid(Left) && IsValid(Right));
	TestTrue(TEXT("generated wheels are owned"), FVehicleWheelSlotProbe::IsLeftGenerated(*Axle)
		&& FVehicleWheelSlotProbe::IsRightGenerated(*Axle));
	TestTrue(TEXT("generated wheel names are deterministic"),
		Left->GetFName() == FName(TEXT("SlotGeneratedAxle_Wheel_L")));
	TestEqual(TEXT("generated wheel count"), CountWheelComponents(Fixture.Actor), 2);

	UVehicleWheelComponent* FirstLeft = Left;
	UVehicleWheelComponent* FirstRight = Right;
	for (int32 Refresh = 0; Refresh < 3; ++Refresh)
	{
		Axle->InitializeWheels();
	}
	Axle->GetWheels(Left, Right);
	TestTrue(TEXT("repeated refresh reuses the generated wheels"), Left == FirstLeft && Right == FirstRight);
	TestEqual(TEXT("repeated refresh does not duplicate wheels"), CountWheelComponents(Fixture.Actor), 2);

	// Generated wheels may be disposed of when the layout stops requiring them.
	FVehicleWheelSlotProbe::SetLayout(*Axle, EVehicleAxleLayout::SingleLeft);
	Axle->InitializeWheels();
	TestFalse(TEXT("an unneeded generated wheel is destroyed"), IsValid(FirstRight));
	TestEqual(TEXT("single-left layout leaves one generated wheel"), CountWheelComponents(Fixture.Actor), 1);

	FVehicleWheelSlotProbe::SetLayout(*Axle, EVehicleAxleLayout::TwoWheels);
	Axle->InitializeWheels();
	Axle->GetWheels(Left, Right);
	TestTrue(TEXT("the missing generated side is recreated"), IsValid(Left) && IsValid(Right));
	TestEqual(TEXT("recreating the missing side keeps one wheel per slot"), CountWheelComponents(Fixture.Actor), 2);
	// Keep multiple retired objects alive until GC. Their temporary names must not collide.
	TSet<FName> RetiredNames;
	RetiredNames.Add(FirstRight->GetFName());
	for (int32 Rebuild = 0; Rebuild < 3; ++Rebuild)
	{
		UVehicleWheelComponent* RetiredRight = Right;
		FVehicleWheelSlotProbe::SetLayout(*Axle, EVehicleAxleLayout::SingleLeft);
		Axle->InitializeWheels();
		TestFalse(TEXT("each removed wheel is destroyed"), IsValid(RetiredRight));
		TestFalse(TEXT("retirement names are unique before GC"), RetiredNames.Contains(RetiredRight->GetFName()));
		RetiredNames.Add(RetiredRight->GetFName());
		FVehicleWheelSlotProbe::SetLayout(*Axle, EVehicleAxleLayout::TwoWheels);
		Axle->InitializeWheels();
		Axle->GetWheels(Left, Right);
		TestTrue(TEXT("same-frame rebuild restores a live right wheel"), IsValid(Right) && Right != RetiredRight);
		TestEqual(TEXT("rebuilds keep deterministic active names"), Right->GetFName(), FName(TEXT("SlotGeneratedAxle_Wheel_R")));
		TestEqual(TEXT("rebuilds do not accumulate live wheels"), CountWheelComponents(Fixture.Actor), 2);
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleWheelSlotModeChangeTest,
	"KinetiForge.Vehicle.WheelSlots.ModeAndReferenceChanges",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleWheelSlotModeChangeTest::RunTest(const FString& Parameters)
{
	using namespace VehicleWheelSlotTest;
	FFixture Fixture;
	if (!Fixture.Build(*this)) return false;
	UVehicleWheelComponent* ManualLeft = Fixture.AddWheel(TEXT("ModeManualLeft"));
	UVehicleWheelComponent* ManualRight = Fixture.AddWheel(TEXT("ModeManualRight"));
	UVehicleWheelComponent* ReplacementLeft = Fixture.AddWheel(TEXT("ModeReplacementLeft"));
	ManualLeft->SetRelativeLocation(FVector(123.f, -213.f, 21.f));
	ManualRight->SetRelativeLocation(FVector(321.f, 312.f, 12.f));
	const FTransform LeftTransform = ManualLeft->GetRelativeTransform();
	const FTransform RightTransform = ManualRight->GetRelativeTransform();
	FVehicleWheelConfig ManualConfig = ManualLeft->GetWheelConfig();
	ManualConfig.Radius = 47.f;
	ManualLeft->SetWheelConfig(ManualConfig);
	UVehicleAxleAssemblyComponent* Axle = Fixture.AddAxle(TEXT("ModeAxle"));
	FVehicleWheelSlotProbe::ConfigureManual(*Axle, ManualLeft->GetFName(), ManualRight->GetFName());
	Axle->RegisterComponent();
	UVehicleWheelComponent* Left = nullptr;
	UVehicleWheelComponent* Right = nullptr;

	for (int32 Switch = 0; Switch < 3; ++Switch)
	{
		FVehicleWheelSlotProbe::ConfigureGenerated(*Axle);
		Axle->InitializeWheels();
		Axle->GetWheels(Left, Right);
		if (!TestTrue(TEXT("generated mode replaces both external slot references"),
			IsValid(Left) && IsValid(Right) && Left != ManualLeft && Right != ManualRight)) return false;
		TestTrue(TEXT("new generated slots are owned"), FVehicleWheelSlotProbe::IsLeftGenerated(*Axle)
			&& FVehicleWheelSlotProbe::IsRightGenerated(*Axle));
		TestEqual(TEXT("external wheels coexist with exactly two generated wheels"), CountWheelComponents(Fixture.Actor), 5);
		UVehicleWheelComponent* GeneratedLeft = Left;
		UVehicleWheelComponent* GeneratedRight = Right;
		FVehicleWheelSlotProbe::ConfigureManual(*Axle, ManualLeft->GetFName(), ManualRight->GetFName());
		Axle->InitializeWheels();
		Axle->GetWheels(Left, Right);
		TestTrue(TEXT("returning to referenced mode resolves original wheels"), Left == ManualLeft && Right == ManualRight);
		TestFalse(TEXT("previous generated wheels are retired"), IsValid(GeneratedLeft) || IsValid(GeneratedRight));
		TestEqual(TEXT("switching back keeps only the three authored wheels"), CountWheelComponents(Fixture.Actor), 3);
		TestTrue(TEXT("manual wheel transforms remain authored"), ManualLeft->GetRelativeTransform().Equals(LeftTransform)
			&& ManualRight->GetRelativeTransform().Equals(RightTransform));
		TestEqual(TEXT("manual wheel radius is not overwritten by a generated template"), ManualLeft->GetWheelConfig().Radius, 47.f);
	}

	// Exercise the public template-copy entry: a valid old reference must not mask a changed name.
	UVehicleAxleAssemblyComponent* ConfigSource = Fixture.AddAxle(TEXT("ModeConfigSource"));
	FVehicleWheelSlotProbe::ConfigureManual(*ConfigSource, ReplacementLeft->GetFName(), ManualRight->GetFName());
	auto PreviousBinding = MakeShared<FVehiclePhysicsStep, ESPMode::ThreadSafe>([](float) {});
	FVehicleWheelSlotProbe::ClaimAxle(*Axle, PreviousBinding);
	UVehicleAxleAssemblyComponent::CopyAxleConfig(ConfigSource, Axle, true);
	TestTrue(TEXT("changing wheel references invalidates the captured drive even on the same chassis"),
		PreviousBinding->IsStopped() && PreviousBinding->IsBindingInvalidated());
	TestFalse(TEXT("old captured drive cannot execute after slot replacement"), PreviousBinding->TryExecute(1.f / 120.f));
	Axle->GetWheels(Left, Right);
	TestTrue(TEXT("copying a changed reference name replaces only the requested slot"), Left == ReplacementLeft && Right == ManualRight);
	TestTrue(TEXT("released manual reference remains registered"), IsValid(ManualLeft) && ManualLeft->IsRegistered());
	FVehicleWheelSlotProbe::ConfigureManual(*ConfigSource, NAME_None, TEXT("MissingWheel"));
	UVehicleAxleAssemblyComponent::CopyAxleConfig(ConfigSource, Axle, true);
	Axle->GetWheels(Left, Right);
	TestNull(TEXT("clearing a configured name releases its old reference"), Left);
	TestNull(TEXT("an unresolved name does not retain the previously valid reference"), Right);
	TestEqual(TEXT("name changes never destroy or duplicate authored wheels"), CountWheelComponents(Fixture.Actor), 3);
	return true;
}

#endif
