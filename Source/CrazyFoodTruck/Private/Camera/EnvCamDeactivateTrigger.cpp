// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera/EnvCamDeactivateTrigger.h"

#include "Camera/CameraEnvironmentManager.h"
#include "CrazyFoodTruck/Public/Vehicle/Vehicle.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

AEnvCamDeactivateTrigger::AEnvCamDeactivateTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->SetGenerateOverlapEvents(true);
}

void AEnvCamDeactivateTrigger::BeginPlay()
{
	Super::BeginPlay();

	if (TriggerBox)
	{
		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AEnvCamDeactivateTrigger::OnTriggerBegin);
	}

	if (!EnvManager)
	{
		if (UWorld* World = GetWorld())
		{
			TArray<AActor*> FoundManagers;
			UGameplayStatics::GetAllActorsOfClass(World, ACameraEnvironmentManager::StaticClass(), FoundManagers);

			if (FoundManagers.Num() > 0)
			{
				EnvManager = Cast<ACameraEnvironmentManager>(FoundManagers[0]);
			}
		}
	}
}

void AEnvCamDeactivateTrigger::OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!EnvManager || !OtherActor)
	{
		return;
	}

	if (OtherActor->IsA(AVehicle::StaticClass()))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				2.f,
				FColor::Red,
				TEXT("EnvCam DEACTIVATE: Truck entered deactivate trigger")
			);
		}

		EnvManager->DeactivateDynamicCamera(BlendBackTime);
	}
}
