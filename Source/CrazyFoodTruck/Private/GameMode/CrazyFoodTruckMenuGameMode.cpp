// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/CrazyFoodTruckMenuGameMode.h"

#include "CrazyFoodTruck/Data/Public/GameInstanceCrazyFoodTruck.h"
#include "PlayerController/MenuPlayerController.h"
#include "LocalMultiplayerSubsystem.h"

#include "EngineUtils.h"

ACrazyFoodTruckMenuGameMode::ACrazyFoodTruckMenuGameMode()
{
	PlayerControllerClass = AMenuPlayerController::StaticClass();

	DefaultPawnClass = nullptr;
	bStartPlayersAsSpectators = true;
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
	if (PC)
	{
		AActor* FoundMenuCamera = nullptr;

		for (TActorIterator<AActor> It(World); It; ++It)
		{
			AActor* Actor = *It;
			if (Actor && Actor->ActorHasTag("MenuCamera"))
			{
				FoundMenuCamera = Actor;
				break;
			}
		}

		if (FoundMenuCamera)
		{
			PC->bAutoManageActiveCameraTarget = false;
			PC->SetViewTarget(FoundMenuCamera);
		}

		PC->bShowMouseCursor = false;

		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		PC->SetInputMode(InputMode);
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
