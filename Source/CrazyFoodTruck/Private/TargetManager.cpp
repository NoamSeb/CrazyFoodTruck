// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetManager.h"


// Sets default values
ATargetManager::ATargetManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATargetManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATargetManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!TargetPawn){return;}
	FVector TruckLocation = TargetPawn->GetActorLocation();
	SetActorLocation(TruckLocation, false, nullptr, ETeleportType::None);
}



