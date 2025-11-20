
#include "Cabestan/CabestanController.h"


ACabestanController::ACabestanController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACabestanController::BeginPlay()
{
	Super::BeginPlay();
}

void ACabestanController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float value = towardInput - backwardInput;
	AddRotationInput(value);
	if (LinkedTurretController)
	{
		LinkedTurretController->AddRotationInput(value);
	}
}

bool ACabestanController::CanPush()
{
	FRotator CurrentRotation = GetActorRotation();
	if (CurrentRotation.Yaw >= AngleMax)
	{
		return false;
	}
	return true;
}

bool ACabestanController::CanBring()
{
	FRotator CurrentRotation = GetActorRotation();
	if (CurrentRotation.Yaw <= AngleMin)
	{
		return false;
	}
	return true;
}

void ACabestanController::ReceiveInputToward(float value)
{
	towardInput = value;
}

void ACabestanController::ReceiveInputBackward(float value)
{
	backwardInput = value;
}

void ACabestanController::AddRotationInput(float value)
{
	FRotator CurrentRotation = GetActorRotation();
	float turnValue = value * RotationSpeed * GetWorld()->GetDeltaSeconds();
	FRotator NewRotation = FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw + turnValue, CurrentRotation.Roll);
	SetActorRotation(NewRotation);
}
