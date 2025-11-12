// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/ForwardCamWidget.h"

#include "Components/Image.h"

#include "Engine/Texture.h"

void UForwardCamWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UForwardCamWidget::SetForwardTexture(UTexture* InTexture)
{
	if (!ForwardView || !InTexture) return;

	FSlateBrush Brush;
	Brush.SetResourceObject(InTexture);
	Brush.ImageSize = FVector2D(512.f, 256.f);
	ForwardView->SetBrush(Brush);
}
