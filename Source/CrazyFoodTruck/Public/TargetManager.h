// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetManager.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API ATargetManager : public AActor
{
	GENERATED_BODY()

public:
	ATargetManager();

protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, Category="Target Parameters")
	APawn* TargetPawn;
	
	virtual void Tick(float DeltaTime) override;
};
