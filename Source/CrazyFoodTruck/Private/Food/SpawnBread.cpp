// Fill out your copyright notice in the Description page of Project Settings.


#include "Food/SpawnBread.h"


// Sets default values
ASpawnBread::ASpawnBread()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpawnBread::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnBread::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

