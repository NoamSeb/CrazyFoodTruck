// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrazyFoodTruck/Data/ERecipe.h"
#include "GameFramework/Actor.h"
#include "ClientManager.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API AClientManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AClientManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region Recipe
private:
	UPROPERTY(EditAnywhere)
	int ChoosenRecipe;

	bool HasBeenServed;
	
	UFUNCTION()
	void ChooseRecipe();

	UFUNCTION()
	void ReceiveRecipe(int RecievedRecipeType);
#pragma endregion
};
