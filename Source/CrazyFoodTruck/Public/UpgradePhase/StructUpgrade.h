// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumUpgrade/CamionUpgrade.h"
#include "EnumUpgrade/JeuUpgrade.h"
#include "EnumUpgrade/ModuleUpgrade.h"
#include "EnumUpgrade/TurretUpgrade.h"
#include "Bullet/EBulletType.h"
#include "Engine/DataTable.h"
#include "EnumUpgrade/ZoneUpgrade.h"
#include "StructUpgrade.generated.h"

USTRUCT(BlueprintType)
struct FStructUpgrade
{
	GENERATED_BODY()

	FStructUpgrade()
		: TextUpgrade()
		, intAddModif(0)
		, UpgradeTurret(ETurretUpgrade::None)
		, UpgradeModule(EModuleUpgrade::None)
		, UpgradeGame(EJeuUpgrade::None)
		, UpgradeBullet(EbulletType::BulletNormal)
		, UpgradeCamion(ECamionUpgrade::None)
		, Zone(EZoneUpgrade::Camion)
		, coutUpgrade(0)
		, IsAlreadyUse(false)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	FString TextUpgrade;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	int32 intAddModif;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	ETurretUpgrade UpgradeTurret = ETurretUpgrade::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	EModuleUpgrade UpgradeModule = EModuleUpgrade::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	EJeuUpgrade UpgradeGame = EJeuUpgrade::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	EbulletType UpgradeBullet = EbulletType::BulletNormal;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	ECamionUpgrade UpgradeCamion = ECamionUpgrade::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	EZoneUpgrade Zone = EZoneUpgrade::Camion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	int32 coutUpgrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	bool IsAlreadyUse = false;
};