// Fill out your copyright notice in the Description page of Project Settings.


#include "Road/RoadManager.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "CrazyFoodTruck/Data/Public/GameInstanceCrazyFoodTruck.h"
#include "Kismet/GameplayStatics.h"
#include "Road/Road.h"
#include "Vehicle/Vehicle.h"


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

    // --- Step 1: Get the Data Table from the given path ---
    UGameInstanceCrazyFoodTruck* GI;
   
    if (GetGameInstance() != nullptr)
    {
        GI = Cast<UGameInstanceCrazyFoodTruck>(GetGameInstance());
    }else
    {
        GI = nullptr;
    }
    int LevelNumber = GI->GameData->LevelNumber;
    

    UDataTable* LevelDataTable = Levels[LevelNumber].Level;
    if (!LevelDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("DataTable not found"));
        return;
    }
    
    TArray<FName> RowNames = LevelDataTable->GetRowNames();
    if (RowNames.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No rows found in DataTable"));
        return;
    }

    FVector SpawnLocation = FVector::ZeroVector;
    FRotator SpawnRotation = FRotator::ZeroRotator;

    // --- Step 3: Fetch the Data Table and get the Road Segment from the row ---
    for (int32 i = 0; i < RowNames.Num(); ++i)
    {
        uint8* RowData = LevelDataTable->FindRowUnchecked(RowNames[i]);
        if (!RowData)
            continue;

        UClass* RoadClass;
        
       GetRoadSegmentFromLevelDataTable(RoadClass, LevelDataTable, RowData);

        // --- Step 5: Spawn the selected Road segment ---
        ARoad* CurrentRoad = GetWorld()->SpawnActor<ARoad>(RoadClass, SpawnLocation, SpawnRotation);
        if (CurrentRoad)
        {
            RoadsSegments.Add(CurrentRoad);
            if (i > 0)
                RegulateRoadSegmentsPosition(CurrentRoad, i);
        }
    }

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Je suis juste avant spawn");
    SpawnTileToSurvivorCamp(SpawnLocation, SpawnRotation);
   
}

/// Place correctly the road segments to make the start point's position equal to previous end point's position
/// @param RoadToMove
/// @param LoopStep 
void ARoadManager::RegulateRoadSegmentsPosition(TObjectPtr<ARoad> RoadToMove, int LoopStep)
{
	TObjectPtr<ARoad> PreviousRoadSegment = RoadsSegments[LoopStep-1];
	
	if (PreviousRoadSegment == nullptr) return;
	
	FVector TargetStartPointLocation = PreviousRoadSegment->EndPoint->GetComponentLocation();
	RoadToMove->StartPoint->SetWorldLocation(TargetStartPointLocation);
}

/// After all the segments were spawned, spawn teh Survivor Camp to go next game phase
/// @param Location 
/// @param Rotation 
void ARoadManager::SpawnTileToSurvivorCamp(FVector Location, FRotator Rotation)
{
    if (SurvivorCamp)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "SPAWNNNNNNNNNNNNN");

        ARoad* SurvivorCampRoad = GetWorld()->SpawnActor<ARoad>(SurvivorCamp, Location, Rotation);
        RegulateRoadSegmentsPosition(SurvivorCampRoad, RoadsSegments.Num());
        AVehicle* FoodTruck = Cast<AVehicle>(
        UGameplayStatics::GetActorOfClass(GetWorld(), AVehicle::StaticClass())
        );
        FoodTruck->ShootLineTrace(SurvivorCampRoad->StartPoint->GetComponentLocation());
    }
}

/// Get the correct Road Segment from the Data Table
/// @param RoadClass 
/// @param LevelDataTable 
/// @param RowData 
void ARoadManager::GetRoadSegmentFromLevelDataTable(UClass* &RoadClass, UDataTable* &LevelDataTable, uint8* &RowData)
{
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

