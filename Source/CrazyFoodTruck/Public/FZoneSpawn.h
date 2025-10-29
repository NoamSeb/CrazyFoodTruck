// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PositionSpawn.h"
#include "CrazyFoodTruck/AreaZombieSpawn.h"
#include "FZoneSpawn.generated.h"

USTRUCT(BlueprintType)
struct FZoneSpawn : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Zone")
	EPositionSpawn PositionSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Zone")
	AAreaZombieSpawn* AreaZombieSpawn;
	
	FZoneSpawn() = default;
	FZoneSpawn(EPositionSpawn PosSpawn, AAreaZombieSpawn* NewArea);
	FZoneSpawn(AAreaZombieSpawn* NewArea);
};