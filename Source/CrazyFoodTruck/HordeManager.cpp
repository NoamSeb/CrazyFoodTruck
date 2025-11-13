// Fill out your copyright notice in the Description page of Project Settings.

#include "HordeManager.h"

#include <string>

#include "Components/BoxComponent.h"
#include "Vehicle/Vehicle.h"

AHordeManager::AHordeManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHordeManager::AddSpawnArea()
{
	//FActorSpawnParameters SpawnParams;
	//std::string Name = "SpawnArea_" + std::to_string(ListSpawnArea.Num());
	//SpawnParams.Name = Name.c_str();
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "SpawnArea_" + ListSpawnArea.Num());
	
	AAreaZombieSpawn* NewArea = GetWorld()->SpawnActor<AAreaZombieSpawn>(AreaZombieSpawn);
	NewArea->InitArea(ListSpawnArea.Num(),this);

	FZoneSpawn newZoneSpawn;
	newZoneSpawn.PositionSpawn = EPositionSpawn::DownLeft;
	newZoneSpawn.AreaZombieSpawn = NewArea;
	
	ListSpawnArea.Add(newZoneSpawn);

	#if WITH_EDITOR
		if (GEditor && NewArea)
		{
			// Nettoyer la sélection précédente
			GEditor->SelectNone(false, true, false);

			// Sélectionner le nouvel acteur
			GEditor->SelectActor(NewArea, true, true, true);

			// Centrer la vue dessus
			GEditor->MoveViewportCamerasToActor(*NewArea, false);
			
		}
	#endif
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
	GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Red, "TRRRY SPAWN ZOMBIE WAVE");

	if (!bCanSpawnHorde){return;}
	if (ListSpawnArea.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "Aucune Zone de Spawn definis");
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Red, "SPAWN ZOMBIE WAVE");
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
				NewZombie->ZombieSpeed = FinalZombieSpeed;
				//NewZombie->SetFollower(CurrentObjArea->ActorFollower);
		
				NewZombie->MainActorToFollower = MainActorToFollow;
				switch (differentePos)
				{
					case EPositionSpawn::MiddleUp:
						NewZombie->FirstActorToFollower = ForwardActorToFollow;
					break;
					case EPositionSpawn::DownLeft:
						NewZombie->FirstActorToFollower = LeftActorToFollow;
					break;
					case EPositionSpawn::DownRight:
						NewZombie->FirstActorToFollower = RightActorToFollow;
					break;
					case EPositionSpawn::MiddleDown:
						NewZombie->FirstActorToFollower = MainActorToFollow;
					break;
				}

				NewZombie->OnZombieDied.AddDynamic(this, &AHordeManager::HandleZombieDied);

				NewZombie->CallRound();
			}
		}
	}
}

void AHordeManager::InitHordeZombies()
{
	if (!bCanSpawnHorde){return;}
	//ajouter à la vitesse du camion
	FinalZombieSpeed = (FoodTruck->TruckMaxSpeed + DifferenceBetweenFoodTruck) * KilometersToMetersConvertingValue;

	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("Food truck value : %f"), FoodTruck->TruckMaxSpeed));
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("Diff value : %f"), DifferenceBetweenFoodTruck * KilometersToMetersConvertingValue));
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("zombie value : %f"), FinalZombieSpeed));
	//TArray<TArray<UStaticMeshComponent*>> Components;

	//c est moche faut pas voir ça
#pragma region Moche a modif
	TArray<UStaticMeshComponent*> Components;
	MainActorToFollow->GetComponents<UStaticMeshComponent>(Components);
	for (auto Component : Components)
	{
		if (!Component){return;}
		Component->SetCanEverAffectNavigation(false);
	}
	RightActorToFollow->GetComponents<UStaticMeshComponent>(Components);
	for (auto Component : Components)
	{
		if (!Component){return;}
		Component->SetCanEverAffectNavigation(false);
	}
	LeftActorToFollow->GetComponents<UStaticMeshComponent>(Components);
	for (auto Component : Components)
	{
		if (!Component){return;}
		Component->SetCanEverAffectNavigation(false);
	}
	ForwardActorToFollow->GetComponents<UStaticMeshComponent>(Components);
	for (auto Component : Components)
	{
		if (!Component){return;}
		Component->SetCanEverAffectNavigation(false);
	}
	BackwardActorToFollow->GetComponents<UStaticMeshComponent>(Components);
	for (auto Component : Components)
	{
		if (!Component){return;}
		Component->SetCanEverAffectNavigation(false);
	}
#pragma endregion
	//SpawnHordeZombie();
}

void AHordeManager::BeginPlay()
{
	Super::BeginPlay();
	
	if (!RightActorToFollow)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Horde Manager : RightActorToFollow is not assigned !"));
		bCanSpawnHorde = false;
		Destroy();
	}
	if (!LeftActorToFollow)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Horde Manager : LeftActorToFollow is not assigned !"));
		bCanSpawnHorde = false;
		Destroy();

	}
	if (!MainActorToFollow)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Horde Manager : MainActorToFollow is not assigned !"));
		bCanSpawnHorde = false;
		Destroy();

	}
	if (!ForwardActorToFollow)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Horde Manager : ForwardActorToFollow is not assigned !"));
		bCanSpawnHorde = false;
		Destroy();

	}
}

void AHordeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHordeManager::HandleZombieDied(AZombieIA* Zombie, AActor* Killer)
{
	++ZombiesKilledTotal;

	ListHordeZombie.Remove(Zombie);

	OnAnyZombieDied.Broadcast(Zombie, Killer);
}
