// Fill out your copyright notice in the Description page of Project Settings.

#include "Damage/RepairProgressBillboard.h"

#include "Widget/RepairProgressWidget.h"

#include "Components/WidgetComponent.h"

URepairProgressBillboard::URepairProgressBillboard()
{
}

void URepairProgressBillboard::InitializeWidget(UWidgetComponent* InWidgetComponent)
{
	WidgetComponent = InWidgetComponent;
	if (UUserWidget* UserWidget = WidgetComponent->GetWidget())
	{
		if (URepairProgressWidget* RepairProgressWidget = Cast<URepairProgressWidget>(UserWidget))
		{
			RepairWidget = RepairProgressWidget;
		}
	}
}

void URepairProgressBillboard::BeginPlay()
{
	Super::BeginPlay();
	UpdateWidget();

	UWorld* World = GetWorld();
	if (UGameInstance* GIBase = World->GetGameInstance())
	{
		GI = Cast<UGameInstanceCrazyFoodTruck>(GIBase);
	}
	TruckSubSystem = GI->GetSubsystem<UFoodTruckDataSubSystem>();
}

bool URepairProgressBillboard::IsDamaged() const
{
	return RepairProgress < 1.f - KINDA_SMALL_NUMBER;
}

void URepairProgressBillboard::SetDamaged(bool bDamaged)
{
	RepairProgress = bDamaged ? 0.f : 1.f;
	UpdateWidget();
}

void URepairProgressBillboard::HandleRepairInput()
{
	if (!IsDamaged()) return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Repairing..."));
	ApplyProgressStep(RepairFillPerPress);
}

void URepairProgressBillboard::ApplyProgressStep(float Step)
{
	RepairProgress = FMath::Clamp(RepairProgress + Step, 0.f, 1.f) + TruckSubSystem->RepairSpeed;
	UpdateWidget();

	if (!IsDamaged())
	{
		FinishRepair();
	}
}

void URepairProgressBillboard::FinishRepair()
{
	RepairProgress = 1.f;
	UpdateWidget();
	OnRepaired.Broadcast();
}

void URepairProgressBillboard::UpdateWidget()
{
	if (!WidgetComponent)
		return;

	WidgetComponent->SetVisibility(IsDamaged());
	if (!RepairWidget)
		return;
	RepairWidget->SetRepairProgress(RepairProgress);
}
