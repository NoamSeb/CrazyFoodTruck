// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MetasoundSource.h"
#include "SoundManager.generated.h"

class UMetaSoundSource;
class UAudioComponent;

struct FKey;

UENUM(BlueprintType)
enum class ESfxType : uint8
{
	// Weapon Related
	ECC_ClassicShoot UMETA(Displayname = "ClassicShoot"), // Done
	ECC_ExplosivShoot UMETA(Displayname = "ExplosivShoot"),
	ECC_GunSocket UMETA(Displayname = "GunSocket"), // Done
	ECC_Reload UMETA(Displayname = "Reload"), // Done
	ECC_RotateTurret UMETA(Displayname = "RotateTurret"),
	// Truck Related
	ECC_Accident UMETA(Displayname = "Accident"), // Done
	// Zombie Related
	ECC_HitZombie UMETA(Displayname = "HitZombie"), // Done
	ECC_ZombieDeath UMETA(Displayname = "ZombieDeath"), // Done
	// Obstacle Related
	ECC_RockDestruction UMETA(Displayname = "RockDestruction"), // Done
	ECC_WoodDestruction UMETA(Displayname = "WoodDestruction"), // Done
	ECC_MetalDestruction UMETA(Displayname = "MetalDestruction"), // Donne
	// UI Related
	ECC_PlayerVote UMETA(Displayname = "PlayerVote"),
	ECC_UpdateModule UMETA(Displayname = "UpdateModule"), // Done
};

USTRUCT(BlueprintType)
struct FST_Sound : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMetaSoundSource* MetaSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESfxType SoundType;
};

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Sound Manager Settings"))
class CRAZYFOODTRUCK_API USoundManager : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:

	USoundManager();
	
	UFUNCTION(BlueprintPure, Category="Sound Manager")
	static USoundManager* Get()
	{
		return const_cast<USoundManager*>(GetDefault<USoundManager>());
	}
	
	UPROPERTY(Config, EditAnywhere, Category="Sound Manager", meta=(DisplayName="MetaSounds Table"))
	TSoftObjectPtr<UDataTable> MetaSoundsTable;
	
	UFUNCTION(BlueprintCallable, Category="Sound Manager")
	void PlaySFX(ESfxType SfxType, UObject* WorldContext);
	
	UFUNCTION(BlueprintCallable, Category="Sound Manager")
	void PlaySFXWithIndex(ESfxType SfxType, UObject* WorldContext, int index);
	
};
