// Fill out your copyright notice in the Description page of Project Settings.


#include "MineExplosive.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"


class UGameInstanceCrazyFoodTruck;

AMineExplosive::AMineExplosive()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMineExplosive::BeginPlay()
{
	Super::BeginPlay();
}

void AMineExplosive::ReceiveDamage(int DamageAmount)
{
    if (bHasExploded){return;}
    bHasExploded = true;
    Explode();
    Destroy();
}

void AMineExplosive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMineExplosive::Explode()
{
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
        ExplosionRadius,            // Sphere radius (en cm)
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
            if (HitActor->GetClass()->ImplementsInterface(UIShootable::StaticClass()))
            {
                IIShootable::Execute_ReceiveDamageBlueprint(HitActor, ExplosionDamage);
            }
            else
            {
                IIShootable* EntityShootable = Cast<IIShootable>(HitActor);
                if (EntityShootable)
                {
                    EntityShootable->ReceiveDamage(ExplosionDamage);
                }
            }
        }
    }

    UGameInstanceCrazyFoodTruck* GI = Cast<UGameInstanceCrazyFoodTruck>(GetGameInstance());
    if (GI)
    {
        GI->PlayerCameraShake(Explosion);
    }

    if (ExplosionEffect)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation());
    }
    UGameplayStatics::PlaySound2D(this, BoomSound);
}
