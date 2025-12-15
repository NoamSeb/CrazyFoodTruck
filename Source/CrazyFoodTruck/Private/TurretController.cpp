// Fill out your copyright notice in the Description page of Project Settings.


#include "CrazyFoodTruck/Public/TurretController.h"

#include "MathUtil.h"
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
	
	// SET VARS //
	


	if (TurretSpline)
	{
		SplineLength = TurretSpline->GetSplineLenght();
	}

	ParentOfTurret = GetAttachParentActor();

	ReferenceComp = Cast<USceneComponent>(ParentOfTurret->GetRootComponent());

	ParentComp = _JointCursor->GetAttachParent();

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
				if (!ActualBulletPrefab)
                {
                }
			}
        }

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
	if (_CurrentAmmo == NewAmmo) return;

	_CurrentAmmo = NewAmmo;

	if (_CurrentAmmo >= _CurrentAmmoMax)
	{
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
	if (!TurretSpline)
	{
		return;
	}
	currentStateSpline += (value * TurretRotationSpeed) * GetWorld()->GetDeltaSeconds();
	float current = FMathf::Lerp(0.f, SplineLength, currentStateSpline);
	UpdateTurretOnSpline(current, current /SplineLength);
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
		return;
	}
	if (!_SpawnBulletTransform)
	{
		return;
	}
	if (_CurrentCoolDown > 0)
	{
		return;
	}
	if (_ActualPlayerReloading > 0)
	{
		return;
	}
	if (!HasAmmo())
	{
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
	}

	AActor* bulletInstance = GetWorld()->SpawnActor<AActor>(ActualBulletPrefab, _SpawnBulletTransform->GetComponentTransform(), bulletParams);
	if (bulletInstance)
	{
		// CAST TO BULLET CONTROLLER
		ABulletBase* BulletBase = Cast<ABulletBase>(bulletInstance);
		if (BulletBase)
		{
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

void ATurretController::InputRoll(const FInputActionValue& Value)
{
	const float Axis = Value.Get<float>();
	if (!_JointCursor) return;

	const float DT = GetWorld()->GetDeltaSeconds();
	const FVector CamRight = GetCameraPlanarRight();

	const FVector WorldDelta = CamRight * (-Axis * _CursorSpeed * DT);

	ApplyCursorDeltaWorld(WorldDelta);
}

void ATurretController::InputYaw(const FInputActionValue& Value)
{
	const float Axis = Value.Get<float>();
	if (!_JointCursor) return;

	const float DT = GetWorld()->GetDeltaSeconds();
	const FVector CamFwd = GetCameraPlanarForward();

	const FVector WorldDelta = CamFwd * (-Axis * _CursorSpeed * DT);

	ApplyCursorDeltaWorld(WorldDelta);
}

void ATurretController::UpdateTurretCanonRotation()
{
    if (!_JointCursor) return;
    if (!_CanonToRotate) return;

    const float Delta = GetWorld()->GetDeltaSeconds();
    RotationUpdateAccumulator += Delta;
    const float Interval = 1.0f / FMath::Max(1.f, RotationUpdateRate);
    if (RotationUpdateAccumulator < Interval)
    {
        return;
    }
    RotationUpdateAccumulator = 0.f;

    const FVector CursorPos = _JointCursor->GetComponentLocation();
    const FVector TurretPos  = _CanonToRotate->GetComponentLocation();

    const float PosThreshSq = PositionChangeThreshold * PositionChangeThreshold;
    const bool bCursorMoved = (CursorPos - LastCursorPosition).SizeSquared() > PosThreshSq;
    const bool bTurretMoved = (TurretPos  - LastTurretPosition).SizeSquared()  > PosThreshSq;

    if (!bCursorMoved && !bTurretMoved)
    {
        const FVector DesiredDir = (CursorPos - TurretPos);
        if (DesiredDir.IsNearlyZero()) return;

        const FVector DesiredDirNorm = DesiredDir.GetSafeNormal();
        const FVector CurrentForward = _CanonToRotate->GetForwardVector();

        // compute dot and compare to cos(threshold) to avoid expensive acos
        const float CosThreshold = FMath::Cos(FMath::DegreesToRadians(AngleChangeThresholdDegrees));
        const float Dot = FVector::DotProduct(CurrentForward, DesiredDirNorm);

        if (Dot > CosThreshold)
        {
            return;
        }
    }

    const FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(TurretPos, CursorPos);

    if (ParentComp)
    {
        const FQuat WorldQ = LookAtRotator.Quaternion();
        const FQuat ParentQ = ParentComp->GetComponentQuat();
        const FQuat LocalQ = ParentQ.Inverse() * WorldQ;
    	_CanonToRotate->SetRelativeRotation(LocalQ);
    }
    else
    {
        _CanonToRotate->SetWorldRotation(LookAtRotator);
    }

    LastCursorPosition = CursorPos;
    LastTurretPosition  = TurretPos;
}


void ATurretController::SetCursorJoint(USceneComponent* NewJoint)
{
	_JointCursor = NewJoint;
}

void ATurretController::UpdateTurretOnSpline(float currentDistance, float alphaDistance)
{
	if (!TurretSpline) return;

	FVector NewPos = FVector::ZeroVector;
	FRotator NewRot = FRotator::ZeroRotator;
	TurretSpline->GetTurretLocationAlongSpline(currentDistance, NewPos, NewRot);

	// compare with cached
	bool bPosDiff = (NewPos - LastActorLocation).SizeSquared() > (ActorTransformEpsilon * ActorTransformEpsilon);
	bool bRotDiff = !NewRot.Quaternion().Equals(LastActorRotation, FMath::DegreesToRadians(0.5f)); // 0.5 degree
	bool bAlphaDiff = FMath::Abs(alphaDistance - LastSplineAlpha) > AlphaThreshold;

	if (bPosDiff || bRotDiff)
	{
		SetActorLocation(NewPos);
		SetActorRotation(NewRot);
		LastActorLocation = NewPos;
		LastActorRotation = NewRot.Quaternion();
	}

	if (CisorTurretActor && bAlphaDiff)
	{
		CisorTurretActor->TurretChangePosition(NewPos, alphaDistance);
		LastSplineAlpha = alphaDistance;
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

void ATurretController::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	//SetActorTickEnabled(true);
}

void ATurretController::UnPossessed()
{
	Super::UnPossessed();
//	SetActorTickEnabled(false);
	
}

FVector ATurretController::GetCameraPlanarForward() const
{
	const APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC || !PC->PlayerCameraManager) return FVector::ForwardVector;

	const float Yaw = PC->PlayerCameraManager->GetCameraRotation().Yaw;
	const FRotator Flat(0.f, Yaw, 0.f);
	return FRotationMatrix(Flat).GetUnitAxis(EAxis::X);
}

FVector ATurretController::GetCameraPlanarRight() const
{
	const APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC || !PC->PlayerCameraManager) return FVector::RightVector;

	const float Yaw = PC->PlayerCameraManager->GetCameraRotation().Yaw;
	const FRotator Flat(0.f, Yaw, 0.f);
	return FRotationMatrix(Flat).GetUnitAxis(EAxis::Y);
}

void ATurretController::ApplyCursorDeltaWorld(const FVector& WorldDelta)
{
	if (!_JointCursor) return;

	if (!ParentComp)
	{
		_JointCursor->AddWorldOffset(WorldDelta);
		SetCursorLocation(_JointCursor->GetRelativeLocation());
		UpdateTurretCanonRotation();
		return;
	}

	const FVector CurrentWorld = _JointCursor->GetComponentLocation();
	const FVector NewWorld = CurrentWorld + WorldDelta;

	const FTransform ParentTransform = ParentComp->GetComponentTransform();
	FVector NewRel = ParentTransform.InverseTransformPosition(NewWorld);

	NewRel.X = FMath::Clamp(NewRel.X, -AreaRangeDepht, AreaRangeDepht);
	NewRel.Y = FMath::Clamp(NewRel.Y, -AreaRangeSide, AreaRangeSide);

	SetCursorLocation(NewRel);
	UpdateTurretCanonRotation();
}
