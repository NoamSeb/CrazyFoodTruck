// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Editor.h"
//#include "Editor/EditorEngine.h"
#include "PlateauCuisine.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API APlateauCuisine : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlateauCuisine();

	UFUNCTION(CallInEditor, BlueprintImplementableEvent)
	void MovePlateau();
	UFUNCTION(CallInEditor, BlueprintImplementableEvent)
	void StopMovePlateau();
	UFUNCTION(CallInEditor, BlueprintImplementableEvent)
	void ReverseMovePlateau();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
