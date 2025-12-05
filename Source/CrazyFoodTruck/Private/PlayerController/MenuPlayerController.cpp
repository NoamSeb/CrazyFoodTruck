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
			//EIC->BindAction(PlayAction, ETriggerEvent::Started, this, &AMenuPlayerController::OnJoinPressed);
		}
	}
}

void AMenuPlayerController::OnJoinPressed(const FInputActionValue& Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Join");
	if (UGameInstanceCrazyFoodTruck* GI = GetGameInstance<UGameInstanceCrazyFoodTruck>())
	{
		UGameDataSubSystem* GameSubSystem = GI->GetSubsystem<UGameDataSubSystem>();

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("join current : %hhd"), GameSubSystem->CurrentGamePhase));
		
		if (GameSubSystem->CurrentGamePhase == EPhaseGameCrazyFoodTruckState::ChoosePlayers)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Join in choose player");
			
			const ULocalPlayer* LP = GetLocalPlayer();
			if (!LP) return;
		
			const int32 ControllerId = LP->GetControllerId();
			GI->TryJoinPlayer(ControllerId);
		}

		//const ULocalPlayer* LP = GetLocalPlayer();
		//if (!LP) return;
		//
		//const int32 ControllerId = LP->GetControllerId();
		//GI->TryJoinPlayer(ControllerId);
	}
}
