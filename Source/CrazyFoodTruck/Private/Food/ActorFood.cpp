// Fill out your copyright notice in the Description page of Project Settings.


#include "Food/ActorFood.h"


// Sets default values
AActorFood::AActorFood()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AActorFood::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActorFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OnGrille)
	{
		if (!IsReturn)
		{
			cuissonFace += DeltaTime;
			if (cuissonFace > TimeGrilled && cuissonFace < TimeBurned)
			{
				CurrentCuissonFace = ECuissonFood::Grilled;
			}
			else if (cuissonFace > TimeBurned && CanBurnedFace)
			{
				CurrentCuissonFace = ECuissonFood::Burned;
				//Animation avant ?
				this->Destroy();
			}
			else
			{
				CurrentCuissonFace = ECuissonFood::NotGrilled;
			}
		}
		else
		{
			cuissonPile += DeltaTime;
			if (cuissonPile > TimeGrilled && cuissonPile < TimeBurned)
			{
				CurrentCuissonPile = ECuissonFood::Grilled;
			}
			else if (cuissonPile > TimeBurned && CanBurnPile)
			{
				CurrentCuissonPile = ECuissonFood::Burned;
				//Animation avant ?
				this->Destroy();
			}
			else
			{
				CurrentCuissonPile = ECuissonFood::NotGrilled;
			}
		}
	}
}

