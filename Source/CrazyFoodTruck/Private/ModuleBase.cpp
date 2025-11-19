// Fill out your copyright notice in the Description page of Project Settings.


#include "ModuleBase.h"

#include <string>

#include "UWModule.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AModuleBase::AModuleBase()
{
	PrimaryActorTick.bCanEverTick = false;
	ModuleWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ModuleWidget"));
	auto UC = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(UC);
	ModuleWidgetComponent->SetupAttachment(RootComponent);
}

void AModuleBase::ResetModule()
{
}

void AModuleBase::BeginPlay()
{
	Super::BeginPlay();
	WidgetModuleClass = Cast<UUWModule>(ModuleWidgetComponent->GetWidget());
	if (!WidgetModuleClass)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Module Widget Class is null"));
	}
}

void AModuleBase::Interact(APlayerController* InstigatorPlayerController, ACrazyFoodTruckCharacter* CrazyCharacter)
{
	ActualCooldown = BaseCooldown;
}

bool AModuleBase::CanInteractWithModule() const
{
	if (ActualCooldown > 0.f)
	{
		return false;
	}
	return true;
}

void AModuleBase::Tick(float DeltaTime)
{
	if (ActualCooldown > 0.f)
	{
		ActualCooldown -= DeltaTime;
		IIUWModule::Execute_UpdateSliderCooldown(WidgetModuleClass, ActualCooldown, BaseCooldown);
		TurnWidgetTowardCamera();
		if (ActualCooldown <= 0.f)
        {
            IIUWModule::Execute_CoolDownComplete(WidgetModuleClass);
        }
	}
	Super::Tick(DeltaTime);
}

void AModuleBase::TurnWidgetTowardCamera()
{
	// GETCAMERA
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FVector CameraLocation = CameraManager->GetCameraLocation();
	FVector WidgetRotation = ModuleWidgetComponent->GetComponentLocation();
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(WidgetRotation, CameraLocation);
	ModuleWidgetComponent->SetWorldRotation(LookAtRotation);
}
