// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#if WITH_DEV_AUTOMATION_TESTS

#include "VehicleDriveAssemblyComponent.h"
#include "VehicleAxleAssemblyComponent.h"
#include "VehicleWheelComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Misc/AutomationTest.h"

namespace
{
	struct FEditorPreviewWorldGuard
	{
		UWorld* World = nullptr;
		~FEditorPreviewWorldGuard()
		{
			if (World) World->DestroyWorld(false);
		}
	};

	int32 CountComponentsOfClass(AActor* Actor, UClass* ComponentClass)
	{
		TInlineComponentArray<UActorComponent*> Components;
		Actor->GetComponents(Components);
		int32 Count = 0;
		for (UActorComponent* Component : Components)
		{
			Count += Component && Component->IsA(ComponentClass);
		}
		return Count;
	}

	struct FPreviewAxleSnapshot
	{
		UVehicleAxleAssemblyComponent* Axle = nullptr;
		FName AxleName;
		FVector AxleRelativeLocation = FVector::ZeroVector;
		USceneComponent* AxleParent = nullptr;
		UVehicleWheelComponent* LeftWheel = nullptr;
		UVehicleWheelComponent* RightWheel = nullptr;
		FName LeftWheelName;
		FName RightWheelName;
		USceneComponent* LeftWheelParent = nullptr;
		USceneComponent* RightWheelParent = nullptr;
		FVector LeftWheelRelativeLocation = FVector::ZeroVector;
		FVector RightWheelRelativeLocation = FVector::ZeroVector;
		USceneComponent* LeftHub = nullptr;
		USceneComponent* RightHub = nullptr;
		UStaticMeshComponent* LeftMesh = nullptr;
		UStaticMeshComponent* RightMesh = nullptr;
		FVector LeftHubRelativeLocation = FVector::ZeroVector;
		FVector RightHubRelativeLocation = FVector::ZeroVector;
		FVector LeftMeshRelativeLocation = FVector::ZeroVector;
		FVector RightMeshRelativeLocation = FVector::ZeroVector;
	};

	FPreviewAxleSnapshot CaptureAxle(UVehicleAxleAssemblyComponent* Axle)
	{
		FPreviewAxleSnapshot Snapshot;
		Snapshot.Axle = Axle;
		Snapshot.AxleName = Axle->GetFName();
		Snapshot.AxleRelativeLocation = Axle->GetRelativeLocation();
		Snapshot.AxleParent = Axle->GetAttachParent();
		Axle->GetWheels(Snapshot.LeftWheel, Snapshot.RightWheel);
		if (Snapshot.LeftWheel)
		{
			Snapshot.LeftWheelRelativeLocation = Snapshot.LeftWheel->GetRelativeLocation();
			Snapshot.LeftWheelName = Snapshot.LeftWheel->GetFName();
			Snapshot.LeftWheelParent = Snapshot.LeftWheel->GetAttachParent();
			Snapshot.LeftHub = Snapshot.LeftWheel->GetWheelHubComponent();
			Snapshot.LeftMesh = Snapshot.LeftWheel->GetWheelMeshComponent();
			if (Snapshot.LeftHub) Snapshot.LeftHubRelativeLocation = Snapshot.LeftHub->GetRelativeLocation();
			if (Snapshot.LeftMesh) Snapshot.LeftMeshRelativeLocation = Snapshot.LeftMesh->GetRelativeLocation();
		}
		if (Snapshot.RightWheel)
		{
			Snapshot.RightWheelRelativeLocation = Snapshot.RightWheel->GetRelativeLocation();
			Snapshot.RightWheelName = Snapshot.RightWheel->GetFName();
			Snapshot.RightWheelParent = Snapshot.RightWheel->GetAttachParent();
			Snapshot.RightHub = Snapshot.RightWheel->GetWheelHubComponent();
			Snapshot.RightMesh = Snapshot.RightWheel->GetWheelMeshComponent();
			if (Snapshot.RightHub) Snapshot.RightHubRelativeLocation = Snapshot.RightHub->GetRelativeLocation();
			if (Snapshot.RightMesh) Snapshot.RightMeshRelativeLocation = Snapshot.RightMesh->GetRelativeLocation();
		}
		return Snapshot;
	}

