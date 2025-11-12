// Fill out your copyright notice in the Description page of Project Settings.


#include "ModuleBase.h"

#include <string>

#include "UWModule.h"

AModuleBase::AModuleBase()
{
	PrimaryActorTick.bCanEverTick = false;
	ModuleWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ModuleWidget"));
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Module Interacted"));
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
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, "Y");
	if (ActualCooldown > 0.f)
	{
		ActualCooldown -= DeltaTime;
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, "X");
		IIUWModule::Execute_UpdateSliderCooldown(WidgetModuleClass, ActualCooldown);
	}
	Super::Tick(DeltaTime);
}
