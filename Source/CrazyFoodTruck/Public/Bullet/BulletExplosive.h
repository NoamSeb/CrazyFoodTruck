// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BulletBase.h"
#include "Kismet/KismetSystemLibrary.h"

#include "BulletExplosive.generated.h"


UCLASS()
class CRAZYFOODTRUCK_API ABulletExplosive : public ABulletBase
{
	GENERATED_BODY()

public:
	ABulletExplosive();
protected:

	UPROPERTY(EditAnywhere, Category="Trace")
	float FloatTraceRadius = 600.f;

	virtual void EnemyHitBlueprint(AActor* EntityActor) override;
	virtual void EnemyHit(IIEntity* Entity) override;
	virtual void GroundHit() override;
	
	void SpawnExplosion();
	
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

private:
	bool bHasExploded = false;
};
