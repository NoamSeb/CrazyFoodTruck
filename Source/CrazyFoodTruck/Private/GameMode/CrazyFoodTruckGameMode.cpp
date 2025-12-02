// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/CrazyFoodTruckGameMode.h"

#include "CrazyFoodTruck/CrazyFoodTruckSettings.h"

#include "Characters/CrazyFoodTruckCharacter.h"
#include "Characters/CrazyFoodTruckCharacterSettings.h"
#include "Characters/CrazyFoodTruckCharacterInputData.h"

#include "GameState/CrazyFoodTruckGameState.h"

#include "Timer/MatchTimerComponent.h"
#include "Score/ScoreManagerComponent.h"

#include "../HordeManager.h"

#include "HUD/CrazyFoodTruckHUD.h"

#include "LocalMultiplayerSubsystem.h"

#include "Components/SceneComponent.h"

#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerController.h"

#include "Kismet/GameplayStatics.h"

ACrazyFoodTruckGameMode::ACrazyFoodTruckGameMode()
{
    GameStateClass = ACrazyFoodTruckGameState::StaticClass();
    ScoreManager = CreateDefaultSubobject<UScoreManagerComponent>(TEXT("ScoreManager"));
}

void ACrazyFoodTruckGameMode::BeginPlay()
{
    Super::BeginPlay();

    UWorld* World = GetWorld();
    if (!World) return;

    UGameInstance* GIBase = GetGameInstance();
    if (!GIBase) return;

    UGameInstanceCrazyFoodTruck* GI = Cast<UGameInstanceCrazyFoodTruck>(GIBase);
    if (!GI) return;

    CreateAndInitPlayers();

    TArray<APlayerStart*> PlayerStartsPoints;
    FindPlayerStartActors(PlayerStartsPoints);
    SpawnCharacters(PlayerStartsPoints);

    AActor* Vehicle = ResolveVehicleActor();
    VehicleActorRef = Vehicle;

    AActor* ViewTarget = ResolveViewTargetActor();
    GlobalViewTarget = ViewTarget;

    ConfigureMovementFrameForAllCharacters(Vehicle);

    if (ULocalMultiplayerSubsystem* LMS = GI->GetSubsystem<ULocalMultiplayerSubsystem>())
    {
        LMS->EnsurePlayerIMCs(ELocalMultiplayerInputMappingType::Player);
    }

    ApplyGlobalViewToAllPlayers();

    if (ACrazyFoodTruckGameState* GS = GetWorld() ? GetWorld()->GetGameState<ACrazyFoodTruckGameState>() : nullptr)
    {
        if (UMatchTimerComponent* Timer = GS->GetMatchTimer())
        {
            Timer->StartTimer(0.f);
            Timer->OnTimerSecond.AddDynamic(this, &ACrazyFoodTruckGameMode::HandleTimerSecondPrint);
        }
    }
}

