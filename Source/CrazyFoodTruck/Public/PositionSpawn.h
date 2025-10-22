// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPositionSpawn : uint8
{
	Right = 0,
	Left,
	Forward,
	Backward,
};