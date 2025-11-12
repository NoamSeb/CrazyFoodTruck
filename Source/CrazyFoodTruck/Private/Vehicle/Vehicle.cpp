// Fill out your copyright notice in the Description page of Project Settings.

#include "CrazyFoodTruck/Public/Vehicle/Vehicle.h"

#include "Interactable/InteractBox.h"
#include "TurretController.h"

#include "Widget/ForwardCamWidget.h"

#include "LocalMultiplayerSettings.h"

#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SceneCaptureComponent2D.h"

#include "GameFramework/FloatingPawnMovement.h"

#include "Engine/TextureRenderTarget2D.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetRenderingLibrary.h"

#include "Blueprint/UserWidget.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AVehicle::AVehicle()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostUpdateWork;

	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
    Root->SetCollisionProfileName(TEXT("Vehicle"));

	ForwardCamRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ForwardCamRoot"));
	ForwardCamRoot->SetupAttachment(RootComponent);

	ForwardCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("ForwardCapture"));
	ForwardCapture->SetupAttachment(ForwardCamRoot);

	ForwardCapture->FOVAngle = ForwardCamFOV;
	ForwardCapture->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
	ForwardCapture->bCaptureEveryFrame = false;
	ForwardCapture->bCaptureOnMovement = false;
	ForwardCapture->TextureTarget = nullptr;
}

void AVehicle::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent = Cast<UFloatingPawnMovement>(GetMovementComponent());
	MovementComponent->MaxSpeed = TruckMaxSpeed * KilometersToMetersConvertingValue;

	if (!MovementComponent->UpdatedComponent)
	{
		MovementComponent->SetUpdatedComponent(RootComponent);
	}
	
	MovementComponent->bUpdateOnlyIfRendered = false;
	MovementComponent->Activate(true);

	MovementComponent->SetPlaneConstraintEnabled(true);
	MovementComponent->SetPlaneConstraintNormal(FVector::UpVector);
	MovementComponent->SetPlaneConstraintOrigin(FVector::ZeroVector);

	CreateAndAssignForwardRenderTarget();
	ConfigureForwardCaptureQuality();
	StopForwardCapture();

	ForwardCamWidget = nullptr;

	ForwardCaptureInterval = (ForwardCaptureFPS > 0.f) ? (1.f / ForwardCaptureFPS) : (1.f / 30.f);
	ForwardCaptureTimer = 0.f;
}

void AVehicle::PossessedBy(AController* NewController)
{
	if (MovementComponent)
	{
		SavedLinearVelocity = MovementComponent->Velocity;
		PossessKeepVelocity = SavedLinearVelocity;
	}
	
	Super::PossessedBy(NewController);

	if (MovementComponent)
	{
		MovementComponent->Velocity = PossessKeepVelocity;
		MovementComponent->UpdateComponentVelocity();

		bHoldSpeedAfterPossess = true;
		HoldSpeedTimer = HoldSpeedDuration;
	}

	if (!ForwardCamWidget && ForwardCamWidgetClass && ForwardRT)
	{
		if (APlayerController* PC = Cast<APlayerController>(NewController))
		{
			ForwardCamWidget = CreateWidget<UForwardCamWidget>(PC, ForwardCamWidgetClass);
			if (ForwardCamWidget)
			{
				ForwardCamWidget->AddToViewport(50);
				ForwardCamWidget->SetForwardTexture(ForwardRT);
				ForwardCamWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}
	}

	if (bLiveCaptureWhilePossessed)
	{
		StartForwardCapture();
	}
	else
	{
		CaptureForwardOnce();
	}
}

void AVehicle::UnPossessed()
{
	if (MovementComponent)
	{
		SavedLinearVelocity = MovementComponent->Velocity;
	}
	
	Super::UnPossessed();

	if (MovementComponent)
	{
		MovementComponent->Velocity = SavedLinearVelocity;
		MovementComponent->UpdateComponentVelocity();
	}

	StopForwardCapture();

	if (ForwardCamWidget)
	{
		ForwardCamWidget->RemoveFromParent();
		ForwardCamWidget = nullptr;
	}
}

void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveForward();

	if (bHoldSpeedAfterPossess)
	{
		const float Speed = PossessKeepVelocity.Size();
		const FVector Fwd2D = FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0.f).GetSafeNormal();
		PossessKeepVelocity = Fwd2D * Speed;

		MovementComponent->Velocity = PossessKeepVelocity;
		MovementComponent->UpdateComponentVelocity();

		AddMovementInput(Fwd2D, 1.f, true);

		HoldSpeedTimer -= DeltaTime;
		if (HoldSpeedTimer <= 0.f)
		{
			bHoldSpeedAfterPossess = false;
		}
	}

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
		const float Alpha = FMath::Clamp(ElapsedTime / SpeedRecoveryDuration, 0.f, 1.f);

		const float VMin = (TruckMaxSpeed - TruckLossSpeed) * KilometersToMetersConvertingValue;
		const float VMax = TruckMaxSpeed * KilometersToMetersConvertingValue;
		MovementComponent->MaxSpeed = FMath::Lerp(VMin, VMax, Alpha);

		if (Alpha >= 1.0f)
		{
			bRecoveringSpeed = false;
		}
	}

	if (ForwardCapture && ForwardCapture->bEnableClipPlane)
	{
		ForwardCapture->ClipPlaneBase = ForwardCapture->GetComponentLocation();
		ForwardCapture->ClipPlaneNormal = ForwardCapture->GetForwardVector();
	}

	UpdateForwardCapture(DeltaTime);
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
	const FVector Fwd2D = FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0.f).GetSafeNormal();

	if (Controller)
	{
		AddMovementInput(Fwd2D, 1.f, true);
		return;
	}

	if (MovementComponent && MovementComponent->UpdatedComponent)
	{
		MovementComponent->ConsumeInputVector();
		MovementComponent->Velocity = Fwd2D * MovementComponent->MaxSpeed;
		MovementComponent->UpdateComponentVelocity();

		const float DT = (GetWorld() ? GetWorld()->GetDeltaSeconds() : 1.f / 60.f);
		AddActorWorldOffset(Fwd2D * MovementComponent->MaxSpeed * DT, true);
	}
}

void AVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// SetupMappingContextIntoController();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		BindInputRotateZAxisAndActions(EnhancedInputComponent);

		if (QuitTruckAction)
		{
			EnhancedInputComponent->BindAction(QuitTruckAction, ETriggerEvent::Started, this, &AVehicle::InputQuitTruck);
		}
	}
}

void AVehicle::SetupMappingContextIntoController() const
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController == nullptr) return;

	ULocalPlayer* Player = PlayerController->GetLocalPlayer();
	if (Player == nullptr) return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (InputSystem == nullptr) return;

	InputSystem->AddMappingContext(FoodTruckInputMappingContext, 0);
}

void AVehicle::BindInputRotateZAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (FoodTruckInputMappingContext == nullptr) return;

	if (TurnTruckAction)
	{
		EnhancedInputComponent->BindAction(TurnTruckAction, ETriggerEvent::Triggered, this, &AVehicle::SetTruckRotatingStates);
		EnhancedInputComponent->BindAction(TurnTruckAction, ETriggerEvent::Completed, this, &AVehicle::SetTruckIdleStates);
	}
}

void AVehicle::InputQuitTruck(const FInputActionValue& InputActionValue)
{
	if (InteractBox)
	{
		InteractBox->UnpossessPawn();
	}
}

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
	
	if (InputRotatingValue > 0)
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

void AVehicle::RotateTruck(float DeltaTime)
{
	destinationRotation.Yaw += (InputRotatingValue * TruckAngleSpeed) * DeltaTime;
	destinationRotation.Yaw = FMath::Clamp(destinationRotation.Yaw, -TruckMaxRotation, TruckMaxRotation);

	destinationRotation.Roll += ((InputRotatingValue * TruckAngleSpeed) / 2) * DeltaTime;
	destinationRotation.Roll = FMath::Clamp(destinationRotation.Roll, -TruckMaxTilt, TruckMaxTilt);

	UpdateRotationTruck(destinationRotation, DeltaTime);
}

void AVehicle::UpdateRotationTruck(FRotator TargetRotation, float DeltaTime)
{
	if (RotationAnimCurve == nullptr) return;

	if (RotationTimer < TruckInterpolationDuration)
		RotationTimer += DeltaTime;

	float RotationPercent = RotationTimer / TruckInterpolationDuration;
	RotationPercent = RotationAnimCurve->GetFloatValue(RotationPercent);

	FRotator ActorRot = GetActorRotation();
	ActorRot.Yaw = FMath::Lerp(StartRotationYaw, TargetRotation.Yaw, RotationPercent);
	ActorRot.Roll = FMath::Lerp(StartRotationRoll, TargetRotation.Roll, RotationPercent);

	ActorRot.Pitch = 0.f;

	SetActorRotation(ActorRot);
}

