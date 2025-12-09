// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructLevels.generated.h"


USTRUCT(BlueprintType)
struct FStructLevels  : public FTableRowBase
{
	GENERATED_BODY()

	FStructLevels()
		: Level()
		, NameLevel()
		, DescriptionLevel()
		, kilometreLevel()
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	TObjectPtr<UDataTable> Level;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	FString NameLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	FString DescriptionLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrade")
	FString kilometreLevel;
};