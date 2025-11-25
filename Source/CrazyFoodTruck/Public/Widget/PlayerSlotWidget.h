// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Data/Public/GameInstanceCrazyFoodTruck.h"
#include "PlayerSlotWidget.generated.h"

class UWidgetSwitcher;
class UImage;
class UTexture2D;

UCLASS()
class CRAZYFOODTRUCK_API UPlayerSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lobby", meta = (ExposeOnSpawn = "true"))
	int32 SlotIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lobby")
	TArray<TObjectPtr<UTexture2D>> PlayerAvatarTextures;

	void UpdateFromSlotData(const FMenuPlayerSlot& SlotData);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UWidgetSwitcher> StateSwitcher;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> AIconImage;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> CharacterImage;
};
