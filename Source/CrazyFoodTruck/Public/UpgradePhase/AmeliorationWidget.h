// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructUpgrade.h"
#include "Blueprint/UserWidget.h"
#include "CrazyFoodTruck/Data/Public/GameInstanceCrazyFoodTruck.h"
#include "AmeliorationWidget.generated.h"

/**
 * 
 */
UCLASS()
class CRAZYFOODTRUCK_API UAmeliorationWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Move(int32 PlayerIndex, int32 PositionIndex);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void MoveModule(int32 PlayerIndex, int32 PositionIndex);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ValidModule(int32 PlayerIndex, int32 PositionIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void MoveOnSkip(int32 PlayerIndex, int32 Move, int32 PositionIndex);
	
	UFUNCTION(BlueprintImplementableEvent)
	void Validate(int32 PlayerIndex, int32 PositionIndex, bool inSkip);

	UFUNCTION(BlueprintImplementableEvent)
	void RemoveValidate(int32 PlayerIndex, int32 PositionIndex, bool inSkip);

	//UFUNCTION(BlueprintCallable, Category = "Inventory|Modifier")
	//void AddItemToInventory(int32 ItemToAdd, bool& bSuccess, int32& InventorySlot);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGameInstanceCrazyFoodTruck* GI;
	

	UFUNCTION(BlueprintCallable)
	int32 GetWinnerIndex(const TArray<int32>& ListVote);
	
	UFUNCTION(BlueprintCallable)
	FStructUpgrade CheckUpgradeChoose(const int32& Index, const TArray<FStructUpgrade>& ListUpgrade);

	UFUNCTION(BlueprintCallable)
	void Init();

	UFUNCTION(BlueprintCallable)
	void ApplyUpgrades();
};
