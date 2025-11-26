// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Data/Public/GameInstanceCrazyFoodTruck.h"
#include "PlayerSlotWidget.generated.h"

class UWidgetSwitcher;
class UImage;
class UBorder;
class UTexture2D;

UCLASS()
class CRAZYFOODTRUCK_API UPlayerSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void UpdateFromSlotData(const FMenuPlayerSlot& SlotData);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby")
	int32 SlotIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby")
	TArray<TObjectPtr<UTexture2D>> PlayerAvatarTextures;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> StateSwitcher = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CharacterImage = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> ColorBorder = nullptr;

	UPROPERTY(meta = (BindWidget, OptionalWidget = true))
	TObjectPtr<UImage> AIconImage = nullptr;
};
