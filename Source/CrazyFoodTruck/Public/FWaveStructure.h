// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PositionSpawn.h"
#include "Engine/DataTable.h"
#include "FWaveStructure.generated.h"

USTRUCT(BlueprintType)
struct FWaveStructure : public FTableRowBase
{
	GENERATED_BODY()

	FWaveStructure()
		: ZombieAmount(0)
		, TempSpawnBetweenWave(1.f)
		, PositionSpawn(EPositionSpawn::TopRight) // exemple default (ajuste selon ton enum)
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave")
	int32 ZombieAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave")
	float TempSpawnBetweenWave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave")
	EPositionSpawn PositionSpawn;
};