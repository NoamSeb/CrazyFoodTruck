// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPhaseGameCrazyFoodTruckState : uint8
{
	Menu = 0,
	ChoosePlayers,
	Route,
	Amelioration,
	Loose
};