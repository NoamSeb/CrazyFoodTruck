// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PhaseGameFoodFoodTruck.h"
#include "Camera/CameraShakeManager.h"
#include "GameInstanceCrazyFoodTruck.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API UGameInstanceCrazyFoodTruck : public UGameInstance
{
	GENERATED_BODY()

	

	virtual void Init() override;

	
	public:

	// FUNCTION

	void SetCameraShakeManager(ACameraShakeManager* NewCameraShakeManager);
	void PlayerCameraShake(ECameraShake ShakeType);

	// VARIABLE 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool InGame;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LevelName;
	UPROPERTY(VisibleAnywhere)
	EPhaseGameCrazyFoodTruckState CurrentGamePhase;

private:
	ACameraShakeManager* CameraShakeManager;
};
