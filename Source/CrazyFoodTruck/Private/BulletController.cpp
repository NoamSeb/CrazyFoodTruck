// Fill out your copyright notice in the Description page of Project Settings.


#include "CrazyFoodTruck/Public/BulletController.h"


// Sets default values
ABulletController::ABulletController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABulletController::Initialize(float speed, float lifeTime)
{
	BulletMovementComponent = FindComponentByClass<UProjectileMovementComponent>();

	_speed = speed;
	_lifeTime = lifeTime;
	_currentLifeTime = lifeTime;
	if (BulletMovementComponent)
	{
		BulletMovementComponent->InitialSpeed = speed;
		//BulletMovementComponent->Velocity = GetActorForwardVector() * speed;
	}
}

void ABulletController::AddVelocity(FVector dir, float speed)
{
}

void ABulletController::BeginPlay()
{
	Super::BeginPlay();
}

void ABulletController::Tick(float DeltaTime)
{
	if (!_alive){return;}

	Super::Tick(DeltaTime);
	
	if (_currentLifeTime >0)
	{
		_currentLifeTime -= DeltaTime;
	}
	else
	{
		_alive = false;
		Destroy();
	}
}

