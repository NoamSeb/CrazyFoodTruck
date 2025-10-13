// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BulletController.h"
#include "GameFramework/Actor.h"
#include "TurretController.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API ATurretController : public AActor
{
	GENERATED_BODY()

public:
	ATurretController();
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABulletController> BulletPrefab;
	
	
	UPROPERTY(EditAnywhere)
	float FireRate = 1.0f;
	

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere)
	float _BaseCoolDownShoot = 0.f;
	float _CurrentCoolDown = 0.f;
	
	void Fire();
	UFUNCTION(BlueprintCallable)
	void SetBulletSpawnTransform(USceneComponent* Scp);

	USceneComponent* _SpawnBulletTransform;
	USceneComponent* _CanonToRotate;

	void ResetCoolDown();
	
};
