// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BulletController.generated.h"

UCLASS()


class CRAZYFOODTRUCK_API ABulletController : public AActor
{
	GENERATED_BODY()

public:
	ABulletController();
	void Initialize(float speed, float lifeTime);
	void AddVelocity(FVector dir, float speed);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	private :

	UProjectileMovementComponent* BulletMovementComponent;
	float _speed;
	float _lifeTime;
	float _currentLifeTime = 0.f;
	bool _alive = true;
};
