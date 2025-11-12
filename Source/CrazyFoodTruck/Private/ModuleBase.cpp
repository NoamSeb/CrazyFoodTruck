// Fill out your copyright notice in the Description page of Project Settings.


#include "ModuleBase.h"

AModuleBase::AModuleBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AModuleBase::ResetModule()
{
}

void AModuleBase::BeginPlay()
{
	Super::BeginPlay();
}

void AModuleBase::Interact(APlayerController* InstigatorPlayerController, ACrazyFoodTruckCharacter* CrazyCharacter)
{
	ActualCooldown = BaseCooldown;
}

bool AModuleBase::CanInteractWithModule() const
{
	if (ActualCooldown > 0.f)
	{
		return false;
	}
	return true;
}

void AModuleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ActualCooldown > 0.f)
	{
		ActualCooldown -= DeltaTime;
	}
}
