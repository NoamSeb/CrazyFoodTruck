// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradePhase/AmeliorationWidget.h"

#include "UpgradePhase/StructUpgrade.h"

void UAmeliorationWidget::Init()
{
	if (UGameInstance* GIBase = GetGameInstance())
	{
		GI = Cast<UGameInstanceCrazyFoodTruck>(GIBase);
	}
}

void UAmeliorationWidget::ApplyUpgrades()
{
	UFoodTruckDataSubSystem* TruckSubSystem = GI->GetSubsystem<UFoodTruckDataSubSystem>();
	
	TArray<FStructUpgrade> Upgrades;
	
	Upgrades = GI->ListUpgradesToAdd(EZoneUpgrade::Camion);
	for (FStructUpgrade Upgrade : Upgrades)
	{
		switch (Upgrade.UpgradeCamion)
		{
		case ECamionUpgrade::Speed :
			TruckSubSystem->Speed += Upgrade.intAddModif;
			break;
		case ECamionUpgrade::TurningSpeed:
			TruckSubSystem->TruckRotationSpeed += Upgrade.intAddModif;
			break;
		case ECamionUpgrade::Heal:
			GI->CurrentLifeFoodTruck += Upgrade.intAddModif;
			break;
		case ECamionUpgrade::FullHeal:
			GI->CurrentLifeFoodTruck = GI->MaxLifeFoodTruck;
		case ECamionUpgrade::ReplenishAmmoWhenKilledZombies:
			TruckSubSystem->ReplenishAmmoWhenTruckKillZombies = true;
		case ECamionUpgrade::FasterRepair:
			TruckSubSystem->RepairSpeed += Upgrade.intAddModif;
		default:
			break;
		}
	}

	Upgrades = GI->ListUpgradesToAdd(EZoneUpgrade::Turret);
	for (FStructUpgrade Upgrade : Upgrades)
	{
		switch (Upgrade.UpgradeTurret)
		{
		case ETurretUpgrade::RotationSpeedCabestan:
			TruckSubSystem->RotationSpeed += Upgrade.intAddModif;
			break;
		case ETurretUpgrade::AugmentationAngle :
			TruckSubSystem->TurretRangeSide += Upgrade.intAddModif;
			break;
		case ETurretUpgrade::Cadence:
			TruckSubSystem->TurretFireRate += Upgrade.intAddModif;
			break;
		case ETurretUpgrade::Damage:
			TruckSubSystem->DamagePerBullet += Upgrade.intAddModif;
			break;
		case ETurretUpgrade::MaxAmmo:
			TruckSubSystem->TurretMaxAmmo = Upgrade.intAddModif;
			break;
		case ETurretUpgrade::SpeedBullet:
			TruckSubSystem->SpeedBullet += Upgrade.intAddModif;
			break;
		case ETurretUpgrade::TripleDamageFor10EmeBullet:
			TruckSubSystem->TripleDamageFor10EBullet = true;
		case ETurretUpgrade::IncreaseDamageWhenFullReload:
			TruckSubSystem->IncreaseDamageWhenFullReload = true;
			TruckSubSystem->DamageIncreaseWhenFullReload = Upgrade.intAddModif;
			break;
		default:
			break;
		}
	}

	Upgrades = GI->ListUpgradesToAdd(EZoneUpgrade::Bullet);
	for (FStructUpgrade Upgrade : Upgrades)
	{
		TruckSubSystem->TypeBullet = Upgrade.UpgradeBullet;
	}


	GI->ClearUpgrades();
}



FStructUpgrade UAmeliorationWidget::CheckUpgradeChoose(const int32& Index, const TArray<FStructUpgrade>& ListUpgrade)
{
	if (Index == 4)
	{
		FStructUpgrade newStruct = FStructUpgrade{};
		newStruct.Zone = EZoneUpgrade::Skip;
		return newStruct;
	}
	return ListUpgrade[Index];
}


int32 UAmeliorationWidget::GetWinnerIndex(const TArray<int32>& ListVote)
{
	int nbrVote = 0;
	int32 indexVoteFinal = 0;
	
	for (int i = 0; i < ListVote.Num(); ++i)
	{
		if (ListVote[i] > nbrVote)
		{
			nbrVote = ListVote[i];
			indexVoteFinal = i;
		} else if (ListVote[i] == nbrVote)
		{
			if (FMath::RandBool())
			{
				nbrVote = ListVote[i];
				indexVoteFinal = i;
			}
		}
	}

	return indexVoteFinal;
}
