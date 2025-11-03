// Fill out your copyright notice in the Description page of Project Settings.


#include "Plate/PlateController.h"


// Sets default values
APlateController::APlateController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlateController::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlateController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float value = rightInput - leftInput;
	if (CanMove)
	{
		MovePlate(value);
	}

	if (rightSteak && leftSteak && !alreadyReturned)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Return Steak");
		alreadyReturned = true;
		ReturnSteak();
	}

	if (!rightSteak || !leftSteak)
	{
		alreadyReturned = false;
	}
}

void APlateController::ReceiveInputRightSteak(float value)
{
	rightInput = value;
}

void APlateController::ReceiveInputLeftSteak(float value)
{
	leftInput = value;
}

void APlateController::ReceiveInputRightSteak(bool value)
{
	rightSteak = value;
}


void APlateController::ReceiveInputLeftSteak(bool value)
{
	leftSteak = value;
}

void APlateController::ReturnSteak()
{
	if (Steak->IsReturn)
	Steak->IsReturn = false;
	else
	{
		Steak->IsReturn = true;
	}
}

void APlateController::MovePlate(float value)
{
	float targetSpeed = value * (Speed * GetWorld()->GetDeltaSeconds());
	FVector CurrentLocation = PlateActor->GetActorLocation();
	FVector newLocation = FVector(CurrentLocation.X + targetSpeed, CurrentLocation.Y, CurrentLocation.Z);
	PlateActor->SetActorLocation(newLocation);
}