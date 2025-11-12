// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmunitionSpawner.h"



// Sets default values
AAmmunitionSpawner::AAmmunitionSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAmmunitionSpawner::SetSpawnState(bool bCanSpawnAmmo)
{
	bCanSpawn = bCanSpawnAmmo;
}

void AAmmunitionSpawner::SetCanTakeAmmo(bool bCanTake)
{
	canTake = bCanTake;
}

void AAmmunitionSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnPoint = FindComponentByClass<USceneComponent>();
	if (!SpawnPoint)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AmmunitionSpawner: No SpawnPoint found!"));
	}
	if (!AmmoPrefab)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AmmunitionSpawner: No AmmoPrefab assigned!"));
	}
	SpawnAmmo();
}

void AAmmunitionSpawner::SpawnAmmo()
{
	if (CurrentRespawnTimer > 0.f){return;}
	if (!bCanSpawn){return;}
	if (AmmoOnHolder){return;}
	if (!AmmoPrefab){return;}
	CurrentAmmoTakeTimer = 1.5f;
	bCanSpawn = false;
	AmmoOnHolder = true;
	
	FVector SpawnLocation = SpawnPoint->GetComponentLocation();
	FRotator SpawnRotation = SpawnPoint->GetComponentRotation();

	auto AmmoActor = GetWorld()->SpawnActor<AActor>(AmmoPrefab, SpawnLocation, SpawnRotation);
	AmmoBoxSpawned = Cast<AAmmoBox>(AmmoActor);
	AmmoBoxSpawned->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	if (AmmoBoxSpawned)
	{
		AmmoBoxSpawned->Initialize();
	}
	
	OnAmmoSpawned.Broadcast(AmmoBoxSpawned);
}

AAmmoBox* AAmmunitionSpawner::TakeAmmo()
{
	AmmoOnHolder = false;
	CurrentRespawnTimer = RespawnTime;
	return AmmoBoxSpawned;
}

void AAmmunitionSpawner::Interact(APlayerController* InstigatorPlayerController, ACrazyFoodTruckCharacter* CrazyCharacter)
{
	if (CurrentAmmoTakeTimer > 0.f){return;}
	if (!canTake){return;}
	if (AmmoOnHolder)
	{
		CrazyCharacter->TakeAmmoBox(AmmoBoxSpawned);
		CrazyCharacter->SetInteractState(false);
		TakeAmmo();

		AmmoBoxSpawned->SetActorLocation(CrazyCharacter->GetActorLocation());
		AmmoBoxSpawned->AttachToActor(CrazyCharacter, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		AmmoBoxSpawned = nullptr;
	}
}


// Called every frame
void AAmmunitionSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(	CurrentAmmoTakeTimer > 0.f)
	{
		CurrentAmmoTakeTimer -= DeltaTime;
		if (CurrentAmmoTakeTimer <= 0.f)
		{
			CurrentAmmoTakeTimer = 0.f;
		}
	}
	if (!bCanSpawn){return;}
	if (CurrentRespawnTimer > 0.f)
	{
		CurrentRespawnTimer -= DeltaTime;
		if (CurrentRespawnTimer <= 0.f)
		{
			SpawnAmmo();
		}
	}
}

