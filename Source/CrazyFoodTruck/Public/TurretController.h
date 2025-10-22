// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BulletBase.h"
#include "EBulletType.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FBulletStructure.h"
#include "GameFramework/Actor.h"
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

	UPROPERTY(editanywhere, Category="Data")
	UDataTable* BulletDataTable;

	
	
	int GetAmmo() const { return _CurrentAmmo;}
	int GetAmmoMax() const { return _AmmoMax;}
	void SetCurrentAmmo(int32 NewAmmo);
	void Reload();
	void DecrementAmmo();
	bool HasAmmo() const { return _CurrentAmmo > 0; }

	UFUNCTION(BlueprintCallable)
	float GetCoolDownBetweenShoot();

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnShootSignature OnAmmoChanged;
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnTurretEvent OnShoot;
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnTurretEvent OnReload;
	
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
	int32 _AmmoMax = 10;

	float _CurrentCoolDown = 0.f;

	UPROPERTY(EditAnywhere, Category="Turret Parameters")
	float _CursorSpeed = 5.f;
	
	UPROPERTY(EditAnywhere, Category="Turret Parameters")
	float AreaRangeSide;
	
	UPROPERTY(EditAnywhere, Category="Turret Parameters")
	float AreaRangeDepht;
	
	void Shoot();
	UFUNCTION(BlueprintCallable)
	void SetBulletSpawnTransform(USceneComponent* Scp);
	USceneComponent* _SpawnBulletTransform;
	
	void ResetCoolDown();
	void ResetAmmo();
	void AddInputMapping();
	void RemoveInputMapping();
	void StartPossessTurret();
	void SwitchBulletType(EbulletType NewType);
	FString GetRowNameFromBulletType(EbulletType Type);



	int BulletDamage;
	float BulletSpeed;
	float BulletFireRate;
	
	int GetBulletDamage() { return BulletDamage ;}
	float GetBulletSpeed() { return BulletSpeed ;}
	float GetBulletFireRate() { return BulletFireRate ;}
	// VIRTUAL

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// INPUT

	void InputShootTriggered(const FInputActionValue& Value);
	void InputYaw(const FInputActionValue& Value);
	void InputRoll(const FInputActionValue& Value);
	void InputChangeBulletType(const FInputActionValue& Value);
	void UpdateTurretCanonRotation();

	int32 mappingPriority = 0;

	void CanonKnockBackAnim();

};

