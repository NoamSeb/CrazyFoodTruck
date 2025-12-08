// Fill out your copyright notice in the Description page of Project Settings.

#include "HordeManager.h"

#include <string>

#include "TargetController.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Vehicle/Vehicle.h"

AHordeManager::AHordeManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AHordeManager::FindTargetPoints()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetController::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (ATargetController* TargetPoint = Cast<ATargetController>(Actor))
		{
			switch (TargetPoint->GetSideTarget())
			{
				case ESideTarget::Left:
				LeftActorToFollow.Add(TargetPoint);
				break;
				case ESideTarget::Middle:
				MainActorToFollow.Add(TargetPoint);
				break;
				case ESideTarget::Right:
				RightActorToFollow.Add(TargetPoint);
				break;
				default:
				break;
			}
		}
	}
}

void AHordeManager::AddSpawnArea()
{
	AAreaZombieSpawn* NewArea = GetWorld()->SpawnActor<AAreaZombieSpawn>(AreaZombieSpawn);
	NewArea->InitArea(ListSpawnArea.Num(),this);

	FZoneSpawn newZoneSpawn;
	newZoneSpawn.PositionSpawn = ETargetZombiePoint::MiddleDown;
	newZoneSpawn.AreaZombieSpawn = NewArea;
	
	ListSpawnArea.Add(newZoneSpawn);

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

void AHordeManager::ClearSpawnArea()
{
	for (FZoneSpawn Element : ListSpawnArea)
	{
		Element.AreaZombieSpawn->Destroy();
	}
	ListSpawnArea.Empty();
}

void AHordeManager::ShowSpawnIndicatorForTarget(ETargetZombiePoint TargetPoint)
{
	if (!ZombieSpawnIndicatorWidgetClass)
	{
		return;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC)
	{
		return;
	}

	int32 ViewX = 0;
	int32 ViewY = 0;
	PC->GetViewportSize(ViewX, ViewY);

	const FVector2D ViewportSize(ViewX, ViewY);
	FVector2D ScreenPos(0.f, 0.f);

	const float Padding = 100.f;

	switch (TargetPoint)
	{
	case ETargetZombiePoint::Left:
		ScreenPos.X = Padding;
		ScreenPos.Y = ViewportSize.Y * 0.5f;
		break;

	case ETargetZombiePoint::Right:
		ScreenPos.X = ViewportSize.X - Padding;
		ScreenPos.Y = ViewportSize.Y * 0.5f;
		break;

	case ETargetZombiePoint::MiddleDown:
	default:
		ScreenPos.X = ViewportSize.X * 0.5f;
		ScreenPos.Y = ViewportSize.Y - Padding;
		break;
	}

	UUserWidget* SpawnIndicator = CreateWidget<UUserWidget>(PC, ZombieSpawnIndicatorWidgetClass);
	if (SpawnIndicator)
	{
		SpawnIndicator->AddToViewport();
		SpawnIndicator->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
		SpawnIndicator->SetPositionInViewport(ScreenPos, true);
	}
}

void AHordeManager::SpawnHordeZombie(int32 nombreZombies, AAreaZombieSpawn* ZoneSpawn, ETargetZombiePoint targetPoint)
{
	if (!GEngine)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("TRY SPAWN ZOMBIE WAVE"));

	if (!bCanSpawnHorde)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("HordeManager: bCanSpawnHorde = false, abort."));
		return;
	}

	if (!ZoneSpawn)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("HordeManager: ZoneSpawn is NULL !"));
		return;
	}

	if (!ZoneSpawn->NewBoxAreaSpawn)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("HordeManager: ZoneSpawn->NewBoxAreaSpawn is NULL !"));
		return;
	}

	if (!PawnZombie)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("HordeManager: PawnZombie is NOT set !"));
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("HordeManager: World is NULL !"));
		return;
	}

	if (ListSpawnArea.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("HordeManager: ListSpawnArea is empty."));
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SPAWN ZOMBIE WAVE"));
	++nbrVague;

	const FVector BoxExtent = ZoneSpawn->NewBoxAreaSpawn->GetScaledBoxExtent();
	const FVector BoxCenter = ZoneSpawn->NewBoxAreaSpawn->GetComponentLocation();

	ShowSpawnIndicatorForTarget(targetPoint);

	for (int32 i = 0; i < nombreZombies; ++i)
	{
		const FVector RandomOffset(
			FMath::RandRange(-BoxExtent.X, BoxExtent.X),
			FMath::RandRange(-BoxExtent.Y, BoxExtent.Y),
			90.0f
		);

		const FVector SpawnLocation = BoxCenter + RandomOffset;
		const FVector SpawnScale(1.0f, 1.0f, 1.0f);

		FTransform NewTransform;
		NewTransform.SetLocation(SpawnLocation);
		NewTransform.SetScale3D(SpawnScale);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AZombieIA* NewZombie = World->SpawnActor<AZombieIA>(PawnZombie, NewTransform, SpawnParams);

		if (!NewZombie)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("HordeManager: Failed to spawn Zombie !"));
			continue;
		}

		ListHordeZombie.Add(NewZombie);

		NewZombie->SpawnDefaultController();
		NewZombie->ZombieSpeed = FinalZombieSpeed;
		int mainActor = FMath::RandRange(0, MainActorToFollow.Num() -1);
		
		NewZombie->MainActorToFollower = MainActorToFollow[mainActor];
		
		switch (targetPoint)
		{
		case ETargetZombiePoint::Left:
			NewZombie->FirstActorToFollower = LeftActorToFollow[FMath::RandRange(0, LeftActorToFollow.Num() -1)];
			break;
		case ETargetZombiePoint::Right:
			NewZombie->FirstActorToFollower = RightActorToFollow[FMath::RandRange(0, RightActorToFollow.Num() -1)];
			break;
		case ETargetZombiePoint::MiddleDown:
		default:
			NewZombie->FirstActorToFollower = MainActorToFollow[mainActor];
			break;
		}

		NewZombie->OnZombieDied.AddDynamic(this, &AHordeManager::HandleZombieDied);
		NewZombie->CallRound();
	}
}

