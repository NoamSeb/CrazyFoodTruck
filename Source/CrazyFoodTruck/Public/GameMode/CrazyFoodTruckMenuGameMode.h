// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CrazyFoodTruckMenuGameMode.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API ACrazyFoodTruckMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ACrazyFoodTruckMenuGameMode();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<AActor> MenuCameraActor;
};
