// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EModuleSide.h"
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

	UFUNCTION(BlueprintCallable, Category="Module")
	AModuleBase* AddModule(FString ModuleID, EModuleSide ModuleSide);
	
	UFUNCTION(BlueprintCallable, Category="Module")
	void Initialize(FVector LeftPos, FVector RightPos);

	UFUNCTION(BlueprintCallable, Category="Module")
	void ResetAllModules();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	FVector LeftPosition;
	UPROPERTY()
	FVector RightPosition;

	UPROPERTY()
	AInteractBox* LeftInteractBox;
	UPROPERTY()
	AInteractBox* RightInteractBox;
	UPROPERTY()
	TArray<AModuleBase*> AllModules;
	
	TSubclassOf<AModuleBase> GetModuleByID(FString ModuleID);
};
