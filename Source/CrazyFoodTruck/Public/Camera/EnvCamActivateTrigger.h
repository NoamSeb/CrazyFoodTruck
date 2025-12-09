// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraEnvironmentManager.h"
#include "EnvCamActivateTrigger.generated.h"

class UBoxComponent;
class AVehicle;

UCLASS()
class CRAZYFOODTRUCK_API AEnvCamActivateTrigger : public AActor
{
	GENERATED_BODY()

public:
	AEnvCamActivateTrigger();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Trigger")
	TObjectPtr<UBoxComponent> TriggerBox = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "EnvCam")
	TObjectPtr<ACameraEnvironmentManager> EnvManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnvCam")
	FEnvCameraPreset CameraPreset;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
