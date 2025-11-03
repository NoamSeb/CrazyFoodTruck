// Fill out your copyright notice in the Description page of Project Settings.


#include "ShakeController.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UShakeController::UShakeController()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UShakeController::StartShake(ECameraShake ShakeType)
{
	if (_GameInstanceCrazyFoodTruck)
	{
		_GameInstanceCrazyFoodTruck->PlayerCameraShake(ShakeType);
	}
}


void UShakeController::BeginPlay()
{
	Super::BeginPlay();
	_GameInstanceCrazyFoodTruck = Cast<UGameInstanceCrazyFoodTruck>(UGameplayStatics::GetGameInstance(this));
	if (!_GameInstanceCrazyFoodTruck)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("TurretController: No GameInstanceCrazyFoodTruck found!"));
		return;
	}
}

void UShakeController::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