void ACrazyFoodTruckGameMode::CreateAndInitPlayers() const
{
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>())
        {
            LocalMultiplayerSubsystem->CreateAndInitPlayers(ELocalMultiplayerInputMappingType::Player);
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
    UWorld* World = GetWorld();
    if (!World) return;

    UGameInstanceCrazyFoodTruck* GI = World->GetGameInstance<UGameInstanceCrazyFoodTruck>();
    if (!GI) return;

    UCrazyFoodTruckCharacterInputData* InputData = LoadInputDataFromConfig();
    UInputMappingContext* InputMappingContext = LoadInputMappingContextFromConfig();

    Characters.Reset();

    const int32 NumSlots = GI->PlayerSlots.Num();

    for (int32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
    {
        if (!GI->PlayerSlots.IsValidIndex(SlotIndex))
        {
            continue;
        }

        const FMenuPlayerSlot& Slot = GI->PlayerSlots[SlotIndex];

        if (!Slot.bIsConnected || Slot.ControllerId == INDEX_NONE)
        {
            continue;
        }

        if (!SpawnPoints.IsValidIndex(SlotIndex))
        {
            UE_LOG(LogTemp, Warning, TEXT("No PlayerStart for SlotIndex %d"), SlotIndex);
            continue;
        }

        APlayerStart* SpawnPoint = SpawnPoints[SlotIndex];
        if (!SpawnPoint)
        {
            continue;
        }

        APlayerController* PC = FindPlayerControllerByControllerId(World, Slot.ControllerId);
        if (!PC)
        {
            UE_LOG(LogTemp, Warning, TEXT("No PlayerController found for ControllerId %d (SlotIndex %d)"), Slot.ControllerId, SlotIndex);
            continue;
        }

        TSubclassOf<ACrazyFoodTruckCharacter> CrazyCharClass = GetCrazyFoodTruckCharacterClassFromSlotIndex(SlotIndex);
        if (!CrazyCharClass)
        {
            continue;
        }

        FTransform SpawnTransform = SpawnPoint->GetActorTransform();

        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        ACrazyFoodTruckCharacter* NewCharacter = World->SpawnActorDeferred<ACrazyFoodTruckCharacter>(CrazyCharClass, SpawnTransform);
        if (!NewCharacter)
        {
            continue;
        }

        NewCharacter->SetInputData(InputData);
        NewCharacter->SetInputMappingContext(InputMappingContext);

        const FLinearColor PlayerColor = GI->GetPlayerColorForIndex(SlotIndex);
        NewCharacter->SetPlayerColor(PlayerColor);

        NewCharacter->FinishSpawning(SpawnTransform);

        PC->Possess(NewCharacter);

        if (InputMappingContext)
        {
            NewCharacter->SetupMappingContextIntoController();
        }

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

TSubclassOf<ACrazyFoodTruckCharacter> ACrazyFoodTruckGameMode::GetCrazyFoodTruckCharacterClassFromSlotIndex(int32 SlotIndex) const
{
    const UCrazyFoodTruckSettings* CrazyFoodTruckSettings = GetDefault<UCrazyFoodTruckSettings>();
    if (!CrazyFoodTruckSettings)
    {
        return nullptr;
    }

    switch (SlotIndex)
    {
    case 0:
        return CrazyFoodTruckSettings->CrazyFoodTruckCharacterClassP0;
    case 1:
        return CrazyFoodTruckSettings->CrazyFoodTruckCharacterClassP1;
    case 2:
        return CrazyFoodTruckSettings->CrazyFoodTruckCharacterClassP2;
    case 3:
        return CrazyFoodTruckSettings->CrazyFoodTruckCharacterClassP3;
    default:
        return CrazyFoodTruckSettings->CrazyFoodTruckCharacterClassP0;
    }
}

APlayerController* ACrazyFoodTruckGameMode::FindPlayerControllerByControllerId(UWorld* World, int32 ControllerId) const
{
    if (!World || ControllerId == INDEX_NONE)
    {
        return nullptr;
    }

    for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
    {
        if (APlayerController* PC = It->Get())
        {
            if (const ULocalPlayer* LP = PC->GetLocalPlayer())
            {
                if (LP->GetControllerId() == ControllerId)
                {
                    return PC;
                }
            }
        }
    }

    return nullptr;
}

AActor* ACrazyFoodTruckGameMode::ResolveVehicleActor() const
{
    UWorld* World = GetWorld();
    if (!World) return nullptr;

    TArray<AActor*> Trucks;
    UGameplayStatics::GetAllActorsWithTag(World, FName("FoodTruck"), Trucks);
    return (Trucks.Num() > 0) ? Trucks[0] : nullptr;
}

AActor* ACrazyFoodTruckGameMode::ResolveViewTargetActor() const
{
    UWorld* World = GetWorld();
    if (!World) return nullptr;

    TArray<AActor*> Cams;
    UGameplayStatics::GetAllActorsWithTag(World, FName("GlobalCamera"), Cams);
    if (Cams.Num() > 0)
    {
        return Cams[0];
    }

    return ResolveVehicleActor();
}

void ACrazyFoodTruckGameMode::ConfigureMovementFrameForAllCharacters(AActor* Vehicle)
{
    for (ACrazyFoodTruckCharacter* C : Characters)
    {
        if (!IsValid(C)) continue;

        if (Vehicle)
        {
            C->UseVehicleFrame(Vehicle);
            C->MovementYawOffsetDegrees = 180.f;
        }
        else
        {
            C->UseWorldFrame();
            C->MovementYawOffsetDegrees = 0.f;
        }
    }
}

void ACrazyFoodTruckGameMode::ApplyGlobalViewTo(APlayerController* PC) const
{
    if (!PC || !GlobalViewTarget.IsValid()) return;

    PC->bAutoManageActiveCameraTarget = false;
    PC->SetViewTargetWithBlend(GlobalViewTarget.Get(), 0.f);
}

void ACrazyFoodTruckGameMode::ApplyGlobalViewToAllPlayers() const
{
    if (!GlobalViewTarget.IsValid()) return;

    UWorld* World = GetWorld();
    if (!World) return;

    for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
    {
        if (APlayerController* PC = It->Get())
        {
            ApplyGlobalViewTo(PC);
        }
    }
}

void ACrazyFoodTruckGameMode::HandleTimerSecondPrint(int32 ElapsedSeconds)
{
    if (!GEngine) return;

    static const int32 MsgKey = 99999;
    const FString Text = FString::Printf(TEXT("Time: %s"), *FormatMMSS(ElapsedSeconds));

    GEngine->AddOnScreenDebugMessage(MsgKey, 1.1f, FColor::Green, Text);

    if (!bHasComputedFinalScore && ElapsedSeconds >= 30)
    {
        EvaluateFinalScore();
    }
}

FString ACrazyFoodTruckGameMode::FormatMMSS(int32 TotalSeconds)
{
    const int32 Minutes = TotalSeconds / 60;
    const int32 Seconds = TotalSeconds % 60;
    return FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
}

void ACrazyFoodTruckGameMode::EvaluateFinalScore()
{
    bHasComputedFinalScore = true;

    UWorld* World = GetWorld();
    if (!World || !ScoreManager) return;

    ACrazyFoodTruckGameState* GS = World->GetGameState<ACrazyFoodTruckGameState>();
    if (!GS || !GS->GetMatchTimer()) return;

    const int32 TimeSeconds = GS->GetMatchTimer()->GetElapsedSeconds();

    AHordeManager* HordeMgr = ResolveHordeManager();
    const int32 Kills = HordeMgr ? HordeMgr->GetZombiesKilledCount() : 0;

    int32 TimeScore = 0;
    int32 KillScore = 0;
    
    const int32 FinalScore = ScoreManager->ComputeTotalScore(TimeSeconds, Kills, TimeScore, KillScore);
    const EScoreGrade Grade = ScoreManager->GetGradeForScore(FinalScore);

    GS->SetScoreValues(FinalScore, TimeScore, KillScore, Grade);

    if (APlayerController* PC = World->GetFirstPlayerController())
    {
        if (ACrazyFoodTruckHUD* HUD = Cast<ACrazyFoodTruckHUD>(PC->GetHUD()))
        {
            HUD->ShowScoreResult(FinalScore, TimeScore, KillScore, Grade);
        }
    }
}

AHordeManager* ACrazyFoodTruckGameMode::ResolveHordeManager() const
{
    UWorld* World = GetWorld();
    if (!World) return nullptr;

    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(World, AHordeManager::StaticClass(), Found);
    return (Found.Num() > 0) ? Cast<AHordeManager>(Found[0]) : nullptr;
}
