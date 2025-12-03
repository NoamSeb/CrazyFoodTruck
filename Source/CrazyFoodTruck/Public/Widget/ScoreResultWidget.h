// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Score/ScoreManagerComponent.h"
#include "ScoreResultWidget.generated.h"

class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnScoreShow, int32, Time, int32, KillCount, int32, LifeRemaining, int32, TicketCounts, EScoreGrade, InGrade);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScoreEnded);

UCLASS()
class CRAZYFOODTRUCK_API UScoreResultWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(Category = "Score")
	void SetScoreData(int32 Time, int32 KillCount, int32 LifeRemaining, int TicketsCount, EScoreGrade InGrade);

	UFUNCTION(BlueprintCallable, Category = "Score")
	void EndScoreDisplay();

	UPROPERTY(BlueprintAssignable, Category = "Score|Events")
	FOnScoreEnded OnScoreEnded;

protected:

	UPROPERTY(BlueprintAssignable, Category = "Score|Events")
	FOnScoreShow OnScoreShow;

private:
	
	FString GradeToString(EScoreGrade Grade) const;
};
