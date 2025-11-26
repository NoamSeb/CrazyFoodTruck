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

class UFoodTruckDataSubSystem;
class UGameDataSubSystem;
class UZombieDataSubSystem;
class UMaterialParameterCollection;

USTRUCT(BlueprintType)
struct FMenuPlayerSlot
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsConnected = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsReady = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ControllerId = INDEX_NONE;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLobbySlotsChanged);

UCLASS()
class CRAZYFOODTRUCK_API UGameInstanceCrazyFoodTruck : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

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
	int32 CurrentLifeFoodTruck = 5;

	int actoraspurr;

	UPROPERTY(BlueprintReadOnly, Category = "Lobby")
	TArray<FMenuPlayerSlot> PlayerSlots;

	UPROPERTY(BlueprintAssignable, Category = "Lobby")
	FOnLobbySlotsChanged OnLobbySlotsChanged;

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void InitLobbySlots();

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void TryJoinPlayer(int32 ControllerId);

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	bool AreAllPlayersConnected() const;

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void ResetLobby();

	UFUNCTION(BlueprintPure, Category = "Lobby|Colors")
	FLinearColor GetPlayerColorForIndex(int32 PlayerIndex) const;

	UFUNCTION(BlueprintPure, Category = "Lobby|Colors")
	int32 GetSlotIndexForControllerId(int32 ControllerId) const;

	UFUNCTION(BlueprintPure, Category = "Lobby|Colors")
	FLinearColor GetPlayerColorForControllerId(int32 ControllerId) const;

private:
	ACameraShakeManager* CameraShakeManager;
};
