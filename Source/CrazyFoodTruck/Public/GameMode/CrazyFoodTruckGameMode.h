// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CrazyFoodTruckGameMode.generated.h"

class ACrazyFoodTruckCharacter;
class UCrazyFoodTruckCharacterInputData;
class UMatchTimerComponent;
class UScoreManagerComponent;
class AHordeManager;

class UInputMappingContext;
class APlayerStart;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinalScoreShown);
UCLASS()
class CRAZYFOODTRUCK_API ACrazyFoodTruckGameMode : public AGameModeBase
{
	GENERATED_BODY()
    
public:
	ACrazyFoodTruckGameMode();

	virtual void BeginPlay() override;

public:
	void ApplyGlobalViewTo(APlayerController* PC) const;
	void ApplyGlobalViewToAllPlayers() const;

	UFUNCTION(BlueprintCallable)
	void EvaluateFinalScore();

	UFUNCTION()
	void ListenScoreEnd();

	UPROPERTY(BlueprintAssignable, Category = "Score|Events")
	FOnFinalScoreShown OnFinalScoreShown;

protected:
	UPROPERTY()
	TArray<ACrazyFoodTruckCharacter*> Characters;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score", meta = (AllowPrivateAccess = "true"))
	UScoreManagerComponent* ScoreManager = nullptr;

	bool bHasComputedFinalScore = false;

	void CreateAndInitPlayers() const;
	void FindPlayerStartActors(TArray<APlayerStart*>& ResultsActors) const;
	void SpawnCharacters(const TArray<APlayerStart*>& SpawnPoints);

	UCrazyFoodTruckCharacterInputData* LoadInputDataFromConfig() const;
	UInputMappingContext* LoadInputMappingContextFromConfig() const;
	TSubclassOf<ACrazyFoodTruckCharacter> GetCrazyFoodTruckCharacterClassFromSlotIndex(int32 SlotIndex) const;
	APlayerController* FindPlayerControllerByControllerId(UWorld* World, int32 ControllerId) const;

	AActor* ResolveVehicleActor() const;
	AActor* ResolveViewTargetActor() const;

	void ConfigureMovementFrameForAllCharacters(AActor* Vehicle, AActor* ViewTarget);

	UFUNCTION()
	void HandleTimerSecondPrint(int32 ElapsedSeconds);

	static FString FormatMMSS(int32 TotalSeconds);


	AHordeManager* ResolveHordeManager() const;

private:
	UPROPERTY()
	TWeakObjectPtr<AActor> VehicleActorRef;

	UPROPERTY()
	TWeakObjectPtr<AActor> GlobalViewTarget;
};
