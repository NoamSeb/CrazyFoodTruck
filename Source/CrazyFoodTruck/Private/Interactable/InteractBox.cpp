// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable/InteractBox.h"

#include "Damage/RepairProgressBillboard.h"
#include "AttachPoint.h"
#include "Characters/CrazyFoodTruckCharacter.h"
#include "GameMode/CrazyFoodTruckGameMode.h"
#include "LocalMultiplayerSubsystem.h"

#include "Components/BoxComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
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

}

void AInteractBox::BeginPlay()
{
	Super::BeginPlay();
	// FindSceneComponent();

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

//void AInteractBox::FindSceneComponent()
//{
//
//}

void AInteractBox::UpdateVisibilityInput(bool bIsVisible)
{
	bShowInputHint = bIsVisible;
}

void AInteractBox::SetFunctional(bool bNewFunctional)
{
	bIsFunctional = bNewFunctional;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 1.f,
			bIsFunctional ? FColor::Green : FColor::Red,
			bIsFunctional ? TEXT("[InteractBox] Enabled") : TEXT("[InteractBox] Disabled (Damaged)")
		);
	}
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

void AInteractBox::OnBoxBeginOverlap(UPrimitiveComponent* Comp, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (bPlayerIsControlling)
		return;

	APlayerController* EnteringPlayerController = GetPlayerControllerFromActor(Other);
	AddOverlappingPlayerController(EnteringPlayerController);

	if (ACrazyFoodTruckCharacter* Character = Cast<ACrazyFoodTruckCharacter>(Other))
	{
		TryDetectPlayer(EnteringPlayerController, Character);
	}
}

void AInteractBox::OnBoxEndOverlap(UPrimitiveComponent* Comp, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex)
{
	if (bPlayerIsControlling)
		return;

	APlayerController* LeavingPlayerController = GetPlayerControllerFromActor(Other);
	RemoveOverlappingPlayerController(LeavingPlayerController);

	if (ACrazyFoodTruckCharacter* Character = Cast<ACrazyFoodTruckCharacter>(Other))
	{
		if (Character->GetFocusedInteractable().GetInterface() == static_cast<IInteractable*>(this))
		{
			Character->SetFocusedInteractable(TScriptInterface<IInteractable>(nullptr));
		}

		OnCollisionExit.Broadcast();
		TryReleaseLockFromActor(Other);

		CachedCharacter = nullptr;
		CachedPlayerController = nullptr;
		CachedPreviousPawn = nullptr;

		if (ACrazyFoodTruckCharacter* Still = DetectPlayerInside())
		{
			APlayerController* NewPlayerController = GetPlayerControllerFromActor(Still);
			TryDetectPlayer(NewPlayerController, Still);
		}
	}
}

void AInteractBox::TryDetectPlayer(APlayerController* PlayerController, ACrazyFoodTruckCharacter* Character)
{
	if (!Character)
		return;

	const bool bLockedByAnother = CurrentInteractorPlayerController.IsValid() && PlayerController && (CurrentInteractorPlayerController.Get() != PlayerController);
	const bool bAnotherInside = IsAnotherPlayerAlreadyInside(PlayerController);

	if (!bLockedByAnother && !bAnotherInside)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Player detected in interact box."));
		Character->SetFocusedInteractable(TScriptInterface<IInteractable>(this));

		if (!CurrentInteractorPlayerController.IsValid())
		{
			OnCollisionEnter.Broadcast();

			if (GEngine)
			{
				const FColor PlayerColor = GetPlayerColorFromPlayerController(PlayerController);
				const int32 PlayerIndex = GetPlayerIndexFromPlayerController(PlayerController);
				const FString PlayerLabel = FString::Printf(TEXT("[P%d] "), PlayerIndex);
				GEngine->AddOnScreenDebugMessage(-1, 2.f, PlayerColor, PlayerLabel + TEXT("Press A to interact."));
			}
		}
	}
}

void AInteractBox::TryReleaseLockFromActor(AActor* LeavingActor)
{
	APlayerController* LeavingPlayerController = GetPlayerControllerFromActor(LeavingActor);
	if (!LeavingPlayerController)
	{
		return;
	}

	if (CurrentInteractorPlayerController.IsValid() && CurrentInteractorPlayerController.Get() == LeavingPlayerController)
	{
		CurrentInteractorPlayerController = nullptr;
		OnInteractionEnded.Broadcast(LeavingPlayerController);

		if (GEngine)
		{
			const FColor PlayerColor = GetPlayerColorFromPlayerController(LeavingPlayerController);
			const int32 PlayerIndex = GetPlayerIndexFromPlayerController(LeavingPlayerController);
			const FString PlayerLabel = FString::Printf(TEXT("[P%d] "), PlayerIndex);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, PlayerColor, PlayerLabel + TEXT("Interaction released."));
		}
	}
}

