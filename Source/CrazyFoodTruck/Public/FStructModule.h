// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModuleBase.h"
#include "FStructModule.generated.h"


USTRUCT(BlueprintType)
struct FStructModule : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AModuleBase> ModuleClasse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ID;
};