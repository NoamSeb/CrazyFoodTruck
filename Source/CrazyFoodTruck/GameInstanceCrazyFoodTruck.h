// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PhaseGameFoodFoodTruck.h"
#include "GameInstanceCrazyFoodTruck.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API UGameInstanceCrazyFoodTruck : public UGameInstance
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool InGame;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LevelName;
	UPROPERTY(VisibleAnywhere)
	EPhaseGameCrazyFoodTruckState CurrentGamePhase;
};
