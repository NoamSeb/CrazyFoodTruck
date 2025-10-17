// Fill out your copyright notice in the Description page of Project Settings.


#include "CrazyFoodTruck/Public/TurretController.h"

#include "UTurretWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ATurretController::ATurretController()
{
	PrimaryActorTick.bCanEverTick = true;


}

void ATurretController::BeginPlay()
{
	Super::BeginPlay();

	auto sceneComponents = K2_GetComponentsByClass(USceneComponent::StaticClass());
	for (auto SceneComponent : sceneComponents)
	{
		if (SceneComponent->GetName() == "SC_JointCanon")
		{
			_CanonToRotate = Cast<USceneComponent>(SceneComponent);
		}
		if (SceneComponent->GetName() == "SC_CursorJoint")
        {
            _CursorJoint = Cast<USceneComponent>(SceneComponent);
        }
	}
	
	ResetCoolDown();
	ResetAmmo();
	AddInputMapping();
	StartPossessTurret();

	Shoot();
	UpdateTurretCanonRotation();
	
}

void ATurretController::SetBulletSpawnTransform(USceneComponent* Scp)
{
	_SpawnBulletTransform = Scp;
}

void ATurretController::ResetCoolDown()
{
	_CurrentCoolDown = _BaseCoolDownShoot;
}

void ATurretController::ResetAmmo()
{
	_CurrentAmmo = _AmmoMax;
}

void ATurretController::AddInputMapping()
{
	if (!TurretMappingContext){return;}

	APlayerController* Pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (Pc)
	{
		if (ULocalPlayer* Lp = Pc->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = Lp->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->AddMappingContext(TurretMappingContext, mappingPriority);
			}
		}
	}

	
}

void ATurretController::RemoveInputMapping()
{
	if (!TurretMappingContext){return;}

	if (APlayerController* Pc = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* Lp = Pc->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = Lp->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->RemoveMappingContext(TurretMappingContext);
			}
		}
	}
}

void ATurretController::StartPossessTurret()
{
	APlayerController* Pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	if (Pc){Pc->Possess(this);}
}

void ATurretController::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AddInputMapping();
}

void ATurretController::UnPossessed()
{
	Super::UnPossessed();
	RemoveInputMapping();
}

void ATurretController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* Eic = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ShootAction)
		{
			Eic->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ATurretController::InputShootTriggered);
		}
		if (YawAction)
		{
			Eic->BindAction(YawAction, ETriggerEvent::Triggered, this, &ATurretController::InputYaw);
		}
		if (RollAction)
		{
			Eic->BindAction(RollAction, ETriggerEvent::Triggered, this, &ATurretController::InputRoll);
		}
	}
}






void ATurretController::CanonKnockBackAnim()
{
}


void ATurretController::SetCurrentAmmo(int32 NewAmmo)
{
	_CurrentAmmo = NewAmmo;
	OnAmmoChanged.Broadcast(_CurrentAmmo, _AmmoMax);
}

void ATurretController::Reload()
{
	_CurrentAmmo = _AmmoMax;
	OnAmmoChanged.Broadcast(_CurrentAmmo, _AmmoMax);
	OnReload.Broadcast();
}

void ATurretController::DecrementAmmo()
{
	_CurrentAmmo -= 1;
	if (_CurrentAmmo <= 0)
	{
		_CurrentAmmo = 0;
		Reload();
	}
	OnAmmoChanged.Broadcast(_CurrentAmmo, _AmmoMax);
}

float ATurretController::GetCoolDownBetweenShoot()
{
	return _BaseCoolDownShoot;
}


void ATurretController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (_CurrentCoolDown > 0)
	{
		_CurrentCoolDown -= DeltaTime;
	}
}

void ATurretController::Shoot()
{
	if (!BulletPrefab) return;
	if (!_SpawnBulletTransform) return;
	if (_CurrentCoolDown > 0) return; 
	if (!HasAmmo()) return;

	ResetCoolDown();
	
	CanonKnockBackAnim();

	DecrementAmmo();
	FActorSpawnParameters bulletParams;
	OnShoot.Broadcast();

	AActor* bulletInstance = GetWorld()->SpawnActor<AActor>(BulletPrefab, _SpawnBulletTransform->GetComponentTransform(), bulletParams);
	if (bulletInstance)
	{
		// CAST TO BULLET CONTROLLER
		ABulletController* bulletController = Cast<ABulletController>(bulletInstance);
		if (bulletController)
		{
			bulletController->Initialize(2200.f, 1.5f);
		}
	}
}

// INPUT

void ATurretController::InputShootTriggered(const FInputActionValue& Value)
{
	Shoot();
}

void ATurretController::InputRoll(const FInputActionValue& Value)
{
	float valueToFloat = Value.Get<float>();

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ROLL INPUT")));

	if (_CursorJoint)
	{
		FVector CurrentLocation = _CursorJoint->GetRelativeLocation();

		CurrentLocation.Y += valueToFloat * (_CursorSpeed * GetWorld()->GetDeltaSeconds());
		CurrentLocation.Y = FMath::Clamp(CurrentLocation.Y,-AreaRangeDepht , AreaRangeDepht);

		_CursorJoint->SetRelativeLocation(CurrentLocation);

		UpdateTurretCanonRotation(); 
	}
}

void ATurretController::UpdateTurretCanonRotation()
{
	if (_CanonToRotate)
	{
		// LOOK AT TARGET
		FVector cursorPosition = _CursorJoint->GetComponentLocation();
		FVector TurretPosition = _CanonToRotate->GetComponentLocation();
		FRotator lookAtRotator = UKismetMathLibrary::FindLookAtRotation(TurretPosition, cursorPosition);
		_CanonToRotate->SetWorldRotation(lookAtRotator);
	}
}

void ATurretController::InputYaw(const FInputActionValue& Value)
{
	float valueToFloat = Value.Get<float>();

	if (_CursorJoint)
	{
		FVector CurrentLocation = _CursorJoint->GetRelativeLocation();

		CurrentLocation.X += valueToFloat * (_CursorSpeed * GetWorld()->GetDeltaSeconds());
		CurrentLocation.X = FMath::Clamp(CurrentLocation.X,-AreaRangeSide , AreaRangeSide);

		_CursorJoint->SetRelativeLocation(CurrentLocation);

		UpdateTurretCanonRotation(); 
	}

	
	

}

