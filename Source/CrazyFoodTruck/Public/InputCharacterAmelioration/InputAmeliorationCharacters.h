// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LocalMultiplayerSettings.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "UpgradePhase/AmeliorationWidget.h"
#include "InputAmeliorationCharacters.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRAZYFOODTRUCK_API UInputAmeliorationCharacters : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,  Category="Color for Upgrades")
	int32 indexPlayerColor;
	
	

	
	//DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPlayerAction, int32, PlayerIndex, int32, PositionIndex, bool, OnSkip);
//
//	UPROPERTY(BlueprintAssignable, Category="Events")
//	FOnPlayerAction OnMoveNext;
//	UPROPERTY(BlueprintAssignable, Category="Events")
//	FOnPlayerAction OnMovePrevious;
//	UPROPERTY(BlueprintAssignable, Category="Events")
//	FOnPlayerAction OnMoveOnSkip;
//	UPROPERTY(BlueprintAssignable, Category="Events")
//	FOnPlayerAction OnValidate;
//	UPROPERTY(BlueprintAssignable, Category="Events")
//	FOnPlayerAction OnRemoveValidate;
	
	// Sets default values for this component's properties
	UInputAmeliorationCharacters();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAmeliorationWidget* UpgradeUserWidget;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext* MoveAmeliorationInputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* MoveAction;

	//UPROPERTY(EditDefaultsOnly, Category="Input")
	//UInputAction* MovePreviousAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* MoveOnSkipAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* ValidateAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* RemoveValidateAction;

	UPROPERTY(EditAnywhere)
	int32 IndexCurrentCible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsValidate = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanMoveOnModule = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 indexPosition = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool OnSkip = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 indexPositionForModule = 0;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,  Category="Input")
	int32 indexPlayerController;
	AActor* SelfRef;

	UFUNCTION(BlueprintCallable)
	void Init();
	
	void Move(const FInputActionValue& Value);
	void MoveOnSkip(const FInputActionValue& Value);
	void Validate(const FInputActionValue& Value);
	void RemoveValidate();

	void MoveInModule(const FInputActionValue& Value);
	void ValidInModule(const FInputActionValue& Value);

public:

	void SetupPlayerInput(UInputComponent* PlayerInputComponent);
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	int32 GetPlayerIndexFromPlayerController(APlayerController* PlayerController) const;
	APlayerController* GetPlayerControllerFromActor(AActor* Actor) const;
	FColor GetPlayerColorFromPlayerController(APlayerController* PlayerController) const;
};
