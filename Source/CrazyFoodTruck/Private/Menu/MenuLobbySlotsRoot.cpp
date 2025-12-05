// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu/MenuLobbySlotsRoot.h"
#include "Menu/MenuPlayerSlot3D.h"

#include "Components/SceneComponent.h"
#include "Components/ChildActorComponent.h"

AMenuLobbySlotsRoot::AMenuLobbySlotsRoot()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Slot0Comp = CreateDefaultSubobject<UChildActorComponent>(TEXT("Slot0"));
	Slot0Comp->SetupAttachment(Root);

	Slot1Comp = CreateDefaultSubobject<UChildActorComponent>(TEXT("Slot1"));
	Slot1Comp->SetupAttachment(Root);

	Slot2Comp = CreateDefaultSubobject<UChildActorComponent>(TEXT("Slot2"));
	Slot2Comp->SetupAttachment(Root);
}

void AMenuLobbySlotsRoot::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	const float StartOffset = -1.5f * SlotSpacing;

	SetupSlot(Slot0Comp, 0, StartOffset + 0 * SlotSpacing);
	SetupSlot(Slot1Comp, 1, StartOffset + 1 * SlotSpacing);
	SetupSlot(Slot2Comp, 2, StartOffset + 2 * SlotSpacing);
}

void AMenuLobbySlotsRoot::SetupSlot(UChildActorComponent* SlotComp, int32 SlotIndex, float OffsetX)
{
	if (!SlotComp)
	{
		return;
	}

	if (SlotClass)
	{
		SlotComp->SetChildActorClass(SlotClass);
	}

	SlotComp->SetRelativeLocation(FVector(OffsetX, 0.f, 0.f));
	SlotComp->SetRelativeRotation(FRotator::ZeroRotator);

	if (AActor* Child = SlotComp->GetChildActor())
	{
		if (AMenuPlayerSlot3D* SlotActor = Cast<AMenuPlayerSlot3D>(Child))
		{
			SlotActor->SetSlotIndex(SlotIndex);
		}
	}
}
