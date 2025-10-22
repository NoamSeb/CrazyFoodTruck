// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "InteractBox.generated.h"

class ACrazyFoodTruckCharacter;

class APlayerController;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractController, APlayerController*, InstigatorPlayerController);

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

public:
	UPROPERTY(BlueprintAssignable, Category="Interact|Events")
	FOnInteractController OnInteractionStarted;

	UPROPERTY(BlueprintAssignable, Category="Interact|Events")
	FOnInteractController OnInteractionEnded;

	virtual void Interact_Implementation(APlayerController* InstigatorPlayerController) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interact")
	TObjectPtr<UBoxComponent> Box = nullptr;

	UPROPERTY(VisibleAnywhere, Category="Interact|State")
	TWeakObjectPtr<APlayerController> CurrentInteractorPlayerController;

	UPROPERTY(VisibleAnywhere, Category="Interact|State")
	TSet<TWeakObjectPtr<APlayerController>> OverlappingPlayerControllers;

private:
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void TryReleaseLockFromActor(AActor* LeavingActor);

	APlayerController* GetPlayerControllerFromActor(AActor* Actor) const;
	int32 GetPlayerIndexFromPlayerController(APlayerController* PlayerController) const;
	FColor GetPlayerColorFromPlayerController(APlayerController* PlayerController) const;

	void AddOverlappingPlayerController(APlayerController* PlayerController);
	void RemoveOverlappingPlayerController(APlayerController* PlayerController);
	bool IsAnotherPlayerAlreadyInside(APlayerController* ThisPlayerController) const;
};
