// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSubsystem.h"

#include "EnhancedInputSubsystems.h"

#include "InputMappingContext.h"

#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

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
	UInputMappingContext* IMC = GetKeyboardIMC(KeyboardProfileIndex, MappingType);
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
			EIS->AddMappingContext(IMC, 0, Options);
		}
	}
}

void ULocalMultiplayerSubsystem::AssignGamepadInputMapping(int PlayerIndex, ELocalMultiplayerInputMappingType MappingType) const
{
	UInputMappingContext* IMC = GetGamepadIMC(MappingType);
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
			EIS->AddMappingContext(IMC, 0, Options);
		}
	}
}

void ULocalMultiplayerSubsystem::EnsurePlayerIMCs(ELocalMultiplayerInputMappingType MappingType)
{
	UGameInstance* GI = GetGameInstance();
	if (!GI) return;

	const TArray<ULocalPlayer*>& LPs = GI->GetLocalPlayers();
	for (int32 PlayerIndex = 0; PlayerIndex < LPs.Num(); ++PlayerIndex)
	{
		if (UEnhancedInputLocalPlayerSubsystem* EIS = GetEISForPlayerIndex(PlayerIndex))
		{
			if (UInputMappingContext* IMC = GetGamepadIMC(MappingType))
			{
				if (!EIS->HasMappingContext(IMC))
				{
					FModifyContextOptions Options;
					Options.bForceImmediately = true;
					EIS->AddMappingContext(IMC, 0, Options);
				}
			}
		}
	}
}

APlayerController* ULocalMultiplayerSubsystem::GetPlayerControllerForIndex(int32 PlayerIndex)
{
	ULocalPlayer* LP = GetLocalPlayerForIndex(PlayerIndex);
	if (!LP)
	{
		return nullptr;
	}

	if (UWorld* World = GetWorld())
	{
		return LP->GetPlayerController(World);
	}
	
	return nullptr;
}

ULocalPlayer* ULocalMultiplayerSubsystem::GetLocalPlayerForIndex(int32 PlayerIndex)
{
	if (UGameInstance* GI = GetGameInstance())
	{
		return GI->GetLocalPlayerByIndex(PlayerIndex);
	}
	
	return nullptr;
}

int32 ULocalMultiplayerSubsystem::GetPlayerIndexFromController(APlayerController* PlayerController) const
{
	if (!PlayerController)
	{
		return -1;
	}

	if (ULocalPlayer* LP = Cast<ULocalPlayer>(PlayerController->Player))
	{
		return LP->GetControllerId();
	}
	
	return -1;
}

bool ULocalMultiplayerSubsystem::PossessPawnForPlayerIndex(int32 PlayerIndex, APawn* PawnToPossess, ELocalMultiplayerInputMappingType MappingType)
{
	if (!PawnToPossess)
	{
		return false;
	}

	APlayerController* PC = GetPlayerControllerForIndex(PlayerIndex);
	if (!PC)
	{
		return false;
	}

	PC->Possess(PawnToPossess);
	
	UMeshComponent* PawnMesh = PawnToPossess->FindComponentByClass<UMeshComponent>();
	if(PawnMesh)
	{
		PawnMesh->SetRenderCustomDepth(true);
		PawnMesh->SetCustomDepthStencilWriteMask(ERendererStencilMask::ERSM_255);
		FString ParamName = FString::Printf(TEXT("Bit_P%d"), PlayerIndex+1);
		FName ParamFName(*ParamName);
		
		UMaterialParameterCollection* MPC = LoadObject<UMaterialParameterCollection>(nullptr, TEXT("/Game/CrazyFoodTruck/Visuals/VFX/Outline/MPC_Outline"));
		if (!MPC)
			UE_LOG(LogTemp, Error, TEXT("MPC Not found !"));

		UMaterialParameterCollectionInstance* MPCInstance = GetWorld()->GetParameterCollectionInstance(MPC);
		if (!MPCInstance)
			UE_LOG(LogTemp, Error, TEXT("MPCInstance Not found !"));
		
		float ScalarValue = UKismetMaterialLibrary::GetScalarParameterValue(GetWorld(), MPC, ParamFName);
		
		int32 StencilValue = FMath::TruncToInt(ScalarValue);
		
		PawnMesh->SetCustomDepthStencilValue(StencilValue);
	}

	if (UInputMappingContext* IMC = GetGamepadIMC(MappingType))
	{
		AddTemporaryMappingForPlayer(PlayerIndex, IMC, 100, true);
	}
	
	return true;
}

