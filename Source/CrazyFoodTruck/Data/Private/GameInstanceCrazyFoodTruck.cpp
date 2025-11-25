// Fill out your copyright notice in the Description page of Project Settings.


#include "CrazyFoodTruck/Data/Public/GameInstanceCrazyFoodTruck.h"

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
		TicketsByGrade.Add(EScoreGrade::F, 0);
	}
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

void UGameInstanceCrazyFoodTruck::AddTicketsForGrade(EScoreGrade Grade)
{
	if (int32* TicketsPtr = TicketsByGrade.Find(Grade))
	{
		nbrTickets += *TicketsPtr;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 5.f, FColor::Cyan,
				FString::Printf(
					TEXT("[Tickets] Grade=%d -> +%d tickets (total=%d)"),
					static_cast<int32>(Grade),
					*TicketsPtr,
					nbrTickets
				)
			);
		}
	}
}
