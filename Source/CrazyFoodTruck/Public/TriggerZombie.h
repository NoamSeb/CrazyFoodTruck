// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrazyFoodTruck/HordeManager.h"
#include "GameFramework/Actor.h"
#include "TriggerZombie.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API ATriggerZombie : public AActor
{
	GENERATED_BODY()

public:
	ATriggerZombie();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="Trigger Zombie")
	FWaveStructure DefaultWave;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trigger Zombie")
	TArray<FWaveStructure> SpawnWaves;

	UFUNCTION(BlueprintCallable, Category="Trigger Zombie")
	TArray<FWaveStructure> GetSpawnWaves() const { return SpawnWaves; }

protected:
	virtual void BeginPlay() override;
	// OVERLAP

	UPROPERTY()
	UBoxComponent* BoxComponent;

	void Initialize(AHordeManager* NewHordeManager);

	UFUNCTION(CallInEditor, Category="Trigger Zombie")
	void SpawnZone();
	void ClearSpawnArea();

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, Category="Trigger Zombie")
	TSubclassOf<AAreaZombieSpawn> AreaZombieSpawn;
	
private :
	UPROPERTY()
	AHordeManager* HordeManager;
	bool bHasTriggered = false;
};
