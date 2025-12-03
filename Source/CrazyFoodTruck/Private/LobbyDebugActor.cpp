// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyDebugActor.h"

#include "CrazyFoodTruck/Data/Public/GameInstanceCrazyFoodTruck.h"

ALobbyDebugActor::ALobbyDebugActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

#if WITH_EDITOR

void ALobbyDebugActor::DebugTogglePlayerSlot(int32 ControllerId) const
{
	if (!GetWorld())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 2.f, FColor::Red,
				TEXT("[LobbyDebugActor] No World")
			);
		}
		
		return;
	}

	UGameInstance* GIBase = GetWorld()->GetGameInstance();
	if (!GIBase)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 2.f, FColor::Red,
				TEXT("[LobbyDebugActor] No GameInstance")
			);
		}
		
		return;
	}

	if (UGameInstanceCrazyFoodTruck* GI = Cast<UGameInstanceCrazyFoodTruck>(GIBase))
	{
		GI->TryJoinPlayer(ControllerId);

		if (GEngine)
		{
			const FString Msg = FString::Printf(
				TEXT("[LobbyDebugActor] Toggled lobby slot for ControllerId = %d"),
				ControllerId
			);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, Msg);
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 2.f, FColor::Red,
				TEXT("[LobbyDebugActor] GameInstance is not UGameInstanceCrazyFoodTruck")
			);
		}
	}
}

void ALobbyDebugActor::DebugConnectPlayer0()
{
	DebugTogglePlayerSlot(0);
}

void ALobbyDebugActor::DebugConnectPlayer1()
{
	DebugTogglePlayerSlot(1);
}

void ALobbyDebugActor::DebugConnectPlayer2()
{
	DebugTogglePlayerSlot(2);
}

void ALobbyDebugActor::DebugConnectPlayer3()
{
	DebugTogglePlayerSlot(3);
}

#endif
