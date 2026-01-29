// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)


#include "VehicleWheelCoordinatorComponent.h"
#include "VehicleWheelComponent.h"
#include "VehicleAxleAssemblyComponent.h"
#include "VehicleDriveAssemblyComponent.h"

DEFINE_LOG_CATEGORY(LogWheelCoordinator);

// Sets default values for this component's properties
UVehicleWheelCoordinatorComponent::UVehicleWheelCoordinatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_PostPhysics;

	// ...
}


// Called when the game starts
void UVehicleWheelCoordinatorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//FindCarbody();

	TimeSinceLastRefresh = FMath::FRandRange(0.f, RefreshInterval);
}

void UVehicleWheelCoordinatorComponent::OnRegister()
{
	Super::OnRegister();
	Carbody = FindPhysicalParent(this);
}

void UVehicleWheelCoordinatorComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	//...
	Carbody = nullptr;

	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

bool UVehicleWheelCoordinatorComponent::UpdateWheelSprungMass()
{
	//remove unavaliable wheels
	RegisteredWheels.RemoveAll([](const TWeakObjectPtr<UVehicleWheelComponent>& W) {return !W.IsValid() || W->IsBeingDestroyed();});

	//get wheel positions
	TArray<FVector3f> Positions;
	for (TWeakObjectPtr<UVehicleWheelComponent> Wheel : RegisteredWheels)
	{
		FVector3f RelativePos = Wheel->GetTopMountRelativeLocation();
		Positions.Add(RelativePos);
	}

	TArray<float> SprungMasses;
	if (ComputeSprungMasses(Positions, CarbodyCOM, Carbody->GetMass(), SprungMasses))
	{
		for (int32 i = 0; i < RegisteredWheels.Num(); i++)
		{
			RegisteredWheels[i]->SetSprungMass(SprungMasses[i]);
		}
		return true;
	}
	else
	{
		for (TWeakObjectPtr<UVehicleWheelComponent> Wheel : RegisteredWheels)
		{
			Wheel->SetSprungMass(0);
		}
		return false;
	}
}

void UVehicleWheelCoordinatorComponent::UpdateWheelBase()
{
	//remove unvalid axles
	RegisteredAxles.RemoveAll([](const TWeakObjectPtr<UVehicleAxleAssemblyComponent>& A) {return !A.IsValid() || A->IsBeingDestroyed();});

	//find the center of all axles
	FVector3f AveragePos = FVector3f(0.f);
	for (TWeakObjectPtr<UVehicleAxleAssemblyComponent> Axle : RegisteredAxles)
	{
		AveragePos += Axle->GetAxleCenter();
	}
	int32 NumOfAxles = RegisteredAxles.Num();
	if (!NumOfAxles)return;
	AveragePos = AveragePos / NumOfAxles;
	for (TWeakObjectPtr<UVehicleAxleAssemblyComponent> Axle : RegisteredAxles)
	{
		Axle->SetWheelBase((AveragePos - (FVector3f)Axle->GetRelativeLocation()).Length() * 2);
	}
}

// Called every frame
void UVehicleWheelCoordinatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//...
	
	TimeSinceLastRefresh += DeltaTime;
	if (TimeSinceLastRefresh > RefreshInterval)
	{
		TimeSinceLastRefresh -= RefreshInterval;
		if (UPrimitiveComponent* CarbodyRaw = Carbody.Get())
		{
			if (CarbodyRaw->IsPhysicsStateCreated())
			{
				//check if center of mass changed
				FVector3f NewCarbodyLocalCOM = (FVector3f)CarbodyRaw->GetComponentTransform().InverseTransformPosition(CarbodyRaw->GetCenterOfMass());
				if ((CarbodyCOM - NewCarbodyLocalCOM).SquaredLength() > 1.f)
				{
					CarbodyCOM = NewCarbodyLocalCOM;
					bMassMatrixDirty = true;
				}
			}
		}
	}

	// check if sprungmass changed
	if (bMassMatrixDirty)
	{
		bMassMatrixDirty = false;
		UpdateWheelSprungMass();
	}

	// check if wheel base changed
	if (bWheelBaseDataDirty)
	{
		bWheelBaseDataDirty = false;
		UpdateWheelBase();
	}
}

UPrimitiveComponent* UVehicleWheelCoordinatorComponent::FindPhysicalParent(USceneComponent* ChildSceneComponent)
{
	if (!IsValid(ChildSceneComponent))return nullptr;

	TArray<USceneComponent*> AllParentComponents;
	ChildSceneComponent->GetParentComponents(AllParentComponents);

	// search for all possible components
	for (USceneComponent* SceneComp : AllParentComponents)
	{
		if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(SceneComp))
		{
			return Primitive;
		}
	}

	// Fallback: try to find root component (usually primitive)
	if (AActor* CompOwner = ChildSceneComponent->GetOwner())
	{
		if (UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(CompOwner->GetRootComponent()))
		{
			return RootPrimitive;
		}
	}

	return nullptr;
}

