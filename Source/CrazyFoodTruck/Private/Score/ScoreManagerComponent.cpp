// Fill out your copyright notice in the Description page of Project Settings.

#include "Score/ScoreManagerComponent.h"

#include "CrazyFoodTruck/Data/Public/GameInstanceCrazyFoodTruck.h"

UScoreManagerComponent::UScoreManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UScoreManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	if (UGameInstance* GIBase = GetOwner()->GetGameInstance())
	{
		auto GI = Cast<UGameInstanceCrazyFoodTruck>(GIBase);
		if (GI)
		{
			GameData = GI->GetSubsystem<UGameDataSubSystem>();
		}
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
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Evaluating Time Score Tier: Value %d vs Threshold %d"), Value, Tier.Threshold));
		if (Value <= Tier.Threshold)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Time Score Tier Matched: Value %d <= Threshold %d => Score %d"), Value, Tier.Threshold, Tier.Score));
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
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("Grade Tier Matched: TotalScore %d >= Threshold %d => Grade %s"), TotalScore, Tier.Threshold, *UEnum::GetValueAsString(Tier.Grade)));
			return Tier.Grade;
		}
		else
		{
			break;
		}
	}

	return BestGrade;
}

int32 UScoreManagerComponent::ComputeTotalScore(int32 TimeSeconds, int32 ZombiesKilled, int32 OutLifeScore, int32& OutTimeScore, int32& OutKillScore) const
{
	if (GameData)
	{
		FStructScoreLevel CurrentScoreLevel = GetScoreLevelData(GameData->LevelNumber);
		OutTimeScore = EvaluateScore_Inverse(CurrentScoreLevel.TimeScoreTiers, TimeSeconds);
		OutKillScore = EvaluateScore_Direct(CurrentScoreLevel.KillScoreTiers, ZombiesKilled);
	}
	return OutTimeScore + OutKillScore + OutLifeScore;
}

EScoreGrade UScoreManagerComponent::GetGradeForScore(int32 TotalScore) const
{
	FStructScoreLevel CurrentScoreLevel = GetScoreLevelData(GameData->LevelNumber);
	return EvaluateGrade(CurrentScoreLevel.GradeTiers, TotalScore);
}

FStructScoreLevel UScoreManagerComponent::GetScoreLevelData(int32 LevelIndex) const
{
	auto data =  DataScoreLevels[LevelIndex];
	if (!data)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ScoreManagerComponent : DataScoreLevels not assigned or invalid !"));
		return FStructScoreLevel();
	}
	auto X = data->FindRow<FStructLevelScoreContainer>(FName("ScoreLevel"), TEXT("") );
	if (!X)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ScoreManagerComponent : Could not find ScoreLevel row in DataTable !"));
		return FStructScoreLevel();
	}
	return 	X->LevelScore;

}
