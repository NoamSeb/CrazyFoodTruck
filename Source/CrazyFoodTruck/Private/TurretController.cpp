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
	SwitchBulletType(EbulletType::BulletNormal);
	Shoot();
	UpdateTurretCanonRotation();
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
	_CurrentAmmo = _AmmoMax;
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
		SetMaxAmmo(ActualBulletStructure->Ammo);
	}
	else
	{
		return;
	}
	
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
	
	OnAmmoChanged.Broadcast(GetAmmo(),_AmmoMax);
	OnTypeChangedGetAmmo.Broadcast(GetAmmo(),_AmmoMax);
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
}


void ATurretController::SetCurrentAmmo(int32 NewAmmo)
{
	_CurrentAmmo = NewAmmo;
	OnAmmoChanged.Broadcast(_CurrentAmmo, _AmmoMax);
}

void ATurretController::SetMaxAmmo(int32 NewAmmo)
{
	_AmmoMax = NewAmmo;
	OnAmmoChanged.Broadcast(_CurrentAmmo, _AmmoMax);
}

// void ATurretController::Reload()
// {
// 	_CurrentAmmo = _AmmoMax;
// 	OnAmmoChanged.Broadcast(_CurrentAmmo, _AmmoMax);
// 	OnReload.Broadcast();
// }

void ATurretController::DecrementAmmo()
{
	_CurrentAmmo -= 1;
	if (_CurrentAmmo <= 0)
	{
		OnAmmoEmpty.Broadcast();
		_CurrentAmmo = 0;
	}
	OnAmmoChanged.Broadcast(_CurrentAmmo, _AmmoMax);
}

void ATurretController::BlueprintShoot()
{
	Shoot();
}

float ATurretController::GetCoolDownBetweenShoot()
{
	return BulletFireRate;
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
	if (!HasAmmo()) return;

	ResetCoolDown();
	DecrementAmmo();
	
	FActorSpawnParameters bulletParams;
	OnShoot.Broadcast();
	OnShootGetAmmo.Broadcast(GetAmmo(),_AmmoMax);

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
	if (_CursorJoint)
	{
		auto WoldDir = FVector::LeftVector;
		auto worldDelta = WoldDir * (valueToFloat * (_CursorSpeed * GetWorld()->GetDeltaSeconds()));

		auto* ParentComp = _CursorJoint->GetAttachParent();
		
		if (!ParentComp)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Parent Component for Cursor Joint"));
			_CursorJoint->AddWorldOffset(worldDelta);
			return;
		}

		const FTransform parentTransform = ParentComp->GetComponentTransform();
		FVector localDelta = parentTransform.InverseTransformVectorNoScale(worldDelta);

		FVector CurrentRelative = _CursorJoint->GetRelativeLocation();
		FVector NewRelative = CurrentRelative + localDelta;

		NewRelative.Y = FMath::Clamp(NewRelative.Y,-AreaRangeSide , AreaRangeSide);
		
		_CursorJoint->SetRelativeLocation(NewRelative);

		UpdateTurretCanonRotation(); 
	}
}


void ATurretController::InputYaw(const FInputActionValue& Value) // X VALUE depht axis ^
{
	float valueToFloat = Value.Get<float>();
	
	if (_CursorJoint)
	{
		auto Fdir = FVector::BackwardVector;
		auto worldDelta = Fdir * (valueToFloat * (_CursorSpeed * GetWorld()->GetDeltaSeconds()));

		auto* ParentComp = _CursorJoint->GetAttachParent();
		if (!ParentComp)
		{
			_CursorJoint->AddWorldOffset(worldDelta);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Parent Component for Cursor Joint"));
			return;
		}
		
		const FTransform parentTransform = ParentComp->GetComponentTransform();
		FVector localDelta = parentTransform.InverseTransformVectorNoScale(worldDelta);

		FVector CurrentRelative = _CursorJoint->GetRelativeLocation();
		FVector NewRelative = CurrentRelative + localDelta;
		NewRelative.X = FMath::Clamp(NewRelative.X,-AreaRangeDepht , AreaRangeDepht);
		
		_CursorJoint->SetRelativeLocation(NewRelative);
		UpdateTurretCanonRotation();
	}
}

void ATurretController::UpdateTurretCanonRotation()
{
	if (_CanonToRotate)
	{
		FVector cursorPosition = _CursorJoint->GetComponentLocation();
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

void ATurretController::Print(FString Message)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Message);
}