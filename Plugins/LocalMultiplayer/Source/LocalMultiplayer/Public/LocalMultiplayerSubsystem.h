// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LocalMultiplayerSettings.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
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
	UInputMappingContext* GetIMCFromEnumType(ELocalMultiplayerInputMappingType MappingType) const;
	void AssignGamepadInputMapping(int PlayerIndex, ELocalMultiplayerInputMappingType MappingType) const;

	// GABRIEL ADD

	APlayerController* GetPlayerControllerForIndex(int PlayerIndex);
	ULocalPlayer* GetLocalPlayerForIndex(int PlayerIndex);

	bool PossessPawnForPlayerIndex(int PlayerIndex, APawn* PawnToPossess,ELocalMultiplayerInputMappingType MappingType);
	bool UnPossessPawnForPlayerIndex(int PlayerIndex, APawn* PlayerPawn, ELocalMultiplayerInputMappingType MappingType);
	int GetPlayerIndexFromController(APlayerController* PlayerController) const;
	
	// MAXIME ADD
	
	UFUNCTION(BlueprintCallable, Category="Local Multiplayer|Input")
	void AddTemporaryMappingForPlayer(int PlayerIndex, UInputMappingContext* IMC, int Priority = 100, bool bForceImmediately = true);

	UFUNCTION(BlueprintCallable, Category="Local Multiplayer|Input")
	void RemoveTemporaryMappingForPlayer(int PlayerIndex, UInputMappingContext* IMC, bool bForceImmediately = true);

private:
	
	UEnhancedInputLocalPlayerSubsystem* GetEISForPlayerIndex(int PlayerIndex) const;
	UEnhancedInputLocalPlayerSubsystem* GetEISForPlayerController(APlayerController* PlayerController) const;
	
protected:
	UPROPERTY()
	uint8 LastAssignedPlayerIndex = 0;

	UPROPERTY()
	TMap<int, int> PlayerIndexFromKeyboardProfileIndex;

	UPROPERTY()
	TMap<int, int> PlayerIndexFromGamepadProfileIndex;
};
