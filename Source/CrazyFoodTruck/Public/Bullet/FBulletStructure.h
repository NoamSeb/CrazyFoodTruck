// FBulletStructure.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // <- important pour FTableRowBase
#include "FBulletStructure.generated.h"

USTRUCT(BlueprintType)
struct FBulletStructure : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bullet")
	int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bullet")
	float Speed; // cm/s

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bullet")
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bullet")
	float AreaSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bullet")
	float AreaDepht;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bullet")
	float GravityScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bullet")
	int Ammo;
};