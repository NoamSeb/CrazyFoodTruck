// Fill out your copyright notice in the Description page of Project Settings.


#include "AttachPoint.h"



AAttachPoint::AAttachPoint()
{
	PrimaryActorTick.bCanEverTick = false;
	auto Box = CreateDefaultSubobject<UBoxComponent>(TEXT("NavigationVisualBox"));
	SetRootComponent(Box);
	Box->ShapeColor = FColor::Red;
	Box->SetLineThickness(3.f);
}

void AAttachPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAttachPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

