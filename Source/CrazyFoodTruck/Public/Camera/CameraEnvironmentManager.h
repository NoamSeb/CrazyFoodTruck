// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraEnvironmentManager.generated.h"

class USpringArmComponent;
class UGameInstanceCrazyFoodTruck;
class AHordeManager;

USTRUCT(BlueprintType)
struct FEnvCameraKey
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RelativeLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "100.0"))
	float TargetArmLength = 800.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float HoldTime = 2.f;
};

UCLASS()
class CRAZYFOODTRUCK_API ACameraEnvironmentManager : public AActor
{
	GENERATED_BODY()

public:
	ACameraEnvironmentManager();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, Category = "EnvCam|Refs")
	TObjectPtr<USpringArmComponent> TargetSpringArm = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "EnvCam|Refs")
	TObjectPtr<AActor> TruckActor = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<AHordeManager> HordeManager = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UGameInstanceCrazyFoodTruck> CFTGameInstance = nullptr;

	UPROPERTY(EditAnywhere, Category = "EnvCam|Path")
	TArray<FEnvCameraKey> EnvCameraKeys;

	UPROPERTY(EditAnywhere, Category = "EnvCam|Path", meta = (ClampMin = "0.1"))
	float BlendTime = 1.5f;

	UPROPERTY(EditAnywhere, Category = "EnvCam|Defaults")
	float DefaultArmLength = 600.f;

	UPROPERTY(EditAnywhere, Category = "EnvCam|Defaults")
	FVector DefaultRelativeLocation = FVector(0.f, 0.f, 300.f);

	UPROPERTY(EditAnywhere, Category = "EnvCam|Defaults")
	FRotator DefaultRelativeRotation = FRotator(-60.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, Category = "EnvCam|Settings")
	bool bEnableEnvironmentMode = true;

	bool bInEnvironmentMode = false;
	bool bHasTriggeredCalmShake = false;

	int32 CurrentKeyIndex = 0;

	float BlendTimer = 0.f;
	float HoldTimer = 0.f;

	float StartArmLength = 0.f;
	FVector StartRelLocation = FVector::ZeroVector;
	FRotator StartRelRotation = FRotator::ZeroRotator;

private:
	void UpdateState(float DeltaTime);

	bool HasAnyZombie() const;

	void FindHordeManager();

	void EnterEnvironmentMode();
	void ExitEnvironmentMode();

	void StartBlendToCurrentKey();
	void ApplyBlend(float Alpha);

	void RestoreDefaultPose();
};
