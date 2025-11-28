// Fill out your copyright notice in the Description page of Project Settings.


#include "SideMutionDrawer.h"

#include "EnhancedInputComponent.h"


// Sets default values
ASideMutionDrawer::ASideMutionDrawer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASideMutionDrawer::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASideMutionDrawer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASideMutionDrawer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// PLAYER ENTERED
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Eic = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (OpenAction)
		{
			Eic->BindAction(OpenAction, ETriggerEvent::Triggered, this, &ASideMutionDrawer::HandleOpen);
		}
		if (QuitAction)
		{
			Eic->BindAction(QuitAction, ETriggerEvent::Started, this, &ASideMutionDrawer::QuitDrawer);
		}
	}
	if (LinkedMunitionDrawer)
	{
		LinkedMunitionDrawer->IncrementPlayerReloading();
	}
}

void ASideMutionDrawer::QuitDrawer()
{
	if (InteractBox)
	{
		InteractBox->UnpossessPawn();
	}
	if (LinkedMunitionDrawer)
	{
		LinkedMunitionDrawer->DecrementPlayerReloading();
	}
}
void ASideMutionDrawer::HandleOpen(const FInputActionValue& Value)
{
	if (!LinkedMunitionDrawer){return;}
	float value = Value.Get<float>();
	value = FMath::RoundToInt(value);
	if (value > 0)
	{
		LinkedMunitionDrawer->ReceiveInputOpen(value);
	}
}

