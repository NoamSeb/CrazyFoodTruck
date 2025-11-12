// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UWModule.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "Interactable/Interactable.h"
#include "ModuleBase.generated.h"

UCLASS()
class CRAZYFOODTRUCK_API AModuleBase : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	AModuleBase();
	void ResetModule();

protected:
	
	virtual void BeginPlay() override;
	virtual void Interact(APlayerController* InstigatorPlayerController, ACrazyFoodTruckCharacter* CrazyCharacter) override;
	bool CanInteractWithModule() const;

	UUWModule* WidgetModuleClass;
	
	UPROPERTY(EditAnywhere, Category="Module|Components")
	UWidgetComponent* ModuleWidgetComponent;
	UPROPERTY(EditAnywhere, Category="Module|Cooldown")
	float BaseCooldown;
	
	float ActualCooldown;
	
public:
	virtual void Tick(float DeltaTime) override;
};
