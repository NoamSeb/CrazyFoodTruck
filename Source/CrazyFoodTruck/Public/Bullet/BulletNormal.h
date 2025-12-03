// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BulletBase.h"
#include "BulletNormal.generated.h"

class UBoxComponent;

UCLASS()
class CRAZYFOODTRUCK_API ABulletNormal : public ABulletBase
{
	GENERATED_BODY()
public:
	
	ABulletNormal();
	virtual void Initialize(FBulletStructure BulletStructure ,const FVector& direction) override;

protected:

	
	virtual void EnemyHitBlueprint(AActor* EntityActor, FVector LocationHit) override;
	virtual void BeginPlay() override;
	virtual void GroundHit(FVector LocationHit) override;
	virtual void EnemyHit(IIShootable* Entity, FVector LocationHit) override;

public:
	virtual void Tick(float DeltaTime) override;
};
