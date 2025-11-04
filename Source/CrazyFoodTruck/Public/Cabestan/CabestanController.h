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
	virtual void Tick(float DeltaTime) override;

private:

	USceneComponent* RootCabestan;

	
	float towardInput = 0.f;
	float backwardInput = 0.f;
};
