// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RepairProgressBillboard.generated.h"

class URepairProgressWidget;

class UWidgetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRepaired);

UCLASS()
class CRAZYFOODTRUCK_API ARepairProgressBillboard : public AActor
{
	GENERATED_BODY()

public:
	ARepairProgressBillboard();

	UFUNCTION(BlueprintCallable, Category = "Repair")
	bool IsDamaged() const;

	UFUNCTION(BlueprintCallable, Category = "Repair")
	void SetDamaged(bool bDamaged);

	UFUNCTION(BlueprintCallable, Category = "Repair")
	void HandleRepairInput();

	UPROPERTY(BlueprintAssignable, Category = "Repair|Events")
	FOnRepaired OnRepaired;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Repair", meta = (ClampMin = "0.01", ClampMax = "1.0"))
	float RepairFillPerPress = 0.10f;

	UPROPERTY(VisibleAnywhere, Category = "Repair|State")
	float RepairProgress = 0.f;

	UPROPERTY(EditAnywhere, Category = "Repair|Visual")
	float BillboardOffsetZ = 150.f;

	UPROPERTY(VisibleAnywhere, Category = "Repair|Visual")
	TObjectPtr<UWidgetComponent> WidgetComponent = nullptr;

	void ApplyProgressStep(float Step);
	void FinishRepair();
	void UpdateWidget();
};
