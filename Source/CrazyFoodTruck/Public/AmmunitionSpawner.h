// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmoBox.h"
#include "Components/BoxComponent.h"
#include "Characters/CrazyFoodTruckCharacter.h"
#include "GameFramework/Actor.h"
#include "Interactable/Interactable.h"
#include "AmmunitionSpawner.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoEv, AAmmoBox*, SpawnedAmmoBox);

UCLASS()
class CRAZYFOODTRUCK_API AAmmunitionSpawner : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAmmunitionSpawner();

	void SetSpawnState(bool bCanSpawnAmmo);
	void SetCanTakeAmmo(bool bCanTake);
	UPROPERTY(EditAnywhere, Category="Ammo")
	TSubclassOf<AAmmoBox> AmmoPrefab;

	UPROPERTY(BlueprintAssignable, Category="Ammo|Events")
	FOnAmmoEv OnAmmoSpawned;
	
protected:
	virtual void BeginPlay() override;

	

	UBoxComponent* BoxComponent;
	
	UPROPERTY()
	USceneComponent* SpawnPoint;

	void SpawnAmmo();
	AAmmoBox* TakeAmmo();

	// interface
	
	virtual void Interact(APlayerController* InstigatorPlayerController, ACrazyFoodTruckCharacter* CrazyCharacter) override;

private:

	bool bCanSpawn = true;
	bool AmmoOnHolder = false;
	bool canTake = true;
	float RespawnTime = 5.f;
	float CurrentRespawnTimer = 0.f;
	float CurrentAmmoTakeTimer = 0.f;

	AAmmoBox* AmmoBoxSpawned;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
