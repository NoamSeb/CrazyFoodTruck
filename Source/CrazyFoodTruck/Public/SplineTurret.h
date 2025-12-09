// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "SplineTurret.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API ASplineTurret : public AActor
{
	GENERATED_BODY()

public:
	ASplineTurret();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	float GetSplineLenght() const;

	UPROPERTY(EditAnywhere, Category="TurretParameters")
	USplineComponent* TurretSpline;

	UFUNCTION()
	void GetTurretLocationAlongSpline(float DistanceAlongSpline, FVector& OutLocation, FRotator& OutRotation) const;
};
