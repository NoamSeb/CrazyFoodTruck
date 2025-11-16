// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CrazyFoodTruckHUD.generated.h"

class UZombieWidget;

UCLASS()
class CRAZYFOODTRUCK_API ACrazyFoodTruckHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UZombieWidget> ZombieWidgetClass;

private:
	UPROPERTY()
	UZombieWidget* ZombieWidgetInstance = nullptr;

	class AHordeManager* ResolveHordeManager() const;
};
