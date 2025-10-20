// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "HordeManager.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API AHordeManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHordeManager();
	//mettre le BP du zombie dans le BP de la horde
	UPROPERTY(EditAnywhere, Category= "Horde Manager | BP Zombie")
	TSubclassOf<ACharacter> PawnZombie = AActor::StaticClass();

	UFUNCTION(BlueprintCallable, Category = "Horde Manager | Spawn Horde")
	void SpawnHordeZombie(int nbrMin, int nbrMax);

	//garder les zombies dans la liste
	UPROPERTY(VisibleAnywhere, Category= "Horde Manager | Spawn Vague")
	TArray<ACharacter*> ListHordeZombie;

	//c'est une aide visuelle pour savoir où les limites où on place les zombies
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spawn Area")
	UBoxComponent* SpawnZone;

	//définir nombre max et min de zombies
	UPROPERTY(EditAnywhere, Category= "Horde Manager | Spawn Vague")
	int8 nbrMinZombies = 5;
	UPROPERTY(EditAnywhere, Category= "Horde Manager | Spawn Vague")
	int8 nbrMaxZombies = 10;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	private:
	void SetAreaSpawnZombie();
};
