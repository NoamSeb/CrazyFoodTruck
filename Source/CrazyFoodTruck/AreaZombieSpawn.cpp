// Fill out your copyright notice in the Description page of Project Settings.


#include "AreaZombieSpawn.h"


// Sets default values
AAreaZombieSpawn::AAreaZombieSpawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// Crée la box
	NewBoxAreaSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnAreaBox"));

	NewBoxAreaSpawn->SetBoxExtent(FVector(500.0f, 500.0f, 200.0f)); // taille par défaut
	NewBoxAreaSpawn->SetHiddenInGame(true);
	NewBoxAreaSpawn->SetVisibility(true);
	NewBoxAreaSpawn->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAreaZombieSpawn::InitArea(int32 IdArea, AActor* ParentActor)
{
	
	if (ParentActor)
	{
		// Attaché au parent pour que ce soit un "enfant"
		AttachToActor(ParentActor, FAttachmentTransformRules::KeepRelativeTransform);
		FVector RelativeLocation = FVector(1000.0f * IdArea, 0.0f, 0.0f);
		SetActorRelativeLocation(RelativeLocation);
	}
}

// Called when the game starts or when spawned
void AAreaZombieSpawn::BeginPlay()
{
	Super::BeginPlay();

	auto x = Get();
	auto y = GetOwner();
	

	// PRINT NAME OF PARENT ACTOR
	if (x)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AreaZombieSpawn Parent Actor: %s"), *x->GetName()));
	}
	if (y)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("AreaZombieSpawn Owner Actor: %s"), *y->GetName()));
	}
}

// Called every frame
void AAreaZombieSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
