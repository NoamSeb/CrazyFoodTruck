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

    // --- Step 1: Build the DataTable path dynamically ---
    FString VarName = TEXT("0"); // Change or make this dynamic depending on your level
    FString DataTablePath = FString::Printf(TEXT("/Game/CrazyFoodTruck/Blueprint/Road/Levels/DT_Level_%s.DT_Level_%s"), *VarName, *VarName);

    UDataTable* LevelDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *DataTablePath));
    if (!LevelDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("DataTable not found at path: %s"), *DataTablePath);
        return;
    }

    // --- Step 2: Get rows ---
    TArray<FName> RowNames = LevelDataTable->GetRowNames();
    if (RowNames.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No rows found in DataTable: %s"), *DataTablePath);
        return;
    }

    FVector SpawnLocation = FVector::ZeroVector;
    FRotator SpawnRotation = FRotator::ZeroRotator;

    // --- Step 3: Iterate through rows in order ---
    for (int32 i = 0; i < RowNames.Num(); ++i)
    {
        uint8* RowData = LevelDataTable->FindRowUnchecked(RowNames[i]);
        if (!RowData)
            continue;

        UClass* RoadClass;
        
       GetRoadSegmentFromLevelDataTable(RoadClass, LevelDataTable, RowData);

        // --- Step 5: Spawn the Road segment ---
        ARoad* CurrentRoad = GetWorld()->SpawnActor<ARoad>(RoadClass, SpawnLocation, SpawnRotation);
        if (CurrentRoad)
        {
            RoadsSegments.Add(CurrentRoad);
            if (i > 0)
                RegulateRoadSegmentsPosition(CurrentRoad, i);
        }
    }

    SpawnTileToSurvivorCamp(SpawnLocation, SpawnRotation);
   
}


void ARoadManager::RegulateRoadSegmentsPosition(TObjectPtr<ARoad> RoadToMove, int LoopStep)
{
	TObjectPtr<ARoad> PreviousRoadSegment = RoadsSegments[LoopStep-1];
	
	if (PreviousRoadSegment == nullptr) return;
	
	FVector TargetStartPointLocation = PreviousRoadSegment->EndPoint->GetComponentLocation();
	RoadToMove->StartPoint->SetWorldLocation(TargetStartPointLocation);
}

void ARoadManager::SpawnTileToSurvivorCamp(FVector Location, FRotator Rotation)
{
    const FString TargetName = TEXT("BP_SurvivorCamp");
    FString LocalFolderPath = TEXT("/Game/CrazyFoodTruck/Blueprint/Road/Segments");
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

    TArray<FAssetData> AssetDataList;
    AssetRegistryModule.Get().GetAssetsByPath(*LocalFolderPath, AssetDataList, true);
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
                    ARoad* SurvivorCampRoad = GetWorld()->SpawnActor<ARoad>(RoadSurvivorClass, Location, Rotation);
                    RegulateRoadSegmentsPosition(SurvivorCampRoad, RoadsSegments.Num());
                }
            }
            break;
        }
    }
}

void ARoadManager::GetRoadSegmentFromLevelDataTable(UClass* &RoadClass, UDataTable* &LevelDataTable, uint8* &RowData)
{
    // --- Step 4: Find the first Class or Object property dynamically ---
    auto* RowStruct = LevelDataTable->GetRowStruct();
    FProperty* ClassProp = nullptr;

    for (TFieldIterator<FProperty> PropIt(RowStruct); PropIt; ++PropIt)
    {
        if (PropIt->IsA(FClassProperty::StaticClass()) || PropIt->IsA(FObjectProperty::StaticClass()))
        {
            ClassProp = *PropIt;
            break;
        }
    }

    if (!ClassProp)
    {
        UE_LOG(LogTemp, Error, TEXT("No Class or Object property found in DataTable struct '%s'"), *RowStruct->GetName());
    }

    RoadClass = nullptr;

    if (FClassProperty* CProp = CastField<FClassProperty>(ClassProp))
    {
        RoadClass = Cast<UClass>(CProp->GetPropertyValue_InContainer(RowData));
    }
    else if (FObjectProperty* OProp = CastField<FObjectProperty>(ClassProp))
    {
        UObject* Obj = OProp->GetObjectPropertyValue_InContainer(RowData);
        if (Obj)
            RoadClass = Cast<UClass>(Obj);
    }

    if (!RoadClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid or missing class in row"));
    }
}

