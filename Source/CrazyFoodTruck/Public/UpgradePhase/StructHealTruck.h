// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumUpgrade/CamionUpgrade.h"
#include "Engine/DataTable.h"
#include "EnumUpgrade/ZoneUpgrade.h"
#include "StructHealTruck.generated.h"

USTRUCT(BlueprintType)
struct FStructHealTruck
{
	GENERATED_BODY()

	FStructHealTruck()
		: ImgUpgrade()
		, TextUpgrade()
		, TextUpgradeDescription()
		, intAddModif(10)
		, UpgradeTruck(ECamionUpgrade::Heal)
		, Zone(EZoneUpgrade::Camion)
		, coutUpgrade(0)
	{}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	UTexture2D* ImgUpgrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	FString TextUpgrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	FString TextUpgradeDescription;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	int32 intAddModif;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Upgrade")
	ECamionUpgrade UpgradeTruck = ECamionUpgrade::Heal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Upgrade")
	EZoneUpgrade Zone = EZoneUpgrade::Camion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	int32 coutUpgrade;
};