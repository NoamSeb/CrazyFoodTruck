// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "Interactable/Interactable.h"
#include "Interface/IPlayer.h"

#include "CrazyFoodTruck/Data/Public/GameDataSubSystem.h"

#include "CrazyFoodTruckCharacter.generated.h"

class UCrazyFoodTruckCharacterInputData;
class AAmmoBox;

class UEnhancedInputComponent;
class UEnhancedInputLocalPlayerSubsystem;

UENUM(BlueprintType)
enum class EMovementFrame : uint8
{
	World   UMETA(DisplayName = "World"),
	Vehicle UMETA(DisplayName = "Vehicle"),
	Camera  UMETA(DisplayName = "Camera")
};

/// Enum to separate differents anims characters has
/// Idle | Walk | Push
UENUM(BlueprintType)
enum class EMovementType : uint8
{
	ECC_Idle   UMETA(DisplayName = "Idle"),
	ECC_Run UMETA(DisplayName = "Run"),
	ECC_Push UMETA(DisplayName = "Push")
};

UCLASS()
class CRAZYFOODTRUCK_API ACrazyFoodTruckCharacter : public ACharacter, public IIPlayer
{
	GENERATED_BODY()

public:
	ACrazyFoodTruckCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; 

public:

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementSpeed = 600.f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	EMovementType MovementType = EMovementType::ECC_Idle;

	UPROPERTY(EditAnywhere, Category = "Movement|Frame")
	EMovementFrame MovementFrame = EMovementFrame::Camera;

	UPROPERTY(EditAnywhere, Category = "Movement|Frame")
	float MovementYawOffsetDegrees = 0.f;

	UPROPERTY(EditAnywhere, Category = "Movement|Rotation")
	float RotationInterpSpeed = 10.f;

	UFUNCTION(BlueprintCallable, Category = "Movement|Frame")
	void SetVehicleMovementRef(AActor* InVehicleActor);

	UFUNCTION(BlueprintCallable, Category = "Movement|Frame")
	void UseWorldFrame();

	UFUNCTION(BlueprintCallable, Category = "Movement|Frame")
	void UseVehicleFrame(AActor* InVehicle);

	UFUNCTION(BlueprintCallable, Category = "Movement|Frame")
	void UseCameraFrame();

	void SetInputData(UCrazyFoodTruckCharacterInputData* InInputData);
	void SetInputMappingContext(UInputMappingContext* InInputMappingContext);

	void SetupMappingContextIntoController() const;

	virtual void SetInteractState(bool bCanInteract) override;
	virtual bool CanInteract() override;
	
	virtual void SetAmmoState(bool bHasAmmo) override;
	virtual bool HasAmmo() override;

	void TakeAmmoBox(AAmmoBox* AmmoBox);
	
	AAmmoBox* DepositAmmoBox();

	const TScriptInterface<IInteractable>& GetFocusedInteractable() const;
	void SetFocusedInteractable(const TScriptInterface<IInteractable>& NewTarget);

	UFUNCTION(BlueprintCallable, Category = "Player")
	int32 GetPlayerIndex() const;

	UFUNCTION(BlueprintCallable, Category = "Player")
	FLinearColor GetPlayerColor() const;

	UFUNCTION(BlueprintCallable, Category = "Player")
	void SetPlayerColor(FLinearColor NewColor);

protected:
	UPROPERTY(EditAnywhere, Category = "Player")
	FLinearColor PlayerColor = FLinearColor::White;

private:
	UPROPERTY()
	TObjectPtr<UCrazyFoodTruckCharacterInputData> InputData = nullptr;

	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;

	UPROPERTY(Transient)
	TWeakObjectPtr<AActor> VehicleRefActor;

	UPROPERTY()
	TScriptInterface<IInteractable> FocusedInteractable;

	bool bCanInteractInternal = true;
	bool bHasAmmoInternal = false;
	
	UPROPERTY(Transient)
	TObjectPtr<AAmmoBox> CarriedAmmoBox = nullptr;

	UGameDataSubSystem* GameDataSubSystem;

	UInputComponent* PlayerInputComp;

	FVector LastMovementDirection = FVector::ZeroVector;

	// APlayerController* GetPlayerControllerFromActor(AActor* Actor) const;
	// int32 GetPlayerIndexFromPlayerController(APlayerController* PlayerController) const;
	
	void AddMappingContext(UInputMappingContext* InputMappingContext, int8 Priority);
	void RemoveMappingContext(UInputMappingContext* InputMappingContext);
	
	UFUNCTION(BlueprintCallable, Category = "Upgrade Phase")
	void AddMappingUpgrade();
	
	UFUNCTION(BlueprintCallable, Category = "Upgrade Phase")
	void RemoveMappingUpgrade();

	void BindInputMoveAction(UEnhancedInputComponent* EnhancedInputComponent);
	void BindInputInteractAction(UEnhancedInputComponent* EnhancedInputComponent);

	void OnInputMove(const FInputActionValue& InputActionValue);
	void OnStopMove();
	void TryInteract();

	void UpdatePlayerColorFromController();
};