void AInteractBox::Interact(APlayerController* InstigatorPlayerController)
{
	if (bPlayerIsControlling) return;
	if (!InstigatorPlayerController) return;

	if (bBreakable && RepairProgressBillboard && RepairProgressBillboard->IsDamaged())
	{
		RepairProgressBillboard->HandleRepairInput();
		return;
	}

	if (!bIsFunctional)
	{
		return;
	}

	const FColor PlayerColor = GetPlayerColorFromPlayerController(InstigatorPlayerController);
	const int32 PlayerIndex = GetPlayerIndexFromPlayerController(InstigatorPlayerController);
	const FString PlayerLabel = FString::Printf(TEXT("[P%d] "), PlayerIndex);

	if (CurrentInteractorPlayerController.IsValid() && CurrentInteractorPlayerController.Get() != InstigatorPlayerController)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, PlayerColor, PlayerLabel + TEXT("Already in use by another player."));
		}

		return;
	}

	if (!CurrentInteractorPlayerController.IsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, PlayerColor, PlayerLabel + TEXT("NOT VALID"));

		CurrentInteractorPlayerController = InstigatorPlayerController;
		OnInteractionStarted.Broadcast(InstigatorPlayerController);

		if (PawnToPossess)
		{
			PossessPawn(InstigatorPlayerController);
		}
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, PlayerColor, PlayerLabel + TEXT("Successful interaction!"));
	}
}

void AInteractBox::PossessPawn(APlayerController* PlayerController)
{
	if (!PlayerController)
		return;

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
	bPlayerIsControlling = false;

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
}

APlayerController* AInteractBox::GetPlayerControllerFromActor(AActor* Actor) const
{
	if (!Actor)
	{
		return nullptr;
	}

	if (APawn* Pawn = Cast<APawn>(Actor))
	{
		return Cast<APlayerController>(Pawn->GetController());
	}

	return Cast<APlayerController>(Actor);
}

int32 AInteractBox::GetPlayerIndexFromPlayerController(APlayerController* PlayerController) const
{
	if (!PlayerController)
	{
		return -1;
	}

	if (const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
	{
		return LocalPlayer->GetControllerId();
	}

	return -1;
}

FColor AInteractBox::GetPlayerColorFromPlayerController(APlayerController* PlayerController) const
{
	if (!PlayerController)
	{
		return FColor::White;
	}

	if (APawn* Pawn = PlayerController->GetPawn())
	{
		if (const ACrazyFoodTruckCharacter* Character = Cast<ACrazyFoodTruckCharacter>(Pawn))
		{
			return Character->GetPlayerColor().ToFColor(true);
		}
	}

	switch (GetPlayerIndexFromPlayerController(PlayerController))
	{
	case 0:
		return FColor(0, 115, 255);
	case 1:
		return FColor(26, 204, 51);
	case 2:
		return FColor(255, 26, 26);
	case 3:
		return FColor(255, 230, 26);
	default:
		return FColor::White;
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

ACrazyFoodTruckCharacter* AInteractBox::DetectPlayerInside()
{
	TArray<AActor*> OverlappingActors;
	Box->GetOverlappingActors(OverlappingActors, APawn::StaticClass());

	for (AActor* Actor : OverlappingActors)
	{
		if (Cast<ACrazyFoodTruckCharacter>(Actor))
		{
			// FOUND
			return Cast<ACrazyFoodTruckCharacter>(Actor);
		}
	}
	
	return nullptr;
}

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
	if (!Pawn)
		return;

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

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Player teleported back, detached and movement/physics blocked for testing."));
}

void AInteractBox::OnRepairCompleted()
{
	SetFunctional(true);
	OnRestored.Broadcast();
}

#if WITH_EDITOR
void AInteractBox::SpawnAttachPointInEditor()
{
	if (UWorld* World = GetWorld(); World && World->IsEditorWorld())
	{
		Modify();
		FActorSpawnParameters P;
		P.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		P.OverrideLevel = GetLevel();
		P.ObjectFlags |= RF_Transactional;

		if (!AttachPoint)
		{
			AttachPoint = World->SpawnActor<AAttachPoint>(AAttachPoint::StaticClass(), GetActorTransform(), P);
			
			if (AttachPoint)
			{
				AttachPoint->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
				AttachPoint->SetActorLabel(TEXT("AttachPoint"));
				AttachPoint->Modify(); AttachPoint->MarkPackageDirty();
			}
			
			MarkPackageDirty();
			return;
		}

		if (!ReleasePoint)
		{
			ReleasePoint = World->SpawnActor<AAttachPoint>(AAttachPoint::StaticClass(), GetActorTransform(), P);
			
			if (ReleasePoint)
			{
				ReleasePoint->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
				ReleasePoint->SetActorLabel(TEXT("ReleasePoint"));
				ReleasePoint->Modify(); ReleasePoint->MarkPackageDirty();
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