UVehicleWheelCoordinatorComponent* UVehicleWheelCoordinatorComponent::FindWheelCoordinator(USceneComponent* Carbody)
{
	//ckeck valid carbody
	if (!IsValid(Carbody))
	{
		return nullptr;
	}

	TArray<USceneComponent*> Children;
	Carbody->GetChildrenComponents(true, Children);
	for (USceneComponent* Child : Children)
	{
		//if found
		if (UVehicleWheelCoordinatorComponent* WheelCoord = Cast<UVehicleWheelCoordinatorComponent>(Child))
		{
			return WheelCoord;
		}
	}

	//if not found
	//create one
	UVehicleWheelCoordinatorComponent* WheelCoord = Cast<UVehicleWheelCoordinatorComponent>
		(Carbody->GetOwner()->AddComponentByClass(UVehicleWheelCoordinatorComponent::StaticClass(), false, FTransform(), false));
		NewObject<UVehicleWheelCoordinatorComponent>(Carbody);
	return WheelCoord;
}

void UVehicleWheelCoordinatorComponent::NotifyWheelMoved()
{
	bMassMatrixDirty = true;
	if (RegisteredAxles.Num() > 0)
	{
		bWheelBaseDataDirty = true;
	}
}

void UVehicleWheelCoordinatorComponent::RegisterWheel(UVehicleWheelComponent* NewWheel)
{
	RegisteredWheels.Add(NewWheel);
	bMassMatrixDirty = true;
}

void UVehicleWheelCoordinatorComponent::RegisterAxle(UVehicleAxleAssemblyComponent* NewAxle)
{
	RegisteredAxles.Add(NewAxle);
	bWheelBaseDataDirty = true;
}

void UVehicleWheelCoordinatorComponent::RegisterDriveAssembly(UVehicleDriveAssemblyComponent* NewVehicleDriveAssemblyComponent)
{
	RegisteredDriveAssemblies.Add(NewVehicleDriveAssemblyComponent);
}

