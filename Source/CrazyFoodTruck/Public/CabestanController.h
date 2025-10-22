// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CabestanController.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API ACabestanController : public AActor
{
	GENERATED_BODY()

public:
	ACabestanController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
