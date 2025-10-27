// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/IBoxInteract.h"
#include "UInteractBoxWidget.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API UUInteractBoxWidget : public UUserWidget
{
	GENERATED_BODY()

public :
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="InteractBox")
	void ShowInteract();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="InteractBox")
	void HideInteract();
	
};
