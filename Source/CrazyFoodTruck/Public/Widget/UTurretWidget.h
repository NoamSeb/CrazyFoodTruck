// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/ITurretWidget.h"
#include "UTurretWidget.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API UUTurretWidget : public UUserWidget, public IITurretWidget
{
	GENERATED_BODY()

public :
	virtual void UpdateAmmoText(int32 currentAmmo, int32 maxAmmo) override;

	
};
