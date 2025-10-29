// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBox.h"


// Sets default values
AAmmoBox::AAmmoBox()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAmmoBox::Initialize()
{
	OnAmmoInitialize.Broadcast();
}

void AAmmoBox::BeginPlay()
{
	Super::BeginPlay();
}

void AAmmoBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

