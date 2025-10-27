// Fill out your copyright notice in the Description page of Project Settings.


#include "Plate/PlateSide.h"

#include "EnhancedInputComponent.h"
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
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("meh"));
			Eic->BindAction(YawAction, ETriggerEvent::Triggered, this, &APlateSide::HandleYaw);
			Eic->BindAction(YawAction, ETriggerEvent::Completed, this, &APlateSide::DropYawInput);
		}
		if (QuitAction)
		{
			Eic->BindAction(QuitAction, ETriggerEvent::Started, this, &APlateSide::HandleQuit);
		}
	}
}


void APlateSide::HandleYaw(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, "Handle Yaw");
	float YawValue = Value.Get<float>();
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Drop Yaw Input"));
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
	if (InteractBox)
	{
		InteractBox->UnpossessPawn();
	}
}


