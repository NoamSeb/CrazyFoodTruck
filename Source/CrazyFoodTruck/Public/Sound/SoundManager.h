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
	ECC_Shoot UMETA(Displayname = "Shoot"),
	ECC_Accident UMETA(Displayname = "Accident"),
	ECC_HitZombie UMETA(Displayname = "HitZombie"),
	ECC_ZombieDeath UMETA(Displayname = "ZombieDeath"),
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

	FName GetRowNameFromEnum(ESfxType SfxType);
};
