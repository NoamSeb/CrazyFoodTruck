// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet/BulletNormal.h"


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

void ABulletNormal::EnemyHitBlueprint(AActor* EntityActor)
{
	Super::EnemyHitBlueprint(EntityActor);
}

void ABulletNormal::GroundHit()
{
	Super::GroundHit();
}

void ABulletNormal::EnemyHit(IIEntity* Entity)
{
	Super::EnemyHit(Entity);
}

void ABulletNormal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

