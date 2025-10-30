// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPlayer.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UIPlayer : public UInterface
{
	GENERATED_BODY()
};

class CRAZYFOODTRUCK_API IIPlayer
{
	GENERATED_BODY()

	virtual void SetAmmoState(bool bHasAmmo) = 0;
	
	virtual bool CanInteract() = 0;
	virtual bool HasAmmo() = 0;
	virtual void SetInteractState(bool state)= 0;
	
public:
	
};
