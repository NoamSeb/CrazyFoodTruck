// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CrazyFoodTruck/PhaseGameFoodFoodTruck.h"
#include "GameDataSubSystem.generated.h"

/**
 * 
 */
UCLASS()
class CRAZYFOODTRUCK_API UGameDataSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool InGame;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int LevelNumber = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPhaseGameCrazyFoodTruckState CurrentGamePhase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NbrZombiesKill = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NbrLevelFinish = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NbrTicketsUse = 0;
};
