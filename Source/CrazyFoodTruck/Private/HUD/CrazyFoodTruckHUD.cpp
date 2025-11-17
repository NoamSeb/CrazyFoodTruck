// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/CrazyFoodTruckHUD.h"

#include "Widget/ZombieWidget.h"
#include "../HordeManager.h"

#include "Blueprint/UserWidget.h"

#include "Kismet/GameplayStatics.h"

void ACrazyFoodTruckHUD::BeginPlay()
{
	Super::BeginPlay();

	if (!ZombieWidgetClass) return;

	APlayerController* PC = GetOwningPlayerController();
	if (!PC) return;

	ZombieWidgetInstance = CreateWidget<UZombieWidget>(PC, ZombieWidgetClass);
	if (!ZombieWidgetInstance) return;

	if (AHordeManager* HM = ResolveHordeManager())
	{
		ZombieWidgetInstance->SetHordeManager(HM);
	}

	ZombieWidgetInstance->AddToViewport();
}

AHordeManager* ACrazyFoodTruckHUD::ResolveHordeManager() const
{
	UWorld* World = GetWorld();
	if (!World) return nullptr;

	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(World, AHordeManager::StaticClass(), Found);
	return (Found.Num() > 0) ? Cast<AHordeManager>(Found[0]) : nullptr;
}
