// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CuissonFood.h"
#include "ActorFood.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API AActorFood : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AActorFood();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsReturn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool OnGrille;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeGrilled = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeBurned = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECuissonFood CurrentCuissonFace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECuissonFood CurrentCuissonPile;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	float cuissonPile = 0.f;
	float cuissonFace = 0.f;
};
