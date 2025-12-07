// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet/BulletBase.h"

#include "Bullet/FBulletStructure.h"
#include "Components/CapsuleComponent.h"
#include "Interface/IShootable.h"
#include "Math/UnitConversion.h"


ABulletBase::ABulletBase()
{
	PrimaryActorTick.bCanEverTick = true;
	_BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Coll_Box"));
	_BoxCollider->SetGenerateOverlapEvents(true);
	//_BoxCollider->SetNotifyRigidBodyCollision(true); // c le generate hit event wtf
	//_BoxCollider->OnComponentHit.AddDynamic(this, &ABulletBase::OnHit);
	_BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ABulletBase::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ABulletBase::BeginPlay()
{
	Super::BeginPlay();
}

void ABulletBase::Initialize(FBulletStructure BulletStructure, const FVector& direction)
{
	this->damage = BulletStructure.Damage;
	bulletSpeed = BulletStructure.Speed;
	GravityScale = BulletStructure.GravityScale;
	
	BulletMovementComponent = FindComponentByClass<UProjectileMovementComponent>();
	if (BulletMovementComponent)
	{
		BulletMovementComponent->InitialSpeed = bulletSpeed;
		BulletMovementComponent->Activate(true);
		BulletMovementComponent->Velocity = direction.GetSafeNormal() * bulletSpeed;
	}
}

void ABulletBase::GroundHit(FVector LocationHit)
{
	if (GroundImpact)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GroundImpact, LocationHit, GetActorRotation());
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("DESTROY BULLET ON GROUND"));
	Destroy();
}

void ABulletBase::EnemyHit(IIShootable* Entity, FVector LocationHit)
{
	if (ZombieImpact)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ZombieImpact, LocationHit, GetActorRotation());
	}
	Destroy();
}

void ABulletBase::EnemyHitBlueprint(AActor* EntityActor, FVector LocationHit)
{
	IIShootable::Execute_ReceiveDamageBlueprint(EntityActor, damage);
	USceneComponent* AttachComp = nullptr;
	
	if (UActorComponent* RootComp = EntityActor->GetRootComponent())
	{
		AttachComp = Cast<USceneComponent>(RootComp);
	}
	if (ZombieImpact)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(ZombieImpact, AttachComp, NAME_None, LocationHit,GetActorRotation().GetInverse(), EAttachLocation::KeepWorldPosition, true);
	}
	
	Destroy();
}

float ABulletBase::GetBulletSpeed()
{
	return 3000.f;
}

int ABulletBase::GetBulletDamage()
{
	return 1;
}


void ABulletBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr || OtherActor == this)
	{
		return;
	}
	
	FVector impactPoint = SweepResult.ImpactPoint;

	if (OtherActor->GetClass()->ImplementsInterface(UIShootable::StaticClass()))
	{
		EnemyHitBlueprint(OtherActor, impactPoint);
		IIShootable* ShootableEntity = Cast<IIShootable>(OtherActor);
		
		if (ShootableEntity)
		{
			if (ShootableEntity == nullptr)
			{
				return;
			}
			ShootableEntity->ReceiveDamage(damage);
			EnemyHit(ShootableEntity, impactPoint);
		}
	}

		
	auto tag = OtherActor->Tags;
	
	if (tag.Num() > 0)
	{
		if (tag.Contains("Ground"))
		{
			GroundHit(impactPoint);
		}
	}

}

void ABulletBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == nullptr || OtherActor == this)
	{
		return;
	}

}

// Called every frame
void ABulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

