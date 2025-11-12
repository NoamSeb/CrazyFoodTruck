// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EInteractionType.h"
#include "GameFramework/Actor.h"
#include "Interactable/Interactable.h"
#include "InteractBox.generated.h"

enum class ELocalMultiplayerInputMappingType;

class ACrazyFoodTruckCharacter;
class URepairProgressBillboard;
class AAttachPoint;

class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractController, APlayerController*, InstigatorPlayerController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamaged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRestored);

UCLASS()
class CRAZYFOODTRUCK_API AInteractBox : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	AInteractBox();

	virtual void Tick(float DeltaSeconds) override;
	virtual void Interact(APlayerController* InstigatorPlayerController, ACrazyFoodTruckCharacter* CrazyCharacter) override;
	void TryInteractWithObject(APlayerController* InstigatorPlayerController, ACrazyFoodTruckCharacter* CrazyCharacter);
	void TryPossesPawn(APlayerController* InstigatorPlayerController);

#if WITH_EDITOR
	UFUNCTION(CallInEditor, Category = "Interact|Editor")
	void SpawnAttachPointInEditor();
	UFUNCTION(CallInEditor, Category = "Interact|Editor")
	void ClearAttachPoint();
#endif

	bool _IsShowingInput;
	
	bool CanDetectOverlapp();

	UFUNCTION(BlueprintCallable, Category = "Interact|State")
	void UpdateVisibilityInput(bool bIsVisible);

	UFUNCTION(BlueprintCallable, Category = "Interact|State")
	void SetInteractableObject(AActor* NewInteractableObject);

	UFUNCTION(BlueprintCallable, Category = "Interact|State")
	AActor* GetInteractableObject() const { return InteractableObject; }

	UFUNCTION(BlueprintCallable, Category = "Interact|State")
	void SetPawnToPossess(APawn* NewPawnToPossess);

	UFUNCTION(BlueprintCallable, Category = "Interact|State")
	APawn* GetPawnToPossess() const { return PawnToPossess; }

	UFUNCTION(BlueprintCallable, Category = "Interact|State")
	bool GetInputVisibilityState() const { return _IsShowingInput; }

	UFUNCTION(BlueprintCallable, Category = "Interact|State")
	void SetFunctional(bool bNewFunctional);

	UFUNCTION(BlueprintCallable, Category = "Interact|State")
	bool IsFunctional() const { return bIsFunctional; }

	UFUNCTION(BlueprintPure, Category = "Damage|Repair")
	bool IsDamaged() const { return bBreakable && !bIsFunctional; }

	UFUNCTION(BlueprintPure, Category = "Damage|Repair")
	bool CanBeBrokenNow() const;

	UFUNCTION(BlueprintCallable, Category = "Damage|Repair")
	bool TryBreakModule();

	void PossessPawn(APlayerController* PlayerController);
	void UnpossessPawn();

	UFUNCTION(BlueprintCallable, Category = "Damage|Repair")
	void SetRepairProgress(URepairProgressBillboard* RPB);

	UPROPERTY(EditAnywhere, Category = "Damage|Repair")
	bool bStartDamaged = false;

	UPROPERTY(EditAnywhere, Category = "Damage|Repair")
	bool bBreakable = true;

	UPROPERTY(EditAnywhere, Category = "Damage|Repair")
	bool bCanBreakWhilePossessed = false;

	UPROPERTY(EditAnywhere, Category = "Damage|Repair")
	TObjectPtr<URepairProgressBillboard> RepairProgressBillboard = nullptr;

	UPROPERTY(BlueprintAssignable, Category = "Damage|Repair")
	FOnDamaged OnDamaged;

	UPROPERTY(BlueprintAssignable, Category = "Damage|Repair")
	FOnRestored OnRestored;

	UFUNCTION(BlueprintCallable, Category = "Damage|Repair")
	void TryForceBreak();

	UPROPERTY(EditAnywhere, Category = "Interact|Components")
	TObjectPtr<AAttachPoint> AttachPoint = nullptr;

	UPROPERTY(EditAnywhere, Category = "Interact|Components")
	TObjectPtr<AAttachPoint> ReleasePoint = nullptr;

	UPROPERTY(EditAnywhere, Category = "Interact|Possess")
	TObjectPtr<APawn> PawnToPossess = nullptr;

	UPROPERTY(EditAnywhere, Category = "Interact|Possess")
	EInteractionType InteractionType;
	
	UPROPERTY(EditAnywhere, Category = "Interact|Possess")
	ELocalMultiplayerInputMappingType MappingType = static_cast<ELocalMultiplayerInputMappingType>(0);
	
	UPROPERTY(EditAnywhere, Category = "Interact|Possess")
	AActor* InteractableObject;
	
	UPROPERTY(BlueprintAssignable, Category = "Interact|Events")
	FOnInteractController OnInteractionStarted;

	UPROPERTY(BlueprintAssignable, Category = "Interact|Events")
	FOnInteractController OnInteractionEnded;

	UPROPERTY(BlueprintAssignable, Category = "Interact|Events")
	FOnCollision OnCollisionEnter;

	UPROPERTY(BlueprintAssignable, Category = "Interact|Events")
	FOnCollision OnCollisionExit;

	UPROPERTY(BlueprintAssignable, Category = "Interact|Events")
	FOnCollision OnPlayerQuit;



