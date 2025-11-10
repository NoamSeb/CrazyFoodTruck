// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModuleBase.h"
#include "Components/ActorComponent.h"
#include "Interactable/InteractBox.h"
#include "ModuleManager.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRAZYFOODTRUCK_API UModuleManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UModuleManager();

	void AddModule(FString ModuleID, EModuleSide ModuleSide);
	TSubclassOf<AModuleBase> GetModule(FString ModuleID);
	TArray<AModuleBase*> AllModules;
	void SetPosition();


protected:
	virtual void BeginPlay() override;
	void SetPosition(FVector LeftPos, FVector RightPos);

private:
	FVector* LeftPosition;
	FVector* RightPosition;

	AInteractBox* LeftInteractBox;
	AInteractBox* RightInteractBox;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
