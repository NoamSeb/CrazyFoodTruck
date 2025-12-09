// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CisorTurret.h"
#include "CisorTurret.h"
#include "Bullet/BulletBase.h"
#include "Bullet/EBulletType.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SplineTurret.h"
#include "Bullet/FBulletStructure.h"
#include "Components/SplineComponent.h"
#include "CrazyFoodTruck/Data/Public/GameInstanceCrazyFoodTruck.h"
#include "GameFramework/Actor.h"
#include "Interactable/Interactable.h"
#include "Interactable/InteractBox.h"
#include "TurretController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShootSignature, int32, AmmoLeft, int32, AmmoMax);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoTypeEvent, float, AreaSide, float, AreaDepht);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurretEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerReload, int32, CurrentReloadingPlayers);

UCLASS()
class CRAZYFOODTRUCK_API ATurretController : public APawn
{
	GENERATED_BODY()

public:
	
	ATurretController();
	
	TSubclassOf<ABulletBase> ActualBulletPrefab;

	UPROPERTY(EditAnywhere, Category="Data")
	UDataTable* BulletDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Variable")
	AInteractBox* InteractBox;

	APlayerController* ActualPlayerController;
	APawn* ActualPawn;
	
	int GetAmmo() const { return _CurrentAmmo;}
	int GetAmmoMax() const { return _CurrentAmmoMax;}
	void SetCurrentAmmo(int32 NewAmmo);
	void SetMaxAmmo(int32 NewAmmo);
	void DecrementAmmo();
	bool HasAmmo() const { return _CurrentAmmo > 0; }

	// TEST

	UFUNCTION(BlueprintCallable)
	void BlueprintShoot();

	UFUNCTION(BlueprintCallable)
	float GetCoolDownBetweenShoot();

	void AddRotationInput(float value);
	
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnAmmoTypeEvent OnAmmoTypeChanged;;
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnShootSignature OnAmmoChanged;
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnShootSignature OnShootGetAmmo;
	FOnShootSignature OnTypeChangedGetAmmo;
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnTurretEvent OnShoot;
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnTurretEvent OnReload;
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnTurretEvent OnAmmoEmpty;

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnPlayerReload OnPlayerReload;

	UPROPERTY(EditAnywhere, Category="Turret|Spline")
	float currentStateSpline = 0.5f;

protected:


#pragma region Upgrades
	UGameInstanceCrazyFoodTruck* GI;
	UFoodTruckDataSubSystem* TruckSubSystem;

	int32 _CurrentAmmoMax;
	//float _CurrentTurretRotationSpeed;

	float TimeDamageWhenFullReload = 6.f;
	float TimerFullReload = 0.f;
	bool IsAmmoFullReload;
	bool IsDamageAlreadyIncrease = false;

	float _CurrentBulletFireRate;
	int _CurrentBulletDamage;
	
#pragma endregion

	UPROPERTY(EditAnywhere, Category="OTHER")
	USceneComponent* _JointCursor;

	UPROPERTY(EditAnywhere, Category="OTHER")
	USceneComponent* _JointCanonTurret;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputMappingContext* TurretMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* ShootAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* YawAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* RollAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* ChangeBulletAction;

	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* QuitTurret;
	
	UPROPERTY(EditDefaultsOnly, Category="Component Mesh")
	USceneComponent* _CanonToRotate;
	
	virtual void BeginPlay() override;

public:
	
	UFUNCTION(BlueprintImplementableEvent, Category="Turret|Events")
	void SetCursorLocation(FVector NewLocation);

	void IncrementPlayerReloading();
	void DecrementPlayerReloading();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<APlayerController> PlayerController;
	
	bool MaxTurnReached();
	bool MinTurnReached();

private:

	float BulletHapticForce;

	int _ActualPlayerReloading = 0;

	EbulletType _actualBulletType;
	FBulletStructure* ActualBulletStructure;

	FBulletStructure BulletChooseForShoot;

	int32 _CurrentAmmo = 0;
	UPROPERTY(EditAnywhere, Category="Turret Parameters")
	int32 _AmmoMax = 25;
	
	UPROPERTY(EditAnywhere, Category="Turret Parameters")
	float TurretRotationSpeed = 10.f;

	float _CurrentCoolDown = 0.f;

	UPROPERTY(EditAnywhere, Category="Turret Parameters")
	float _CursorSpeed = 300.f;
	
	float AreaRangeSide;
	float AreaRangeDepht;
	
	void Shoot();
	UFUNCTION(BlueprintCallable)
	void SetBulletSpawnTransform(USceneComponent* Scp);
	USceneComponent* _SpawnBulletTransform;
	
	void ResetCoolDown();
	void ResetAmmo();
	void SwitchBulletType(EbulletType NewType);
	FString GetRowNameFromBulletType(EbulletType Type);

	int BulletDamage;
	float BulletSpeed;
	float BulletFireRate;
	
	int GetBulletDamage() { return _CurrentBulletDamage ;}
	float GetBulletSpeed() { return BulletSpeed ;}
	float GetBulletFireRate() { return _CurrentBulletFireRate ;}
	
	UFUNCTION(BlueprintCallable)
	float GetBulletHapticForce(){ return BulletHapticForce ;}

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// INPUT

	void InputShootTriggered(const FInputActionValue& Value);
	void InputYaw(const FInputActionValue& Value);
	void InputRoll(const FInputActionValue& Value);
	void InputChangeBulletType(const FInputActionValue& Value);
	void InputQuitTurret(const FInputActionValue& Value);
	void Print(FString Message);

	void UpdateTurretCanonRotation();


	UFUNCTION(BlueprintCallable)
	void SetCursorJoint(USceneComponent* NewJoint);

	int32 mappingPriority = 0;

	void UpdateTurretOnSpline(float alpha);
	
	UPROPERTY(EditAnywhere, Category="TurretParameters")
	ASplineTurret* TurretSpline;

	UPROPERTY(EditAnywhere, Category="TurretParameters")
	ACisorTurret* CisorTurretActor;
};


