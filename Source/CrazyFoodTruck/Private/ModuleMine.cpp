// Fill out your copyright notice in the Description page of Project Settings.


#include "ModuleMine.h"

#include "MineExplosive.h"


// Sets default values
AModuleMine::AModuleMine()
{
	PrimaryActorTick.bCanEverTick = true;
	SpawnMinePoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnMinePoint"));
	SpawnMinePoint->SetupAttachment(RootComponent);
}

void AModuleMine::BeginPlay()
{
	Super::BeginPlay();
}

void AModuleMine::Interact(APlayerController* InstigatorPlayerController, ACrazyFoodTruckCharacter* CrazyCharacter)
{
	if (!CanInteractWithModule()){return;}

	DropMine();
	
	Super::Interact(InstigatorPlayerController, CrazyCharacter);
}

void AModuleMine::DropMine()
{
	auto Mine = GetWorld()->SpawnActor<AMineExplosive>(MinePrefab, SpawnMinePoint->GetComponentLocation(), FRotator::ZeroRotator);
	auto PhysicsComponent = Mine->FindComponentByClass<UStaticMeshComponent>();
	if (!PhysicsComponent){return;}
	PhysicsComponent->AddAngularImpulseInDegrees(FVector(0.f, 0.f, 500.f), NAME_None, true);
}

void AModuleMine::DropTestMine()
{
	DropMine();
}

void AModuleMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

