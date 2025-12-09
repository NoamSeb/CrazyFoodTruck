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
			UGameplayStatics::PlaySound2D(WorldContext, Row->MetaSound);
			return;
		}
	}
}