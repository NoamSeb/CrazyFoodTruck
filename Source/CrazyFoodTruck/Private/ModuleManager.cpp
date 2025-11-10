// Fill out your copyright notice in the Description page of Project Settings.


#include "ModuleManager.h"

#include "EModuleSide.h"


// Sets default values for this component's properties
UModuleManager::UModuleManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UModuleManager::AddModule(FString ModuleID, EModuleSide ModuleSide)
{
	auto CurrentModule = GetModule(ModuleID);
	if (CurrentModule)
	{
		// SPAWN MODULE
		FVector SpawnLocation = FVector::ZeroVector;
		switch (ModuleSide)
		{
			case EModuleSide::Left:
				if (LeftPosition)
				{
					SpawnLocation = *LeftPosition;
				}
				break;
			case EModuleSide::Right:
				if (RightPosition)
				{
					SpawnLocation = *RightPosition;
				}
			break;
			default:
				break;
		}
		
		AModuleBase* ModuleInstance = GetWorld()->SpawnActor<AModuleBase>(CurrentModule, SpawnLocation, FRotator::ZeroRotator);
		AllModules.Add(ModuleInstance);
	}
}

TSubclassOf<AModuleBase> UModuleManager::GetModule(FString ModuleID)
{

	FString FullPath = FString::Printf(TEXT("/Game/Resources/Module/%s.%s_C"), ModuleID, ModuleID);
	
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

void UModuleManager::SetPosition(FVector LeftPos, FVector RightPos)
{
	LeftPosition = &LeftPos;
	RightPosition = &RightPos;
}


// Called every frame
void UModuleManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

