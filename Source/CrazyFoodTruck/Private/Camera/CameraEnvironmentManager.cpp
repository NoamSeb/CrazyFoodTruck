// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera/CameraEnvironmentManager.h"

#include "CrazyFoodTruck/Data/Public/GameInstanceCrazyFoodTruck.h"
#include "Camera/ECameraShake.h"

#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

ACameraEnvironmentManager::ACameraEnvironmentManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACameraEnvironmentManager::BeginPlay()
{
	Super::BeginPlay();

	CFTGameInstance = Cast<UGameInstanceCrazyFoodTruck>(UGameplayStatics::GetGameInstance(this));

	if (!TargetSpringArm && TruckActor)
	{
		TargetSpringArm = TruckActor->FindComponentByClass<USpringArmComponent>();
	}

	if (TargetSpringArm)
	{
		DefaultRelativeLocation = TargetSpringArm->GetRelativeLocation();
		DefaultRelativeRotation = TargetSpringArm->GetRelativeRotation();
		DefaultArmLength = TargetSpringArm->TargetArmLength;
	}
}

void ACameraEnvironmentManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!TargetSpringArm || !bEnableEnvironmentMode)
	{
		return;
	}

	if (bIsBlending)
	{
		UpdateBlend(DeltaTime);
	}
}

void ACameraEnvironmentManager::ActivateDynamicCamera(const FEnvCameraPreset& Preset)
{
	if (!TargetSpringArm || !bEnableEnvironmentMode)
	{
		return;
	}

	ActivePreset = Preset;

	StartBlend(true, ActivePreset.BlendTime);

	if (bPlayShakeOnActivate && CFTGameInstance)
	{
		CFTGameInstance->PlayerCameraShake(ECameraShake::CalmAmbient);
	}
}

void ACameraEnvironmentManager::DeactivateDynamicCamera(float BlendTimeOverride)
{
	if (!TargetSpringArm || !bEnableEnvironmentMode)
	{
		return;
	}

	const float Duration = (BlendTimeOverride > 0.f) ? BlendTimeOverride : DefaultBlendBackTime;

	StartBlend(false, Duration);
}

void ACameraEnvironmentManager::StartBlend(bool bTowardsDynamic, float BlendDuration)
{
	if (!TargetSpringArm)
	{
		return;
	}

	bIsBlending = true;
	bBlendToDynamic = bTowardsDynamic;
	BlendTimer = 0.f;
	CurrentBlendDuration = FMath::Max(BlendDuration, 0.05f);

	StartRelLocation = TargetSpringArm->GetRelativeLocation();
	StartRelRotation = TargetSpringArm->GetRelativeRotation();
	StartArmLength = TargetSpringArm->TargetArmLength;
}

void ACameraEnvironmentManager::UpdateBlend(float DeltaTime)
{
	if (!TargetSpringArm)
	{
		bIsBlending = false;
		return;
	}

	BlendTimer += DeltaTime;
	const float Alpha = FMath::Clamp(BlendTimer / CurrentBlendDuration, 0.f, 1.f);

	ApplyBlend(Alpha, bBlendToDynamic);

	if (Alpha >= 1.f)
	{
		bIsBlending = false;
	}
}

void ACameraEnvironmentManager::ApplyBlend(float Alpha, bool bTowardsDynamic)
{
	if (!TargetSpringArm)
	{
		return;
	}

	FVector TargetLocation;
	FRotator TargetRotation;
	float TargetArmLength;

	if (bTowardsDynamic)
	{
		TargetLocation = ActivePreset.RelativeLocation;
		TargetRotation = ActivePreset.RelativeRotation;
		TargetArmLength = ActivePreset.TargetArmLength;
	}
	else
	{
		TargetLocation = DefaultRelativeLocation;
		TargetRotation = DefaultRelativeRotation;
		TargetArmLength = DefaultArmLength;
	}

	const FVector NewLocation = FMath::Lerp(StartRelLocation, TargetLocation, Alpha);
	const FRotator NewRotation = FMath::Lerp(StartRelRotation, TargetRotation, Alpha);
	const float NewArmLength = FMath::Lerp(StartArmLength, TargetArmLength, Alpha);

	TargetSpringArm->SetRelativeLocation(NewLocation);
	TargetSpringArm->SetRelativeRotation(NewRotation);
	TargetSpringArm->TargetArmLength = NewArmLength;
}

void ACameraEnvironmentManager::DebugMoveCamera(const FVector& LocalOffset, float DeltaTime)
{
	if (!bEnableDebugCamera || !TargetSpringArm) return;

	TargetSpringArm->AddRelativeLocation(LocalOffset * DebugMoveSpeed * DeltaTime);
}

void ACameraEnvironmentManager::DebugRotateCamera(const FRotator& DeltaRot, float DeltaTime)
{
	if (!bEnableDebugCamera || !TargetSpringArm) return;

	TargetSpringArm->AddRelativeRotation(DeltaRot * DebugRotateSpeed * DeltaTime);
}

void ACameraEnvironmentManager::DebugZoomCamera(float AxisValue, float DeltaTime)
{
	if (!bEnableDebugCamera || !TargetSpringArm) return;

	TargetSpringArm->TargetArmLength = FMath::Clamp(TargetSpringArm->TargetArmLength + AxisValue * DebugZoomSpeed * DeltaTime, 100.f, 10000.f);
}

void ACameraEnvironmentManager::DebugResetCamera()
{
	if (!TargetSpringArm) return;

	TargetSpringArm->SetRelativeLocation(DefaultRelativeLocation);
	TargetSpringArm->SetRelativeRotation(DefaultRelativeRotation);
	TargetSpringArm->TargetArmLength = DefaultArmLength;
}
