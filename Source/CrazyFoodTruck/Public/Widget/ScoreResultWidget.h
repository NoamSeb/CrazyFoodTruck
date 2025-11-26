// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Score/ScoreManagerComponent.h"
#include "ScoreResultWidget.generated.h"

class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnScoreShow, int32, Time, int32, KillCount, int32, LifeRemaining, EScoreGrade, InGrade);


UCLASS()
class CRAZYFOODTRUCK_API UScoreResultWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(Category = "Score")
	void SetScoreData(int32 Time, int32 KillCount, int32 LifeRemaining, EScoreGrade InGrade);

protected:
	
	// UPROPERTY(meta = (BindWidget))
	// UTextBlock* FinalScoreText = nullptr;
	//
	// UPROPERTY(meta = (BindWidget))
	// UTextBlock* TimeScoreText = nullptr;
	//
	// UPROPERTY(meta = (BindWidget))
	// UTextBlock* KillScoreText = nullptr;
	//
	// UPROPERTY(meta = (BindWidget))
	// UTextBlock* GradeText = nullptr;

	UPROPERTY(BlueprintAssignable, Category = "Score|Events")
	FOnScoreShow OnScoreShow;

private:
	FString GradeToString(EScoreGrade Grade) const;
};
