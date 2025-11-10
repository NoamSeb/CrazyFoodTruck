
#include "CameraShakeManager.h"

#include "CrazyFoodTruck/Data/Public/GameInstanceCrazyFoodTruck.h"
#include "Kismet/GameplayStatics.h"


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
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CameraShakeManager: Explosion is not assigned!"));
	}
	if(!ZombieHitShake)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CameraShakeManager: ZombieHitShake is not assigned!"));
	}
}


void ACameraShakeManager::PlayShake(ECameraShake shakeType)
{
	switch (shakeType)
	{
		case FireBullet:
		PlayerCameraManager->StartCameraShake(FireBulletShake);
		break;
	case Explosion:
		PlayerCameraManager->StartCameraShake(ExplosionShake);
		break;
	case ZombieHit:
		PlayerCameraManager->StartCameraShake(ZombieHitShake);
		break;
	default:
		break;
	}
}

void ACameraShakeManager::BeginPlay()
{
	Super::BeginPlay();
	
	CheckEachShakeReference();
	
	auto GetInstance = Cast<UGameInstanceCrazyFoodTruck>(GetGameInstance());
	if (GetInstance)
	{
		GetInstance->SetCameraShakeManager(this);
	}
	
	PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
}

void ACameraShakeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

