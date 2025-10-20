// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BulletController.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ITurretWidget.h"
#include "UTurretWidget.h"
#include "Components/WidgetComponent.h"
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
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABulletController> BulletPrefab;
	
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

	UPROPERTY(EditDefaultsOnly, Category="Component Mesh")
	USceneComponent* _CanonToRotate;
	
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:

	int32 _CurrentAmmo = 0;
	UPROPERTY(EditAnywhere, Category="Turret Parameters")
	int32 _AmmoMax = 10;

	UPROPERTY(EditAnywhere, Category="Turret Parameters")
	float _BaseCoolDownShoot = 0.f;

	UPROPERTY(EditAnywhere, Category="Turret Parameters")
	float _CurrentCoolDown = 0.f;

	UPROPERTY(EditAnywhere, Category="Turret Parameters")
	float _CursorSpeed = 5.f;
	
	UPROPERTY(EditAnywhere, Category="Turret Parameters")
	float AreaRangeSide = 200.f;
	
	UPROPERTY(EditAnywhere, Category="Turret Parameters")
	float AreaRangeDepht = 200.f;

	void Shoot();
	UFUNCTION(BlueprintCallable)
	void SetBulletSpawnTransform(USceneComponent* Scp);
	USceneComponent* _SpawnBulletTransform;
	
	void ResetCoolDown();
	void ResetAmmo();
	void AddInputMapping();
	void RemoveInputMapping();
	void StartPossessTurret();

	// VIRTUAL

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// INPUT

	void InputShootTriggered(const FInputActionValue& Value);
	void InputYaw(const FInputActionValue& Value);
	void InputRoll(const FInputActionValue& Value);
	void UpdateTurretCanonRotation();

	int32 mappingPriority = 0;

	void CanonKnockBackAnim();

};
