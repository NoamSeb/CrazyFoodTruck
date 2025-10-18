// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/InteractBox.h"

#include "Players/LocalMultiplayerCharacter.h"

#include "Components/BoxComponent.h"
#include "Engine/Engine.h"

// Sets default values
AInteractBox::AInteractBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
	if (ALocalMultiplayerCharacter* Character = Cast<ALocalMultiplayerCharacter>(OtherActor))
	{
		Character->SetFocusedInteractable(TScriptInterface<IInteractable>(this));
	}
}

void AInteractBox::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ALocalMultiplayerCharacter* Character = Cast<ALocalMultiplayerCharacter>(OtherActor))
	{
		if (Character->GetFocusedInteractable().GetInterface() == static_cast<IInteractable*>(this))
		{
			Character->SetFocusedInteractable(TScriptInterface<IInteractable>(nullptr));
		}
	}
}

void AInteractBox::Interact_Implementation(ALocalMultiplayerCharacter* InstigatorCharacter)
{
	if (GEngine)
	{
		const FString Who = InstigatorCharacter ? InstigatorCharacter->GetName() : TEXT("Unknown");
		const FString FullMessage = FString::Printf(TEXT("[%s] %s"), *Who, *Message);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FullMessage);
	}
}

