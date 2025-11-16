// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrazyFoodTruck/Data/Public/GameInstanceCrazyFoodTruck.h"
#include "Interface/IEntity.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MineExplosive.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API AMineExplosive : public AActor
{
	GENERATED_BODY()

public:
	AMineExplosive();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
private:

	UFUNCTION(BlueprintCallable, Category="Explosive")
	void Explode();
	UPROPERTY(EditAnywhere, Category = "Module|Parameters")
	float ExplosionRadius = 400.f;
	UPROPERTY(EditAnywhere, Category ="Module|Parameters")
	int ExplosionDamage = 10;
};
