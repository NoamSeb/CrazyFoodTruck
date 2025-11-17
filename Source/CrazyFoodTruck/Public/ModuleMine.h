// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MineExplosive.h"
#include "ModuleBase.h"
#include "ModuleMine.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API AModuleMine : public AModuleBase
{
	GENERATED_BODY()

public:
	AModuleMine();
	UPROPERTY(editanywhere, Category="Module|Parameters")
	USceneComponent* SpawnMinePoint;

	UPROPERTY(editanywhere, Category="Module|Parameters")
	TSubclassOf<AMineExplosive> MinePrefab;
	
protected:
	virtual void BeginPlay() override;
	virtual void Interact(APlayerController* InstigatorPlayerController, ACrazyFoodTruckCharacter* CrazyCharacter) override;
	void DropMine();

	UFUNCTION(BlueprintCallable)
	void DropTestMine();

public:
	virtual void Tick(float DeltaTime) override;

private:

};
