// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable/InteractBox.h"

#include "Damage/RepairProgressBillboard.h"
#include "AttachPoint.h"
#include "Characters/CrazyFoodTruckCharacter.h"
#include "GameMode/CrazyFoodTruckGameMode.h"
#include "LocalMultiplayerSubsystem.h"
#include "Components/BoxComponent.h"
#include "Components/Image.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/IVehicule.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
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
        if (ExitTimer <= 0.f)
        {
            if (!PlayerStillInsideCheck(EnteringCharacter))
            {
                GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Exit Timer finished"));
                TryExitPlayer(EnteringCharacter);
            }
        }
    }

    if (OverlappTimer > 0.f)
    {
        OverlappTimer -= DeltaSeconds;
        if (OverlappTimer <= 0.f)
        {
           ACrazyFoodTruckCharacter* playerInsidfe = DetectPlayerInside();
            if (playerInsidfe == nullptr)
            {
                if (!EnteringCharacter){return;}
               // GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("No player inside"));
                TryExitPlayer(EnteringCharacter);
            }
            else if (playerInsidfe != EnteringCharacter)
            {
                APlayerController* PC = GetPlayerControllerFromActor(playerInsidfe);
                TryDetectPlayer(PC, playerInsidfe);
               // GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Same player inside"));
            }
        }
    }
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
    //if (!CanDetectOverlapp()) return;
    if (bPlayerIsControlling)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red ,TEXT("Player Controlling"));
        return;
    }

    APlayerController* EnteringPlayerController = GetPlayerControllerFromActor(Other);
    AddOverlappingPlayerController(EnteringPlayerController);

    if (ACrazyFoodTruckCharacter* Character = Cast<ACrazyFoodTruckCharacter>(Other))
    {
        EnteringCharacter = Character;
        TryDetectPlayer(EnteringPlayerController, Character);
    }
}

void AInteractBox::OnBoxEndOverlap(UPrimitiveComponent* Comp, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex)
{
   // if (!CanDetectOverlapp()) return;
    if (bPlayerIsControlling) return;

    APlayerController* LeavingPlayerController = GetPlayerControllerFromActor(Other);
    ACrazyFoodTruckCharacter* Character = Cast<ACrazyFoodTruckCharacter>(Other);

    RemoveOverlappingPlayerController(LeavingPlayerController);
    ExitTimer = BaseOverlappTimer;
}

void AInteractBox::TryExitPlayer(ACrazyFoodTruckCharacter* Character)
{
    if (bPlayerIsControlling){return;}
    if (Character)
    {
        if (Character->GetFocusedInteractable().GetInterface() == static_cast<IInteractable*>(this))
        {
            Character->SetFocusedInteractable(TScriptInterface<IInteractable>(nullptr));
        }
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green ,TEXT("Player exited interact box."));
        OnCollisionExit.Broadcast();
        CurrentInteractorPlayerController = nullptr;
        CachedCharacter = nullptr;
        CachedPlayerController = nullptr;
        CachedPreviousPawn = nullptr;
        EnteringCharacter = nullptr;
    }else
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red ,TEXT("No character to exit."));
    }
}

void AInteractBox::TryDetectPlayer(APlayerController* PlayerController, ACrazyFoodTruckCharacter* Character)
{
    if (!Character)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red ,TEXT("No Character"));
        return;
    }
    if (!PlayerController)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red ,TEXT("No CONTROLLLER"));

        return;
    }

    const bool bLockedByAnother = CurrentInteractorPlayerController.IsValid() && PlayerController && (CurrentInteractorPlayerController.Get() != PlayerController);
    const bool bAnotherInside = IsAnotherPlayerAlreadyInside(PlayerController);

    if (!bLockedByAnother && !bAnotherInside)
    {
        Character->SetFocusedInteractable(TScriptInterface<IInteractable>(this));
        if (!CurrentInteractorPlayerController.IsValid())
        {
            OnCollisionEnter.Broadcast();
        }
    }
    else
    {
        if (bLockedByAnother)
        {
             GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("InteractBox: Already in use by another player."));
        }
        else if (bAnotherInside)
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("InteractBox: Another player is already inside."));
        }
    }
}

//////////////////////////////////////////////////////////////////////////
// Interaction dispatch (possess vs interact object)

void AInteractBox::Interact(APlayerController* InstigatorPlayerController, ACrazyFoodTruckCharacter* CrazyCharacter)
{
    if (bPlayerIsControlling) return;
    if (!InstigatorPlayerController) return;

    // If it's repairable and damaged, treat input as repair.
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
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, PlayerColor, PlayerLabel + TEXT("Already in use by another player."));
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
        // require the character to be able to interact (optional hook)
        if (CrazyCharacter && CrazyCharacter->CanInteract())
        {
            TryPossesPawn(InstigatorPlayerController);
        }
        break;
    case EInteractionType::Interactable:
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
                LocalMultiplayerSubsystem->PossessPawnForPlayerIndex(PlayerIndex, PawnToPossess, MappingType);
                
                if(Cast<AVehicle>(PawnToPossess))
                {
                    AddOutlineToForwardCamera(PlayerIndex);
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
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "UnpossessPawn");
    OnPlayerQuit.Broadcast();
    
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
                LMS->UnPossessPawnForPlayerIndex(PlayerIndex, CachedPreviousPawn.Get(), MappingType);
            }
        }
    }

    TeleportBackAndDetachPlayer(CachedPlayerController.Get());

    if (CachedCharacter.IsValid())
    {
        // Restore actor rotation and controller rotation snapshot
        CachedCharacter->SetActorRotation(RotationActorOnEnter);
        if (CachedPlayerController.IsValid())
        {
            CachedPlayerController->SetControlRotation(RotationControllerOnEnter);
        }

        if (PlayerStillInsideCheck(CachedCharacter.Get()))
        {
            TryDetectPlayer(CachedPlayerController.Get(), CachedCharacter.Get());
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

    if (UWorld* World = GetWorld())
    {
        if (auto* GM = Cast<ACrazyFoodTruckGameMode>(UGameplayStatics::GetGameMode(World)))
        {
            GM->ApplyGlobalViewTo(CachedPlayerController.Get());
        }
    }

    CurrentInteractorPlayerController = nullptr;
    CachedPlayerController = nullptr;
    CachedCharacter = nullptr;
    CachedPreviousPawn = nullptr;
    EnteringCharacter = nullptr;
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
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Invalid AttachPoint!"));
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
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Invalid ReleasePoint!"));
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

    if (ACharacter* Char2 = Cast<ACharacter>(Pawn))
    {
        if (UCharacterMovementComponent* MoveComp2 = Char2->GetCharacterMovement())
        {
            MoveComp2->StopMovementImmediately();
            MoveComp2->Velocity = FVector::ZeroVector;
        }
    }

    if (ACharacter* Char = Cast<ACharacter>(Pawn))
    {
        if (UCharacterMovementComponent* Move2 = Char->GetCharacterMovement())
        {
            Move2->SetComponentTickEnabled(true);
            Move2->Activate();
            Move2->SetMovementMode(MOVE_Walking);
        }
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
       // OnInteractionEnded.Broadcast(LeavingPlayerController);
    }
}
