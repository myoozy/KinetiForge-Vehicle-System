// Copyright (c) 2025 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Curves/CurveFloat.h"
#include "VehicleEngineComponent.generated.h"

class UVehicleClutchComponent;

UENUM(BlueprintType)
enum class EEngineState : uint8
{
	On  UMETA(DisplayName = "EngineON"),
	Off UMETA(DisplayName = "EngineOFF"),
	Starting    UMETA(DisplayName = "EngineSTARTING"),
	Shutting    UMETA(DisplayName = "EngineSHUTTING")
};

USTRUCT(BlueprintType, Blueprintable)
struct FVehicleNaturallyAspiratedEngineConfig
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0"))
	float MaxEngineTorque = 400;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0"))
	UCurveFloat* EngineTorqueCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0"))
	float EngineIdleRPM = 900;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0"))
	float EngineMaxRPM = 6000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0"))
	float EngineInertia = 0.2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0"))
	float StartFriction = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0"))
	float FrictionCoefficient = 0.005;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0"))
	float IdleThrottleInterpSpeed = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EngineSetup", meta = (ClampMin = "0.0"))
	float RevLimiterTime = 0.05;

};

USTRUCT(BlueprintType, Blueprintable)
struct FVehicleEngineTurboConfig
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup", meta = (ClampMin = "0.0", ToolTip = "if boost torque > 0, will be considered as turbo charged"))
	float TurboBoostTorque = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup", meta = (ClampMin = "0.0"))
	float TurboStartRPM = 1200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup", meta = (ClampMin = "0.0"))
	float TurboFinishRPM = 2000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup", meta = (ClampMin = "0.0"))
	float TurboMaxPressure = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup", meta = (ClampMin = "-1.0", ClampMax = "0.0"))
	float TurboNegPressure = -0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup", meta = (ClampMin = "0.0"))
	float TurboLag = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup", meta = (ClampMin = "0.0"))
	float TurboWasteGateLag = 0.1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurboSetup")
	bool bAntiLag = false;
};

USTRUCT(BlueprintType, Blueprintable)
struct FVehicleEngineSimData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EEngineState State = EEngineState::Starting;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DeltaTime")
	float PhysicsDeltaTime = 0.00833333;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float EngineAngularVelocity = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	float RawThrottleInput = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	float IdleThrottle = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	float RealThrottle = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float EffectiveTorque = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float LoadTorque = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force")
	float TorqueRequiredToStartEngine = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force")
	float StarterMotorTorque = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float EngineRPM = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float EngineOffRPM = 0.f;	//under this rpm, the engine will be considered as off
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turbo")
	float TurboPressure = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float RevLimiterTimer = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force")
	float P1MotorTorque = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bSpark = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIdleRPMInValid = false;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class KINETIFORGE_API UVehicleEngineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVehicleEngineComponent();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	FVehicleNaturallyAspiratedEngineConfig NAConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	FVehicleEngineTurboConfig TurboConfig;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FVehicleEngineSimData SimData;

	void EngineAcceleration();
	float SafeDivide(float a, float b);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Physics")
	void UpdatePhysics(float InDeltaTime, float InThrottle, float InLoadTorque);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	void GetEngineMovement(FVehicleEngineSimData& Out) { Out = SimData; }
	UFUNCTION(BlueprintCallable, Category = "Physics")
	void SetP1MotorTorque(float NewTorque) {SimData.P1MotorTorque = NewTorque; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Physics")
	float GetP1MotorTorque() { return SimData.P1MotorTorque; }
	UFUNCTION(BlueprintCallable, Category = "Initialize")
	void Initialize();
	UFUNCTION(BlueprintCallable, Category = "Input")
	EEngineState StartVehicleEngine();
	UFUNCTION(BlueprintCallable, Category = "Input")
	EEngineState ShutVehicleEngine();
	float GetRPM() { return SimData.EngineRPM; }
	float GetAngularVelocity() { return SimData.EngineAngularVelocity; }

private:
	float RPMToRad = PI / 30;
	float RadToRPM = 30 / PI;


	//cache
	float CachedStartFriction;
	float CachedEngineIdleRPM;
	float CachedFrictionCoefficient;
	float CachedMaxEngineTorque;
	float CachedTurboLag;
	float CachedTurboWasteGateLag;
	float CachedTurboNegPressure;
	float CachedTurboMaxPressure;
};
