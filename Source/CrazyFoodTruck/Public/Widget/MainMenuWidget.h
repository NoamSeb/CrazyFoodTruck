// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UGameInstanceCrazyFoodTruck;
class UPlayerSlotWidget;

class UButton;

UCLASS()
class CRAZYFOODTRUCK_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UPlayerSlotWidget> Slot0;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UPlayerSlotWidget> Slot1;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UPlayerSlotWidget> Slot2;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> PlayButton;

private:
	UPROPERTY()
	TObjectPtr<UGameInstanceCrazyFoodTruck> GIRef = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<UPlayerSlotWidget>> SlotWidgets;

	UFUNCTION()
	void OnLobbySlotsChanged();

	void RefreshSlots();
};
