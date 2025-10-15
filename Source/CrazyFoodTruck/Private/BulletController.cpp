// Fill out your copyright notice in the Description page of Project Settings.


#include "CrazyFoodTruck/Public/BulletController.h"

#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ABulletController::ABulletController()
{
	PrimaryActorTick.bCanEverTick = true;

	_BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Coll_Box"));
	_BoxCollider->SetGenerateOverlapEvents(true);
	_BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ABulletController::OnOverlapBegin);
	
}

void ABulletController::Initialize(float speed, float lifeTime)
{
	BulletMovementComponent = FindComponentByClass<UProjectileMovementComponent>();

	_speed = speed;
	_lifeTime = lifeTime;
	_currentLifeTime = lifeTime;
	if (BulletMovementComponent)
	{
		BulletMovementComponent->InitialSpeed = speed;
	}
}

void ABulletController::AddVelocity(FVector dir, float speed)
{
}

void ABulletController::BeginPlay()
{
	Super::BeginPlay();
}

void ABulletController::Tick(float DeltaTime)
{
	if (!_alive){return;}

	Super::Tick(DeltaTime);
	
	if (_currentLifeTime >0)
	{
		_currentLifeTime -= DeltaTime;
	}
	else
	{
		_alive = false;
		Destroy();
	}
}

void ABulletController::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto* entity = Cast<IIEntity>(OtherActor);
	if (entity)
	{
		entity->ReceiveDamage(1);
		Destroy();
	}
}

