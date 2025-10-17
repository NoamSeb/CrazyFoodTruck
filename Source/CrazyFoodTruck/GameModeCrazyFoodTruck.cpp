// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeCrazyFoodTruck.h"

AGameModeCrazyFoodTruck::AGameModeCrazyFoodTruck()
{
	GetInstance = Cast<UGameInstanceCrazyFoodTruck>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void AGameModeCrazyFoodTruck::ChangeGamePhase()
{
	switch (GetInstance->CurrentGamePhase)
	{
	case EPhaseGameCrazyFoodTruckState::Menu:
		GamePhaseRoute();
	case EPhaseGameCrazyFoodTruckState::Route:
		GamePhaseBase();
	case EPhaseGameCrazyFoodTruckState::Base:
		GamePhaseAmelioration();
	case EPhaseGameCrazyFoodTruckState::Amelioration:
		GamePhaseRoute();
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
	//setup pour la phase d'amelioration
	//augmentation de la difficulté pour le phase route
}

bool AGameModeCrazyFoodTruck::CheckDefeat()
{
	return false;
} 