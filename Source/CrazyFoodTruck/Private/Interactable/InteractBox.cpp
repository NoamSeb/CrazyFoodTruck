// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable/InteractBox.h"

#include "LocalMultiplayerSubsystem.h"

#include "Components/BoxComponent.h"

// Sets default values
AInteractBox::AInteractBox()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);

	Box->InitBoxExtent(FVector(100.f, 100.f, 100.f));
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
//	FindSceneComponent();
}

void AInteractBox::FindSceneComponent()
{
	// FIND SCENE COMPONENT
	// auto sceneComponents = K2_GetComponentsByClass(USceneComponent::StaticClass());
	// for (auto SceneComponent : sceneComponents)
	// {
	// 	if (SceneComponent->GetName() == "AttachPoint")
	// 	{
	// 		AttachPoint = Cast<USceneComponent>(SceneComponent);
	// 	}
	// 	if (SceneComponent->GetName() == "ReleasePoint")
	// 	{
	// 		ReleasePoint = Cast<USceneComponent>(SceneComponent);
	// 	}
	// }
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


#if WITH_EDITOR
void AInteractBox::SpawnAttachPointInEditor()
{
    UWorld* World = GetWorld();
    if (!World || !World->IsEditorWorld())
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnAttachPointInEditor: not in editor world."));return;
    }
    Modify();
    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    Params.OverrideLevel = GetLevel();
    Params.ObjectFlags |= RF_Transactional;
    if (!AttachPoint)
    {
        AAttachPoint* NewPoint = World->SpawnActor<AAttachPoint>(AAttachPoint::StaticClass(), GetActorTransform(), Params);
        if (NewPoint)
        {
            NewPoint->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
            NewPoint->SetActorLabel("AttachPoint");
            NewPoint->Modify();
            AttachPoint = NewPoint;
            NewPoint->MarkPackageDirty();
            MarkPackageDirty();
        }

        return;
    }

    // Spawn le ReleasePoint s'il n'existe pas
    if (!ReleasePoint)
    {
        FTransform SpawnTransform = GetActorTransform();

        AAttachPoint* NewPoint = World->SpawnActor<AAttachPoint>(AAttachPoint::StaticClass(), SpawnTransform, Params);
        if (NewPoint)
        {
            NewPoint->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
            NewPoint->SetActorLabel("ReleasePoint");
            NewPoint->Modify();
            ReleasePoint = NewPoint;
            NewPoint->MarkPackageDirty();
            MarkPackageDirty();
        }

        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("SpawnAttachPointInEditor: AttachPoint and ReleasePoint already exist."));
}

void AInteractBox::ClearAttachPoint()
{
	UWorld* World = GetWorld();
	if (!World || !World->IsEditorWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnAttachPointInEditor: not in editor world."));return;
	}
	Modify();
	if (AttachPoint)
	{
		AttachPoint->Destroy();
	}
	if (ReleasePoint)
	{
		ReleasePoint->Destroy();
	}
	AttachPoint = nullptr;
	ReleasePoint = nullptr;
	MarkPackageDirty();
}
#endif // WITH_EDITOR



void AInteractBox::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (_IsPlayerControlling){return;} // IF PLAYER CONTROL WE DONT DETECT ENTER
	APlayerController* EnteringPlayerController = GetPlayerControllerFromActor(OtherActor);
	AddOverlappingPlayerController(EnteringPlayerController);

	ACrazyFoodTruckCharacter* Character = Cast<ACrazyFoodTruckCharacter>(OtherActor);
	TryDetectPlayer(EnteringPlayerController, Character);
}

void AInteractBox::TryDetectPlayer(APlayerController* PlayerController, ACrazyFoodTruckCharacter* Character)
{
	if (Character)
	{
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
		}else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Player detected in interact box but locked by another or another inside."));
		}
	}
}

