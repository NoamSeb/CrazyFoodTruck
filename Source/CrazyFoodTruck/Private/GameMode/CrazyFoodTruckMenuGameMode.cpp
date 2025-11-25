// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/CrazyFoodTruckMenuGameMode.h"

#include "CrazyFoodTruck/Data/Public/GameInstanceCrazyFoodTruck.h"
#include "PlayerController/MenuPlayerController.h"
#include "Widget/MainMenuWidget.h"
#include "LocalMultiplayerSubsystem.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

ACrazyFoodTruckMenuGameMode::ACrazyFoodTruckMenuGameMode()
{
	PlayerControllerClass = AMenuPlayerController::StaticClass();
}

void ACrazyFoodTruckMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (UGameInstanceCrazyFoodTruck* GI = GetGameInstance<UGameInstanceCrazyFoodTruck>())
	{
		GI->ResetLobby();
	}

	APlayerController* PC = World->GetFirstPlayerController();
	if (PC && MainMenuWidgetClass)
	{
		if (UMainMenuWidget* MenuWidget = CreateWidget<UMainMenuWidget>(PC, MainMenuWidgetClass))
		{
			MenuWidget->AddToViewport();
		}

		PC->bShowMouseCursor = false;

		FInputModeGameAndUI Mode;
		Mode.SetHideCursorDuringCapture(false);
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		PC->SetInputMode(Mode);
	}

	if (UGameInstance* GIBase = GetGameInstance())
	{
		if (ULocalMultiplayerSubsystem* LMS = GIBase->GetSubsystem<ULocalMultiplayerSubsystem>())
		{
			LMS->SetCurrentMappingType(ELocalMultiplayerInputMappingType::Menu);
			LMS->EnsurePlayerIMCs(ELocalMultiplayerInputMappingType::Menu);
		}
	}
}
