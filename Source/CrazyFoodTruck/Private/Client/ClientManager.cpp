// Fill out your copyright notice in the Description page of Project Settings.


#include "Client/ClientManager.h"

#include "CrazyFoodTruck/GameInstanceCrazyFoodTruck.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AClientManager::AClientManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AClientManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AClientManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AClientManager::ChooseRecipe()
{
	ChoosenRecipe =  UKismetMathLibrary::RandomIntegerInRange(0, 1);
}

void AClientManager::ReceiveRecipe(int RecievedRecipeType)
{
	if (ChoosenRecipe == RecievedRecipeType)
	{
		TObjectPtr<UGameInstance> GI = GetGameInstance();
		auto Gameinstance = Cast<UGameInstanceCrazyFoodTruck>(GI);
		if (Gameinstance)
		{
			Gameinstance->Tickets+=1;
		}
		else
		{
			return;
		}
	}
}

