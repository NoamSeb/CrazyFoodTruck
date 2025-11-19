// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TransitionCamionUpgradePhase.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API ATransitionCamionUpgradePhase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATransitionCamionUpgradePhase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* FoodTruck;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
