// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ZombieDataSubSystem.generated.h"

/**
 * 
 */
UCLASS()
class CRAZYFOODTRUCK_API UZombieDataSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float ZombieSpeed;

	UPROPERTY(EditAnywhere)
	int ZombieHealth;
};
