// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable/InteractBox.h"

#include "Characters/CrazyFoodTruckCharacter.h"

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

void AInteractBox::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerController* EnteringPlayerController = GetPlayerControllerFromActor(OtherActor);
	AddOverlappingPlayerController(EnteringPlayerController);

	if (ACrazyFoodTruckCharacter* Character = Cast<ACrazyFoodTruckCharacter>(OtherActor))
	{
		const bool bLockedByAnother = CurrentInteractorPlayerController.IsValid() && EnteringPlayerController && (CurrentInteractorPlayerController.Get() != EnteringPlayerController);
		const bool bAnotherInside = IsAnotherPlayerAlreadyInside(EnteringPlayerController);

		if (!bLockedByAnother && !bAnotherInside)
		{
			Character->SetFocusedInteractable(TScriptInterface<IInteractable>(this));

			if (!CurrentInteractorPlayerController.IsValid())
			{
				OnCollisionEnter.Broadcast();

				if (GEngine)
				{
					const FColor PlayerColor = GetPlayerColorFromPlayerController(EnteringPlayerController);
					const int32 PlayerIndex = GetPlayerIndexFromPlayerController(EnteringPlayerController);
					const FString PlayerLabel = FString::Printf(TEXT("[P%d] "), PlayerIndex);
					GEngine->AddOnScreenDebugMessage(-1, 2.f, PlayerColor, PlayerLabel + TEXT("Press A to interact."));
				}
			}
		}
	}
}

void AInteractBox::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerController* LeavingPlayerController = GetPlayerControllerFromActor(OtherActor);
	RemoveOverlappingPlayerController(LeavingPlayerController);

	if (ACrazyFoodTruckCharacter* Character = Cast<ACrazyFoodTruckCharacter>(OtherActor))
	{
		if (Character->GetFocusedInteractable().GetInterface() == static_cast<IInteractable*>(this))
		{
			Character->SetFocusedInteractable(TScriptInterface<IInteractable>(nullptr));
		}

		OnCollisionExit.Broadcast();
		TryReleaseLockFromActor(OtherActor);
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
	CachedPlayerController = PlayerController;
	CachedPreviousPawn = PlayerController ? PlayerController->GetPawn() : nullptr;

	int PlayerIndex = GetPlayerIndexFromPlayerController(PlayerController);
	if (PlayerIndex == -1)
	{
		return;
	}
	
	if (PawnToPossess)
	{
		if (UGameInstance* GameInstance = GetGameInstance())
		{
			if (ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>())
			{
				LocalMultiplayerSubsystem->PossessPawnForPlayerIndex(PlayerIndex, PawnToPossess, ELocalMultiplayerInputMappingType::Turret);
			}
		}
	}
}

void AInteractBox::UnpossessPawn()
{
	int PlayerIndex = GetPlayerIndexFromPlayerController(CachedPlayerController);
	if (PlayerIndex == -1)
	{
		return;
	}

	if (PawnToPossess)
	{
		if (UGameInstance* GameInstance = GetGameInstance())
		{
			if (ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>())
			{
				LocalMultiplayerSubsystem->UnPossessPawnForPlayerIndex(PlayerIndex, CachedPreviousPawn, ELocalMultiplayerInputMappingType::Turret);
			}
		}
	}

	CachedPlayerController = nullptr;
}
