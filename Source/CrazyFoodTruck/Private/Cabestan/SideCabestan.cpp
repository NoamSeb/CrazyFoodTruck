// Fill out your copyright notice in the Description page of Project Settings.


#include "SideCabestan.h"

#include "InputActionValue.h"


// Sets default values
ASideCabestan::ASideCabestan()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASideCabestan::BeginPlay()
{
	Super::BeginPlay();
	if (!_CabestanController)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SideCabestan : CabestanController is not assigned !"));
	}

	if (InteractBox)
	{
		InteractBox->OnInteractionStarted.AddDynamic(this, &ASideCabestan::PlayerInteracted);
	}
}

void ASideCabestan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASideCabestan::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Eic = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (YawAction) // X
		{
		}
		if (RollAction) // Y
		{
			Eic->BindAction(RollAction, ETriggerEvent::Triggered, this, &ASideCabestan::HandleRoll);
			Eic->BindAction(RollAction, ETriggerEvent::Completed, this, &ASideCabestan::DropRollInput);
		}
		if (QuitAction)
		{
			Eic->BindAction(QuitAction, ETriggerEvent::Started, this, &ASideCabestan::HandleQuit);
		}
	}
}

void ASideCabestan::HandleYaw(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, "Handle Yaw");
	float YawValue = Value.Get<float>();
	switch (_Side)
	{
	case ESideCabestan::Toward:
		_CabestanController->ReceiveInputToward(YawValue);
		break;
	case ESideCabestan::Backward:
		_CabestanController->ReceiveInputBackward(YawValue);
		break;
	default:
		break;
	}
}

void ASideCabestan::DropRollInput()
{
	float RollValue = 0.f;
	switch (_Side)
	{
	case ESideCabestan::Toward:
		_CabestanController->ReceiveInputToward(RollValue);
		break;
	case ESideCabestan::Backward:
		_CabestanController->ReceiveInputBackward(RollValue);
		break;
	default:
		break;
	}
}

void ASideCabestan::DropYawInput()
{
	float YawValue = 0.f;
	switch (_Side)
	{
	case ESideCabestan::Toward:
		_CabestanController->ReceiveInputToward(YawValue);
		break;
	case ESideCabestan::Backward:
		_CabestanController->ReceiveInputBackward(YawValue);
		break;
	default:
		break;
	}
}

void ASideCabestan::PlayerInteracted(APlayerController* PlayerController)
{
}

void ASideCabestan::HandleRoll(const FInputActionValue& Value)
{
	float RollValue = Value.Get<float>();
	switch (_Side)
	{
	case ESideCabestan::Toward:
		_CabestanController->ReceiveInputToward(RollValue);
		break;
	case ESideCabestan::Backward:
		_CabestanController->ReceiveInputBackward(RollValue);
		break;
	default:
		break;
	}
}


void ASideCabestan::HandleQuit(const FInputActionValue& Value)
{
	if (InteractBox)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, "UnPossessed");
		InteractBox->UnpossessPawn();
	}
}


