// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/CrazyFoodTruckHUD.h"

#include "Widget/ZombieWidget.h"
#include "Widget/ScoreResultWidget.h"
#include "../HordeManager.h"

#include "Blueprint/UserWidget.h"

#include "GameFramework/PlayerController.h"

#include "Kismet/GameplayStatics.h"

void ACrazyFoodTruckHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = GetOwningPlayerController();
	if (!PC) return;

	if (ZombieWidgetClass)
	{
		ZombieWidgetInstance = CreateWidget<UZombieWidget>(PC, ZombieWidgetClass);
		if (ZombieWidgetInstance)
		{
			if (AHordeManager* HM = ResolveHordeManager())
			{
				ZombieWidgetInstance->SetHordeManager(HM);
			}
			
			ZombieWidgetInstance->AddToViewport();
		}
	}

	if (ScoreResultWidgetClass)
	{
		ScoreResultWidgetInstance = CreateWidget<UScoreResultWidget>(PC, ScoreResultWidgetClass);
	}
}

void ACrazyFoodTruckHUD::ShowScoreResult(int32 Time, int32 KillCount, int32 LifeRemaining, int TicketsCount, EScoreGrade InGrade)
{
	APlayerController* PC = GetOwningPlayerController();
	if (!PC) return;

	if (!ScoreResultWidgetInstance && ScoreResultWidgetClass)
	{
		ScoreResultWidgetInstance = CreateWidget<UScoreResultWidget>(PC, ScoreResultWidgetClass);
	}

	if (!ScoreResultWidgetInstance)
	{
		return;
	}

	if (!ScoreResultWidgetInstance->IsInViewport())
	{
		ScoreResultWidgetInstance->AddToViewport(1);
	}
	
	ScoreResultWidgetInstance->SetScoreData(Time, KillCount, LifeRemaining,TicketsCount, InGrade);

	ScoreResultWidgetInstance->OnScoreEnded.AddDynamic(this, &ACrazyFoodTruckHUD::ListenScoreEnd);
}

void ACrazyFoodTruckHUD::ListenScoreEnd()
{
	OnFinalScoreShownHUD.Broadcast();
}

AHordeManager* ACrazyFoodTruckHUD::ResolveHordeManager() const
{
	UWorld* World = GetWorld();
	if (!World) return nullptr;

	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(World, AHordeManager::StaticClass(), Found);
	return (Found.Num() > 0) ? Cast<AHordeManager>(Found[0]) : nullptr;
}