void AInteractBox::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (_IsPlayerControlling){return;} // IF PLAYER CONTROL WE DONT DETECT EXIT
	APlayerController* LeavingPlayerController = GetPlayerControllerFromActor(OtherActor);
	RemoveOverlappingPlayerController(LeavingPlayerController);

	if (ACrazyFoodTruckCharacter* Character = Cast<ACrazyFoodTruckCharacter>(OtherActor)) // PLAYER THAT WAS FIRST QUIT
	{
		if (Character->GetFocusedInteractable().GetInterface() == static_cast<IInteractable*>(this)) // CLEAR IF INTERACTABLE WAS THIS
		{
			Character->SetFocusedInteractable(TScriptInterface<IInteractable>(nullptr));
		}

		OnCollisionExit.Broadcast();
		TryReleaseLockFromActor(OtherActor);

		CachedCharacter = nullptr;
		CachedPlayerController = nullptr;
		CachedPreviousPawn = nullptr;

		auto playerLeft = DetectPlayerInside();
		if (playerLeft)
		{
			APlayerController* NewPlayerController = GetPlayerControllerFromActor(playerLeft);
			TryDetectPlayer(NewPlayerController, Cast<ACrazyFoodTruckCharacter>(playerLeft));
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
	if (_IsPlayerControlling){return;}
	if (!InstigatorPlayerController)
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
	_IsPlayerControlling = true;
	CachedPlayerController = PlayerController;
	CachedCharacter = PlayerController ? Cast<ACrazyFoodTruckCharacter>(PlayerController->GetPawn()) : nullptr;
	CachedPreviousPawn = PlayerController ? PlayerController->GetPawn() : nullptr;

	int PlayerIndex = GetPlayerIndexFromPlayerController(PlayerController);
	if (PlayerIndex == -1)
	{
		return;
	}
	
	if (PawnToPossess)
	{
		TeleportAndAttachPlayer(PlayerController);

		if (UGameInstance* GameInstance = GetGameInstance()) // SET POSSESS AND INPUT MAPPING
		{
			if (ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>())
			{
				LocalMultiplayerSubsystem->PossessPawnForPlayerIndex(PlayerIndex, PawnToPossess, InputMapping);
			}
		}
	}
}

void AInteractBox::UnpossessPawn()
{
	_IsPlayerControlling = false;
	CurrentInteractorPlayerController = nullptr;

	if (!CachedPlayerController)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("InteractBox::UnpossessPawn: CachedPlayerController is null!"));
		return;
	}
	int PlayerIndex = GetPlayerIndexFromPlayerController(CachedPlayerController);
	if (PlayerIndex == -1)
	{
		return;
	}
	
	OnPlayerQuit.Broadcast();

	if (PawnToPossess)
	{
		if (UGameInstance* GameInstance = GetGameInstance())
		{
			if (ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>())
			{
				LocalMultiplayerSubsystem->UnPossessPawnForPlayerIndex(PlayerIndex, CachedPreviousPawn, InputMapping);
			}
		}
	}

	TeleportBackAndDetachPlayer(CachedPlayerController);


	// CHECK IF PLAYER STILL INSIDE

	if (CachedCharacter)
	{
		if (PlayerStillInsideCheck(CachedCharacter))
		{
			// PLAYER STILL INSIDE
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Player STILL INSIDE !."));

			TryDetectPlayer(CachedPlayerController, CachedCharacter);
			return;
		}
	}
	else
	{
		CachedCharacter->SetFocusedInteractable(nullptr);
	}
	
	
	CachedPlayerController = nullptr;
	CachedCharacter = nullptr;
	CachedPreviousPawn = nullptr;
	
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

ACrazyFoodTruckCharacter * AInteractBox::DetectPlayerInside()
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

void AInteractBox::UpdateVisibilityInput(bool bIsVisible)
{
	_IsShowingInput = bIsVisible;
}

void AInteractBox::TeleportAndAttachPlayer(APlayerController* PlayerController)
{
	if (!AttachPoint)
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
	if (!ReleasePoint)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Invalid ReleasePoint!"));
		return;
	}

	if (APawn* Pawn = PlayerController->GetPawn())
	{
		Pawn->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Pawn->SetActorLocation(ReleasePoint->GetActorLocation());
		//Pawn->SetActorRotation(ReleasePoint->GetComponentRotation());
	}
}