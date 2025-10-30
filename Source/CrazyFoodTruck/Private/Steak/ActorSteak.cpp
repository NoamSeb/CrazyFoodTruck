// Fill out your copyright notice in the Description page of Project Settings.


#include "Steak/ActorSteak.h"


// Sets default values
AActorSteak::AActorSteak()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AActorSteak::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActorSteak::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OnGrille)
	{
		if (!IsReturn)
		{
			cuissonFace += DeltaTime;
			if (cuissonFace > TempsSteakCuit && cuissonFace < TempsSteakBrule)
			{
				CurrentCuissonFace = ECuissonSteak::Cuit;
			}
			else if (cuissonFace > TempsSteakBrule)
			{
				CurrentCuissonFace = ECuissonSteak::Brule;
				//Animation avant ?
				this->Destroy();
			}
			else
			{
				CurrentCuissonFace = ECuissonSteak::PasCuit;
			}
		}
		else
		{
			cuissonPile += DeltaTime;
			if (cuissonPile > TempsSteakCuit && cuissonPile < TempsSteakBrule)
			{
				CurrentCuissonPile = ECuissonSteak::Cuit;
			}
			else if (cuissonPile > TempsSteakBrule)
			{
				CurrentCuissonPile = ECuissonSteak::Brule;
				//Animation avant ?
				this->Destroy();
			}
			else
			{
				CurrentCuissonPile = ECuissonSteak::PasCuit;
			}
		}
	}
}

