// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable/Interactable.h"
#include "ButtonDoor.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API AButtonDoor : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	AButtonDoor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category="ButtonDoor|Events")
	void OnDoorPressed();

	virtual void Interact(APlayerController* InstigatorPlayerController, ACrazyFoodTruckCharacter* CrazyCharacter) override;

public:
	virtual void Tick(float DeltaTime) override;
};
