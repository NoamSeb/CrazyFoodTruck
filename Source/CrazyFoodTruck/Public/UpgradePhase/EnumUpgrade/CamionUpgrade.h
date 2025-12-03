// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECamionUpgrade : uint8
{
	None = 0,
	Speed,
	TurningSpeed,
	AddBoostTime,
	Heal
};
