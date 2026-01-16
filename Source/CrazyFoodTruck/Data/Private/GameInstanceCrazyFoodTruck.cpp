// Fill out your copyright notice in the Description page of Project Settings.


#include "CrazyFoodTruck/Data/Public/GameInstanceCrazyFoodTruck.h"

#include "LocalMultiplayerSettings.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"

#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

void UGameInstanceCrazyFoodTruck::Init()
{
	Super::Init();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("INIT Game Instance Crazy Food Truck"));

	FoodTruckData = GetSubsystem<UFoodTruckDataSubSystem>();
	GameData = GetSubsystem<UGameDataSubSystem>();
	ZombieData = GetSubsystem<UZombieDataSubSystem>();
	
	if (TicketsByGrade.Num() == 0)
	{
		TicketsByGrade.Add(EScoreGrade::S, 10);
		TicketsByGrade.Add(EScoreGrade::A, 7);
		TicketsByGrade.Add(EScoreGrade::B, 5);
		TicketsByGrade.Add(EScoreGrade::C, 3);
		TicketsByGrade.Add(EScoreGrade::D, 2);
		TicketsByGrade.Add(EScoreGrade::E, 1);
		TicketsByGrade.Add(EScoreGrade::F, 5);
	}

	InitLobbySlots();
}

void UGameInstanceCrazyFoodTruck::SetCameraShakeManager(ACameraShakeManager* NewCameraShakeManager)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Set Camera Shake Manager in Game Instance"));
	CameraShakeManager = NewCameraShakeManager;
}

void UGameInstanceCrazyFoodTruck::PlayerCameraShake(ECameraShake ShakeType)
{
	if (CameraShakeManager)
	{
		CameraShakeManager->PlayShake(ShakeType);
	}
}

TArray<FStructUpgrade> UGameInstanceCrazyFoodTruck::ListUpgradesToAdd(EZoneUpgrade ZoneUpgrade)
{
	TArray<FStructUpgrade> ListUpgradesToReturn;
	
	for (FStructUpgrade Upgrade : ListUpgrades)
	{
		if (Upgrade.Zone == ZoneUpgrade)
		{
			ListUpgradesToReturn.Add(Upgrade);
		}
	}

	return ListUpgradesToReturn;
}

void UGameInstanceCrazyFoodTruck::ClearUpgrades()
{
	ListUpgrades.Empty();
}

int UGameInstanceCrazyFoodTruck::GetTicketsNumber() const
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Get Tickets Number: %d"), nbrTickets));
	return nbrTickets;
}

int UGameInstanceCrazyFoodTruck::IncrementTicketsNumber(int Amount)
{
	nbrTickets += Amount;
	return nbrTickets;
}

int UGameInstanceCrazyFoodTruck::DecrementTicketsNumber(int Amount)
{
	GameData->NbrTicketsUse += Amount;
	nbrTickets -= Amount;
	return nbrTickets;
}

int32 UGameInstanceCrazyFoodTruck::AddTicketsForGrade(EScoreGrade Grade)
{
	if (int32* TicketsPtr = TicketsByGrade.Find(Grade))
	{
		IncrementTicketsNumber(*TicketsPtr);
		return *TicketsPtr;
	}
	return 0;
}

void UGameInstanceCrazyFoodTruck::InitLobbySlots()
{
	PlayerSlots.Empty();
	PlayerSlots.SetNum(3);

	for (int32 i = 0; i < PlayerSlots.Num(); ++i)
	{
		FMenuPlayerSlot& Slot = PlayerSlots[i];
		Slot.bIsConnected = false;
		Slot.bIsReady = false;
		Slot.ControllerId = INDEX_NONE;
	}

	OnLobbySlotsChanged.Broadcast();
}

void UGameInstanceCrazyFoodTruck::TryJoinPlayer(int32 ControllerId)
{
	if (ControllerId == INDEX_NONE)
	{
		return;
	}

	for (int32 SlotIndex = 0; SlotIndex < PlayerSlots.Num(); ++SlotIndex)
	{
		FMenuPlayerSlot& Slot = PlayerSlots[SlotIndex];
		if (Slot.ControllerId == ControllerId)
		{
			if (!Slot.bIsConnected)
			{
				Slot.bIsConnected = true;
				Slot.bIsReady = true;
				OnLobbySlotsChanged.Broadcast();
			}
			
			return;
		}
	}

	for (int32 SlotIndex = 0; SlotIndex < PlayerSlots.Num(); ++SlotIndex)
	{
		FMenuPlayerSlot& Slot = PlayerSlots[SlotIndex];
		if (!Slot.bIsConnected)
		{
			Slot.bIsConnected = true;
			Slot.bIsReady = true;
			Slot.ControllerId = ControllerId;

			OnLobbySlotsChanged.Broadcast();
			return;
		}
	}
}

bool UGameInstanceCrazyFoodTruck::AreAllPlayersConnected() const
{
	for (const FMenuPlayerSlot& Slot : PlayerSlots)
	{
		if (!Slot.bIsConnected)
		{
			return false;
		}
	}
	
	return true;
}

void UGameInstanceCrazyFoodTruck::ResetLobby()
{
	for (int32 i = 0; i < PlayerSlots.Num(); ++i)
	{
		FMenuPlayerSlot& Slot = PlayerSlots[i];
		Slot.bIsConnected = false;
		Slot.bIsReady = false;
		Slot.ControllerId = INDEX_NONE;
	}

	OnLobbySlotsChanged.Broadcast();
}

FLinearColor UGameInstanceCrazyFoodTruck::GetPlayerColorForIndex(int32 PlayerIndex) const
{
	const ULocalMultiplayerSettings* LocalSettings = GetDefault<ULocalMultiplayerSettings>();
	if (!LocalSettings)
	{
		return FLinearColor::White;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return FLinearColor::White;
	}

	UMaterialParameterCollection* MPC = LocalSettings->MPCOutline.LoadSynchronous();
	if (!MPC)
	{
		return FLinearColor::White;
	}

	const int32 OneBasedIndex = PlayerIndex + 1;
	const FName ParamName(*FString::Printf(TEXT("OutlineColor_P%d"), OneBasedIndex));

	const FLinearColor Color = UKismetMaterialLibrary::GetVectorParameterValue(World, MPC, ParamName);

	return Color;
}

int32 UGameInstanceCrazyFoodTruck::GetSlotIndexForControllerId(int32 ControllerId) const
{
	if (ControllerId == INDEX_NONE)
	{
		return INDEX_NONE;
	}

	for (int32 SlotIndex = 0; SlotIndex < PlayerSlots.Num(); ++SlotIndex)
	{
		const FMenuPlayerSlot& Slot = PlayerSlots[SlotIndex];
		if (Slot.bIsConnected && Slot.ControllerId == ControllerId)
		{
			return SlotIndex;
		}
	}

	return INDEX_NONE;
}

FLinearColor UGameInstanceCrazyFoodTruck::GetPlayerColorForControllerId(int32 ControllerId) const
{
	const int32 SlotIndex = GetSlotIndexForControllerId(ControllerId);
	if (SlotIndex == INDEX_NONE)
	{
		return FLinearColor::White;
	}

	const FLinearColor Color = GetPlayerColorForIndex(SlotIndex);

	return Color;
}
