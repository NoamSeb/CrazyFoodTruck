// Fill out your copyright notice in the Description page of Project Settings.


#include "Cabestan/SideCabestan.h"

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
	bPlayerIn = true;
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* Eic = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (PushAction)
		{
			Eic->BindAction(PushAction, ETriggerEvent::Triggered, this, &ASideCabestan::Push);
			Eic->BindAction(PushAction, ETriggerEvent::Completed, this, &ASideCabestan::StopPush);
		}
		if (BringAction)
        {
            Eic->BindAction(BringAction, ETriggerEvent::Triggered, this, &ASideCabestan::Bring);
			Eic->BindAction(BringAction, ETriggerEvent::Completed, this, &ASideCabestan::StopPush);
        }
		if (YawAction) // X
		{
		}
		if (RollAction) // Y
		{
		}
		if (QuitAction)
		{
			Eic->BindAction(QuitAction, ETriggerEvent::Started, this, &ASideCabestan::HandleQuit);
		}
	}
}

void ASideCabestan::StopPush()
{
	switch (_Side)
	{
	case ESideCabestan::Toward:
		_CabestanController->ReceiveInputToward(0);
		break;
	case ESideCabestan::Backward:
		_CabestanController->ReceiveInputBackward(0);
		break;
	default:
		break;
	}
}

void ASideCabestan::Push(const FInputActionValue& Value)
{
	if (!bPlayerIn){return;}

	if (!_CabestanController->CanPush())
	{
		_CabestanController->ReceiveInputToward(0);
		_CabestanController->ReceiveInputBackward(0);
		return;
	}

	switch (_Side)
	{
	case ESideCabestan::Toward:
		_CabestanController->ReceiveInputToward(1);
		break;
	case ESideCabestan::Backward:
		_CabestanController->ReceiveInputBackward(-1);
		break;
	default:
		break;
	}
}

void ASideCabestan::Bring(const FInputActionValue& Value)
{
	if (!bPlayerIn){return;}
	if (!_CabestanController->CanBring())
	{
		_CabestanController->ReceiveInputToward(0);
		_CabestanController->ReceiveInputBackward(0);
		return;
	}

	switch (_Side)
	{
	case ESideCabestan::Toward:
		_CabestanController->ReceiveInputToward(-1);
		break;
	case ESideCabestan::Backward:
		_CabestanController->ReceiveInputBackward(1);
		break;
	default:
		break;
	}
}

void ASideCabestan::HandleYaw(const FInputActionValue& Value)
{
	float YawValue = Value.Get<float>();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Yaw Value: %f"), YawValue));
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
		bPlayerIn = false;
		StopPush();
		InteractBox->UnpossessPawn();
	}
}

