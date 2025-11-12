// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/CrazyFoodTruckCharacter.h"

#include "Characters/CrazyFoodTruckCharacterInputData.h"
#include "AmmoBox.h"
#include "LocalMultiplayerSettings.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ACrazyFoodTruckCharacter::ACrazyFoodTruckCharacter()
{
 	// Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    if (UCharacterMovementComponent* Move = GetCharacterMovement())
    {
        Move->bOrientRotationToMovement = true;
        Move->RotationRate = FRotator(0.f, 540.f, 0.f);
        Move->MaxWalkSpeed = MovementSpeed;
    }
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
}

int32 ACrazyFoodTruckCharacter::GetPlayerIndex() const
{
    const APlayerController* PC = Cast<APlayerController>(Controller);
    if (!PC) return -1;

    const ULocalPlayer* LP = PC->GetLocalPlayer();
    return LP ? LP->GetControllerId() : -1;
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

static void BasisFromYaw(const float YawDeg, FVector& OutForward, FVector& OutRight)
{
    const FRotator Flat(0.f, YawDeg, 0.f);
    const FRotationMatrix RM(Flat);
    OutForward = RM.GetUnitAxis(EAxis::X);
    OutRight = RM.GetUnitAxis(EAxis::Y);
}

void ACrazyFoodTruckCharacter::OnInputMove(const FInputActionValue& InputActionValue)
{
    if (InputActionValue.GetValueType() != EInputActionValueType::Axis2D) return;

    const FVector2D Raw = InputActionValue.Get<FVector2D>();

    constexpr float Deadzone = 0.20f;
    if (Raw.SizeSquared() < Deadzone * Deadzone) return;

    const float X = Raw.X;
    const float Y = Raw.Y;

    FVector Forward, Right;

    switch (MovementFrame)
    {
    case EMovementFrame::Vehicle:
    {
        if (!VehicleRefActor.IsValid()) return;
        const float Yaw = VehicleRefActor->GetActorRotation().Yaw + MovementYawOffsetDegrees;
        BasisFromYaw(Yaw, Forward, Right);
        break;
    }
    case EMovementFrame::World:
    default:
    {
        const APlayerController* PC = Cast<APlayerController>(Controller);
        const float Yaw = (PC ? PC->GetControlRotation().Yaw : GetActorRotation().Yaw) + MovementYawOffsetDegrees;
        BasisFromYaw(Yaw, Forward, Right);
        break;
    }
    }

    AddMovementInput(Forward, Y);
    AddMovementInput(Right, X);
}

void ACrazyFoodTruckCharacter::BindInputMoveAction(UEnhancedInputComponent* EnhancedInputComponent)
{
    if (InputData && InputData->InputActionMove)
    {
        EnhancedInputComponent->BindAction(InputData->InputActionMove, ETriggerEvent::Triggered, this, &ACrazyFoodTruckCharacter::OnInputMove);
    }
}

void ACrazyFoodTruckCharacter::BindInputInteractAction(UEnhancedInputComponent* EnhancedInputComponent)
{
    if (InputData && InputData->InputActionInteract)
    {
        EnhancedInputComponent->BindAction(InputData->InputActionInteract, ETriggerEvent::Started, this, &ACrazyFoodTruckCharacter::TryInteract);
    }
}

void ACrazyFoodTruckCharacter::TryInteract()
{
    if (!FocusedInteractable) return;

    UObject* Obj = FocusedInteractable.GetObject();
    if (!Obj) return;

    APlayerController* PC = Cast<APlayerController>(Controller);
    if (!PC) return;

    if (Obj->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
    {
        FocusedInteractable->Interact(PC, this);
    }
}

void ACrazyFoodTruckCharacter::SetInteractState(bool bCanInteract)
{
    bCanInteractInternal = bCanInteract;
}

bool ACrazyFoodTruckCharacter::CanInteract()
{
    return bCanInteractInternal;
}


void ACrazyFoodTruckCharacter::SetAmmoState(bool bHasAmmo)
{
    bHasAmmoInternal = bHasAmmo;
}

bool ACrazyFoodTruckCharacter::HasAmmo()
{
    return bHasAmmoInternal;
}

void ACrazyFoodTruckCharacter::TakeAmmoBox(AAmmoBox* AmmoBox)
{
    CarriedAmmoBox = AmmoBox;
    SetAmmoState(true);
}

AAmmoBox* ACrazyFoodTruckCharacter::DepositAmmoBox()
{
    return CarriedAmmoBox;
}

void ACrazyFoodTruckCharacter::SetVehicleMovementRef(AActor* InVehicleActor)
{
    VehicleRefActor = InVehicleActor;
}

void ACrazyFoodTruckCharacter::UseWorldFrame()
{
    MovementFrame = EMovementFrame::World;
    VehicleRefActor = nullptr;
}

void ACrazyFoodTruckCharacter::UseVehicleFrame(AActor* InVehicle)
{
    MovementFrame = EMovementFrame::Vehicle;
    VehicleRefActor = InVehicle;
}

const TScriptInterface<IInteractable>& ACrazyFoodTruckCharacter::GetFocusedInteractable() const
{
    return FocusedInteractable;
}

void ACrazyFoodTruckCharacter::SetFocusedInteractable(const TScriptInterface<IInteractable>& NewTarget)
{
    FocusedInteractable = NewTarget;
}
