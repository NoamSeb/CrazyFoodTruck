// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/CrazyFoodTruckCharacter.h"

#include "Characters/CrazyFoodTruckCharacterInputData.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "LocalMultiplayerSettings.h"

// Sets default values
ACrazyFoodTruckCharacter::ACrazyFoodTruckCharacter()
{
 	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
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

    UpdatePlayerColorFromController();
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
    
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        BindInputMoveAction(EnhancedInputComponent);
        BindInputInteractAction(EnhancedInputComponent);
    }

    if (UInputAmeliorationCharacters* InputAmeliorationComp = FindComponentByClass<UInputAmeliorationCharacters>())
    {
        AddMappingContext(InputAmeliorationComp->MoveAmeliorationInputMappingContext, 10);
        InputAmeliorationComp->SetupPlayerInput(PlayerInputComponent);
    }
}

int32 ACrazyFoodTruckCharacter::GetPlayerIndex() const
{
    const APlayerController* PlayerController = Cast<APlayerController>(Controller);
    if (!PlayerController)
    {
        return -1;
    }

    const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
    return LocalPlayer ? LocalPlayer->GetControllerId() : -1;
}

FLinearColor ACrazyFoodTruckCharacter::GetPlayerColor() const
{
    return PlayerColor;
}

void ACrazyFoodTruckCharacter::SetPlayerColor(FLinearColor NewColor)
{
    PlayerColor = NewColor;
}

void ACrazyFoodTruckCharacter::UpdatePlayerColorFromController()
{
    switch (GetPlayerIndex())
    {
    case 0:
        PlayerColor = FLinearColor(0.0f, 0.45f, 1.0f);
        break;
    case 1:
        PlayerColor = FLinearColor(0.1f, 0.8f, 0.2f); 
        break;
    case 2:
        PlayerColor = FLinearColor(1.0f, 0.1f, 0.1f); 
        break;
    case 3:
        PlayerColor = FLinearColor(1.0f, 0.9f, 0.1f);
        break;
    default:
        PlayerColor = FLinearColor::White;
        break;
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

    const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
    if (!LocalPlayer)
    {
        return;
    }

    if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
    {
        EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);
    }
}

void ACrazyFoodTruckCharacter::AddMappingContext(UInputMappingContext* InputMappingContextParam, int8 Priority)
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

void ACrazyFoodTruckCharacter::BindInputMoveAction(UEnhancedInputComponent* EnhancedInputComponent)
{
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

void ACrazyFoodTruckCharacter::BindInputInteractAction(UEnhancedInputComponent* EnhancedInputComponent)
{
    if (InputData->InputActionInteract)
    {
        EnhancedInputComponent->BindAction(InputData->InputActionInteract, ETriggerEvent::Started, this, &ACrazyFoodTruckCharacter::TryInteract);
    }
}

void ACrazyFoodTruckCharacter::TryInteract()
{
    if (!FocusedInteractable)
    {
        if (GEngine)
        {
        }
        
        return;
    }

    UObject* Object = FocusedInteractable.GetObject();
    if (!Object)
    {
        return;
    }

    APlayerController* PlayerController = Cast<APlayerController>(Controller);
    if (!PlayerController)
    {
        return;
    }


    if (Object->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
    {
        FocusedInteractable->Interact(PlayerController, this);
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

// INTERFACE
void ACrazyFoodTruckCharacter::SetInteractState(bool bCanInteract)
{
    _canInteract = bCanInteract;
}

bool ACrazyFoodTruckCharacter::CanInteract()
{
    return _canInteract;
}

void ACrazyFoodTruckCharacter::SetAmmoState(bool bHasAmmo)
{
    _hasAmmo = bHasAmmo;
}

void ACrazyFoodTruckCharacter::TakeAmmoBox(AAmmoBox* AmmoBox)
{
    _carriedAmmoBox = AmmoBox;
    SetAmmoState(true);
}

AAmmoBox* ACrazyFoodTruckCharacter::DepositAmmoBox()
{
    return  _carriedAmmoBox;
}


bool ACrazyFoodTruckCharacter::HasAmmo()
{
    return _hasAmmo;
}
