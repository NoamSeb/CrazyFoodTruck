// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FZoneSpawn.h"
#include "PositionSpawn.h"
#include "Engine/DataTable.h"
#include "FWaveStructure.generated.h"

USTRUCT(BlueprintType)
struct FWaveStructure : public FTableRowBase
{
	GENERATED_BODY()

	FWaveStructure()
		: ZombieAmount(0)
		, ZoneSpawn()
	, TargetZombiePoint(ETargetZombiePoint::MiddleDown)
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave")
	int32 ZombieAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave")
	AAreaZombieSpawn* ZoneSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave")
	ETargetZombiePoint TargetZombiePoint;
};