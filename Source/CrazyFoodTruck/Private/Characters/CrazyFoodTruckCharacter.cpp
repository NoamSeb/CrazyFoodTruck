// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CrazyFoodTruckCharacter.h"

#include "Characters/CrazyFoodTruckCharacterInputData.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"

// Sets default values
ACrazyFoodTruckCharacter::ACrazyFoodTruckCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 540.f, 0);
}

// Called when the game starts or when spawned
void ACrazyFoodTruckCharacter::BeginPlay()
{
	Super::BeginPlay();

    if (UCharacterMovementComponent* Move = GetCharacterMovement())
    {
        Move->MaxWalkSpeed = MovementSpeed;
    }
}

// Called every frame
void ACrazyFoodTruckCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACrazyFoodTruckCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    // SetupMappingContextIntoController();

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        BindInputMoveAction(EnhancedInputComponent);
        BindInputInteract(EnhancedInputComponent);
    }
}

void ACrazyFoodTruckCharacter::SetInputData(UCrazyFoodTruckCharacterInputData* InInputData)
{
    InputData = InInputData;
}

void ACrazyFoodTruckCharacter::SetInputMappingContext(UInputMappingContext* InInputMappingContext)
{
    InputMappingContext = InInputMappingContext;
}

void ACrazyFoodTruckCharacter::SetupMappingContextIntoController() const
{
    const APlayerController* PlayerController = Cast<APlayerController>(Controller);
    if (!PlayerController)
    {
        return;
    }

    const ULocalPlayer* Player = PlayerController->GetLocalPlayer();
    if (!Player)
    {
        return;
    }

    if (UEnhancedInputLocalPlayerSubsystem* InputSystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
    {
        if (InputMappingContext)
        {
            InputSystem->AddMappingContext(InputMappingContext, 0);
        }
    }
}

void ACrazyFoodTruckCharacter::BindInputMoveAction(UEnhancedInputComponent* EnhancedInputComponent)
{
    if (!InputData)
    {
        return;
    }

    if (InputData->InputActionMove)
    {
        EnhancedInputComponent->BindAction(InputData->InputActionMove, ETriggerEvent::Triggered, this, &ACrazyFoodTruckCharacter::OnInputMove);
    }
}

void ACrazyFoodTruckCharacter::OnInputMove(const FInputActionValue& InputActionValue)
{
    if (InputActionValue.GetValueType() == EInputActionValueType::Axis2D)
    {
        FVector2D Axis = InputActionValue.Get<FVector2D>();

        constexpr float Deadzone = 0.20f;
        if (Axis.SizeSquared() < Deadzone * Deadzone)
        {
            return;
        }

        const FRotator ControlRot(0.f, GetControlRotation().Yaw, 0.f);
        
        const FVector Forward = FRotationMatrix(ControlRot).GetUnitAxis(EAxis::X);
        const FVector Right = FRotationMatrix(ControlRot).GetUnitAxis(EAxis::Y);

        AddMovementInput(Forward, Axis.Y);
        AddMovementInput(Right, Axis.X);
    }
}

void ACrazyFoodTruckCharacter::BindInputInteract(UEnhancedInputComponent* EnhancedInputComponent)
{
    if (!InputData)
    {
        return;
    }

    if (InputData->InputActionInteract)
    {
        EnhancedInputComponent->BindAction(InputData->InputActionInteract, ETriggerEvent::Started, this, &ACrazyFoodTruckCharacter::TryInteract);
    }
}

void ACrazyFoodTruckCharacter::TryInteract()
{
    if (FocusedInteractable)
    {
        IInteractable::Execute_Interact(FocusedInteractable.GetObject(), this);
    }
    else
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, TEXT("Nothing to interact with here."));
        }
    }
}

const TScriptInterface<IInteractable>& ACrazyFoodTruckCharacter::GetFocusedInteractable() const
{
    return FocusedInteractable;
}

void ACrazyFoodTruckCharacter::SetFocusedInteractable(const TScriptInterface<IInteractable>& NewTarget)
{
    FocusedInteractable = NewTarget;
}

