// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraShakeManager.h"

#include "CrazyFoodTruck/Data/Public/GameInstanceCrazyFoodTruck.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"

ACameraShakeManager::ACameraShakeManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACameraShakeManager::CheckEachShakeReference()
{
	if (!FireBulletShake)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CameraShakeManager: FireBulletShake is not assigned!"));
	}
	if (!ExplosionShake)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CameraShakeManager: ExplosionShake is not assigned!"));
	}
	if (!ZombieHitShake)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CameraShakeManager: ZombieHitShake is not assigned!"));
	}
	if (!CalmAmbientShake)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CameraShakeManager: CalmAmbientShake is not assigned (optional)."));
	}
}

void ACameraShakeManager::PlayShake(ECameraShake ShakeType)
{
	if (!PlayerCameraManager) return;

	switch (ShakeType)
	{
	case FireBullet:
		if (FireBulletShake)
		{
			PlayerCameraManager->StartCameraShake(FireBulletShake);
		}
		break;

	case Explosion:
		if (ExplosionShake)
		{
			PlayerCameraManager->StartCameraShake(ExplosionShake);
		}
		break;

	case ZombieHit:
		if (ZombieHitShake)
		{
			PlayerCameraManager->StartCameraShake(ZombieHitShake);
		}
		break;

	case CalmAmbient:
		if (CalmAmbientShake)
		{
			PlayerCameraManager->StartCameraShake(CalmAmbientShake);
		}
		break;

	default:
		break;
	}
}

void ACameraShakeManager::BeginPlay()
{
	Super::BeginPlay();

	CheckEachShakeReference();

	if (UGameInstanceCrazyFoodTruck* GI = Cast<UGameInstanceCrazyFoodTruck>(GetGameInstance()))
	{
		GI->SetCameraShakeManager(this);
	}

	PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
}

void ACameraShakeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
