// Fill out your copyright notice in the Description page of Project Settings.


#include "ModuleExplosive.h"

#include "CrazyFoodTruck/Data/Public/GameInstanceCrazyFoodTruck.h"
#include "Interface/IEntity.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AModuleExplosive::AModuleExplosive()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AModuleExplosive::BeginPlay()
{
	Super::BeginPlay();
}

void AModuleExplosive::Interact(APlayerController* InstigatorPlayerController, ACrazyFoodTruckCharacter* CrazyCharacter)
{
    if (!CanInteractWithModule()){return;}
	Explode();
    Super::Interact(InstigatorPlayerController, CrazyCharacter);
}

void AModuleExplosive::Explode()
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
            if (HitActor->GetClass()->ImplementsInterface(UIEntity::StaticClass()))
            {
                IIEntity::Execute_ReceiveDamageBlueprint(HitActor, ExplosionDamage);
            }
            else
            {
                IIEntity* Entity = Cast<IIEntity>(HitActor);
                if (Entity)
                {
                    Entity->ReceiveDamage(ExplosionDamage);
                }
            }
        }
    }

    UGameInstanceCrazyFoodTruck* GI = Cast<UGameInstanceCrazyFoodTruck>(GetGameInstance());
    if (GI)
    {
        GI->PlayerCameraShake(Explosion);
    }

}

void AModuleExplosive::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

}


