// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ITurretWidget.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UITurretWidget : public UInterface
{
	GENERATED_BODY()
};

class CRAZYFOODTRUCK_API IITurretWidget
{
	GENERATED_BODY()

public:
	 virtual void UpdateAmmoText(int32 currentAmmo, int32 maxAmmo) = 0;
};
