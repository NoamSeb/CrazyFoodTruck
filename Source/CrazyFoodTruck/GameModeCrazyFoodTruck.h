// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PhaseGameFoodFoodTruck.h"
#include "Data/Public/GameInstanceCrazyFoodTruck.h"
#include "GameModeCrazyFoodTruck.generated.h"

/**
 * 
 */
UCLASS()
class CRAZYFOODTRUCK_API AGameModeCrazyFoodTruck : public AGameMode
{
	GENERATED_BODY()

	AGameModeCrazyFoodTruck();

	public:
	const UGameInstanceCrazyFoodTruck* GetInstance;
	UFUNCTION(BlueprintCallable, Category = "Game Mode")
	void ChangeGamePhase();
	UFUNCTION(BlueprintCallable, Category = "Game Mode")
	bool CheckDefeat();


	protected:
	virtual void BeginPlay() override;
	
	private:
	void GamePhaseRoute();
	void GamePhaseBase();
	void GamePhaseAmelioration();
};
