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

	const int NbKeyboardProfiles = LocalMultiplayerSettings->GetNbKeyboardProfiles();
	const int NbMaxGamepads = LocalMultiplayerSettings->NbMaxGamepads;
	const int TargetPlayers = FMath::Max(1, NbKeyboardProfiles + NbMaxGamepads);

	while (GameInstance->GetLocalPlayers().Num() < TargetPlayers)
	{
		const int ControllerId = GameInstance->GetLocalPlayers().Num();
		FString OutError;
		GameInstance->CreateLocalPlayer(ControllerId, OutError, true);
	}

	LastAssignedPlayerIndex = 0;
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

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (!GameInstance->GetLocalPlayerByIndex(AssignedPlayerIndex))
		{
			FString OutError;
			GameInstance->CreateLocalPlayer(AssignedPlayerIndex, OutError, true);
		}
	}

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

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (!GameInstance->GetLocalPlayerByIndex(AssignedPlayerIndex))
		{
			FString OutError;
			GameInstance->CreateLocalPlayer(AssignedPlayerIndex, OutError, true);
		}
	}

	return AssignedPlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignKeyboardInputMapping(int PlayerIndex, int KeyboardProfileIndex, ELocalMultiplayerInputMappingType MappingType) const
{
	UInputMappingContext* IMC = GetIMCFromEnumType(MappingType);
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

UInputMappingContext* ULocalMultiplayerSubsystem::GetIMCFromEnumType(ELocalMultiplayerInputMappingType MappingType) const
{
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	return LocalMultiplayerSettings->GamepadProfileData.GetIMCFromType(MappingType);
}

void ULocalMultiplayerSubsystem::AssignGamepadInputMapping(int PlayerIndex, ELocalMultiplayerInputMappingType MappingType) const
{
	UInputMappingContext* IMC = GetIMCFromEnumType(MappingType);
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

	if (UEnhancedInputLocalPlayerSubsystem* EIS = GetEISForPlayerIndex(PlayerIndex))
	{

		if (!EIS->HasMappingContext(IMC))
		{
			FModifyContextOptions Options;
			Options.bForceImmediately = true;
			EIS->AddMappingContext(const_cast<UInputMappingContext*>(IMC), 0, Options);
		}else
		{
		}
	}

}

APlayerController* ULocalMultiplayerSubsystem::GetPlayerControllerForIndex(int PlayerIndex)
{
	ULocalPlayer* LP = GetLocalPlayerForIndex(PlayerIndex);
	if (!LP) return nullptr;

	if (UWorld* World = GetWorld())
	{
		return LP->GetPlayerController(World);
	}
	return nullptr;
}

ULocalPlayer* ULocalMultiplayerSubsystem::GetLocalPlayerForIndex(int PlayerIndex)
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (GI->GetLocalPlayers().IsValidIndex(PlayerIndex))
		{
			return GI->GetLocalPlayers()[PlayerIndex];
		}
	}
	return nullptr;
}

bool ULocalMultiplayerSubsystem::PossessPawnForPlayerIndex(int PlayerIndex, APawn* PawnToPossess, ELocalMultiplayerInputMappingType MappingType)
{
	if (!PawnToPossess) return false;
	APlayerController* PlayerController = GetPlayerControllerForIndex(PlayerIndex);
	if (!PlayerController) return false;
	PlayerController->Possess(PawnToPossess);
	AddTemporaryMappingForPlayer(PlayerIndex, GetIMCFromEnumType(MappingType));
	return true;
}

bool ULocalMultiplayerSubsystem::UnPossessPawnForPlayerIndex(int PlayerIndex, APawn* PlayerPawn, ELocalMultiplayerInputMappingType MappingType)
{
	APlayerController* PlayerController = GetPlayerControllerForIndex(PlayerIndex);
	if (!PlayerController) return false;
	PlayerController->UnPossess();
	PlayerController->Possess(PlayerPawn);
	RemoveTemporaryMappingForPlayer(PlayerIndex, GetIMCFromEnumType(MappingType));
	return true;
}

int ULocalMultiplayerSubsystem::GetPlayerIndexFromController(APlayerController* PlayerController) const
{
	if (!PlayerController) return -1;

	ULocalPlayer* LP = Cast<ULocalPlayer>(PlayerController->Player);
	if (!LP) -1;

	return LP->GetControllerId();
}



// MAXIME ADD


void ULocalMultiplayerSubsystem::AddTemporaryMappingForPlayer(int PlayerIndex, UInputMappingContext* IMC, int Priority,bool bForceImmediately)
{
	
	if (!IMC) return;

	if (UEnhancedInputLocalPlayerSubsystem* EIS = GetEISForPlayerIndex(PlayerIndex))
	{
		if (!EIS->HasMappingContext(IMC))
		{
			FModifyContextOptions Opt;
			Opt.bForceImmediately = bForceImmediately;
			EIS->AddMappingContext(IMC, Priority, Opt);
		}
	}
}

void ULocalMultiplayerSubsystem::RemoveTemporaryMappingForPlayer(int PlayerIndex, UInputMappingContext* IMC, bool bForceImmediately)
{
	if (!IMC) return;
	if (UEnhancedInputLocalPlayerSubsystem* EIS = GetEISForPlayerIndex(PlayerIndex))
	{
		if (EIS->HasMappingContext(IMC))
		{
			FModifyContextOptions Opt;
			Opt.bForceImmediately = bForceImmediately;
			EIS->RemoveMappingContext(IMC, Opt);
		}
	}
	AssignGamepadInputMapping(PlayerIndex, ELocalMultiplayerInputMappingType::InGame);
}


UEnhancedInputLocalPlayerSubsystem* ULocalMultiplayerSubsystem::GetEISForPlayerIndex(int PlayerIndex) const
{
	UGameInstance* GI = GetGameInstance();
	if (!GI) return nullptr;

	ULocalPlayer* LP = GI->GetLocalPlayerByIndex(PlayerIndex);
	if (!LP) return nullptr;

	return LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
}

UEnhancedInputLocalPlayerSubsystem* ULocalMultiplayerSubsystem::GetEISForPlayerController(APlayerController* PlayerController) const
{
	if (!PlayerController) return nullptr;

	ULocalPlayer* LP = Cast<ULocalPlayer>(PlayerController->Player);
	if (!LP) return nullptr;

	return LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
}
