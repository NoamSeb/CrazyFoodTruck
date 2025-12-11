// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrazyFoodTruck/Data/Public/GameInstanceCrazyFoodTruck.h"
#include "MenuPlayerSlot3D.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class UWidgetComponent;
class USkeletalMeshComponent;
class USkeletalMesh;

USTRUCT(BlueprintType)
struct FPreviewMeshStructure
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> PreviewMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMaterial> PreviewMaterial;
	
};
UCLASS()
class CRAZYFOODTRUCK_API AMenuPlayerSlot3D : public AActor
{
	GENERATED_BODY()

public:
	AMenuPlayerSlot3D();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void SetSlotIndex(int32 InIndex) { SlotIndex = InIndex; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lobby")
	int32 SlotIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lobby|Preview")
	TArray<FPreviewMeshStructure> PlayerPreviewMeshes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> PedestalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> PreviewRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> PreviewMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWidgetComponent> JoinWidget;

private:
	UPROPERTY()
	TObjectPtr<UGameInstanceCrazyFoodTruck> GIRef;

	UFUNCTION()
	void OnLobbySlotsChanged();

	void RefreshFromSlotData();
};
