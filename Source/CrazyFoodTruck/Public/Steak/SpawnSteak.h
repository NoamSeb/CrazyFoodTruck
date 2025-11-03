// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorSteak.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Plate/PlateController.h"
#include "SpawnSteak.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API ASpawnSteak : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnSteak();

	UBoxComponent* CollisionMesh = nullptr;
	//TArray<UBoxComponent*> CollisionMesh;

	UFUNCTION(BlueprintCallable)
	void SpawnSteak();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActorSteak> SpawnedSteak = AActorSteak::StaticClass();

	UPROPERTY(EditAnywhere)
	AActor* PointSpawn;

	UPROPERTY(EditAnywhere)
	float Distance = 10;

	UPROPERTY(EditAnywhere)
	APlateController* Plate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
