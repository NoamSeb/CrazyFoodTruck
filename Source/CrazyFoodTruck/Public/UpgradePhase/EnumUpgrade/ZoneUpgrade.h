// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EZoneUpgrade : uint8
{
	Camion = 0,
	Turret,
	Bullet,
	Module,
	Jeu,
	Skip
};
