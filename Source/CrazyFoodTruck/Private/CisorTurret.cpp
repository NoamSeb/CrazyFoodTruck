
#include "CisorTurret.h"

ACisorTurret::ACisorTurret()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACisorTurret::BeginPlay()
{
	Super::BeginPlay();
}

void ACisorTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACisorTurret::TurretChangePosition(FVector NewPosition, float currentAlpha)
{
	FVector Direction = NewPosition - GetActorLocation();
 	FRotator TargetRotation = Direction.Rotation();
 	TargetRotation.Pitch = 0.f;
 	TargetRotation.Roll = 0.f;
 	SetActorRotation(TargetRotation);
	
	if (!TurretCurve){return;}
	float CurveValue = TurretCurve->GetFloatValue(currentAlpha);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(" Value : %f, Alpha %f"), CurveValue, currentAlpha));
	SetAnimationAlpha(CurveValue);
}

