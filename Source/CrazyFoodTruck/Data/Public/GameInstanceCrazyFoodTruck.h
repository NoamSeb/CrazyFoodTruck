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

USTRUCT(BlueprintType)
struct FMenuPlayerSlot
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Lobby")
	bool bIsConnected = false;

	UPROPERTY(BlueprintReadOnly, Category = "Lobby")
	int32 ControllerId = INDEX_NONE;

	UPROPERTY(BlueprintReadOnly, Category = "Lobby")
	bool bIsReady = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLobbySlotsChanged);

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
	int32 CurrentLifeFoodTruck = 5;

	int actoraspurr;

	UPROPERTY(BlueprintReadOnly, Category = "CrazyFoodTruck|Lobby")
	TArray<FMenuPlayerSlot> PlayerSlots;

	UPROPERTY(BlueprintAssignable, Category = "CrazyFoodTruck|Lobby")
	FOnLobbySlotsChanged OnLobbySlotsChanged;

	UFUNCTION(BlueprintCallable, Category = "CrazyFoodTruck|Lobby")
	void TryJoinPlayer(int32 ControllerId);

	UFUNCTION(BlueprintCallable, Category = "CrazyFoodTruck|Lobby")
	bool AreAllPlayersConnected() const;

	UFUNCTION(BlueprintCallable, Category = "CrazyFoodTruck|Lobby")
	void ResetLobby();

	UFUNCTION(BlueprintCallable, Category = "CrazyFoodTruck|Lobby")
	void StartGameFromLobby();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CrazyFoodTruck|Lobby")
	FName GameLevelName = FName(TEXT("SandboxGab"));

private:
	ACameraShakeManager* CameraShakeManager;

	void InitLobbySlots();
};
