// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/PlayerSlotWidget.h"

#include "CrazyFoodTruck/Data/Public/GameInstanceCrazyFoodTruck.h"

#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"
#include "Components/Border.h"

void UPlayerSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StateSwitcher)
	{
		StateSwitcher->SetActiveWidgetIndex(0);
	}

    if (CharacterImage)
    {
        CharacterImage->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (ColorBorder)
    {
        ColorBorder->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.3f));
    }
}

void UPlayerSlotWidget::UpdateFromSlotData(const FMenuPlayerSlot& SlotData)
{
	if (!SlotData.bIsConnected)
	{
		if (StateSwitcher)
		{
			StateSwitcher->SetActiveWidgetIndex(0);
		}

		if (CharacterImage)
		{
			CharacterImage->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (ColorBorder)
		{
			ColorBorder->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.3f));
		}

		return;
	}

	if (StateSwitcher)
	{
		StateSwitcher->SetActiveWidgetIndex(1);
	}

	FLinearColor PlayerColor = FLinearColor::White;

	if (UWorld* World = GetWorld())
	{
		if (UGameInstanceCrazyFoodTruck* GI = World->GetGameInstance<UGameInstanceCrazyFoodTruck>())
		{
			PlayerColor = GI->GetPlayerColorForIndex(SlotIndex);
		}
	}

	if (ColorBorder)
	{
		FLinearColor BGColor = PlayerColor;
		BGColor.A = 0.4f;
		ColorBorder->SetBrushColor(BGColor);
	}

	if (CharacterImage)
	{
		UTexture2D* AvatarTex = nullptr;

		if (PlayerAvatarTextures.IsValidIndex(SlotIndex))
		{
			AvatarTex = PlayerAvatarTextures[SlotIndex];
		}

		if (AvatarTex)
		{
			FSlateBrush Brush;
			Brush.SetResourceObject(AvatarTex);
			CharacterImage->SetBrush(Brush);
			CharacterImage->SetColorAndOpacity(FLinearColor::White);
			CharacterImage->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			CharacterImage->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
