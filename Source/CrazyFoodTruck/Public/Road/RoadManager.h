// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructLevels.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FStructLevels> Levels;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARoad> SurvivorCamp;
	
	UPROPERTY(EditAnywhere)
	int RoadSegmentsNumber;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SpawnRoadSegment();
	void UnloadRoadSegments();
	void RegulateRoadSegmentsPosition(TObjectPtr<ARoad> RoadToMove, int LoopStep);

	void SpawnTileToSurvivorCamp(FVector Location, FRotator Rotation);
	void GetRoadSegmentFromLevelDataTable(UClass* &RoadClass, UDataTable* &LevelDataTable, uint8* &RowData);

private:
	TArray<TObjectPtr<ARoad>> SpawnedRoadsSegments;
};
