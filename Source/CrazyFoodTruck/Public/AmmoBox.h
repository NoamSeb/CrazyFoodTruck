// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAmmoEvent);

UCLASS()
class CRAZYFOODTRUCK_API AAmmoBox : public AActor
{
	GENERATED_BODY()

public:
	AAmmoBox();
	void Initialize();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable, Category="Ammo|Events")
	FOnAmmoEvent OnAmmoInitialize;

public:
	virtual void Tick(float DeltaTime) override;
};
