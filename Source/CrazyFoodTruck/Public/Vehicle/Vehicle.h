// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Vehicle.generated.h"

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
	// Sets default values for this pawn's properties
	AVehicle();
	
	UPROPERTY()
	TObjectPtr<UFloatingPawnMovement> MovementComponent;

	UPROPERTY(EditAnywhere, Category = "Variable")
	AInteractBox* InteractBox;
	
#pragma region Vehicle settings
	
#pragma region Speed settings
	UPROPERTY(EditAnywhere, meta=(ToolTip="Variable in kilometers per hour", ForceUnits="km/h"), Category="Vehicle Settings | Speed ")
	float TruckMaxSpeed = 50.f;

	UPROPERTY(EditAnywhere, meta=(ToolTip="Variable in kilometers per hour", ForceUnits="km/h"), Category="Vehicle Settings | Speed")
	float TruckLossSpeed = 10.f;
	
	UPROPERTY(EditAnywhere, meta=(Tooltip="Represent the time the truck need to recover his full speed", Units="seconds"), Category="Vehicle Settings | Speed")
	float SpeedRecoveryDuration = 2.0f;
	
#pragma endregion
	
	UPROPERTY(EditAnywhere, meta=(ToolTip="Represent the speed of rotation of the Truck per frame", Units="Degrees"), Category="Vehicle Settings")
	float TruckAngleSpeed = 1.f;
	
	UPROPERTY(EditAnywhere, meta=(ToolTip="Represent the max rotation of the truck", Units="Degrees"), Category="Vehicle Settings | Rotation")
	float TruckMaxRotation = 25.f;
	
	UPROPERTY(EditAnywhere, meta=(ToolTip="Represent the time the truck need to rotate", Units="seconds"), Category="Vehicle Settings | Rotation")
	float TruckInterpolationDuration = 1.f;
	
	UPROPERTY(EditAnywhere, Category="Vehicle Settings | Rotation")
	TObjectPtr<UCurveFloat> RotationAnimCurve;
	
	UPROPERTY(EditAnywhere, meta=(ToolTip="Represent the max rotation of the truck", Units="Degrees"), Category="Vehicle Settings | Tilt")
	float TruckMaxTilt = 10.f;
	
	UPROPERTY(EditAnywhere, meta=(ToolTip="Represent the time the truck need to tilt", Units="seconds"), Category="Vehicle Settings | Tilt")
	float TruckInterpolationTilt = 1;

	UPROPERTY(EditAnywhere, Category="Vehicle Settings | Tilt")
	TObjectPtr<UCurveFloat> TiltAnimCurve;

#pragma endregion
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private :
	VehicleStates TruckState;
	VehicleOrientation TruckOrientation;
	float InputRotatingValue;
	float KilometersToMetersConvertingValue = 27.777777777778;
		
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

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UFUNCTION()
	void MoveForward();

	UFUNCTION(BlueprintImplementableEvent, Category = "Custom")
	void ChangeMap();

#pragma region Input Data
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input");
	TObjectPtr<UInputMappingContext> FoodTruckInputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input");
	TObjectPtr<UInputAction> TurnTruckAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input YAY")
	TObjectPtr<UInputAction> QuitTruckAction ;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void SetupMappingContextIntoController() const;

#pragma endregion
	
#pragma region  Input Rotate Truck
private:
	void BindInputRotateZAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent);

#pragma region Input Quit Truck
	void InputQuitTruck(const FInputActionValue& InputActionValue);
#pragma endregion

#pragma region Truck State
	void SetTruckRotatingStates(const FInputActionValue& InputActionValue);
	void SetTruckIdleStates();
#pragma endregion
	
	void RotateTruck(float DeltaTime);
	void UpdateRotationTruck(FRotator TargetRotation, float DeltaTime);
	void ResetTruckTilt(float DeltaTime);
#pragma endregion

#pragma region Truck Speed Management
private:
	void ReduceSpeed();
	void StartSpeedRecovery();
#pragma endregion
};
