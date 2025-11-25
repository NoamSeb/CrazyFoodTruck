// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bullet/EBulletType.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotationSpeed;
#pragma endregion Cabestan
	
#pragma region Turret
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TurretFireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TurretRangeSide;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TurretRangeDepth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TurretMaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DamagePerBullet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EbulletType TypeBullet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool TripleDamageFor10EBullet;

	int indexBulletShoot = 0;
	
#pragma endregion Turret

#pragma region Driving
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpeedRecovery;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TruckRotationSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Heal;


#pragma endregion Driving
public:
	// = Repair Fill Per Press
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RepairSpeed;
};
