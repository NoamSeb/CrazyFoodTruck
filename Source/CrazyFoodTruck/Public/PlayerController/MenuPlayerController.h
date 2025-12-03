// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuPlayerController.generated.h"

class UInputAction;

UCLASS()
class CRAZYFOODTRUCK_API AMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void SetupInputComponent() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> JoinAction;

private:
	UFUNCTION()
	void OnJoinPressed(const struct FInputActionValue& Value);
};
