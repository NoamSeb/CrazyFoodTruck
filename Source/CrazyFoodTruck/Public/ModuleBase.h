// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable/Interactable.h"
#include "ModuleBase.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API AModuleBase : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	AModuleBase();
	

protected:
	virtual void BeginPlay() override;
	virtual void Interact(APlayerController* InstigatorPlayerController, ACrazyFoodTruckCharacter* CrazyCharacter) override;
	

public:
	virtual void Tick(float DeltaTime) override;
};
