// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CrazyFoodTruckGameMode.generated.h"

class ACrazyFoodTruckCharacter;
class UCrazyFoodTruckCharacterInputData;

class UInputMappingContext;
class APlayerStart;

UCLASS()
class CRAZYFOODTRUCK_API ACrazyFoodTruckGameMode : public AGameModeBase
{
	GENERATED_BODY()
    
public:
	virtual void BeginPlay() override;

public:
	void ApplyGlobalViewTo(APlayerController* PC) const;
	void ApplyGlobalViewToAllPlayers() const;

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

	AActor* ResolveVehicleActor() const;
	AActor* ResolveViewTargetActor() const;

	void ConfigureMovementFrameForAllCharacters(AActor* Vehicle);

private:
	UPROPERTY()
	TWeakObjectPtr<AActor> VehicleActorRef;

	UPROPERTY()
	TWeakObjectPtr<AActor> GlobalViewTarget;
};
