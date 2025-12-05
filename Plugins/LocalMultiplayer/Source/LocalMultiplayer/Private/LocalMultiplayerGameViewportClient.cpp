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
	
	MaxSplitscreenPlayers = 3;
}

bool ULocalMultiplayerGameViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	if (!EventArgs.Key.IsGamepadKey())
	{
		return Super::InputKey(EventArgs);
	}

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

	const ELocalMultiplayerInputMappingType MappingType = LocalMultiplayerSubsystem->GetCurrentMappingType();

	const int DeviceID = EventArgs.InputDevice.GetId();

	int PlayerIndex = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromGamepadDeviceID(DeviceID);
	const bool bWasAssigned = (PlayerIndex != -1);
	if (!bWasAssigned)
	{
		PlayerIndex = LocalMultiplayerSubsystem->AssignNewPlayerToGamepadDeviceID(DeviceID);
		if (PlayerIndex == -1)
		{
			return Super::InputKey(EventArgs);
		}

		LocalMultiplayerSubsystem->AssignGamepadInputMapping(PlayerIndex, MappingType);
	}

	if (ULocalPlayer* LocalPlayer = GameInstancePtr->GetLocalPlayerByIndex(PlayerIndex))
	{
		if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(WorldPtr))
		{
			const float AmountDepressed = (EventArgs.Event == IE_Released) ? 0.f : 1.f;
			return PlayerController->InputKey(EventArgs.Key, EventArgs.Event, AmountDepressed, true);
		}
	}

	return Super::InputKey(EventArgs);
}

bool ULocalMultiplayerGameViewportClient::InputAxis(FViewport* InViewport, FInputDeviceId InputDevice, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	if (!bGamepad && !Key.IsGamepadKey())
	{
		return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
	}

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

	const ELocalMultiplayerInputMappingType MappingType = LocalMultiplayerSubsystem->GetCurrentMappingType();

	const int DeviceID = InputDevice.GetId();

	int PlayerIndex = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromGamepadDeviceID(DeviceID);
	const bool bWasAssigned = (PlayerIndex != -1);
	if (!bWasAssigned)
	{
		PlayerIndex = LocalMultiplayerSubsystem->AssignNewPlayerToGamepadDeviceID(DeviceID);
		if (PlayerIndex == -1)
		{
			return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
		}

		LocalMultiplayerSubsystem->AssignGamepadInputMapping(PlayerIndex, MappingType);
	}

	if (ULocalPlayer* LocalPlayer = GameInstancePtr->GetLocalPlayerByIndex(PlayerIndex))
	{
		if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(WorldPtr))
		{
			return PlayerController->InputAxis(Key, Delta, DeltaTime, NumSamples, true);
		}
	}

	return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
}
