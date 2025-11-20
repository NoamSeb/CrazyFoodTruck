// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradePhase/AmeliorationWidget.h"

#include "UpgradePhase/StructUpgrade.h"

int32 UAmeliorationWidget::GetWinnerIndex(const TArray<int32>& ListVote)
{
	int nbrVote = 0;
	int32 indexVoteFinal = 0;
	
	for (int i = 0; i < ListVote.Num(); ++i)
	{
		if (ListVote[i] > nbrVote)
		{
			nbrVote = ListVote[i];
			indexVoteFinal = i;
		} else if (ListVote[i] == nbrVote)
		{
			if (FMath::RandBool())
			{
				nbrVote = ListVote[i];
				indexVoteFinal = i;
			}
		}
	}

	return indexVoteFinal;
}

FStructUpgrade UAmeliorationWidget::CheckUpgradeChoose(const int32& Index, const TArray<FStructUpgrade>& ListUpgrade)
{
	if (Index == 4)
	{
		FStructUpgrade newStruct = FStructUpgrade{};
		newStruct.Zone = EZoneUpgrade::Skip;
		return newStruct;
	}
	return ListUpgrade[Index];
}
