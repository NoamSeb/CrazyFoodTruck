// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet/BulletController.h"

#include "Kismet/KismetSystemLibrary.h"


// Sets default values
ABulletController::ABulletController()
{
	PrimaryActorTick.bCanEverTick = true;
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

	if (OtherActor == nullptr || OtherActor == this)
    {
        return;
    }
	
	if (OtherActor->GetClass()->ImplementsInterface(UIEntity::StaticClass()))
	{
		IIEntity::Execute_ReceiveDamageBlueprint(OtherActor, 1);
		IIEntity* EntityInterface = Cast<IIEntity>(OtherActor);

		if (EntityInterface)
		{
			if (EntityInterface == nullptr)
			{
				return;
			}
			EntityInterface->ReceiveDamage(1);
		}
		Destroy();
	}
	
	auto tag = OtherActor->Tags;

	
	if (tag.Num() > 0)
    {
		if (tag.Contains("Ground"))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Hit Ground");
			Destroy();
		}
    }

}



