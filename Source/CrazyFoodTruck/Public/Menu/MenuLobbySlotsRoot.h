// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MenuLobbySlotsRoot.generated.h"

class USceneComponent;
class UChildActorComponent;
class AMenuPlayerSlot3D;

UCLASS()
class CRAZYFOODTRUCK_API AMenuLobbySlotsRoot : public AActor
{
	GENERATED_BODY()

public:
	AMenuLobbySlotsRoot();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UChildActorComponent> Slot0Comp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UChildActorComponent> Slot1Comp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UChildActorComponent> Slot2Comp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UChildActorComponent> Slot3Comp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lobby")
	TSubclassOf<AMenuPlayerSlot3D> SlotClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lobby")
	float SlotSpacing = 300.f;

private:
	void SetupSlot(UChildActorComponent* SlotComp, int32 SlotIndex, float OffsetX);
};
