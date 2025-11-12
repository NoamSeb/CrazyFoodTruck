// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ForwardCamWidget.generated.h"

class UImage;
class UTexture;

UCLASS()
class CRAZYFOODTRUCK_API UForwardCamWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetForwardTexture(UTexture* InTexture);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ForwardView = nullptr;

	virtual void NativeConstruct() override;
};
