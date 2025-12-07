// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetController.h"


// Sets default values
ATargetController::ATargetController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATargetController::BeginPlay()
{
	Super::BeginPlay();
	SetTargetPosition();
}

void ATargetController::SetTargetPosition()
{
	auto parent = GetAttachParentActor();
	FVector TruckLocation = parent->GetActorLocation();
	FVector Direction = FVector::Zero();
	float CurrentLocationZ = GetActorLocation().Z;
	
	switch (SideTarget)
	{
	case ESideTarget::Left:
		Direction = -parent->GetActorRightVector();
		break;
	case ESideTarget::Right:
		Direction = parent->GetActorRightVector();
		break;
	case  ESideTarget::Middle:
		Direction = parent->GetActorRightVector();
		break;
	}
	Direction.Normalize();
	FVector TargetLocation = (DistanceLenght * Direction) + TruckLocation;
	FVector FinalLocation = FVector(TargetLocation.X, TargetLocation.Y, CurrentLocationZ);
	SetActorLocation(FinalLocation);
}

// Called every frame
void ATargetController::Tick(float DeltaTime)
{

}

