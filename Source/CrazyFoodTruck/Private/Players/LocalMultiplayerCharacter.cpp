// Fill out your copyright notice in the Description page of Project Settings.


#include "Players/LocalMultiplayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputActionValue.h"

// Sets default values
ALocalMultiplayerCharacter::ALocalMultiplayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 540.f, 0);
}

// Called when the game starts or when spawned
void ALocalMultiplayerCharacter::BeginPlay()
{
	Super::BeginPlay();

    if (UCharacterMovementComponent* Move = GetCharacterMovement())
    {
        Move->MaxWalkSpeed = MovementSpeed;
    }
}

// Called every frame
void ALocalMultiplayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ALocalMultiplayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (IA_Move)
        {
            EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ALocalMultiplayerCharacter::OnInputMove);
            EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Ongoing, this, &ALocalMultiplayerCharacter::OnInputMove);
        }
    }
}

void ALocalMultiplayerCharacter::OnInputMove(const FInputActionValue& InputActionValue)
{
    const FVector2D Axis = InputActionValue.Get<FVector2D>();

    const FRotator ControlRot(0.f, GetControlRotation().Yaw, 0.f);

    const FVector Forward = FRotationMatrix(ControlRot).GetUnitAxis(EAxis::X);
    const FVector Right = FRotationMatrix(ControlRot).GetUnitAxis(EAxis::Y);

    AddMovementInput(Forward, Axis.Y);
    AddMovementInput(Right, Axis.X);
}

