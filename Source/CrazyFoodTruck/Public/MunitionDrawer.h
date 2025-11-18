// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurretController.h"
#include "GameFramework/Actor.h"
#include "MunitionDrawer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoUpdate, float, CurrentOpenValue);
UCLASS()
class CRAZYFOODTRUCK_API AMunitionDrawer : public AActor
{
	GENERATED_BODY()

public:
	AMunitionDrawer();
	
	void UpdateMuntionCount(int change);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void TurretShoot(int AmmoLeft, int AmmoMax);

	UFUNCTION()
	void UpdateValueWithTurret(int AmmoLeft, int AmmoMax);

public:

	void IncrementPlayerReloading();
    void DecrementPlayerReloading();
	
	void ReceiveInputOpen(float value);
	void UpdateValue(float valueChange, bool giveAmmo);
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(BlueprintAssignable)
	FOnAmmoUpdate OnAmmoUpdate;

	UPROPERTY(EditAnywhere, Category="MunitionDrawer")
	ATurretController* LinkedTurretController;

private:

	int munitionMax = 0;
	int currentMunition = 0;
	float _ActualOpenValue = 0.f;

	// UFUNCTION()
	// void UpdateTurretInfo(int AmmoLeft, int AmmoMax);

	UPROPERTY(EditAnywhere, Category="MunitionDrawer")
	float _OpenSpeed = 5.f;
};
