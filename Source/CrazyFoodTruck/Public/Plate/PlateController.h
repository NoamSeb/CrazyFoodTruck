// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlateController.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API APlateController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlateController();
	
	void ReceiveInputRight(float value);
	void ReceiveInputLeft(float value);
	void MovePlate(float value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasSteak = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanMove = true;

	UPROPERTY(EditAnywhere)
	AActor* PlateActor;

	UPROPERTY(EditAnywhere)
	float Speed = 300;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	float rightInput = 0.f;
	float leftInput = 0.f;
};
