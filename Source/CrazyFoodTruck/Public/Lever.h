// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lever.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API ALever : public AActor
{
	GENERATED_BODY()

public:
	ALever();

	UFUNCTION(BlueprintImplementableEvent, Category="Lever")
	void SetStateCooldown(float alpha);

protected:
	virtual void BeginPlay() override;
	

public:
	virtual void Tick(float DeltaTime) override;
};