protected:
	virtual void BeginPlay() override;
	
	float OverlappTimer = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "Interact|State")
	float BaseOverlappTimer = 0.01f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact|Components")
	TObjectPtr<UBoxComponent> Box = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Interact|State")
	TWeakObjectPtr<APlayerController> CurrentInteractorPlayerController;

	UPROPERTY(VisibleAnywhere, Category = "Interact|State")
	TSet<TWeakObjectPtr<APlayerController>> OverlappingPlayerControllers;

	UPROPERTY(VisibleAnywhere, Category = "Interact|State")
	bool bPlayerIsControlling = false;

	UPROPERTY(VisibleAnywhere, Category = "Interact|State")
	bool bIsFunctional = true;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> CachedPlayerController;

	UPROPERTY()
	ACrazyFoodTruckCharacter* EnteringCharacter;
	
	UPROPERTY()
	TWeakObjectPtr<ACrazyFoodTruckCharacter> CachedCharacter;

	UPROPERTY()
	TWeakObjectPtr<APawn> CachedPreviousPawn;
	
	FRotator RotationActorOnEnter = FRotator::ZeroRotator;
	FRotator RotationControllerOnEnter = FRotator::ZeroRotator;

private:


	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* Comp, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& Hit);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* Comp, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex);
	void TryExitPlayer(ACrazyFoodTruckCharacter* CharacterController);

	void TryDetectPlayer(APlayerController* PlayerController, ACrazyFoodTruckCharacter* Character);
	void TryReleaseLockFromActor(APlayerController* LeavingPlayerController);
	bool PlayerStillInsideCheck(ACrazyFoodTruckCharacter* TargetCharacter);
	ACrazyFoodTruckCharacter* DetectPlayerInside();

	APlayerController* GetPlayerControllerFromActor(AActor* Actor) const;
	int32 GetPlayerIndexFromPlayerController(APlayerController* PlayerController) const;
	FColor GetPlayerColorFromPlayerController(APlayerController* PlayerController) const;

	void AddOverlappingPlayerController(APlayerController* PlayerController);
	void RemoveOverlappingPlayerController(APlayerController* PlayerController);
	bool IsAnotherPlayerAlreadyInside(APlayerController* ThisPlayerController) const;

	void TeleportAndAttachPlayer(APlayerController* PlayerController);
	void TeleportBackAndDetachPlayer(APlayerController* PlayerController);

	UFUNCTION()
	void OnRepairCompleted();
};
