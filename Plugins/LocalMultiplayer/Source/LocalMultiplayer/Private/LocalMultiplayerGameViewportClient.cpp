// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerGameViewportClient.h"

#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"

#include "Engine/EngineBaseTypes.h"
#include "Engine/GameInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"
#include "InputKeyEventArgs.h"

void ULocalMultiplayerGameViewportClient::PostInitProperties()
{
	Super::PostInitProperties();
	
	MaxSplitscreenPlayers = 8;
}

bool ULocalMultiplayerGameViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	if (EventArgs.Event != IE_Pressed && EventArgs.Event != IE_Repeat && EventArgs.Event != IE_Released)
	{
		return Super::InputKey(EventArgs);
	}

	UWorld* WorldPtr = GetWorld();
	if (!WorldPtr)
	{
		return Super::InputKey(EventArgs);
	}

	UGameInstance* GameInstancePtr = GetGameInstance();
	if (!GameInstancePtr)
	{
		return Super::InputKey(EventArgs);
	}

	ULocalMultiplayerSettings* LocalMultiplayerSettings = GetMutableDefault<ULocalMultiplayerSettings>();
	if (!LocalMultiplayerSettings)
	{
		return Super::InputKey(EventArgs);
	}

	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstancePtr->GetSubsystem<ULocalMultiplayerSubsystem>();
	if (!LocalMultiplayerSubsystem)
	{
		return Super::InputKey(EventArgs);
	}

	const ELocalMultiplayerInputMappingType MappingType = ELocalMultiplayerInputMappingType::InGame;

	if (!EventArgs.Key.IsGamepadKey())
	{
		const int KeyboardProfileIndex = LocalMultiplayerSettings->FindKeyboardProfileIndexFromKey(EventArgs.Key, MappingType);
		if (KeyboardProfileIndex != -1)
		{
			int PlayerIndex = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfileIndex);
			const bool bWasAssigned = (PlayerIndex != -1);
			if (!bWasAssigned)
			{
				PlayerIndex = LocalMultiplayerSubsystem->AssignNewPlayerToKeyboardProfileIndex(KeyboardProfileIndex);
				LocalMultiplayerSubsystem->AssignKeyboardInputMapping(PlayerIndex, KeyboardProfileIndex, MappingType);
			}

			if (ULocalPlayer* LocalPlayer = GameInstancePtr->GetLocalPlayerByIndex(PlayerIndex))
			{
				if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(WorldPtr))
				{
					const float AmountDepressed = (EventArgs.Event == IE_Released) ? 0.f : 1.f;
					return PlayerController->InputKey(EventArgs.Key, EventArgs.Event, AmountDepressed, false);
				}
			}
		}
	}

	if (EventArgs.Key.IsGamepadKey())
	{
		const int DeviceID = EventArgs.InputDevice.GetId();

		int PlayerIndex = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromGamepadDeviceID(DeviceID);
		const bool bWasAssigned = (PlayerIndex != -1);
		if (!bWasAssigned)
		{
			PlayerIndex = LocalMultiplayerSubsystem->AssignNewPlayerToGamepadDeviceID(DeviceID);
			LocalMultiplayerSubsystem->AssignGamepadInputMapping(PlayerIndex, MappingType);
		}

		if (ULocalPlayer* LocalPlayer = GameInstancePtr->GetLocalPlayerByIndex(PlayerIndex))
		{
			if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(WorldPtr))
			{
				const float AmountDepressed = (EventArgs.Event == IE_Released) ? 0.f : 1.f;
				return PlayerController->InputKey(EventArgs.Key, EventArgs.Event, AmountDepressed, false);
			}
		}
	}

	return Super::InputKey(EventArgs);
}

bool ULocalMultiplayerGameViewportClient::InputAxis(FViewport* InViewport, FInputDeviceId InputDevice, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	UWorld* WorldPtr = GetWorld();
	if (!WorldPtr)
	{
		return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
	}

	UGameInstance* GameInstancePtr = GetGameInstance();
	if (!GameInstancePtr)
	{
		return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
	}

	ULocalMultiplayerSettings* LocalMultiplayerSettings = GetMutableDefault<ULocalMultiplayerSettings>();
	if (!LocalMultiplayerSettings)
	{
		return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
	}

	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstancePtr->GetSubsystem<ULocalMultiplayerSubsystem>();
	if (!LocalMultiplayerSubsystem)
	{
		return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
	}

	const ELocalMultiplayerInputMappingType MappingType = ELocalMultiplayerInputMappingType::InGame;

	if (!bGamepad && !Key.IsGamepadKey())
	{
		const int KeyboardProfileIndex = LocalMultiplayerSettings->FindKeyboardProfileIndexFromKey(Key, MappingType);
		if (KeyboardProfileIndex != -1)
		{
			int PlayerIndex = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfileIndex);
			const bool bWasAssigned = (PlayerIndex != -1);
			if (!bWasAssigned)
			{
				PlayerIndex = LocalMultiplayerSubsystem->AssignNewPlayerToKeyboardProfileIndex(KeyboardProfileIndex);
				LocalMultiplayerSubsystem->AssignKeyboardInputMapping(PlayerIndex, KeyboardProfileIndex, MappingType);
			}

			if (ULocalPlayer* LocalPlayer = GameInstancePtr->GetLocalPlayerByIndex(PlayerIndex))
			{
				if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(WorldPtr))
				{
					return PlayerController->InputAxis(Key, Delta, DeltaTime, NumSamples, false);
				}
			}
		}
	}

	if (bGamepad || Key.IsGamepadKey())
	{
		const int DeviceID = InputDevice.GetId();

		int PlayerIndex = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromGamepadDeviceID(DeviceID);
		const bool bWasAssigned = (PlayerIndex != -1);
		if (!bWasAssigned)
		{
			PlayerIndex = LocalMultiplayerSubsystem->AssignNewPlayerToGamepadDeviceID(DeviceID);
			LocalMultiplayerSubsystem->AssignGamepadInputMapping(PlayerIndex, MappingType);
		}

		if (ULocalPlayer* LocalPlayer = GameInstancePtr->GetLocalPlayerByIndex(PlayerIndex))
		{
			if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(WorldPtr))
			{
				return PlayerController->InputAxis(Key, Delta, DeltaTime, NumSamples, false);
			}
		}
	}

	return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
}

