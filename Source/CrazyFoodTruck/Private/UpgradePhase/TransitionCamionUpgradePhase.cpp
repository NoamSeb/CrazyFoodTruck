// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradePhase/TransitionCamionUpgradePhase.h"


// Sets default values
ATransitionCamionUpgradePhase::ATransitionCamionUpgradePhase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATransitionCamionUpgradePhase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATransitionCamionUpgradePhase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

