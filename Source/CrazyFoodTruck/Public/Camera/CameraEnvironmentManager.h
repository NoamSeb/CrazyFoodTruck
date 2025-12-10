// Fill out your copyright notice in the Description page of Project Settings.

// CameraEnvironmentManager.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraEnvironmentManager.generated.h"

class USpringArmComponent;
class UGameInstanceCrazyFoodTruck;

USTRUCT(BlueprintType)
struct FEnvCameraPreset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RelativeLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "100.0"))
	float TargetArmLength = 800.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.05"))
	float BlendTime = 1.5f;
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
	TObjectPtr<AActor> TruckActor = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "EnvCam|Refs")
	TObjectPtr<USpringArmComponent> TargetSpringArm = nullptr;

	UPROPERTY()
	TObjectPtr<UGameInstanceCrazyFoodTruck> CFTGameInstance = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "EnvCam|Defaults")
	float DefaultArmLength = 800.f;

	UPROPERTY(VisibleAnywhere, Category = "EnvCam|Defaults")
	FVector DefaultRelativeLocation = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, Category = "EnvCam|Defaults")
	FRotator DefaultRelativeRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, Category = "EnvCam|Settings")
	bool bEnableEnvironmentMode = true;

	UPROPERTY(EditAnywhere, Category = "EnvCam|Settings", meta = (ClampMin = "0.05"))
	float DefaultBlendBackTime = 1.5f;

	UPROPERTY(EditAnywhere, Category = "EnvCam|Settings")
	bool bPlayShakeOnActivate = true;

	UPROPERTY(VisibleAnywhere, Category = "EnvCam|Runtime")
	bool bIsBlending = false;

	UPROPERTY(VisibleAnywhere, Category = "EnvCam|Runtime")
	bool bBlendToDynamic = false;

	UPROPERTY(VisibleAnywhere, Category = "EnvCam|Runtime")
	float BlendTimer = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "EnvCam|Runtime")
	float CurrentBlendDuration = 1.0f;

	float StartArmLength = 0.f;
	FVector StartRelLocation = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, Category = "EnvCam|Runtime")
	FEnvCameraPreset ActivePreset;

public:
	UFUNCTION(BlueprintCallable, Category = "EnvCam")
	void ActivateDynamicCamera(const FEnvCameraPreset& Preset);

	UFUNCTION(BlueprintCallable, Category = "EnvCam")
	void DeactivateDynamicCamera(float BlendTimeOverride = -1.f);

private:
	void StartBlend(bool bTowardsDynamic, float BlendDuration);
	void UpdateBlend(float DeltaTime);
	void ApplyBlend(float Alpha, bool bTowardsDynamic);
};
