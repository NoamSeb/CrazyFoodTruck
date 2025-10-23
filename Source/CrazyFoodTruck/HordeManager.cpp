// Fill out your copyright notice in the Description page of Project Settings.

#include "HordeManager.h"
#include "Components/BoxComponent.h"


AHordeManager::AHordeManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHordeManager::AddSpawnArea()
{
	AAreaZombieSpawn* NewArea = GetWorld()->SpawnActor<AAreaZombieSpawn>(AreaZombieSpawn);
	NewArea->InitArea(ListSpawnArea.Num(),this);
	
	ListSpawnArea.Add(FZoneSpawn(NewArea));
}

void AHordeManager::ClearSpawnArea()
{
	for (FZoneSpawn Element : ListSpawnArea)
	{
		Element.AreaZombieSpawn->Destroy();
	}
	ListSpawnArea.Empty();
}

void AHordeManager::SpawnHordeZombie(int32 nombreZombies, EPositionSpawn differentePos)
{
	if (ListSpawnArea.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "Aucune Zone de Spawn definis");
		return;
	}
	
	nbrVague++;

	for (FZoneSpawn Element : ListSpawnArea)
	{
		if (Element.PositionSpawn == differentePos)
		{
			TObjectPtr<UBoxComponent> SpawnCurrentArea = Element.AreaZombieSpawn->NewBoxAreaSpawn;
			FVector BoxExtent = SpawnCurrentArea->GetScaledBoxExtent();
			
			for (int i = 0; i < nombreZombies; i++)
			{
				FVector SpawnLocation = SpawnCurrentArea->GetComponentLocation() + FVector(
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
				NewZombie->vitesseZombie = vitesseFinalZombie;
				//NewZombie->SetFollower(CurrentObjArea->ActorFollower);
		
				NewZombie->MainActorToFollower = MainActorToFollow;
				switch (differentePos)
				{
					case EPositionSpawn::Right:
						GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, "Droite");
						NewZombie->FirstActorToFollower = RightActorToFollow;
					break;
					case EPositionSpawn::Left:
						GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, "Gauche");
						NewZombie->FirstActorToFollower = LeftActorToFollow;
					break;
					case EPositionSpawn::Forward:
						GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, "Devant");
						NewZombie->FirstActorToFollower = ForwardActorToFollow;
					break;
					case EPositionSpawn::Backward:
						GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, "Derriere");
						NewZombie->FirstActorToFollower = RightActorToFollow;
					break;
				}
				
				NewZombie->CallRound();
			}
		}
	}
}

void AHordeManager::InitHordeZombies()
{
	//ajouter à la vitesse du camion
	vitesseFinalZombie = DifferenceBetweenFoodTruck * KilometersToMetersConvertingValue;
	//TArray<TArray<UStaticMeshComponent*>> Components;

	//c est moche faut pas voir ça
#pragma region Moche a modif
	TArray<UStaticMeshComponent*> Components;
	MainActorToFollow->GetComponents<UStaticMeshComponent>(Components);
	for (auto Component : Components)
	{
		Component->SetCanEverAffectNavigation(false);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, Component->GetName());
	}
	RightActorToFollow->GetComponents<UStaticMeshComponent>(Components);
	for (auto Component : Components)
	{
		Component->SetCanEverAffectNavigation(false);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, Component->GetName());
	}
	LeftActorToFollow->GetComponents<UStaticMeshComponent>(Components);
	for (auto Component : Components)
	{
		Component->SetCanEverAffectNavigation(false);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, Component->GetName());
	}
	ForwardActorToFollow->GetComponents<UStaticMeshComponent>(Components);
	for (auto Component : Components)
	{
		Component->SetCanEverAffectNavigation(false);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, Component->GetName());
	}
	BackwardActorToFollow->GetComponents<UStaticMeshComponent>(Components);
	
	for (auto Component : Components)
	{
		Component->SetCanEverAffectNavigation(false);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, Component->GetName());
	}
#pragma endregion
	//SpawnHordeZombie();
}

void AHordeManager::BeginPlay()
{
	Super::BeginPlay();
}


void AHordeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


