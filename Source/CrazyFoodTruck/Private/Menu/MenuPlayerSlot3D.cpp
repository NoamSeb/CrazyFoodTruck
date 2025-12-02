// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu/MenuPlayerSlot3D.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SkeletalMeshComponent.h"

AMenuPlayerSlot3D::AMenuPlayerSlot3D()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	PedestalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PedestalMesh"));
	PedestalMesh->SetupAttachment(Root);

	PreviewRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PreviewRoot"));
	PreviewRoot->SetupAttachment(Root);
	PreviewRoot->SetRelativeLocation(FVector(0.f, 0.f, 80.f));

	PreviewMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PreviewMesh"));
	PreviewMesh->SetupAttachment(PreviewRoot);
	PreviewMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PreviewMesh->SetRelativeLocation(FVector::ZeroVector);
	PreviewMesh->SetVisibility(false);

	JoinWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("JoinWidget"));
	JoinWidget->SetupAttachment(Root);
	JoinWidget->SetRelativeLocation(FVector(0.f, 0.f, 40.f));
	JoinWidget->SetDrawSize(FVector2D(256.f, 256.f));
	JoinWidget->SetWidgetSpace(EWidgetSpace::World);
}

void AMenuPlayerSlot3D::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GIBase = World->GetGameInstance())
		{
			GIRef = Cast<UGameInstanceCrazyFoodTruck>(GIBase);
		}
	}

	if (GIRef)
	{
		GIRef->OnLobbySlotsChanged.AddDynamic(this, &AMenuPlayerSlot3D::OnLobbySlotsChanged);
	}

	RefreshFromSlotData();
}

void AMenuPlayerSlot3D::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GIRef)
	{
		GIRef->OnLobbySlotsChanged.RemoveDynamic(this, &AMenuPlayerSlot3D::OnLobbySlotsChanged);
	}

	Super::EndPlay(EndPlayReason);
}

void AMenuPlayerSlot3D::OnLobbySlotsChanged()
{
	RefreshFromSlotData();
}

void AMenuPlayerSlot3D::RefreshFromSlotData()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Refresh Slot Index: %d"), SlotIndex));
	if (!GIRef || !GIRef->PlayerSlots.IsValidIndex(SlotIndex))
	{
		return;
	}

	const FMenuPlayerSlot& SlotData = GIRef->PlayerSlots[SlotIndex];

	if (!SlotData.bIsConnected)
	{
		if (PreviewMesh)
		{
			PreviewMesh->SetVisibility(false);
			PreviewMesh->SetSkeletalMesh(nullptr);
		}

		if (JoinWidget)
		{
			JoinWidget->SetVisibility(true);
		}

		if (PedestalMesh)
		{
			if (UMaterialInstanceDynamic* MID = PedestalMesh->CreateAndSetMaterialInstanceDynamic(0))
			{
				MID->SetVectorParameterValue(TEXT("OutlineColor"), FLinearColor(0.2f, 0.2f, 0.2f, 1.f));
			}
		}

		return;
	}

	if (JoinWidget)
	{
		JoinWidget->SetVisibility(false);
	}

	if (PreviewMesh && PlayerPreviewMeshes.IsValidIndex(SlotIndex))
	{
		PreviewMesh->SetSkeletalMesh(PlayerPreviewMeshes[SlotIndex]);
		PreviewMesh->SetVisibility(true);
	}

	const FLinearColor PlayerColor = GIRef->GetPlayerColorForIndex(SlotIndex);

	if (PedestalMesh)
	{
		if (UMaterialInstanceDynamic* MID = PedestalMesh->CreateAndSetMaterialInstanceDynamic(0))
		{
			MID->SetVectorParameterValue(TEXT("OutlineColor"), PlayerColor);
		}
	}
}
