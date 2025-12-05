// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonDoor.h"


// Sets default values
AButtonDoor::AButtonDoor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AButtonDoor::BeginPlay()
{
	Super::BeginPlay();
}

void AButtonDoor::Interact(APlayerController* InstigatorPlayerController, ACrazyFoodTruckCharacter* CrazyCharacter)
{
	OnDoorPressed();
}

void AButtonDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

