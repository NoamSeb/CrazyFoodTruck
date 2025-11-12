// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet/BulletExplosive.h"

#include "CrazyFoodTruck/Data/Public/GameInstanceCrazyFoodTruck.h"


// Sets default values
ABulletExplosive::ABulletExplosive()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABulletExplosive::EnemyHitBlueprint(AActor* EntityActor, FVector LocationHit)
{
    SpawnExplosion(LocationHit);
}

void ABulletExplosive::EnemyHit(IIEntity* Entity, FVector LocationHit)
{
    SpawnExplosion(LocationHit);
}

void ABulletExplosive::GroundHit(FVector LocationHit)
{
    SpawnExplosion(LocationHit);
}

void ABulletExplosive::SpawnExplosion(FVector LocationHit)
{
    if (bHasExploded){return;}
    bHasExploded = true;
    
    const FVector TraceStart = GetActorLocation();
    const FVector TraceEnd = TraceStart;

    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);

    TArray<FHitResult> OutHits;

    const FLinearColor TraceColor = FLinearColor::Red;
    const FLinearColor HitColor = FLinearColor::Green;
    const float DrawTimeSeconds = 3.0f;
    
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType( ECC_Pawn));

    bool bHitSomething = UKismetSystemLibrary::SphereTraceMultiForObjects(
        GetWorld(),                 // World context
        TraceStart,                 // Start
        TraceEnd,                   // End (ici identique)
        FloatTraceRadius,            // Sphere radius (en cm)
        ObjectTypes,         // Quels types d'objets rechercher (voir explication)
        false,                      // bTraceComplex (trace contre la géométrie complexe)
        ActorsToIgnore,             // actors to ignore
        EDrawDebugTrace::ForDuration, // debug draw (None, ForOneFrame, ForDuration, Persistent)
        OutHits,                    // sortie : array de hits
        true,                       // bIgnoreSelf (utile mais on a déjà ajouté this dans ActorsToIgnore)
        TraceColor,                 // couleur trace
        HitColor,                   // couleur hit
        DrawTimeSeconds            // durée d'affichage du debug
    );

    if (bHitSomething)
    {
        for (const FHitResult& Hit : OutHits) // HIT ZOMBIES
        {
            AActor* HitActor = Hit.GetActor();
            if (!HitActor) continue;
            if (HitActor->GetClass()->ImplementsInterface(UIEntity::StaticClass()))
            {
                Super::EnemyHitBlueprint(HitActor, Hit.ImpactPoint);
            }
            else
            {
                IIEntity* Entity = Cast<IIEntity>(HitActor);
                if (Entity)
                {
                    Super::EnemyHit(Entity, Hit.ImpactPoint);
                }
            }
        }
    }

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, LocationHit, FRotator::ZeroRotator);
    UGameInstanceCrazyFoodTruck* GI = Cast<UGameInstanceCrazyFoodTruck>(GetGameInstance());
    if (GI)
    {
        GI->PlayerCameraShake(Explosion);
    }


    Destroy();
}

// Called when the game starts or when spawned
void ABulletExplosive::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABulletExplosive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

