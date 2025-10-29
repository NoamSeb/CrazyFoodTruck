// Fill out your copyright notice in the Description page of Project Settings.


#include "CrazyFoodTruck/Public/Vehicle/Vehicle.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AVehicle::AVehicle()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AVehicle::BeginPlay()
{
	Super::BeginPlay();
	MovementComponent = Cast<UFloatingPawnMovement>(this->GetMovementComponent());
	MovementComponent->MaxSpeed = TruckMaxSpeed * KilometersToMetersConvertingValue;
	//GetWorld()->GetFirstPlayerController()->Possess(this);
}

// Called every frame
void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveForward();

	switch (TruckState)
	{
		case VehicleStates::Idle:
			ResetTruckTilt(DeltaTime);
			break;
		case VehicleStates::Rotating:
			RotateTruck(DeltaTime);
			break;
		default:
			break;
	}

	if (bRecoveringSpeed)
	{
		ElapsedTime += DeltaTime;
		float Alpha = FMath::Clamp(ElapsedTime / SpeedRecoveryDuration, 0.f, 1.f);
		
		MovementComponent->MaxSpeed = FMath::Lerp(TruckMaxSpeed * KilometersToMetersConvertingValue - TruckLossSpeed * KilometersToMetersConvertingValue, TruckMaxSpeed * KilometersToMetersConvertingValue, Alpha);

		if (Alpha >= 1.0f)
		{
			bRecoveringSpeed = false;
		}
	}
}

void AVehicle::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (!OtherActor) return;

	if (OtherActor->Tags.Contains("Obstacle"))
	{
		OtherActor->Destroy();
		ReduceSpeed();
	}
	else if (OtherActor->Tags.Contains("MapSwitch"))
	{
		ChangeMap();
	}
}
void AVehicle::MoveForward()
{
	AddMovementInput(GetActorForwardVector(),1);
}


#pragma region Input
void AVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	SetupMappingContextIntoController();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent == nullptr) return;

	BindInputRotateZAxisAndActions(EnhancedInputComponent);
}

void AVehicle::SetupMappingContextIntoController() const
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController == nullptr) return;

	ULocalPlayer* Player = PlayerController->GetLocalPlayer();
	if (Player == nullptr) return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (InputSystem == nullptr) return;

	// replace 0 by the player who takes control of the FoodTruck
	InputSystem->AddMappingContext(FoodTruckInputMappingContext, 0);
}

#pragma endregion

#pragma region Input Rotate Truck

void AVehicle::BindInputRotateZAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (FoodTruckInputMappingContext == nullptr) return;

	if (TurnTruckAction)
	{
		EnhancedInputComponent->BindAction(TurnTruckAction, ETriggerEvent::Triggered, this, &AVehicle::SetTruckRotatingStates);
		EnhancedInputComponent->BindAction(TurnTruckAction, ETriggerEvent::Completed, this, &AVehicle::SetTruckIdleStates);
	}
}

#pragma region Truck States
void AVehicle::SetTruckRotatingStates(const FInputActionValue& InputActionValue)
{
	
	TruckState = VehicleStates::Rotating;
	InputRotatingValue = InputActionValue.Get<float>();
	if (!AlreadyPassed)
	{
		RotationTimer = 0.f;
		StartRotationYaw = GetActorRotation().Yaw;
		StartRotationRoll = GetActorRotation().Roll;
	}
	if (InputRotatingValue > 0 )
	{
		TruckOrientation = VehicleOrientation::Right;
	}
	else
	{
		TruckOrientation = VehicleOrientation::Left;
	}
	AlreadyPassed = true;
}

void AVehicle::SetTruckIdleStates()
{
	TruckState = VehicleStates::Idle;
	TiltTimer = 0.f;
	StartRotationRoll = GetActorRotation().Roll;
	AlreadyPassed = false;
}
#pragma endregion

void AVehicle::RotateTruck(float DeltaTime)
{
	destinationRotation.Yaw += (InputRotatingValue * TruckAngleSpeed) * DeltaTime;
	destinationRotation.Yaw = FMath::Clamp(destinationRotation.Yaw, -TruckMaxRotation, TruckMaxRotation);

	destinationRotation.Roll += ((InputRotatingValue * TruckAngleSpeed)/2) * DeltaTime;
	destinationRotation.Roll = FMath::Clamp(destinationRotation.Roll, -TruckMaxTilt, TruckMaxTilt);
	
	UpdateRotationTruck(destinationRotation, DeltaTime);
}

void AVehicle::UpdateRotationTruck(FRotator TargetRotation, float DeltaTime)
{
	if(RotationAnimCurve == nullptr) return;
	
	if(RotationTimer < TruckInterpolationDuration)
		RotationTimer += DeltaTime;

	float RotationPercent = RotationTimer / TruckInterpolationDuration;
	
	RotationPercent = RotationAnimCurve->GetFloatValue(RotationPercent);
	
	FRotator actorRotation = GetActorRotation();
	actorRotation.Yaw = FMath::Lerp(StartRotationYaw, TargetRotation.Yaw, RotationPercent);
	actorRotation.Roll = FMath::Lerp(StartRotationRoll, TargetRotation.Roll, RotationPercent);

	SetActorRotation(actorRotation);
}

void AVehicle::ResetTruckTilt(float DeltaTime)
{
	if(TiltAnimCurve == nullptr) return;
	
	if(TiltTimer < TruckInterpolationTilt)
		TiltTimer += DeltaTime;

	float TiltPercent = TiltTimer / TruckInterpolationTilt;
	
	TiltPercent = TiltAnimCurve->GetFloatValue(TiltPercent);
	
	FRotator actorRotation = GetActorRotation();
	actorRotation.Roll = FMath::Lerp(StartRotationRoll, 0.f, TiltPercent);
	
	SetActorRotation(actorRotation);
}
#pragma endregion

#pragma region Truck Speed Management
void AVehicle::ReduceSpeed()
{
	StartSpeed = MovementComponent->MaxSpeed;
	MovementComponent->MaxSpeed -= KilometersToMetersConvertingValue;
	
	GetWorld()->GetTimerManager().SetTimer(
		SpeedRecoveryHandle,
		this,
		&AVehicle::StartSpeedRecovery,
		0.5f,
		false
	);
	
}

void AVehicle::StartSpeedRecovery()
{
	bRecoveringSpeed = true;
	ElapsedTime = 0.0f;
}
#pragma endregion
