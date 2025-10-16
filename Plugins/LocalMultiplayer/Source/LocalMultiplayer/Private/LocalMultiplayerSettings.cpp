// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSettings.h"

#include "EnhancedActionKeyMapping.h"
#include "InputCoreTypes.h"
#include "InputMappingContext.h"

UInputMappingContext* FLocalMultiplayerProfileData::GetIMCFromType(ELocalMultiplayerInputMappingType MappingType) const
{
	switch (MappingType)
	{
	case ELocalMultiplayerInputMappingType::Menu:
		return IMCMenu;
	case ELocalMultiplayerInputMappingType::InGame:
		return IMCInGame;
	default:
		return nullptr;
	}
}

bool FLocalMultiplayerProfileData::ContainsKey(const FKey& Key, ELocalMultiplayerInputMappingType MappingType) const
{
	const UInputMappingContext* IMC = GetIMCFromType(MappingType);
	if (!IMC)
	{
		return false;
	}

	const TArray<FEnhancedActionKeyMapping>& Mappings = IMC->GetMappings();
	for (const FEnhancedActionKeyMapping& Mapping : Mappings)
	{
		if (Mapping.Key == Key)
		{
			return true;
		}
	}
	
	return false;
}

int ULocalMultiplayerSettings::GetNbKeyboardProfiles() const
{
	return KeyboardProfilesData.Num();
}

int ULocalMultiplayerSettings::FindKeyboardProfileIndexFromKey(const FKey& Key, ELocalMultiplayerInputMappingType MappingType) const
{
	for (int KeyboardProfileIndex = 0; KeyboardProfileIndex < KeyboardProfilesData.Num(); ++KeyboardProfileIndex)
	{
		if (KeyboardProfilesData[KeyboardProfileIndex].ContainsKey(Key, MappingType))
		{
			return KeyboardProfileIndex;
		}
	}
	
	return -1;
}

