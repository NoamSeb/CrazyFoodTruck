// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IEntity.h"
#include "GameFramework/Actor.h"
#include "EntityTest.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API AEntityTest : public AActor, public IIEntity
{
	GENERATED_BODY()

public:
	AEntityTest();

protected:
	virtual void BeginPlay() override;
	virtual void ReceiveDamage(int8 DamageAmount) override;

public:
	
	virtual void Tick(float DeltaTime) override;
};
