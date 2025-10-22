// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable/InteractBox.h"

#include "Characters/CrazyFoodTruckCharacter.h"

#include "Components/BoxComponent.h"

// Sets default values
AInteractBox::AInteractBox()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
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

// Called when the game starts or when spawned
void AInteractBox::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractBox::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACrazyFoodTruckCharacter* Character = Cast<ACrazyFoodTruckCharacter>(OtherActor))
	{
		Character->SetFocusedInteractable(TScriptInterface<IInteractable>(this));

		if (GEngine)
		{
			const FColor PlayerColor = Character->GetPlayerColor().ToFColor(true);
			const FString PlayerLabel = FString::Printf(TEXT("[P%d] "), Character->GetPlayerIndex());
			GEngine->AddOnScreenDebugMessage(-1, 2.f, PlayerColor, PlayerLabel + TEXT("Press X to interact"));
		}
	}
}

void AInteractBox::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ACrazyFoodTruckCharacter* Character = Cast<ACrazyFoodTruckCharacter>(OtherActor))
	{
		if (Character->GetFocusedInteractable().GetInterface() == static_cast<IInteractable*>(this))
		{
			Character->SetFocusedInteractable(TScriptInterface<IInteractable>(nullptr));
		}

		TryReleaseLock(Character);
	}
}

void AInteractBox::TryReleaseLock(ACrazyFoodTruckCharacter* LeavingCharacter)
{
	if (CurrentInteractor.IsValid() && CurrentInteractor.Get() == LeavingCharacter)
	{
		CurrentInteractor = nullptr;

		if (GEngine)
		{
			const FColor PlayerColor = LeavingCharacter->GetPlayerColor().ToFColor(true);
			const FString PlayerLabel = FString::Printf(TEXT("[P%d] "), LeavingCharacter->GetPlayerIndex());
			GEngine->AddOnScreenDebugMessage(-1, 2.f, PlayerColor, PlayerLabel + TEXT("Interaction released."));
		}
	}
}

void AInteractBox::Interact_Implementation(ACrazyFoodTruckCharacter* InstigatorCharacter)
{
	if (!InstigatorCharacter)
	{
		return;
	}

	const FColor PlayerColor = InstigatorCharacter->GetPlayerColor().ToFColor(true);
	const FString PlayerLabel = FString::Printf(TEXT("[P%d] "), InstigatorCharacter->GetPlayerIndex());

	if (CurrentInteractor.IsValid() && CurrentInteractor.Get() != InstigatorCharacter)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, PlayerColor, PlayerLabel + TEXT("Already in use by another player."));
		}
		
		return;
	}

	if (!CurrentInteractor.IsValid())
	{
		CurrentInteractor = InstigatorCharacter;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, PlayerColor, PlayerLabel + TEXT("Successful interaction!"));
	}
}
