// Fill out your copyright notice in the Description page of Project Settings.

#include "Damage/TruckDamageDirectorComponent.h"

#include "Interactable/InteractBox.h"

#include "Kismet/KismetMathLibrary.h"

UTruckDamageDirectorComponent::UTruckDamageDirectorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoScanAttached = true;
	bIncludeAttachedDescendants = true;
	bShowDebugLog = true;
}

void UTruckDamageDirectorComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bAutoScanAttached)
	{
		RefreshRegisteredModules();
	}

	if (bShowDebugLog && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 5.f, FColor::Cyan,
			FString::Printf(TEXT("[TruckDamageDirector] %d registered breakable modules."), RegisteredModules.Num())
		);
	}
}

void UTruckDamageDirectorComponent::RefreshRegisteredModules()
{
	TArray<AInteractBox*> Found;
	CollectAttachedInteractBoxes(Found);

	MergeAndCleanRegistered(Found);

	for (int32 i = RegisteredModules.Num() - 1; i >= 0; --i)
	{
		AInteractBox* Box = RegisteredModules[i];
		if (!IsValid(Box) || !Box->bBreakable)
		{
			RegisteredModules.RemoveAt(i);
		}
	}
}

void UTruckDamageDirectorComponent::CollectAttachedInteractBoxes(TArray<AInteractBox*>& OutFound) const
{
	OutFound.Reset();

	AActor* Owner = GetOwner();
	if (!Owner) return;

	TArray<AActor*> Queue;
	Owner->GetAttachedActors(Queue);

	for (int32 i = 0; i < Queue.Num(); ++i)
	{
		AActor* Current = Queue[i];
		if (!IsValid(Current)) continue;

		if (AInteractBox* Box = Cast<AInteractBox>(Current))
		{
			if (Box->bBreakable)
			{
				OutFound.Add(Box);

				if (bShowDebugLog)
				{
					UE_LOG(LogTemp, Log, TEXT("[TruckDamageDirector] Module found: %s (breakable)"), *Box->GetName());
				}
			}
			else if (bShowDebugLog)
			{
				UE_LOG(LogTemp, Log, TEXT("[TruckDamageDirector] Module ignored: %s (unbreakable)"), *Box->GetName());
			}
		}

		if (bIncludeAttachedDescendants)
		{
			TArray<AActor*> AttachedChildren;
			Current->GetAttachedActors(AttachedChildren);
			for (AActor* Child : AttachedChildren)
			{
				if (IsValid(Child))
				{
					Queue.Add(Child);
				}
			}
		}
	}
}

void UTruckDamageDirectorComponent::MergeAndCleanRegistered(const TArray<AInteractBox*>& InToAdd)
{
	for (int32 i = RegisteredModules.Num() - 1; i >= 0; --i)
	{
		if (!IsValid(RegisteredModules[i]))
		{
			RegisteredModules.RemoveAt(i);
		}
	}

	for (AInteractBox* Box : InToAdd)
	{
		if (IsValid(Box) && !RegisteredModules.Contains(Box))
		{
			RegisteredModules.Add(Box);
		}
	}
}

void UTruckDamageDirectorComponent::GetBreakableCandidates(TArray<AInteractBox*>& OutCandidates) const
{
	OutCandidates.Reset();

	for (AInteractBox* Box : RegisteredModules)
	{
		if (IsValid(Box) && Box->CanBeBrokenNow())
		{
			OutCandidates.Add(Box);
		}
	}
}

AInteractBox* UTruckDamageDirectorComponent::PickRandomAndBreak(const TArray<AInteractBox*>& Pool, bool& bSucceeded) const
{
	bSucceeded = false;
	if (Pool.Num() == 0) return nullptr;

	const int32 Index = UKismetMathLibrary::RandomIntegerInRange(0, Pool.Num() - 1);
	AInteractBox* Picked = Pool[Index];
	if (!IsValid(Picked)) return nullptr;

	if (Picked->CanBeBrokenNow())
	{
		bSucceeded = Picked->TryBreakModule();

		if (bShowDebugLog && GEngine)
		{
			const FString Msg = FString::Printf(TEXT("Broken module: %s"), *Picked->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, Msg);
		}

		return bSucceeded ? Picked : nullptr;
	}

	return nullptr;
}

AInteractBox* UTruckDamageDirectorComponent::DamageRandomModule(bool& bSucceeded)
{
	TArray<AInteractBox*> Candidates;
	GetBreakableCandidates(Candidates);

	if (Candidates.Num() == 0)
	{
		bSucceeded = false;

		if (bShowDebugLog && GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("No breakable modules available."));
		}

		return nullptr;
	}

	return PickRandomAndBreak(Candidates, bSucceeded);
}
