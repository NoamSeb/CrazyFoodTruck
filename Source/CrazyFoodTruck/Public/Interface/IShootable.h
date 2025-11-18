// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IShootable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UIShootable : public UInterface
{
	GENERATED_BODY()
};

class CRAZYFOODTRUCK_API IIShootable
{
	GENERATED_BODY()

public:
	
	virtual void ReceiveDamage(int DamageAmount);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Shootable")
	void ReceiveDamageBlueprint(int DamageAmount);
	
};
