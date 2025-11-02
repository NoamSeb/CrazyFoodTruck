
#include "CameraShakeManager.h"

#include "CrazyFoodTruck/GameInstanceCrazyFoodTruck.h"
#include "Kismet/GameplayStatics.h"


ACameraShakeManager::ACameraShakeManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACameraShakeManager::CheckEachShakeReference()
{
	if (!FireBulletShake)
	{
		throw std::runtime_error("CameraShakeManager: FireBulletShake reference is missing !");
		return;
	}
}

void ACameraShakeManager::PlayShake(ECameraShake shakeType)
{
	switch (shakeType)
	{
		case FireBullet:
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(FireBulletShake);
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
}

void ACameraShakeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

