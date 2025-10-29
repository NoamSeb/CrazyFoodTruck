// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieIA.h"


// Sets default values
AZombieIA::AZombieIA()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AZombieIA::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZombieIA::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AZombieIA::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

