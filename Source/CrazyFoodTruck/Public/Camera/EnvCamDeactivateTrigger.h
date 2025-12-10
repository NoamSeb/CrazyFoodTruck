// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnvCamDeactivateTrigger.generated.h"

class UBoxComponent;
class ACameraEnvironmentManager;
class AVehicle;

UCLASS()
class CRAZYFOODTRUCK_API AEnvCamDeactivateTrigger : public AActor
{
	GENERATED_BODY()

public:
	AEnvCamDeactivateTrigger();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Trigger")
	TObjectPtr<UBoxComponent> TriggerBox = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "EnvCam")
	TObjectPtr<ACameraEnvironmentManager> EnvManager = nullptr;

	UPROPERTY(EditAnywhere, Category = "EnvCam", meta = (ClampMin = "0.05"))
	float BlendBackTime = 1.5f;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
