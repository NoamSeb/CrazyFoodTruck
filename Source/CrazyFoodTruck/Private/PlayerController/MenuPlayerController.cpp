// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerController/MenuPlayerController.h"

#include "CrazyFoodTruck/Data/Public/GameInstanceCrazyFoodTruck.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"

void AMenuPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (JoinAction)
		{
			EIC->BindAction(JoinAction, ETriggerEvent::Started, this, &AMenuPlayerController::OnJoinPressed);
		}
	}
}

void AMenuPlayerController::OnJoinPressed(const FInputActionValue& Value)
{
	if (UGameInstanceCrazyFoodTruck* GI = GetGameInstance<UGameInstanceCrazyFoodTruck>())
	{
		const ULocalPlayer* LP = GetLocalPlayer();
		if (!LP) return;

		const int32 ControllerId = LP->GetControllerId();
		GI->TryJoinPlayer(ControllerId);
	}
}
