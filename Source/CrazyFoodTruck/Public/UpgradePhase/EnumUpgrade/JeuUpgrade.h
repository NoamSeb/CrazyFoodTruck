// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EJeuUpgrade : uint8
{
	None = 0,
	RandomUpgrades, //fait
	//MoreTickets, //fait - pas avec le grade
	BetterHeal //fait
};