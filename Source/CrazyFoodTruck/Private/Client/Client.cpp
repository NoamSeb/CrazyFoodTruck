// Fill out your copyright notice in the Description page of Project Settings.


#include "Client/Client.h"


// Sets default values
AClient::AClient()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AClient::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AClient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AClient::VerifFoodIsGood(ECuissonFood CuissonFace, ECuissonFood CuissonPile)
{
	if (CuissonFace == ECuissonFood::Grilled && CuissonPile == ECuissonFood::Grilled)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Bonne Nourriture");
		if (Food)
		{
			Food->Destroy();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Noooo");
	}
}
