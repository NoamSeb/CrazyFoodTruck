// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModuleBase.h"
#include "ModuleExplosive.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API AModuleExplosive : public AModuleBase
{
	GENERATED_BODY()

public:
	AModuleExplosive();

	UPROPERTY(EditAnywhere)
	float ExplosionRadius = 300.f;
	int ExplosionDamage = 10;
	
protected:
	virtual void BeginPlay() override;
	virtual void Interact(APlayerController* InstigatorPlayerController, ACrazyFoodTruckCharacter* CrazyCharacter) override;

private:
	void Explode();
	bool bCanExplode = true;

public:
	virtual void Tick(float DeltaTime) override;
};
