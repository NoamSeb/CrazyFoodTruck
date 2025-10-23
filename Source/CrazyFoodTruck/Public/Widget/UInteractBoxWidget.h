// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UInteractBoxWidget.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API UUInteractBoxWidget : public UUserWidget
{
	GENERATED_BODY()

public :
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowInteract();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HideInteract();
	
};
