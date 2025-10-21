// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CrazyFoodTruckCharacterInputData.generated.h"

class UInputAction;

/**
 * 
 */
UCLASS()
class CRAZYFOODTRUCK_API UCrazyFoodTruckCharacterInputData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta=(DisplayName="Input Action Move"))
	TObjectPtr<UInputAction> InputActionMove = nullptr;

	UPROPERTY(EditAnywhere, meta=(DisplayName="Input Action Interact"))
	TObjectPtr<UInputAction> InputActionInteract = nullptr;
};
