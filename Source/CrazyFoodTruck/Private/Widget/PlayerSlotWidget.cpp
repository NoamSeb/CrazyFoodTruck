// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/PlayerSlotWidget.h"

#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"

void UPlayerSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StateSwitcher)
	{
		StateSwitcher->SetActiveWidgetIndex(0);
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
        
        return;
    }

    if (StateSwitcher)
    {
        StateSwitcher->SetActiveWidgetIndex(1);
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
            CharacterImage->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            CharacterImage->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}
