// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "InteractBox.generated.h"

class ACrazyFoodTruckCharacter;

class UBoxComponent;

UCLASS()
class CRAZYFOODTRUCK_API AInteractBox : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interact")
	TObjectPtr<UBoxComponent> Box = nullptr;

	UPROPERTY(VisibleAnywhere, Category="Interact|State")
	TWeakObjectPtr<ACrazyFoodTruckCharacter> CurrentInteractor;

	virtual void Interact_Implementation(ACrazyFoodTruckCharacter* InstigatorCharacter) override;

private:
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void TryReleaseLock(ACrazyFoodTruckCharacter* LeavingCharacter);
};
