// Fill out your copyright notice in the Description page of Project Settings.


#include "CrazyFoodTruck/Public/TurretController.h"

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

#pragma region Upgrades
	if (UGameInstance* GIBase = GetGameInstance())
	{
		GI = Cast<UGameInstanceCrazyFoodTruck>(GIBase);
	}
	TruckSubSystem = GI->GetSubsystem<UFoodTruckDataSubSystem>();

	_CurrentAmmoMax = _AmmoMax + TruckSubSystem->TurretMaxAmmo;
	_CurrentBulletFireRate = BulletFireRate + TruckSubSystem->TurretFireRate;
	_CurrentBulletDamage = BulletDamage + TruckSubSystem->DamagePerBullet;
#pragma endregion
	

	auto sceneComponents = K2_GetComponentsByClass(USceneComponent::StaticClass());
	for (auto SceneComponent : sceneComponents)
	{
		if (SceneComponent->GetName() == "SC_JointBody")
		{
			_CanonToRotate = Cast<USceneComponent>(SceneComponent);
		}
		if (SceneComponent->GetName() == "SC_CursorJoint")
        {
            _JointCursor = Cast<USceneComponent>(SceneComponent);
        }
	}
	
	ResetCoolDown();
	SwitchBulletType(TruckSubSystem->TypeBullet);
	Shoot();
	UpdateTurretCanonRotation();
}

void ATurretController::IncrementPlayerReloading()
{
	_ActualPlayerReloading++;
	OnPlayerReload.Broadcast(_ActualPlayerReloading);
}

void ATurretController::DecrementPlayerReloading()
{
	_ActualPlayerReloading--;
	OnPlayerReload.Broadcast(_ActualPlayerReloading);
	if (_ActualPlayerReloading <= 0)
    {
		OnAmmoChanged.Broadcast(_CurrentAmmo, _CurrentAmmoMax);
    }
}

void ATurretController::SetBulletSpawnTransform(USceneComponent* Scp)
{
	_SpawnBulletTransform = Scp;
}

void ATurretController::ResetCoolDown()
{
	_CurrentCoolDown = GetCoolDownBetweenShoot();
}

void ATurretController::ResetAmmo()
{
	_CurrentAmmo = _CurrentAmmoMax;
}

void ATurretController::SwitchBulletType(EbulletType NewType)
{
	_actualBulletType = NewType;
	
	FString TargetName = GetRowNameFromBulletType(_actualBulletType);
	ActualBulletStructure = BulletDataTable->FindRow<FBulletStructure>(FName(*TargetName), "", true);
	if (ActualBulletStructure != nullptr)
	{
		BulletSpeed = ActualBulletStructure->Speed;
		BulletDamage = ActualBulletStructure->Damage;
		BulletFireRate = ActualBulletStructure->FireRate;
		AreaRangeSide = ActualBulletStructure->AreaSide;
		AreaRangeDepht = ActualBulletStructure->AreaDepth;
		BulletHapticForce = ActualBulletStructure->HapticsScale;
		SetMaxAmmo(ActualBulletStructure->Ammo);

		//Pour Upgrades
		_CurrentBulletFireRate = BulletFireRate + TruckSubSystem->TurretFireRate;
		_CurrentBulletDamage = BulletDamage + TruckSubSystem->DamagePerBullet;
	}
	else{return;}
	
	FString FullPath = FString::Printf(TEXT("/Game/Resources/Bullet/%s.%s_C"), *TargetName, *TargetName);
	
	UClass* LoadedClass = LoadClass<ABulletBase>(nullptr, *FullPath);
	if (LoadedClass)
	{
		ActualBulletPrefab = LoadedClass;
	}
	else
	{
		ActualBulletPrefab = nullptr;
	}
	
	OnAmmoChanged.Broadcast(GetAmmo(),_CurrentAmmoMax);
	OnTypeChangedGetAmmo.Broadcast(GetAmmo(),_CurrentAmmoMax);
	OnAmmoTypeChanged.Broadcast(AreaRangeSide, AreaRangeDepht);
}

FString ATurretController::GetRowNameFromBulletType(EbulletType Type)
{
	switch (Type)
	{
		case EbulletType::BulletNormal:
			return FString("BulletNormal");
		case EbulletType::BulletExplosive:
			return FString("BulletExplosive");
		case EbulletType::BulletFlamethrower:
			return FString("BulletFlamethrower");
		case EbulletType::BulletBouncy:
			return FString("BulletBouncy");
		default:
			return FString("normal");
	}
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
		if (ChangeBulletAction)
		{
			Eic->BindAction(ChangeBulletAction, ETriggerEvent::Started, this, &ATurretController::InputChangeBulletType);
		}
		if (QuitTurret)
		{
			Eic->BindAction(QuitTurret, ETriggerEvent::Started, this, &ATurretController::InputQuitTurret);
		}
	}
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PlayerController = PC;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller Not found !"));
	}
}


void ATurretController::SetCurrentAmmo(int32 NewAmmo)
{
	_CurrentAmmo = NewAmmo;
	OnAmmoChanged.Broadcast(_CurrentAmmo, _CurrentAmmoMax);
}

void ATurretController::SetMaxAmmo(int32 NewAmmo)
{
	_CurrentAmmoMax = NewAmmo;
	OnAmmoChanged.Broadcast(_CurrentAmmo, _CurrentAmmoMax);
}

