// Fill out your copyright notice in the Description page of Project Settings.

#include "Score/ScoreManagerComponent.h"

UScoreManagerComponent::UScoreManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	{
		FScoreTier T30; T30.Threshold = 30; T30.Score = 400;
		FScoreTier T60; T60.Threshold = 60; T60.Score = 250;
		FScoreTier T90; T90.Threshold = 90; T90.Score = 100;
		TimeScoreTiers = { T30, T60, T90 };
	}

	{
		FScoreTier K50;  K50.Threshold = 50;  K50.Score = 150;
		FScoreTier K100; K100.Threshold = 100; K100.Score = 350;
		FScoreTier K200; K200.Threshold = 200; K200.Score = 700;
		KillScoreTiers = { K50, K100, K200 };
	}

	{
		FScoreGradeTier G_F; G_F.Threshold = 0;    G_F.Grade = EScoreGrade::F;
		FScoreGradeTier G_E; G_E.Threshold = 500;  G_E.Grade = EScoreGrade::E;
		FScoreGradeTier G_D; G_D.Threshold = 1000; G_D.Grade = EScoreGrade::D;
		FScoreGradeTier G_C; G_C.Threshold = 2000; G_C.Grade = EScoreGrade::C;
		FScoreGradeTier G_B; G_B.Threshold = 3000; G_B.Grade = EScoreGrade::B;
		FScoreGradeTier G_A; G_A.Threshold = 4000; G_A.Grade = EScoreGrade::A;
		FScoreGradeTier G_S; G_S.Threshold = 5000; G_S.Grade = EScoreGrade::S;
		GradeTiers = { G_F, G_E, G_D, G_C, G_B, G_A, G_S };
	}
}

int32 UScoreManagerComponent::EvaluateScore_Direct(const TArray<FScoreTier>& Tiers, int32 Value) const
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

int32 UScoreManagerComponent::EvaluateScore_Inverse(const TArray<FScoreTier>& Tiers, int32 Value) const
{
	for (const FScoreTier& Tier : Tiers)
	{
		if (Value <= Tier.Threshold)
		{
			return Tier.Score;
		}
	}

	return 0;
}

EScoreGrade UScoreManagerComponent::EvaluateGrade(const TArray<FScoreGradeTier>& Tiers, int32 TotalScore) const
{
	EScoreGrade BestGrade = EScoreGrade::F;

	for (const FScoreGradeTier& Tier : Tiers)
	{
		if (TotalScore >= Tier.Threshold)
		{
			BestGrade = Tier.Grade;
		}
		else
		{
			break;
		}
	}

	return BestGrade;
}

int32 UScoreManagerComponent::ComputeTotalScore(int32 TimeSeconds, int32 ZombiesKilled, int32& OutTimeScore, int32& OutKillScore) const
{
	OutTimeScore = EvaluateScore_Inverse(TimeScoreTiers, TimeSeconds);
	OutKillScore = EvaluateScore_Direct(KillScoreTiers, ZombiesKilled);
	return OutTimeScore + OutKillScore;
}

EScoreGrade UScoreManagerComponent::GetGradeForScore(int32 TotalScore) const
{
	return EvaluateGrade(GradeTiers, TotalScore);
}
