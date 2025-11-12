// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrazyFoodTruck/Data/Public/GameInstanceCrazyFoodTruck.h"
#include "ShakeController.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRAZYFOODTRUCK_API UShakeController : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UShakeController();

	UFUNCTION(blueprintCallable, Category="Shake")
	void StartShake(ECameraShake ShakeType);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	UGameInstanceCrazyFoodTruck* _GameInstanceCrazyFoodTruck;
};
