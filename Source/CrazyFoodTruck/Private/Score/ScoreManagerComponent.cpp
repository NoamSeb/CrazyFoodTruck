// Fill out your copyright notice in the Description page of Project Settings.

#include "Score/ScoreManagerComponent.h"

UScoreManagerComponent::UScoreManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	{
		FScoreTier T30; T30.Threshold = 30; T30.Score = 100;
		FScoreTier T60; T60.Threshold = 60; T60.Score = 250;
		FScoreTier T90; T90.Threshold = 90; T90.Score = 400;
		TimeScoreTiers = { T30, T60, T90 };
	}

	{
		FScoreTier K50;  K50.Threshold = 50;  K50.Score = 150;
		FScoreTier K100; K100.Threshold = 100; K100.Score = 350;
		FScoreTier K200; K200.Threshold = 200; K200.Score = 700;
		KillScoreTiers = { K50, K100, K200 };
	}
}

int32 UScoreManagerComponent::EvaluateFromTiers(const TArray<FScoreTier>& Tiers, int32 Value) const
{
	int32 BestScore = 0;

	for (const FScoreTier& Tier : Tiers)
	{
		if (Value >= Tier.Threshold)
		{
			BestScore = Tier.Score;
		}
		else
		{
			break;
		}
	}

	return BestScore;
}

int32 UScoreManagerComponent::ComputeTotalScore(int32 TimeSeconds, int32 ZombiesKilled, int32& OutTimeScore, int32& OutKillScore) const
{
	OutTimeScore = EvaluateFromTiers(TimeScoreTiers, TimeSeconds);
	OutKillScore = EvaluateFromTiers(KillScoreTiers, ZombiesKilled);
	return OutTimeScore + OutKillScore;
}
