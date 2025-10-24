// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FBulletStructure.h"
#include "Interface/IEntity.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BulletBase.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API ABulletBase : public AActor
{
	GENERATED_BODY()

public:
	ABulletBase();
	virtual void Initialize(FBulletStructure* BulletStructure, const FVector& direction);
	
protected:
	
	virtual void BeginPlay() override;
	virtual void GroundHit();
	
	virtual void EnemyHit(IIEntity *Entity);
	virtual void EnemyHitBlueprint(AActor* EntityActor);
	float GetBulletSpeed();
	int GetBulletDamage();
	
	UProjectileMovementComponent* BulletMovementComponent;

	UPROPERTY()
	float speed;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult);

	UBoxComponent* _BoxCollider;

	int damage;
	float bulletSpeed;
	float GravityScale;


public:
	virtual void Tick(float DeltaTime) override;
};
