// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Score/ScoreManagerComponent.h"
#include "ScoreResultWidget.generated.h"

class UTextBlock;

UCLASS()
class CRAZYFOODTRUCK_API UScoreResultWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Score")
	void SetScoreData(int32 InFinalScore, int32 InTimeScore, int32 InKillScore, EScoreGrade InGrade);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* FinalScoreText = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeScoreText = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* KillScoreText = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GradeText = nullptr;

private:
	FString GradeToString(EScoreGrade Grade) const;
};
