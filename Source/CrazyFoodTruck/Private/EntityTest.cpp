// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityTest.h"


AEntityTest::AEntityTest()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEntityTest::BeginPlay()
{
	Super::BeginPlay();
}


void AEntityTest::CrushUnderTruck()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Crushed !");
}


// Called every frame
void AEntityTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

