// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputMappingContext.h"
#include "Vehicle.generated.h"

class UCurveFloat;
class UInputAction;
class UEnhancedInputComponent;
class UEnhancedInputLocalPlayerSubsystem;
class UFloatingPawnMovement;
class AInteractBox;

UENUM()
enum class VehicleStates
{
	Idle,
	Rotating
};
UENUM()
enum class VehicleOrientation
{
	Left,
	Right
};

UCLASS()
class CRAZYFOODTRUCK_API AVehicle : public APawn
{
	GENERATED_BODY()

public:
	AVehicle();

	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

private:
	// ===== Components =====
	UPROPERTY()
	TObjectPtr<UFloatingPawnMovement> MovementComponent;

public:
	// ===== Interact =====
	UPROPERTY(EditAnywhere, Category = "Interact")
	TObjectPtr<AInteractBox> InteractBox;

	// ===== Input Data =====
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> FoodTruckInputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> TurnTruckAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> QuitTruckAction;

private:
	void BindInputRotateZAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent);
	void InputQuitTruck(const FInputActionValue& InputActionValue);
	void SetupMappingContextIntoController() const;

	// ===== Vehicle Settings =====
public:
	// Speed
	UPROPERTY(EditAnywhere, meta=(ToolTip="Variable in kilometers per hour", ForceUnits="km/h"), Category="Vehicle Settings | Speed ")
	float TruckMaxSpeed = 50.f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Variable in kilometers per hour", ForceUnits="km/h"), Category="Vehicle Settings | Speed")
	float TruckLossSpeed = 10.f;
	
	UPROPERTY(EditAnywhere, meta=(Tooltip="Represent the time the truck need to recover his full speed", Units="seconds"), Category="Vehicle Settings | Speed")
	float SpeedRecoveryDuration = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Vehicle Settings | Speed", meta = (ToolTip = "Durée de maintien de la vitesse après Possess (s)", Units = "seconds"))
	float HoldSpeedDuration = 0.08f;
	
	// Rotation (Yaw)
	UPROPERTY(EditAnywhere, meta=(ToolTip="Represent the speed of rotation of the Truck per frame", Units="Degrees"), Category="Vehicle Settings")
	float TruckAngleSpeed = 1.f;
	
	UPROPERTY(EditAnywhere, meta=(ToolTip="Represent the max rotation of the truck", Units="Degrees"), Category="Vehicle Settings | Rotation")
	float TruckMaxRotation = 25.f;
	
	UPROPERTY(EditAnywhere, meta=(ToolTip="Represent the time the truck need to rotate", Units="seconds"), Category="Vehicle Settings | Rotation")
	float TruckInterpolationDuration = 1.f;
	
	UPROPERTY(EditAnywhere, Category="Vehicle Settings | Rotation")
	TObjectPtr<UCurveFloat> RotationAnimCurve;
	
	// Tilt (Roll)
	UPROPERTY(EditAnywhere, meta=(ToolTip="Represent the max rotation of the truck", Units="Degrees"), Category="Vehicle Settings | Tilt")
	float TruckMaxTilt = 10.f;
	
	UPROPERTY(EditAnywhere, meta=(ToolTip="Represent the time the truck need to tilt", Units="seconds"), Category="Vehicle Settings | Tilt")
	float TruckInterpolationTilt = 1;

	UPROPERTY(EditAnywhere, Category="Vehicle Settings | Tilt")
	TObjectPtr<UCurveFloat> TiltAnimCurve;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Custom")
	void ChangeMap();

private:
	// ===== Runtime State =====
	static constexpr float KilometersToMetersConvertingValue = 27.777777777778f;

	VehicleStates TruckState;
	VehicleOrientation TruckOrientation;

	float InputRotatingValue;
		
	float RotationTimer;
	float TiltTimer;
	float StartRotationYaw;
	float StartRotationRoll;
	bool AlreadyPassed = false;

	FRotator destinationRotation;

	FTimerHandle SpeedRecoveryHandle;
	float ElapsedTime = 0.0f;
	bool bRecoveringSpeed = false;
	float StartSpeed = 0.0f;

	FVector SavedLinearVelocity = FVector::ZeroVector;
	FVector PossessKeepVelocity = FVector::ZeroVector;
	bool bHoldSpeedAfterPossess = false;
	float HoldSpeedTimer = 0.f;

private:
	UFUNCTION()
	void MoveForward();

	void SetTruckRotatingStates(const FInputActionValue& InputActionValue);
	void SetTruckIdleStates();

	void RotateTruck(float DeltaTime);
	void UpdateRotationTruck(FRotator TargetRotation, float DeltaTime);
	void ResetTruckTilt(float DeltaTime);

	void ReduceSpeed();
	void StartSpeedRecovery();
};
