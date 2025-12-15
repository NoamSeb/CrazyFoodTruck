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
	void Initialize(USceneComponent* LeftPos, USceneComponent* RightPos, ALever* LeftLev, ALever* RightLev);

	UFUNCTION(BlueprintCallable, Category="Module")
	void ResetAllModules();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Module")
	UDataTable* ModuleDataTable;

private:
	UPROPERTY()
	USceneComponent* LeftPosition;
	UPROPERTY()
	USceneComponent*  RightPosition;

	UPROPERTY()
	ALever* LeftLever;

	UPROPERTY()
	ALever* RightLever;

	UPROPERTY()
	AInteractBox* LeftInteractBox;
	UPROPERTY()
	AInteractBox* RightInteractBox;
	UPROPERTY()
	TArray<AModuleBase*> AllModules;
	
	TSubclassOf<AModuleBase> GetModuleByID(FString ModuleID);
};
