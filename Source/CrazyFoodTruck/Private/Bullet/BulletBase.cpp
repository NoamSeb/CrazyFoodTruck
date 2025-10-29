// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet/BulletBase.h"

#include "Bullet/FBulletStructure.h"
#include "Math/UnitConversion.h"


ABulletBase::ABulletBase()
{
	PrimaryActorTick.bCanEverTick = true;
	_BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Coll_Box"));
	_BoxCollider->SetGenerateOverlapEvents(true);
	_BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ABulletBase::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ABulletBase::BeginPlay()
{
	Super::BeginPlay();
}

void ABulletBase::Initialize(FBulletStructure* BulletStructure, const FVector& direction)
{
	this->damage = BulletStructure->Damage;
	bulletSpeed = BulletStructure->Speed;
	GravityScale = BulletStructure->GravityScale;
	
	BulletMovementComponent = FindComponentByClass<UProjectileMovementComponent>();
	if (BulletMovementComponent)
	{
		BulletMovementComponent->InitialSpeed = bulletSpeed;
		BulletMovementComponent->Activate(true);
		BulletMovementComponent->Velocity = direction.GetSafeNormal() * bulletSpeed;
	}
}

void ABulletBase::GroundHit()
{
	Destroy();
}

void ABulletBase::EnemyHit(IIEntity* Entity)
{
	Destroy();
}

void ABulletBase::EnemyHitBlueprint(AActor* EntityActor)
{
	IIEntity::Execute_ReceiveDamageBlueprint(EntityActor, damage);
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
	
	if (OtherActor->GetClass()->ImplementsInterface(UIEntity::StaticClass()))
	{
		EnemyHitBlueprint(OtherActor);
		IIEntity* EntityInterface = Cast<IIEntity>(OtherActor);
		
		if (EntityInterface)
		{
			if (EntityInterface == nullptr)
			{
				return;
			}
			EntityInterface->ReceiveDamage(damage);
			EnemyHit(EntityInterface);
		}
	}
	
	auto tag = OtherActor->Tags;

	
	if (tag.Num() > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Bullet hit tag: %s"), *tag[0].ToString()));
		if (tag.Contains("Ground"))
		{
			GroundHit();
		}
	}
}

// Called every frame
void ABulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

