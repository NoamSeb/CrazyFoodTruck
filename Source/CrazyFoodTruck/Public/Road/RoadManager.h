// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoadManager.generated.h"

class ARoad;

UCLASS()
class CRAZYFOODTRUCK_API ARoadManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARoadManager();
	UPROPERTY()
	TArray<TObjectPtr<ARoad>> RoadsSegments;
	
	UPROPERTY(EditAnywhere)
	int RoadSegmentsNumber;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SpawnRoadSegment();
	void RegulateRoadSegmentsPosition(TObjectPtr<ARoad> RoadToMove, int LoopStep);
};
