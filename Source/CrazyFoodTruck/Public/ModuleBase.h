// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lever.h"
#include "UWModule.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "Interactable/Interactable.h"
#include "ModuleBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnModuleEvent);


UCLASS()
class CRAZYFOODTRUCK_API AModuleBase : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	AModuleBase();
	void ResetModule();

	void SetLever(ALever* NewLever) { LinkedLever = NewLever; }
	ALever* GetLever() const { return LinkedLever; }

	// BLUEPRINT EVENTS
	UPROPERTY(BlueprintAssignable, Category="Module|Events")
	FOnModuleEvent OnModuleUsed;

	UPROPERTY(BlueprintAssignable, Category="Module|Events")
	FOnModuleEvent OnModuleReady;


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
	void TurnWidgetTowardCamera();

private:
	
	ALever* LinkedLever;
	
};
