// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructHealTruck.h"
#include "StructUpgrade.h"
#include "StructListUpgrade.generated.h"

USTRUCT(BlueprintType)
struct FStructListUpgrade  : public FTableRowBase
{
	GENERATED_BODY()

	FStructListUpgrade()
		: ListCamion()
		, ListModule()
		, ListBullet()
		, ListTurret()
		, ListGame()
		, HealTruck()
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	TArray<FStructUpgrade> ListCamion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	TArray<FStructUpgrade> ListModule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	TArray<FStructUpgrade> ListBullet;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	TArray<FStructUpgrade> ListTurret;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	TArray<FStructUpgrade> ListGame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	FStructHealTruck HealTruck;
};