// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerZombie.h"

#include "Interface/IVehicule.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


ATriggerZombie::ATriggerZombie()
{
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("TriggerZombie");
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATriggerZombie::OnOverlapBegin);
	BoxComponent->SetLineThickness(50.f);
	BoxComponent->ShapeColor = FColor::Red;
}

void ATriggerZombie::BeginPlay()
{
	Super::BeginPlay();
	if (!HordeManager)
	{
		auto hordeActor = UGameplayStatics::GetActorOfClass(GetWorld(), AHordeManager::StaticClass());
		HordeManager = Cast<AHordeManager>(hordeActor);
	}
}

void ATriggerZombie::Initialize(AHordeManager* NewHordeManager)
{
	HordeManager = NewHordeManager;
}

void ATriggerZombie::SpawnZone()
{
	AAreaZombieSpawn* NewArea = GetWorld()->SpawnActor<AAreaZombieSpawn>(AreaZombieSpawn, GetActorLocation(), GetActorRotation());
	if (!NewArea){return;}
	// for (auto W : SpawnWaves)
	// {
	// 	if (W.ZoneSpawn == nullptr)
	// 	{
	// 		W.ZoneSpawn = NewArea;
	// 	}
	// }

#if WITH_EDITOR
	if (GEditor && NewArea)
	{
		// Nettoyer la sélection précédente
		GEditor->SelectNone(false, true, false);

		// Sélectionner le nouvel acteur
		GEditor->SelectActor(NewArea, true, true, true);

		// Centrer la vue dessus
		GEditor->MoveViewportCamerasToActor(*NewArea, false);
			
	}
#endif

}

void ATriggerZombie::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bHasTriggered){return;}
	if (OtherActor && (OtherActor != this))
	{
		if (OtherActor->GetClass()->ImplementsInterface(UIVehicule::StaticClass()))
		{
			// VEHICULE ENTER
			if (!HordeManager)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Red, "NO HORDE MANAGER ON TRIGGER ZOMBIE");
				return;
			}
			
			BoxComponent->OnComponentBeginOverlap.RemoveDynamic(this, &ATriggerZombie::OnOverlapBegin);
			bHasTriggered = true;
			
			for (auto W : SpawnWaves)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.F, FColor::Green, "Trigger Zombie Spawn Horde ");
				HordeManager->SpawnHordeZombie(W.ZombieAmount, W.ZoneSpawn, W.TargetZombiePoint);
			}
		}
	}
}

// Called every frame
void ATriggerZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

