// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehiclePowerUnitRuntime.h"
#include "VehicleEngineComponent.h"
#include "VehicleClutchComponent.h"
#include "VehicleGearboxComponent.h"

#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "UObject/StrongObjectPtr.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVehiclePowerUnitRuntimeOrderTest,
	"KinetiForge.Drivetrain.Runtime.PowerUnitOrder",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVehiclePowerUnitRuntimeOrderTest::RunTest(const FString& Parameters)
{
	TStrongObjectPtr<UVehicleEngineComponent> Engine(NewObject<UVehicleEngineComponent>());
	TStrongObjectPtr<UVehicleClutchComponent> Clutch(NewObject<UVehicleClutchComponent>());
	TStrongObjectPtr<UVehicleGearboxComponent> Gearbox(NewObject<UVehicleGearboxComponent>());
	FVehicleNaturallyAspiratedEngineConfig EngineConfig;
	EngineConfig.EngineTorqueCurve = nullptr; // Explicit flat curve, no asset dependency.
	Engine->SetNAConfig(EngineConfig);
	FVehiclePowerUnitRuntime Runtime;
	Runtime.EngineParameters = FVehicleEngineParameters::FromConfig(EngineConfig);
	Runtime.GearboxConfig.FirstGear = 3.f;
	Runtime.GearboxConfig.TopGear = 1.f;
	Runtime.GearboxConfig.NumberOfGears = 2;
	Runtime.GearboxConfig.NumOfReverseGears = 1;
	Runtime.GearboxConfig.Efficiency = 0.9f;
	Gearbox->SetConfig(Runtime.GearboxConfig);
	Gearbox->CalculateGearRatios();
	TestTrue(TEXT("Value-owned runtime initializes"), Runtime.Initialize());
	// Match the unregistered legacy fixture, whose BeginPlay has not reset the limiter.
	Runtime.EngineState = Engine->GetEngineState();
	Clutch->SetConfig(Runtime.ClutchConfig);
	Engine->SetP1MotorTorque(20.f);
	Runtime.EngineState.P1MotorTorque = 20.f;
	Gearbox->SetP2MotorTorque(5.f);
	Runtime.GearboxState.P2MotorTorque = 5.f;
	Gearbox->ShiftToTargetGear(1, true);
	TestTrue(TEXT("Runtime selects the same initial gear"), Runtime.ShiftToGear(1, true));

	// Reference the existing DriveAssembly order through its component APIs.
	// This checks composition and the previous-step clutch feedback, not a second engine formula.
	for (int32 StepIndex = 0; StepIndex < 8; ++StepIndex)
	{
		const bool bHasDrivenAxles = StepIndex != 4;
		const float PreviousClutchTorque = bHasDrivenAxles ? Clutch->GetCluchTorque() : 0.f;
		Engine->UpdatePhysics(0.01f, 0.5f, PreviousClutchTorque, Gearbox->GetShouldCutSpark());
		float ComponentOutputTorque = 0.f;
		Gearbox->UpdateOutputShaft(PreviousClutchTorque, ComponentOutputTorque);
		const float RuntimeOutputTorque = Runtime.UpdateEngineAndCalculateOutputTorque(0.01f, 0.5f, bHasDrivenAxles);
		TestTrue(TEXT("Output torque preserves cached clutch feedback and P2 position"),
			FMath::IsNearlyEqual(RuntimeOutputTorque, ComponentOutputTorque, 0.001f));
		TestEqual(TEXT("Engine receives the previous clutch torque, or zero with no driven axles"),
			Runtime.EngineState.LoadTorque, PreviousClutchTorque);
		TestTrue(TEXT("Engine integration matches the legacy component order"),
			FMath::IsNearlyEqual(Runtime.EngineState.EngineAngularVelocity, Engine->GetAngularVelocity(), 0.00001f));

		float InputAngularVelocity, ReflectedInertia, ReflectedStiffness, GearRatio;
		Gearbox->UpdateInputShaft(40.f, 18.f, 900.f,
			InputAngularVelocity, ReflectedInertia, ReflectedStiffness, GearRatio);
		Clutch->UpdatePhysics(0.01f, 0.f, InputAngularVelocity, ReflectedInertia, GearRatio,
			ReflectedStiffness, Engine->GetAngularVelocity(), Engine->GetEngineInertia(), Engine->GetMaxEngineTorque());
		Runtime.UpdateClutchFromOutputShaft(0.01f, 0.f, 40.f, 18.f, 900.f);
		TestTrue(TEXT("Downstream feedback computes the next-step clutch torque"),
			FMath::IsNearlyEqual(Runtime.ClutchState.ClutchTorque, Clutch->GetCluchTorque(), 0.001f));
	}
	return true;
}
#endif
