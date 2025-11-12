// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Steak/ActorSteak.h"
#include "PlateController.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API APlateController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlateController();
	
	void ReceiveInputRightSteak(float value);
	void ReceiveInputLeftSteak(float value);
	void MovePlate(float value);

	void ReceiveInputRightSteak(bool value);
	void ReceiveInputLeftSteak(bool value);

	void ReturnSteak();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActorSteak* Steak;
	
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

	bool rightSteak = false;
	bool leftSteak = false;

	bool alreadyReturned = false;
};
