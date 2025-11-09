// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ESideTarget.h"
#include "GameFramework/Actor.h"
#include "TargetController.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API ATargetController : public AActor
{
	GENERATED_BODY()
ATargetController();

private:
	void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category="Target Parameters")
	APawn* TargetPawn;
	
	UPROPERTY(EditAnywhere, Category="Target Parameters")
	ESideTarget SideTarget = ESideTarget::Left;
	
	UPROPERTY(EditAnywhere, Category="Target Parameters")
	float DistanceLenght = 10.f;
	
	
};
