// Fill out your copyright notice in the Description page of Project Settings.

#include "GameState/CrazyFoodTruckGameState.h"

#include "Timer/MatchTimerComponent.h"

ACrazyFoodTruckGameState::ACrazyFoodTruckGameState()
{
	MatchTimer = CreateDefaultSubobject<UMatchTimerComponent>(TEXT("MatchTimer"));
}

void ACrazyFoodTruckGameState::SetScoreValues(int32 InFinal, int32 InTimeScore, int32 InKillScore, EScoreGrade InGrade)
{
	FinalScore = InFinal;
	TimeScore = InTimeScore;
	KillScore = InKillScore;
	FinalGrade = InGrade;
}
