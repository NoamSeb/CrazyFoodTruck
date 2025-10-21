// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CrazyFoodTruckCharacterSettings.generated.h"

class UCrazyFoodTruckCharacterInputData;

class UInputMappingContext;

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="CrazyFoodTruck Character Settings"))
class CRAZYFOODTRUCK_API UCrazyFoodTruckCharacterSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
    UPROPERTY(Config, EditAnywhere, Category="Inputs")
    TSoftObjectPtr<UCrazyFoodTruckCharacterInputData> InputData;

    UPROPERTY(Config, EditAnywhere, Category="Inputs")
    TSoftObjectPtr<UInputMappingContext> InputMappingContext;
};
