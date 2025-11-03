// Fill out your copyright notice in the Description page of Project Settings.

#include "Damage/RepairProgressBillboard.h"

#include "Widget/RepairProgressWidget.h"

#include "Components/WidgetComponent.h"

ARepairProgressBillboard::ARepairProgressBillboard()
{
	PrimaryActorTick.bCanEverTick = true;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("RepairWidget"));
	SetRootComponent(WidgetComponent);

	WidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	WidgetComponent->SetTwoSided(true);
	WidgetComponent->SetDrawAtDesiredSize(true);
	WidgetComponent->SetPivot(FVector2D(0.5f, 0.0f));
	WidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, BillboardOffsetZ));
	WidgetComponent->SetDrawSize(FVector2D(280.f, 32.f));
	WidgetComponent->SetVisibility(false);
}

void ARepairProgressBillboard::BeginPlay()
{
	Super::BeginPlay();
	UpdateWidget();
}

bool ARepairProgressBillboard::IsDamaged() const
{
	return RepairProgress < 1.f - KINDA_SMALL_NUMBER;
}

void ARepairProgressBillboard::SetDamaged(bool bDamaged)
{
	RepairProgress = bDamaged ? 0.f : 1.f;
	UpdateWidget();
}

void ARepairProgressBillboard::HandleRepairInput()
{
	if (!IsDamaged()) return;
	ApplyProgressStep(RepairFillPerPress);
}

void ARepairProgressBillboard::ApplyProgressStep(float Step)
{
	RepairProgress = FMath::Clamp(RepairProgress + Step, 0.f, 1.f);
	UpdateWidget();

	if (!IsDamaged())
	{
		FinishRepair();
	}
}

void ARepairProgressBillboard::FinishRepair()
{
	RepairProgress = 1.f;
	UpdateWidget();
	OnRepaired.Broadcast();
}

void ARepairProgressBillboard::UpdateWidget()
{
	if (!WidgetComponent)
		return;

	WidgetComponent->SetVisibility(IsDamaged());
	
	if (UUserWidget* UserWidget = WidgetComponent->GetWidget())
	{
		if (URepairProgressWidget* RepairProgressWidget = Cast<URepairProgressWidget>(UserWidget))
		{
			RepairProgressWidget->SetRepairProgress(RepairProgress);
		}
	}
}
