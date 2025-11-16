// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LocalMultiplayerSettings.h"
#include "UpgradePhase.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API AUpgradePhase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUpgradePhase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	ELocalMultiplayerInputMappingType MappingType = static_cast<ELocalMultiplayerInputMappingType>(0);

private:
	int32 GetPlayerIndexFromPlayerController(APlayerController* PlayerController) const;
};