	void TestAxleSnapshot(FAutomationTestBase& Test, const FPreviewAxleSnapshot& Expected,
		UVehicleAxleAssemblyComponent* ActualAxle, const TCHAR* Label)
	{
		Test.TestTrue(FString::Printf(TEXT("%s axle pointer stable"), Label), ActualAxle == Expected.Axle);
		Test.TestTrue(FString::Printf(TEXT("%s axle name stable"), Label), ActualAxle->GetFName() == Expected.AxleName);
		Test.TestTrue(FString::Printf(TEXT("%s axle location preserved"), Label), ActualAxle->GetRelativeLocation().Equals(Expected.AxleRelativeLocation));
		Test.TestTrue(FString::Printf(TEXT("%s axle parent preserved"), Label), ActualAxle->GetAttachParent() == Expected.AxleParent);
		UVehicleWheelComponent* LeftWheel = nullptr;
		UVehicleWheelComponent* RightWheel = nullptr;
		ActualAxle->GetWheels(LeftWheel, RightWheel);
		Test.TestTrue(FString::Printf(TEXT("%s wheel identities stable"), Label), LeftWheel == Expected.LeftWheel && RightWheel == Expected.RightWheel);
		Test.TestTrue(FString::Printf(TEXT("%s wheel locations preserved"), Label),
			LeftWheel && RightWheel && LeftWheel->GetRelativeLocation().Equals(Expected.LeftWheelRelativeLocation)
			&& RightWheel->GetRelativeLocation().Equals(Expected.RightWheelRelativeLocation));
		Test.TestTrue(FString::Printf(TEXT("%s wheel names and parents preserved"), Label),
			LeftWheel && RightWheel && LeftWheel->GetFName() == Expected.LeftWheelName && RightWheel->GetFName() == Expected.RightWheelName
			&& LeftWheel->GetAttachParent() == Expected.LeftWheelParent && RightWheel->GetAttachParent() == Expected.RightWheelParent);
		USceneComponent* LeftHub = LeftWheel ? LeftWheel->GetWheelHubComponent() : nullptr;
		USceneComponent* RightHub = RightWheel ? RightWheel->GetWheelHubComponent() : nullptr;
		USceneComponent* LeftMesh = LeftWheel ? LeftWheel->GetWheelMeshComponent() : nullptr;
		USceneComponent* RightMesh = RightWheel ? RightWheel->GetWheelMeshComponent() : nullptr;
		Test.TestTrue(FString::Printf(TEXT("%s left hub valid and stable"), Label), IsValid(Expected.LeftHub) && LeftHub == Expected.LeftHub);
		Test.TestTrue(FString::Printf(TEXT("%s right hub valid and stable"), Label), IsValid(Expected.RightHub) && RightHub == Expected.RightHub);
		Test.TestTrue(FString::Printf(TEXT("%s left mesh valid and stable"), Label), IsValid(Expected.LeftMesh) && LeftMesh == Expected.LeftMesh);
		Test.TestTrue(FString::Printf(TEXT("%s right mesh valid and stable"), Label), IsValid(Expected.RightMesh) && RightMesh == Expected.RightMesh);
		Test.TestTrue(FString::Printf(TEXT("%s left hub parent preserved"), Label), IsValid(LeftHub) && LeftHub->GetAttachParent() == LeftWheel);
		Test.TestTrue(FString::Printf(TEXT("%s right hub parent preserved"), Label), IsValid(RightHub) && RightHub->GetAttachParent() == RightWheel);
		Test.TestTrue(FString::Printf(TEXT("%s left mesh parent preserved"), Label), IsValid(LeftMesh) && LeftMesh->GetAttachParent() == LeftHub);
		Test.TestTrue(FString::Printf(TEXT("%s right mesh parent preserved"), Label), IsValid(RightMesh) && RightMesh->GetAttachParent() == RightHub);
		Test.TestTrue(FString::Printf(TEXT("%s left hub location preserved"), Label), IsValid(LeftHub) && LeftHub->GetRelativeLocation().Equals(Expected.LeftHubRelativeLocation));
		Test.TestTrue(FString::Printf(TEXT("%s right hub location preserved"), Label), IsValid(RightHub) && RightHub->GetRelativeLocation().Equals(Expected.RightHubRelativeLocation));
		Test.TestTrue(FString::Printf(TEXT("%s left mesh location preserved"), Label), IsValid(LeftMesh) && LeftMesh->GetRelativeLocation().Equals(Expected.LeftMeshRelativeLocation));
		Test.TestTrue(FString::Printf(TEXT("%s right mesh location preserved"), Label), IsValid(RightMesh) && RightMesh->GetRelativeLocation().Equals(Expected.RightMeshRelativeLocation));
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehicleRegistrationEditorPreviewTest,
	"KinetiForge.Drivetrain.Registration.EditorPreview",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehicleRegistrationEditorPreviewTest::RunTest(const FString& Parameters)
{
	FEditorPreviewWorldGuard WorldGuard;
	WorldGuard.World = UWorld::CreateWorld(EWorldType::EditorPreview, false, NAME_None, nullptr, true, ERHIFeatureLevel::Num, nullptr, true);
	if (!TestNotNull(TEXT("editor preview world created"), WorldGuard.World)) return false;
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

	AActor* Owner = WorldGuard.World->SpawnActor<AActor>();
	if (!TestNotNull(TEXT("preview actor spawned"), Owner)) return false;
	UBoxComponent* Chassis = NewObject<UBoxComponent>(Owner, TEXT("PreviewChassis"));
	Owner->AddInstanceComponent(Chassis);
	Owner->SetRootComponent(Chassis);
	Chassis->SetSimulatePhysics(false);
	Chassis->RegisterComponent();
	TestTrue(TEXT("registered primitive is the actor root"), Chassis->IsRegistered() && Owner->GetRootComponent() == Chassis);

	UVehicleDriveAssemblyComponent* Drive = NewObject<UVehicleDriveAssemblyComponent>(Owner, TEXT("PreviewDrive"));
	Owner->AddInstanceComponent(Drive);
	Drive->SetupAttachment(Chassis);
	FVehicleAxleSpawnTemplate FrontTemplate;
	FrontTemplate.AxlePosition = FVector(150.f, 0.f, 0.f);
	FrontTemplate.TrackWidthOverride = 174.f;
	FVehicleAxleSpawnTemplate RearTemplate;
	RearTemplate.AxlePosition = FVector(-150.f, 0.f, 0.f);
	RearTemplate.TrackWidthOverride = 162.f;
	Drive->AxleTemplates = { FrontTemplate, RearTemplate };
	Drive->RegisterComponent();
	TestTrue(TEXT("drive registered for editor preview"), Drive->IsRegistered());

	TArray<UVehicleAxleAssemblyComponent*> Axles = Drive->GetAxles();
	TestEqual(TEXT("two axle templates immediately create two axles"), Axles.Num(), 2);
	if (Axles.Num() != 2) return false;
	TArray<FPreviewAxleSnapshot> InitialSnapshots;
	for (int32 Index = 0; Index < Axles.Num(); ++Index)
	{
		UVehicleAxleAssemblyComponent* Axle = Axles[Index];
		TestNotNull(TEXT("preview axle valid"), Axle);
		TestTrue(TEXT("preview axle attached under chassis"), Axle && Axle->GetAttachParent() == Chassis);
		TestTrue(TEXT("track override applied"), Axle && Axle->GetAxleConfig().TrackWidth == (Index == 0 ? 174.f : 162.f));
		UVehicleWheelComponent* LeftWheel = nullptr;
		UVehicleWheelComponent* RightWheel = nullptr;
		Axle->GetWheels(LeftWheel, RightWheel);
		TestTrue(TEXT("preview wheels are immediately available"), IsValid(LeftWheel) && IsValid(RightWheel));
		if (LeftWheel && RightWheel)
		{
			TestTrue(TEXT("wheel hubs and mesh components are immediately available"),
				IsValid(LeftWheel->GetWheelHubComponent()) && IsValid(RightWheel->GetWheelHubComponent())
				&& IsValid(LeftWheel->GetWheelMeshComponent()) && IsValid(RightWheel->GetWheelMeshComponent()));
			TestTrue(TEXT("wheel objects attach to chassis"), LeftWheel->GetAttachParent() == Chassis && RightWheel->GetAttachParent() == Chassis);
		}
		InitialSnapshots.Add(CaptureAxle(Axle));
	}
	const int32 InitialAxleCount = CountComponentsOfClass(Owner, UVehicleAxleAssemblyComponent::StaticClass());
	const int32 InitialWheelCount = CountComponentsOfClass(Owner, UVehicleWheelComponent::StaticClass());
	TestEqual(TEXT("generated axle component count"), InitialAxleCount, 2);
	TestEqual(TEXT("generated wheel component count"), InitialWheelCount, 4);

	for (int32 Cycle = 0; Cycle < 2; ++Cycle)
	{
		Drive->UnregisterComponent();
		Drive->RegisterComponent();
		for (const FPreviewAxleSnapshot& Snapshot : InitialSnapshots)
		{
			TestAxleSnapshot(*this, Snapshot, Snapshot.Axle, TEXT("drive register cycle"));
		}
		TestEqual(TEXT("drive re-register does not duplicate axles"), CountComponentsOfClass(Owner, UVehicleAxleAssemblyComponent::StaticClass()), InitialAxleCount);
		TestEqual(TEXT("drive re-register does not duplicate wheels"), CountComponentsOfClass(Owner, UVehicleWheelComponent::StaticClass()), InitialWheelCount);

		for (const FPreviewAxleSnapshot& Snapshot : InitialSnapshots)
		{
			Snapshot.Axle->UnregisterComponent();
			Snapshot.Axle->RegisterComponent();
			TestAxleSnapshot(*this, Snapshot, Snapshot.Axle, TEXT("axle register cycle"));
			// A wheel's own OnRegister path calls InitializeWheel(), which solves that wheel's
			// designed suspension pose. For a solid axle this differs from the axle-level
			// postprocessed pose, so capture the explicit wheel-initialization baseline here.
			Snapshot.LeftWheel->InitializeWheel();
			Snapshot.RightWheel->InitializeWheel();
			const FPreviewAxleSnapshot WheelInitializationBaseline = CaptureAxle(Snapshot.Axle);
			Snapshot.LeftWheel->UnregisterComponent();
			Snapshot.LeftWheel->RegisterComponent();
			Snapshot.RightWheel->UnregisterComponent();
			Snapshot.RightWheel->RegisterComponent();
			TestAxleSnapshot(*this, WheelInitializationBaseline, Snapshot.Axle, TEXT("wheel register cycle"));
		}
		TestEqual(TEXT("axle/wheel cycles keep axle count"), CountComponentsOfClass(Owner, UVehicleAxleAssemblyComponent::StaticClass()), InitialAxleCount);
		TestEqual(TEXT("axle/wheel cycles keep wheel count"), CountComponentsOfClass(Owner, UVehicleWheelComponent::StaticClass()), InitialWheelCount);
	}

	// Re-registering after an authored template edit must reuse the same generated objects and refresh positions.
	Drive->AxleTemplates[0].AxlePosition = FVector(230.f, 0.f, 0.f);
	Drive->AxleTemplates[1].AxlePosition = FVector(-230.f, 0.f, 0.f);
	Drive->UnregisterComponent();
	Drive->RegisterComponent();
	Axles = Drive->GetAxles();
	TestEqual(TEXT("template edit still has exactly two axles"), Axles.Num(), 2);
	if (Axles.Num() == 2)
	{
		TestTrue(TEXT("front preview location refreshed"), Axles[0] == InitialSnapshots[0].Axle && Axles[0]->GetRelativeLocation().Equals(FVector(230.f, 0.f, 0.f)));
		TestTrue(TEXT("rear preview location refreshed"), Axles[1] == InitialSnapshots[1].Axle && Axles[1]->GetRelativeLocation().Equals(FVector(-230.f, 0.f, 0.f)));
	}
	TestEqual(TEXT("template edit does not duplicate wheels"), CountComponentsOfClass(Owner, UVehicleWheelComponent::StaticClass()), InitialWheelCount);

	// A manually owned axle referenced by a template remains outside Drive's generated-component cleanup list.
	UVehicleAxleAssemblyComponent* ManualAxle = NewObject<UVehicleAxleAssemblyComponent>(Owner, TEXT("UserManualAxle"));
	Owner->AddInstanceComponent(ManualAxle);
	ManualAxle->SetupAttachment(Chassis);
	ManualAxle->RegisterComponent();
	UVehicleWheelComponent* ManualLeftWheel = nullptr;
	UVehicleWheelComponent* ManualRightWheel = nullptr;
	ManualAxle->GetWheels(ManualLeftWheel, ManualRightWheel);
	FVehicleAxleSpawnTemplate ManualTemplate;
	ManualTemplate.bUseExistingComponent = true;
	ManualTemplate.AxleComponentName = ManualAxle->GetFName();
	Drive->AxleTemplates.Add(ManualTemplate);
	for (int32 ManualCycle = 0; ManualCycle < 2; ++ManualCycle)
	{
		Drive->UnregisterComponent();
		Drive->RegisterComponent();
		TestTrue(TEXT("manual axle remains the actor component named by the template"),
			IsValid(ManualAxle) && FindObject<UVehicleAxleAssemblyComponent>(Owner, *ManualTemplate.AxleComponentName.ToString()) == ManualAxle);
	}
	Drive->DestroyComponent();
	TestTrue(TEXT("destroying Drive preserves manual axle"), IsValid(ManualAxle));
	TestTrue(TEXT("destroying Drive preserves manual axle wheels"), IsValid(ManualLeftWheel) && IsValid(ManualRightWheel));
	TestEqual(TEXT("manual axle survives generated cleanup"), CountComponentsOfClass(Owner, UVehicleAxleAssemblyComponent::StaticClass()), 1);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
