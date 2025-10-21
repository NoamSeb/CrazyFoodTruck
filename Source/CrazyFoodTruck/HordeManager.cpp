// Fill out your copyright notice in the Description page of Project Settings.


#include "HordeManager.h"
#include "NavigationSystem.h"


AHordeManager::AHordeManager()
{
	PrimaryActorTick.bCanEverTick = false;

	SpawnZone = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnZone"));

	// Couleur / visibilité dans l'éditeur
	SpawnZone->SetBoxExtent(FVector(500.0f, 500.0f, 200.0f)); // taille par défaut
	SpawnZone->SetHiddenInGame(true);
	SpawnZone->SetVisibility(true);
	SpawnZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AHordeManager::SetAreaSpawnZombie()
{
	SpawnZone = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnZone"));

	// Couleur / visibilité dans l'éditeur
	SpawnZone->SetBoxExtent(FVector(500.0f, 500.0f, 200.0f)); // taille par défaut
	SpawnZone->SetHiddenInGame(true);
	SpawnZone->SetVisibility(true);
	SpawnZone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AHordeManager::SpawnHordeZombie(int nbrMin, int nbrMax)
{
	//if (!ListHordeZombie.IsEmpty())
	//{
	//	for (AZombieIA* Zombie : ListHordeZombie)
	//	{
	//		Zombie->Destroy();
	//	}
	//	ListHordeZombie.Empty();
	//}
	
	if (nbrMinZombies > nbrMaxZombies)
	{
		int8 intTempo = nbrMinZombies;
		nbrMinZombies = nbrMaxZombies;
		nbrMaxZombies = intTempo;

		GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Magenta,"min et max changer");
	}
	
	FVector BoxExtent = SpawnZone->GetScaledBoxExtent();

	//aléatoir min et max inclus
	int8 numberZombies = FMath::RandRange(nbrMin, nbrMax);
	
	
	
	for (int i = 0; i < numberZombies; i++)
	{
		//FVector SpawnLocation(FMath::RandRange(-BoxExtent.X, BoxExtent.X), FMath::RandRange(-BoxExtent.Y, BoxExtent.Y), 90.0f);
		//pos aléatoire dans les limites de la box (l'aide visuelle)
		FVector SpawnLocation = SpawnZone->GetComponentLocation() + FVector(
	FMath::RandRange(-BoxExtent.X, BoxExtent.X),FMath::RandRange(-BoxExtent.Y, BoxExtent.Y),90.0f);
		FVector SpawnScale(1.0f, 1.0f, 1.0f);
		
		FTransform NewTransform;
		NewTransform.SetLocation(SpawnLocation);
		NewTransform.SetScale3D(SpawnScale);

		//définir comment les zombies spawn et leurs collision quand ils spawn
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		//faire spawn un character de la class PawnZombie remplis avant avec son transfom
		AZombieIA* NewZombie = GetWorld()->SpawnActor<AZombieIA>(PawnZombie, NewTransform, SpawnParams);
		ListHordeZombie.Add(NewZombie);

		//lui ajouté manuellement un controller sinon il ne bougera pas 
		NewZombie->SpawnDefaultController();

		NewZombie->SetFollower(CharacterFollower);
		NewZombie->CallRound();
	}
}

void AHordeManager::InitHordeZombies()
{
	TArray<UStaticMeshComponent*> Components;
	CharacterFollower->GetComponents<UStaticMeshComponent>(Components);
	for (auto Component : Components)
	{
		Component->SetCanEverAffectNavigation(false);
	}
	SpawnHordeZombie(nbrMinZombies, nbrMaxZombies);
}

void AHordeManager::BeginPlay()
{
	Super::BeginPlay();
}


void AHordeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

