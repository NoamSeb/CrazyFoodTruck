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


	// 1) On lit le joystick (tu dois déjà stocker InputX et InputY quelque part)
	InputX = FMath::RoundToInt(InputX);
	InputY = FMath::RoundToInt(InputY);
	
	FVector2D Input(InputX, InputY);
	if (Input.IsNearlyZero())
	{
		StopPush();
		return; // rien si joystick neutre
	}
	
	Input.Normalize();
	
	float AngleRad = FMath::Atan2(Input.Y, Input.X);
	float AngleDeg = -FMath::RadiansToDegrees(AngleRad); // inversion du sens (ton problème initial)
	
	float SideYaw = GetActorRotation().Yaw;
	float Delta = FMath::FindDeltaAngleDegrees(SideYaw, AngleDeg);
	
	Delta = FMath::RoundToInt(Delta);
	//StopPush();
	// 5) Décision :
	//    Delta < 0  → joystick pousse dans le sens horaire → PUSH
	//    Delta > 0  → joystick tire dans l'anti-horaire   → BRING
	
	if (Delta < 0.f)
	{
		Push(FInputActionValue());
	}
	else
	{
		Bring(FInputActionValue());
	}
	
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
			Eic->BindAction(YawAction, ETriggerEvent::Triggered, this, &ASideCabestan::MoveY);
			Eic->BindAction(YawAction, ETriggerEvent::Canceled, this, &ASideCabestan::StopPush);
			Eic->BindAction(YawAction, ETriggerEvent::Completed, this, &ASideCabestan::StopPush);
		}
		if (RollAction) // Y
		{
			//Eic->BindAction(RollAction, ETriggerEvent::Triggered, this, &ASideCabestan::TurnCabestan);
			Eic->BindAction(RollAction, ETriggerEvent::Triggered, this, &ASideCabestan::MoveX);
			Eic->BindAction(RollAction, ETriggerEvent::Canceled, this, &ASideCabestan::StopPush);
			Eic->BindAction(RollAction, ETriggerEvent::Completed, this, &ASideCabestan::StopPush);
		}
		if (QuitAction)
		{
			Eic->BindAction(QuitAction, ETriggerEvent::Started, this, &ASideCabestan::HandleQuit);
		}
	}
}

void ASideCabestan::MoveX(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "MoveX");
	InputX = FMath::RoundToInt(Value.Get<float>());
	//InputX = Value.Get<float>();
}

void ASideCabestan::MoveY(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "MoveX");
	InputY = FMath::RoundToInt(Value.Get<float>());
	//InputY = Value.Get<float>();
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
	if (FMath::RoundToInt(Value.Get<float>()) >= 1)
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
}

void ASideCabestan::Bring(const FInputActionValue& Value)
{
	if (FMath::RoundToInt(Value.Get<float>()) >= 1)
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

