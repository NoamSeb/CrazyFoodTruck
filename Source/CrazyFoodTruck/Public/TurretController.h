// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bullet/BulletBase.h"
#include "Bullet/EBulletType.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Bullet/FBulletStructure.h"
#include "CrazyFoodTruck/GameInstanceCrazyFoodTruck.h"
#include "GameFramework/Actor.h"
#include "Interactable/Interactable.h"
#include "Interactable/InteractBox.h"
#include "TurretController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShootSignature, int32, AmmoLeft, int32, AmmoMax); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTurretEvent);

UCLASS()
class CRAZYFOODTRUCK_API ATurretController : public APawn
{
	GENERATED_BODY()

public:
	
	ATurretController();
	
	TSubclassOf<ABulletBase> ActualBulletPrefab;

	UPROPERTY(EditAnywhere, Category="Data")
	UDataTable* BulletDataTable;

	UPROPERTY(EditAnywhere, Category="Variable")
	AInteractBox* InteractBox;

	APlayerController* ActualPlayerController;
	APawn* ActualPawn;
	
	int GetAmmo() const { return _CurrentAmmo;}
	int GetAmmoMax() const { return _AmmoMax;}
	void SetCurrentAmmo(int32 NewAmmo);
	void Reload();
	void DecrementAmmo();
	bool HasAmmo() const { return _CurrentAmmo > 0; }


	UFUNCTION(BlueprintCallable)
	float GetCoolDownBetweenShoot();

	void AddRotationInput(float value);

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnShootSignature OnAmmoChanged;
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnTurretEvent OnShoot;
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnTurretEvent OnReload;
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnTurretEvent OnAmmoEmpty;
protected:

	UPROPERTY(EditAnywhere, Category="OTHER")
	USceneComponent* _CursorJoint;

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
	
	virtual void Tick(float DeltaTime) override;

private:

	EbulletType _actualBulletType;
	FBulletStructure* ActualBulletStructure;

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
	
	int GetBulletDamage() { return BulletDamage ;}
	float GetBulletSpeed() { return BulletSpeed ;}
	float GetBulletFireRate() { return BulletFireRate ;}

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// INPUT

	void InputShootTriggered(const FInputActionValue& Value);
	void InputYaw(const FInputActionValue& Value);
	void InputRoll(const FInputActionValue& Value);
	void InputChangeBulletType(const FInputActionValue& Value);
	void InputQuitTurret(const FInputActionValue& Value);
	void TestingFunction(const FInputActionValue& Value);
	void Print(FString Message);

	void UpdateTurretCanonRotation();

	int32 mappingPriority = 0;

};


