// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.generated.h"

class UEnhancedInputLocalPlayerSubsystem;

class UInputMappingContext;

class UKismetMaterialLibrary;

class AVehicle;

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

	void EnsurePlayerIMCs(ELocalMultiplayerInputMappingType MappingType);

	APlayerController* GetPlayerControllerForIndex(int32 PlayerIndex);
	ULocalPlayer* GetLocalPlayerForIndex(int32 PlayerIndex);
	int32 GetPlayerIndexFromController(APlayerController* PlayerController) const;

	bool PossessPawnForPlayerIndex(int32 PlayerIndex, APawn* PawnToPossess, ELocalMultiplayerInputMappingType MappingType);
	bool UnPossessPawnForPlayerIndex(int32 PlayerIndex, APawn* PlayerPawn, ELocalMultiplayerInputMappingType MappingType);

	void ApplyOutline(APawn* OutlinedPawn, int PlayerIndex);
	
	UFUNCTION(BlueprintCallable, Category="Local Multiplayer|Input")
	void AddTemporaryMappingForPlayer(int32 PlayerIndex, UInputMappingContext* IMC, int32 Priority = 100, bool bForceImmediately = true);

	UFUNCTION(BlueprintCallable, Category="Local Multiplayer|Input")
	void RemoveTemporaryMappingForPlayer(int32 PlayerIndex, UInputMappingContext* IMC, bool bForceImmediately = true);

	UInputMappingContext* GetGamepadIMC(ELocalMultiplayerInputMappingType MappingType) const;

	void SetCurrentMappingType(ELocalMultiplayerInputMappingType NewType) { CurrentMappingType = NewType; }
	ELocalMultiplayerInputMappingType GetCurrentMappingType() const { return CurrentMappingType; }
	
protected:
	UPROPERTY()
	uint8 LastAssignedPlayerIndex = 0;

	UPROPERTY()
	TMap<int, int> PlayerIndexFromKeyboardProfileIndex;

	UPROPERTY()
	TMap<int, int> PlayerIndexFromGamepadProfileIndex;

private:
	UEnhancedInputLocalPlayerSubsystem* GetEISForPlayerIndex(int32 PlayerIndex) const;

	UInputMappingContext* GetKeyboardIMC(int32 KeyboardProfileIndex, ELocalMultiplayerInputMappingType MappingType) const;

	UPROPERTY()
	ELocalMultiplayerInputMappingType CurrentMappingType = ELocalMultiplayerInputMappingType::Player;
};
