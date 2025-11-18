// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PositionSpawn.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"

#include "AreaZombieSpawn.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API AAreaZombieSpawn : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAreaZombieSpawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
	UBoxComponent* NewBoxAreaSpawn;
	
	void InitArea(int32 IdArea, AActor* ParentActor);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spawn")
	ETargetZombiePoint PositionSpawn;

    ETargetZombiePoint GetPositionSpawn () const { return PositionSpawn; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
