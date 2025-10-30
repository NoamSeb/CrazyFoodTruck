// Fill out your copyright notice in the Description page of Project Settings.


#include "FZoneSpawn.h"

FZoneSpawn::FZoneSpawn(EPositionSpawn PosSpawn, AAreaZombieSpawn* NewArea)
{
	PositionSpawn = PosSpawn;
	AreaZombieSpawn = NewArea;
}

FZoneSpawn::FZoneSpawn(AAreaZombieSpawn* NewArea)
{
	PositionSpawn = EPositionSpawn::TopRight;
	AreaZombieSpawn = NewArea;
}
