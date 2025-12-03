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
#include "Score/ScoreManagerComponent.h"
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

private:
	
	int32 nbrTickets;
	
public:

	// FUNCTION

	void SetCameraShakeManager(ACameraShakeManager* NewCameraShakeManager);
	void PlayerCameraShake(ECameraShake ShakeType);

	TObjectPtr<UFoodTruckDataSubSystem> FoodTruckData;
	TObjectPtr<UGameDataSubSystem> GameData;
	TObjectPtr<UZombieDataSubSystem> ZombieData;


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

	UFUNCTION(BlueprintCallable, Category = "Tickets")
	int GetTicketsNumber() const;

	UFUNCTION(BlueprintCallable, Category = "Tickets")
	int IncrementTicketsNumber(int Amount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tickets")
	TMap<EScoreGrade, int32> TicketsByGrade;

	UFUNCTION(BlueprintCallable, Category = "Tickets")
	int32 AddTicketsForGrade(EScoreGrade Grade);

private:
	ACameraShakeManager* CameraShakeManager;
};
