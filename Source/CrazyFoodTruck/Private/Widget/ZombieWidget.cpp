// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/ZombieWidget.h"

#include "../ZombieIA.h"
#include "../HordeManager.h"

#include "Components/TextBlock.h"

void UZombieWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (HordeManager.IsValid())
	{
		BindToManager();
		RefreshKillsText();
	}
}

void UZombieWidget::NativeDestruct()
{
	UnbindFromManager();
	Super::NativeDestruct();
}

void UZombieWidget::SetHordeManager(AHordeManager* InManager)
{
	if (HordeManager.Get() == InManager)
	{
		return;
	}

	UnbindFromManager();
	HordeManager = InManager;

	if (IsConstructed())
	{
		BindToManager();
		RefreshKillsText();
	}
}

void UZombieWidget::BindToManager()
{
	if (HordeManager.IsValid())
	{
		HordeManager->OnAnyZombieDied.AddDynamic(this, &UZombieWidget::HandleAnyZombieDied);
	}
}

void UZombieWidget::UnbindFromManager()
{
	if (HordeManager.IsValid())
	{
		HordeManager->OnAnyZombieDied.RemoveDynamic(this, &UZombieWidget::HandleAnyZombieDied);
	}
}

void UZombieWidget::HandleAnyZombieDied(AZombieIA* Zombie, AActor* Killer)
{
	RefreshKillsText();
}

void UZombieWidget::RefreshKillsText()
{
	if (!KillsText) return;

	int32 Count = 0;
	if (HordeManager.IsValid())
	{
		Count = HordeManager->GetZombiesKilledCount();
	}

	KillsText->SetText(FText::FromString(FString::Printf(TEXT("Zombies: %d"), Count)));
}
