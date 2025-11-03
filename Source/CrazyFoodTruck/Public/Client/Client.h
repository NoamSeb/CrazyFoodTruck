// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Food/ActorFood.h"
#include "Food/CuissonFood.h"
#include "GameFramework/Actor.h"
#include "Client.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API AClient : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AClient();

	UFUNCTION(BlueprintCallable)
	void VerifFoodIsGood(ECuissonFood CuissonFace, ECuissonFood CuissonPile);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasFood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActorFood* Food;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
