// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cabestan/CabestanController.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ESideCabestan.h"
#include "Bullet/FBulletStructure.h"
#include "GameFramework/Pawn.h"
#include "SideCabestan.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API ASideCabestan : public APawn
{
	GENERATED_BODY()

public:
	
	ASideCabestan();
	UPROPERTY(EditAnywhere, Category="Cabestan")
	ESideCabestan _Side;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext* SideCabestanInputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* YawAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* RollAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* QuitAction;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void HandleYaw(const FInputActionValue& Value);
	void HandleRoll(const FInputActionValue& Value);
	void HandleQuit(const FInputActionValue& Value);

	void DropRollInput();
	void DropYawInput();

	UPROPERTY(EditAnywhere)
	AInteractBox* InteractBox;

	
	void PlayerInteracted(APlayerController* PlayerController);

public:
	
	virtual void Tick(float DeltaTime) override;

private:

	// VARIABLE


	UPROPERTY(EditAnywhere, Category="Cabestan")
	ACabestanController* _CabestanController = nullptr;
};
