// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LocalMultiplayerGameMode.h"

#include "LocalMultiplayerSubsystem.h"

void ALocalMultiplayerGameMode::BeginPlay()
{
    Super::BeginPlay();

    CreateAndInitPlayers();
}

void ALocalMultiplayerGameMode::CreateAndInitPlayers() const
{
    UGameInstance* GameInstance = GetGameInstance();
    if (!GameInstance)
    {
        return;
    }

    ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
    if (!LocalMultiplayerSubsystem)
    {
        return;
    }

    LocalMultiplayerSubsystem->CreateAndInitPlayers(ELocalMultiplayerInputMappingType::InGame);
}

