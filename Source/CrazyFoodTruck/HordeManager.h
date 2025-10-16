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
	//mettre le BP du zombie dans le BP de la horde
	UPROPERTY(EditAnywhere)
	TSubclassOf<ACharacter> PawnZombie = AActor::StaticClass();

	void SpawnHordeZombie();

	//garder les zombies dans la liste
	UPROPERTY(VisibleAnywhere)
	TArray<ACharacter*> ListHordeZombie;

	//c'est une aide visuelle pour savoir où les limites où on place les zombies
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawn Area")
	UBoxComponent* SpawnZone;

	//définir nombre max et min de zombies
	UPROPERTY(EditAnywhere)
	int8 nbrMinZombies = 5;
	UPROPERTY(EditAnywhere)
	int8 nbrMaxZombies = 10;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
