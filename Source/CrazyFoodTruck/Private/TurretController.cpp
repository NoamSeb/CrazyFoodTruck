// Fill out your copyright notice in the Description page of Project Settings.


#include "CrazyFoodTruck/Public/TurretController.h"

#include "MathUtil.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundManager.h"
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
	_CurrentBulletDamage = BulletDamage + TruckSubSystem->DamagePerBullet;

	TruckSubSystem->indexBulletShoot = 0;
	
#pragma endregion
	
	auto sceneComponents = K2_GetComponentsByClass(USceneComponent::StaticClass());
	for (auto SceneComponent : sceneComponents)
	{
		if (SceneComponent->GetName() == "SC_JointBody")
		{
			_CanonToRotate = Cast<USceneComponent>(SceneComponent);
		}
	}
	
	SwitchBulletType(TruckSubSystem->TypeBullet);
	ResetCoolDown();
	UpdateTurretCanonRotation();
	AddRotationInput(0.f);
}

void ATurretController::IncrementPlayerReloading()
{
	_ActualPlayerReloading++;
	OnPlayerReload.Broadcast(_ActualPlayerReloading);
}

void ATurretController::DecrementPlayerReloading()
{
	if (_ActualPlayerReloading <= 0) return;
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

		BulletChooseForShoot = *ActualBulletStructure;

		if (BulletChooseForShoot.BulletClass)
        {
			ActualBulletPrefab = BulletChooseForShoot.BulletClass.Get();
			
			if (!ActualBulletPrefab)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Bullet Class Loaded Synchronously !"));
				if (!ActualBulletPrefab)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to Load Bullet Class Synchronously !"));
                }
			}
        }else
        {
        	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Bullet Class Assigned in DataTable !"));
        }

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Turquoise, FString::Printf(TEXT("value fireRate Truck : %f"), TruckSubSystem->TurretFireRate));

		if (TruckSubSystem->TurretFireRate != 0)
		{
			_CurrentBulletFireRate = BulletFireRate - (TruckSubSystem->TurretFireRate * BulletFireRate / 100);
		}
		else
		{
			_CurrentBulletFireRate = BulletFireRate;
		}

		
		_CurrentBulletDamage = BulletDamage + TruckSubSystem->DamagePerBullet;

		BulletChooseForShoot.Damage = _CurrentBulletDamage;
		BulletChooseForShoot.FireRate = _CurrentBulletFireRate;
		BulletChooseForShoot.Speed = BulletSpeed + TruckSubSystem->SpeedBullet;

		_CurrentAmmoMax = BulletChooseForShoot.Ammo + TruckSubSystem->TurretMaxAmmo;

		SetMaxAmmo(_CurrentAmmoMax);
	}
	else
	{
		return;
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

	if (_CurrentAmmo >= _CurrentAmmoMax)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(_CurrentAmmo));
		if (TruckSubSystem->IncreaseDamageWhenFullReload && !IsDamageAlreadyIncrease)
		{
			IsAmmoFullReload = true;
			BulletChooseForShoot.Damage += 2;
			TimerFullReload = TimeDamageWhenFullReload;
			IsDamageAlreadyIncrease = true;
		}
	}
	
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
	if (!TurretSpline) return;
	currentStateSpline += (value * TurretRotationSpeed) * GetWorld()->GetDeltaSeconds();
	float current = FMathf::Lerp(0.f, TurretSpline->GetSplineLenght(), currentStateSpline);
	UpdateTurretOnSpline(current, current / TurretSpline->GetSplineLenght());
}


void ATurretController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (_CurrentCoolDown > 0)
	{
		_CurrentCoolDown -= DeltaTime;
	}
	if (IsDamageAlreadyIncrease)
	{
		if (TimerFullReload > 0.f)
		{
			TimerFullReload -= DeltaTime;
		}
		else
		{
			BulletChooseForShoot.Damage = _CurrentBulletDamage;
			IsDamageAlreadyIncrease = false;
			IsAmmoFullReload = false;
		}
	}
}

