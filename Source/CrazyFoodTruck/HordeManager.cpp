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
	AAreaZombieSpawn* NewArea = GetWorld()->SpawnActor<AAreaZombieSpawn>(AreaZombieSpawn);
	NewArea->InitArea(ListSpawnArea.Num(),this);

	FZoneSpawn newZoneSpawn;
	newZoneSpawn.PositionSpawn = ETargetZombiePoint::MiddleDown;
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

void AHordeManager::SpawnHordeZombie(int32 nombreZombies, AAreaZombieSpawn* ZoneSpawn, ETargetZombiePoint targetPoint)
{
	if (!GEngine)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("TRY SPAWN ZOMBIE WAVE"));

	if (!bCanSpawnHorde)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("HordeManager: bCanSpawnHorde = false, abort."));
		return;
	}

	if (!ZoneSpawn)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("HordeManager: ZoneSpawn is NULL !"));
		return;
	}

	if (!ZoneSpawn->NewBoxAreaSpawn)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("HordeManager: ZoneSpawn->NewBoxAreaSpawn is NULL !"));
		return;
	}

	if (ListSpawnArea.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("HordeManager: ListSpawnArea is empty."));
	}

	if (!PawnZombie)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("HordeManager: PawnZombie is NOT set !"));
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SPAWN ZOMBIE WAVE"));
	++nbrVague;

	const FVector BoxExtent = ZoneSpawn->NewBoxAreaSpawn->GetScaledBoxExtent();
	const FVector BoxCenter = ZoneSpawn->NewBoxAreaSpawn->GetComponentLocation();

	UWorld* World = GetWorld();
	if (!World)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("HordeManager: World is NULL !"));
		return;
	}

	for (int32 i = 0; i < nombreZombies; ++i)
	{
		const FVector RandomOffset(
			FMath::RandRange(-BoxExtent.X, BoxExtent.X),
			FMath::RandRange(-BoxExtent.Y, BoxExtent.Y),
			90.0f
		);

		const FVector SpawnLocation = BoxCenter + RandomOffset;
		const FVector SpawnScale(1.0f, 1.0f, 1.0f);

		FTransform NewTransform;
		NewTransform.SetLocation(SpawnLocation);
		NewTransform.SetScale3D(SpawnScale);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AZombieIA* NewZombie = World->SpawnActor<AZombieIA>(PawnZombie, NewTransform, SpawnParams);

		if (!NewZombie)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("HordeManager: Failed to spawn Zombie !"));
			continue;
		}

		ListHordeZombie.Add(NewZombie);

		NewZombie->SpawnDefaultController();
		NewZombie->ZombieSpeed = (FoodTruck->_CurrentTruckMaxSpeed + DifferenceBetweenFoodTruck) * KilometersToMetersConvertingValue;

		NewZombie->MainActorToFollower = MainActorToFollow;

		switch (targetPoint)
		{
		case ETargetZombiePoint::Up:
			NewZombie->FirstActorToFollower = ForwardActorToFollow;
			break;
		case ETargetZombiePoint::Left:
			NewZombie->FirstActorToFollower = LeftActorToFollow;
			break;
		case ETargetZombiePoint::Right:
			NewZombie->FirstActorToFollower = RightActorToFollow;
			break;
		case ETargetZombiePoint::MiddleDown:
		default:
			NewZombie->FirstActorToFollower = MainActorToFollow;
			break;
		}

		NewZombie->OnZombieDied.AddDynamic(this, &AHordeManager::HandleZombieDied);

		NewZombie->CallRound();
	}
}

void AHordeManager::InitHordeZombies()
{
	if (!bCanSpawnHorde){return;}
	//ajouter à la vitesse du camion
	FinalZombieSpeed = (FoodTruck->_CurrentTruckMaxSpeed + DifferenceBetweenFoodTruck) * KilometersToMetersConvertingValue;

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

void AHordeManager::HandleZombieDied(AZombieIA* Zombie, AActor* Killer)
{
	++ZombiesKilledTotal;
	ListHordeZombie.Remove(Zombie);
	OnAnyZombieDied.Broadcast(Zombie, Killer);
}

void AHordeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

