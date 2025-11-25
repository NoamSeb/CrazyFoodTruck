// Fill out your copyright notice in the Description page of Project Settings.


#include "CrazyFoodTruck/Data/Public/GameInstanceCrazyFoodTruck.h"

#include "Kismet/GameplayStatics.h"

void UGameInstanceCrazyFoodTruck::Init()
{
	Super::Init();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("INIT Game Instance Crazy Food Truck"));

	FoodTruckData = GetSubsystem<UFoodTruckDataSubSystem>();
	GameData = GetSubsystem<UGameDataSubSystem>();
	ZombieData = GetSubsystem<UZombieDataSubSystem>();
	
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

void UGameInstanceCrazyFoodTruck::InitLobbySlots()
{
	PlayerSlots.Empty();
	PlayerSlots.SetNum(4);

	for (FMenuPlayerSlot& Slot : PlayerSlots)
	{
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

	for (FMenuPlayerSlot& Slot : PlayerSlots)
	{
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

	for (FMenuPlayerSlot& Slot : PlayerSlots)
	{
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
	for (FMenuPlayerSlot& Slot : PlayerSlots)
	{
		Slot.bIsConnected = false;
		Slot.bIsReady = false;
		Slot.ControllerId = INDEX_NONE;
	}

	OnLobbySlotsChanged.Broadcast();
}

void UGameInstanceCrazyFoodTruck::StartGameFromLobby()
{
	if (!AreAllPlayersConnected())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				3.f,
				FColor::Red,
				TEXT("Cannot start game: not all players are connected.")
			);
		}
		
		return;
	}

	if (GameLevelName.IsNone())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				3.f,
				FColor::Red,
				TEXT("GameLevelName is not set on GameInstanceCrazyFoodTruck.")
			);
		}
		
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	UGameplayStatics::OpenLevel(World, GameLevelName);
}
