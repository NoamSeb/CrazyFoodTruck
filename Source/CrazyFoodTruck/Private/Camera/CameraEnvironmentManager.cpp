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
		DefaultArmLength = TargetSpringArm->TargetArmLength;
		DefaultRelativeLocation = TargetSpringArm->GetRelativeLocation();
		DefaultRelativeRotation = TargetSpringArm->GetRelativeRotation();
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

	StartArmLength = TargetSpringArm->TargetArmLength;
	StartRelLocation = TargetSpringArm->GetRelativeLocation();
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

	float TargetArmLength;
	FVector TargetLocation;

	if (bTowardsDynamic)
	{
		TargetArmLength = ActivePreset.TargetArmLength;
		TargetLocation = ActivePreset.RelativeLocation;
	}
	else
	{
		TargetArmLength = DefaultArmLength;
		TargetLocation = DefaultRelativeLocation;
	}

	const float NewArmLength = FMath::Lerp(StartArmLength, TargetArmLength, Alpha);
	const FVector NewLocation = FMath::Lerp(StartRelLocation, TargetLocation, Alpha);

	TargetSpringArm->TargetArmLength = NewArmLength;
	TargetSpringArm->SetRelativeLocation(NewLocation);

	if (!bTowardsDynamic)
	{
		TargetSpringArm->SetRelativeRotation(DefaultRelativeRotation);
	}
}
