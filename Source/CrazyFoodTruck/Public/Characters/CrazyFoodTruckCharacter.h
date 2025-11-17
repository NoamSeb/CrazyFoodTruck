// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "Interactable/Interactable.h"
#include "Interface/IPlayer.h"
#include "InputCharacterAmelioration/InputAmeliorationCharacters.h"
#include "CrazyFoodTruckCharacter.generated.h"

class UCrazyFoodTruckCharacterInputData;
class AAmmoBox;

class UEnhancedInputComponent;
class UEnhancedInputLocalPlayerSubsystem;

UENUM(BlueprintType)
enum class EMovementFrame : uint8
{
	World   UMETA(DisplayName = "World"),
	Vehicle UMETA(DisplayName = "Vehicle")
};

UCLASS()
class CRAZYFOODTRUCK_API ACrazyFoodTruckCharacter : public ACharacter, public IIPlayer
{
	GENERATED_BODY()

#pragma region Unreal Default

public:
	// Sets default values for this character's properties
	ACrazyFoodTruckCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// IIPlayer Interface
	virtual void SetInteractState(bool bCanInteract) override;
	virtual bool CanInteract() override;
	virtual void SetAmmoState(bool bHasAmmo) override;
	virtual bool HasAmmo() override;

	// Helpers
	void TakeAmmoBox(AAmmoBox* AmmoBox);
	AAmmoBox* DepositAmmoBox();

	// Player Infos
	UFUNCTION(BlueprintCallable, Category = "Player")
	int32 GetPlayerIndex() const;

	UFUNCTION(BlueprintCallable, Category = "Player")
	FLinearColor GetPlayerColor() const;

	UFUNCTION(BlueprintCallable, Category = "Player")
	void SetPlayerColor(FLinearColor NewColor);

	// Input Data / Mapping
	void SetInputData(UCrazyFoodTruckCharacterInputData* InInputData);
	void SetInputMappingContext(UInputMappingContext* InInputMappingContext);
	void SetupMappingContextIntoController() const;

	// Movement
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementSpeed = 600.f;

	UPROPERTY(EditAnywhere, Category = "Movement|Frame")
	EMovementFrame MovementFrame = EMovementFrame::Vehicle;

	UPROPERTY(EditAnywhere, Category = "Movement|Frame")
	float MovementYawOffsetDegrees = 0.f;

	UFUNCTION(BlueprintCallable, Category = "Movement|Frame")
	void SetVehicleMovementRef(AActor* InVehicleActor);

	UFUNCTION(BlueprintCallable, Category = "Movement|Frame")
	void UseWorldFrame();

	UFUNCTION(BlueprintCallable, Category = "Movement|Frame")
	void UseVehicleFrame(AActor* InVehicle);

	// Interact
	const TScriptInterface<IInteractable>& GetFocusedInteractable() const;
	void SetFocusedInteractable(const TScriptInterface<IInteractable>& NewTarget);

	void AddMappingContext(UInputMappingContext* InputMappingContext, int8 Priority);

protected:
	UPROPERTY(EditAnywhere, Category = "Player")
	FLinearColor PlayerColor = FLinearColor::White;

	UPROPERTY()
	TObjectPtr<UCrazyFoodTruckCharacterInputData> InputData = nullptr;

	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;

	UPROPERTY(Transient)
	TWeakObjectPtr<AActor> VehicleRefActor;

	UPROPERTY()
	TScriptInterface<IInteractable> FocusedInteractable;
	

private:
	bool bCanInteractInternal = true;
	bool bHasAmmoInternal = false;
	
	UPROPERTY(Transient)
	TObjectPtr<AAmmoBox> CarriedAmmoBox = nullptr;

	void BindInputMoveAction(UEnhancedInputComponent* EnhancedInputComponent);
	void BindInputInteractAction(UEnhancedInputComponent* EnhancedInputComponent);

	void OnInputMove(const FInputActionValue& InputActionValue);
	void TryInteract();

	void UpdatePlayerColorFromController();
};