void ATurretController::DecrementAmmo()
{
	_CurrentAmmo -= 1;
	if (_CurrentAmmo <= 0)
	{
		OnAmmoEmpty.Broadcast();
		_CurrentAmmo = 0;
	}
	OnAmmoChanged.Broadcast(_CurrentAmmo, _CurrentAmmoMax);
}

void ATurretController::BlueprintShoot()
{
	Shoot();
}

float ATurretController::GetCoolDownBetweenShoot()
{
	return _CurrentBulletFireRate;
}

void ATurretController::AddRotationInput(float value)
{
	float targetSpeed = value * (TurretRotationSpeed * GetWorld()->GetDeltaSeconds());
	FRotator CurrentRotation = GetActorRotation();
	FRotator NewRotation = FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw + targetSpeed, CurrentRotation.Roll);
	SetActorRotation(NewRotation);
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
	if (!ActualBulletPrefab) return;
	if (!_SpawnBulletTransform) return;
	if (_CurrentCoolDown > 0) return;
	if (_ActualPlayerReloading > 0) return;
	if (!HasAmmo()) return;

	ResetCoolDown();
	DecrementAmmo();
	
	FActorSpawnParameters bulletParams;
	OnShoot.Broadcast();
	OnShootGetAmmo.Broadcast(GetAmmo(),_CurrentAmmoMax);

	AActor* bulletInstance = GetWorld()->SpawnActor<AActor>(ActualBulletPrefab, _SpawnBulletTransform->GetComponentTransform(), bulletParams);
	if (bulletInstance)
	{
		// CAST TO BULLET CONTROLLER
		ABulletBase* BulletBase = Cast<ABulletBase>(bulletInstance);
		if (BulletBase)
		{
			BulletBase->Initialize(ActualBulletStructure, _CanonToRotate->GetForwardVector());
		}
	}
}

// INPUT

void ATurretController::InputChangeBulletType(const FInputActionValue& Value)
{
	EbulletType nextType = static_cast<EbulletType>((static_cast<int>(_actualBulletType) + 1));
	if (nextType > EbulletType::BulletBouncy)
	{
		nextType = EbulletType::BulletNormal;
	}
	
	SwitchBulletType(nextType);
}

void ATurretController::InputShootTriggered(const FInputActionValue& Value)
{
	Shoot();
}

void ATurretController::InputRoll(const FInputActionValue& Value) // MOVE ALONG Y AXIS // side AXIS <->
{
	float valueToFloat = Value.Get<float>();
	if (_JointCursor)
	{
		auto WoldDir = FVector::LeftVector;
		auto worldDelta = WoldDir * (valueToFloat * (_CursorSpeed * GetWorld()->GetDeltaSeconds()));

		auto* ParentComp = _JointCursor->GetAttachParent();
		
		if (!ParentComp)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Parent Component for Cursor Joint"));
			_JointCursor->AddWorldOffset(worldDelta);
			return;
		}

		const FTransform parentTransform = ParentComp->GetComponentTransform();
		FVector localDelta = parentTransform.InverseTransformVectorNoScale(worldDelta);

		FVector CurrentRelative = _JointCursor->GetRelativeLocation();
		FVector NewRelative = CurrentRelative + localDelta;

		NewRelative.Y = FMath::Clamp(NewRelative.Y,-AreaRangeSide , AreaRangeSide);
		
		_JointCursor->SetRelativeLocation(NewRelative);

		UpdateTurretCanonRotation(); 
	}
}


void ATurretController::InputYaw(const FInputActionValue& Value) // X VALUE depht axis ^
{
	float valueToFloat = Value.Get<float>();
	
	if (_JointCursor)
	{
		auto Fdir = FVector::BackwardVector;
		auto worldDelta = Fdir * (valueToFloat * (_CursorSpeed * GetWorld()->GetDeltaSeconds()));

		auto* ParentComp = _JointCursor->GetAttachParent();
		if (!ParentComp)
		{
			_JointCursor->AddWorldOffset(worldDelta);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Parent Component for Cursor Joint"));
			return;
		}
		
		const FTransform parentTransform = ParentComp->GetComponentTransform();
		FVector localDelta = parentTransform.InverseTransformVectorNoScale(worldDelta);

		FVector CurrentRelative = _JointCursor->GetRelativeLocation();
		FVector NewRelative = CurrentRelative + localDelta;
		NewRelative.X = FMath::Clamp(NewRelative.X,-AreaRangeDepht , AreaRangeDepht);
		
		_JointCursor->SetRelativeLocation(NewRelative);
		UpdateTurretCanonRotation();
	}
}

void ATurretController::UpdateTurretCanonRotation()
{
	if (_CanonToRotate)
	{
		FVector cursorPosition = _JointCursor->GetComponentLocation();
		FVector TurretPosition = _CanonToRotate->GetComponentLocation();
		FRotator lookAtRotator = UKismetMathLibrary::FindLookAtRotation(TurretPosition, cursorPosition);
		_CanonToRotate->SetWorldRotation(lookAtRotator);
	}
}

void ATurretController::InputQuitTurret(const FInputActionValue& Value)
{
	if (InteractBox)
	{
		InteractBox->UnpossessPawn();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Player quit turret."));
	}
}
