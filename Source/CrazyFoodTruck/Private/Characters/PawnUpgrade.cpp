// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PawnUpgrade.h"


// Sets default values
APawnUpgrade::APawnUpgrade()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APawnUpgrade::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APawnUpgrade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APawnUpgrade::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    
	if (UInputAmeliorationCharacters* InputAmeliorationComp = FindComponentByClass<UInputAmeliorationCharacters>())
	{
		AddMappingContext(InputAmeliorationComp->MoveAmeliorationInputMappingContext, 10);
		InputAmeliorationComp->SetupPlayerInput(PlayerInputComponent);
	}
}


void APawnUpgrade::AddMappingContext(UInputMappingContext* InputMappingContextParam, int8 Priority)
{
	const APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController)
	{
		return;
	}

	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (!LocalPlayer)
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContextParam, Priority);
	}
}

