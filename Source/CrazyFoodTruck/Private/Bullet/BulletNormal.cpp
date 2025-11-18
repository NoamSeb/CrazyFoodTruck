// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet/BulletNormal.h"

#include "Interface/IShootable.h"


ABulletNormal::ABulletNormal()
{
	PrimaryActorTick.bCanEverTick = true;
	speed = 3000.f;
}

void ABulletNormal::BeginPlay()
{
	Super::BeginPlay();
}

void ABulletNormal::Initialize(FBulletStructure* BulletStructure, const FVector& direction)
{
	Super::Initialize(BulletStructure, direction);
}

void ABulletNormal::EnemyHitBlueprint(AActor* EntityActor, FVector LocationHit)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("DESTROY BULLET"));
	Super::EnemyHitBlueprint(EntityActor, LocationHit); 
}

void ABulletNormal::GroundHit(FVector LocationHit)
{
	Super::GroundHit(LocationHit);
}

void ABulletNormal::EnemyHit(IIShootable* Entity, FVector LocationHit)
{
	Super::EnemyHit(Entity, LocationHit);
}

void ABulletNormal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