bool UVehicleWheelCoordinatorComponent::ComputeSprungMasses(const TArray<FVector3f>& MassSpringPositions, const float TotalMass, TArray<float>& OutSprungMasses)
{
	// 1. Validation & Initialization
	const int32 NumSprings = MassSpringPositions.Num();
	OutSprungMasses.Init(0.f, NumSprings);

	if (NumSprings <= 0)
	{
		// Log warning: No springs defined.
		return false;
	}

	if (TotalMass <= SMALL_NUMBER)
	{
		// Log warning: Invalid total mass.
		return false;
	}

	// -------------------------------------------------------------------------
	// Case I: Single Support Point
	// -------------------------------------------------------------------------
	if (NumSprings == 1)
	{
		OutSprungMasses[0] = TotalMass;
		return true;
	}

	// -------------------------------------------------------------------------
	// Case II: Two Support Points (Line Segment Projection)
	// -------------------------------------------------------------------------
	else if (NumSprings == 2)
	{
		/*
		 * Logic:
		 * We project the Center of Mass (which is at local origin 0,0) onto the
		 * line segment defined by the two springs. The mass is then distributed
		 * based on the inverse distance ratio (Lever Rule).
		 */

		const FVector3f& PointA = MassSpringPositions[0];
		const FVector3f& PointB = MassSpringPositions[1];

		const float DiffX = PointB.X - PointA.X;
		const float DiffY = PointB.Y - PointA.Y;

		// Calculate distance between springs
		const float DistanceSq = (DiffX * DiffX) + (DiffY * DiffY);
		const float Distance = FMath::Sqrt(DistanceSq);

		// Handle case where springs are overlapping
		if (Distance <= SMALL_NUMBER)
		{
			OutSprungMasses[0] = TotalMass * 0.5f;
			OutSprungMasses[1] = TotalMass * 0.5f;
			return true;
		}

		// Project CoM onto the vector AB
		const float InvDistance = 1.f / Distance;
		const float UnitDirX = DiffX * InvDistance;
		const float UnitDirY = DiffY * InvDistance;

		// Calculate projection scalar (Dot Product)
		// Note: PointA is relative to CoM.
		const float Projection = (PointA.X * UnitDirX) + (PointA.Y * UnitDirY);

		// Calculate mass for the first point based on the lever arm
		OutSprungMasses[0] = -TotalMass * Projection * InvDistance;
		OutSprungMasses[1] = TotalMass - OutSprungMasses[0];

		// Validation: Masses must be non-negative (CoM must be between springs)
		if (OutSprungMasses[0] >= 0.f && OutSprungMasses[1] >= 0.f)
		{
			return true;
		}

		// Fallback for invalid CoM placement
		return false;
	}

	// -------------------------------------------------------------------------
	// Case III: Multiple Support Points (N >= 3)
	// -------------------------------------------------------------------------
	/*
	 * Logic:
	 * Solve a constrained optimization problem using Lagrange Multipliers.
	 * Objective: Minimize mass variance across springs.
	 * Constraints:
	 * 1. Sum of masses = TotalMass
	 * 2. Sum of moments (Torque) = 0
	 *
	 * This results in a linear system A*x = B, solved via matrix inversion.
	 */

	const float CountN = (float)NumSprings;
	const float MeanMass = TotalMass / CountN;

	// Step A: Calculate Statistical Moments (Covariance terms)
	float SumX = 0.f;
	float SumY = 0.f;
	float SumSqX = 0.f; // Sum of X^2
	float SumSqY = 0.f; // Sum of Y^2
	float SumXY = 0.f;  // Sum of X*Y

	for (const FVector3f& Pos : MassSpringPositions)
	{
		SumX += Pos.X;
		SumY += Pos.Y;
		SumSqX += Pos.X * Pos.X;
		SumSqY += Pos.Y * Pos.Y;
		SumXY += Pos.X * Pos.Y;
	}

	// Step B: Compute Determinant of the Coefficient Matrix
	// Matrix Form:
	// | SumSqX  SumXY   SumX |
	// | SumXY   SumSqY  SumY |
	// | SumX    SumY    N    |

	const float Term1 = SumSqX * SumSqY * CountN;
	const float Term2 = 2.f * SumXY * SumX * SumY;
	const float Term3 = SumSqY * SumX * SumX;
	const float Term4 = SumSqX * SumY * SumY;
	const float Term5 = SumXY * SumXY * CountN;

	const float Determinant = Term1 + Term2 - Term3 - Term4 - Term5;

	// Check for singularity (e.g., collinear points)
	if (FMath::Abs(Determinant) < SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Warning, TEXT("ComputeSprungMasses: Solver failed. Springs may be collinear or overlapping."));
		return false;
	}

	// Step C: Calculate Inverse Matrix Elements (Symmetric 3x3)
	// We only need the upper triangle values due to symmetry.
	const float InvDet = 1.f / Determinant;

	const float InvM00 = (CountN * SumSqY - SumY * SumY) * InvDet;
	const float InvM01 = (SumX * SumY - CountN * SumXY) * InvDet;
	const float InvM02 = (SumY * SumXY - SumX * SumSqY) * InvDet;

	// const float InvM10 = InvM01;
	const float InvM11 = (CountN * SumSqX - SumX * SumX) * InvDet;
	const float InvM12 = (SumX * SumXY - SumY * SumSqX) * InvDet;

	// const float InvM20 = InvM02;
	// const float InvM21 = InvM12;
	const float InvM22 = (SumSqX * SumSqY - SumXY * SumXY) * InvDet;

	// Step D: Solve for Lagrange Multipliers
	// Right-Hand Side (RHS) vector based on constraints
	const float RHS_X = 2.f * MeanMass * SumX;
	const float RHS_Y = 2.f * MeanMass * SumY;
	const float RHS_C = (2.f * MeanMass * CountN) - (2.f * TotalMass);

	const float Lambda0 = (InvM00 * RHS_X) + (InvM01 * RHS_Y) + (InvM02 * RHS_C);
	const float Lambda1 = (InvM01 * RHS_X) + (InvM11 * RHS_Y) + (InvM12 * RHS_C);
	const float Lambda2 = (InvM02 * RHS_X) + (InvM12 * RHS_Y) + (InvM22 * RHS_C);

	// Step E: Compute Final Mass Distribution
	for (int32 i = 0; i < NumSprings; ++i)
	{
		const float PosX = MassSpringPositions[i].X;
		const float PosY = MassSpringPositions[i].Y;

		// Formula: m_i = Mean - 0.5 * (Lambda dot Position)
		const float CorrectionTerm = (PosX * Lambda0) + (PosY * Lambda1) + Lambda2;
		OutSprungMasses[i] = MeanMass - (0.5f * CorrectionTerm);

		// Physical Sanity Check: Mass cannot be negative.
		if (OutSprungMasses[i] < 0.f)
		{
			UE_LOG(LogTemp, Warning, TEXT("ComputeSprungMasses: Center of Mass is outside the support polygon. Negative mass calculated."));
			return false;
		}
	}

	return true;
}

bool UVehicleWheelCoordinatorComponent::ComputeSprungMasses(const TArray<FVector3f>& LocalSpringPositions, const FVector3f& LocalCenterOfMass, const float TotalMass, TArray<float>& OutSprungMasses)
{
	// Transform spring positions to be relative to the Center of Mass
	const int32 Count = LocalSpringPositions.Num();
	TArray<FVector3f> RelativePositions;
	RelativePositions.SetNumUninitialized(Count);

	for (int32 i = 0; i < Count; ++i)
	{
		RelativePositions[i] = LocalSpringPositions[i] - LocalCenterOfMass;
	}

	// Delegate to the core solver
	return ComputeSprungMasses(RelativePositions, TotalMass, OutSprungMasses);
}


