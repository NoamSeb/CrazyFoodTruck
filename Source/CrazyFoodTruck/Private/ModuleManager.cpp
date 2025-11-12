// Fill out your copyright notice in the Description page of Project Settings.


#include "ModuleManager.h"

#include "EModuleSide.h"


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
	switch (ModuleSide)
	{
	case EModuleSide::Left:
		SpawnLocation = LeftPosition;
		break;
	case EModuleSide::Right:
		SpawnLocation = RightPosition;
		break;
	default:
		break;
	}
		
	AModuleBase* ModuleInstance = GetWorld()->SpawnActor<AModuleBase>(CurrentModule, SpawnLocation, FRotator::ZeroRotator);
	ModuleInstance->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform);
	AllModules.Add(ModuleInstance);

	return ModuleInstance;
}

TSubclassOf<AModuleBase> UModuleManager::GetModuleByID(FString ModuleID)
{

	FString FullPath = FString::Printf(TEXT("/Game/Resources/Module/%s.%s_C"), *ModuleID, *ModuleID);
	
	UClass* LoadedClass = LoadClass<AModuleBase>(nullptr, *FullPath);
	if (LoadedClass)
	{
		return LoadedClass;
	}

	return nullptr;
}


void UModuleManager::BeginPlay()
{
	Super::BeginPlay();
}

void UModuleManager::Initialize(FVector LeftPos, FVector RightPos)
{
	LeftPosition = LeftPos;
	GEngine->AddOnScreenDebugMessage(-1,7.f, FColor::Black, TEXT("Left Position Set" + LeftPosition.ToString()));
	RightPosition = RightPos;
}

void UModuleManager::ResetAllModules()
{
	for (auto mod : AllModules)
	{
		mod->ResetModule();
	}
}

