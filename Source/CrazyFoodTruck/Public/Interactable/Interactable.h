// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

class ACrazyFoodTruckCharacter;

UINTERFACE(BlueprintType)
class CRAZYFOODTRUCK_API UInteractable : public UInterface
{
	GENERATED_BODY()
};

class CRAZYFOODTRUCK_API IInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interactable")
	void Interact(ACrazyFoodTruckCharacter* InstigatorCharacter);
};
