// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interactable/Interactable.h"
#include "LocalMultiplayerCharacter.generated.h"

class UInputAction;

struct FInputActionValue;

UCLASS()
class CRAZYFOODTRUCK_API ALocalMultiplayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALocalMultiplayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void OnInputMove(const FInputActionValue& InputActionValue);
	void TryInteract();

public:
	UPROPERTY(EditAnywhere, Category="Movement")
	float MovementSpeed = 600.f;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> IA_Move = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> IA_Interact = nullptr;

	const TScriptInterface<IInteractable>& GetFocusedInteractable() const;
	void SetFocusedInteractable(const TScriptInterface<IInteractable>& NewTarget);

private:
	UPROPERTY()
	TScriptInterface<IInteractable> FocusedInteractable;
};
