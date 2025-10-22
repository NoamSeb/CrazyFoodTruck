// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IEntity.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnDamage, int, damageAmount);

UINTERFACE()
class UIEntity : public UInterface
{
	GENERATED_BODY()
};

class CRAZYFOODTRUCK_API IIEntity
{
	GENERATED_BODY()

public:

	// DAMAGE
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Entity")
	void ReceiveDamageBlueprint(int DamageAmount);

	virtual void ReceiveDamage(int DamageAmount) = 0;

	// ROAD
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Entity")
	void CrushUnderTruckBlueprint(int DamageAmount);
	
	virtual void CrushUnderTruck() = 0;
};
