// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoDeposit.h"

#include "Characters/CrazyFoodTruckCharacter.h"


AAmmoDeposit::AAmmoDeposit()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAmmoDeposit::BeginPlay()
{
	Super::BeginPlay();
	DepositPoint = FindComponentByClass<USceneComponent>();
	if (!DepositPoint)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AmmoDeposit: No DepositPoint found!"));
	}
	if (!LinkedAmmoSpawner)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AmmoDeposit: No LinkedAmmoSpawner assigned!"));
	}
	if (!LinkedTurretController)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AmmoDeposit: No LinkedTurretController assigned!"));
	}

	if (LinkedAmmoSpawner)
	{
		LinkedAmmoSpawner->SetSpawnState(true);
	}
	if (LinkedTurretController)
	{
		LinkedTurretController->OnAmmoEmpty.AddDynamic(this,&AAmmoDeposit::TurretAmmoEmpty);
	}
}

void AAmmoDeposit::Interact(APlayerController* InstigatorPlayerController, ACrazyFoodTruckCharacter* CrazyCharacter)
{
	if (!CrazyCharacter){return;}
	if (CrazyCharacter->HasAmmo())
	{
		Reload();
		CrazyCharacter->SetAmmoState(false);
		ReloadedAmmoBox = CrazyCharacter->DepositAmmoBox();
		
		if (ReloadedAmmoBox)
		{
			ReloadedAmmoBox->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			ReloadedAmmoBox->SetActorRotation(DepositPoint->GetComponentRotation());
		}
		
		CrazyCharacter->SetAmmoState(false);
		CrazyCharacter->SetInteractState(true);
		LinkedAmmoSpawner->SetSpawnState(true);
		OnAmmoDeposited.Broadcast(ReloadedAmmoBox);
	}
}

void AAmmoDeposit::TurretAmmoEmpty()
{
	if (ReloadedAmmoBox)
	{
		LinkedAmmoSpawner->SetCanTakeAmmo(true);
		ReloadedAmmoBox->Destroy();
		ReloadedAmmoBox = nullptr;
	}
}

void AAmmoDeposit::Reload()
{
	LinkedTurretController->Reload();
}

// Called every frame
void AAmmoDeposit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

