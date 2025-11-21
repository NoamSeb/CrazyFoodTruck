// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/IEntity.h"
#include "GameFramework/Actor.h"
#include "EntityTest.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLifeChange, int, NewLife);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEvent);

UCLASS()
class CRAZYFOODTRUCK_API AEntityTest : public AActor, public IIEntity
{
	GENERATED_BODY()

public:
	AEntityTest();
	UFUNCTION(BlueprintCallable)
	int GetHealth() const {return Health;}

protected:
	
	virtual void BeginPlay() override;

	virtual void CrushUnderTruck() override;
	
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnLifeChange OnLifeChange;
	FOnEvent OnDeath;

public:
	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Stats")
	int Health = 5;
};
