// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet/BulletExplosive.h"



// Sets default values
ABulletExplosive::ABulletExplosive()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ABulletExplosive::EnemyHitBlueprint(AActor* EntityActor)
{
    SpawnExplosion();
}

void ABulletExplosive::EnemyHit(IIEntity* Entity)
{
    SpawnExplosion();
}

void ABulletExplosive::GroundHit()
{
    SpawnExplosion();
}

void ABulletExplosive::SpawnExplosion()
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
        for (const FHitResult& Hit : OutHits)
        {
            AActor* HitActor = Hit.GetActor();
            if (!HitActor) continue;
            if (HitActor->GetClass()->ImplementsInterface(UIEntity::StaticClass()))
            {
                Super::EnemyHitBlueprint(HitActor);
            }
            else
            {
                IIEntity* Entity = Cast<IIEntity>(HitActor);
                if (Entity)
                {
                    Super::EnemyHit(Entity);
                }
            }
        }
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

