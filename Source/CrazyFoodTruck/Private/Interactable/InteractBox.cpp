// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable/InteractBox.h"

#include "Damage/RepairProgressBillboard.h"
#include "AttachPoint.h"
#include "Characters/CrazyFoodTruckCharacter.h"
#include "GameMode/CrazyFoodTruckGameMode.h"
#include "LocalMultiplayerSubsystem.h"
#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Components/BoxComponent.h"
#include "Components/Image.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/IVehicule.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Vehicle/Vehicle.h"

//////////////////////////////////////////////////////////////////////////
// Constructor / BeginPlay / Tick

AInteractBox::AInteractBox()
{
    PrimaryActorTick.bCanEverTick = false;

    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    SetRootComponent(Box);

    Box->InitBoxExtent(FVector(100.f));
    Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Box->SetCollisionObjectType(ECC_WorldDynamic);
    Box->SetCollisionResponseToAllChannels(ECR_Ignore);
    Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    Box->OnComponentBeginOverlap.AddDynamic(this, &AInteractBox::OnBoxBeginOverlap);
    Box->OnComponentEndOverlap.AddDynamic(this, &AInteractBox::OnBoxEndOverlap);

    // Defaults (au cas où)
    OverlappTimer = 0.f;
    ExitTimer = BaseOverlappTimer;
    bPlayerIsControlling = false;
    bIsFunctional = true;

    AttachLookPoint = CreateDefaultSubobject<USceneComponent>(TEXT("AttachLookPoint"));
    AttachLookPoint->AttachToComponent(Box, FAttachmentTransformRules::KeepRelativeTransform, "");
}

void AInteractBox::SetWidgetInteractBox(UUInteractBoxWidget* NewWidget)
{
    InteractBoxWidget = NewWidget;
}

void AInteractBox::BeginPlay()
{
    Super::BeginPlay();
}

void AInteractBox::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (ExitTimer> 0.f)
    {
        ExitTimer -= DeltaSeconds;
        if (ExitTimer <= 0.f) // TIME TO CHECK IF PLAYER STILL INSIDE
        {
            DetectPlayerAfterExit();
        }
    }
}

void AInteractBox::DetectPlayerAfterExit()
{
    PlayerDetected.Empty();
    auto playerInsides = DetectPlayersInside();
    if (playerInsides.Num() == 0) // NO PLAYER DETECTED
    {
        if (!EnteringCharacter){return;}
        TryExitPlayer(EnteringCharacter);
        return;
    }
    for (auto PIX : playerInsides)
    {
        if (PIX == EnteringCharacter) // SAME PLAYER STILL INSIDE
        {
            TryDetectPlayer(CurrentInteractorPlayerController.Get(), EnteringCharacter);
            PlayerDetected.Empty();
            return;
        }
    }
    
    if (playerInsides[0] != nullptr)
    {
        if (EnteringCharacter)
        {
            TryExitPlayer(EnteringCharacter);
        }
        APlayerController* PC = GetPlayerControllerFromActor(playerInsides[0]);
        TryDetectPlayer(PC, playerInsides[0]);
        
    }
    PlayerDetected.Empty();
}

bool AInteractBox::CanDetectOverlapp()
{
    if (OverlappTimer > 0.f)
    {
        return false;
    }
    OverlappTimer = BaseOverlappTimer;
    return true;
}

void AInteractBox::UpdateVisibilityInput(bool bIsVisible)
{
    _IsShowingInput = bIsVisible;
}

void AInteractBox::SetInteractableObject(AActor* NewInteractableObject)
{
    InteractableObject = NewInteractableObject;
}

void AInteractBox::SetPawnToPossess(APawn* NewPawnToPossess)
{
    PawnToPossess = NewPawnToPossess;
}

//////////////////////////////////////////////////////////////////////////
// Editor helpers (spawn attach points)

