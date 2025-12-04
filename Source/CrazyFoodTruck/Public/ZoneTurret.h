// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZoneTurret.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API AZoneTurret : public AActor
{
	GENERATED_BODY()

public:
	AZoneTurret();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
