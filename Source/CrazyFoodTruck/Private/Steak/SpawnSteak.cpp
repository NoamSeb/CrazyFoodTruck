// Fill out your copyright notice in the Description page of Project Settings.


#include "Steak/SpawnSteak.h"

#include "Characters/CrazyFoodTruckCharacter.h"
#include "Steak/ActorSteak.h"


// Sets default values
ASpawnSteak::ASpawnSteak()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//this->GetComponents<UBoxComponent>(CollisionMesh);
	CollisionMesh = this->FindComponentByClass<UBoxComponent>();

	//CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &ASpawnSteak::OnBoxBeginOverlap);
}

// Called when the game starts or when spawned
void ASpawnSteak::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnSteak::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ASpawnSteak::SpawnSteak()
{
	
	FVector SpawnLocation = PointSpawn->GetActorLocation();
	//FVector SpawnScale(1.0f, 1.0f, 1.0f);
	SpawnLocation.Z = SpawnLocation.Z + Distance;
				
	FTransform NewTransform;
	NewTransform.SetLocation(SpawnLocation);
			
	//NewTransform.SetScale3D(SpawnScale);

	//définir comment les zombies spawn et leurs collision quand ils spawn
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				
	//faire spawn un character de la class PawnZombie remplis avant avec son transfom
	AActorSteak* NewSteak = GetWorld()->SpawnActor<AActorSteak>(SpawnedSteak, NewTransform, SpawnParams);

	NewSteak->AttachToActor(Plate, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	//NewSteak->AttachToComponent(Plate, FAttachmentTransformRules::KeepRelativeTransform);
}


