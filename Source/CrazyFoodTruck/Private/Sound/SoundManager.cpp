// Fill out your copyright notice in the Description page of Project Settings.


#include "Sound/SoundManager.h"

#include "Kismet/GameplayStatics.h"

USoundManager::USoundManager()
{
}

void USoundManager::PlaySFX(ESfxType SfxType, UObject* WorldContext)
{
	UDataTable* Table = MetaSoundsTable.LoadSynchronous();
	if (!Table) return;

	static const FString Context = TEXT("PlaySFX");
	
	TArray<FST_Sound*> AllRows;
	Table->GetAllRows(Context, AllRows);

	for (FST_Sound* Row : AllRows)
	{
		if (Row && Row->SoundType == SfxType && Row->MetaSound)
		{
			UWorld* World = GEngine->GetWorldFromContextObjectChecked(this);
			UGameplayStatics::PlaySound2D(World, Row->MetaSound);
			return;
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("PlaySFX: aucun son trouvé pour %d"), (int)SfxType);
}

FName USoundManager::GetRowNameFromEnum(ESfxType SfxType)
{
	switch(SfxType)
	{
	case ESfxType::ECC_Shoot:      return FName("Shoot");
	case ESfxType::ECC_Accident:   return FName("Accident");
	default:                       return NAME_None;
	}
}