void AHordeManager::InitHordeZombies()
{
	if (!bCanSpawnHorde){return;}
	FinalZombieSpeed = (FoodTruck->TruckMaxSpeed + DifferenceBetweenFoodTruck) * KilometersToMetersConvertingValue;

// #pragma region Moche a modif
// 	TArray<UStaticMeshComponent*> Components;
// 	MainActorToFollow->GetComponents<UStaticMeshComponent>(Components);
// 	for (auto Component : Components)
// 	{
// 		if (!Component){return;}
// 		Component->SetCanEverAffectNavigation(false);
// 	}
// 	RightActorToFollow->GetComponents<UStaticMeshComponent>(Components);
// 	for (auto Component : Components)
// 	{
// 		if (!Component){return;}
// 		Component->SetCanEverAffectNavigation(false);
// 	}
// 	LeftActorToFollow->GetComponents<UStaticMeshComponent>(Components);
// 	for (auto Component : Components)
// 	{
// 		if (!Component){return;}
// 		Component->SetCanEverAffectNavigation(false);
// 	}
// 	ForwardActorToFollow->GetComponents<UStaticMeshComponent>(Components);
// 	for (auto Component : Components)
// 	{
// 		if (!Component){return;}
// 		Component->SetCanEverAffectNavigation(false);
// 	}
// 	BackwardActorToFollow->GetComponents<UStaticMeshComponent>(Components);
// 	for (auto Component : Components)
// 	{
// 		if (!Component){return;}
// 		Component->SetCanEverAffectNavigation(false);
// 	}
#pragma endregion
}

void AHordeManager::BeginPlay()
{
	Super::BeginPlay();
	FindTargetPoints();
}

void AHordeManager::HandleZombieDied(AZombieIA* Zombie, AActor* Killer)
{
	++ZombiesKilledTotal;
	ListHordeZombie.Remove(Zombie);
	OnAnyZombieDied.Broadcast(Zombie, Killer);
}

void AHordeManager::IncrementZombiesKilled()
{
	++ZombiesKilledTotal;
}

void AHordeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}