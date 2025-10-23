// Fill out your copyright notice in the Description page of Project Settings.


#include "Road/RoadManager.h"

#include "MeshPaintVisualize.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Road/Road.h"


// Sets default values
ARoadManager::ARoadManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARoadManager::BeginPlay()
{
	Super::BeginPlay();
	SpawnRoadSegment();
}

// Called every frame
void ARoadManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARoadManager::SpawnRoadSegment()
{
	RoadsSegments.Empty();
	
	FString LocalFolderPath = TEXT("/Game/CrazyFoodTruck/Blueprint/Road/Segments");
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	TArray<FAssetData> AssetDataList;
	AssetRegistryModule.Get().GetAssetsByPath(*LocalFolderPath, AssetDataList, true);

	FVector SpawnLocation = FVector::ZeroVector;
	FRotator SpawnRotation = FRotator::ZeroRotator;
	
	// Nom à exclure du tirage aléatoire
	const FString ExcludedName = TEXT("BP_SurvivorCamp");
	
	for (int i = 0; i < RoadSegmentsNumber; ++i)
	{
		FAssetData AssetData;
		int32 RandomIndex = -1;

		// On boucle jusqu’à trouver un asset qui n’est pas exclu
		do
		{
			RandomIndex = FMath::RandRange(0, AssetDataList.Num() - 1);
			AssetData = AssetDataList[RandomIndex];
		}
		while (AssetData.AssetName.ToString() == ExcludedName);

		// On charge et spawn le Blueprint sélectionné
		TObjectPtr<UObject> LoadedObject = AssetData.GetAsset();
		TObjectPtr<UBlueprint> BlueprintAsset = Cast<UBlueprint>(LoadedObject);

		if (BlueprintAsset && BlueprintAsset->GeneratedClass->IsChildOf(ARoad::StaticClass()))
		{
			TSubclassOf<ARoad> RoadClass = Cast<UClass>(BlueprintAsset->GeneratedClass);
			if (RoadClass)
			{
				ARoad* CurrentRoad = GetWorld()->SpawnActor<ARoad>(RoadClass, SpawnLocation, SpawnRotation);
				RoadsSegments.Add(CurrentRoad);

				if (i > 0)
				{
					RegulateRoadSegmentsPosition(CurrentRoad, i);
				}
			}
		}
	}

	// Spawn du BP du camp de survivant pour passer à la phase 2
	const FString TargetName = TEXT("BP_SurvivorCamp");
	TObjectPtr<UObject> FoundAsset = nullptr;
	
	for (const FAssetData& AssetData : AssetDataList)
	{
		if (AssetData.AssetName.ToString() == TargetName)
		{
			FoundAsset = AssetData.GetAsset();
			TObjectPtr<UBlueprint> BlueprintAsset = Cast<UBlueprint>(FoundAsset);
			if (BlueprintAsset && BlueprintAsset->GeneratedClass->IsChildOf(ARoad::StaticClass()))
			{
				TSubclassOf<ARoad> RoadSurvivorClass = Cast<UClass>(BlueprintAsset->GeneratedClass);
				if (RoadSurvivorClass)
				{
					ARoad* SurvivorCampRoad = GetWorld()->SpawnActor<ARoad>(RoadSurvivorClass, SpawnLocation, SpawnRotation);
					RegulateRoadSegmentsPosition(SurvivorCampRoad, RoadsSegments.Num());
				}
			}
			break;
		}
	}
}


void ARoadManager::RegulateRoadSegmentsPosition(TObjectPtr<ARoad> RoadToMove, int LoopStep)
{
	TObjectPtr<ARoad> PreviousRoadSegment = RoadsSegments[LoopStep-1];
	
	if (PreviousRoadSegment == nullptr) return;
	
	FVector TargetStartPointLocation = PreviousRoadSegment->EndPoint->GetComponentLocation();
	RoadToMove->StartPoint->SetWorldLocation(TargetStartPointLocation);
}

