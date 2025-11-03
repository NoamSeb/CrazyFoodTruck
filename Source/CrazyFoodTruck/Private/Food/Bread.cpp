// Fill out your copyright notice in the Description page of Project Settings.


#include "Food/Bread.h"


// Sets default values
ABread::ABread()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CanBurnedFace = false;
	CanBurnPile = false;
}

// Called when the game starts or when spawned
void ABread::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABread::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

