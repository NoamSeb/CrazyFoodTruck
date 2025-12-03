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

	#pragma region Angle Cardinaux
	
	FVector CabestanPos = _CabestanController->GetActorLocation();
	FVector SidePos = this->GetActorLocation();

	FVector Direction = SidePos - CabestanPos;
	Direction.Z = 0;

	float Angle = FMath::Atan2(Direction.Y, Direction.X);
	float AngleDeg = FMath::RadiansToDegrees(Angle);
	if (AngleDeg < 0) AngleDeg += 360;   // 0 → 360

	
	if (AngleDeg >= 355.f || AngleDeg < 2.f)
	{
		LocationPose = ELocationPlayerCabestan::Nord;
	}
	else if (AngleDeg < 85.f)
	{
		LocationPose = ELocationPlayerCabestan::NordEst;
	}
	else if (AngleDeg < 95.f)
	{
		LocationPose = ELocationPlayerCabestan::Est;
	}
	else if (AngleDeg < 175.f)
	{
		LocationPose = ELocationPlayerCabestan::EstSud;
	}
	else if (AngleDeg < 185.f)
	{
		LocationPose = ELocationPlayerCabestan::Sud;
	}
	else if (AngleDeg < 265.f)
	{
		LocationPose = ELocationPlayerCabestan::SudOuest;
	}
	else if (AngleDeg < 275.f)
	{
		LocationPose = ELocationPlayerCabestan::Ouest;
	}
	else
	{
		LocationPose = ELocationPlayerCabestan::OuestNord;
	}
	#pragma endregion

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
		if (UpDownAction) // Y
		{
			Eic->BindAction(UpDownAction, ETriggerEvent::Triggered, this, &ASideCabestan::MoveY);
			Eic->BindAction(UpDownAction, ETriggerEvent::Canceled, this, &ASideCabestan::StopPush);
			Eic->BindAction(UpDownAction, ETriggerEvent::Completed, this, &ASideCabestan::StopPush);
		}
		if (RightLeftAction) // X
		{
			Eic->BindAction(RightLeftAction, ETriggerEvent::Triggered, this, &ASideCabestan::MoveX);
			Eic->BindAction(RightLeftAction, ETriggerEvent::Canceled, this, &ASideCabestan::StopPush);
			Eic->BindAction(RightLeftAction, ETriggerEvent::Completed, this, &ASideCabestan::StopPush);
		}
		if (QuitAction)
		{
			Eic->BindAction(QuitAction, ETriggerEvent::Started, this, &ASideCabestan::HandleQuit);
		}
	}
}

void ASideCabestan::MoveX(const FInputActionValue& Value)
{
	InputX = FMath::RoundToInt(Value.Get<float>());

	#pragma region Droite
	
	if (InputX == 1) //droite
	{
		switch (LocationPose)
		{
		case ELocationPlayerCabestan::Nord:
			Push(1);
			break;
		case ELocationPlayerCabestan::Sud:
			Bring(1);
			break;
		case ELocationPlayerCabestan::NordEst:
			Push(1);
			break;
		case ELocationPlayerCabestan::EstSud:
			Bring(1);
			break;
		case ELocationPlayerCabestan::OuestNord:
			Push(1);
			break;
		case ELocationPlayerCabestan::SudOuest:
			Bring(1);
			break;
		//case ELocationPlayerCabestan::Est:
		//	StopPush();
		//	break;
		//case ELocationPlayerCabestan::Ouest:
		//	StopPush();
		//	break;
		default:
			StopPush();
			break;
		}
	}
	
	#pragma endregion 

	#pragma region Gauche
	
	if (InputX == -1) //gauche
	{
		switch (LocationPose)
		{
		case ELocationPlayerCabestan::Nord:
			Bring(1);
			break;
		case ELocationPlayerCabestan::Sud:
			Push(1);
			break;
		case ELocationPlayerCabestan::NordEst:
			Bring(1);
			break;
		case ELocationPlayerCabestan::EstSud:
			Push(1);
			break;
		case ELocationPlayerCabestan::OuestNord:
			Bring(1);
			break;
		case ELocationPlayerCabestan::SudOuest:
			Push(1);
			break;
		//case ELocationPlayerCabestan::Est:
		//	StopPush();
		//	break;
		//case ELocationPlayerCabestan::Ouest:
		//	StopPush();
		//	break;
		default:
			StopPush();
			break;
		}
	}

	#pragma endregion 
}

void ASideCabestan::MoveY(const FInputActionValue& Value)
{
	InputY = FMath::RoundToInt(Value.Get<float>());

	#pragma region Haut
	
	if (InputY == 1) //haut
	{
		switch (LocationPose)
		{
		case ELocationPlayerCabestan::Est:
			Push(1);
			break;
		case ELocationPlayerCabestan::Ouest:
			Bring(1);
			break;
		case ELocationPlayerCabestan::NordEst:
			Bring(1);
			break;
		case ELocationPlayerCabestan::EstSud:
			Bring(1);
			break;
		case ELocationPlayerCabestan::OuestNord:
			Push(1);
			break;
		case ELocationPlayerCabestan::SudOuest:
			Push(1);
			break;
		//case ELocationPlayerCabestan::Nord:
		//	StopPush();
		//	break;
		//case ELocationPlayerCabestan::Sud:
		//	StopPush();
		//	break;
		default:
			StopPush();
			break;
		}
	}

	#pragma endregion 

	#pragma region Bas
	
	if (InputY == -1) //bas
	{
		switch (LocationPose)
		{
		case ELocationPlayerCabestan::Est:
			Bring(1);
			break;
		case ELocationPlayerCabestan::Ouest:
			Push(1);
			break;
		case ELocationPlayerCabestan::NordEst:
			 Push(1);
			break;
		case ELocationPlayerCabestan::EstSud:
			Push(1);
			break;
		case ELocationPlayerCabestan::OuestNord:
			 Bring(1);
			break;
		case ELocationPlayerCabestan::SudOuest:
			Bring(1);
			break;
		//case ELocationPlayerCabestan::Nord:
		//	StopPush();
		//	break;
		//case ELocationPlayerCabestan::Sud:
		//	StopPush();
		//	break;
		default:
			StopPush();
			break;
		}
	}

	#pragma endregion 
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Push");
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Bring");
	
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

