// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ECameraShake.h"
#include "GameFramework/Actor.h"
#include "CameraShakeManager.generated.h"

class UCameraShakeBase;
class APlayerCameraManager;

UCLASS()
class CRAZYFOODTRUCK_API ACameraShakeManager : public AActor
{
	GENERATED_BODY()

public:
	ACameraShakeManager();
	
	void PlayShake(ECameraShake ShakeType);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<UCameraShakeBase> FireBulletShake;

	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<UCameraShakeBase> ExplosionShake;

	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<UCameraShakeBase> ZombieHitShake;

	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<UCameraShakeBase> CalmAmbientShake;

	APlayerCameraManager* PlayerCameraManager = nullptr;

private:
	void CheckEachShakeReference();
	 
public:
	virtual void Tick(float DeltaTime) override;
};
