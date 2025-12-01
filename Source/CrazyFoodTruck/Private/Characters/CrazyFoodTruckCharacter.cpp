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
#include "LocalMultiplayerSubsystem.h"
#include "InputCharacterAmelioration/InputAmeliorationCharacters.h"

static void BasisFromYaw(const float YawDeg, FVector& OutForward, FVector& OutRight)
{
    const FRotator Flat(0.f, YawDeg, 0.f);
    const FRotationMatrix RM(Flat);
    OutForward = RM.GetUnitAxis(EAxis::X);
    OutRight = RM.GetUnitAxis(EAxis::Y);
}

ACrazyFoodTruckCharacter::ACrazyFoodTruckCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

    if (UCharacterMovementComponent* Move = GetCharacterMovement())
    {
        Move->bOrientRotationToMovement = true;
        Move->RotationRate = FRotator(0.f, 540.f, 0.f);
        Move->MaxWalkSpeed = MovementSpeed;
    }
}

void ACrazyFoodTruckCharacter::BeginPlay()
{
	Super::BeginPlay();

    if (UCharacterMovementComponent* Move = GetCharacterMovement())
    {
        Move->MaxWalkSpeed = MovementSpeed;
    }

    GameDataSubSystem = GetGameInstance()->GetSubsystem<UGameDataSubSystem>();

    //UGameInstance* GI = GetGameInstance();
    //if (ULocalMultiplayerSubsystem* LMS = GI->GetSubsystem<ULocalMultiplayerSubsystem>())
    //{
    //    LMS->EnsurePlayerIMCs(ELocalMultiplayerInputMappingType::Player);
    //}

    UpdatePlayerColorFromController();
}

void ACrazyFoodTruckCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        BindInputMoveAction(EnhancedInputComponent);
        BindInputInteractAction(EnhancedInputComponent);
    }
    

    PlayerInputComp = PlayerInputComponent;
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

void ACrazyFoodTruckCharacter::RemoveMappingContext(UInputMappingContext* MappingContext)
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
        EnhancedInputLocalPlayerSubsystem->RemoveMappingContext(MappingContext);
    }
}

void ACrazyFoodTruckCharacter::AddMappingUpgrade()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "add");
    if (UInputAmeliorationCharacters* InputAmeliorationComp = FindComponentByClass<UInputAmeliorationCharacters>())
    {
        AddMappingContext(InputAmeliorationComp->MoveAmeliorationInputMappingContext, 10);
        InputAmeliorationComp->SetupPlayerInput(PlayerInputComp);
    }
}

void ACrazyFoodTruckCharacter::RemoveMappingUpgrade()
{
    if (UInputAmeliorationCharacters* InputAmeliorationComp = FindComponentByClass<UInputAmeliorationCharacters>())
    {
        RemoveMappingContext(InputAmeliorationComp->MoveAmeliorationInputMappingContext);
    }
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

void ACrazyFoodTruckCharacter::BindInputMoveAction(UEnhancedInputComponent* EnhancedInputComponent)
{
    if (InputData && InputData->InputActionMove)
    {
        EnhancedInputComponent->BindAction(InputData->InputActionMove, ETriggerEvent::Triggered, this, &ACrazyFoodTruckCharacter::OnInputMove);
    }
}

void ACrazyFoodTruckCharacter::OnInputMove(const FInputActionValue& InputActionValue)
{
    if (GameDataSubSystem->CurrentGamePhase != EPhaseGameCrazyFoodTruckState::Amelioration)
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
                if (VehicleRefActor.IsValid())
                {
                    const float Yaw = VehicleRefActor->GetActorRotation().Yaw + MovementYawOffsetDegrees;
                    BasisFromYaw(Yaw, Forward, Right);
                }
                else
                {
                    const APlayerController* PC = Cast<APlayerController>(Controller);
                    const float Yaw = (PC ? PC->GetControlRotation().Yaw : GetActorRotation().Yaw) + MovementYawOffsetDegrees;
                    BasisFromYaw(Yaw, Forward, Right);
                }
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
    if (GameDataSubSystem->CurrentGamePhase != EPhaseGameCrazyFoodTruckState::Amelioration)
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

const TScriptInterface<IInteractable>& ACrazyFoodTruckCharacter::GetFocusedInteractable() const
{
    return FocusedInteractable;
}

void ACrazyFoodTruckCharacter::SetFocusedInteractable(const TScriptInterface<IInteractable>& NewTarget)
{
    FocusedInteractable = NewTarget;
}

int32 ACrazyFoodTruckCharacter::GetPlayerIndex() const
{
    const APlayerController* PC = Cast<APlayerController>(Controller);
    if (!PC)
    {
        return -1;
    }

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
    const int32 ControllerId = GetPlayerIndex();
    if (ControllerId < 0)
    {
        PlayerColor = FLinearColor::White;
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        PlayerColor = FLinearColor::White;
        return;
    }

    if (UGameInstanceCrazyFoodTruck* GI = World->GetGameInstance<UGameInstanceCrazyFoodTruck>())
    {
        PlayerColor = GI->GetPlayerColorForControllerId(ControllerId);
    }
    else
    {
        PlayerColor = FLinearColor::White;
    }

    //TArray<UMeshComponent*> MeshComponents;
    //GetComponents<UMeshComponent>(MeshComponents);

    //for (UMeshComponent* MeshComp : MeshComponents)
    //{
    //    if (!MeshComp)
    //    {
    //        continue;
    //    }

    //    const int32 MatCount = MeshComp->GetNumMaterials();
    //    for (int32 MatIndex = 0; MatIndex < MatCount; ++MatIndex)
    //    {
    //        if (UMaterialInstanceDynamic* MID = MeshComp->CreateAndSetMaterialInstanceDynamic(MatIndex))
    //        {
    //            MID->SetVectorParameterValue(TEXT("PlayerColor"), PlayerColor);
    //        }
    //    }
    //}
}