#if WITH_EDITOR
void AInteractBox::SpawnAttachPointInEditor()
{
    if (UWorld* World = GetWorld(); World && World->IsEditorWorld())
    {
        Modify();
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        Params.OverrideLevel = GetLevel();
        Params.ObjectFlags |= RF_Transactional;

        // AttachPoint
        if (!AttachPoint)
        {
            AttachPoint = World->SpawnActor<AAttachPoint>(AAttachPoint::StaticClass(), GetActorTransform(), Params);
            if (AttachPoint)
            {
                AttachPoint->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
                AttachPoint->SetActorLabel(TEXT("AttachPoint"));
                AttachPoint->Modify();
                AttachPoint->MarkPackageDirty();
            }
            MarkPackageDirty();
            return;
        }

        // ReleasePoint
        if (!ReleasePoint)
        {
            ReleasePoint = World->SpawnActor<AAttachPoint>(AAttachPoint::StaticClass(), GetActorTransform(), Params);
            if (ReleasePoint)
            {
                ReleasePoint->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
                ReleasePoint->SetActorLabel(TEXT("ReleasePoint"));
                ReleasePoint->Modify();
                ReleasePoint->MarkPackageDirty();
            }
            MarkPackageDirty();
            return;
        }
    }
}

void AInteractBox::ClearAttachPoint()
{
    if (UWorld* World = GetWorld(); World && World->IsEditorWorld())
    {
        Modify();
        if (AttachPoint) { AttachPoint->Destroy(); AttachPoint = nullptr; }
        if (ReleasePoint) { ReleasePoint->Destroy(); ReleasePoint = nullptr; }
        MarkPackageDirty();
    }
}
#endif

//////////////////////////////////////////////////////////////////////////
// Overlap / detection

void AInteractBox::OnBoxBeginOverlap(UPrimitiveComponent* Comp, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& Hit)
{
    APlayerController* NewPlayerInside = GetPlayerControllerFromActor(Other);
    AddOverlappingPlayerController(NewPlayerInside);
    
    if (bPlayerIsControlling)
    {
        return;
    }

    ACrazyFoodTruckCharacter* NewCharacter = Cast<ACrazyFoodTruckCharacter>(Other);
    if (!NewCharacter){return;}
    
    if (EnteringCharacter != nullptr)
    {
        if (NewCharacter != EnteringCharacter)
        {
            return;
        }
    }
    
    if (NewCharacter)
    {
        EnteringCharacter = NewCharacter;
        TryDetectPlayer(NewPlayerInside, EnteringCharacter);
    }else
    {
    }
}

void AInteractBox::OnBoxEndOverlap(UPrimitiveComponent* Comp, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex)
{
   // if (!CanDetectOverlapp()) return;
    if (bPlayerIsControlling) return;

    APlayerController* LeavingPlayerController = GetPlayerControllerFromActor(Other);

    ACrazyFoodTruckCharacter* Character = Cast<ACrazyFoodTruckCharacter>(Other);

    RemoveOverlappingPlayerController(LeavingPlayerController);

    if (EnteringCharacter != Character)
    {
        return;
    }

    ExitTimer = BaseOverlappTimer;
}

void AInteractBox::TryExitPlayer(ACrazyFoodTruckCharacter* Character)
{
    if (bPlayerIsControlling)
    {
        return;
    }
    if (Character)
    {
        // PRINT INDEX OF THE CHARACTER EXITING
       // GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red , FString::Printf(TEXT("CHARACTER EXITING - P%d"), GetPlayerIndexFromPlayerController(GetPlayerControllerFromActor(Character))));
        if (Character->GetFocusedInteractable().GetInterface() == static_cast<IInteractable*>(this))
        {
            Character->SetFocusedInteractable(TScriptInterface<IInteractable>(nullptr));
        }
        OnCollisionExit.Broadcast();
        CurrentInteractorPlayerController = nullptr;
        CachedCharacter = nullptr;
        CachedPlayerController = nullptr;
        CachedPreviousPawn = nullptr;
        EnteringCharacter = nullptr;
    }else
    {
    }
}

