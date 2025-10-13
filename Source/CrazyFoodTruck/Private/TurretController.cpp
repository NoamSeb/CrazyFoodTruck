// Fill out your copyright notice in the Description page of Project Settings.


#include "CrazyFoodTruck/Public/TurretController.h"


// Sets default values
ATurretController::ATurretController()
{

	PrimaryActorTick.bCanEverTick = true;
}

void ATurretController::SetBulletSpawnTransform(USceneComponent* Scp)
{
	_SpawnBulletTransform = Scp;
}

void ATurretController::ResetCoolDown()
{
	_CurrentCoolDown = _BaseCoolDownShoot;
}


// Called when the game starts or when spawned
void ATurretController::BeginPlay()
{
	Super::BeginPlay();
	ResetCoolDown();
}

// Called every frame
void ATurretController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Fire();
	if (_CurrentCoolDown > 0)
	{
		_CurrentCoolDown -= DeltaTime;
	}
}

void ATurretController::Fire()
{
	if (!BulletPrefab) return;
	if (!_SpawnBulletTransform) return;
	if (_CurrentCoolDown > 0) return;

	ResetCoolDown();
	
	FActorSpawnParameters bulletParams;

	AActor* bulletInstance = GetWorld()->SpawnActor<AActor>(BulletPrefab, _SpawnBulletTransform->GetComponentTransform(), bulletParams);
	if (bulletInstance)
	{
		// CAST TO BULLET CONTROLLER
		ABulletController* bulletController = Cast<ABulletController>(bulletInstance);
		if (bulletController)
		{
			bulletController->Initialize(2200.f, 1.5f);
		}
	}
}

