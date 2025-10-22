// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/CrazyFoodTruckGameMode.h"

#include "CrazyFoodTruck/CrazyFoodTruckSettings.h"

#include "Characters/CrazyFoodTruckCharacter.h"
#include "Characters/CrazyFoodTruckCharacterSettings.h"
#include "Characters/CrazyFoodTruckCharacterInputData.h"

#include "LocalMultiplayerSubsystem.h"

#include "GameFramework/PlayerStart.h"

#include "Kismet/GameplayStatics.h"

void ACrazyFoodTruckGameMode::BeginPlay()
{
    Super::BeginPlay();

    CreateAndInitPlayers();

    TArray<APlayerStart*> PlayerStartsPoints;
    FindPlayerStartActors(PlayerStartsPoints);
    SpawnCharacters(PlayerStartsPoints);
}

void ACrazyFoodTruckGameMode::CreateAndInitPlayers() const
{
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>())
        {
            LocalMultiplayerSubsystem->CreateAndInitPlayers(ELocalMultiplayerInputMappingType::InGame);
        }
    }
}

void ACrazyFoodTruckGameMode::FindPlayerStartActors(TArray<APlayerStart*>& ResultsActors) const
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        if (APlayerStart* PlayerStartActor = Cast<APlayerStart>(Actor))
        {
            ResultsActors.Add(PlayerStartActor);
        }
    }
}

void ACrazyFoodTruckGameMode::SpawnCharacters(const TArray<APlayerStart*>& SpawnPoints)
{
    UCrazyFoodTruckCharacterInputData* InputData = LoadInputDataFromConfig();
    UInputMappingContext* InputMappingContext = LoadInputMappingContextFromConfig();

    for (APlayerStart* SpawnPoint : SpawnPoints)
    {
        const EAutoReceiveInput::Type InputType = SpawnPoint->AutoReceiveInput.GetValue();
        TSubclassOf<ACrazyFoodTruckCharacter> CrazyFoodTruckCharacterClass = GetCrazyFoodTruckCharacterClassFromInputType(InputType);
        if (!CrazyFoodTruckCharacterClass)
        {
            continue;
        }

        ACrazyFoodTruckCharacter* NewCharacter = GetWorld()->SpawnActorDeferred<ACrazyFoodTruckCharacter>(CrazyFoodTruckCharacterClass, SpawnPoint->GetTransform());
        if (!NewCharacter)
        {
            continue;
        }

        NewCharacter->AutoPossessPlayer = SpawnPoint->AutoReceiveInput;

        NewCharacter->SetInputData(InputData);
        // NewCharacter->SetInputMappingContext(InputMappingContext);

        NewCharacter->FinishSpawning(SpawnPoint->GetTransform());

        Characters.Add(NewCharacter);
    }
}

UCrazyFoodTruckCharacterInputData* ACrazyFoodTruckGameMode::LoadInputDataFromConfig() const
{
    const UCrazyFoodTruckCharacterSettings* CharacterSettings = GetDefault<UCrazyFoodTruckCharacterSettings>();
    return CharacterSettings ? CharacterSettings->InputData.LoadSynchronous() : nullptr;
}

UInputMappingContext* ACrazyFoodTruckGameMode::LoadInputMappingContextFromConfig() const
{
    const UCrazyFoodTruckCharacterSettings* CharacterSettings = GetDefault<UCrazyFoodTruckCharacterSettings>();
    return CharacterSettings ? CharacterSettings->InputMappingContext.LoadSynchronous() : nullptr;
}

TSubclassOf<ACrazyFoodTruckCharacter> ACrazyFoodTruckGameMode::GetCrazyFoodTruckCharacterClassFromInputType(EAutoReceiveInput::Type InputType) const
{
    const UCrazyFoodTruckSettings* CrazyFoodTruckSettings = GetDefault<UCrazyFoodTruckSettings>();
    if (!CrazyFoodTruckSettings)
    {
        return nullptr;
    }

    switch (InputType)
    {
    case EAutoReceiveInput::Player0:
        return CrazyFoodTruckSettings->CrazyFoodTruckCharacterClassP0;
    case EAutoReceiveInput::Player1:
        return CrazyFoodTruckSettings->CrazyFoodTruckCharacterClassP1;
    case EAutoReceiveInput::Player2:
        return CrazyFoodTruckSettings->CrazyFoodTruckCharacterClassP2;
    case EAutoReceiveInput::Player3:
        return CrazyFoodTruckSettings->CrazyFoodTruckCharacterClassP3;
    default:
        return nullptr;
    }
}
