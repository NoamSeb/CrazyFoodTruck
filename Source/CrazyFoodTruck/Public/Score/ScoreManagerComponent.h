// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScoreManagerComponent.generated.h"

UENUM(BlueprintType)
enum class EScoreGrade : uint8
{
	S UMETA(DisplayName = "S"),
	A UMETA(DisplayName = "A"),
	B UMETA(DisplayName = "B"),
	C UMETA(DisplayName = "C"),
	D UMETA(DisplayName = "D"),
	E UMETA(DisplayName = "E"),
	F UMETA(DisplayName = "F")
};

USTRUCT(BlueprintType)
struct FScoreTier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Threshold = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score = 0;
};

USTRUCT(BlueprintType)
struct FScoreGradeTier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Threshold = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
	EScoreGrade Grade = EScoreGrade::F;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score|Grade")
	TArray<FScoreGradeTier> GradeTiers;

	UFUNCTION(BlueprintCallable, Category = "Score")
	int32 ComputeTotalScore(int32 TimeSeconds, int32 ZombiesKilled, int32& OutTimeScore, int32& OutKillScore) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Score|Grade")
	EScoreGrade GetGradeForScore(int32 TotalScore) const;

private:
	int32 EvaluateScore_Direct(const TArray<FScoreTier>& Tiers, int32 Value) const;
	int32 EvaluateScore_Inverse(const TArray<FScoreTier>& Tiers, int32 Value) const;
	EScoreGrade EvaluateGrade(const TArray<FScoreGradeTier>& Tiers, int32 TotalScore) const;
};
