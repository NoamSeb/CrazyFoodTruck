// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera/CameraEnvironmentManager.h"

#include "CrazyFoodTruck/Data/Public/GameInstanceCrazyFoodTruck.h"
#include "../HordeManager.h"
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

	FindHordeManager();
}

void ACameraEnvironmentManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!TargetSpringArm || !bEnableEnvironmentMode)
	{
		return;
	}

	UpdateState(DeltaTime);
}

void ACameraEnvironmentManager::FindHordeManager()
{
	if (!GetWorld()) return;

	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHordeManager::StaticClass(), Found);
	if (Found.Num() > 0)
	{
		HordeManager = Cast<AHordeManager>(Found[0]);
	}
}

bool ACameraEnvironmentManager::HasAnyZombie() const
{
	if (!HordeManager) return true;
	return HordeManager->ListHordeZombie.Num() > 0;
}

void ACameraEnvironmentManager::UpdateState(float DeltaTime)
{
	const bool bHasZombies = HasAnyZombie();

	if (bInEnvironmentMode && bHasZombies)
	{
		ExitEnvironmentMode();
		return;
	}

	if (!bInEnvironmentMode && !bHasZombies && EnvCameraKeys.Num() > 0)
	{
		EnterEnvironmentMode();
	}

	if (!bInEnvironmentMode)
	{
		return;
	}

	FEnvCameraKey& CurrentKey = EnvCameraKeys[CurrentKeyIndex];

	if (BlendTimer < BlendTime)
	{
		BlendTimer += DeltaTime;
		const float Alpha = FMath::Clamp(BlendTimer / BlendTime, 0.f, 1.f);
		ApplyBlend(Alpha);
		return;
	}

	HoldTimer += DeltaTime;
	if (HoldTimer >= CurrentKey.HoldTime)
	{
		CurrentKeyIndex = (CurrentKeyIndex + 1) % EnvCameraKeys.Num();
		StartBlendToCurrentKey();
	}
}

void ACameraEnvironmentManager::EnterEnvironmentMode()
{
	bInEnvironmentMode = true;
	CurrentKeyIndex = 0;
	BlendTimer = 0.f;
	HoldTimer = 0.f;
	bHasTriggeredCalmShake = false;

	StartBlendToCurrentKey();

	if (CFTGameInstance && !bHasTriggeredCalmShake)
	{
		CFTGameInstance->PlayerCameraShake(ECameraShake::CalmAmbient);
		bHasTriggeredCalmShake = true;
	}
}

void ACameraEnvironmentManager::ExitEnvironmentMode()
{
	bInEnvironmentMode = false;
	bHasTriggeredCalmShake = false;
	RestoreDefaultPose();
}

void ACameraEnvironmentManager::StartBlendToCurrentKey()
{
	if (!TargetSpringArm || EnvCameraKeys.Num() == 0) return;

	BlendTimer = 0.f;
	HoldTimer = 0.f;

	StartArmLength = TargetSpringArm->TargetArmLength;
	StartRelLocation = TargetSpringArm->GetRelativeLocation();
	StartRelRotation = TargetSpringArm->GetRelativeRotation();
}

void ACameraEnvironmentManager::ApplyBlend(float Alpha)
{
	if (!TargetSpringArm || EnvCameraKeys.Num() == 0) return;

	const FEnvCameraKey& Key = EnvCameraKeys[CurrentKeyIndex];

	const float NewArmLength = FMath::Lerp(StartArmLength, Key.TargetArmLength, Alpha);
	const FVector NewLocation = FMath::Lerp(StartRelLocation, Key.RelativeLocation, Alpha);

	TargetSpringArm->TargetArmLength = NewArmLength;
	TargetSpringArm->SetRelativeLocation(NewLocation);
}

void ACameraEnvironmentManager::RestoreDefaultPose()
{
	if (!TargetSpringArm) return;

	TargetSpringArm->TargetArmLength = DefaultArmLength;
	TargetSpringArm->SetRelativeLocation(DefaultRelativeLocation);
	TargetSpringArm->SetRelativeRotation(DefaultRelativeRotation);
}
