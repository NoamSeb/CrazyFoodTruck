// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CuissonSteak.h"
#include "ActorSteak.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API AActorSteak : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AActorSteak();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsReturn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool OnGrille;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TempsSteakCuit = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TempsSteakBrule = 20.f;

	UPROPERTY(EditAnywhere)
	ECuissonSteak CurrentCuissonFace;
	UPROPERTY(EditAnywhere)
	ECuissonSteak CurrentCuissonPile;

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
