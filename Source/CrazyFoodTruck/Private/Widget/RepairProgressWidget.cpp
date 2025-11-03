// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/RepairProgressWidget.h"

#include "Components/ProgressBar.h"

void URepairProgressWidget::SetRepairProgress(float InProgress)
{
	if (!RepairProgressBar)
		return;

	const float ClampedProgress = FMath::Clamp(InProgress, 0.f, 1.f);
	RepairProgressBar->SetPercent(ClampedProgress);

	FLinearColor BarColor;
	if (ClampedProgress < 0.5f)
	{
		const float Alpha = ClampedProgress / 0.5f;
		BarColor = FLinearColor::LerpUsingHSV(FLinearColor::Red, FLinearColor::Yellow, Alpha);
	}
	else
	{
		const float Alpha = (ClampedProgress - 0.5f) / 0.5f;
		BarColor = FLinearColor::LerpUsingHSV(FLinearColor::Yellow, FLinearColor::Green, Alpha);
	}

	RepairProgressBar->SetFillColorAndOpacity(BarColor);
}
