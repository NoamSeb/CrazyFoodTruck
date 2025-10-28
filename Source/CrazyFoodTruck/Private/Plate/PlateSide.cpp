// Fill out your copyright notice in the Description page of Project Settings.


#include "Plate/PlateSide.h"
#include "Interactable/InteractBox.h"


// Sets default values
APlateSide::APlateSide()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlateSide::BeginPlay()
{
	Super::BeginPlay();

	if (InteractBox)
	{
		InteractBox->OnInteractionStarted.AddDynamic(this, &APlateSide::PlayerInteracted);
	}
}

// Called every frame
void APlateSide::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// Called to bind functionality to input
void APlateSide::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Eic = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (YawAction) // X
		{
			Eic->BindAction(YawAction, ETriggerEvent::Triggered, this, &APlateSide::HandleYaw);
			Eic->BindAction(YawAction, ETriggerEvent::Completed, this, &APlateSide::DropYawInput);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Yaw");
		}
		if (QuitAction)
		{
			Eic->BindAction(QuitAction, ETriggerEvent::Started, this, &APlateSide::HandleQuit);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Quit");
		}
	}
}


void APlateSide::HandleYaw(const FInputActionValue& Value)
{
	float YawValue = Value.Get<float>();
	YawValue = FMath::RoundToInt(YawValue);
	// PRINT VALUE %f
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("YAWABALUE : %f"), YawValue));
	switch (_Side)
	{
	case ESidePlate::Left:
		_PlateController->ReceiveInputRight(YawValue);
		break;
	case ESidePlate::Right:
		_PlateController->ReceiveInputLeft(YawValue);
		break;
	default:
		break;
	}
}

void APlateSide::DropYawInput()
{
	float YawValue = 0.f;
	switch (_Side)
	{
	case ESidePlate::Left:
		_PlateController->ReceiveInputRight(YawValue);
		break;
	case ESidePlate::Right:
		_PlateController->ReceiveInputLeft(YawValue);
		break;
	default:
		break;
	}
}

void APlateSide::PlayerInteracted(APlayerController* PlayerController)
{
	
}

void APlateSide::HandleQuit(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Quit");
	if (InteractBox)
	{
		InteractBox->UnpossessPawn();
	}
}


