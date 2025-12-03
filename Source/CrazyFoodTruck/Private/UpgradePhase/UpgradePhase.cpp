// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradePhase/UpgradePhase.h"
#include "LocalMultiplayerSubsystem.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AUpgradePhase::AUpgradePhase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AUpgradePhase::BeginPlay()
{
	Super::BeginPlay();

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>())
		{

			TArray<AActor*> FoundControllers;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), FoundControllers);


			for (AActor* FoundController : FoundControllers)
			{
				APlayerController* PlayerController = Cast<APlayerController>(FoundController);
				const int32 PlayerIndex = GetPlayerIndexFromPlayerController(PlayerController);
				if (UInputMappingContext* IMC = LocalMultiplayerSubsystem->GetGamepadIMC(MappingType))
				{
					LocalMultiplayerSubsystem->AddTemporaryMappingForPlayer(PlayerIndex, IMC, 100, true);
				}
			}
		}
	}
	
}

// Called every frame
void AUpgradePhase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int32 AUpgradePhase::GetPlayerIndexFromPlayerController(APlayerController* PlayerController) const
{
	if (!PlayerController) return -1;
	if (const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
	{
		return LocalPlayer->GetControllerId();
	}
	return -1;
}