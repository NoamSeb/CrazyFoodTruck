// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CrazyFoodTruckGameMode.generated.h"

class ACrazyFoodTruckCharacter;
class UCrazyFoodTruckCharacterInputData;

class UInputMappingContext;

UCLASS()
class CRAZYFOODTRUCK_API ACrazyFoodTruckGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	TArray<ACrazyFoodTruckCharacter*> Characters;

private:
	void CreateAndInitPlayers() const;

	void FindPlayerStartActors(TArray<APlayerStart*>& ResultsActors) const;
	void SpawnCharacters(const TArray<APlayerStart*>& SpawnPoints);

	UCrazyFoodTruckCharacterInputData* LoadInputDataFromConfig() const;
	UInputMappingContext* LoadInputMappingContextFromConfig() const;

	TSubclassOf<ACrazyFoodTruckCharacter> GetCrazyFoodTruckCharacterClassFromInputType(EAutoReceiveInput::Type InputType) const;

	AActor* FindTruckActor() const;
	
	void ForceTruckCameraForAllPlayers(AActor* TruckActor) const;
	void ForceTruckCameraFor(APlayerController* PlayerController, AActor* TruckActor) const;
};
