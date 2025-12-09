// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CisorTurret.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API ACisorTurret : public AActor
{
	GENERATED_BODY()

public:
	ACisorTurret();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void TurretChangePosition(FVector NewPosition);
};