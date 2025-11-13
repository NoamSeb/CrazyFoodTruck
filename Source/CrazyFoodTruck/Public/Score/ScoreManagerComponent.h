// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScoreManagerComponent.generated.h"

USTRUCT(BlueprintType)
struct FScoreTier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Threshold = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score = 0;
};

UCLASS(ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class CRAZYFOODTRUCK_API UScoreManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UScoreManagerComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	TArray<FScoreTier> TimeScoreTiers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	TArray<FScoreTier> KillScoreTiers;

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 ComputeTotalScore(int32 TimeSeconds, int32 ZombiesKilled, int32& OutTimeScore, int32& OutKillScore) const;

private:
	int32 EvaluateFromTiers(const TArray<FScoreTier>& Tiers, int32 Value) const;
};
