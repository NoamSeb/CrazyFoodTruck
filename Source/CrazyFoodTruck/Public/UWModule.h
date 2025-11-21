// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/IUWModule.h"
#include "UWModule.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateSlider,float, CooldownPercent);

UCLASS()
class CRAZYFOODTRUCK_API UUWModule : public UUserWidget, public IIUWModule
{
	GENERATED_BODY()

public:
private:

};

