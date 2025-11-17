// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmunitionSpawner.h"
#include "TurretController.h"
#include "GameFramework/Actor.h"
#include "Interactable/Interactable.h"
#include "AmmoDeposit.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDepositEvent, AAmmoBox*, SpawnedAmmoBox);

UCLASS()
class CRAZYFOODTRUCK_API AAmmoDeposit : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	AAmmoDeposit();
	UPROPERTY(EditAnywhere, Category="Ammo")
	AAmmunitionSpawner* LinkedAmmoSpawner;

	UPROPERTY(EditAnywhere, Category="Ammo")
	ATurretController* LinkedTurretController;


	UPROPERTY(BlueprintAssignable, Category="Ammo|Events")
	FOnDepositEvent OnAmmoDeposited;

protected:
	virtual void BeginPlay() override;
	virtual void Interact(APlayerController* InstigatorPlayerController, ACrazyFoodTruckCharacter* CrazyCharacter) override;

	UFUNCTION()
	void TurretAmmoEmpty();

private:
	// void Reload();

	USceneComponent* DepositPoint;

	AAmmoBox* ReloadedAmmoBox;
public:
	virtual void Tick(float DeltaTime) override;
};
