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

	FVector CabestanCenter = _CabestanController->GetActorLocation();
	FVector PlanchePos = this->GetActorLocation();

	FVector Dir = PlanchePos - CabestanCenter;
	Dir.Z = 0;

	float Angle = FMath::Atan2(Dir.Y, Dir.X);
	float AngleDeg = FMath::RadiansToDegrees(Angle);
	if (AngleDeg < 0) AngleDeg += 360;   // 0 → 360

	if (AngleDeg < 90)
		LocationPose = ELocationPlayerCabestan::NordEst;
	else if (AngleDeg < 180)
		LocationPose = ELocationPlayerCabestan::EstSud;
	else if (AngleDeg < 270)
		LocationPose = ELocationPlayerCabestan::SudOuest;
	else
		LocationPose = ELocationPlayerCabestan::OuestNord;

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
		if (YawAction) // Y
		{
			Eic->BindAction(YawAction, ETriggerEvent::Triggered, this, &ASideCabestan::MoveY);
			Eic->BindAction(YawAction, ETriggerEvent::Canceled, this, &ASideCabestan::StopPush);
			Eic->BindAction(YawAction, ETriggerEvent::Completed, this, &ASideCabestan::StopPush);
		}
		if (RollAction) // X
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
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("pos : %d"), LocationPose));

	InputX = FMath::RoundToInt(Value.Get<float>());
	

	
	if (InputX == 1 && LocationPose == ELocationPlayerCabestan::NordEst)
	{
		Push(Value);
	}
	if (InputX == 1 && LocationPose == ELocationPlayerCabestan::EstSud)
	{
		Bring(Value);
	}

	if (InputX == -1 && LocationPose == ELocationPlayerCabestan::SudOuest)
	{
		Push(1);
	}
	
	if (InputX == -1 && LocationPose == ELocationPlayerCabestan::OuestNord)
	{
		Bring(1);
	}
	
	
	//InputX = Value.Get<float>();
}

void ASideCabestan::MoveY(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, FString::Printf(TEXT("pos : %f"), InputY));

	InputY = FMath::RoundToInt(Value.Get<float>());
	
	if (InputY == -1 && LocationPose == ELocationPlayerCabestan::NordEst)
	{
		Bring(1);
	}

	if (InputY == -1 && LocationPose == ELocationPlayerCabestan::OuestNord)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, "A Push dans MoveY");
		Push(1);
	}

	if (InputY == 1 && LocationPose == ELocationPlayerCabestan::EstSud)
	{
		Push(1);
	}

	if (InputY == 1 && LocationPose == ELocationPlayerCabestan::SudOuest)
	{
		Bring(1);
	}

	//InputY = Value.Get<float>();
}



void ASideCabestan::StopPush()
{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Stop Push");
	
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

