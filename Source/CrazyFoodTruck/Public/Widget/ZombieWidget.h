// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZombieWidget.generated.h"

class AZombieIA;
class AHordeManager;

class UTextBlock;

UCLASS()
class CRAZYFOODTRUCK_API UZombieWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetHordeManager(AHordeManager* InManager);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* KillsText = nullptr;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	TWeakObjectPtr<AHordeManager> HordeManager;

	void BindToManager();
	void UnbindFromManager();

	void RefreshKillsText();

	UFUNCTION()
	void HandleAnyZombieDied(AZombieIA* Zombie, AActor* Killer);
};