bool ULocalMultiplayerSubsystem::UnPossessPawnForPlayerIndex(int32 PlayerIndex, APawn* PlayerPawn, ELocalMultiplayerInputMappingType MappingType)
{
	APlayerController* PC = GetPlayerControllerForIndex(PlayerIndex);

	if (!PC)
	{
		return false;
	}
	UMeshComponent* PawnToUnPossessMesh = PC->FindComponentByClass<UMeshComponent>();
	
	if(PawnToUnPossessMesh)
		PawnToUnPossessMesh->SetRenderCustomDepth(false);
	
	PC->UnPossess();
	if (PlayerPawn)
	{
		PC->Possess(PlayerPawn);
	}

	if (UInputMappingContext* IMC = GetGamepadIMC(MappingType))
	{
		RemoveTemporaryMappingForPlayer(PlayerIndex, IMC, true);
	}

	return true;
}

void ULocalMultiplayerSubsystem::AddTemporaryMappingForPlayer(int32 PlayerIndex, UInputMappingContext* IMC, int32 Priority, bool bForceImmediately)
{
	if (!IMC)
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* EIS = GetEISForPlayerIndex(PlayerIndex))
	{
		if (!EIS->HasMappingContext(IMC))
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "GOOD MAP");
			FModifyContextOptions Options;
			Options.bForceImmediately = bForceImmediately;
			EIS->AddMappingContext(IMC, Priority, Options);
		}
	}
}

void ULocalMultiplayerSubsystem::RemoveTemporaryMappingForPlayer(int32 PlayerIndex, UInputMappingContext* IMC, bool bForceImmediately)
{
	if (!IMC)
	{
		return;
	}
	// PRINT IMC NAME
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("REMOVING IMC : %s"), *IMC->GetName()));
	if (UEnhancedInputLocalPlayerSubsystem* EIS = GetEISForPlayerIndex(PlayerIndex))
	{

		if (EIS->HasMappingContext(IMC))
		{
			FModifyContextOptions Options;
			Options.bForceImmediately = bForceImmediately;
			EIS->RemoveMappingContext(IMC, Options);
		}
	}
}

UEnhancedInputLocalPlayerSubsystem* ULocalMultiplayerSubsystem::GetEISForPlayerIndex(int32 PlayerIndex) const
{
	UGameInstance* GI = GetGameInstance();
	if (!GI)
	{
		return nullptr;
	}

	ULocalPlayer* LP = GI->GetLocalPlayerByIndex(PlayerIndex);
	if (!LP)
	{
		return nullptr;
	}

	return LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
}

UInputMappingContext* ULocalMultiplayerSubsystem::GetKeyboardIMC(int32 KeyboardProfileIndex, ELocalMultiplayerInputMappingType MappingType) const
{
	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
	if (!Settings)
	{
		return nullptr;
	}

	if (!Settings->KeyboardProfilesData.IsValidIndex(KeyboardProfileIndex))
	{
		return nullptr;
	}

	return Settings->KeyboardProfilesData[KeyboardProfileIndex].GetIMCFromType(MappingType);
}

UInputMappingContext* ULocalMultiplayerSubsystem::GetGamepadIMC(ELocalMultiplayerInputMappingType MappingType) const
{
	const ULocalMultiplayerSettings* Settings = GetDefault<ULocalMultiplayerSettings>();
	if (!Settings)
	{
		return nullptr;
	}

	return Settings->GamepadProfileData.GetIMCFromType(MappingType);
}
