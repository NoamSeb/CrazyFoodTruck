// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "GameFramework/Actor.h"
#include "CrazyFoodTruck/Data/Public/GameInstanceCrazyFoodTruck.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interface/IShootable.h"
#include "MineExplosive.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API AMineExplosive : public AActor, public IIShootable
{
	GENERATED_BODY()

public:
	AMineExplosive();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Components")
	UNiagaraSystem* ExplosionEffect;

public:
	virtual void ReceiveDamage(int DamageAmount) override;
	virtual void Tick(float DeltaTime) override;
private:

	UFUNCTION(BlueprintCallable, Category="Explosive")
	void Explode();
	UPROPERTY(EditAnywhere, Category = "Module|Parameters")
	float ExplosionRadius = 400.f;
	UPROPERTY(EditAnywhere, Category ="Module|Parameters")
	int ExplosionDamage = 10;
	UPROPERTY(EditAnywhere, Category = "Module|Parameters")
	USoundBase* BoomSound = nullptr;
	bool bHasExploded = false;
};