void ATurretController::Shoot()
{
	if (!ActualBulletPrefab)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Bullet Prefab Assigned !"));
		return;
	}
	if (!_SpawnBulletTransform)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Spawn Bullet Transform Assigned !"));
		return;
	}
	if (_CurrentCoolDown > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("value fireRate : %f"), _CurrentCoolDown));
		return;
	}
	if (_ActualPlayerReloading > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Player is Reloading !"));
		return;
	}
	if (!HasAmmo())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Turret no ammo !"));
		return;
	}

	ResetCoolDown();
	DecrementAmmo();
	
	FActorSpawnParameters bulletParams;
	OnShoot.Broadcast();
	OnShootGetAmmo.Broadcast(GetAmmo(),_CurrentAmmoMax);

	if (TruckSubSystem->TripleDamageFor10EBullet)
	{
		if (TruckSubSystem->indexBulletShoot >= 9)
		{
			BulletChooseForShoot.Damage = _CurrentBulletDamage * 3;
			TruckSubSystem->indexBulletShoot = 0;
		} else
		{
			BulletChooseForShoot.Damage = _CurrentBulletDamage;
			TruckSubSystem->indexBulletShoot ++;
		}
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("value index : %d"), TruckSubSystem->indexBulletShoot));
		
	}

	AActor* bulletInstance = GetWorld()->SpawnActor<AActor>(ActualBulletPrefab, _SpawnBulletTransform->GetComponentTransform(), bulletParams);
	if (bulletInstance)
	{
		// CAST TO BULLET CONTROLLER
		ABulletBase* BulletBase = Cast<ABulletBase>(bulletInstance);
		if (BulletBase)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("value damage : %d"), BulletChooseForShoot.Damage));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::FromInt(BulletChooseForShoot.Speed));
			
			BulletBase->Initialize(BulletChooseForShoot, _CanonToRotate->GetForwardVector());
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

// void ATurretController::InputRoll(const FInputActionValue& Value) // side (local Y)
// {
//     float valueToFloat = Value.Get<float>();
//     if (!_JointCursor) return;
//
//     USceneComponent* ParentComp = _JointCursor->GetAttachParent();
//     if (!ParentComp)
//     {
//         GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Parent Component for Cursor Joint"));
//         _JointCursor->AddWorldOffset(FVector::RightVector * (valueToFloat * (_CursorSpeed * GetWorld()->GetDeltaSeconds())));
//         UpdateTurretCanonRotation();
//         return;
//     }
//
//     FVector CurrentRelative = _JointCursor->GetRelativeLocation();
//
//     FVector localDelta = FVector::ZeroVector;
//     localDelta.Y = valueToFloat * (_CursorSpeed * GetWorld()->GetDeltaSeconds());
//
//     FVector NewRelative = CurrentRelative + localDelta;
//
//     // Clamp BOTH local axes (X = depth, Y = side)
//     NewRelative.X = FMath::Clamp(NewRelative.X, -AreaRangeDepht, AreaRangeDepht);
//     NewRelative.Y = FMath::Clamp(NewRelative.Y, -AreaRangeSide, AreaRangeSide);
//
//     // applique en relatif -> suit le parent
//     _JointCursor->SetRelativeLocation(NewRelative);
//     UpdateTurretCanonRotation();
// }
//
// void ATurretController::InputYaw(const FInputActionValue& Value) // depth (local X)
// {
//     float valueToFloat = Value.Get<float>();
//     if (!_JointCursor) return;
//
//     USceneComponent* ParentComp = _JointCursor->GetAttachParent();
//     if (!ParentComp)
//     {
//         _JointCursor->AddWorldOffset(FVector::ForwardVector * (valueToFloat * (_CursorSpeed * GetWorld()->GetDeltaSeconds())));
//         GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Parent Component for Cursor Joint"));
//         UpdateTurretCanonRotation();
//         return;
//     }
//
//     FVector CurrentRelative = _JointCursor->GetRelativeLocation();
//
//     // delta **en espace local du parent** sur l'axe X (depth)
//     FVector localDelta = FVector::ZeroVector;
//     localDelta.X = valueToFloat * (_CursorSpeed * GetWorld()->GetDeltaSeconds()); // signe à inverser si besoin
//
//     FVector NewRelative = CurrentRelative + localDelta;
//
//     // Clamp BOTH local axes (X = depth, Y = side)
//     NewRelative.X = FMath::Clamp(NewRelative.X, -AreaRangeDepht, AreaRangeDepht);
//     NewRelative.Y = FMath::Clamp(NewRelative.Y, -AreaRangeSide, AreaRangeSide);
//
//     _JointCursor->SetRelativeLocation(NewRelative);
//     UpdateTurretCanonRotation();
// }

