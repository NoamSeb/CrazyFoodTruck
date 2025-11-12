// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IUWModule.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UIUWModule : public UInterface
{
	GENERATED_BODY()
};
class CRAZYFOODTRUCK_API IIUWModule
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateSliderCooldown(float CooldownPercent);
	
public:
};
