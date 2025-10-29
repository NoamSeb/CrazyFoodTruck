// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeCrazyFoodTruck.h"

AGameModeCrazyFoodTruck::AGameModeCrazyFoodTruck()
{
}

void AGameModeCrazyFoodTruck::BeginPlay()
{
	Super::BeginPlay();
	GetInstance = Cast<UGameInstanceCrazyFoodTruck>(GetGameInstance());
	//GetInstance = UGameInstanceCrazyFoodTruck().GameInstance;  ???
}


void AGameModeCrazyFoodTruck::ChangeGamePhase()
{
	switch (GetInstance->CurrentGamePhase)
	{
	case EPhaseGameCrazyFoodTruckState::Route:
		GamePhaseBase();
		break;
	case EPhaseGameCrazyFoodTruckState::Base:
		GamePhaseAmelioration();
		break;
	case EPhaseGameCrazyFoodTruckState::Amelioration:
		GamePhaseRoute();
		break;
	}
}

void AGameModeCrazyFoodTruck::GamePhaseRoute()
{
	//setup la phase route
}

void AGameModeCrazyFoodTruck::GamePhaseBase()
{
	//transition ?
	//setup pour la phase base
}

void AGameModeCrazyFoodTruck::GamePhaseAmelioration()
{
	//setup pour la phase d amelioration
	//augmentation de la difficult pour le phase route
}

bool AGameModeCrazyFoodTruck::CheckDefeat()
{
	return false;
} 