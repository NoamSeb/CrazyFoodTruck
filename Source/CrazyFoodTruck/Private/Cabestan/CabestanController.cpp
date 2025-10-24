// Fill out your copyright notice in the Description page of Project Settings.


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
	FRotator NewRotation = FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw + value, CurrentRotation.Roll);
	SetActorRotation(NewRotation);
}

