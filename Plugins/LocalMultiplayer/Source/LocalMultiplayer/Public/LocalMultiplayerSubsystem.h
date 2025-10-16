// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class LOCALMULTIPLAYER_API ULocalMultiplayerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Local Multiplayer")
	void CreateAndInitPlayers(ELocalMultiplayerInputMappingType MappingType);

	int GetAssignedPlayerIndexFromKeyboardProfileIndex(int KeyboardProfileIndex) const;
	int GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID) const;

	int AssignNewPlayerToKeyboardProfileIndex(int KeyboardProfileIndex);
	int AssignNewPlayerToGamepadDeviceID(int DeviceID);

	void AssignKeyboardInputMapping(int PlayerIndex, int KeyboardProfileIndex, ELocalMultiplayerInputMappingType MappingType) const;
	void AssignGamepadInputMapping(int PlayerIndex, ELocalMultiplayerInputMappingType MappingType) const;

protected:
	UPROPERTY()
	uint8 LastAssignedPlayerIndex = 0;

	UPROPERTY()
	TMap<int, int> PlayerIndexFromKeyboardProfileIndex;

	UPROPERTY()
	TMap<int, int> PlayerIndexFromGamepadProfileIndex;
};
