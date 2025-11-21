// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "MunitionDrawer.h"
#include "GameFramework/Pawn.h"
#include "SideMutionDrawer.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API ASideMutionDrawer : public APawn
{
	GENERATED_BODY()

public:
	ASideMutionDrawer();

protected:
	virtual void BeginPlay() override;

	// INPUT ACTION

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* OpenAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* QuitAction;
	
public:
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:

	UPROPERTY(EditAnywhere, Category="Posess Drawer")
	AInteractBox* InteractBox;

	UPROPERTY(EditAnywhere, Category="MunitionDrawer")
	AMunitionDrawer* LinkedMunitionDrawer;

	// INPUT FUNCTION
	void HandleOpen(const FInputActionValue& Value);
	void QuitDrawer();

};
