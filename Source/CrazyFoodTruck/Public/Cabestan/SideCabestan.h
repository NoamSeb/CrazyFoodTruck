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

UENUM()
enum class ELocationPlayerCabestan
{
	Nord,
	NordEst,
	Est,
	EstSud,
	Sud,
	SudOuest,
	Ouest,
	OuestNord
};

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
	UInputAction* UpDownAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* PushAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* BringAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* RightLeftAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* QuitAction;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void StopPush();
	void Push(const FInputActionValue& Value);

	void Bring(const FInputActionValue& Value);
	void HandleYaw(const FInputActionValue& Value);
	void HandleRoll(const FInputActionValue& Value);
	void HandleQuit(const FInputActionValue& Value);

	void TurnCabestan(const FInputActionValue& Value);
	void MoveX(const FInputActionValue& Value);
	void MoveY(const FInputActionValue& Value);
	float InputX;
	float InputY;
	ELocationPlayerCabestan LocationPose;

	void DropRollInput();
	void DropYawInput();

	UPROPERTY(EditAnywhere)
	AInteractBox* InteractBox;

	UFUNCTION()
	void PlayerInteracted(APlayerController* PlayerController);

public:
	
	virtual void Tick(float DeltaTime) override;


private:

	bool bPlayerIn = false;
	// VARIABLE

	UPROPERTY(EditAnywhere, Category="Cabestan")
	ACabestanController* _CabestanController = nullptr;
};
