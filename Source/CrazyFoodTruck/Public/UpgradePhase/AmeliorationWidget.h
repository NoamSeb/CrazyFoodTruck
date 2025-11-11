// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmeliorationWidget.generated.h"

/**
 * 
 */
UCLASS()
class CRAZYFOODTRUCK_API UAmeliorationWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Move(int32 PlayerIndex, int32 PositionIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void MoveOnSkip(int32 PlayerIndex, int32 Move, int32 PositionIndex);
	
	UFUNCTION(BlueprintImplementableEvent)
	void Validate(int32 PlayerIndex, int32 PositionIndex, bool inSkip);

	UFUNCTION(BlueprintImplementableEvent)
	void RemoveValidate(int32 PlayerIndex, int32 PositionIndex, bool inSkip);
};
