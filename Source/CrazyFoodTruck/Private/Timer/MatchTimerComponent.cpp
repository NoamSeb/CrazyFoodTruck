// Fill out your copyright notice in the Description page of Project Settings.

#include "Timer/MatchTimerComponent.h"

#include "TimerManager.h"

UMatchTimerComponent::UMatchTimerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMatchTimerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bAutoStartOnBeginPlay)
	{
		StartTimer();
	}
}

void UMatchTimerComponent::StartTimer(float FirstDelay)
{
	if (TimerHandle.IsValid())
	{
		return;
	}

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&UMatchTimerComponent::HandleSecondTick,
			1.0f,
			true,
			FMath::Max(0.f, FirstDelay)
		);
	}
}

void UMatchTimerComponent::StopTimer()
{
	if (!TimerHandle.IsValid())
	{
		return;
	}

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}

	TimerHandle.Invalidate();
}

void UMatchTimerComponent::ResetTimer()
{
	ElapsedSeconds = 0;
	OnTimerSecond.Broadcast(ElapsedSeconds);
}

void UMatchTimerComponent::HandleSecondTick()
{
	++ElapsedSeconds;
	OnTimerSecond.Broadcast(ElapsedSeconds);
}
