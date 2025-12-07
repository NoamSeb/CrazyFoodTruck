// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CrazyFoodTruckSettings.generated.h"

class ACrazyFoodTruckCharacter;

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="CrazyFoodTruck Settings"))
class CRAZYFOODTRUCK_API UCrazyFoodTruckSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="Characters")
	TSubclassOf<ACrazyFoodTruckCharacter> CrazyFoodTruckCharacterClassP0;

	UPROPERTY(Config, EditAnywhere, Category="Characters")
	TSubclassOf<ACrazyFoodTruckCharacter> CrazyFoodTruckCharacterClassP1;

	UPROPERTY(Config, EditAnywhere, Category="Characters")
	TSubclassOf<ACrazyFoodTruckCharacter> CrazyFoodTruckCharacterClassP2;
};
