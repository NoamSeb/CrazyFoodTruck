// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LobbyDebugActor.generated.h"

class UGameInstanceCrazyFoodTruck;

UCLASS()
class CRAZYFOODTRUCK_API ALobbyDebugActor : public AActor
{
	GENERATED_BODY()

public:
	ALobbyDebugActor();

protected:
#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = "Debug|Lobby")
	void DebugConnectPlayer0();

	UFUNCTION(CallInEditor, Category = "Debug|Lobby")
	void DebugConnectPlayer1();

	UFUNCTION(CallInEditor, Category = "Debug|Lobby")
	void DebugConnectPlayer2();

	UFUNCTION(CallInEditor, Category = "Debug|Lobby")
	void DebugConnectPlayer3();
#endif

private:
#if WITH_EDITOR
	void DebugTogglePlayerSlot(int32 ControllerId) const;
#endif
};
