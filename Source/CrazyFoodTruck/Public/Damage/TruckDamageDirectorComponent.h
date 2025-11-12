// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TruckDamageDirectorComponent.generated.h"

class AInteractBox;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRAZYFOODTRUCK_API UTruckDamageDirectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTruckDamageDirectorComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Truck|Damage")
	void RefreshRegisteredModules();

	UFUNCTION(BlueprintCallable, Category = "Truck|Damage")
	AInteractBox* DamageRandomModule(bool& bSucceeded);

private:
	void CollectAttachedInteractBoxes(TArray<AInteractBox*>& OutFound) const;
	void MergeAndCleanRegistered(const TArray<AInteractBox*>& InToAdd);
	void GetBreakableCandidates(TArray<AInteractBox*>& OutCandidates) const;
	AInteractBox* PickRandomAndBreak(const TArray<AInteractBox*>& Pool, bool& bSucceeded) const;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Truck|Damage")
	TArray<TObjectPtr<AInteractBox>> RegisteredModules;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Truck|Scan")
	bool bAutoScanAttached = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Truck|Scan")
	bool bIncludeAttachedDescendants = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bShowDebugLog = true;
};
