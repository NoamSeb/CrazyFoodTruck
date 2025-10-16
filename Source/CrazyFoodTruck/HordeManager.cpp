// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeManager.h"
#include "GameFramework/Character.h"


// Sets default values
AHordeManager::AHordeManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpawnZone = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnZone"));
	RootComponent = SpawnZone;

	// Couleur / visibilité dans l'éditeur
	SpawnZone->SetBoxExtent(FVector(500.0f, 500.0f, 200.0f)); // taille par défaut
	SpawnZone->SetHiddenInGame(true);
	SpawnZone->SetVisibility(true);
	SpawnZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AHordeManager::SpawnHordeZombie()
{
	FVector BoxExtent = SpawnZone->GetScaledBoxExtent();

	srand(time(0));
	int8 numberZombies = rand() % 10 + 5;

	GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::Red, FString::Printf(TEXT("Number Zombies: %d"), numberZombies));
	
	for (int i = 0; i < numberZombies; i++)
	{
		//FVector SpawnLocation(FMath::RandRange(-BoxExtent.X, BoxExtent.X), FMath::RandRange(-BoxExtent.Y, BoxExtent.Y), 90.0f);
		FVector SpawnLocation = SpawnZone->GetComponentLocation() + FVector(
	FMath::RandRange(-BoxExtent.X, BoxExtent.X),
	FMath::RandRange(-BoxExtent.Y, BoxExtent.Y),
	90.0f
);
		FRotator SpawnRotation(0.0f, 0.0f, 0.0f);
		FVector SpawnScale(1.0f, 1.0f, 1.0f);
	
		
		FTransform NewTransform;
		NewTransform.SetLocation(SpawnLocation);
		NewTransform.SetScale3D(SpawnScale);

		ACharacter* NewZombie = GetWorld()->SpawnActor<ACharacter>(PawnZombie, NewTransform);
		ListHordeZombie.Add(NewZombie);

		NewZombie->SpawnDefaultController();

		//à voir pour remplacer plus tard le ce round d'ici pas un custom event init
		UFunction* EventRoundZombie = NewZombie->FindFunction(FName("CE_Round"));
		if (EventRoundZombie)
		{
			NewZombie->ProcessEvent(EventRoundZombie, nullptr);
		}
	}
}

// Called when the game starts or when spawned
void AHordeManager::BeginPlay()
{
	Super::BeginPlay();
	SpawnHordeZombie();
}

// Called every frame
void AHordeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

