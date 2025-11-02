// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceCrazyFoodTruck.h"

void UGameInstanceCrazyFoodTruck::Init()
{
	Super::Init();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("INIT Game Instance Crazy Food Truck"));

}

void UGameInstanceCrazyFoodTruck::SetCameraShakeManager(ACameraShakeManager* NewCameraShakeManager)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Set Camera Shake Manager in Game Instance"));
	CameraShakeManager = NewCameraShakeManager;
}

void UGameInstanceCrazyFoodTruck::PlayerCameraShake(ECameraShake ShakeType)
{
	if (CameraShakeManager)
	{
		CameraShakeManager->PlayShake(ShakeType);
	}
}
