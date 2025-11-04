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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EFFECT", meta=(AllowPrivateAccess="true"))
	UNiagaraSystem* ExplosionEffect;
	
	virtual void EnemyHitBlueprint(AActor* EntityActor, FVector LocationHit) override;
	virtual void EnemyHit(IIEntity* Entity, FVector LocationHit) override;
	virtual void GroundHit(FVector LocationHit) override;
	
	void SpawnExplosion(FVector LocationHit);
	
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

private:
	bool bHasExploded = false;
};
