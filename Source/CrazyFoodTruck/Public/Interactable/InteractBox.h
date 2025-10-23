// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable/Interactable.h"
#include "InteractBox.generated.h"

class ACrazyFoodTruckCharacter;

class APlayerController;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractController, APlayerController*, InstigatorPlayerController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollision);

UCLASS()
class CRAZYFOODTRUCK_API AInteractBox : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	AInteractBox();
	// GABRIEL ADD
	
	void PosessPawn(APlayerController* PlayerController);
	void UnPossessPawn();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable, Category="Interact|Events")
	FOnInteractController OnInteractionStarted;

	UPROPERTY(BlueprintAssignable, Category="Interact|Events")
	FOnInteractController OnInteractionEnded;

	UPROPERTY(BlueprintAssignable, Category="Interact|Events")
	FOnCollision OnCollisionEnter;

	UPROPERTY(BlueprintAssignable, Category="Interact|Events")
	FOnCollision OnCollisionExit;

	//virtual void Interact_Implementation(APlayerController* InstigatorPlayerController) override;
	virtual void Interact(APlayerController* InstigatorPlayerController) override;
	UPROPERTY(EditAnywhere)
	APawn* PawnToPossess = nullptr;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interact")
	TObjectPtr<UBoxComponent> Box = nullptr;

	UPROPERTY(VisibleAnywhere, Category="Interact|State")
	TWeakObjectPtr<APlayerController> CurrentInteractorPlayerController;

private:
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void TryReleaseLockFromActor(AActor* LeavingActor);

	APlayerController* GetPlayerControllerFromActor(AActor* Actor) const;
	int32 GetPlayerIndexFromPlayerController(APlayerController* PlayerController) const;
	FColor GetPlayerColorFromPlayerController(APlayerController* PlayerController) const;
	
	APlayerController* ActualPlayerController;
	APawn* ActualPawn;
};
