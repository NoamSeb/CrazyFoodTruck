// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieIA.h"
#include "GameFramework/Actor.h"
#include "AreaZombieSpawn.h"
#include "FWaveStructure.h"

#include "Vehicle/Vehicle.h"

#if WITH_EDITOR
#include "Editor.h"
#include "Editor/EditorEngine.h"
#endif
#include "FZoneSpawn.h"
#include "Engine/World.h"
#include "Engine/Level.h"

#include "UObject/ConstructorHelpers.h"

#include "HordeManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAnyZombieDied, AZombieIA*, Zombie, AActor*, Killer);

UCLASS()

class CRAZYFOODTRUCK_API AHordeManager : public AActor
{
	GENERATED_BODY()

public:
	AHordeManager();
	void FindTargetPoints();

	UPROPERTY(EditAnywhere, Category= "Horde Manager | BP Food Truck")
    AVehicle* FoodTruck;
	
	UFUNCTION(BlueprintCallable , Category = "Horde Manager | Stats")
	void IncrementZombiesKilled();

#pragma region Counter

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	int32 ZombiesKilledTotal = 0;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	int32 GetZombiesKilledCount() const { return ZombiesKilledTotal; }

	UPROPERTY(BlueprintAssignable, Category = "Horde|Events")
	FOnAnyZombieDied OnAnyZombieDied;

#pragma endregion

#pragma region Rapport avec le zombie 
	//mettre le BP du zombie dans le BP de la horde
	UPROPERTY(EditAnywhere, Category= "Horde Manager | BP Zombie")
	TSubclassOf<AZombieIA> PawnZombie = AActor::StaticClass();

	UPROPERTY(EditAnywhere, meta=(ToolTip="Variable in kilometers per hour", ForceUnits="km/h"), Category= "Horde Manager | BP Zombie")
	float DifferenceBetweenFoodTruck = 5.f;
	float KilometersToMetersConvertingValue = 27.777777777778;
	float FinalZombieSpeed;
	
#pragma endregion

#pragma region Actor to Follow
	//Attention sur Character Follower regarder si ça affect nav mash parce que ça peut ne pas fonctionner avec ça si c'est le cas mettre Can Ever Affect Navigation en false
	UPROPERTY(EditAnywhere, Category = "Horde Manager | Character to Follow")
	TArray< TObjectPtr<AActor>> MainActorToFollow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horde Manager | Character to Follow")
	TArray< TObjectPtr<AActor>> LeftActorToFollow;

	UPROPERTY(EditAnywhere, Category = "Horde Manager | Character to Follow")
	TArray< TObjectPtr<AActor>> RightActorToFollow;

#pragma endregion

	UFUNCTION(BlueprintCallable, Category = "Horde Manager | Spawn Horde")
	void SpawnHordeZombie(int32 nombreZombies, AAreaZombieSpawn* ZoneSpawn, ETargetZombiePoint PositionSpawn);



#pragma region Spawn Horde Zombie
	
	//garder les zombies dans la liste
	UPROPERTY(VisibleAnywhere, Category= "Horde Manager | Spawn Vague")
	TArray<AZombieIA*> ListHordeZombie;

	UPROPERTY(VisibleAnywhere, Category = "Horde Manager | Spawn Vague")
	int8 nbrZombiesDefinis = 5;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Horde Manager | Spawn Vague")
	int32 nbrVague = 0;;

	UFUNCTION(BlueprintCallable, Category = "Horde Manager | Spawn Vague")
	void InitHordeZombies();
#pragma endregion

#pragma region Area Spawn

	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Spawn Area")
	TSubclassOf<AAreaZombieSpawn> AreaZombieSpawn = AActor::StaticClass();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Area")
	TArray<FZoneSpawn> ListSpawnArea;
	
	UFUNCTION(CallInEditor, Category = "Spawn Area")
	void AddSpawnArea();
	
	UFUNCTION(CallInEditor, Category = "Spawn Area")
	void ClearSpawnArea();
	
#pragma endregion 

#pragma region UI Spawn Indicator

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Horde Manager | UI")
	TSubclassOf<UUserWidget> ZombieSpawnIndicatorWidgetClass;

#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	
	bool bCanSpawnHorde = true;
	
	UFUNCTION()
	void HandleZombieDied(AZombieIA* Zombie, AActor* Killer);

	void ShowSpawnIndicatorForTarget(ETargetZombiePoint TargetPoint);
};
