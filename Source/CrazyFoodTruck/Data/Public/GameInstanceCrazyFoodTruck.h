// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Camera/CameraShakeManager.h"
#include "FoodTruckDataSubSystem.h"
#include "GameDataSubSystem.h"
#include "ZombieDataSubSystem.h"
#include "UpgradePhase/StructUpgrade.h"
#include "UpgradePhase/EnumUpgrade/ZoneUpgrade.h"
#include "GameInstanceCrazyFoodTruck.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API UGameInstanceCrazyFoodTruck : public UGameInstance
{
	GENERATED_BODY()

	

	virtual void Init() override;

	
	public:

	// FUNCTION

	void SetCameraShakeManager(ACameraShakeManager* NewCameraShakeManager);
	void PlayerCameraShake(ECameraShake ShakeType);

	TObjectPtr<UFoodTruckDataSubSystem> FoodTruckData;
	TObjectPtr<UGameDataSubSystem> GameData;
	TObjectPtr<UZombieDataSubSystem> ZombieData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 nbrTickets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FStructUpgrade> ListUpgrades;

	UFUNCTION(BlueprintCallable)
	TArray<FStructUpgrade> ListUpgradesToAdd(EZoneUpgrade ZoneUpgrade);

	UFUNCTION(BlueprintCallable)
	void ClearUpgrades();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EModuleUpgrade ModuleUpgradeLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EModuleUpgrade ModuleUpgradeRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentLifeFoodTruck = 20;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 MaxLifeFoodTruck = 20;

	int actoraspurr;

private:
	ACameraShakeManager* CameraShakeManager;
};
