#pragma once
 
#include "CoreMinimal.h"
#include "ERecipe.generated.h"
 

UENUM(BlueprintType)
enum class ERecipe : uint8
{
	Steak,
	Burger,
	Unfinished
};