void ATurretController::InputRoll(const FInputActionValue& Value)
{
    float valueToFloat = Value.Get<float>();
    if (!_JointCursor) return;

    const FVector worldDelta = FVector::LeftVector * (valueToFloat * (_CursorSpeed * GetWorld()->GetDeltaSeconds()));

    USceneComponent* ParentComp = _JointCursor->GetAttachParent();
    if (!ParentComp)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Parent Component for Cursor Joint"));
        _JointCursor->AddWorldOffset(worldDelta);
        UpdateTurretCanonRotation();
        return;
    }

    const FTransform parentTransform = ParentComp->GetComponentTransform();
    const FVector currentWorld = _JointCursor->GetComponentLocation();
    const FVector newWorld = currentWorld + worldDelta;
    FVector newRelative = parentTransform.InverseTransformPosition(newWorld);

    newRelative.X = FMath::Clamp(newRelative.X, -AreaRangeDepht, AreaRangeDepht);
    newRelative.Y = FMath::Clamp(newRelative.Y, -AreaRangeSide, AreaRangeSide);

    SetCursorLocation(newRelative);
    UpdateTurretCanonRotation();
}

void ATurretController::InputYaw(const FInputActionValue& Value)
{
    float valueToFloat = Value.Get<float>();
    if (!_JointCursor) return;

    const FVector worldDelta = FVector::BackwardVector * (valueToFloat * (_CursorSpeed * GetWorld()->GetDeltaSeconds()));

    USceneComponent* ParentComp = _JointCursor->GetAttachParent();
    if (!ParentComp)
    {
        _JointCursor->AddWorldOffset(worldDelta);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Parent Component for Cursor Joint"));
        UpdateTurretCanonRotation();
        return;
    }

    const FTransform parentTransform = ParentComp->GetComponentTransform();
    const FVector currentWorld = _JointCursor->GetComponentLocation();
    const FVector newWorld = currentWorld + worldDelta;
    FVector newRelative = parentTransform.InverseTransformPosition(newWorld);

    newRelative.X = FMath::Clamp(newRelative.X, -AreaRangeDepht, AreaRangeDepht);
    newRelative.Y = FMath::Clamp(newRelative.Y, -AreaRangeSide, AreaRangeSide);

    SetCursorLocation(newRelative);
    UpdateTurretCanonRotation();
}


void ATurretController::UpdateTurretCanonRotation()
{
	if (!_JointCursor){return;}
	if (!_CanonToRotate){return;}
	if (_CanonToRotate)
	{
		FVector cursorPosition = _JointCursor->GetComponentLocation();
		FVector TurretPosition = _CanonToRotate->GetComponentLocation();
		FRotator lookAtRotator = UKismetMathLibrary::FindLookAtRotation(TurretPosition, cursorPosition);
		_CanonToRotate->SetWorldRotation(lookAtRotator);
	}
}

void ATurretController::SetCursorJoint(USceneComponent* NewJoint)
{
	_JointCursor = NewJoint;
}

void ATurretController::UpdateTurretOnSpline(float currentDistance, float alphaDistance)
{
	if (!TurretSpline) return;

	FVector StartPosition = FVector::ZeroVector;
	FRotator StartRotation = FRotator::ZeroRotator;
	
	TurretSpline->GetTurretLocationAlongSpline(currentDistance, StartPosition, StartRotation);
	
	SetActorLocation(StartPosition);
	SetActorRotation(StartRotation);

	if (CisorTurretActor)
	{
		CisorTurretActor->TurretChangePosition(StartPosition, alphaDistance);
	}
}

void ATurretController::InputQuitTurret(const FInputActionValue& Value)
{
	if (InteractBox)
	{
		InteractBox->UnpossessPawn();
	}
}

bool ATurretController::MaxTurnReached()
{
	if (currentStateSpline >= 1.f)
	{
		return true;
	}
	return false;
}

bool ATurretController::MinTurnReached()
{
	if (currentStateSpline <= 0.f)
	{
		return true;
	}
	return false;
}
