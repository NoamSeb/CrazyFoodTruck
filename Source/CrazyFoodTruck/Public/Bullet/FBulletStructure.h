#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // <- important pour FTableRowBase
#include "FBulletStructure.generated.h"

class ABulletBase;

USTRUCT(BlueprintType)
struct FBulletStructure : public FTableRowBase
{
	GENERATED_BODY()

	FBulletStructure()
	: Damage(1)
	, Speed(3000.f)
	, FireRate(0.5f)
	, AreaSide(0.f)
	, AreaDepth(0.f)
	, GravityScale(0.f)
	, Ammo(0)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bullet")
	TSubclassOf<ABulletBase> BulletClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bullet")
	int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bullet")
	float Speed; // cm/s

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bullet")
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bullet")
	float AreaSide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bullet")
	float AreaDepth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bullet")
	float GravityScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bullet")
	int32 Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Bullet")
	float HapticsScale;
};