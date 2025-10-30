// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

class ACrazyFoodTruckCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInterfaceEvent);

UINTERFACE(BlueprintType)
class CRAZYFOODTRUCK_API UInteractable : public UInterface
{
	GENERATED_BODY()
};

class CRAZYFOODTRUCK_API IInteractable
{
	GENERATED_BODY()

public:
	virtual void Interact(APlayerController* InstigatorPlayerController, ACrazyFoodTruckCharacter* CrazyCharacter) = 0;
	// virtual FOnInterfaceEvent OnInteractionStarted() = 0;
	// virtual FOnInterfaceEvent OnInteractionEnded() = 0;
	//
	// virtual FOnInterfaceEvent OnObjectCanInteract() = 0;
	// virtual FOnInterfaceEvent OnObjectCannotInteract() = 0;
};
