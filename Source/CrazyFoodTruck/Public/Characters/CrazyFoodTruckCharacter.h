// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmoBox.h"
#include "GameFramework/Character.h"
#include "Interactable/Interactable.h"
#include "InputMappingContext.h"
#include "Interface/IPlayer.h"
#include "CrazyFoodTruckCharacter.generated.h"

class UCrazyFoodTruckCharacterInputData;

class UEnhancedInputComponent;

UCLASS()
class CRAZYFOODTRUCK_API ACrazyFoodTruckCharacter : public ACharacter, public IIPlayer
{
	GENERATED_BODY()

#pragma region Unreal Default

public:
	// Sets default values for this character's properties
	ACrazyFoodTruckCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// INTERFACE IPLAYER
	virtual void SetInteractState(bool bCanInteract) override;
	virtual bool CanInteract() override;
	virtual void SetAmmoState(bool bHasAmmo) override;
	void TakeAmmoBox(AAmmoBox* AmmoBox);
	AAmmoBox* DepositAmmoBox();
	virtual bool HasAmmo() override;

	
	
private:

	bool _canInteract = true;
	bool _hasAmmo = false;
	AAmmoBox* _carriedAmmoBox = nullptr;
	
#pragma endregion

#pragma region Player Infos

public:
	UFUNCTION(BlueprintCallable, Category="Player")
	int32 GetPlayerIndex() const;

	UFUNCTION(BlueprintCallable, Category="Player")
	FLinearColor GetPlayerColor() const;

	UFUNCTION(BlueprintCallable, Category="Player")
	void SetPlayerColor(FLinearColor NewColor);

protected:
	UPROPERTY(EditAnywhere, Category="Player")
	FLinearColor PlayerColor = FLinearColor::White;

private:
	void UpdatePlayerColorFromController();

#pragma endregion

#pragma region Input Data / Mapping Context

public:
	void SetInputData(UCrazyFoodTruckCharacterInputData* InInputData);
	void SetInputMappingContext(UInputMappingContext* InInputMappingContext);

protected:
	UPROPERTY()
	TObjectPtr<UCrazyFoodTruckCharacterInputData> InputData = nullptr;

	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;

	void SetupMappingContextIntoController() const;

#pragma endregion

#pragma region Input Move

public:
	UPROPERTY(EditAnywhere, Category="Movement")
	float MovementSpeed = 600.f;

private:
	void BindInputMoveAction(UEnhancedInputComponent* EnhancedInputComponent);
	void OnInputMove(const FInputActionValue& InputActionValue);

#pragma endregion

#pragma region Input Interact

public:
	const TScriptInterface<IInteractable>& GetFocusedInteractable() const;
	void SetFocusedInteractable(const TScriptInterface<IInteractable>& NewTarget);

private:
	UPROPERTY()
	TScriptInterface<IInteractable> FocusedInteractable;

	void BindInputInteractAction(UEnhancedInputComponent* EnhancedInputComponent);
	void TryInteract();

#pragma endregion
};
