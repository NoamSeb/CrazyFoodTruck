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
	if (!TargetPawn)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("TargetController : TargetPawn is not assigned !"));
	}
}

// Called every frame
void ATargetController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!TargetPawn){return;}

	FVector TruckLocation = TargetPawn->GetActorLocation();
	FVector Direction = FVector::Zero();
	float CurrentLocationZ = GetActorLocation().Z;
	
	switch (SideTarget)
	{
	case ESideTarget::Left:
		Direction = -TargetPawn->GetActorRightVector();
		break;
	case ESideTarget::Right:
		Direction = TargetPawn->GetActorRightVector();
		break;
	}
	Direction.Normalize();
	FVector TargetLocation = (DistanceLenght * Direction) + TruckLocation;
	FVector FinalLocation = FVector(TargetLocation.X, TargetLocation.Y, CurrentLocationZ);
	SetActorLocation(FinalLocation);
}

