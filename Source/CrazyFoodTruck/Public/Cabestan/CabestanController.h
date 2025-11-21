// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurretController.h"
#include "GameFramework/Actor.h"
#include "CabestanController.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API ACabestanController : public AActor
{
	GENERATED_BODY()

public:
	ACabestanController();

	void ReceiveInputToward(float value);
	void ReceiveInputBackward(float value);
	void AddRotationInput(float value);

	UPROPERTY(EditAnywhere)
	ATurretController* LinkedTurretController;

protected:
	virtual void BeginPlay() override;


public:

	UPROPERTY(EditAnywhere)
	float AngleMax = 90.f;

	UPROPERTY(EditAnywhere)
	float AngleMin = -90.f;

	
	virtual void Tick(float DeltaTime) override;

	
	bool CanPush();
	bool CanBring();

private:
	UGameInstanceCrazyFoodTruck* GI;
	UFoodTruckDataSubSystem* TruckSubSystem;

	USceneComponent* RootCabestan;
	float RotationSpeed = 10.f;

	float CurrentRotationSpeed;
	
	float towardInput = 0.f;
	float backwardInput = 0.f;
};
