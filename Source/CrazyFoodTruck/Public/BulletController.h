// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "IEntity.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BulletController.generated.h"

UCLASS()


class CRAZYFOODTRUCK_API ABulletController : public AActor
{
	GENERATED_BODY()

public:
	ABulletController();
	void Initialize(float speed, float lifeTime);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
						AActor* OtherActor,
						UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex,
						bool bFromSweep,
						const FHitResult& SweepResult);

private :

	UBoxComponent* _BoxCollider;

	

	UProjectileMovementComponent* BulletMovementComponent;
	float _speed;
	float _lifeTime;
	float _currentLifeTime = 0.f;
	bool _alive = true;
};
