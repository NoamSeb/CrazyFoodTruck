// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "stdlib.h"
#include "time.h"
#include "Components/BoxComponent.h"
#include "HordeManager.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API AHordeManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHordeManager();
	UPROPERTY(EditAnywhere)
	TSubclassOf<ACharacter> PawnZombie = AActor::StaticClass();
	void SpawnHordeZombie();
	UPROPERTY(VisibleAnywhere)
	TArray<ACharacter*> ListHordeZombie;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawn Area")
	UBoxComponent* SpawnZone;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
