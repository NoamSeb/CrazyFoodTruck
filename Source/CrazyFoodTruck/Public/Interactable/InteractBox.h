// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable/Interactable.h"
#include "InteractBox.generated.h"

enum class ELocalMultiplayerInputMappingType;

class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractController, APlayerController*, InstigatorPlayerController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollision);

UCLASS()
class CRAZYFOODTRUCK_API AInteractBox : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	AInteractBox();

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

	UPROPERTY(EditAnywhere, Category="Interact|Possess")
	APawn* PawnToPossess = nullptr;

	UPROPERTY(EditAnywhere, Category="Interact|Possess")
	ELocalMultiplayerInputMappingType MappingType;

	virtual void Interact(APlayerController* InstigatorPlayerController) override;

	void PossessPawn(APlayerController* PlayerController);
	void UnpossessPawn();

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
	
	APlayerController* CachedPlayerController = nullptr;
	APawn* CachedPreviousPawn = nullptr;
};
