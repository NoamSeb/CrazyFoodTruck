// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSubsystem.h"

#include "LocalMultiplayerSettings.h"

#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

void ULocalMultiplayerSubsystem::CreateAndInitPlayers(ELocalMultiplayerInputMappingType MappingType)
{
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	if (!LocalMultiplayerSettings)
	{
		return;
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
	{
		return;
	}

	const int TargetPlayers = LocalMultiplayerSettings->GetNbKeyboardProfiles();

	while (GameInstance->GetLocalPlayers().Num() < TargetPlayers)
	{
		const int ControllerId = GameInstance->GetLocalPlayers().Num();
		FString OutError;
		GameInstance->CreateLocalPlayer(ControllerId, OutError, true);
	}

	LastAssignedPlayerIndex = 0;
	PlayerIndexFromKeyboardProfileIndex.Reset();
	PlayerIndexFromGamepadProfileIndex.Reset();
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromKeyboardProfileIndex(int KeyboardProfileIndex) const
{
	if (const int* PlayerIndex = PlayerIndexFromKeyboardProfileIndex.Find(KeyboardProfileIndex))
	{
		return *PlayerIndex;
	}
	
	return -1;
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID) const
{
	if (const int* PlayerIndex = PlayerIndexFromGamepadProfileIndex.Find(DeviceID))
	{
		return *PlayerIndex;
	}

	return -1;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToKeyboardProfileIndex(int KeyboardProfileIndex)
{
	const int PlayerIndex = GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfileIndex);
	if (PlayerIndex != -1)
	{
		return PlayerIndex;
	}

	const int AssignedPlayerIndex = LastAssignedPlayerIndex++;
	PlayerIndexFromKeyboardProfileIndex.Add(KeyboardProfileIndex, AssignedPlayerIndex);
	return AssignedPlayerIndex;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToGamepadDeviceID(int DeviceID)
{
	const int PlayerIndex = GetAssignedPlayerIndexFromGamepadDeviceID(DeviceID);
	if (PlayerIndex != -1)
	{
		return PlayerIndex;
	}

	const int AssignedPlayerIndex = LastAssignedPlayerIndex++;
	PlayerIndexFromGamepadProfileIndex.Add(DeviceID, AssignedPlayerIndex);
	return AssignedPlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignKeyboardInputMapping(int PlayerIndex, int KeyboardProfileIndex, ELocalMultiplayerInputMappingType MappingType) const
{
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	if (!LocalMultiplayerSettings)
	{
		return;
	}

	if (!LocalMultiplayerSettings->KeyboardProfilesData.IsValidIndex(KeyboardProfileIndex))
	{
		return;
	}

	const UInputMappingContext* IMC = LocalMultiplayerSettings->KeyboardProfilesData[KeyboardProfileIndex].GetIMCFromType(MappingType);
	if (!IMC)
	{
		return;
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
	{
		return;
	}


	ULocalPlayer* LocalPlayer = GameInstance->GetLocalPlayerByIndex(PlayerIndex);
	if (!LocalPlayer)
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* EIS = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		if (!EIS->HasMappingContext(IMC))
		{
			FModifyContextOptions Options;
			Options.bForceImmediately = true;

			EIS->AddMappingContext(const_cast<UInputMappingContext*>(IMC), 0, Options);
		}
	}
}

void ULocalMultiplayerSubsystem::AssignGamepadInputMapping(int PlayerIndex, ELocalMultiplayerInputMappingType MappingType) const
{
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	if (!LocalMultiplayerSettings)
	{
		return;
	}

	const UInputMappingContext* IMC = LocalMultiplayerSettings->GamepadProfileData.GetIMCFromType(MappingType);
	if (!IMC)
	{
		return;
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
	{
		return;
	}
	
	ULocalPlayer* LocalPlayer = GameInstance->GetLocalPlayerByIndex(PlayerIndex);
	if (!LocalPlayer)
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* EIS = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		if (!EIS->HasMappingContext(IMC))
		{
			FModifyContextOptions Options;
			Options.bForceImmediately = true;

			EIS->AddMappingContext(const_cast<UInputMappingContext*>(IMC), 0, Options);
		}
	}
}

