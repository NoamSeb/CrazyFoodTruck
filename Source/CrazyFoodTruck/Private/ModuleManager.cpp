// Fill out your copyright notice in the Description page of Project Settings.


#include "ModuleManager.h"

#include "EModuleSide.h"
#include "FStructModule.h"


UModuleManager::UModuleManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

AModuleBase* UModuleManager::AddModule(FString ModuleID, EModuleSide ModuleSide)
{
	auto CurrentModule = GetModuleByID(ModuleID);
	if (!CurrentModule){return nullptr;}

	// SPAWN MODULE
	FVector SpawnLocation = FVector::ZeroVector;
	FRotator SpawnRotation = FRotator::ZeroRotator;
	switch (ModuleSide)
	{
	case EModuleSide::Left:
		SpawnLocation = LeftPosition->GetComponentLocation();
		SpawnRotation = LeftPosition->GetComponentRotation();
		break;
	case EModuleSide::Right:
		SpawnLocation = RightPosition->GetComponentLocation();
		SpawnRotation = RightPosition->GetComponentRotation();
		break;
	default:
		break;
	}
		
	AModuleBase* ModuleInstance = GetWorld()->SpawnActor<AModuleBase>(CurrentModule, SpawnLocation, SpawnRotation);
	ModuleInstance->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
	AllModules.Add(ModuleInstance);

	return ModuleInstance;
}

TSubclassOf<AModuleBase> UModuleManager::GetModuleByID(FString ModuleID)
{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ModuleID);
	if(!ModuleDataTable)
	{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "NO DATA TABLE");
		return nullptr;
	}
	auto ModuleStruct = ModuleDataTable->FindRow<FStructModule>(FName(*ModuleID), "", true);
	if(ModuleStruct)
	{
		auto CurrentModule = ModuleStruct->ModuleClasse;
		if(CurrentModule)
		{
			return CurrentModule;
		}else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "NO CURRENT MODULE");

		}
	}else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "NO MODULE STRUCT");
	}
	return nullptr;
}


void UModuleManager::BeginPlay()
{
	Super::BeginPlay();
}

void UModuleManager::Initialize(USceneComponent* LeftPos, USceneComponent* RightPos)
{
	LeftPosition = LeftPos;
	RightPosition = RightPos;
}

void UModuleManager::ResetAllModules()
{
	for (auto mod : AllModules)
	{
		mod->ResetModule();
	}
}