void AInteractBox::TryDetectPlayer(APlayerController* PlayerController, ACrazyFoodTruckCharacter* Character)
{
    if (!Character)
    {
        return;
    }
    if (!PlayerController)
    {
        return;
    }

    const bool bLockedByAnother = CurrentInteractorPlayerController.IsValid() && PlayerController && (CurrentInteractorPlayerController.Get() != PlayerController);
    const bool bAnotherInside = IsAnotherPlayerAlreadyInside(PlayerController);

    
    if (!bLockedByAnother && !bAnotherInside || Character == EnteringCharacter)
    {
        Character->SetFocusedInteractable(TScriptInterface<IInteractable>(this));
        if (!CurrentInteractorPlayerController.IsValid())
        {
            OnCollisionEnter.Broadcast();
            if (InteractBoxWidget)
            {
                InteractBoxWidget->SetColor(GetPlayerColorFromPlayerController(PlayerController));
            }
            if (!EnteringCharacter)
            {
                EnteringCharacter = Character;
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////
// Interaction dispatch (possess vs interact object)

void AInteractBox::Interact(APlayerController* InstigatorPlayerController, ACrazyFoodTruckCharacter* CrazyCharacter)
{
    if (bPlayerIsControlling) return;
    if (!InstigatorPlayerController) return;

    if (bBreakable && RepairProgressBillboard && RepairProgressBillboard->IsDamaged())
    {
        RepairProgressBillboard->HandleRepairInput();
        return;
    }else
    {
    }

    const FColor PlayerColor = GetPlayerColorFromPlayerController(InstigatorPlayerController);
    const int32 PlayerIndex = GetPlayerIndexFromPlayerController(InstigatorPlayerController);
    const FString PlayerLabel = FString::Printf(TEXT("[P%d] "), PlayerIndex);

    if (CurrentInteractorPlayerController.IsValid() && CurrentInteractorPlayerController.Get() != InstigatorPlayerController)
    {
        return;
    }

    // Lock and dispatch
    if (!CurrentInteractorPlayerController.IsValid())
    {
        CurrentInteractorPlayerController = InstigatorPlayerController;
        OnInteractionStarted.Broadcast(InstigatorPlayerController);
    }

    switch (InteractionType)
    {
    case EInteractionType::Possess:
        if (CrazyCharacter && CrazyCharacter->CanInteract())
        {
            TryPossesPawn(InstigatorPlayerController);
        }
        break;
    case EInteractionType::Interactable:
        TryInteractWithObject(InstigatorPlayerController, CrazyCharacter);
        break;
    case EInteractionType::Both:
        if (CrazyCharacter && CrazyCharacter->CanInteract())
        {
            TryPossesPawn(InstigatorPlayerController);
        }
        TryInteractWithObject(InstigatorPlayerController, CrazyCharacter);
        break;
    default:
        break;
    }

}

void AInteractBox::TryInteractWithObject(APlayerController* InstigatorPlayerController,ACrazyFoodTruckCharacter* CrazyCharacter)
{
    if (!InteractableObject) return;
    if (!InstigatorPlayerController) return;
    if (!CrazyCharacter) return;
    
    IInteractable* InteractableObjectInterface = Cast<IInteractable>(InteractableObject);
    if (InteractableObjectInterface)
    {
        InteractableObjectInterface->Interact(InstigatorPlayerController, CrazyCharacter);
        OnInteractionStarted.Broadcast(InstigatorPlayerController);
    }
}

void AInteractBox::TryPossesPawn(APlayerController* InstigatorPlayerController)
{
    if (!InstigatorPlayerController) return;
    if (!CurrentInteractorPlayerController.IsValid())
    {
        //PLAY SOUND 2D
        
        UGameplayStatics::PlaySound2D(GetWorld(), InteractionSound);
        CurrentInteractorPlayerController = InstigatorPlayerController;
        OnInteractionStarted.Broadcast(InstigatorPlayerController);
    }

    if (PawnToPossess)
    {
        PossessPawn(InstigatorPlayerController);
    }
}

//////////////////////////////////////////////////////////////////////////
// Possess / Unpossess flow

void AInteractBox::PossessPawn(APlayerController* PlayerController)
{
    if (!PlayerController) return;

    CurrentPlayerId = GetPlayerIndexFromPlayerController(PlayerController);

    bPlayerIsControlling = true;
    CachedPlayerController = PlayerController;
    CachedCharacter = PlayerController ? Cast<ACrazyFoodTruckCharacter>(PlayerController->GetPawn()) : nullptr;
    CachedPreviousPawn = PlayerController ? PlayerController->GetPawn() : nullptr;

    if (CachedCharacter.IsValid())
    {
        RotationActorOnEnter = CachedCharacter->GetActorRotation();
        RotationControllerOnEnter = PlayerController->GetControlRotation();
    }
    
    if (!bCanBreakWhilePossessed && bBreakable)
    {
        SetFunctional(true);
        if (RepairProgressBillboard)
        {
            RepairProgressBillboard->SetDamaged(false);
        }
    }
    
    TeleportAndAttachPlayer(PlayerController);
    
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>())
        {
            const int32 PlayerIndex = GetPlayerIndexFromPlayerController(PlayerController);
            if (PlayerIndex != -1 && PawnToPossess)
            {
                CachedCharacter->bIsInModule = true;
                if(Cast<AVehicle>(PawnToPossess))
                {
                    APlayerController* PC = CurrentInteractorPlayerController.Get(); 
                    int CurrentPlayerIndex = GetPlayerIndexFromPlayerController(PC);
                    AddOutlineToForwardCamera(CurrentPlayerIndex);
                    LocalMultiplayerSubsystem->PossessPawnForPlayerIndex(PlayerIndex, PawnToPossess, MappingType, true);
                }else
                {
                    LocalMultiplayerSubsystem->PossessPawnForPlayerIndex(PlayerIndex, PawnToPossess, MappingType, false);
                    CachedCharacter->MovementType = EMovementType::ECC_Push;
                    auto targetRotation = UKismetMathLibrary::FindLookAtRotation(CachedCharacter->GetActorLocation(), AttachLookPoint->GetComponentLocation());
                    CachedCharacter->SetActorRotation(targetRotation);
                    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green , "ENTER SET ROTAION");
                }
            }
        }
    }
    
    if (UWorld* World = GetWorld())
    {
        if (auto* GM = Cast<ACrazyFoodTruckGameMode>(UGameplayStatics::GetGameMode(World)))
        {
            GM->ApplyGlobalViewTo(PlayerController);
        }
    }
}

void AInteractBox::UnpossessPawn()
{
    OnPlayerQuit.Broadcast();
    if (CachedCharacter.IsValid())
    {
        CachedCharacter->MovementType = EMovementType::ECC_Idle;
    }
    
    bPlayerIsControlling = false;
    CurrentInteractorPlayerController = nullptr;
    
    if (!CachedPlayerController.IsValid())
        return;
    
    const int32 PlayerIndex = GetPlayerIndexFromPlayerController(CachedPlayerController.Get());
    if (PlayerIndex != -1)
    {
        if (UGameInstance* GI = GetGameInstance())
        {
            if (ULocalMultiplayerSubsystem* LMS = GI->GetSubsystem<ULocalMultiplayerSubsystem>())
            {
                CachedCharacter->bIsInModule = false;
                if(Cast<AVehicle>(PawnToPossess))
                {
                    LMS->UnPossessPawnForPlayerIndex(PlayerIndex, CachedPreviousPawn.Get(), MappingType, true);
                }else
                {
                    LMS->UnPossessPawnForPlayerIndex(PlayerIndex, CachedPreviousPawn.Get(), MappingType, false);
                }
            }
        }
    }
    
    TeleportBackAndDetachPlayer(CachedPlayerController.Get());
    
    if (CachedCharacter.IsValid())
    {
        CachedCharacter->SetActorRotation(RotationActorOnEnter);
        if (CachedPlayerController.IsValid())
        {
            CachedPlayerController->SetControlRotation(RotationControllerOnEnter);
        }
    }
    else
    {
        // safe clear
        if (CachedPlayerController.IsValid())
        {
            if (APawn* Pawn = CachedPlayerController->GetPawn())
            {
                if (ACrazyFoodTruckCharacter* Character = Cast<ACrazyFoodTruckCharacter>(Pawn))
                {
                    Character->SetFocusedInteractable(nullptr);
                }
            }
        }
    }
    
    auto players = DetectPlayerInside();
    if (players)
    {
        APlayerController* PC = GetPlayerControllerFromActor(players);
        TryDetectPlayer(PC, players);
    }
    
    if (UWorld* World = GetWorld())
    {
        if (auto* GM = Cast<ACrazyFoodTruckGameMode>(UGameplayStatics::GetGameMode(World)))
        {
            GM->ApplyGlobalViewTo(CachedPlayerController.Get());
        }
    }
}

void AInteractBox::SetRepairProgress(URepairProgressBillboard* RPB)
{
    this->RepairProgressBillboard = RPB;

    if (RepairProgressBillboard)
    {
        RepairProgressBillboard->OnRepaired.AddDynamic(this, &AInteractBox::OnRepairCompleted);
    }

    if (bStartDamaged && bBreakable)
    {
        TryForceBreak();
    }
    else
    {
        SetFunctional(true);
        if (RepairProgressBillboard)
        {
            RepairProgressBillboard->SetDamaged(false);
        }
    }
}

//////////////////////////////////////////////////////////////////////////
// Teleport / attach helpers

void AInteractBox::TeleportAndAttachPlayer(APlayerController* PlayerController)
{
    if (!PlayerController || !AttachPoint)
    {
        return;
    }

    if (APawn* Pawn = PlayerController->GetPawn())
    {
        Pawn->SetActorLocation(AttachPoint->GetActorLocation());
        Pawn->AttachToActor(AttachPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    }
}

void AInteractBox::TeleportBackAndDetachPlayer(APlayerController* PlayerController)
{
    if (!PlayerController || !ReleasePoint)
    {
        return;
    }

    APawn* Pawn = PlayerController->GetPawn();
    if (!Pawn) return;

    const FVector TargetLocation = ReleasePoint->GetActorLocation();
    const FRotator TargetRotation = ReleasePoint->GetActorRotation();

    Pawn->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

    if (ACharacter* Char = Cast<ACharacter>(Pawn))
    {
        if (UCharacterMovementComponent* Move = Char->GetCharacterMovement())
        {
            Move->StopMovementImmediately();
            Move->Velocity = FVector::ZeroVector;
            Move->DisableMovement();
            Move->SetComponentTickEnabled(false);
        }
    }

    if (!Pawn->SetActorLocation(TargetLocation, false, nullptr, ETeleportType::TeleportPhysics))
    {
        Pawn->TeleportTo(TargetLocation, TargetRotation, false, true);
    }
    
    if (ACharacter* Char = Cast<ACharacter>(Pawn))
    {
        if (UCharacterMovementComponent* Move2 = Char->GetCharacterMovement())
        {
            Move2->StopMovementImmediately();
            Move2->Velocity = FVector::ZeroVector;
            Move2->SetComponentTickEnabled(true);
            Move2->Activate();
            Move2->SetMovementMode(MOVE_Walking);
        }
        Char->GetMesh()->SetCustomDepthStencilValue(true);
    }

}

//////////////////////////////////////////////////////////////////////////
// Utility / housekeeping

APlayerController* AInteractBox::GetPlayerControllerFromActor(AActor* Actor) const
{
    if (!Actor) return nullptr;
    if (APawn* Pawn = Cast<APawn>(Actor))
    {
        return Cast<APlayerController>(Pawn->GetController());
    }
    return Cast<APlayerController>(Actor);
}

int32 AInteractBox::GetPlayerIndexFromPlayerController(APlayerController* PlayerController) const
{
    if (!PlayerController) return -1;
    if (const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
    {
        return LocalPlayer->GetControllerId();
    }
    return -1;
}

FColor AInteractBox::GetPlayerColorFromPlayerController(APlayerController* PlayerController) const
{
    if (!PlayerController) return FColor::White;
    if (APawn* Pawn = PlayerController->GetPawn())
    {
        if (const ACrazyFoodTruckCharacter* Character = Cast<ACrazyFoodTruckCharacter>(Pawn))
        {
            return Character->GetPlayerColor().ToFColor(true);
        }
    }

    switch (GetPlayerIndexFromPlayerController(PlayerController))
    {
    case 0: return FColor(0,115,255);
    case 1: return FColor(26,204,51);
    case 2: return FColor(255,26,26);
    case 3: return FColor(255,230,26);
    default: return FColor::White;
    }
}

void AInteractBox::AddOverlappingPlayerController(APlayerController* PlayerController)
{
    if (PlayerController)
    {
        OverlappingPlayerControllers.Add(PlayerController);
    }
}

void AInteractBox::RemoveOverlappingPlayerController(APlayerController* PlayerController)
{
    if (PlayerController)
    {
        OverlappingPlayerControllers.Remove(PlayerController);
    }
}

bool AInteractBox::IsAnotherPlayerAlreadyInside(APlayerController* ThisPlayerController) const
{
    int32 ValidCount = 0;
    for (const TWeakObjectPtr<APlayerController>& It : OverlappingPlayerControllers)
    {
        if (It.IsValid())
        {
            ++ValidCount;
            if (ThisPlayerController && It.Get() != ThisPlayerController)
            {
                return true;
            }
        }
    }
    return ValidCount > 1;
}

ACrazyFoodTruckCharacter* AInteractBox::DetectPlayerInside()
{
    TArray<AActor*> OverlappingActors;
    Box->GetOverlappingActors(OverlappingActors, APawn::StaticClass());
    for (AActor* Actor : OverlappingActors)
    {
        if (ACrazyFoodTruckCharacter* Char = Cast<ACrazyFoodTruckCharacter>(Actor))
        {
            return Char;
        }
    }
    return nullptr;
}

TArray<ACrazyFoodTruckCharacter*> AInteractBox::DetectPlayersInside()
{
    PlayerDetected.Empty();
    Box->GetOverlappingActors(DetectedActors, APawn::StaticClass());
    for (AActor* Actor : DetectedActors)
    {
        if (ACrazyFoodTruckCharacter* Char = Cast<ACrazyFoodTruckCharacter>(Actor))
        {
            PlayerDetected.Add(Char);
        }
    }
    DetectedActors.Empty();
    return PlayerDetected;
}

bool AInteractBox::PlayerStillInsideCheck(ACrazyFoodTruckCharacter* TargetCharacter)
{
    TArray<AActor*> OverlappingActors;
    Box->GetOverlappingActors(OverlappingActors, APawn::StaticClass());
    for (AActor* Actor : OverlappingActors)
    {
        if (Cast<ACrazyFoodTruckCharacter>(Actor) == TargetCharacter)
        {
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////
// Break / repair

void AInteractBox::SetFunctional(bool bNewFunctional)
{
    bIsFunctional = bNewFunctional;
}

bool AInteractBox::CanBeBrokenNow() const
{
    return bBreakable && bIsFunctional && (!bPlayerIsControlling || bCanBreakWhilePossessed);
}

bool AInteractBox::TryBreakModule()
{
    if (CanBeBrokenNow())
    {
        TryForceBreak();
        return true;
    }
    return false;
}

void AInteractBox::TryForceBreak()
{
    if (!bBreakable) return;
    if (bPlayerIsControlling && !bCanBreakWhilePossessed) return;

    SetFunctional(false);
    if (RepairProgressBillboard)
    {
        RepairProgressBillboard->SetDamaged(true);
    }
    OnDamaged.Broadcast();
}

void AInteractBox::OnRepairCompleted()
{
    SetFunctional(true);
    OnRestored.Broadcast();
}

void AInteractBox::TryReleaseLockFromActor(APlayerController* LeavingPlayerController)
{
    if (!LeavingPlayerController)
    {
        return;
    }
    if (CurrentInteractorPlayerController.IsValid() && CurrentInteractorPlayerController.Get() == LeavingPlayerController)
    {
        CurrentInteractorPlayerController = nullptr;
    }
}