void AVehicle::ResetTruckTilt(float DeltaTime)
{
	if (TiltAnimCurve == nullptr) return;

	if (TiltTimer < TruckInterpolationTilt)
	{
		TiltTimer += DeltaTime;
	}

	float TiltPercent = TiltTimer / TruckInterpolationTilt;
	TiltPercent = TiltAnimCurve->GetFloatValue(TiltPercent);

	FRotator ActorRot = GetActorRotation();
	ActorRot.Roll = FMath::Lerp(StartRotationRoll, 0.f, TiltPercent);
	ActorRot.Pitch = 0.f;

	SetActorRotation(ActorRot);
}

void AVehicle::ReduceSpeed()
{
	if (!MovementComponent) return;

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

void AVehicle::CreateAndAssignForwardRenderTarget()
{
	if (!ForwardRT)
	{
		ForwardRT = UKismetRenderingLibrary::CreateRenderTarget2D(this, ForwardRT_Width, ForwardRT_Height, RTF_RGBA8);
		if (ForwardRT)
		{
			ForwardRT->bAutoGenerateMips = false;
			ForwardRT->ClearColor = FLinearColor::Black;
			ForwardRT->TargetGamma = 2.2f;
		}
	}

	if (ForwardCapture)
	{
		ForwardCapture->FOVAngle = ForwardCamFOV;
		ForwardCapture->bCaptureEveryFrame = false;
		ForwardCapture->bCaptureOnMovement = false;
		ForwardCapture->TextureTarget = nullptr;
		ForwardCapture->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
	}
}

void AVehicle::ConfigureForwardCaptureQuality()
{
	if (!ForwardCapture) return;

	auto& SF = ForwardCapture->ShowFlags;

	SF.SetLighting(true);
	SF.SetPostProcessing(true);
	SF.SetTemporalAA(true);

	SF.SetAtmosphere(false);
	SF.SetFog(false);

	SF.SetScreenSpaceReflections(false);
	SF.SetAmbientOcclusion(false);
	SF.SetMotionBlur(false);
	SF.SetBloom(false);

	SF.SetMaterials(true);
	SF.SetTranslucency(true);

	//ForwardCapture->PostProcessBlendWeight = 0.f;
	//ForwardCapture->PostProcessSettings = FPostProcessSettings{};
	//ForwardCapture->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
	//ForwardCapture->MaxViewDistanceOverride = 50000.f;
	//ForwardCapture->LODDistanceFactor = 2.f;

	ForwardCapture->bEnableClipPlane = true;
	ForwardCapture->ClipPlaneBase = ForwardCapture->GetComponentLocation();
	ForwardCapture->ClipPlaneNormal = ForwardCapture->GetForwardVector();
}

void AVehicle::StartForwardCapture()
{
	if (!ForwardCapture || !ForwardRT) return;

	ForwardCapture->FOVAngle = ForwardCamFOV;
	ForwardCapture->TextureTarget = ForwardRT;
	ForwardCapture->bCaptureOnMovement = false;
	ForwardCapture->bCaptureEveryFrame = false;
	ForwardCapture->Activate(true);

	bForwardCaptureActive = true;
	ForwardCaptureTimer = 0.f;
}

void AVehicle::StopForwardCapture()
{
	if (!ForwardCapture) return;

	bForwardCaptureActive = false;
	ForwardCaptureTimer = 0.f;

	ForwardCapture->bCaptureEveryFrame = false;
	ForwardCapture->bCaptureOnMovement = false;
	ForwardCapture->Deactivate();
	ForwardCapture->TextureTarget = nullptr;
}

void AVehicle::CaptureForwardOnce()
{
	if (!ForwardCapture || !ForwardRT) return;

	ForwardCapture->FOVAngle = ForwardCamFOV;
	ForwardCapture->TextureTarget = ForwardRT;
	ForwardCapture->bCaptureEveryFrame = false;
	ForwardCapture->bCaptureOnMovement = false;

	ForwardCapture->CaptureScene();
	ForwardCapture->Deactivate();
	ForwardCapture->TextureTarget = nullptr;
}

void AVehicle::UpdateForwardCapture(float DeltaTime)
{
	if (!bForwardCaptureActive || !ForwardCapture || !ForwardRT) return;
	if (!ShouldCaptureForward()) return;

	ForwardCaptureTimer += DeltaTime;
	if (ForwardCaptureTimer < ForwardCaptureInterval) return;

	ForwardCaptureTimer = 0.f;
	ForwardCapture->CaptureScene();
}

bool AVehicle::ShouldCaptureForward() const
{
	if (!bLiveCaptureWhilePossessed && Controller == nullptr)
	{
		return false;
	}
	
	return true;
}
