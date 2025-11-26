// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/MainMenuWidget.h"

#include "CrazyFoodTruck/Data/Public/GameInstanceCrazyFoodTruck.h"
#include "Widget/PlayerSlotWidget.h"

#include "Components/Button.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GIBase = World->GetGameInstance())
		{
			GIRef = Cast<UGameInstanceCrazyFoodTruck>(GIBase);
		}
	}

	SlotWidgets.Empty();
	if (Slot0) SlotWidgets.Add(Slot0);
	if (Slot1) SlotWidgets.Add(Slot1);
	if (Slot2) SlotWidgets.Add(Slot2);
	if (Slot3) SlotWidgets.Add(Slot3);

	if (PlayButton)
	{
		PlayButton->SetIsEnabled(false);
	}

	if (GIRef)
	{
		GIRef->OnLobbySlotsChanged.AddDynamic(this, &UMainMenuWidget::OnLobbySlotsChanged);
	}

	RefreshSlots();
}

void UMainMenuWidget::NativeDestruct()
{
	if (GIRef)
	{
		GIRef->OnLobbySlotsChanged.RemoveDynamic(this, &UMainMenuWidget::OnLobbySlotsChanged);
	}

	Super::NativeDestruct();
}

void UMainMenuWidget::OnLobbySlotsChanged()
{
	RefreshSlots();
}

void UMainMenuWidget::RefreshSlots()
{
	if (!GIRef)
	{
		return;
	}

	const TArray<FMenuPlayerSlot>& SlotsData = GIRef->PlayerSlots;

	for (int32 Index = 0; Index < SlotWidgets.Num(); ++Index)
	{
		if (UPlayerSlotWidget* SlotWidget = SlotWidgets[Index])
		{
			if (SlotsData.IsValidIndex(Index))
			{
				SlotWidget->UpdateFromSlotData(SlotsData[Index]);
			}
		}
	}

	if (PlayButton)
	{
		const bool bAllConnected = GIRef->AreAllPlayersConnected();
		PlayButton->SetIsEnabled(bAllConnected);
	}
}
