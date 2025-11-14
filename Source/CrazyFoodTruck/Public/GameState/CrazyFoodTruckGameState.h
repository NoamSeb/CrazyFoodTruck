// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Score/ScoreManagerComponent.h"
#include "CrazyFoodTruckGameState.generated.h"

class UMatchTimerComponent;

UCLASS()
class CRAZYFOODTRUCK_API ACrazyFoodTruckGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ACrazyFoodTruckGameState();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timer")
	UMatchTimerComponent* MatchTimer;

	UFUNCTION(BlueprintPure, Category = "Timer")
	UMatchTimerComponent* GetMatchTimer() const { return MatchTimer; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 FinalScore = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 TimeScore = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 KillScore = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	EScoreGrade FinalGrade = EScoreGrade::F;

	UFUNCTION(BlueprintCallable, Category = "Score")
	void SetScoreValues(int32 InFinal, int32 InTimeScore, int32 InKillScore, EScoreGrade InGrade);
};
