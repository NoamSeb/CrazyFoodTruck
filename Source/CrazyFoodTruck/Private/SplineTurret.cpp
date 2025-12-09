// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineTurret.h"

#include "MathUtil.h"

ASplineTurret::ASplineTurret()
{
	PrimaryActorTick.bCanEverTick = true;

	TurretSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Turret Spline"));
}

void ASplineTurret::BeginPlay()
{
	Super::BeginPlay();
}

void ASplineTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float ASplineTurret::GetSplineLenght() const
{
	return TurretSpline->GetSplineLength();
}


void ASplineTurret::GetTurretLocationAlongSpline(float DistanceAlongSpline, FVector& OutLocation, FRotator& OutRotation) const
{
	OutLocation = TurretSpline->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	FRotator TargetRotation = TurretSpline->GetRotationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
	TargetRotation.Yaw += 90.f; // Adjust for turret orientation
	OutRotation = TargetRotation;
}

