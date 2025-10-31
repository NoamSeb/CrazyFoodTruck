// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Plate/PlateController.h"
#include "SpawnBread.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API ASpawnBread : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnBread();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActorFood> PrefabBread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlateController* PlateController;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//virtual void Interact(APlayerController* InstigatorPlayerController, ACrazyFoodTruckCharacter* CrazyCharacter) = 0;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
