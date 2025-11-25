// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETurretUpgrade : uint8
{
	None = 0,
	RotationSpeedCabestan,
	AugmentationAngle,
	Cadence,
	Damage,
	TripleDamageFor10EmeBullet,
	MaxAmmo,
	SpeedBullet
};