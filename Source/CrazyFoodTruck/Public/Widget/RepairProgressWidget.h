// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RepairProgressWidget.generated.h"

class UProgressBar;

UCLASS()
class CRAZYFOODTRUCK_API URepairProgressWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Repair")
	void SetRepairProgress(float InProgress);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> RepairProgressBar = nullptr;
};
