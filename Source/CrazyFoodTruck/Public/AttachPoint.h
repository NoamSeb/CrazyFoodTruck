// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "AttachPoint.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API AAttachPoint : public AActor
{
	GENERATED_BODY()

public:
	AAttachPoint();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
