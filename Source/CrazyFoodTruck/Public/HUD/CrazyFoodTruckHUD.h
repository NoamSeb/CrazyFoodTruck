// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Score/ScoreManagerComponent.h"
#include "CrazyFoodTruckHUD.generated.h"

class UZombieWidget;
class UScoreResultWidget;
class AHordeManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinalScoreShownHUD);

UCLASS()
class CRAZYFOODTRUCK_API ACrazyFoodTruckHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void ShowScoreResult(int32 Time, int32 KillCount, int32 LifeRemaining, int TicketsCount, EScoreGrade InGrade);

	UFUNCTION()
	void ListenScoreEnd();

	FOnFinalScoreShownHUD OnFinalScoreShownHUD;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UZombieWidget> ZombieWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UScoreResultWidget> ScoreResultWidgetClass;

private:
	UPROPERTY()
	UZombieWidget* ZombieWidgetInstance = nullptr;

	UPROPERTY()
	UScoreResultWidget* ScoreResultWidgetInstance = nullptr;

	AHordeManager* ResolveHordeManager() const;
};
