// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EInteractionType : uint8
{
	Possess = 0,
	Interactable,
	Both,
};
