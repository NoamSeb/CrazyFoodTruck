// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FoodTruckDataSubSystem.generated.h"

/**
 * 
 */
UCLASS()
class CRAZYFOODTRUCK_API UFoodTruckDataSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

#pragma region Cabestan
public:
	UPROPERTY(EditAnywhere)
	float RotationSpeed;
#pragma endregion Cabestan
	
#pragma region Turret
public:
	UPROPERTY(EditAnywhere)
	float TurretFireRate;

	UPROPERTY(EditAnywhere)
	float TurretRangeSide;
	
	UPROPERTY(EditAnywhere)
	float TurretRangeDepth;

	UPROPERTY(EditAnywhere)
	int TurretMaxAmmo;

	UPROPERTY(EditAnywhere)
	int DamagePerBullet;
#pragma endregion Turret

#pragma region Driving
public:
	UPROPERTY(EditAnywhere)
	float SpeedRecovery;
	
	UPROPERTY(EditAnywhere)
	float TruckRotationSpeed;
#pragma endregion Driving
public:
	// = Repair Fill Per Press
	UPROPERTY(EditAnywhere)
	float RepairSpeed;
};
